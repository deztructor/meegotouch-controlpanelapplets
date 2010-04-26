#ifndef _USBBUSINESSLOGIC_H
#define _USBBUSINESSLOGIC_H

#include <QObject>
#include <QDBusError>

class QDBusInterface;
class MGConfItem;

// Buttons in UI exists only for the first three modes
enum usb_modes {
    USB_OVI_SUITE = 10, 
    USB_MASS_STORAGE,
    USB_AUTO,
    USB_NOOP,
    USB_NOTCONNECTED
};

Q_DECLARE_METATYPE(usb_modes);

class UsbSettingsLogic : public QObject
{
    Q_OBJECT
    Q_ENUMS(usb_modes)

public:
    UsbSettingsLogic (QObject *parent = 0);
    virtual ~UsbSettingsLogic ();

    usb_modes getUsbSetting ();
    usb_modes getCurrentMode ();

signals:
    // emitted when some usb mode was activated...
    void currentModeChanged (usb_modes currentMode);

public slots:
    // mode <= USB_NOOP
    void setUsbSetting (usb_modes mode);

private slots:
    // usb_moded signal handler
    void usbModeChange (QString mode);
    void dbusError (QDBusError error);

private:
    QDBusInterface      *m_usb_moded;
    MGConfItem          *m_gconfkey;
    usb_modes            m_currentMode;

#ifdef UNIT_TEST
    friend class Ut_UsbSettingsLogic;
#endif
};

#endif
