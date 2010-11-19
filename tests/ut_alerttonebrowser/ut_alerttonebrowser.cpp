/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <QString>
#include <QStringList>
#include "ut_alerttonebrowser.h"
#include "alerttonebrowser.h"
#include "drilldownitem.h"

#include <QGraphicsLinearLayout>

static const QString validSoundFile1 = "/usr/share/sounds/ring-tones/Beep.aac";

/******************************************************************************
 * Ut_AlertToneBrowser implementation.
 */
struct  MCustomContentItem: public MContentItem
{
	QString fullPath;
	MCustomContentItem (MContentItem::ContentItemStyle itemStyle=MContentItem::IconAndTwoTextLabels, QGraphicsItem *parent=0)
	:MContentItem(itemStyle,parent),fullPath("") {}
};

void
Ut_AlertToneBrowserTests::init()
{
}

void
Ut_AlertToneBrowserTests::cleanup()
{
}

void
Ut_AlertToneBrowserTests::initTestCase()
{
      at = new AlertTone("email.alert.tone");
      savedAlertTone = at->fileName();      

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
Ut_AlertToneBrowserTests::cleanupTestCase()
{
    
      if (at)
      {
            at->set(QVariant (savedAlertTone));
            delete at;
            at = 0;
      }
      gst_deinit();

      free((*m_argv));
	  free(m_argv);
      
      if (m_App)
      delete  m_App;
}

void
Ut_AlertToneBrowserTests::alerttonebrowserConstructor ()
{
      AlertTone at("email.alert.tone");
      AlertToneBrowser atbt(&at);

      QCOMPARE(atbt.m_tone->key(),              QString("email.alert.tone"));
      QCOMPARE(atbt.m_my_music->objectName (),  QString("MContentItem_pickFromMyMusic"));
      QCOMPARE(atbt.m_ovi_store->objectName (), QString("MContentItem_getMoreFromOviStore"));
}

void
Ut_AlertToneBrowserTests::alerttonebrowserRetranslateUi()
{
      AlertToneBrowser atbt(at);
      atbt.retranslateUi();

	QCOMPARE(atbt.m_my_music->property("title").toString() ,qtTrId("qtn_sond_pick_music"));
	QCOMPARE(atbt.m_ovi_store->property("title").toString(), qtTrId("qtn_sond_store"));
}

/*
 * FIXME: This test causes segmentation fault, the contentpicker should be
 * stubbed.
 */
#if 0
void
Ut_AlertToneBrowserTests::alerttonebrowserLaunchMusicBrowser()
{
      AlertToneBrowser atbt (at);

      atbt.launchMusicBrowser();
      QVERIFY (atbt.m_MusicBrowser);
}
#endif

void 
Ut_AlertToneBrowserTests::alerttonebrowserSetAlertTone()
{
    const QString filename = "/usr/share/sounds/ring-tones/Polaris.aac";
    AlertToneBrowser atbt(at);

    /*
     * Setting the alert tone and checking if the value has been set. FIXME: is
     * this properly stubbed?
     * Please note that setAlertTone() is not starting the playback any more.
     */
    atbt.setAlertTone(filename);
    QCOMPARE(atbt.currSelectedFile, filename);
      
    /*
     * Checking the playback.
     */
    atbt.stopPlayingSound();
    atbt.startPlayingSound (filename);
    QVERIFY(atbt.m_preview != NULL);

    // Setting the same filename again should stop the playback!
    atbt.startPlayingSound (filename);
    QVERIFY(atbt.m_preview == NULL);
}


void 
Ut_AlertToneBrowserTests::alerttonebrowserCurrentClicked()
{
      AlertToneBrowser atbt(at);
      atbt.m_current->fullPath = "test_value";
      atbt.currentClicked();
      QCOMPARE(atbt.currSelectedFile, QString("test_value")); 
}

/*!
 * FIXME: This test depends on the filesystem, the file must exists.
 */
void 
Ut_AlertToneBrowserTests::alerttonebrowserAccept()
{
    AlertToneBrowser atbt(at);

    atbt.currSelectedFile = validSoundFile1;
    atbt.accept();   
      
    QCOMPARE(at->fileName(), validSoundFile1);
}



QTEST_APPLESS_MAIN(Ut_AlertToneBrowserTests)
