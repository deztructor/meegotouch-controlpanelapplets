/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <QDebug>
#include <MAction>
#include <DcpWidget>
#include <MLocale>

#include "usbbrief.h"
#include "usbview.h"
#include "usbapplet.h"

Q_EXPORT_PLUGIN2(usbapplet, UsbApplet)

void 
UsbApplet::init (void)
{
    m_Brief = NULL;
}

DcpWidget *
UsbApplet::constructWidget (
        int widgetId)
{
    UsbView  *view;

    Q_UNUSED(widgetId);

    view = new UsbView ();

    connect (view,  SIGNAL (settingsChanged (int)), 
             m_Brief, SLOT (settingsChanged (int)));

    return view;
}

QString 
UsbApplet::title (void) const
{
    //% "USB"
    return qtTrId ("qtn_usb_title");
}

QVector<MAction*> 
UsbApplet::viewMenuItems (
        void)
{
    QVector<MAction*> vector;

    return vector;
}

DcpBrief * 
UsbApplet::constructBrief (
        int partId)
{
    Q_UNUSED(partId);
    
    m_Brief = new UsbBrief();
    
    return m_Brief;
}

