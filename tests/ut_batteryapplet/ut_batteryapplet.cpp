/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_batteryapplet.h"
#include "batteryapplet.h"
#include "batterywidget.h"

#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_BatteryApplet implementation. 
 */
void 
Ut_BatteryApplet::init()
{
}

void 
Ut_BatteryApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_BatteryApplet";

void 
Ut_BatteryApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new BatteryApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_BatteryApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_BatteryApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_BatteryApplet::testConstructWidget ()
{
    DcpWidget *widget;
    bool       backAccepted;

    /*
     * Testing if the applet creates a widget the first time.
     */
    widget = m_Applet->constructWidget (0);
    QVERIFY (widget);
    QVERIFY (m_Applet->m_MainWidget == widget);
    
    /*
     * Testing if the widget accepts the back. Our applets always accept back.
     */
    backAccepted = widget->back();
    QVERIFY (backAccepted);

    /*
     * Testing if the applet knows about the destruction of the widget.
     */
    delete widget;
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_BatteryApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    QVERIFY (items.size() == 1);
}

void 
Ut_BatteryApplet::testConstructbrief ()
{
    DcpBrief *brief1 = m_Applet->constructBrief();

    QVERIFY (brief1);
    delete brief1;
}


QTEST_APPLESS_MAIN(Ut_BatteryApplet)


