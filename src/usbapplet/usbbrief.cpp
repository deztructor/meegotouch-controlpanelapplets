/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include "usbbrief.h"

#include <QVariant>
#include <QString>

using namespace Maemo;

#define DEBUG
#include "../debug.h"

UsbBrief::UsbBrief (Maemo::QmUSBMode *logic) :
    m_logic (logic)
{
    connect (m_logic, SIGNAL (modeChanged (Maemo::QmUSBMode::Mode)),
             this, SIGNAL (valuesChanged ()));
}

void 
UsbBrief::settingsChanged ()
{
    emit valuesChanged ();
}

void
UsbBrief::retranslateUi ()
{
    // The translated text (valueText) may change...
    emit valuesChanged (); 
}

QString
UsbBrief::valueText () const
{
    QString currentSetting; 

    // Get the current setting localised string
    switch (m_logic->getDefaultMode ())
    {
        case QmUSBMode::OviSuite:
            //% "Ovi Suite mode"
            currentSetting = qtTrId ("qtn_usb_ovi_suite");
            break;
        case QmUSBMode::MassStorage:
            //% "Mass Storage mode"
            currentSetting = qtTrId ("qtn_usb_mass_storage");
            break;
        case QmUSBMode::Ask:
        default:
            //% "Always ask"
            currentSetting = qtTrId ("qtn_usb_always_ask");
            break;
    }

    // Check the currently active mode
    switch (m_logic->getMode ())
    {
        case QmUSBMode::Disconnected:
            return currentSetting; // just return the current setting value
            break;
        case QmUSBMode::MassStorage:
        case QmUSBMode::OviSuite:
            if (m_logic->getMode () == m_logic->getDefaultMode ())
            {
                //% "%1 active"
                return qtTrId ("qtn_usb_active_mode").arg (currentSetting);
            }
            else if (m_logic->getDefaultMode () == QmUSBMode::Ask)
            {
                //% "Always ask - %1 active"
                return qtTrId ("qtn_usb_ask_active").arg (currentSetting);
            }
            break;
        default:
            SYS_DEBUG ("What about %d mode?", m_logic->getMode ());
            SYS_DEBUG ("current-setting: %s", SYS_STR (currentSetting));
            break;
    }

    return currentSetting;
}

