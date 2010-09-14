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

#include "ut_profileapplet.h"
#include "profileapplet.h"
#include "profilewidget.h"

#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_ProfileApplet implementation. 
 */
void 
Ut_ProfileApplet::init()
{
}

void 
Ut_ProfileApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ProfileApplet";

void 
Ut_ProfileApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new ProfileApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_ProfileApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_ProfileApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_ProfileApplet::testConstructWidget ()
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

    /*
     * For an invalid window ID we expect the null pointer. ;)
     */
    //QVERIFY (!m_Applet->constructWidget(1));
}

void 
Ut_ProfileApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    // I'm not sure why this applet returns no items, but it does.
    QVERIFY (items.size() == 0);
}

void 
Ut_ProfileApplet::testConstructbrief ()
{
    DcpBrief *brief1 = m_Applet->constructBrief(0);

    // I'm not sure why this applet returns NULL but apparently it does.
    QVERIFY (!brief1);
    //delete brief1;
}


QTEST_APPLESS_MAIN(Ut_ProfileApplet)



