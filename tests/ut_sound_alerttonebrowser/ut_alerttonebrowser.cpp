/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MApplication>
#include <QString>
#include <QStringList>
#include "ut_alerttonebrowser.h"
#include "alerttonebrowser.h"
#include "drilldownitem.h"

#include <MContentItem>
#include <gst/gst.h>
#include <QGraphicsLinearLayout>

static const QString validSoundFile1 = "/usr/share/sounds/ring-tones/Beep.aac";

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

/******************************************************************************
 * Stubs
 */

#ifdef HAVE_LIBRESOURCEQT
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
 * Ut_AlertToneBrowser implementation.
 */
class MCustomContentItem: public MContentItem
{
public:
	MCustomContentItem (MContentItem::ContentItemStyle itemStyle=MContentItem::IconAndTwoTextLabels, QGraphicsItem *parent=0)
	:MContentItem(itemStyle,parent),fullPath("") {}

	QString fullPath;
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

    QCOMPARE (atbt.m_tone->key(), QString ("email.alert.tone"));
#ifdef HAVE_CONTENT_MANAGER
    QCOMPARE (atbt.m_my_music->objectName (),
              QString ("MContentItem_pickFromMyMusic"));
#endif
    QCOMPARE (atbt.m_ovi_store->objectName (),
              QString ("MContentItem_getMoreFromOviStore"));
}

void
Ut_AlertToneBrowserTests::alerttonebrowserRetranslateUi()
{
    AlertToneBrowser atbt(at);
    atbt.retranslateUi();

#ifdef HAVE_CONTENT_MANAGER
	QCOMPARE (atbt.m_my_music->property ("title").toString (),
              qtTrId ("qtn_sond_pick_music"));
#endif
	QCOMPARE (atbt.m_ovi_store->property ("title").toString(),
              qtTrId ("qtn_sond_store"));
}

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
