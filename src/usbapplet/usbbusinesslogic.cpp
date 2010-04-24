#include "usbbusinesslogic.h"

#define DEBUG
#include "../debug.h"

#include <usb_moded-modes.h>
#include <usb_moded-dbus.h>

#include <QDBusInterface>
#include <MGConfItem>

#define USB_GCONF_KEY "/Meego/System/UsbMode"

UsbSettingsLogic::UsbSettingsLogic (QObject *parent) :
    QObject (parent),
    m_usb_moded (0),
    m_gconfkey (new MGConfItem (USB_GCONF_KEY))
{
    m_currentMode = USB_NOTCONNECTED;

    m_usb_moded =
        new QDBusInterface (QString (USB_MODE_SERVICE),
                            QString (USB_MODE_OBJECT),
                            QString (USB_MODE_INTERFACE),
                            QDBusConnection::systemBus ());

    connect (m_usb_moded, SIGNAL (sig_usb_state_ind (QString)),
             this, SLOT (usbModeChange (QString)));

    // Request the current state from usb_moded
    m_usb_moded->callWithCallback (QString (USB_MODE_STATE_REQUEST),
                                   QList<QVariant> (), this,
                                   SLOT (usbModeChange (QString)),
                                   SLOT (dbusError (QString)));
}

UsbSettingsLogic::~UsbSettingsLogic ()
{
    delete m_gconfkey;
    delete m_usb_moded;
}

usb_modes
UsbSettingsLogic::getUsbSetting ()
{
    if (m_gconfkey->value ().type () == QVariant::Invalid)
        return USB_AUTO;

    QString val = m_gconfkey->value ().toString ();

//    SYS_DEBUG ("Get %s: %s", USB_GCONF_KEY, SYS_STR (val));

    if (val == MODE_MASS_STORAGE)
        return USB_MASS_STORAGE;
    else if (val == MODE_OVI_SUITE)
        return USB_OVI_SUITE;
/*
 * XXX: this one can be an easter egg ;-)
 *
    else if (val == MODE_CHARGING)
        return USB_NOOP;
 */
    else /* MODE_UNDEFINED && MODE_ASK */
        return USB_AUTO;
}

void
UsbSettingsLogic::setUsbSetting (usb_modes mode)
{
    QString setting (MODE_ASK);

    if (mode > USB_NOOP)
        return; // disconnection cannot be done here

    switch (mode)
    {
        case USB_OVI_SUITE:
            setting = MODE_OVI_SUITE;
            break;
        case USB_MASS_STORAGE:
            setting = MODE_MASS_STORAGE;
            break;
        default:
            break;
    }

//    SYS_DEBUG ("Set %s: %s", USB_GCONF_KEY, SYS_STR (setting));

    m_gconfkey->set (setting);
}

usb_modes
UsbSettingsLogic::getCurrentMode ()
{
    return m_currentMode;
}

// The usb_moded signal handler:
void
UsbSettingsLogic::usbModeChange (QString mode)
{
    SYS_DEBUG ("usb_moded mode = %s", SYS_STR (mode));

    if (mode == USB_DISCONNECTED)
        m_currentMode = USB_NOTCONNECTED;
    else if (mode == MODE_MASS_STORAGE)
        m_currentMode = USB_MASS_STORAGE;
    else if (mode == MODE_OVI_SUITE)
        m_currentMode = USB_OVI_SUITE;
    else
        m_currentMode = USB_NOOP;

    emit currentModeChanged (m_currentMode);
}

void
UsbSettingsLogic::dbusError (QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name ()),
                           SYS_STR (error.message ()));
}

