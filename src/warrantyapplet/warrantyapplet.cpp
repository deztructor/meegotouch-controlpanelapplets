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
#include "warrantyapplet.h"
#include "warrantywidget.h"
#include "warrantybrief.h"

#include <MAction>

#ifndef UNIT_TEST
#include <MLibrary>
M_LIBRARY
#endif

#undef DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(warrantyapplet, WarrantyApplet)

#define LOAD_THEME_FILE
#ifdef LOAD_THEME_FILE
#include <MTheme>
const QString cssDir = 
    "/usr/share/themes/base/meegotouch/libwarrantyapplet/style/";
#endif


WarrantyApplet::WarrantyApplet()
{
}

WarrantyApplet::~WarrantyApplet() 
{
    if (!m_WarrantyBusinessLogic.isNull ())
        delete m_WarrantyBusinessLogic.data ();
}

void 
WarrantyApplet::init()
{
    #ifdef LOAD_THEME_FILE
    QString themeFile = cssDir + "libwarrantyapplet.css";
    MTheme::loadCSS (themeFile);
    #endif
}

DcpWidget *
WarrantyApplet::pageMain(
        int widgetId)
{
    if (m_WarrantyBusinessLogic.isNull ())
        m_WarrantyBusinessLogic = new WarrantyBusinessLogic;

    SYS_DEBUG ("widgetId = %d", widgetId);

    switch (widgetId) {
        case 0:
            if (m_MainWidget == 0) 
                m_MainWidget = new WarrantyWidget (m_WarrantyBusinessLogic);
            return m_MainWidget;

        default:
            SYS_WARNING ("Unknown widgetId: %d", widgetId);
    }

    return 0;
}

DcpWidget *
WarrantyApplet::constructWidget (
        int widgetId)
{
    SYS_DEBUG ("-----------------------------------");
    SYS_DEBUG ("*** widgetId = %d", widgetId);
    return pageMain (widgetId);
}

QString
WarrantyApplet::title() const
{
    //% "Warranty"
    return qtTrId ("qtn_warr_title");
}

QVector<MAction*>
WarrantyApplet::viewMenuItems()
{
    QVector<MAction*>   vector;

    return vector;
}

DcpBrief *
WarrantyApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new WarrantyBrief;
}

