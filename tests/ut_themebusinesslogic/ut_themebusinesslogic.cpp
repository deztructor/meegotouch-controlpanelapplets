/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themebusinesslogic.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <DuiApplication>

#define DEBUG
#include "../../src/debug.h"

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

    m_App = new DuiApplication (argc, &app_name);
    m_Api = new ThemeBusinessLogic;
}

void 
Ut_ThemeBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

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

QTEST_APPLESS_MAIN(Ut_ThemeBusinessLogic)

