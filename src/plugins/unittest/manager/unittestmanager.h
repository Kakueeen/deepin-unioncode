// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UNITTESTMANAGER_H
#define UNITTESTMANAGER_H

#include <QObject>

class UnitTestManagerPrivate;
class UnitTestManager : public QObject
{
    Q_OBJECT
public:
    UnitTestManager(QObject *parent = nullptr);
    ~UnitTestManager();

    void addPrompt(const QString &title, const QString &prompt);
    QMap<QString, QString> allPrompts() const;
    void setCurrentPrompt(const QString &title);
    QString currentPrompt() const;
    void removePrompt(const QString &title);
    void savePrompt();

    void setUTFilePath(const QString &path);
    QString utFilePath() const;
    void setUTFileNameFormat(const QString &format);
    QString utFileNameFormat() const;
    QStringList formatHistory() const;
    void addUTTemplate(const QString &temp);
    void removeUTTemplate(const QString &temp);
    QStringList allTemplates() const;
    void setCurrentTemplate(const QString &temp);
    QString currentTemplate() const;

    void generateUTFiles();
    void runTest();
    void generateCoverageReport();
    
private:
    UnitTestManagerPrivate *const d;
};

#endif   // UNITTESTMANAGER_H
