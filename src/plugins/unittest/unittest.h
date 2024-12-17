// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UNITTEST_H
#define UNITTEST_H

#include <framework/framework.h>

class UnitTest : public dpf::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.deepin.plugin.unioncode" FILE "unittest.json")
public:
    virtual void initialize() override;
    virtual bool start() override;
    virtual dpf::Plugin::ShutdownFlag stop() override;
};

#endif   // UNITTEST_H
