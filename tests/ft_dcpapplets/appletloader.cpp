#include "appletloader.h"
#include <QTest>
#include <dcpappletif.h>
#include <dcpappletmetadata.h>
#include <dcpbrief.h>
#include <dcpwidget.h>

#define DESKTOP_PATH "/usr/lib/duicontrolpanel/"

void
Ft_AppletLoader::init ()
{
    // Instantiate the classes what we want to test
}

void
Ft_AppletLoader::cleanup ()
{
    // Drop the (tested) objects
}

void
Ft_AppletLoader::initTestCase ()
{
    // Init QT/DUI
    int argc = 1;
    char* app_name = (char*) "./ft_dcpapplets";

    m_app = new DuiApplication (argc, &app_name);
}

void
Ft_AppletLoader::cleanupTestCase ()
{
    delete m_app;
}

void
Ft_AppletLoader::DoAppletTest (const char *desktopfile)
{
    DcpAppletMetadata metadata (QString (DESKTOP_PATH) +
                                QString (desktopfile));
    DcpAppletLoader loader (&metadata);

    DcpAppletIf *applet = loader.applet ();

    //Applet loading
    QVERIFY2(applet, qPrintable (loader.errorMsg ()));

    applet->init ();

    // Applet title test
    QVERIFY(! applet->title ().isEmpty ());

    // Constructing brief
    QVERIFY2(applet->constructBrief (), "Error when creating brief widget");

    // Constructing widget
    QVERIFY2(applet->constructWidget (0), "Error when creating applet's main widget");
}

void
Ft_AppletLoader::testbatteryapplet ()
{
    DoAppletTest ("battery.desktop");
}

void
Ft_AppletLoader::testdisplayapplet ()
{
    DoAppletTest ("display.desktop");
}

void
Ft_AppletLoader::testledapplet ()
{
    DoAppletTest ("ledapplet.desktop");
}

void
Ft_AppletLoader::testprofileapplet ()
{
    DoAppletTest ("profile.desktop");
}

void
Ft_AppletLoader::testusbapplet ()
{
    DoAppletTest ("usbapplet.desktop");
}

QTEST_APPLESS_MAIN(Ft_AppletLoader)
