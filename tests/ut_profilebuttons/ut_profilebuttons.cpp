/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_profilebuttons.h"
#include "profilebuttons.h"

#include <MApplication>
#include <QMap>
#include <QPair>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * SignalSink implementation. 
 */
void
SignalSink::profileSelected (
        int id)
{
    SYS_DEBUG ("*** id = %d", id);
}

/******************************************************************************
 * Ut_ProfileButtons implementation. 
 */
void 
Ut_ProfileButtons::init()
{
}

void 
Ut_ProfileButtons::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ProfileButtons";

void 
Ut_ProfileButtons::initTestCase()
{
    bool connectSuccess;

    m_App = new MApplication (argc, &app_name);
    m_Buttons = new ProfileButtons;

    /*
     * Testing if the ProfileButtons has the proper signals and slots.
     */
    connectSuccess = connect (
            m_Buttons, SIGNAL(profileSelected(int)),
            &m_SignalSink, SLOT(profileSelected(int)));
    QVERIFY(connectSuccess);

    connectSuccess = connect (
            &m_SignalSink, SIGNAL(selectProfile(int)),
            m_Buttons, SLOT(selectProfile(int)));
    QVERIFY(connectSuccess);
}

void 
Ut_ProfileButtons::cleanupTestCase()
{
    delete m_Buttons;
    m_App->deleteLater ();
}

/*
 iconId  = icon-m-common-volume
 name    = Normal
 id      = 0
 checked = true

 iconId  = icon-m-profile-silent
 name    = Silent
 id      = 1
 checked = false

 iconId  = icon-m-profile-beep
 name    = Beep
 id      = 2
 checked = false

 iconId  = icon-m-profile-loud
 name    = Loud
 id      = 3
 checked = false
*/
void 
Ut_ProfileButtons::testInit ()
{
    QString selectedProfile;
    QMap<int, QPair<QString, QString> > map;
    QPair<QString, QString> pair0 (
            QString ("icon-m-common-volume"),
            QString ("Normal"));
    QPair<QString, QString> pair1 (
            QString ("icon-m-profile-silent"),
            QString ("Silent"));
    QPair<QString, QString> pair2 (
            QString ("icon-m-profile-beep"),
            QString ("Beep"));
    QPair<QString, QString> pair3 (
            QString ("icon-m-profile-loud"),
            QString ("Loud"));
    

    map.insert (0, pair0);
    map.insert (1, pair1);
    map.insert (2, pair2);
    map.insert (3, pair3);

    /*
     * Pushing all the data and also selecting the index 0 as the current
     * profile.
     */
    m_Buttons->init (map, 0);

    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Normal");

    /*
     * Now we can test the other profiles with the selectProfile().
     */
    emit m_SignalSink.selectProfile (1);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Silent");
    
    emit m_SignalSink.selectProfile (2);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Beep");
    
    emit m_SignalSink.selectProfile (3);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Loud");
    
    /*
     * Now we create an other map that contains the new profile names and
     * retranslate the ProfileButtons. Then we can check the new names coming
     * back. 
     */
    QMap<int, QString> languageMap;

    languageMap.insert (0, "Normal1");
    languageMap.insert (1, "Silent1");
    languageMap.insert (2, "Beep1");
    languageMap.insert (3, "Loud1");
    
    m_Buttons->retranslate (languageMap);

    emit m_SignalSink.selectProfile (0);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Normal1");

    emit m_SignalSink.selectProfile (1);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Silent1");
    
    emit m_SignalSink.selectProfile (2);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Beep1");
    
    emit m_SignalSink.selectProfile (3);
    selectedProfile = m_Buttons->selectedProfileName ();
    SYS_DEBUG ("*** selectedProfile = %s", SYS_STR(selectedProfile));
    QVERIFY (selectedProfile == "Loud1");
}

QTEST_APPLESS_MAIN(Ut_ProfileButtons)




