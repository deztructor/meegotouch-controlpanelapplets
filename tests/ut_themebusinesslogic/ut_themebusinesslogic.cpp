/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themebusinesslogic.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MApplication>
#include <MTheme>

#define DEBUG
#include "../../src/debug.h"

static const int themeWaitMSec = 1000;

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

void 
Ut_ThemeBusinessLogic::testThemeDescriptorInvalid()
{
    const QString codeName = "CodeName";
    ThemeDescriptor  desc ("no_such_a_file", codeName);

    QVERIFY (!desc.isValid());
    QVERIFY (!desc.isVisible());
    QVERIFY (desc.name().isEmpty());
    QVERIFY (desc.codeName() == codeName);
    QVERIFY (desc.iconName().isEmpty());
}

/*!
 * Checks if the information on the current theme is available and valid.
 */
void
Ut_ThemeBusinessLogic::testCurrentTheme ()
{
    QString themeCodeName;

    themeCodeName = m_Api->currentThemeCodeName ();
    SYS_DEBUG ("*** currentThemeCodeName() = %s", SYS_STR(themeCodeName));
    QVERIFY (!themeCodeName.isEmpty());
    QVERIFY (themeCodeName == MTheme::instance()->currentTheme());
}


QTEST_APPLESS_MAIN(Ut_ThemeBusinessLogic)

