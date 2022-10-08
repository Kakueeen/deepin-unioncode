/*
 * Copyright (C) 2020 ~ 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     huanyu<huanyub@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             huangyu<huangyub@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.h"
#include "client.h"
#include "jsontabwidget.h"
#include "perfflamegraphscripts.h"
#include "perfrecorddisplay.h"
#include "jsontabwidget.h"
#include "common/common.h"

#include <QTime>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QToolButton>
#include <QLineEdit>

class MainWindowPrivate
{
    friend class MainWindow;
    JsonTabWidget *jsonTabWidget{nullptr};
    QToolBar *toolbar{nullptr};
    //    QAction *attachAction{nullptr};
    //    QMenu *attachMenu{nullptr};
    //    QAction *fromPidAction{nullptr};
    //    QMenu *fromPidMenu{nullptr};
    //    QAction *fromProgramAction{nullptr};
    //    QMenu *fromProgramMenu{nullptr};
    QLineEdit *editPid{nullptr};
    QToolButton *ctrlButton{nullptr};
    QDockWidget *perfRecordDock{nullptr};
    PerfRecordDisplay *perfRecordDisplay{nullptr};
    FlameGraphGenTask *flameGraphGenTask{nullptr};

    Client *client{nullptr};
    QThread *cliThread{nullptr};
    QProcess *server{nullptr};
    QTimer *timer{nullptr};
    QString attach{QMenu::tr("Attach")};
    QString strat{QToolBar::tr("Start")};
    QString stop{QToolBar::tr("Stop")};
    QString fromPid{QAction::tr("Pid")};
    QString fromProgram{QAction::tr("Program")};
    std::string host{"127.0.0.1"};
    int port{3309};
};

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , d (new MainWindowPrivate)
{
    d->toolbar = new QToolBar;
    //    d->attachAction = new QAction(d->attach);
    //    d->attachMenu = new QMenu();
    //    d->fromPidAction = new QAction(d->fromPid, d->attachAction);
    //    d->fromPidMenu = new QMenu();
    //    d->fromProgramAction = new QAction(d->fromProgram, d->attachAction);
    //    d->fromProgramMenu = new QMenu();
    d->editPid = new QLineEdit();
    d->ctrlButton = new QToolButton;
    d->perfRecordDock = new QDockWidget;
    d->perfRecordDisplay = new PerfRecordDisplay;
    d->jsonTabWidget = new JsonTabWidget;

    d->ctrlButton->setChecked(true);
    d->ctrlButton->setCheckable(true);
    d->ctrlButton->setText(d->strat);
    QObject::connect(d->ctrlButton, &QToolButton::toggled, [=](bool checked){
        if (checked) {
            QString pid = d->editPid->text();
            if (pid.isEmpty()) {
                ContextDialog::okCancel(QMessageBox::tr("attach processId can't empty!"));
                return;
            }

            QByteArray queryPidLines;
            ProcessUtil::execute("ps", {"--pid", pid},
                                 [&queryPidLines](const QByteArray &data) {
                queryPidLines = data;
            });
            if (queryPidLines.isEmpty() || queryPidLines.count('\n') < 2) {
                ContextDialog::okCancel(QMessageBox::tr("attach processId no exites!"));
                return;
            }

            d->ctrlButton->setText(d->stop);
            start(d->editPid->text().toUInt());
        } else {
            d->ctrlButton->setText(d->strat);
            stop();
        }
    });

    //    d->attachAction->setMenu(d->attachMenu);
    //    d->attachMenu->addAction(d->fromPidAction);
    //    d->attachMenu->addAction(d->fromProgramAction);
    //    d->toolbar->addAction(d->attachAction);
    d->toolbar->addSeparator();
    d->toolbar->addWidget(d->editPid);
    d->toolbar->addWidget(d->ctrlButton);
    addToolBar(d->toolbar);

    d->perfRecordDock->setWidget(d->perfRecordDisplay);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, d->perfRecordDock);

    setCentralWidget(d->jsonTabWidget);

    //    start(getpid());
}

MainWindow::~MainWindow()
{
    stop();

    if (d) {
        delete d;
    }
}

void MainWindow::start(uint pid)
{
    if (!d->server) {
        QString toolsPath = CustomPaths::global(CustomPaths::Tools);
        QString adapter = toolsPath + QDir::separator() + "performanceadapter";
        d->server = new QProcess();

        QObject::connect(d->server, &QProcess::errorOccurred, [=](QProcess::ProcessError error){
            qCritical() << "server >> " << error << d->server->errorString();
        });
        QObject::connect(d->server, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                         [](int exit, QProcess::ExitStatus status) {
            qCritical() << "server >> "
                        << "exit: " << exit
                        << "status: " << status;
        });
#ifdef QT_DEBUG
        QObject::connect(d->server, &QProcess::readyReadStandardError, [=](){
            qInfo() << "server stderr >> " << d->server->readAllStandardError();
        });
        QObject::connect(d->server, &QProcess::readyReadStandardOutput, [=](){
            qInfo() << "server stdout >> " << d->server->readAllStandardOutput();
        });
#endif
        qInfo() << adapter << "--port" << d->port;
        d->server->start(adapter, {"--port", QString::number(d->port)});
        d->server->waitForReadyRead();
    }

    if (!d->client) {
        d->client = new Client(d->host, d->port);
        auto cliThread = new QThread();
        d->client->moveToThread(cliThread);
        QObject::connect(d->client, &Client::pullDataResult,
                         d->jsonTabWidget, &JsonTabWidget::parseJson,
                         Qt::UniqueConnection);
        cliThread->start();
    }

    QObject::metaObject()->invokeMethod(d->client, "initialzation", Q_ARG(int, pid));

    d->timer = new QTimer(this);
    QObject::connect(d->timer, &QTimer::timeout, [=](){
        if (d->client) {
            QObject::metaObject()->invokeMethod(d->client, "pullData");
        } else {
            d->timer->stop();
        }
    });
    d->timer->start(1000);


    if (!d->flameGraphGenTask) {
        d->flameGraphGenTask = new FlameGraphGenTask;
        QObject::connect(d->perfRecordDisplay, &PerfRecordDisplay::showWebBrowserGP,
                         d->flameGraphGenTask, &FlameGraphGenTask::showWebBrowser,
                         Qt::UniqueConnection);

    }
    d->flameGraphGenTask->start(pid);
}

void MainWindow::stop()
{
    if (d->flameGraphGenTask) {
        d->flameGraphGenTask->stop();
    }

    if (d->timer) {
        d->timer->stop();
        delete d->timer;
        d->timer = nullptr;
    }

    if (d->cliThread) {
        d->cliThread->exit(0);
        delete d->cliThread;
        d->cliThread = nullptr;
    }

    if (d->client) {
        d->client->shutdown();
        d->client->exit();
        delete d->client;
        d->client = nullptr;
    }

    if (d->server) {
        d->server->kill();
        delete d->server;
        d->server = nullptr;
    }
}