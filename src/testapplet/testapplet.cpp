/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "testapplet.h"
#include "testwidget.h"
#include "testbrief.h"

#include <DuiTheme>
#include <DuiAction>

#define DEBUG
#include "../debug.h"

Q_EXPORT_PLUGIN2(testapplet, TestApplet)


TestApplet::TestApplet() 
{
}

TestApplet::~TestApplet() 
{
}

void 
TestApplet::init()
{
    //DuiTheme::loadCSS(cssDir + "themeapplet.css");
}

DcpWidget *
TestApplet::pageMain(
        int Id)
{
    static int i = 0;

    SYS_DEBUG ("Entering %dnth times for id %d", i, Id);
    ++i;

    switch (Id) {
        case 0:
            if (m_MainWidget == NULL) {
                m_MainWidget = new TestWidget (Id);
                SYS_DEBUG ("Widget created");
            }

            return m_MainWidget;

        case 1:
            if (m_Page1 == NULL) {
                m_Page1 = new TestWidget (Id);
                 SYS_DEBUG ("Widget created");
            }

            return m_Page1;
        
        case 2:
            if (m_Page2 == NULL) {
                m_Page2 = new TestWidget (Id);
                 SYS_DEBUG ("Widget created");
            }

            return m_Page2;

        case 3:
            if (m_Page3 == NULL) {
                m_Page3 = new TestWidget (Id);
                 SYS_DEBUG ("Widget created");
            }

            return m_Page3;

        case 4:
            if (m_Page4 == NULL) {
                m_Page4 = new TestWidget (Id);
                 SYS_DEBUG ("Widget created");
            }

            return m_Page4;

        case 5:
            if (m_Page5 == NULL) {
                m_Page5 = new TestWidget (Id);
                 SYS_DEBUG ("Widget created");
            }

            return m_Page5;

        default:
            SYS_WARNING ("Invalid Id: %d", Id);
            Q_ASSERT (false);
    }

    return m_MainWidget;
}

DcpWidget *
TestApplet::constructWidget (
        int widgetId)
{
    Q_ASSERT (widgetId >= 0);
    Q_ASSERT (widgetId <= 5);

    SYS_DEBUG ("-----------------------------------");
    SYS_DEBUG ("*** widgetId = %d", widgetId);

    if (widgetId == 0 && m_MainWidget != 0) {
        SYS_WARNING ("m_MainWidget already exists!");
    } else if (widgetId == 1 && m_Page1 != 0) {
        SYS_WARNING ("m_Page1 already exists");
    } else if (widgetId == 2 && m_Page2 != 0) {
        SYS_WARNING ("m_Page2 already exists");
    } else if (widgetId == 3 && m_Page3 != 0) {
        SYS_WARNING ("m_Page3 already exists");
    } else if (widgetId == 4 && m_Page4 != 0) {
        SYS_WARNING ("m_Page4 already exists");
    } else if (widgetId == 5 && m_Page5 != 0) {
        SYS_WARNING ("m_Page5 already exists");
    }

    return pageMain (widgetId);
}

QString
TestApplet::title() const
{
    //% "Test"
    return qtTrId ("qtn_test_test"); // This is not official logical id
}

QVector<DuiAction*>
TestApplet::viewMenuItems()
{
    QVector<DuiAction*> vector;
    //% "Help"
    DuiAction* helpAction = new DuiAction (qtTrId ("qtn_comm_help"), 
            pageMain (0));

    vector.append(helpAction);
    helpAction->setLocation(DuiAction::ApplicationMenuLocation);

    return vector;
}

DcpBrief *
TestApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return new TestBrief ();
}


