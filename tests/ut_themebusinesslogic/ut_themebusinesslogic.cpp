/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themebusinesslogic.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <DuiApplication>

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


void 
Ut_ThemeBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_ThemeBusinessLogic";

    m_Priv = new UtThemeBusinessLogicPrivate;
    m_App = new DuiApplication (argc, &app_name);
    m_Api = new ThemeBusinessLogic;

    connect (m_Api, SIGNAL(themeChanged(QString)),
            m_Priv, SLOT(themeChanged(QString)));
}

void 
Ut_ThemeBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
    delete m_Priv;
}

/*!
 * Checks if the information on the current theme is available and valid.
 */
void
Ut_ThemeBusinessLogic::testCurrentTheme ()
{
    QString value;

    value = m_Api->currentThemeCodeName ();
    qDebug() << "*** currentThemeCodeName = " << value;
    QVERIFY (!value.isEmpty());

    value = m_Api->currentThemeName ();
    qDebug() << "*** currentThemeName     = " << value;
    QVERIFY (!value.isEmpty());
}

/*!
 * Checks if there are available themes (at least one) and checks if the name,
 * icon name and codename is not an empty string for all the visible themes.
 * Moreover all the available themes must be valid.
 */
void
Ut_ThemeBusinessLogic::testAvailableThemes ()
{
    QList<ThemeDescriptor *> list = m_Api->availableThemes ();

    QVERIFY (list.size() > 0);

    foreach (ThemeDescriptor *desc, list) {
        qDebug() << "--------------------------------------";
        qDebug() << "*** name      = " << desc->name();
        qDebug() << "*** codeName  = " << desc->codeName();
        qDebug() << "*** iconName  = " << desc->iconName();
        qDebug() << "*** isValid   = " << (desc->isValid() ? "yes" : "no");
        qDebug() << "*** isVisible = " << (desc->isVisible() ? "yes" : "no");

        QVERIFY (desc->isValid());
        if (desc->isVisible()) {
            QVERIFY (!desc->name().isEmpty());
            QVERIFY (!desc->codeName().isEmpty());
            QVERIFY (!desc->iconName().isEmpty());
        }
    }
}

/*!
 * Takes all the themes that are visible and changes the current theme to this
 * themes one by one. Checks if the themeChanged() signal is arrived with this
 * new theme.
 */
void
Ut_ThemeBusinessLogic::testChangeTheme ()
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

QTEST_APPLESS_MAIN(Ut_ThemeBusinessLogic)

