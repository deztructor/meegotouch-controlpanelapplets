/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_OFFLINEAPPLET_H
#define UT_OFFLINEAPPLET_H

#include <QtTest/QtTest>
#include <QObject>

class OfflineApplet;
class MApplication;

class Ut_OfflineApplet : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testTitle ();
    void testMenu ();
    void testConstructWidget ();
    void testBriefConstruct();
    void testCurrentText ();
    void testBriefInit ();
    void testBriefValueText ();
    void testBriefSetToggle ();
    void testProcessDialogResult ();

private:
    OfflineApplet             *m_Applet;
    MApplication              *m_App;
};

#endif

