/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "warrantyapplet.h"
#include "warrantywidget.h"
#include "warrantybrief.h"

#include <MTheme>
#include <MAction>

#define DEBUG
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
    return qtTrId ("qtn_warranty"); // not official
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
