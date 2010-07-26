/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperbrief.h"
#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"

#include <dcpwidgettypes.h>

//#define DEBUG
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

