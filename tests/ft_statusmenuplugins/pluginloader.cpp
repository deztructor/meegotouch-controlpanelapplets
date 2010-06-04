/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "pluginloader.h"
#include <QTest>
#include <MWidget>
#include <MApplicationWindow>
#include "StatusIndicatorMenuStub.h"

using namespace QTest;

#define LIBDIR "/usr/lib/meegotouch/statusindicatormenuplugins/"

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

    MStatusIndicatorMenuPluginInterface* plugin =
        qobject_cast<MStatusIndicatorMenuPluginInterface *> (object);

    QVERIFY(plugin);

    qWait (150);

    QVERIFY2(plugin->constructWidget (*m_smstub),
             "Error at plugin widget construction");

    qWait (150);

    delete object;
}

void
Ft_PluginLoader::testprofileplugin ()
{
    DoPluginTest ("libprofile.so");
}

/*
 * FIXME: This test is crashing in a weird way:
(gdb) back
#0  0x4112cd40 in strlen () from /lib/libc.so.6
#1  0x41219920 in XSetCommand () from /usr/lib/libX11.so.6
#2  0x4121e488 in XSetWMProperties () from /usr/lib/libX11.so.6
#3  0x405a1cc0 in QWidgetPrivate::create_sys (this=0x172ac8, 
    window=<value optimized out>, initializeWindow=true, destroyOldWindow=true)
    at kernel/qwidget_x11.cpp:818
#4  0x4055899c in QWidget::create (this=0x172aa8, window=0, 
    initializeWindow=true, destroyOldWindow=true) at kernel/qwidget.cpp:1323
#5  0x40554470 in QWidgetPrivate::createWinId (this=<value optimized out>, 
    winid=0) at kernel/qwidget.cpp:2343
#6  0x405590e8 in QWidget::winId (this=0x172aa8) at kernel/qwidget.cpp:2306
#7  0x4019ce40 in MWindowPrivate::appendVisibilityChangeMask (
    this=<value optimized out>)
    at /home/bifh2/harmattan-arm-unreleased.cs2009q1/work/libmeegotouch-0.20.2/src/corelib/widgets/mwindow.cpp:150
#8  0x4019dc98 in MWindowPrivate::init (this=0x172860)
    at /home/bifh2/harmattan-arm-unreleased.cs2009q1/work/libmeegotouch-0.20.2/src/corelib/widgets/mwindow.cpp:126
#9  0x4019e688 in MWindow (this=0x172aa8, sceneManager=0x16fef8, 
    parent=<value optimized out>)
    at /home/bifh2/harmattan-arm-unreleased.cs2009q1/work/libmeegotouch-0.20.2/src/corelib/widgets/mwindow.cpp:308
#10 0x433474ec in VolumeOverlay::constructUi() ()
---Type <return> to continue, or q <return> to quit---
   from /usr/lib/meegotouch/statusindicatormenuplugins/libvolume.so
#11 0x43347854 in VolumeOverlay::VolumeOverlay(QGraphicsItem*) ()
   from /usr/lib/meegotouch/statusindicatormenuplugins/libvolume.so
#12 0x43348138 in VolumeBar::VolumeBar(MStatusIndicatorMenuInterface&, QGraphicsItem*) () from /usr/lib/meegotouch/statusindicatormenuplugins/libvolume.so
#13 0x433464f4 in VolumeBarPlugin::constructWidget(MStatusIndicatorMenuInterface&) () from /usr/lib/meegotouch/statusindicatormenuplugins/libvolume.so
#14 0x0000c0cc in ?? ()
#15 0x0000c0cc in ?? ()
Backtrace stopped: previous frame identical to this frame (corrupt stack?)

void
Ft_PluginLoader::testvolumeplugin ()
{
    DoPluginTest ("libvolume.so");
}
*/

QTEST_APPLESS_MAIN(Ft_PluginLoader)
