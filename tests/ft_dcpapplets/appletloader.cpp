/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "appletloader.h"
#include <QTest>
#include <dcpappletif.h>
#include <dcpappletmetadata.h>
#include <dcpbrief.h>
#include <dcpwidget.h>

#ifndef DCP_APPLET_LOADER_DEPRECATED
#include <dcpappletloader.h>
#else
#include <dcpappletplugin.h>
#endif

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
    int argc = 1;
    char *argv[] = {
        (char *) "./ft_dcpapplets",
        NULL
    };

    m_app = new DuiApplication (argc, argv);
}

void
Ft_AppletLoader::cleanupTestCase ()
{
    delete m_app;
}

/*!
 * This function will do the basic loading/unloading tests on the applet variant
 * described in the deaktop file.
 */
void
Ft_AppletLoader::DoAppletTest (const char *desktopfile)
{
    DcpAppletMetadata metadata(QString (DESKTOP_PATH) + QString (desktopfile));

#ifndef DCP_APPLET_LOADER_DEPRECATED
    DcpAppletLoader loader (&metadata);
#else
    DcpAppletPlugin loader (&metadata);
#endif

    /*
     * Getting the DcpAppletIf applet interface object. Checking if the applet
     * loaded, the type of the applet interface is right.
     */
    DcpAppletIf *applet = loader.applet ();
    QVERIFY2(applet, qPrintable (loader.errorMsg ()));

    /*
     * Checking if the applet has a non-empty title string.
     */
    QVERIFY(! applet->title ().isEmpty ());

    /*
     * Checking if the applet brief is constructed.
     */
    DcpBrief *brief = applet->constructBrief ();
    QVERIFY2(brief, 
		    "Error when creating brief widget");

    /*
     * Checking if the the main view (the applet widget) is constructed. FIXME:
     * We should call the getMainWidgetId here, but I'm not sure how it is done
     * after the refactoring.
     */
    DuiWidget *widget = applet->constructWidget (0);
    QVERIFY2(widget, 
		    "Error when creating applet's main widget");

    delete widget;
    delete brief;
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
