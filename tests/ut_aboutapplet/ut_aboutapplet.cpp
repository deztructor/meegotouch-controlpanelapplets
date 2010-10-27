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

#include "ut_aboutapplet.h"
#include "aboutapplet.h"
#include "aboutwidget.h"

#include "aboutbrief.h"

#include <dcpwidgettypes.h>
#include <dcpbrief.h>

#include <QSystemInfo>
#include <QSystemDeviceInfo>

QTM_USE_NAMESPACE

#include <mdesktopentry.h>
#include <MApplication>
#include <MAction>
#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_AboutApplet implementation.
 */
void
Ut_AboutApplet::init()
{
}

void
Ut_AboutApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_AboutApplet";

void
Ut_AboutApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);

    /*
     * We create an AboutApplet here. The applet will create a businesslogic,
     * for which we check that the data collection is not started.
     */
    m_Applet = new AboutApplet;
    QVERIFY (m_Applet->m_AboutBusinessLogic);

    /*
     * We are not testing the AboutBusinesslogic here, so the following lines
     * act as a kind of mocking, the aboutbusinesslogic will not initiate
     * outside communication if it has the data already.
     */
    m_Applet->m_AboutBusinessLogic->m_BluetoothAddress = "fakeBluetooth";
    m_Applet->m_AboutBusinessLogic->m_Imei = "FakeImei";
    m_Applet->m_AboutBusinessLogic->m_WifiAddress = "FakeWifi";
    m_Applet->m_AboutBusinessLogic->m_OsName = "FakeOsName";
    m_Applet->m_AboutBusinessLogic->m_OsVersion = "FakeOsVersion";


    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();

    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);

    /*
     * TODO: FIXME: remove this, but output can be helpful
     * for development & testing purposes
     */
    QSystemDeviceInfo deviceInfo;
    SYS_DEBUG ("\n****** QSystemDeviceInfo *****\n"
               "***** imei: %s\n***** manufacturer: %s\n"
               "***** model: %s\n***** productName: %s\n",
               SYS_STR(deviceInfo.imei ()), SYS_STR(deviceInfo.manufacturer ()),
               SYS_STR(deviceInfo.model ()), SYS_STR(deviceInfo.productName ()));

    QSystemInfo systemInfo;
    SYS_DEBUG ("\n****** QSystemInfo versions *****\n"
               "***** Os: %s\n***** QtCore: %s\n***** Firmware: %s\n",
               SYS_STR(systemInfo.version (QSystemInfo::Os)),
               SYS_STR(systemInfo.version (QSystemInfo::QtCore)),
               SYS_STR(systemInfo.version (QSystemInfo::Firmware)));
}

void
Ut_AboutApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void
Ut_AboutApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_AboutApplet::testConstructWidget ()
{
    AboutWidget *widget;
    bool       backAccepted;

    /*
     * Testing if the applet creates a widget the first time.
     */
    widget = (AboutWidget *) m_Applet->constructWidget (0);
    QVERIFY (widget);
    QVERIFY (m_Applet->m_MainWidget == widget);
    QVERIFY (!m_Applet->constructWidget (1));

    /*
     * FIXME: These are not much checking, more like coverage making calls.
     */
    widget->retranslateUi ();

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
Ut_AboutApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();

    QVERIFY (items.size() == 0);
}

void
Ut_AboutApplet::testConstructbrief ()
{
    AboutBrief *brief1 = (AboutBrief *) m_Applet->constructBrief(0);
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


QTEST_APPLESS_MAIN(Ut_AboutApplet)




