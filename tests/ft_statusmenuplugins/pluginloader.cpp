/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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
