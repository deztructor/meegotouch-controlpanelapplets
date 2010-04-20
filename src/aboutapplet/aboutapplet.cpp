/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "aboutapplet.h"
#include "aboutwidget.h"
#include "aboutbrief.h"

#include <MTheme>
#include <MAction>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(aboutapplet, AboutApplet)

AboutApplet::AboutApplet() :
    m_AboutBusinessLogic (new AboutBusinessLogic)
{
}

AboutApplet::~AboutApplet() 
{
}

void 
AboutApplet::init()
{
}

DcpWidget *
AboutApplet::pageMain(
        int widgetId)
{
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
    return qtTrId ("qtn_about_product"); // not official
}

QVector<MAction*>
AboutApplet::viewMenuItems()
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
AboutApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new AboutBrief (m_AboutBusinessLogic);
}
