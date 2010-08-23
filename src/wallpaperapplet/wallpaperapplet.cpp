/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "wallpaperapplet.h"
#include "wallpaperwidget.h"
#include "wallpapereditorwidget.h"
#include "wallpaperbrief.h"

#include <MTheme>
#include <MAction>
#include <QDBusInterface>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(wallpaperapplet, WallpaperApplet)

const QString cssDir = "/usr/share/themes/base/meegotouch/duicontrolpanel/style/";

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
    QString themeFile = cssDir + "wallpaper.css";
   
    SYS_DEBUG ("Adding pixmap dir %s", SYS_STR(cssDir));
    MTheme::addPixmapDirectory (cssDir, M::Recursive);
    SYS_DEBUG ("Loading theme %s", SYS_STR(themeFile));
    MTheme::loadCSS (themeFile);
}

DcpWidget *
WallpaperApplet::pageMain(
        int widgetId)
{
    SYS_DEBUG ("widgetId = %d", widgetId);
    switch (widgetId) {
        case MainWidget:
            if (m_MainWidget == 0) 
                m_MainWidget = new WallpaperWidget (m_WallpaperBusinessLogic);
            return m_MainWidget;

        case EditorWidget:
            if (m_EditorWidget == 0)
                m_EditorWidget = new WallpaperEditorWidget (
                        m_WallpaperBusinessLogic);
            return m_EditorWidget;

        default:
            SYS_WARNING ("Unknown widgetId: %d", widgetId);
    }

    return 0;
}

DcpWidget *
WallpaperApplet::constructWidget (
        int widgetId)
{
    return pageMain (widgetId);
}

QString
WallpaperApplet::title() const
{
    //% "Wallpaper"
    return qtTrId ("qtn_wall_wallpaper");
}

QVector<MAction*>
WallpaperApplet::viewMenuItems()
{
    MAction            *helpAction;
    QVector<MAction*>   vector;

    SYS_DEBUG ("");
    helpAction = new MAction (
            //% "User guide"
            qtTrId ("qtn_comm_userguide"), 
            pageMain (0));
    helpAction->setLocation (MAction::ApplicationMenuLocation);
    
    vector.append(helpAction);

    connect (helpAction, SIGNAL (triggered (bool)),
             this, SLOT (userGuide ()));

    return vector;
}

DcpBrief *
WallpaperApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);

    return new WallpaperBrief (m_WallpaperBusinessLogic);
}

void
WallpaperApplet::userGuide ()
{
    SYS_DEBUG ("");

    QDBusInterface userguide ("com.nokia.userguide", "/",
                              "com.nokia.UserGuideIf");
    userguide.call ("pageByPath", "fullguide-1-2-list-1.cfg");
}

