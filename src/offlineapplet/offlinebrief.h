/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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

#ifndef OFFLINEBRIEF_H
#define OFFLINEBRIEF_H

#include <DcpBrief>

#ifdef HAVE_QMSYSTEM
#  include <qmdevicemode.h>
using namespace Maemo;
#endif

class OfflineBrief: public DcpBrief{
    Q_OBJECT
public:
    OfflineBrief();
    ~OfflineBrief();
    virtual QString valueText() const;

    virtual void setToggle (bool toggle);
    virtual int widgetTypeID() const;

private:
    QString currentText() const;

private slots:
    #ifdef HAVE_QMSYSTEM
    void devModeChanged(Maemo::QmDeviceMode::DeviceMode mode);
    #endif
    void processDialogResult();

private:
    #ifdef HAVE_QMSYSTEM
    Maemo::QmDeviceMode::DeviceMode  m_LastMode;
    Maemo::QmDeviceMode             *m_DevMode;
    #endif

#ifdef UNIT_TEST
    friend class Ut_OfflineApplet;
#endif

};


#endif
