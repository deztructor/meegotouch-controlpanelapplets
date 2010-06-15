/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_profiledatainterface.h"
#include "profiledatainterface.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_ProfileDataInterface implementation. 
 */
void 
Ut_ProfileDataInterface::init()
{
}

void 
Ut_ProfileDataInterface::cleanup()
{
}

static int argc = 1;
static char* app_name = (char*) "./Ut_ProfileDataInterface";

void 
Ut_ProfileDataInterface::initTestCase()
{

    m_App = new MApplication (argc, &app_name);
    m_Api = new ProfileDataInterface;
}

void 
Ut_ProfileDataInterface::cleanupTestCase()
{
    delete m_Api;
    m_App->deleteLater();
}

/*!
 * Gets the current profile name, checks if the profile name is not an empty
 * string.
 */
void 
Ut_ProfileDataInterface::testGetCurrentProfileName ()
{
    QString prof = m_Api->getCurrentProfileName ();

    qDebug() << "*** CurrentProfileName = " << prof;
    QVERIFY(!prof.isEmpty());
}

/*!
 * Reads the profile data and prints the details with debug messages. Chacks if
 * there is at least 1 available profiles.
 */
void 
Ut_ProfileDataInterface::testGetProfilesData ()
{
    QList<ProfileDataInterface::ProfileData> list = m_Api->getProfilesData ();
    int n = 0;

    foreach (ProfileDataInterface::ProfileData item, list) {
        qDebug() << "----------------------------------------------";
        qDebug() << "profile[" << n << "].visualData.second        = " <<
            item.visualData.second;
        qDebug() << "profile[" << n << "].visualData.first        = " <<
            item.visualData.first;
        qDebug() << "profile[" << n << "].id          = " <<
            item.profileId;
        qDebug() << "profile[" << n << "].vibrationEnabled = " <<
            (item.vibrationEnabled ? "yes" : "no");

        ++n;
    }

    /*
     * We have to get at least one profile. We actually should get 4 of them?
     */
    QVERIFY (n > 0);
}

/*!
 * Will go through the profiles and set each and every one of them as current
 * profile. Tests if the setting of the current profile was successfull.
 */
void 
Ut_ProfileDataInterface::testSetGetProfile ()
{
    for (int profileId = 0; profileId <= 3; ++profileId) {
        qDebug() << "Setting profile to " << profileId;
        m_Api->setProfile (profileId);

        QVERIFY (m_Api->getCurrentProfile() == profileId);
    }
}

/*!
 * Tests if the vibration can be enabled/disabled in the profile. 
 *
 * Please note that this test is currently disabled, since there is some bug in
 * the backend. Ok, as NB#161433 is fixed I re-enable the test.
 */
void 
Ut_ProfileDataInterface::testSetVibration ()
{
    QList<ProfileDataInterface::ProfileData> list;

    /*
     * Let's disable the vibration for all the profiles and check if it is
     * really diabled.
     */
    list = m_Api->getProfilesData ();
    foreach (ProfileDataInterface::ProfileData item, list) {
        m_Api->setVibration (item.profileId, false);
    }

    list = m_Api->getProfilesData ();
    foreach (ProfileDataInterface::ProfileData item, list) {
        if (item.vibrationEnabled)
            qDebug() << "Vibration for " << item.visualData.second << " is " <<
                (item.vibrationEnabled ? "enabled" : "disabled") <<
                "(should be disabled)";

        QVERIFY (!item.vibrationEnabled);
    }

    /*
     * The same with enabled vibration.
     */
    list = m_Api->getProfilesData ();
    foreach (ProfileDataInterface::ProfileData item, list) {
        m_Api->setVibration (item.profileId, true);
    }

    list = m_Api->getProfilesData ();
    foreach (ProfileDataInterface::ProfileData item, list) {
        if (!item.vibrationEnabled)
            qDebug() << "Vibration for " << item.visualData.second << " is " <<
                (item.vibrationEnabled ? "enabled" : "disabled") <<
                "(should be enabled)";

        QVERIFY (item.vibrationEnabled);
    }
}

QTEST_APPLESS_MAIN(Ut_ProfileDataInterface)
