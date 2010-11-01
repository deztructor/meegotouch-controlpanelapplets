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

#include "usbbrief.h"

#include <QVariant>
#include <QString>
#include <MGConfItem>
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"

const QString usbModeGConfKey = "/Meego/System/UsbMode";

#ifdef HAVE_QMSYSTEM
UsbBrief::UsbBrief (MeeGo::QmUSBMode *logic) :
    m_logic (logic)
{
    SYS_DEBUG ("instance: %p", this);
    connect (m_logic, SIGNAL (modeChanged (MeeGo::QmUSBMode::Mode)),
             this, SIGNAL (valuesChanged ()));

    /*
     * due to controlpanel changes, we need to
     * listen also for gconf key changes as well...
     */
    m_gconfitem = new MGConfItem (usbModeGConfKey, this);
    connect (m_gconfitem, SIGNAL (valueChanged ()),
             this, SIGNAL (valuesChanged ()));
}
#else
/*
 * FIXME: To implement the version that works without the QmSystem library.
 */
UsbBrief::UsbBrief (void *logic) 
{
    m_gconfitem = new MGConfItem (usbModeGConfKey, this);
    connect (m_gconfitem, SIGNAL (valueChanged ()),
             this, SIGNAL (valuesChanged ()));
}
#endif


int
UsbBrief::widgetTypeID() const
{
    return DcpWidgetType::Label;
}

void
UsbBrief::settingsChanged ()
{
    SYS_DEBUG ("");
    emit valuesChanged ();
}

void
UsbBrief::retranslateUi ()
{
    // The translated text (valueText) may change...
    emit valuesChanged ();
}

#ifdef HAVE_QMSYSTEM
QString
UsbBrief::valueText () const
{
    QmUSBMode::Mode active = m_logic->getMode ();
    QmUSBMode::Mode setting = m_logic->getDefaultMode ();

    if (setting == QmUSBMode::Ask)
    {
        if (active == QmUSBMode::MassStorage)
            //% "Always ask - %1 active"
            return qtTrId ("qtn_usb_ask_active").arg (
            //% "Mass Storage mode"
                   qtTrId ("qtn_usb_mass_storage"));
        else if (active == QmUSBMode::OviSuite)
            //% "Always ask - %1 active"
            return qtTrId ("qtn_usb_ask_active").arg (
            //% "Ovi Suite mode"
                   qtTrId ("qtn_usb_ovi_suite"));
        else
            //% "Always ask"
            return qtTrId ("qtn_usb_always_ask");
    }

    QString currentSetting;

    switch (setting)
    {
        case QmUSBMode::OviSuite:
            //% "Ovi Suite mode"
            currentSetting = qtTrId ("qtn_usb_ovi_suite");
            break;
        case QmUSBMode::MassStorage:
            //% "Mass Storage mode"
            currentSetting = qtTrId ("qtn_usb_mass_storage");
            break;
        default:
            //% "Always ask"
            currentSetting = qtTrId ("qtn_usb_always_ask");
            break;
    }

    if ((active == setting) &&
        ((active == QmUSBMode::OviSuite) ||
         (active == QmUSBMode::MassStorage)))
        //% "%1 active"
        return qtTrId ("qtn_usb_active_mode").arg (currentSetting);
    else
        return currentSetting;
}
#else
/*
 * FIXME: To implement a version that works without the QmSystem
 */
QString
UsbBrief::valueText () const
{
    return QString ("QmSystem is not available");
}
#endif
