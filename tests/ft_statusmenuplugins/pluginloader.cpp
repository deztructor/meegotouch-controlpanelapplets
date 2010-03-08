#include "appletloader.h"
#include <QTest>
#include <DuiWidget>
#include <DuiStatusIndicatorMenuPluginInterface>

#define LIBDIR "/usr/lib/duistatusindicatormenu/plugins/"

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
    // Init QT/DUI
    int argc = 1;
    char* app_name = (char*) "./ft_statusmenuplugins";

    m_app = new DuiApplication (argc, &app_name);
}

void
Ft_PluginLoader::cleanupTestCase ()
{
    delete m_app;
}

void
Ft_PluginLoader::DoAppletTest (const QString &soname)
{
    QPluginLoader  loader (QString (LIBDIR) + soname);
    QObject       *object = loader.instance ();
    DuiWidget     *widget = 0;

    DuiStatusIndicatorMenuInterface StatusMenuStub;

    DuiStatusIndicatorMenuPluginInterface* plugin =
        qobject_cast<DuiStatusIndicatorMenuPluginInterface *> (object);

    QVERIFY(plugin);

    QVERIFY2(plugin->constructWidget (StatusMenuStub),
             "Error at plugin widget construction");

    delete object;
}

void
Ft_PluginLoader::testbatteryplugin ()
{
    DoPluginTest ("libbattery.so");
}

void
Ft_PluginLoader::testprofileplugin ()
{
    DoPluginTest ("libprofile.so");
}

void
Ft_PluginLoader::testvolumeplugin ()
{
    DoPluginTest ("libvolume.so");
}

QTEST_APPLESS_MAIN(Ft_PluginLoader)
