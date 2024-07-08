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
#include "kis_image_manager.h"

#include <KoToolProxy_p.h>

const int margin = 10;
const int canvasMargin = 50;

CustomKisToolProxy::CustomKisToolProxy(KoCanvasBase *canvas, QObject *parent)
    : KisToolProxy(canvas, parent)
{
}

KisNodeSP CustomKisToolProxy::nodeAtPoint(const QPointF &docPoint) const
{
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

        bounds.adjust(-margin, -margin, margin, margin);
        qDebug() << "Node bounds" << bounds << " and point " << pixelPoint;
        if (bounds.contains(pixelPoint.toPoint())) {
            qDebug() << "Node found" << node->name();
            selectedNode = node;
            break;
        }
    }

    return selectedNode;
}

void CustomKisToolProxy::setCurrentNode(KisNodeSP node)
{
    KoCanvasResourceProvider *res = canvas()->resourceManager();
    QVariant v;
    v.setValue(KisNodeWSP(node));
    res->setResource(KoCanvasResource::CurrentKritaNode, v);
}

KisNodeSP CustomKisToolProxy::currentNode() const
{
    KoCanvasResourceProvider *res = canvas()->resourceManager();
    QVariant v = res->resource(KoCanvasResource::CurrentKritaNode);
    return v.value<KisNodeWSP>();
}

void CustomKisToolProxy::forwardToTool(ActionState state,
                                       KisTool::ToolAction action,
                                       QEvent *event,
                                       const QPointF &docPoint)
{
    KisMainWindow *win = KisPart::instance()->currentMainwindow();

    if (state == KisToolProxy::ActionState::BEGIN) {
        // KisTool *tool = dynamic_cast<KisTool *>(priv()->activeTool);
        KisNodeManager *nodeManager = win->viewManager()->nodeManager();

        KisNodeSP selectedNode = nodeAtPoint(docPoint);
        if (selectedNode == nullptr) {
            qDebug() << "Creating new layer";
            selectedNode = nodeManager->createNode("KisPaintLayer");
        }

        // We need this to be updated earlier, so the user can smoothly continue drawing on new layer
        setCurrentNode(selectedNode);
        // TODO: new layer is not actually selected

    } else if (state == KisToolProxy::ActionState::END) {
        // Increase canvas to fit all content
        KisNodeSP selectedNode = currentNode();
        QPoint pos = QPoint(selectedNode->x(), selectedNode->y());
        QRect bounds = selectedNode->extent();

        KisImageWSP image = win->viewManager()->image();

        QPoint offset = QPoint();
        QPoint addSize = QPoint();

        if (bounds.x() < 0) {
            int add = -bounds.x() + canvasMargin;
            addSize += QPoint(add, 0);
            offset += QPoint(add, 0);
        }
        if (bounds.y() < 0) {
            int add = -bounds.y();
            addSize += QPoint(0, add);
            offset += QPoint(0, add);
        }

        int width_end = bounds.x() + bounds.width();
        if (width_end > image->width()) {
            int diff = width_end - image->width();
            addSize += QPoint(diff + canvasMargin, 0);
        }

        int height_end = bounds.y() + bounds.height();
        if (height_end > image->height()) {
            int diff = height_end - image->height();
            addSize += QPoint(0, diff + canvasMargin);
        }

        if (addSize.x() > 0 || addSize.y() > 0) {
            win->viewManager()->imageManager()->resizeCurrentImage(image->width() + addSize.x(),
                                                                   image->height() + addSize.y(),
                                                                   offset.x(), offset.y());

            qDebug() << "Resizing image to fit the bounds by " << addSize << " with offset " << offset << " -> " << image->bounds();
        }
    }

    KisToolProxy::forwardToTool(state, action, event, docPoint);
}