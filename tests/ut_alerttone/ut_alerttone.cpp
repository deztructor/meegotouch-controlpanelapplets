/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <QString>
#include <QStringList>
#include "ut_alerttone.h"
#include "alerttone.h"
#include <QtTracker/Tracker>
#include <QDebug>

#define DEBUG 
#define WARNING
#include "debug.h"

static const QString validSoundFile1 = "/usr/share/sounds/ring-tones/Beep.aac";

/******************************************************************************
 * Ut_AlertTone implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_alerttonetests",
      NULL };

void
Ut_AlertToneTests::initTestCase()
{
      called_alertToneChanged = false;
      m_App = new MApplication(argc, argv);
}

void
Ut_AlertToneTests::cleanupTestCase()
{
    if (m_App)
        delete m_App;
}

void
Ut_AlertToneTests::alerttonesAlertTones()
{
      QList<AlertTone *>  tones;
      tones = AlertTone::alertTones();

      for ( int i = 0 ; i <tones.size() ;++i)
      {
          
          qDebug ()  <<  tones[i]->key() << "\n\n\n";
            QVERIFY(checkIfAlarmTone (tones[i]->key()) );
      }
}

#if 0
/*
 * AlertTone::niceNameFromFileName() has been removed, use
 * TrackerConnection::niceNameFromFileName() instead.
 */
void
Ut_AlertToneTests::alerttonesNiceNameFromFileName()
{
      QString trackerID;
      QString niceName;
      QVector<QStringList> result = ::tracker()->rawSparqlQuery(QString( "SELECT ?u WHERE { ?x a nmm:MusicPiece; tracker:available true; nie:url ?u }"));

      if (result.size() > 0)
      {
            QStringList  list,secondlist;
            list = result.at(0).at(0).split ( "//");
            secondlist = list.at(1).split ( "/");

            QString  expectName =  secondlist.at(secondlist.size() - 1);

            expectName = expectName.left(expectName.lastIndexOf('.'));

            niceName = AlertTone::niceNameFromFileName(list.at(1), true, &trackerID);
            QVERIFY( expectName.contains (niceName) );
            QVERIFY(!trackerID.isEmpty ());

            niceName = AlertTone::niceNameFromFileName(list.at(1), false, &trackerID);
            QCOMPARE( niceName , expectName );
      }

      niceName = AlertTone::niceNameFromFileName("something.test", true, &trackerID);
      QCOMPARE( niceName , QString("something") );

      niceName = AlertTone::niceNameFromFileName("directory/something.test", true, &trackerID);
      QCOMPARE( niceName , QString("something") );
}
#endif


void
Ut_AlertToneTests::alerttoneFetchFromBackend()
{
      AlertTone at("email.alert.tone");
      at.m_niceName= "";
      at.fetchFromBackend();
      QVERIFY (at.m_niceName != QString(""));
}

void
Ut_AlertToneTests::alertToneChanged()
{
	called_alertToneChanged = true;
}

/*!
 * This test will set the value an AlerTone object and check if the signal about
 * the value change is indeed emitted.
 *
 * FIXME: This test rely on the sounfile being available in the filesystem. The
 * AlertTone object will reject the value if it is not a file path of an 
 * existing file.
 */
void
Ut_AlertToneTests::alerttoneRealSetValue()
{
    AlertTone at("email.alert.tone");
	
    connect(&at, SIGNAL(changed()), this, SLOT(alertToneChanged()));

    at.realSetValue(QVariant(validSoundFile1));

    for (int Nix = 0 ; Nix < 300; Nix += 1000) {
        QTest::qWait(100);
        if (called_alertToneChanged)
            break;
    }
	
    called_alertToneChanged = false;
    QVERIFY (at.value() == QVariant(validSoundFile1));
}

void
Ut_AlertToneTests::alerttoneMaybeUpdate()
{
      AlertTone at("email.alert.tone");
      at.m_val.clear();
      at.maybeUpdate();
      QVERIFY (at.m_val.type() != QVariant::Invalid);
}

bool
Ut_AlertToneTests::checkIfAlarmTone (QString name)
{
      if (  "calendar.alert.tone" ==  name ||
            "email.alert.tone" ==  name ||
            "im.alert.tone" ==  name ||
            "ringing.alert.tone" ==  name ||
            "voip.alert.tone" ==  name ||
            "sms.alert.tone" ==  name )
      return  true;

      return  false;

}


QTEST_APPLESS_MAIN(Ut_AlertToneTests)
