#ifndef __CUSTOM_KIS_TOOL_PROXY_H
#define __CUSTOM_KIS_TOOL_PROXY_H

#include <kritaui_export.h>
#include <KoToolProxy.h>
#include <kis_tool.h>
#include "kis_tool_proxy.h"


class KRITAUI_EXPORT CustomKisToolProxy : public KisToolProxy
{
    Q_OBJECT

public:
    CustomKisToolProxy(KoCanvasBase *canvas, QObject *parent = 0);

protected:
    void forwardToTool(ActionState state, KisTool::ToolAction action, QEvent *event, const QPointF &docPoint) override;
};

#endif /* __CUSTOM_KIS_TOOL_PROXY_H */
