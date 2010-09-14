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

#include "pluginloader.h"
#include <QTest>
#include <MWidget>
#include <MApplicationWindow>
#include "StatusIndicatorMenuStub.h"

using namespace QTest;

#define LIBDIR "/usr/lib/meegotouch/applicationextensions/"

void
Ft_PluginLoader::init ()
{
    // Instantiate the classes what we want to test
}

void
Ft_PluginLoader::cleanup ()
{
    // Drop the (tested) objects
}

void
Ft_PluginLoader::initTestCase ()
{
    // Init QT/M
    int argc = 1;
    char* app_name = (char*) "./ft_statusmenuplugins";

    m_app = new MApplication (argc, &app_name);
    m_window = new MApplicationWindow;
    m_smstub = new StatusIndicatorMenuStub;
}

void
Ft_PluginLoader::cleanupTestCase ()
{
// FIXME: This causes crash: 'free(): invalid pointer: 0x09b4e43c'
//    delete m_smstub;

    delete m_window;
    delete m_app;
}

void
Ft_PluginLoader::DoPluginTest (const QString &soname)
{
    qDebug() << "Loading applet from " << QString (LIBDIR) + soname;

    QPluginLoader  loader (QString (LIBDIR) + soname);
    QObject       *object = loader.instance ();

    MStatusIndicatorMenuExtensionInterface* plugin =
        qobject_cast<MStatusIndicatorMenuExtensionInterface *> (object);

    QVERIFY(plugin);

    qWait (150);
    QVERIFY2(plugin->initialize ("Habla"),
             "Error at plugin initialization");


    QVERIFY2(plugin->widget (),
             "Error at plugin widget construction");

    qWait (150);

    delete object;
}

void
Ft_PluginLoader::testprofileplugin ()
{
    DoPluginTest ("libstatusindicatormenu-profile.so");
}

QTEST_APPLESS_MAIN(Ft_PluginLoader)
