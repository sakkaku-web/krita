#include "CustomApp.h"
#include "KisPart.h"
#include "KisDocument.h"
#include "KisView.h"
#include "KisMainWindow.h"
#include "KisRecentFilesManager.h"
#include "KoColorModelStandardIds.h"
#include "kis_node.h"
#include "kis_layer.h"
#include "kis_group_layer.h"
#include "kis_config.h"

Q_GLOBAL_STATIC(CustomApp, s_instance)

CustomApp* CustomApp::instance() {
    return s_instance;
}

CustomApp::CustomApp() {
}

CustomApp::~CustomApp() {
}

void CustomApp::loadInitialDocument() {
    QList<QPointer<KisDocument>> openDocs = KisPart::instance()->documents();
    if (openDocs.empty()) {
        qDebug() << "Starting custom code";

        QList<QUrl> files = KisRecentFilesManager::instance()->recentUrlsLatestFirst();
        if (files.empty()) {
            qDebug() << "Creating new blank document";

            // Document doc = Krita::instance()->createDocument(1000, 1000, "Background", "RGBA", "U8", "", 1.0);

            KisDocument *doc = KisPart::instance()->createDocument();
            doc->setObjectName("Background");

            KisPart::instance()->addDocument(doc, false);

            const KoColorSpace *cs = KoColorSpaceRegistry::instance()->colorSpace(RGBAColorModelID.id(), Integer8BitsColorDepthID.id(), "");
            Q_ASSERT(cs);

            QColor qc(Qt::gray);
            KoColor bgColor(qc, cs);

            if (!doc->newImage("Background", 1000, 1000, cs, bgColor, KisConfig::RASTER_LAYER, 1, "", double(1.0 / 72) )) {
                qDebug() << "Failed to create default background for document";
                return;
            }

            Q_ASSERT(doc->image());
            qDebug() << "Successfully created new document with image. " << KisPart::instance()->documentCount();

            KisGroupLayerSP root = doc->image()->rootLayer();
            int i = 0;
            for (i = 0; i < root->childCount(); i++) {
                root->at(i)->setUserLocked(true);
            }
            // Q_FOREACH(const KisNode node, ) {
            // }
            // qDebug() << "Locking existing nodes";

            KisMainWindow *mainWin = KisPart::instance()->currentMainwindow();
            mainWin->showDocument(doc);
            qDebug() << "Open document in current main window";
        } else {
            QUrl first = files.first();
            qDebug() << "Loading recent file: " << first;
        }
    }
}