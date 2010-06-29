/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_resetapplet.h"
#include "resetapplet.h"
#include "resetwidget.h"

#include "resetbrief.h"

#include <dcpwidgettypes.h>
#include <dcpbrief.h>

#include <mdesktopentry.h>
#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_ResetApplet implementation. 
 */
void 
Ut_ResetApplet::init()
{
}

void 
Ut_ResetApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ResetApplet";

void 
Ut_ResetApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new ResetApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_ResetApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_ResetApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_ResetApplet::testConstructWidget ()
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
Ut_ResetApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    QVERIFY (items.size() == 1);
}

void 
Ut_ResetApplet::testConstructbrief ()
{
    ResetBrief *brief1 = (ResetBrief *) m_Applet->constructBrief(0);
    QString   iconName;
    QString   text;

    QVERIFY (brief1);
    QVERIFY (brief1->widgetTypeID() == DcpWidgetType::Label);

    iconName = brief1->icon ();
    QVERIFY (iconName.isEmpty());

    text = brief1->valueText ();
    QVERIFY (text.isEmpty());

    // FIXME: Why does this method has an argument, it must be some leftover
    // code. Also: we should check if the signal is emitted.
    delete brief1;
}


QTEST_APPLESS_MAIN(Ut_ResetApplet)




