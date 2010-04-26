/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include <MAction>
#include <DcpWidget>
#include <MLocale>

#include "usbbusinesslogic.h"
#include "usbbrief.h"
#include "usbview.h"
#include "usbapplet.h"

Q_EXPORT_PLUGIN2(usbapplet, UsbApplet)

void
UsbApplet::init (void)
{
    m_logic = new UsbSettingsLogic (this);
    m_brief = NULL;
}

DcpWidget *
UsbApplet::constructWidget (int widgetId)
{
    Q_UNUSED (widgetId);

    UsbView  *view =
        new UsbView (m_logic);

    connect (view,  SIGNAL (settingsChanged ()),
             m_brief, SLOT (settingsChanged ()));

    return view;
}

QString
UsbApplet::title (void) const
{
    //% "USB"
    return qtTrId ("qtn_usb_title");
}

QVector<MAction*>
UsbApplet::viewMenuItems ()
{
    QVector<MAction*> vector;

    return vector;
}

DcpBrief *
UsbApplet::constructBrief (int partId)
{
    Q_UNUSED (partId);

    m_brief = new UsbBrief (m_logic);

    return m_brief;
}

