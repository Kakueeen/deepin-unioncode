// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"

#include <DTitlebar>
#include <DIconButton>
#include <DStatusBar>
#include <DDockWidget>
#include <DToolBar>

#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

class MainWindowPrivate
{
    QMap<QString, QDockWidget *> dockList;

    DWidget *topToolbar { nullptr };
    QMap<QString, QWidget *> topToolList;

    QString centralWidgetName;
    DMenu *menu { nullptr };

    friend class MainWindow;
};

Qt::DockWidgetArea MainWindow::positionTodockArea(Position pos)
{
    if(pos == FullWindow || pos == Central)
    {
        qWarning() << "only converting values of Left、Right、Top、Bottom";
        return Qt::NoDockWidgetArea;
    }

    switch (pos) {
    case Left:
        return Qt::LeftDockWidgetArea;
    case Right:
        return Qt::RightDockWidgetArea;
    case Top:
        return Qt::TopDockWidgetArea;
    case Bottom:
        return Qt::BottomDockWidgetArea;
    default:
        return Qt::LeftDockWidgetArea;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent), d(new MainWindowPrivate)
{
    titlebar()->setTitle("Deepin Union Code");
    resize(800, 1000);

    setCorner(Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea);
    setCorner(Qt::Corner::TopLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addWidget(const QString &name, QWidget *widget, Position pos)
{
    if (d->dockList.contains(name)) {
        qWarning() << "dockWidget-" << name << "is already exist";
        return;
    }

    if (pos == FullWindow)
        removeAllDockWidget();

    if (pos == Central || pos == FullWindow) {
        setCentralWidget(widget);
        d->centralWidgetName = name;
        return;
    }

    auto area = positionTodockArea(pos);
    DDockWidget *dock = new DDockWidget(this);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    //todo:可以添加参数，是否显示header
    if (0)
        dock->setWindowTitle(name);
    if (dock->titleBarWidget())
        delete dock->titleBarWidget();
    dock->setTitleBarWidget(new DWidget(this));

    dock->setWidget(widget);
    d->dockList.insert(name, dock);
    addDockWidget(area, dock);
}

void MainWindow::replaceWidget(const QString &name, QWidget *widget, Position pos)
{
    if (d->dockList.contains(name)) {
        qWarning() << "no dockWidget named: " << name;
        return;
    }

    if (pos != FullWindow)
        removeWidget(pos);
    addWidget(name, widget, pos);
}

void MainWindow::removeWidget(Position pos)
{
    if (pos == FullWindow)
        removeAllDockWidget();

    if (pos == Central || pos == FullWindow) {
        auto widget = takeCentralWidget();
        delete widget;
        return;
    }

    auto area = positionTodockArea(pos);
    foreach (auto name, d->dockList.keys()) {
        if (dockWidgetArea(d->dockList[name]) == area) {
            removeDockWidget(d->dockList[name]);
            d->dockList.remove(name);
        }
    }
}

void MainWindow::removeWidget(const QString &name)
{
    if (name == d->centralWidgetName) {
        auto widget = takeCentralWidget();
        delete widget;
        return;
    }
    if (!d->dockList.contains(name)) {
        qWarning() << "no dockWidget named: " << name;
        return;
    }

    removeDockWidget(d->dockList[name]);
    d->dockList.remove(name);
}

void MainWindow::hideWidget(const QString &name)
{
    if (!d->dockList.contains(name)) {
        qWarning() << "no dockWidget named: " << name;
        return;
    }

    d->dockList[name]->setVisible(false);
}

void MainWindow::showWidget(const QString &name)
{
    if (!d->dockList.contains(name)) {
        qWarning() << "no dockWidget named: " << name;
        return;
    }

    d->dockList[name]->setVisible(true);
}

void MainWindow::removeAllDockWidget()
{
    for (int i = 0; i < d->dockList.count(); i++)
        removeDockWidget(d->dockList.values().at(i));

    d->dockList.clear();
}

void MainWindow::splitWidgetOrientation(const QString &first, const QString &second, Qt::Orientation orientation)
{
    if (!d->dockList.contains(first) || !d->dockList.contains(second))
        return;

    splitDockWidget(d->dockList[first], d->dockList[second], orientation);
}

void MainWindow::setToolbar(Qt::ToolBarArea area, QWidget *widget)
{
    DToolBar *tb = new DToolBar(this);
    tb->setContentsMargins(0, 0, 0, 0);
    tb->setMovable(false);
    tb->setFloatable(false);

    tb->setStyleSheet("QToolBar { border: 0; }");

    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tb->addWidget(widget);

    addToolBar(area, tb);
}

void MainWindow::addTopToolBar()
{
    if (d->topToolbar)
        return;

    d->topToolbar = new DWidget(this);
    QHBoxLayout *hl = new QHBoxLayout(d->topToolbar);
    hl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout *titleBarLayout = static_cast<QHBoxLayout *>(titlebar()->layout());
    titleBarLayout->insertWidget(1, d->topToolbar, Qt::AlignLeft);
}

DIconButton *MainWindow::createIconButton(QAction *action)
{
    DIconButton *iconBtn = new DIconButton(this);
    iconBtn->setFocusPolicy(Qt::NoFocus);
    iconBtn->setEnabled(action->isEnabled());
    iconBtn->setIcon(action->icon());
    iconBtn->setFixedSize(QSize(36, 36));
    iconBtn->setIconSize(QSize(15, 15));

    QString toolTipStr = action->text() + " " + action->shortcut().toString();
    iconBtn->setToolTip(toolTipStr);
    iconBtn->setShortcut(action->shortcut());

    connect(iconBtn, &DIconButton::clicked, action, &QAction::triggered);
    connect(action, &QAction::changed, iconBtn, [=] {
        if (action->shortcut() != iconBtn->shortcut()) {
            QString toolTipStr = action->text() + " " + action->shortcut().toString();
            iconBtn->setToolTip(toolTipStr);
            iconBtn->setShortcut(action->shortcut());
        }

        iconBtn->setEnabled(action->isEnabled());
    });

    return iconBtn;
}

void MainWindow::addTopToolItem(const QString &name, QAction *action)
{
    if (!action)
        return;
    if (!d->topToolbar)
        addTopToolBar();
    titlebar()->setTitle(QString(""));

    auto iconBtn = createIconButton(action);
    auto toolBarLayout = qobject_cast<QHBoxLayout *>(d->topToolbar->layout());
    toolBarLayout->addWidget(iconBtn, Qt::AlignLeft);
    d->topToolList.insert(name, iconBtn);
}

void MainWindow::addTopToolBarSpacing(int spacing)
{
    auto toolBarLayout = qobject_cast<QHBoxLayout *>(d->topToolbar->layout());
    toolBarLayout->addSpacing(spacing);
}

void MainWindow::clearTopTollBar()
{
    QHBoxLayout *titleBarLayout = static_cast<QHBoxLayout *>(titlebar()->layout());
    titleBarLayout->removeWidget(d->topToolbar);
    delete d->topToolbar;
    d->topToolbar = nullptr;

    d->topToolList.clear();
    titlebar()->setTitle(QString("Deepin Union Code"));
}

DMenu *MainWindow::getMenu()
{
    if (!d->menu) {
        d->menu = new DMenu(titlebar());
        titlebar()->setMenu(d->menu);
    }

    return d->menu;
}