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
#include "aboutapplet.h"
#include "aboutwidget.h"
#include "aboutbrief.h"

#include <MAction>

#ifndef UNIT_TEST
#include <MLibrary>
M_LIBRARY
#endif

#undef DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(aboutapplet, AboutApplet)

#define LOAD_THEME_FILE

#ifdef LOAD_THEME_FILE
#include <MTheme>
const QString cssDir = 
    "/usr/share/themes/base/meegotouch/libaboutapplet/style/";
#endif

AboutApplet::AboutApplet() : m_AboutBusinessLogic (0)
{
}

AboutApplet::~AboutApplet() 
{
}

void 
AboutApplet::init()
{
    #ifdef LOAD_THEME_FILE
    QString themeFile = cssDir + "libaboutapplet.css";
    MTheme::loadCSS (themeFile);
    #endif
}

DcpWidget *
AboutApplet::pageMain(
        int widgetId)
{
    if (m_AboutBusinessLogic.isNull ())
        m_AboutBusinessLogic = new AboutBusinessLogic;

    SYS_DEBUG ("widgetId = %d", widgetId);

    switch (widgetId) {
        case 0:
            if (m_MainWidget == 0) 
                m_MainWidget = new AboutWidget (m_AboutBusinessLogic);
            return m_MainWidget;

        default:
            SYS_WARNING ("Unknown widgetId: %d", widgetId);
    }

    return 0;
}

DcpWidget *
AboutApplet::constructWidget (
        int widgetId)
{
    SYS_DEBUG ("-----------------------------------");
    SYS_DEBUG ("*** widgetId = %d", widgetId);
    return pageMain (widgetId);
}

QString
AboutApplet::title() const
{
    //% "About product"
    return qtTrId ("qtn_prod_about_product");
}

QVector<MAction*>
AboutApplet::viewMenuItems()
{
    QVector<MAction*>   vector;

    return vector;
}

DcpBrief *
AboutApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new AboutBrief ();
}

