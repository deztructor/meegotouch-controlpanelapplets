#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QtTest/QtTest>
// From libDUI
#include <duiapplication.h>
// From Dui Status Indicator Menu (system-ui)
#include <duistatusindicatormenuplugininterface.h>


class Ft_PluginLoader : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();
    // The testcases
    void testbatteryplugin ();
    void testprofileplugin ();
    void testvolumeplugin ();

private:
    DuiApplication                      *m_app;
    DuiStatusIndicatorMenuInterface     *m_smstub;

    void    DoPluginTest (const QString &soname);
};

#endif
