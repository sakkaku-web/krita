/*
 *  SPDX-FileCopyrightText: 2011 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "custom_kis_tool_proxy.h"
#include "KisMainWindow.h"
#include "KisPart.h"
#include "KisViewManager.h"
#include "input/kis_tablet_debugger.h"
#include "kis_action.h"
#include "kis_action_manager.h"
#include "kis_canvas2.h"
#include "kis_canvas_resource_provider.h"
#include "kis_group_layer.h"
#include "kis_node.h"
#include "kis_node_manager.h"
#include "kis_node_selection_adapter.h"
#include "kis_tool_proxy.h"

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
    qDebug() << "Foward Event " << state << ", Tool: " << priv()->activeTool << ", Event:" << event;
    if (state == KisToolProxy::ActionState::BEGIN) {
        KisImageWSP image = dynamic_cast<KisCanvas2 *>(canvas())->currentImage();
        KisGroupLayerSP root = image->rootLayer();
        QPointF pixelPoint = image->documentToPixel(docPoint);

        KisNodeSP selectedNode = nullptr;
        for (int i = root->childCount()-1; i >= 0; i--) {
            KisNodeSP node = root->at(i);
            if (node->userLocked())
                continue;

            QRect bounds = node->extent();
            if (selectedNode == nullptr && bounds.width() == 0 && bounds.height() == 0) {
                selectedNode = node;
                continue;
            }

            qDebug() << "Node bounds" << bounds << " and point " << pixelPoint;
            if (bounds.contains(pixelPoint.toPoint())) {
                qDebug() << "Node found" << node->name();
                selectedNode = node;
                break;
            }
        }

        KisTool *tool = dynamic_cast<KisTool *>(priv()->activeTool);
        KisMainWindow *win = KisPart::instance()->currentMainwindow();
        KisNodeManager *nodeManager = win->viewManager()->nodeManager();
        if (selectedNode == nullptr) {
            qDebug() << "Creating new layer";
            selectedNode = nodeManager->createNode("KisPaintLayer");
        }

        // We need this to be updated earlier, so the user can smoothly continue drawing on new layer
        KoCanvasResourceProvider *res = canvas()->resourceManager();
        qDebug() << res->resource(KoCanvasResource::CurrentKritaNode).value<KisNodeWSP>();
        QVariant v;
        v.setValue(KisNodeWSP(selectedNode));
        res->setResource(KoCanvasResource::CurrentKritaNode, v);
    }

    KisToolProxy::forwardToTool(state, action, event, docPoint);
}