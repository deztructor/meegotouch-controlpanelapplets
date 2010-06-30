/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_RESETAPPLET_H
#define UT_RESETAPPLET_H

#include <QtTest/QtTest>
#include <QObject>

class ResetApplet;
class MApplication;

class Ut_ResetApplet : public QObject 
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
    void testResetBusinessLogic ();
private:
    MApplication              *m_App;
    ResetApplet               *m_Applet;
};

#endif


