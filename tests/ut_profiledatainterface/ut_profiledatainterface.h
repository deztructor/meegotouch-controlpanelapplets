/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_PROFILEDATAINTERFACE_H
#define UT_PROFILEDATAINTERFACE_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class ProfileDataInterface;

class Ut_ProfileDataInterface : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testGetProfilesData ();
    void testSetGetProfile ();
    void testSetVibration ();

private:
    MApplication       *m_App;
    ProfileDataInterface *m_Api;
};

#endif
