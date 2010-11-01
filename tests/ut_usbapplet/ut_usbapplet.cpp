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

#include "ut_usbapplet.h"
#include "usbapplet.h"
#include "usbview.h"
#include "usbbrief.h"

#ifdef HAVE_QMSYSTEM
#  include "qmusbmode.h"
#  ifndef QMUSBMODE_STUB_H
#    error "The QmUSBMode is not stubbed, can not continue."
#  endif
using namespace MeeGo;
#endif


#include <dcpwidgettypes.h>
#include <dcpbrief.h>

#include <mdesktopentry.h>
#include <MApplication>
#include <MAction>
#include <MNotification>

#include <QVector>
#include <QList>
#include <QMap>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * The translations for the USBApplet contains special characters (like %1) so
 * we need to stub the qtTrId() method here.
 */
static QMap<QString, QString> catalog;
static bool                   catalogInitialized = false;

void
initializeCatalog ()
{
    if (catalogInitialized)
        return;
    
    catalog["qtn_usb_ask_active"] = "qtn_usb_ask_active - %1";
    catalog["qtn_usb_active_mode"] = "qtn_usb_active_mode %1";
}

QString
qtTrId (
        const char  *id, 
        int          n)
{
    QString retVal;

    Q_UNUSED (n);
    initializeCatalog ();
    
    retVal = catalog[id];
    if (retVal.isEmpty())
        retVal = id;

    //SYS_DEBUG ("*** id          = %s", id);
    //SYS_DEBUG ("*** translation = %s", SYS_STR(retVal));

    return retVal;
}

/******************************************************************************
 * Stubbing MNotification
 */
static QString lastPublishedNotification;

bool 
MNotification::publish()
{
    //SYS_DEBUG ("*** body() = %s", SYS_STR(body()));
    lastPublishedNotification = body();
    return true;
}


/******************************************************************************
 * Ut_UsbApplet implementation. 
 */
void 
Ut_UsbApplet::init()
{
}

void 
Ut_UsbApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_UsbApplet";

void 
Ut_UsbApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new UsbApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_UsbApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_UsbApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_UsbApplet::testConstructWidget ()
{
    #ifdef HAVE_QMSYSTEM
    UsbView                 *widget;
    bool                     backAccepted;
    QList<QmUSBMode::Mode>   availableModes;

    availableModes <<
            QmUSBMode::Connected <<
            QmUSBMode::DataInUse <<   
            QmUSBMode::Disconnected <<
            QmUSBMode::MassStorage <<
            QmUSBMode::ChargingOnly << 
            QmUSBMode::OviSuite <<
            QmUSBMode::ModeRequest <<
            QmUSBMode::Ask << 
            QmUSBMode::Undefined;

    /*
     * Testing if the applet creates a widget the first time.
     */
    widget = (UsbView *) m_Applet->constructWidget (0);
    QVERIFY (widget);
    QVERIFY (m_Applet->m_MainWidget == widget);
    
    /*
     * Testing if the widget accepts the back. Our applets always accept back.
     */
    backAccepted = widget->back();
    QVERIFY (backAccepted);

    /*
     * For all default modes and modes we try to change to all the modes.
     */
    foreach (QmUSBMode::Mode mode, availableModes) {
        foreach (QmUSBMode::Mode defaultMode, availableModes) {
            m_Applet->m_logic->setDefaultMode (defaultMode);
            m_Applet->m_logic->setMode (mode);

            for (int id = 0; id < 8; ++id) {
                lastPublishedNotification = "";
                widget->selectionChanged (id);

                if (id == mode) {
                    // If we want to change to the current mode nothing happens.
                    QVERIFY (lastPublishedNotification.isEmpty());
                } else if (mode == QmUSBMode::MassStorage ||
                        mode == QmUSBMode::OviSuite) {
                    // If we are connected and try to change the mode we got the
                    // error message and we stay in the same mode we were in.
                    QVERIFY (lastPublishedNotification == 
                            "<p>qtn_usb_change_incorrect</p>");
                    QVERIFY (m_Applet->m_logic->getDefaultMode() == 
                            defaultMode);
                    QVERIFY (m_Applet->m_logic->getMode() == mode);
                } else {
                    // Otherwise we should have reached the new mode.
                    QVERIFY (lastPublishedNotification.isEmpty());
                    QVERIFY (m_Applet->m_logic->getDefaultMode() == id);
                    // FIXME: There are the QmUSBMode::ChargingOnly case might
                    // be tested here along with the signal.
                }

            }
        }
    }


    /*
     * Testing if the applet knows about the destruction of the widget.
     */
    delete widget;
    QVERIFY (!m_Applet->m_MainWidget);
    #endif
}

void 
Ut_UsbApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    //QVERIFY (items.size() == 1);
    SYS_DEBUG ("items.size() = %d", items.size());
}

void 
Ut_UsbApplet::testConstructbrief ()
{
    #ifdef HAVE_QMSYSTEM
    UsbBrief *brief1 = (UsbBrief *) m_Applet->constructBrief(0);
    QList<QmUSBMode::Mode>   availableModes;
    QString   iconName;
    QString   text;
    
    availableModes <<
            QmUSBMode::Connected <<
            QmUSBMode::DataInUse <<   
            QmUSBMode::Disconnected <<
            QmUSBMode::MassStorage <<
            QmUSBMode::ChargingOnly << 
            QmUSBMode::OviSuite <<
            QmUSBMode::ModeRequest <<
            QmUSBMode::Ask << 
            QmUSBMode::Undefined;
    /*
     * Getting the brief and checking its properties.
     */
    QVERIFY (brief1);
    QVERIFY (brief1->widgetTypeID() == DcpWidgetType::Label);
    iconName = brief1->icon ();
    QVERIFY (iconName.isEmpty());

    /*
     * Testing all the combinations when the default mode is QmUSBMode::Ask.
     */
    foreach (QmUSBMode::Mode mode, availableModes) {
        foreach (QmUSBMode::Mode defaultMode, availableModes) {
            m_Applet->m_logic->setDefaultMode (defaultMode);
            m_Applet->m_logic->setMode (mode);
            

            text = brief1->valueText ();
            //SYS_DEBUG ("*** text        = %s", SYS_STR(text));
            //SYS_DEBUG ("*** mode        = %d", mode);
            //SYS_DEBUG ("*** defaultMode = %d", defaultMode);

            if (defaultMode == QmUSBMode::Ask &&
                    mode == QmUSBMode::MassStorage)
                QVERIFY (text == "qtn_usb_ask_active - qtn_usb_mass_storage");
            else if (defaultMode == QmUSBMode::Ask &&
                    mode == QmUSBMode::OviSuite)
                QVERIFY (text == "qtn_usb_ask_active - qtn_usb_ovi_suite");
            else if (defaultMode == QmUSBMode::Ask)
                QVERIFY (text == "qtn_usb_always_ask");
            else if (defaultMode == QmUSBMode::OviSuite &&
                    mode == QmUSBMode::OviSuite)
                QVERIFY (text == "qtn_usb_active_mode qtn_usb_ovi_suite");
            else if (defaultMode == QmUSBMode::MassStorage &&
                    mode == QmUSBMode::MassStorage)
                QVERIFY (text == "qtn_usb_active_mode qtn_usb_mass_storage");
            else if (defaultMode == QmUSBMode::OviSuite)
                QVERIFY (text == "qtn_usb_ovi_suite");
            else if (defaultMode == QmUSBMode::MassStorage)
                QVERIFY (text == "qtn_usb_mass_storage");
            else {
                QVERIFY (text == "qtn_usb_always_ask");
            }
        }
    }
    
    /*
     * FIXME: These should be tested somehow.
     */
    brief1->settingsChanged ();
    brief1->retranslateUi ();

    delete brief1;
    #endif
}


QTEST_APPLESS_MAIN(Ut_UsbApplet)




