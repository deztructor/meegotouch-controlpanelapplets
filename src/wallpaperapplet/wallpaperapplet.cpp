/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "wallpaperapplet.h"
#include "wallpaperwidget.h"
#include "wallpaperbrief.h"

#include <DuiTheme>
#include <DuiAction>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(wallpaperapplet, WallpaperApplet)

const QString cssDir = "/usr/share/themes/base/dui/duicontrolpanel/style/";

WallpaperApplet::WallpaperApplet() :
    m_WallpaperBusinessLogic (new WallpaperBusinessLogic)
{
}

WallpaperApplet::~WallpaperApplet() 
{
}

void 
WallpaperApplet::init()
{
    DuiTheme::loadCSS(cssDir + "wallpaperapplet.css");
}

DcpWidget *
WallpaperApplet::pageMain()
{
    static int i = 0;

    SYS_DEBUG ("Entering %d", i);
    if (m_MainWidget == NULL) {
        m_MainWidget = new WallpaperWidget (m_WallpaperBusinessLogic);
        SYS_DEBUG ("Widget created");
    }

    SYS_DEBUG ("returning m_MainWidget: %d", i);
    ++i;
    return m_MainWidget;
}

DcpWidget *
WallpaperApplet::constructWidget (
        int widgetId)
{
    static int i = 0;
    Q_UNUSED(widgetId);

    SYS_DEBUG ("We are called: %d", i);
    ++i;
    return pageMain ();
}

QString
WallpaperApplet::title() const
{
    //% "Wallpapers"
    return qtTrId ("qtn_theme_wallpaper"); // This is not official logical id
}

QVector<DuiAction*>
WallpaperApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction (qtTrId ("qtn_comm_help"), 
            pageMain ());

    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);

    return vector;
}

DcpBrief *
WallpaperApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new WallpaperBrief (m_WallpaperBusinessLogic);
}
