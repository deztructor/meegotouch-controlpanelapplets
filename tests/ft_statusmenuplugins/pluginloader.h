#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QtTest/QtTest>
#include <mapplication.h>
#include <mstatusindicatormenuplugininterface.h>

class MApplicationWindow;

class Ft_PluginLoader : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();
    // The testcases

    void testprofileplugin ();
//    void testvolumeplugin ();

private:
    MApplication                    *m_app;
    MApplicationWindow              *m_window;
    MStatusIndicatorMenuInterface   *m_smstub;

    void    DoPluginTest (const QString &soname);
};

#endif
