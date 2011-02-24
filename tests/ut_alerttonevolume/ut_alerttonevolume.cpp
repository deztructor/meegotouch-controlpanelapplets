/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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
#include "ut_alerttonevolume.h"
#include "alerttonevolume.h"

#include <MApplication>
#include <MApplicationWindow>
#include <QString>
#include <QStringList>

/******************************************************************************
 * Stubs
 */

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
namespace ResourcePolicy {
//ResourceSet::ResourceSet(const QString &applicationClass, QObject *parent)
//{
//    Q_UNUSED(applicationClass);
//    Q_UNUSED(parent);
//}

bool
ResourceSet::addResource (ResourceType resourceType)
{
    Q_UNUSED(resourceType);
    return true;
}

void
ResourceSet::addResourceObject (Resource *resource)
{
    Q_UNUSED(resource);
}

bool
ResourceSet::acquire ()
{
    return true;
}

bool
ResourceSet::release ()
{
    return true;
}

//AudioResource::AudioResource (const QString &audioGroup)
//{
//    Q_UNUSED(audioGroup);
//}

void
AudioResource::setProcessID (quint32 newPID)
{
    Q_UNUSED(newPID);
}

void
AudioResource::setStreamTag (const QString &name, const QString &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
}

}
#endif

/******************************************************************************
 * Ut_AlertToneVolume implementation.
 */
int   argc = 1;
char *argv[] = {
      (char *) "./ut_alerttonevolume",
      NULL };

void
Ut_AlertToneVolumeTests::initTestCase()
{
    qDebug () << "TEST";
    m_App = new MApplication(argc, argv);
}

void
Ut_AlertToneVolumeTests::cleanupTestCase()
{
    delete m_App;
    m_App = 0;
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
