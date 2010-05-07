/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetbusinesslogic.h"
#include <QString>
#include <MGConfItem>

#define DEBUG
#include "../debug.h"

static const QString autolockGConKey = 
    "/system/osso/dsm/locks/devicelock_autolock_timeout";

ResetBusinessLogic::ResetBusinessLogic()
{
    MGConfItem   gconfItem (autolockGConKey);
    QString      value = gconfItem.value().toString();

    /*
     * Well, I'm not sure how to interpret this value, but the 
     * duicontrolpanel-devicelockapplet is not available for my meego target 
     * yet.
     */
    SYS_DEBUG ("*** autolockValue = %s", SYS_STR(value));
}

ResetBusinessLogic::~ResetBusinessLogic()
{
}

/*!
 * Performs the 'Restore original settings' action.
 */
void
ResetBusinessLogic::performRestoreSettings ()
{
    char *command = (char *) "/usr/sbin/clean-device.sh --rfs";
    int   retval;

    SYS_DEBUG ("*** executing command: %s", command);
    retval = system (command);
    if (retval != 0) {
        SYS_WARNING ("The command '%s' failed: %m", command);
    }
}

/*!
 * Performs the 'Clear device' action.
 */
void 
ResetBusinessLogic::performClearData ()
{
    char *command = (char *) "/usr/sbin/clean-device.sh --cud";
    int   retval;

    SYS_DEBUG ("*** executing command: %s", command);
    retval = system (command);
    if (retval != 0) {
        SYS_WARNING ("The command '%s' failed: %m", command);
    }
}

