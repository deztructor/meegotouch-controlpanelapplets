/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetbusinesslogic.h"

#define DEBUG
#include "../debug.h"

ResetBusinessLogic::ResetBusinessLogic()
{
}

ResetBusinessLogic::~ResetBusinessLogic()
{
}

/*!
 * Performs teh 'Restore original settings' action.
 */
void
ResetBusinessLogic::performRestoreSettings ()
{
    char *command = "/usr/sbin/clean-device.sh --rfs";
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
    char *command = "/usr/sbin/clean-device.sh --cud";
    int   retval;

    SYS_DEBUG ("*** executing command: %s", command);
    retval = system (command);
    if (retval != 0) {
        SYS_WARNING ("The command '%s' failed: %m", command);
    }
}

