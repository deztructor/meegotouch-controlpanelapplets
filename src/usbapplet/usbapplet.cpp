/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <MAction>
#include <DcpWidget>
#include <MLocale>

#include "usbbrief.h"
#include "usbview.h"
#include "usbapplet.h"

#define DEBUG
#include "../debug.h"

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

    SYS_DEBUG ("applet instance : %p (brief instance : %p)");

    return m_brief;
}

