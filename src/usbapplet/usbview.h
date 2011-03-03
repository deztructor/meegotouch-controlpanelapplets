/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef USBVIEW_H
#define USBVIEW_H

#include <DcpWidget>

#ifdef HAVE_QMSYSTEM
#  include <qmusbmode.h>
using namespace MeeGo;
#endif

class MLabel;
class MButton;
class MComboBox;
class MNotification;
class MLinearLayoutPolicy;

class UsbView : public DcpWidget
{
    Q_OBJECT

public:
    #ifdef HAVE_QMSYSTEM
    UsbView (MeeGo::QmUSBMode *logic);
    #else
    UsbView (void *logic);
    #endif
    ~UsbView ();

signals:
    void settingsChanged ();

private slots:
    void usbModeActivated (int idx);

protected:
    void retranslateUi ();

private:
    enum {
       UsbModeAlwaysAsk = 0,
       UsbModeMassStorage,
       UsbModeOviSuite
    };
    void initWidget (void);
    MLabel *addTitleLabel (MLinearLayoutPolicy    *targetPolicy,
                           const char             *panelStyleName,
                           const char             *labelStyleName);

private:
    #ifdef HAVE_QMSYSTEM
    MeeGo::QmUSBMode    *m_logic;
    #endif
    MComboBox           *m_UsbModeCombo;
    MLabel              *m_info_label;
    MNotification       *m_error;
    #ifdef UNIT_TEST
    friend class Ut_UsbApplet;
    #endif
};

#endif

