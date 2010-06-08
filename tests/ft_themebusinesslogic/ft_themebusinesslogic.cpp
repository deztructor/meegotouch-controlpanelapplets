/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ft_themebusinesslogic.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

static const int themeWaitMSec = 1000;

/******************************************************************************
 * Ft_ThemeBusinessLogicPrivate implementation. 
 */
void
FtThemeBusinessLogicPrivate::themeChanged (
        QString themeCodeName)
{
    m_ThemeCodeName = themeCodeName;
}

/******************************************************************************
 * Ut_ThemeBusinessLogic implementation. 
 */
void 
Ft_ThemeBusinessLogic::init()
{
}

void 
Ft_ThemeBusinessLogic::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ft_ThemeBusinessLogic";

void 
Ft_ThemeBusinessLogic::initTestCase()
{
    bool connectSuccess;

    m_Priv = new FtThemeBusinessLogicPrivate;
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
Ft_ThemeBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
    delete m_Priv;
}

/*!
 * Checks if the information on the current theme is available and valid.
 */
void
Ft_ThemeBusinessLogic::testCurrentTheme ()
{
    QString themeCodeName, themeName;

    // We are checking this in the unit test already, but it is simple...
    themeCodeName = m_Api->currentThemeCodeName ();
    SYS_DEBUG ("*** currentThemeCodeName = %s", SYS_STR(themeCodeName));
    QVERIFY (!themeCodeName.isEmpty());

    themeName = m_Api->currentThemeName ();
    SYS_DEBUG ("*** currentThemeName     = %s", SYS_STR(themeName));
    QVERIFY (!themeName.isEmpty());
}

/*!
 * Checks if there are available themes (at least one) and checks if the name,
 * icon name and codename is not an empty string for all the visible themes.
 * Moreover all the available themes must be valid.
 *
 * Checks if the invisible themes are filtered out.
 */
void
Ft_ThemeBusinessLogic::testAvailableThemes ()
{
    QList<ThemeDescriptor *> list = m_Api->availableThemes ();

    QVERIFY (list.size() > 0);

    foreach (ThemeDescriptor *desc, list) {
        #if 0
        qDebug() << "--------------------------------------";
        qDebug() << "*** name      = " << desc->name();
        qDebug() << "*** codeName  = " << desc->codeName();
        qDebug() << "*** iconName  = " << desc->iconName();
        qDebug() << "*** isValid   = " << (desc->isValid() ? "yes" : "no");
        qDebug() << "*** isVisible = " << (desc->isVisible() ? "yes" : "no");
        #endif

        QVERIFY (desc->isValid());
        QVERIFY (!desc->name().isEmpty());
        QVERIFY (!desc->codeName().isEmpty());
        QVERIFY (!desc->iconName().isEmpty());
	    QVERIFY (desc->isVisible());
    }
}

/*!
 * Takes all the themes that are visible and changes the current theme to this
 * themes one by one. Checks if the themeChanged() signal is arrived with this
 * new theme.
 */
void
Ft_ThemeBusinessLogic::testChangeTheme ()
{
    QList<ThemeDescriptor *> list = m_Api->availableThemes ();

    foreach (ThemeDescriptor *desc, list) {
        if (!desc->isVisible()) {
            continue;
        }

        qDebug() << "Changing theme to " << desc->codeName();
        m_Api->changeTheme (desc->codeName());
        QTest::qWait (themeWaitMSec);

        qDebug() << "Theme is          " << m_Priv->m_ThemeCodeName;
        QVERIFY (m_Priv->m_ThemeCodeName == desc->codeName());
    }
}

QTEST_APPLESS_MAIN(Ft_ThemeBusinessLogic)

