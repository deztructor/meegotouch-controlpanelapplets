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
#include "ft_applets.h"

#include <QTest>
#include <QDir>
#include <QTimer>
#include <dcpappletif.h>
#include <dcpbrief.h>
#include <dcpwidget.h>
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#define WARNING
#include "debug.h"

/*
 * Stub QTimer singleShot to avoid random crashing due to
 * quick creation & destroy
 */
void
QTimer::singleShot(int msec, QObject *receiver, const char *member) {
    Q_UNUSED (msec);
    SYS_DEBUG (" ignoring singleshot [ %s, SLOT (%s) ] ",
               SYS_STR (receiver->objectName ()), member);
}

static int argc = 1;
static char *argv[] =
    {
        (char *) "./ft_applets",
        NULL
    };

void
Ft_Applets::initTestCase ()
{
    m_app = new MApplication (argc, argv);
    m_window = new MApplicationWindow;
}

void
Ft_Applets::cleanupTestCase ()
{
    delete m_window;
//    delete m_app;
    m_app->deleteLater ();
}

void
Ft_Applets::init ()
{
}

void
Ft_Applets::cleanup ()
{
}

/*!
 * This function will do the basic loading/unloading tests on the applet.
 */
void
Ft_Applets::doAppletTest (const char *soname, bool hasBrief, bool hasView)
{
    QPluginLoader loader;
    QDir appletPath (APPLET_LIBS);
    QString fileName = appletPath.absoluteFilePath (soname);
    SYS_DEBUG ("fileName = \ˇ%s\"", SYS_STR (fileName));

    loader.setFileName (fileName);
    loader.setLoadHints (QLibrary::ResolveAllSymbolsHint);

    /*
     * Getting the DcpAppletIf applet interface object. Checking if the applet
     * loaded, the type of the applet interface is right.
     */
    QVERIFY2 (loader.load (), qPrintable (loader.fileName() +
                                          loader.errorString ()));


    DcpAppletIf *applet = qobject_cast<DcpAppletIf *>(loader.instance ());

    QVERIFY (applet != 0);

    /*
     * Do initialization... in the same way how controlpanel doing:
     */
    applet->init ();

    /*
     * Checking if the applet brief is constructed.
     */
    DcpBrief *brief = applet->constructBrief ();
    if (hasBrief)
    {
        QVERIFY2(brief, "Error when creating brief widget");
    }
    else
    {
        QVERIFY2(!brief, "This applet should not have a Brief");
    }

    /*
     * Checking if the the main view (the applet widget) is constructed. FIXME:
     * We should call the getMainWidgetId here, but I'm not sure how it is done
     * after the refactoring.
     */
    DcpWidget *widget = applet->constructWidget (0);
    /* TODO:
     * once duicontrolpanel 0.9.7 is out we need to check this:
     * constructStylableWidget
     */

    if (hasView)
    {
        QVERIFY2 (widget, "Error when creating applet's main widget");
    }
    else
    {
        QVERIFY2 (!widget, "Error the applet should not have view!");
    }

    QTest::qWait (100);

    delete brief;
    delete widget;
    loader.unload ();

}

void
Ft_Applets::testbatteryapplet ()
{
    doAppletTest ("libbatteryapplet.so");
}

void
Ft_Applets::testdisplayapplet ()
{
    doAppletTest ("libdisplayapplet.so");
}

void
Ft_Applets::testprofileapplet ()
{
    doAppletTest ("libprofileapplet.so", false);
}

void
Ft_Applets::testusbapplet ()
{
    doAppletTest ("libusbapplet.so");
}

void
Ft_Applets::testresetapplet ()
{
    doAppletTest ("libresetapplet.so");
}

void
Ft_Applets::testaboutapplet ()
{
    doAppletTest ("libaboutapplet.so");
}

void
Ft_Applets::testthemeapplet ()
{
    doAppletTest ("libthemeapplet.so");
}

void
Ft_Applets::testwallpaperapplet ()
{
    doAppletTest ("libwallpaperapplet.so");
}

void
Ft_Applets::testwarrantyapplet ()
{
    doAppletTest ("libwarrantyapplet.so");
}

void
Ft_Applets::testofflineapplet ()
{
    doAppletTest ("libofflineapplet.so", true, false);
}

void
Ft_Applets::testsoundsettingsapplet ()
{
    doAppletTest ("libsoundsettingsapplet.so", false);
}


QTEST_APPLESS_MAIN(Ft_Applets)

