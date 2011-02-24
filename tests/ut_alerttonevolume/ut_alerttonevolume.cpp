/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QStringList>
#include "ut_alerttonevolume.h"
#include "alerttonevolume.h"

/******************************************************************************
 * Ut_AlertToneVolume implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_alerttonevolume",
      NULL };

void
Ut_AlertToneVolumeTests::init()
{
      m_App = new MApplication(argc, argv);
}

void
Ut_AlertToneVolumeTests::cleanup()
{
      if ( m_App )
      delete  m_App;
}

void
Ut_AlertToneVolumeTests::alerttonevolumeConstructor ()
{
      MApplicationWindow aw;
      AlertToneVolume   atv;

      QCOMPARE(atv.m_fileName.key() ,QString ("ringing.alert.tone@general") );
      QCOMPARE(atv.m_volume.key()   ,QString ("ringing.alert.volume@general") );
}

void
Ut_AlertToneVolumeTests::alerttonevolumeVolumeChanged_data()
{
      QTest::addColumn<int>("volume");
      QTest::newRow("") << 10;
      QTest::newRow("") << 90;
      QTest::newRow("") << -1;
      QTest::newRow("") << 190;
}

void
Ut_AlertToneVolumeTests::alerttonevolumeVolumeChanged()
{
      QFETCH(int, volume);
      MApplicationWindow aw;
      AlertToneVolume   atv;
      atv.m_volume.m_val =  QVariant(volume);
      atv.m_volume.emit_changed();

      if (volume <0 )
      volume =0;
      else if (volume > 100)
      volume =100;

      QCOMPARE(atv.value (),volume);
}

void
Ut_AlertToneVolumeTests::alerttonevolumeFileNameChanged()
{
      MApplicationWindow aw;
      AlertToneVolume   atv;
      QVERIFY( 0 == (int)atv.m_preview);

      atv.setState (MSliderModel::Released);
      atv.m_fileName.m_val =  QVariant("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
      atv.m_fileName.emit_changed();
      QVERIFY( 0 == (int)atv.m_preview);

      atv.setState (MSliderModel::Pressed);
      atv.m_fileName.m_val =  QVariant("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
      atv.m_fileName.emit_changed();
      QVERIFY( 0 != (int)atv.m_preview);
}

void
Ut_AlertToneVolumeTests::alerttonevolumeSlider()
{
      MApplicationWindow aw;
      AlertToneVolume   atv;
      QVERIFY( 0 == (int)atv.m_preview);
      atv.m_fileName.m_val =  QVariant("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
      atv.slotSliderPressed();
      QVERIFY( 0 != (int)atv.m_preview);
      AlertTonePreview * atp = atv.m_preview;
      atv.slotSliderPressed();
      QVERIFY( atp == atv.m_preview);
      atv.slotSliderReleased();
      QVERIFY( !atv.m_preview);
}
/* ???????????????????why ???????????????? */
void
Ut_AlertToneVolumeTests::alerttonevolumeWidgetVolumeChanged()
{
      /*
      MApplicationWindow aw;
      AlertToneVolume   atv;
      QVERIFY( 0 == (int)atv.m_preview);
      atv.m_fileName.m_val =  QVariant("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
*/
//setValue(int value)
}

QTEST_APPLESS_MAIN(Ut_AlertToneVolumeTests)
