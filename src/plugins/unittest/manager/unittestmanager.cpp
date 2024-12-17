// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unittestmanager.h"

#include "common/settings/settings.h"

constexpr char kGeneralGroup[] { "General" };
constexpr char kActiveGroup[] { "ActiveSettings" };

constexpr char kPrompts[] { "Prompts" };
constexpr char kTitle[] { "title" };
constexpr char kPrompt[] { "prompt" };
constexpr char kTemplates[] { "Templates" };
constexpr char kFormats[] { "Formats" };
constexpr char kActivePrompt[] { "ActivePrompt" };
constexpr char kActiveTemplate[] { "ActiveTemplate" };
constexpr char kActiveFormat[] { "ActiveFormat" };

class UnitTestManagerPrivate
{
public:
    Settings utSetting;

    QMap<QString, QString> promptMap;
};

UnitTestManager::UnitTestManager(QObject *parent)
    : QObject(parent),
      d(new UnitTestManagerPrivate)
{
}

UnitTestManager::~UnitTestManager()
{
    delete d;
}

void UnitTestManager::addPrompt(const QString &title, const QString &prompt)
{
    d->promptMap.insert(title, prompt);
    d->utSetting.setValue(kGeneralGroup, kPrompts, qVariantFromValue(d->promptMap));
}

QMap<QString, QString> UnitTestManager::allPrompts() const
{
    if (!d->promptMap.isEmpty())
        return d->promptMap;

    const auto &map = d->utSetting.value(kGeneralGroup, kPrompts).toMap();
    // std::transform(map.cbegin(), map.cend(), std::inserter(d->promptMap, d->promptMap.end()),
    //                [](const QPair<QString, QVariant> &pair) {
    //                    return qMakePair(pair.first, pair.second.toString());
    //                });
    return d->promptMap;
}

void UnitTestManager::setCurrentPrompt(const QString &title)
{
}

QString UnitTestManager::currentPrompt() const
{
}

void UnitTestManager::removePrompt(const QString &title)
{
}

void UnitTestManager::savePrompt()
{
}

void UnitTestManager::setUTFilePath(const QString &path)
{
}

QString UnitTestManager::utFilePath() const
{
}

void UnitTestManager::setUTFileNameFormat(const QString &format)
{
}

QString UnitTestManager::utFileNameFormat() const
{
}

QStringList UnitTestManager::formatHistory() const
{
}

void UnitTestManager::addUTTemplate(const QString &temp)
{
}

void UnitTestManager::removeUTTemplate(const QString &temp)
{
}

QStringList UnitTestManager::allTemplates() const
{
}

void UnitTestManager::setCurrentTemplate(const QString &temp)
{
}

QString UnitTestManager::currentTemplate() const
{
}

void UnitTestManager::generateUTFiles()
{
}

void UnitTestManager::runTest()
{
}

void UnitTestManager::generateCoverageReport()
{
}
