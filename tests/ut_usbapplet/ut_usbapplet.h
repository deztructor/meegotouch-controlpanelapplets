/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_USBAPPLET_H
#define UT_USBAPPLET_H

#include <QtTest/QtTest>
#include <QObject>

class UsbApplet;
class MApplication;

class Ut_UsbApplet : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testTitle ();
    void testConstructWidget ();
    void testMenuItems ();
    void testConstructbrief ();

private:
    MApplication              *m_App;
    UsbApplet                 *m_Applet;
};

#endif


