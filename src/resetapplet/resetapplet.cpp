/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "resetapplet.h"
#include "resetwidget.h"
#include "resetbrief.h"

#include <MTheme>
#include <MAction>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(resetapplet, ResetApplet)

ResetApplet::ResetApplet() :
    m_ResetBusinessLogic (new ResetBusinessLogic)
{
}

ResetApplet::~ResetApplet() 
{
    delete m_ResetBusinessLogic;
}

void 
ResetApplet::init()
{
}

DcpWidget *
ResetApplet::pageMain(
        int widgetId)
{
    SYS_DEBUG ("widgetId = %d", widgetId);
    switch (widgetId) {
        case 0:
            if (m_MainWidget == 0) 
                m_MainWidget = new ResetWidget (m_ResetBusinessLogic);
            return m_MainWidget;

        default:
            SYS_WARNING ("Unknown widgetId: %d", widgetId);
    }

    return 0;
}

DcpWidget *
ResetApplet::constructWidget (
        int widgetId)
{
    SYS_DEBUG ("-----------------------------------");
    SYS_DEBUG ("*** widgetId = %d", widgetId);
    return pageMain (widgetId);
}

QString
ResetApplet::title() const
{
    //% "Reset settings"
    return qtTrId ("qtn_rset_reset_settings");
}

QVector<MAction*>
ResetApplet::viewMenuItems()
{
    MAction            *helpAction;
    QVector<MAction*>   vector;

    SYS_DEBUG ("");
    helpAction = new MAction (
            //% "Help"
            qtTrId ("qtn_comm_help"), 
            pageMain (0));
    helpAction->setLocation (MAction::ApplicationMenuLocation);
    vector.append(helpAction);

    return vector;
}

DcpBrief *
ResetApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new ResetBrief (m_ResetBusinessLogic);
}
