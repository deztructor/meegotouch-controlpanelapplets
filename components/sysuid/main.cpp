#include "sysuid.h"

#include <QTimer>
#include <QDebug>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>

#include <QObject>

#define DEBUG
#include "../debug.h"

#include "signal.h"

static DuiApplication *exitPtr;

void sysuid_exit (int sig)
{
    Q_UNUSED (sig);
    if (exitPtr) {
        exitPtr->quit ();
        exitPtr = NULL;
    }
}

int main (int argc, char** argv)
{
    DuiApplication app (argc, argv);
    exitPtr = &app;

    SYS_DEBUG ("- System-UI start");

    DuiApplicationWindow win;
    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    win.setWindowOpacity (0);
    win.setWindowFlags (flags);

    // qDebug and qWarning shows nothing without this:
    // FIXME: Use SYS_DEBUG, SYS_WARNING instead of q...
    //        and then remove this call
    qInstallMsgHandler (0);

    signal (SIGINT, sysuid_exit);

    Sysuid daemon;

    SYS_DEBUG ("- System-UI daemon initialized");

    // re-install the translations on locale settings changed signal
    QObject::connect (&app, SIGNAL (localeSettingsChanged ()),
                      &daemon, SLOT (retranslate ()));

    int ret = app.exec ();

    return ret;
}

