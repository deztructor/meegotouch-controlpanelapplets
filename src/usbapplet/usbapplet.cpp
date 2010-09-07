/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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
    #ifdef HAVE_QMSYSTEM
    m_logic = new QmUSBMode (this);
    #endif

    m_brief = NULL;
}

DcpWidget *
UsbApplet::constructWidget (int widgetId)
{
    Q_UNUSED (widgetId);

    if (!m_MainWidget) {
        #ifdef HAVE_QMSYSTEM
        m_MainWidget = new UsbView (m_logic);
        #else
        /*
         * FIXME: To implement a variant that does not use QmSystem.
         */
        m_MainWidget = new UsbView (NULL);
        #endif
        connect (m_MainWidget,  SIGNAL (settingsChanged ()),
                m_brief, SLOT (settingsChanged ()));
    }

    return m_MainWidget;
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

    #ifdef HAVE_QMSYSTEM
    m_brief = new UsbBrief (m_logic);
    #else
    /*
     * FIXME: To implement a variant that does not use QmSystem.
     */
     m_MainWidget = new UsbView (NULL);
    #endif

    return m_brief;
}

