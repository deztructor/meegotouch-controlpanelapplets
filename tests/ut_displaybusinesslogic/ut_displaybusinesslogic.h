/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_DISPLAYBUSINESSLOGIC_H
#define UT_DISPLAYBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class DisplayBusinessLogic;

class Ut_DisplayBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testGetBrightnessValues ();
    void testLightTimeouts ();
    void testSetBrightness ();
    
private:
    MApplication       *m_App;
    DisplayBusinessLogic *m_Api;
    int                   m_MaxBrightness, m_MinBrightness;
    int                   m_Maxtimeout, m_Mintimeout;
};

#endif
