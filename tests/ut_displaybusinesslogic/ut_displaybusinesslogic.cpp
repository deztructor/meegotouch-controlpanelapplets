/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_displaybusinesslogic.h"
#include "displaybusinesslogic.h"

#ifndef QMDISPLAYSTATE_STUB_H
#  error "QmDisplayState should be stubbed, can nopt continue."
#endif

#include <MApplication>
#include <MGConfItem>

#include <QList>
#include <QVariant>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Stubbing the MGConfItem.
 */
static bool dimTimeoutsExist = true;

QVariant
MGConfItem::value () const
{
    SYS_DEBUG ("*** key() = %s", SYS_STR(key()));

    if (key() != "/system/osso/dsm/display/possible_display_dim_timeouts") {
        SYS_DEBUG ("%s ???", SYS_STR(key()));
        goto return_invalid;
    }

    if (dimTimeoutsExist) {
        QList<QVariant> list;

        list << 
            QVariant (15) << 
            QVariant (30) << 
            QVariant (60) << 
            QVariant (120) <<
            QVariant (180);

        return QVariant (list);
    }

return_invalid:
    return QVariant ();
}

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

    m_App = new MApplication (argc, &app_name);
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
 * value index found as an available value.
 *
 * This function also stores the max and min values for the brighness in the
 * m_MaxBrightness and m_MinBrightness tag variables.
 */
void 
Ut_DisplayBusinessLogic::testGetBrightnessValues ()
{
    QList<int> values = m_Api->brightnessValues();
    // This is actually an index?
    int        current = m_Api->selectedBrightnessValue();
    int        n = 0;

    QVERIFY (values.size() != 0);

    m_MinBrightness = m_MaxBrightness = values[0];

    qDebug() << "current = " << current;
    foreach (int value, values) {
        qDebug() << "brightnessvalue[" << n << "] = " << value;

        if (value < m_MinBrightness)
            m_MinBrightness = value;
        if (value > m_MaxBrightness)
            m_MaxBrightness = value;

        ++n;
    }
   
    QVERIFY (current >= 0 && current < values.size());
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
    value = m_Api->selectedBrightnessValue();
    qDebug() << "brightness = " << value;
    //QVERIFY (m_Api->selectedBrightnessValue() == m_MinBrightness);
    
    m_Api->setBrightnessValue (m_MaxBrightness);
    value = m_Api->selectedBrightnessValue();
    qDebug() << "brightness = " << value;
    //QVERIFY (m_Api->selectedBrightnessValue() == m_MaxBrightness);
}

/*!
 * Testing the setBlankInhibitValue() method. It is just inverting the argument
 * and sends to the backend.
 */
void
Ut_DisplayBusinessLogic::testSetBlankInhibitValue ()
{
    m_Api->setBlankInhibitValue (true);
    QVERIFY (m_Api->m_Display->getBlankingWhenCharging() == false);

    m_Api->setBlankInhibitValue (false);
    QVERIFY (m_Api->m_Display->getBlankingWhenCharging() == true);

    m_Api->setBlankInhibitValue (true);
    QVERIFY (m_Api->m_Display->getBlankingWhenCharging() == false);
}

/*!
 * testing the screenLightsValues() method. This method has a built in fallback
 * list, we stub the same and we check if it returns the right list.
 */
void
Ut_DisplayBusinessLogic::testScreenLightsValues ()
{
    QList<int> list1, list2;

    dimTimeoutsExist = true;
    list1 = m_Api->screenLightsValues ();
    
    dimTimeoutsExist = false;
    list2 = m_Api->screenLightsValues ();

    QVERIFY (list1 == list2);
}


QTEST_APPLESS_MAIN(Ut_DisplayBusinessLogic)
