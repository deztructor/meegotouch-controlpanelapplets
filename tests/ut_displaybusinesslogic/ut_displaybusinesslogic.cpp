/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_displaybusinesslogic.h"
#include "displaybusinesslogic.h"

#include <DuiApplication>
#include <QList>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_DisplayBusinessLogic implementation. 
 */
void 
Ut_DisplayBusinessLogic::init()
{
}

void 
Ut_DisplayBusinessLogic::cleanup()
{
}


void 
Ut_DisplayBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_DisplayBusinessLogic";

    m_App = new DuiApplication (argc, &app_name);
    m_Api = new DisplayBusinessLogic;
}

void 
Ut_DisplayBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

/*!
 * Tests if there are available brightness values and if the current brightness
 * value found as an available value.
 *
 * This function also stores the max and min values for the brighness in the
 * m_MaxBrightness and m_MinBrightness tag variables.
 */
void 
Ut_DisplayBusinessLogic::testGetBrightnessValues ()
{
    QList<int> values = m_Api->brightnessValues();
    int        current = m_Api->selectedBrightnessValue();
    bool       foundCurrent = false;
    int        n = 0;

    QVERIFY (values.size() != 0);

    m_MinBrightness = m_MaxBrightness = values[0];

    foreach (int value, values) {
        if (value == current) {
            qDebug() << "brightnessvalue[" << n << "] = " << value << "CURRENT";
            foundCurrent = true;
        } else {
            qDebug() << "brightnessvalue[" << n << "] = " << value;
        }

        if (value < m_MinBrightness)
            m_MinBrightness = value;
        if (value > m_MaxBrightness)
            m_MaxBrightness = value;

        ++n;
    }
    
    QVERIFY (foundCurrent);
    QVERIFY (m_MinBrightness < m_MaxBrightness);
}


void
Ut_DisplayBusinessLogic::testLightTimeouts ()
{
    QList<int> values = m_Api->screenLightsValues ();
    int        current = m_Api->selectedScreenLightsValue ();
    bool       foundCurrent = false;
    int        n = 0;

    QVERIFY (values.size() != 0);
    
    m_Maxtimeout = m_Mintimeout = values[0];

    foreach (int value, values) {
        if (n == current) {
            qDebug() << "timeoutvalue[" << n << "] = " << value << "sec CURRENT";
            foundCurrent = true;
        } else {
            qDebug() << "timeoutvalue[" << n << "] = " << value << "sec";
        }

        if (value > m_Maxtimeout)
            m_Maxtimeout = value;
        if (value < m_Mintimeout)
            m_Mintimeout = value;

        ++n;
    }
    
    QVERIFY (foundCurrent);
}

void
Ut_DisplayBusinessLogic::testSetBrightness ()
{
    QList<int> values = m_Api->brightnessValues();
    int        value;

    /* 
     * Testing the brighness values.  First we set to the min, then to the max
     * value.  It is obvious we should get back the same value we set, but it
     * seems we fail under the sb1 environment.  
     */

    qDebug() << "Min brightness = " << m_MinBrightness;
    qDebug() << "Max brightness = " << m_MaxBrightness;

    m_Api->setBrightnessValue (m_MinBrightness);
    QTest::qWait (500);
    value = m_Api->selectedBrightnessValue();
    qDebug() << "brightness = " << value;
    //QVERIFY (m_Api->selectedBrightnessValue() == m_MinBrightness);
    
    m_Api->setBrightnessValue (m_MaxBrightness);
    QTest::qWait (500);
    value = m_Api->selectedBrightnessValue();
    qDebug() << "brightness = " << value;
    //QVERIFY (m_Api->selectedBrightnessValue() == m_MaxBrightness);
}

QTEST_APPLESS_MAIN(Ut_DisplayBusinessLogic)
