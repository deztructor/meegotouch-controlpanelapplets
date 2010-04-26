/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef APPLET_LOADER_H
#define APPLET_LOADER_H

#include <QObject>
#include <QtTest/QtTest>
#include <mapplication.h>

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
    void testresetapplet ();
    void testaboutapplet ();
    void testthemeapplet ();
    void testwallpaperapplet ();

private:
    MApplication      *m_app;

    void    DoAppletTest (const char *desktopfile);
};

#endif
