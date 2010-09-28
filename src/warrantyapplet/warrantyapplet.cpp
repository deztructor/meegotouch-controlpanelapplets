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

#include <MTheme>
#include <MAction>

#undef DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(warrantyapplet, WarrantyApplet)

WarrantyApplet::WarrantyApplet() :
    m_WarrantyBusinessLogic (new WarrantyBusinessLogic)
{
}

WarrantyApplet::~WarrantyApplet() 
{
}

void 
WarrantyApplet::init()
{
}

DcpWidget *
WarrantyApplet::pageMain(
        int widgetId)
{
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
    return new WarrantyBrief (m_WarrantyBusinessLogic);
}

