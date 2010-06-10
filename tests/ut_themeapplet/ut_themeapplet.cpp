/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themeapplet.h"
#include "themeapplet.h"
#include "themewidget.h"

#include "dcpbrief.h"

#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_ThemeApplet implementation. 
 */
void 
Ut_ThemeApplet::init()
{
}

void 
Ut_ThemeApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ThemeApplet";

void 
Ut_ThemeApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new ThemeApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_ThemeApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_ThemeApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_ThemeApplet::testConstructWidget ()
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
Ut_ThemeApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    QVERIFY (items.size() == 1);
}

void 
Ut_ThemeApplet::testConstructbrief ()
{
    DcpBrief *brief1 = m_Applet->constructBrief(0);

    QVERIFY (brief1);
    delete brief1;
}


QTEST_APPLESS_MAIN(Ut_ThemeApplet)



