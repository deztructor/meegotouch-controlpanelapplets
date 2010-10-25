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

#include "appletloader.h"

#include <QTest>
#include <dcpappletif.h>
#include <dcpbrief.h>
#include <dcpwidget.h>

#define APPLET_PATH "/usr/lib/duicontrolpanel/applets/"

#undef TEST_NEW_TITLE_METHODS

void
Ft_AppletLoader::initTestCase ()
{
    // Instantiate the classes what we want to test
}

void
Ft_AppletLoader::cleanupTestCase ()
{
    // Drop the (tested) objects
}

static int argc = 1;
static char *argv[] =
    {
        (char *) "./ft_dcpapplets",
        NULL
    };

void
Ft_AppletLoader::init ()
{
    m_app = new MApplication (argc, argv);
    m_window = new MApplicationWindow;
}

void
Ft_AppletLoader::cleanup ()
{
    delete m_app;
}

/*!
 * This function will do the basic loading/unloading tests on the applet.
 */
void
Ft_AppletLoader::DoAppletTest (const char *soname, bool hasBrief)
{
    QPluginLoader loader;

    loader.setFileName (QString (APPLET_PATH) + QString (soname));
    loader.setLoadHints (QLibrary::ResolveAllSymbolsHint);

    /*
     * Getting the DcpAppletIf applet interface object. Checking if the applet
     * loaded, the type of the applet interface is right.
     */
    QVERIFY2 (loader.load (), qPrintable (loader.errorString ()));

    DcpAppletIf *applet = qobject_cast<DcpAppletIf *>(loader.instance ());

    QVERIFY (applet != 0);

    /*
     * Do initialization... in the same way how controlpanel doing:
     */
    applet->init ();

#ifndef TEST_NEW_TITLE_METHODS
    QVERIFY (! applet->title ().isEmpty ());
#endif

    /*
     * Checking if the applet brief is constructed.
     */
    DcpBrief *brief = applet->constructBrief ();
    if (hasBrief)
    {
        QVERIFY2(brief, 
		    "Error when creating brief widget");
#ifdef TEST_NEW_TITLE_METHODS
        /*
         * Checking if the applet has a non-empty title string.
         */
        QVERIFY (! brief->titleText ().isEmpty ());
#endif
    }
    else
        QVERIFY2(!brief, 
		    "This applet should not have a Brief");

    /*
     * Checking if the the main view (the applet widget) is constructed. FIXME:
     * We should call the getMainWidgetId here, but I'm not sure how it is done
     * after the refactoring.
     */
    DcpWidget *widget = applet->constructWidget (0);
    QVERIFY2 (widget, "Error when creating applet's main widget");

#ifdef TEST_NEW_TITLE_METHODS
    /*
     * Check if the applet widget has a non-empty title string.
     */
    QVERIFY (! widget->title ().isEmpty ());
#endif

    QTest::qWait (1000);

//    loader.unload ();
//    delete widget;
//    delete brief;
}

void
Ft_AppletLoader::testbatteryapplet ()
{
    DoAppletTest ("libbatteryapplet.so", true);
}

void
Ft_AppletLoader::testdisplayapplet ()
{
    DoAppletTest ("libdisplayapplet.so", true);
}

void
Ft_AppletLoader::testprofileapplet ()
{
    DoAppletTest ("libprofileapplet.so", false);
}

void
Ft_AppletLoader::testusbapplet ()
{
    DoAppletTest ("libusbapplet.so", true);
}

void
Ft_AppletLoader::testresetapplet ()
{
    DoAppletTest ("libresetapplet.so", true);
}

void
Ft_AppletLoader::testaboutapplet ()
{
    DoAppletTest ("libaboutapplet.so", true);
}

void
Ft_AppletLoader::testthemeapplet ()
{
    DoAppletTest ("libthemeapplet.so", true);
}

void
Ft_AppletLoader::testwallpaperapplet ()
{
    DoAppletTest ("libwallpaperapplet.so", true);
}

void
Ft_AppletLoader::testwarrantyapplet ()
{
    DoAppletTest ("libwarrantyapplet.so", true);
}

QTEST_APPLESS_MAIN(Ft_AppletLoader)
