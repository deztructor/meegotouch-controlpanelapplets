/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <QString>
#include <QStringList>
#include "ut_alerttoneappletmaps.h"
#include "alerttoneappletmaps.h"
#include <QtTracker/Tracker>
#include <profiled/libprofile.h>
#include <QDebug>

/******************************************************************************
 * Ut_AlertToneAppletMaps implementation.
 */
void
Ut_AlertToneAppletMapsTests::init()
{
}

void
Ut_AlertToneAppletMapsTests::cleanup()
{
}

void Ut_AlertToneAppletMapsTests::alertToneAppletMapsContent_data()
{
      QTest::addColumn<QString>("key");
      QTest::addColumn<QString>("content");
      QTest::newRow("") << "ringing.alert.tone" <<"qtn_sond_ring_tone";
      QTest::newRow("") << "email.alert.tone" <<"qtn_sond_email_tone";
      QTest::newRow("") << "sms.alert.tone" <<"qtn_sond_message_tone";
      QTest::newRow("") << "im.alert.tone" <<"qtn_sond_ins_messaging";
      QTest::newRow("") << "calendar.alert.tone" <<"qtn_sond_org_reminders";
      QTest::newRow("") << "keypad.sound.level" <<"qtn_sond_keyboard";
      QTest::newRow("") << "system.sound.level" <<"qtn_sond_system";
      QTest::newRow("") << "/meegotouch/input_feedback/volume/pulse" <<"qtn_sond_touch_screen";
      QTest::newRow("") << "/meegotouch/input_feedback/volume/vibra" <<"qtn_sond_touch_vibra";
      QTest::newRow("") << "off" <<"qtn_comm_settings_off";
      QTest::newRow("") << "low" <<"qtn_sond_level_1";
      QTest::newRow("") << "medium" <<"qtn_sond_level_2";
      QTest::newRow("") << "high" <<"qtn_sond_level_3";
}

void Ut_AlertToneAppletMapsTests::alertToneAppletMapsContent()
{
      AlertToneAppletMaps  atam;
      QFETCH(QString, key);
      QFETCH(QString, content);

      QString  result;
      result =  atam.map(key);
      QCOMPARE( content , result );
}

MApplication *app;



QTEST_APPLESS_MAIN(Ut_AlertToneAppletMapsTests)
