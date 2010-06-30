/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef USBBRIEF_H
#define USBBRIEF_H

#include <MLocale>
#include <DcpBrief>
#include <qmusbmode.h>

class UsbBrief: public DcpBrief
{
    Q_OBJECT

public:
    UsbBrief (Maemo::QmUSBMode *logic);
    QString valueText () const;
    virtual int widgetTypeID() const;

protected:
    void    retranslateUi ();

public slots:
    void    settingsChanged ();

private:
    Maemo::QmUSBMode    *m_logic;
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif
