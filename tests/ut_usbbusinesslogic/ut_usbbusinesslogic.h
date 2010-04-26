/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef UT_USBBUSINESSLOGIC_H
#define UT_USBBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class UsbSettingsLogic;

class Ut_UsbSettingsLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testValidSaveLoadConfig ();
    void testInvalidSaveLoadConfig ();
    void testUsbModedRelation ();
    
private:
    MApplication        *m_App;
    UsbSettingsLogic    *m_Api;
};

#endif
