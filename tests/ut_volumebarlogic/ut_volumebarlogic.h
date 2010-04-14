/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_VOLUMEBARLOGIC_H
#define UT_VOLUMEBARLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class QCoreApplication;
class VolumeBarLogic;

class Ut_VolumeBarLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testVolumeSetGet ();
    void testVolumeChangeByPa ();
    void testSignaling ();
    
private:
    VolumeBarLogic  *m_Api;
};

#endif
