/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QStringList>
#include  "ut_qgconfvalue.h"
#include  "qgconfvalue.h"

#define DEBUG
#define WARNING
#include "debug.h"

static const QString testKey = "/meegotouch/input_feedback/volume/pulse";

/******************************************************************************
 * GConf library stub. We are not going to touch the GConf database, this is a
 * unit test.
 */
#include <gconf/gconf-client.h>

static QString lastGConfKey;
static QVariant lastGConfValue;

gboolean
gconf_client_set_bool (
        GConfClient *client, 
        const gchar *key,
        gboolean val, 
        GError** err)
{
    lastGConfKey = key;
    lastGConfValue = val;
    return true;
}

gboolean
gconf_client_set_string (
        GConfClient* client, 
        const gchar* key,
        const gchar* val, 
        GError** err)
{
    lastGConfKey = key;
    lastGConfValue = val;
    return true;
}

gboolean
gconf_client_set_int (
        GConfClient* client, 
        const gchar* key,
        gint val, 
        GError** err)
{
    lastGConfKey = key;
    lastGConfValue = val;
    return true;
}

gboolean
gconf_client_set_float (
        GConfClient* client, 
        const gchar* key,
        gdouble val, 
        GError** err)
{
    lastGConfKey = key;
    lastGConfValue = val;
    return true;
}

/******************************************************************************
 * Ut_QGConfValue implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_qgconfvalue",
      NULL };

void
Ut_QGConfValueTests::initTestCase()
{
      m_App = new MApplication(argc, argv);
      m_SignalFlag = false;
}

void
Ut_QGConfValueTests::cleanupTestCase()
{
      if (m_App)
      delete m_App;
}

/*!
 * This function will test the constructor if the QGConfValue class, will
 * check if the m_lsDir private tag is properly initialized.
 */
void
Ut_QGConfValueTests::qgconfvalueConstructor ()
{
      MApplicationWindow aw;
      QGConfValue  qgcv(testKey);

      QVERIFY(qgcv.m_notifyId != 0 );

      QCOMPARE(qgcv.m_lsDir[0], QString("meegotouch"));
      QCOMPARE(qgcv.m_lsDir[1], QString("input_feedback"));
      QCOMPARE(qgcv.m_lsDir[2], QString("volume"));
}

/*!
 * This test will check if the test object has SIGNAL(changed()), will call the 
 * QGConfValue::notifyValue() static function to see if the signal is emitted,
 * and the value in the test object is in invalid state after.
 */
void
Ut_QGConfValueTests::qgconfvalueNotifyValue ()
{
    MApplicationWindow   aw;
    QGConfValue          qgcv(testKey);
    bool                 success;

    success = connect (
            &qgcv, SIGNAL(changed()), 
            this, SLOT(testIfNotify()));
    QVERIFY (success);

    SYS_DEBUG ("1");
    m_SignalFlag = false;

    // This is the static method we are checking, it does not use the first
    // three parameters.
    QGConfValue::notifyValue (NULL, 0, 0, &qgcv);
    QVERIFY (m_SignalFlag);
    QCOMPARE ((int)qgcv.m_val.type(), (int) QVariant::Invalid);
}

/*
 * This is not a test (but certainly considered as a test) but a function to
 * test if a signal is emitted.
 */
void
Ut_QGConfValueTests::testIfNotify ()
{
    SYS_DEBUG ("");
    m_SignalFlag = true;
}

void Ut_QGConfValueTests::qgconfvalueRealSetValue_data()
{
      QTest::addColumn<QVariant>("value");
      QTest::newRow("") << QVariant(true);
      QTest::newRow("") << QVariant(false);
      QTest::newRow("") << QVariant(3.2);
      QTest::newRow("") << QVariant(2);
      QTest::newRow("") << QVariant("test string");
}

/*!
 * This test will call the QGConfValue::realSetValue() and check if the value
 * has been set. Using a stubbed version of the GConf library the real database
 * will not be modified.
 */
void
Ut_QGConfValueTests::qgconfvalueRealSetValue ()
{
    QFETCH(QVariant, value);
    QGConfValue  qgcv(testKey);
    qgcv.realSetValue(value);

    SYS_DEBUG ("*** value.type() = %d", (int)value.type());

    /*
     * Floating point nummbers are never equal. :-(
     */
    if (value.type() == QVariant::Double) {
        double  expected = value.toDouble();
        double  wehave = lastGConfValue.toDouble();
        QVERIFY (wehave < expected + 0.1);
        QVERIFY (wehave > expected - 0.1);
    } else {
        QCOMPARE (lastGConfKey, testKey);
        QCOMPARE (lastGConfValue, value);
    }
}

QTEST_APPLESS_MAIN(Ut_QGConfValueTests)
