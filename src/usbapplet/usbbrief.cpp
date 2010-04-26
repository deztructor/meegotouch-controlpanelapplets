/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MLocale>
#include <QLocale>
#include <QVariant>
#include <QString>

#define DEBUG
#include "../debug.h"
#include "usbbrief.h"

UsbBrief::UsbBrief (UsbSettingsLogic *logic) : DcpBrief (),
    m_logic (logic)
{
    connect (m_logic, SIGNAL (currentModeChanged (usb_modes)),
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
    switch (m_logic->getUsbSetting ())
    {
        case USB_OVI_SUITE:
            //% "Ovi Suite mode"
            currentSetting = qtTrId ("qtn_usb_ovi_suite");
            break;
        case USB_MASS_STORAGE:
            //% "Mass Storage mode"
            currentSetting = qtTrId ("qtn_usb_mass_storage");
            break;
        case USB_AUTO:
        default:
            //% "Always ask"
            currentSetting = qtTrId ("qtn_usb_always_ask");
            break;
    }

    // Check the currently active mode
    switch (m_logic->getCurrentMode ())
    {
        case USB_NOTCONNECTED:
            return currentSetting; // just return the current setting value
            break;
        case USB_MASS_STORAGE:
        case USB_OVI_SUITE:
            if (m_logic->getCurrentMode () == m_logic->getUsbSetting ())
            {
                //% "%1 active"
                return qtTrId ("qtn_usb_active_mode").arg (currentSetting);
            }
            else if (m_logic->getUsbSetting () == USB_AUTO)
            {
                //% "Always ask - %1 active"
                return qtTrId ("qtn_usb_ask_active").arg (currentSetting);
            }
            break;
        case USB_NOOP:
        case USB_AUTO:
        default:
            SYS_DEBUG ("What about %d mode?", m_logic->getCurrentMode ());
            SYS_DEBUG ("current-setting: %s", SYS_STR (currentSetting));
            break;
    }

    return currentSetting;
}

