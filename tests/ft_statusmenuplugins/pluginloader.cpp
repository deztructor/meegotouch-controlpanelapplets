#include "pluginloader.h"
#include <QTest>
#include <DuiWidget>
#include "StatusIndicatorMenuStub.h"

using namespace QTest;

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
    m_smstub = new StatusIndicatorMenuStub;
}

void
Ft_PluginLoader::cleanupTestCase ()
{
    delete m_smstub;

    delete m_app;
}

void
Ft_PluginLoader::DoPluginTest (const QString &soname)
{
    QPluginLoader  loader (QString (LIBDIR) + soname);
    QObject       *object = loader.instance ();

    DuiStatusIndicatorMenuPluginInterface* plugin =
        qobject_cast<DuiStatusIndicatorMenuPluginInterface *> (object);

    QVERIFY(plugin);

    qWait (150);

    QVERIFY2(plugin->constructWidget (*m_smstub),
             "Error at plugin widget construction");

    qWait (150);

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
