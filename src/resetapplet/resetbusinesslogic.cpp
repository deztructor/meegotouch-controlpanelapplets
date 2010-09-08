/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetbusinesslogic.h"
#include <QString>
#include <MGConfItem>

#include <QVariant>
#include <QDBusInterface>
#include <QDBusMessage>
#ifdef HAVE_DEVICELOCK
#include <devicelock/devicelock.h>
#endif

#define DEBUG
#include "../debug.h"

ResetBusinessLogic::ResetBusinessLogic() :
    m_devlock (0)
{
#ifdef HAVE_DEVICELOCK
    m_devlock =
        new QDBusInterface (QString (DEVLOCK_SERVICE),
                            QString (DEVLOCK_PATH),
                            QString ("com.nokia.devicelock"),
                            QDBusConnection::systemBus ());
    connect (m_devlock, SIGNAL (passwordPromptResult (bool)),
             this, SLOT (passwordResult (bool)));
#endif
}

ResetBusinessLogic::~ResetBusinessLogic()
{
    delete m_devlock;
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

/*!
 * Asks user for device-lock code when it is set
 */
void
ResetBusinessLogic::getAccess ()
{
    bool success = false;

#ifdef HAVE_DEVICELOCK
    QDBusMessage message;
    message = m_devlock->call (QDBus::Block, 
                               QString ("setState"),
                               DeviceLock::DeviceLockEnums::Device,
                               DeviceLock::DeviceLockEnums::PasswordPrompt);

    QString errorString = message.errorMessage();
    QString errorName = message.errorName ();
    SYS_DEBUG ("*** errorMessage = %s", SYS_STR(errorString));
    SYS_DEBUG ("*** errorName    = %s", SYS_STR(errorName));

    /*
     * For now i don't care about errors, i assume when some
     * error happening means there is no device-lock daemon
     * running on the device...
     */
    success = message.arguments().at(0).toBool ();
    SYS_DEBUG ("*** reply = %s", SYS_BOOL (success));
#endif

    /*
     * No password needed [ie. device-lock is disabled]
     */
    if (success == false)
        emit gotAccess ();
}

void
ResetBusinessLogic::passwordResult (bool result)
{
    SYS_DEBUG ("*** result = %s", SYS_BOOL (result));
    if (result == true)
        emit gotAccess ();
}

