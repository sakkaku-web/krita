/*
 * defaultpaintops_plugin.cc -- Part of Krita
 *
 * Copyright (c) 2004 Boudewijn Rempt (boud@valdyas.org)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <klocale.h>
#include <kiconloader.h>
#include <kcomponentdata.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kgenericfactory.h>

#include <kis_paintop_registry.h>

#include "kis_airbrushop.h"
#include "kis_brushop.h"
#include "kis_duplicateop.h"
#include "kis_eraseop.h"
#include "kis_penop.h"
#include "kis_global.h"
#include "kis_paintop_registry.h"

#include "defaultpaintops_plugin.h"

typedef KGenericFactory<DefaultPaintOpsPlugin> DefaultPaintOpsPluginFactory;
K_EXPORT_COMPONENT_FACTORY( kritadefaultpaintops, DefaultPaintOpsPluginFactory( "krita" ) )


DefaultPaintOpsPlugin::DefaultPaintOpsPlugin(QObject *parent, const QStringList &)
    : KParts::Plugin(parent)
{
    setComponentData(DefaultPaintOpsPluginFactory::componentData());

    KisPaintOpRegistry *r = KisPaintOpRegistry::instance();
    r->add (KisPaintOpFactorySP( new KisAirbrushOpFactory ));
    r->add (KisPaintOpFactorySP( new KisBrushOpFactory ));
    r->add (KisPaintOpFactorySP( new KisDuplicateOpFactory ));
    r->add (KisPaintOpFactorySP( new KisEraseOpFactory ));
    r->add (KisPaintOpFactorySP( new KisPenOpFactory ));
}

DefaultPaintOpsPlugin::~DefaultPaintOpsPlugin()
{
}

#include "defaultpaintops_plugin.moc"
