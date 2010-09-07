/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef USBBRIEF_H
#define USBBRIEF_H

#include <MLocale>
#include <DcpBrief>

#ifdef HAVE_QMSYSTEM
#  include <qmusbmode.h>
using namespace Maemo;
#endif

class UsbBrief: public DcpBrief
{
    Q_OBJECT

public:
    #ifdef HAVE_QMSYSTEM
    UsbBrief (Maemo::QmUSBMode *logic);
    #else
    UsbBrief (void *logic);
    #endif
    QString valueText () const;
    virtual int widgetTypeID() const;

protected:
    void    retranslateUi ();

public slots:
    void    settingsChanged ();

private:
    #ifdef HAVE_QMSYSTEM
    Maemo::QmUSBMode    *m_logic;
    #endif
    
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif
