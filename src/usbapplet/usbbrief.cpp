/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include "usbbrief.h"

#include <QVariant>
#include <QString>
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"

#ifdef HAVE_QMSYSTEM
UsbBrief::UsbBrief (Maemo::QmUSBMode *logic) :
    m_logic (logic)
{
    connect (m_logic, SIGNAL (modeChanged (Maemo::QmUSBMode::Mode)),
             this, SIGNAL (valuesChanged ()));
}
#else
/*
 * FIXME: To implement the version that works without the QmSystem library.
 */
UsbBrief::UsbBrief (void *logic) 
{
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
