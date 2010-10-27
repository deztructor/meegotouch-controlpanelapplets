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

#include "wallpaperapplet.h"
#include "wallpaperwidget.h"
#include "wallpapereditorwidget.h"
#include "wallpaperbrief.h"

#include <MTheme>
#include <MAction>
#include <QDBusInterface>

//#define DEBUG
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
    //MAction            *helpAction;
    QVector<MAction*>   vector;
#if 0
    SYS_DEBUG ("");
    helpAction = new MAction (
            //% "User guide"
            qtTrId ("qtn_comm_userguide"), 
            pageMain (0));
    helpAction->setLocation (MAction::ApplicationMenuLocation);
    
    vector.append(helpAction);

    connect (helpAction, SIGNAL (triggered (bool)),
             this, SLOT (userGuide ()));
#endif
    return vector;
}

DcpBrief *
WallpaperApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);

    return new WallpaperBrief (m_WallpaperBusinessLogic);
}

#if 0
void
WallpaperApplet::userGuide ()
{
    SYS_DEBUG ("");

    QDBusInterface userguide ("com.nokia.userguide", "/",
                              "com.nokia.UserGuideIf");
    userguide.call ("pageByPath", "fullguide-1-2-list-1.cfg");
}
#endif
