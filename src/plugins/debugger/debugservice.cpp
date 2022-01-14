/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     luzhen<luzhen@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             luzhen<luzhen@uniontech.com>
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
#include "debugservice.h"
#include "debugmodel.h"
#include "debugsession.h"

DebugService::DebugService(QObject *parent)
    : QObject(parent)
{
    model.reset(new DebugModel(this));
}

void DebugService::sendAllBreakpoints(DebugSession *session)
{
    sendBreakpoints(undefined, session, false);
    sendFunctionBreakpoints(session);
    sendDataBreakpoints(session);
    sendInstructionBreakpoints(session);
    // send exception breakpoints at the end since some debug adapters rely on the order
    sendExceptionBreakpoints(session);
}

dap::array<IBreakpoint> DebugService::addBreakpoints(
        QUrl uri, dap::array<IBreakpointData> rawBreakpoints, dap::optional<DebugSession *> session)
{
    auto breakpoints = model->addBreakpoints(uri, rawBreakpoints);
    if (session)
        sendBreakpoints(uri, session.value());

    return breakpoints;
}

void DebugService::sendBreakpoints(dap::optional<QUrl> uri, DebugSession *session, bool sourceModified)
{
    Q_UNUSED(sourceModified)
    auto breakpointsToSend = model->getBreakpoints(uri, undefined, undefined, true);
    session->sendBreakpoints(breakpointsToSend);
}

void DebugService::sendFunctionBreakpoints(DebugSession *session)
{
    Q_UNUSED(session)
    // TODO(mozart)
}

void DebugService::sendDataBreakpoints(DebugSession *session)
{
    Q_UNUSED(session)
    // TODO(mozart)
}

void DebugService::sendInstructionBreakpoints(DebugSession *session)
{
    Q_UNUSED(session)
    // TODO(mozart)
}

void DebugService::sendExceptionBreakpoints(DebugSession *session)
{
    Q_UNUSED(session)
    // TODO(mozart)
}