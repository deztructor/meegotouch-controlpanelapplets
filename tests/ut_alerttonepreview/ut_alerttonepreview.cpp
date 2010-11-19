/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <stdlib.h>
#include <string.h>
#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QStringList>
#include  "ut_alerttonepreview.h"
#include  "alerttonepreview.h"

/******************************************************************************
 * Ut_AlertTonePreview implementation.
 */

void
Ut_AlertTonePreviewTests::initTestCase()
{
      char *argv1;

      m_argc = 1;
      m_argv = (char **)malloc(2 * sizeof(char *));
      memset(m_argv, 0, 2 * sizeof(char *));
      argv1 = (char *)malloc(strlen("my_argv") + 1);
      strcpy(argv1, "my_argv");
      (*m_argv) = argv1;

      gst_init(&m_argc, &m_argv);

      m_App = new MApplication(m_argc, m_argv);
}

void
Ut_AlertTonePreviewTests::cleanupTestCase()
{
	gst_deinit();

	free((*m_argv));
	free(m_argv);
}

void
Ut_AlertTonePreviewTests::alerttonepreviewConstructor ()
{
      MApplicationWindow aw;
      AlertTonePreview  atp("/usr/share/sounds/ring-tones/Lucid dreaming.aac");

      QVERIFY(atp.m_gstPipeline != 0 );
      QVERIFY(atp.m_gstFilesrc != 0 );
      QVERIFY(atp.m_gstVolume != 0 );
      QCOMPARE(atp.m_profileVolume.key(), QString( "ringing.alert.volume" ));
}

void
Ut_AlertTonePreviewTests::alerttonepreviewfname()
{
      MApplicationWindow aw;
      AlertTonePreview  atp("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
      QCOMPARE(atp.fname(), QString( "/usr/share/sounds/ring-tones/Lucid dreaming.aac" ));
}

void
Ut_AlertTonePreviewTests::alerttonepreviewProfileToGstVolume()
{
      MApplicationWindow aw;
      AlertTonePreview  atp("/usr/share/sounds/ring-tones/Lucid dreaming.aac");
      qDebug() << atp.profileToGstVolume();
      double  volume;
      volume = atp.profileToGstVolume();
      QVERIFY ( volume >= 0 && volume <= 1 );



}

void
Ut_AlertTonePreviewTests::alerttonepreviewProfileVolumeChanged()
{
      MApplicationWindow aw;
      AlertTonePreview  atp("/usr/share/sounds/ring-tones/Lucid dreaming.aac");

      double  *  cnt = new double;
      atp.m_profileVolume.set(QVariant(50));
      atp.m_profileVolume.emit_changed();

      g_object_get(G_OBJECT(atp.m_gstVolume), "volume", cnt, NULL);

      QVERIFY (qFuzzyCompare ( (float) *cnt, (float) 0.5 ) );

      atp.m_profileVolume.set(QVariant(90));
      atp.m_profileVolume.emit_changed();

      g_object_get(G_OBJECT(atp.m_gstVolume), "volume", cnt, NULL);
      QVERIFY (qFuzzyCompare ( (float) *cnt, (float) 0.9 ) );
}

QTEST_APPLESS_MAIN(Ut_AlertTonePreviewTests)
