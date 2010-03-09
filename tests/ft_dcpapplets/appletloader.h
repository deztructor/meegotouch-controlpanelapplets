#ifndef APPLET_LOADER_H
#define APPLET_LOADER_H

#include <QObject>
#include <QtTest/QtTest>
// From DuiControlPanel
#include <duiapplication.h>

class Ft_AppletLoader : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();
    // The testcases
    void testbatteryapplet ();
    void testdisplayapplet ();
    void testledapplet ();
    void testprofileapplet ();
    void testusbapplet ();


private:
    DuiApplication      *m_app;

    void    DoAppletTest (const char *desktopfile);
};

#endif
