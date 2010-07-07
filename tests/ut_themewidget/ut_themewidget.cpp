/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themewidget.h"

#include "themeapplet.h"
#include "themewidget.h"
#include "themedescriptor.h"

#include <MApplication>
#include <MDialog>
#include <MTheme>

#include <QModelIndex>
#include <QVariant>

//#define LOTDEBUG
#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Stubbing the MDialog, so the UI will not be really shown.
 */
QString lastDialogTitle;

void
MDialog::appear (
        MSceneWindow::DeletionPolicy policy)
{
    SYS_DEBUG ("----------------------------------------------------------");
    SYS_DEBUG ("*** this    = %p", this);
    SYS_DEBUG ("*** title   = %s", SYS_STR(title()));

    lastDialogTitle = title();
}
/******************************************************************************
 * Ut_ThemeWidget implementation. 
 */
void 
Ut_ThemeWidget::init()
{
}

void 
Ut_ThemeWidget::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ThemeWidget";

void 
Ut_ThemeWidget::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_ThemeBusinessLogic = new ThemeBusinessLogic;
    m_ThemeWidget = new ThemeWidget (m_ThemeBusinessLogic);
}

void 
Ut_ThemeWidget::cleanupTestCase()
{
    delete m_ThemeWidget;
    delete m_ThemeBusinessLogic;

    m_App->deleteLater ();
}

/*!
 * This test will call the themeActivated() method of the widget with each and
 * every one of the rows found in the model. Then the method will check that the
 * widget shown a dialog. This test is using a simple MDialog stub found in this
 * source file.
 */
void 
Ut_ThemeWidget::testThemeActivated ()
{
    QModelIndex      index;
    QVariant         data;
    QStringList      stringList;
    QString          name;
    int              rows;

    rows = m_ThemeWidget->m_ThemeListModel->rowCount (index);

    SYS_DEBUG ("*** rows    = %d", rows);
    for (int n = 0; n < rows; ++n) {
        index = m_ThemeWidget->m_ThemeListModel->index (n, 0);
        data = index.data();
        stringList = data.value<QStringList>();

        QVERIFY (stringList.size() >= 2);
        name = stringList[1];
        SYS_DEBUG ("*** name            = %s", SYS_STR(name));
        #ifdef LOTDEBUG
        SYS_DEBUG ("--------------------------------------------------");
        foreach (QString sval, stringList) {
            SYS_DEBUG ("*** sval = %s", SYS_STR(sval));
        }
        #endif

        QVERIFY (index.isValid ());
        QVERIFY (index.column() == 0);
        QVERIFY (index.row() == n);

        lastDialogTitle = "";
        m_ThemeWidget->themeActivated (index);
        /*
         * If this is the current theme there should be no dialog shown, if not,
         * a dialog should be shown with the theme name set as title.
         */
        SYS_DEBUG ("*** lastDialogTitle = %s", SYS_STR(lastDialogTitle));
        if (m_ThemeBusinessLogic->currentThemeName() != name)
            QVERIFY (lastDialogTitle == name);
        else
            QVERIFY (lastDialogTitle == "");
    }
}

QTEST_APPLESS_MAIN(Ut_ThemeWidget)


