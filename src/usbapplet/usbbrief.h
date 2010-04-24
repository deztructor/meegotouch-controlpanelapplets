/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef USBBRIEF_H
#define USBBRIEF_H

#include <MLocale>
#include <DcpBrief>
#include "usbbusinesslogic.h"

class UsbBrief: public DcpBrief
{
    Q_OBJECT

public:
    UsbBrief (UsbSettingsLogic *logic);
    QString valueText () const;

protected:
    void    retranslateUi ();

public slots:
    void settingsChanged ();

private slots:
    void currentModeChanged (usb_modes mode);

private:
    UsbSettingsLogic    *m_logic;
};

#endif
