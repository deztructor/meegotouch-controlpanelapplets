/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "wallpaperbrief.h"
#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"

#include <dcpwidgettypes.h>

#undef DEBUG
#include "../debug.h"

WallpaperBrief::WallpaperBrief (
        WallpaperBusinessLogic *businessLogic) :
    m_WallpaperBusinessLogic (businessLogic)
{
    connect (businessLogic, SIGNAL(wallpaperChanged()),
        this, SIGNAL(valuesChanged()));
}

int
WallpaperBrief::widgetTypeID() const
{
    SYS_DEBUG ("");
    return DcpWidgetType::Image;
}

QString 
WallpaperBrief::valueText() const
{
    WallpaperCurrentDescriptor *desc = WallpaperCurrentDescriptor::instance();

    if (desc->valid()) {
        return desc->title();
    }
     
    //% "None"
    return qtTrId("wallpaper_none");
}

QString 
WallpaperBrief::image () const
{
    WallpaperCurrentDescriptor *desc = WallpaperCurrentDescriptor::instance();
    
    if (desc->valid()) {
        // FIXME: We should have a method to get the thumbnail filename
        return desc->filename();
    }
     
    return "";
}

QString
WallpaperBrief::icon () const
{
    WallpaperCurrentDescriptor *desc = WallpaperCurrentDescriptor::instance();
    
    if (desc->valid()) {
        return desc->imageID ();
    }
     
    return "";
}

