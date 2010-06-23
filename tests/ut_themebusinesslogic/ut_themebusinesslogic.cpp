/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themebusinesslogic.h"
#include "mdesktopentry.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MApplication>
#include <MGConfItem>
#include <MTheme>

#ifndef MDESKTOPENTRY_STUB_H
# error "The MDesktopEntry class is not stubbed, can't continue."
#endif

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Fake MGconfItem implementation. 
 */
QString  GConfLastKey;
QString  GConfLastValue;

void
MGConfItem::set (
        const QVariant &val)
{
    GConfLastKey   = key();
    GConfLastValue = val.toString();
    SYS_DEBUG ("*** value = %s", SYS_STR(val.toString()));
    SYS_DEBUG ("*** key   = %s", SYS_STR(key()));
}

/******************************************************************************
 * Ut_ThemeBusinessLogicPrivate implementation. 
 */
void
UtThemeBusinessLogicPrivate::themeChanged (
        QString themeCodeName)
{
    m_ThemeCodeName = themeCodeName;
}

/******************************************************************************
 * Ut_ThemeBusinessLogic implementation. 
 */
void 
Ut_ThemeBusinessLogic::init()
{
}

void 
Ut_ThemeBusinessLogic::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ThemeBusinessLogic";

void 
Ut_ThemeBusinessLogic::initTestCase()
{
    bool connectSuccess;

    m_Priv = new UtThemeBusinessLogicPrivate;
    m_App = new MApplication (argc, &app_name);
    m_Api = new ThemeBusinessLogic;

    connectSuccess = connect (
            m_Api, SIGNAL(themeChanged(QString)),
            m_Priv, SLOT(themeChanged(QString)));
    QVERIFY (connectSuccess);
    
    connectSuccess = connect (
            m_Priv, SIGNAL(changeTheme(QString)),
            m_Api, SLOT(changeTheme(QString)));
    QVERIFY (connectSuccess);

}

void 
Ut_ThemeBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
    delete m_Priv;
}

/*!
 * This method will test the available themes stubbed, exactly as it is in the 
 * qdirstub.h and in the mdesktopentry stub databse.
 */
void
Ut_ThemeBusinessLogic::testAvailableThemes ()
{
    QList<ThemeDescriptor *> list = m_Api->availableThemes ();
    ThemeDescriptor *desc;

    /*
     * In the mdesktopentry stub implementation currently there is this much 
     * available valid and visible themes.
     */
    SYS_DEBUG ("*** we have %d valid themes.", list.size());
    QVERIFY (list.size() == 2);
    desc = list[0];

    QVERIFY(desc->isValid());
    QVERIFY(desc->isVisible());
    QVERIFY(desc->name() == NAMEDesktopFilePerfect);
    QVERIFY(desc->codeName() == CODENAMEDesktopFilePerfect);
    QVERIFY(desc->iconName() == ICONDesktopFilePerfect);
}


/*!
 * Checks if the information on the current theme is available and valid.
 */
void
Ut_ThemeBusinessLogic::testCurrentTheme ()
{
    QString themeCodeName;
    QString themeName;
    QString themeIconName;

    // We are stubbed, how can we test??
    themeCodeName = m_Api->currentThemeCodeName ();
    themeName = m_Api->currentThemeName ();
    themeIconName = m_Api->currentThemeIconName();
    
    SYS_DEBUG ("*** currentThemeCodeName() = %s", SYS_STR(themeCodeName));
    SYS_DEBUG ("*** currentThemeName()     = %s", SYS_STR(themeName));
    SYS_DEBUG ("*** currentThemeIconName() = %s", SYS_STR(themeIconName));

    QVERIFY (!themeCodeName.isEmpty());
    QVERIFY (themeCodeName == MTheme::instance()->currentTheme());
    QVERIFY (themeName == NAMEDesktopFileCurrent);
    QVERIFY (themeIconName == ICONDesktopFileCurrent);
}

void
Ut_ThemeBusinessLogic::testSetTheme ()
{
    QString fakeName = "FakeThemeCodeName";

    m_Api->changeTheme (fakeName);
    QVERIFY (GConfLastKey == "/meegotouch/theme/name");
    QVERIFY (GConfLastValue == fakeName);
}

QTEST_APPLESS_MAIN(Ut_ThemeBusinessLogic)

