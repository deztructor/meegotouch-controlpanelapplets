/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "qmdevicemode.h"

#define DEBUG
#define WARNING
#include "../../src/debug.h"

using namespace Maemo;

QmDeviceMode::QmDeviceMode (
        QObject *parent) :
    QObject (parent),
    m_PSMSTate (PSMStateOff), 
    m_PSMBatteryMode (-1)
{
    SYS_DEBUG ("Creating %p", this);
}

QmDeviceMode::~QmDeviceMode ()
{
    SYS_DEBUG ("Destroying %p", this);
}


QmDeviceMode::PSMState
QmDeviceMode::getPSMState () const
{
    SYS_DEBUG ("returning %s", SYS_STR(PSMStateName(m_PSMSTate)));
    return m_PSMSTate;
}

bool
QmDeviceMode::setPSMState (
        PSMState state)
{
    SYS_DEBUG ("Setting state %s -> %s",
            SYS_STR(PSMStateName(m_PSMSTate)),
            SYS_STR(PSMStateName(state)));

    if (m_PSMSTate == state)
        return false;

    m_PSMSTate = state;

    SYS_DEBUG ("Emitting devicePSMStateChanged(%s)", 
            SYS_STR(PSMStateName(state)));
    emit devicePSMStateChanged (state);

    return true;
}

int
QmDeviceMode::getPSMBatteryMode()
{
    return m_PSMBatteryMode;
}

bool
QmDeviceMode::setPSMBatteryMode (
        int percentages)
{
    m_PSMBatteryMode = percentages;
    return true;
}

/******************************************************************************
 * Low level functions to help the testing. These are not in the original!
 */
QString
QmDeviceMode::PSMStateName (
        PSMState state) const
{
    switch (state) {
        case PSMError:
            return "QmDeviceMode::PSMError";
        case PSMStateOff:
            return "QmDeviceMode::PSMStateOff";
        case PSMStateOn:
            return "QmDeviceMode::PSMStateOn";
    }

    return "NOSUCHASTATE";
}
