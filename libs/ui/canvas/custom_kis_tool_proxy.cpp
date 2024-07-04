/*
 *  SPDX-FileCopyrightText: 2011 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "input/kis_tablet_debugger.h"
#include "kis_canvas2.h"
#include "kis_tool_proxy.h"
#include "custom_kis_tool_proxy.h"

#include <KoToolProxy_p.h>

CustomKisToolProxy::CustomKisToolProxy(KoCanvasBase *canvas, QObject *parent)
    : KisToolProxy(canvas, parent)
{
}

void CustomKisToolProxy::forwardToTool(ActionState state,
                                       KisTool::ToolAction action,
                                       QEvent *event,
                                       const QPointF &docPoint)
{
    qDebug() << "Foward Tool" << priv()->activeTool;
    KisToolProxy::forwardToTool(state, action, event, docPoint);
}