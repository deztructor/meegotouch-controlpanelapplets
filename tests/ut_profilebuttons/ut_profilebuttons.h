/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_PROFILEBUTTONS_H
#define UT_PROFILEBUTTONS_H

#include <QtTest/QtTest>
#include <QObject>

class ProfileButtons;
class MApplication;

class Ut_ProfileButtons : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

private:
    MApplication              *m_App;
    ProfileButtons            *m_Buttons;
};

#endif


