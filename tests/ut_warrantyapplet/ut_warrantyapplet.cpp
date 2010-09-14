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

#include "ut_warrantyapplet.h"
#include "warrantyapplet.h"
#include "warrantywidget.h"

#include "warrantybrief.h"

#include <dcpwidgettypes.h>
#include <dcpbrief.h>

#include <mdesktopentry.h>
#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_WarrantyApplet implementation. 
 */
void 
Ut_WarrantyApplet::init()
{
}

void 
Ut_WarrantyApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WarrantyApplet";

void 
Ut_WarrantyApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new WarrantyApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_WarrantyApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_WarrantyApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_WarrantyApplet::testConstructWidget ()
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
Ut_WarrantyApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    QVERIFY (items.size() == 0);
}

void 
Ut_WarrantyApplet::testConstructbrief ()
{
    WarrantyBrief *brief1 = (WarrantyBrief *) m_Applet->constructBrief(0);
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


QTEST_APPLESS_MAIN(Ut_WarrantyApplet)




