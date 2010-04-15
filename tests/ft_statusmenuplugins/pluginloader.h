#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QtTest/QtTest>
#include <mapplication.h>
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
#if 0
    void testbatteryplugin ();
#endif
    void testprofileplugin ();
    void testvolumeplugin ();

private:
    MApplication                      *m_app;
    DuiStatusIndicatorMenuInterface   *m_smstub;

    void    DoPluginTest (const QString &soname);
};

#endif
