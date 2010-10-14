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
#ifndef QMDEVICEMODE_STUB
#define QMDEVICEMODE_STUB

#include <qmdevicemode.h>
#include "stubbase.h"
/*
 * XXX: This stub contains only those methods which are 
 * used by the BatteryBusinessLogic class ...
 */

/*
 * Declare stub
 */
class QmDeviceModeStub : public StubBase
{
public:
    virtual Maemo::QmDeviceMode::PSMState getPSMState (); 
    virtual bool setMode (Maemo::QmDeviceMode::DeviceMode mode);
    virtual Maemo::QmDeviceMode::DeviceMode getMode (); 
    virtual void setPSMState (Maemo::QmDeviceMode::PSMState state);
    virtual int getPSMBatteryMode ();
    virtual void setPSMBatteryMode (int percentages);
};

bool
QmDeviceModeStub::setMode (Maemo::QmDeviceMode::DeviceMode mode)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<Maemo::QmDeviceMode::DeviceMode> (mode));
    stubMethodEntered ("setMode", params);
    return true;
}
Maemo::QmDeviceMode::DeviceMode
QmDeviceModeStub::getMode ()
{
    stubMethodEntered ("getMode");
    return stubReturnValue<Maemo::QmDeviceMode::DeviceMode> ("getMode");
}

Maemo::QmDeviceMode::PSMState
QmDeviceModeStub::getPSMState ()
{
    stubMethodEntered ("getPSMState");
    return stubReturnValue<Maemo::QmDeviceMode::PSMState> ("getPSMState");
}

void
QmDeviceModeStub::setPSMState (Maemo::QmDeviceMode::PSMState state)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<Maemo::QmDeviceMode::PSMState> (state));
    stubMethodEntered ("setPSMState", params);
}

int
QmDeviceModeStub::getPSMBatteryMode ()
{
    stubMethodEntered ("getPSMBatteryMode");
    return stubReturnValue<int> ("getPSMBatteryMode");
}

void
QmDeviceModeStub::setPSMBatteryMode (int percentages)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<int> (percentages));
    stubMethodEntered ("setPSMBatteryMode", params);
}

/*
 * Create a stub instance
 */
QmDeviceModeStub gDefaultQmDeviceModeStub;
QmDeviceModeStub *gQmDeviceModeStub = &gDefaultQmDeviceModeStub;

/*
 * Create a proxy which calls the stub
 */
namespace Maemo
{

QmDeviceMode::QmDeviceMode (QObject *parent)
{
    Q_UNUSED (parent);
    /*
     * Do nothing, we don't want to instantiate a real QmDeviceMode...
     */
}

QmDeviceMode::~QmDeviceMode ()
{

}

QmDeviceMode::PSMState
QmDeviceMode::getPSMState () const
{
    return gQmDeviceModeStub->getPSMState ();
}

bool
QmDeviceMode::setPSMState (Maemo::QmDeviceMode::PSMState state)
{
    gQmDeviceModeStub->setPSMState (state);
    return true;
}

int
QmDeviceMode::getPSMBatteryMode ()
{
    return gQmDeviceModeStub->getPSMBatteryMode ();
}

bool
QmDeviceMode::setPSMBatteryMode (int percentages)
{
    gQmDeviceModeStub->setPSMBatteryMode (percentages);
    return true;
}

QmDeviceMode::DeviceMode QmDeviceMode::getMode() const{
    return gQmDeviceModeStub->getMode ();
}

bool QmDeviceMode::setMode(QmDeviceMode::DeviceMode mode){
    return gQmDeviceModeStub->setMode (mode);
}
} /* namespace Maemo */

#endif
