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
#ifndef USBBRIEF_H
#define USBBRIEF_H

#include <MLocale>
#include <DcpBrief>

class MGConfItem;

#ifdef HAVE_QMSYSTEM
#  include <qmusbmode.h>
using namespace MeeGo;
#endif

class UsbBrief: public DcpBrief
{
    Q_OBJECT

public:
    #ifdef HAVE_QMSYSTEM
    UsbBrief (MeeGo::QmUSBMode *logic);
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
    MGConfItem          *m_gconfitem;
    #ifdef HAVE_QMSYSTEM
    MeeGo::QmUSBMode    *m_logic;
    #endif
    
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif
