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
#include "offlinebrief.h"

#include <DcpWidgetTypes>
#include <MBanner>
#include <MMessageBox>
#include <MLabel>

#undef DEBUG
#include "../debug.h"

#ifdef HAVE_QMSYSTEM
OfflineBrief::OfflineBrief():
    m_DevMode(new QmDeviceMode())
{
    connect(m_DevMode, SIGNAL(deviceModeChanged(Maemo::QmDeviceMode::DeviceMode)),
            this, SLOT(devModeChanged(Maemo::QmDeviceMode::DeviceMode)));
    m_LastMode = m_DevMode->getMode();
}
#else
OfflineBrief::OfflineBrief()
{
    /*
     * FIXME: To install a version that works without the help of the QmSystem
     * library.
     */
}
#endif

OfflineBrief::~OfflineBrief()
{
    #ifdef HAVE_QMSYSTEM
    delete m_DevMode;
    #endif
}

#ifdef HAVE_QMSYSTEM
void 
OfflineBrief::devModeChanged (
        Maemo::QmDeviceMode::DeviceMode mode)
{
    SYS_DEBUG("newmode %d", mode);
    m_LastMode = mode;
    emit valuesChanged();
}
#endif

QString OfflineBrief::valueText() const
{
    SYS_DEBUG("");
    return currentText();
}

QString 
OfflineBrief::currentText() const
{
    SYS_DEBUG("");

    #ifdef HAVE_QMSYSTEM
    switch (m_LastMode)
    {
        case QmDeviceMode::Flight:
            //% "Deactivate offline mode"
            return qtTrId("qtn_offl_deactivate");
        case QmDeviceMode::Normal:
        default:
            //% "Activate offline mode"
            return qtTrId("qtn_offl_activate");
    }
    #endif

    return QString();
}

void 
OfflineBrief::setToggle (
        bool toggle)
{
    Q_UNUSED(toggle);
    
    #ifdef HAVE_QMSYSTEM
    SYS_DEBUG("");
    if (m_LastMode == QmDeviceMode::Flight)
    {
        //% "Exit offline mode?"
        MMessageBox* dialog = new MMessageBox("", qtTrId("qtn_offl_exiting"),
            M::YesButton | M::NoButton);
        connect(dialog, SIGNAL(disappeared()), this, SLOT(processDialogResult()));
        dialog->appear();
    }
    else
    {
        if (m_DevMode->setMode(QmDeviceMode::Flight))
        {
            MBanner *infoBanner = new MBanner();
            //% "Closing all connections. Switching to offline mode."
            infoBanner->setSubtitle(qtTrId("qtn_offl_entering"));
            infoBanner->appear(MSceneWindow::DestroyWhenDone);
        }
    }
    #endif
}

void OfflineBrief::processDialogResult()
{
    #ifdef HAVE_QMSYSTEM
    MMessageBox *dialog = static_cast<MMessageBox*>(sender());
    if(dialog->result() == MDialog::Accepted)
    {
        m_DevMode->setMode(QmDeviceMode::Normal);
    }
    #endif
}

int OfflineBrief::widgetTypeID() const
{
    SYS_DEBUG("");
    return DcpWidgetType::Button;
}

