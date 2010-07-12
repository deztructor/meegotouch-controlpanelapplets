/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themecellcreator.h"
#include "mdesktopentry.h"
#include "themebusinesslogic.h"
#include "themedescriptor.h"
#include "themelistmodel.h"
#include "themecellcreator.h"

#include <MApplication>
#include <MGConfItem>
#include <MTheme>
#include <MWidgetRecycler>
#include <MAdvancedListItem>
#include <MImageWidget>

#include <QModelIndex>

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
 * Ut_ThemeCellCreator implementation. 
 */
void 
Ut_ThemeCellCreator::init()
{
}

void 
Ut_ThemeCellCreator::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_ThemeCellCreator";

void 
Ut_ThemeCellCreator::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_ThemeBusinessLogic = new ThemeBusinessLogic;
    m_ThemeDescList = m_ThemeBusinessLogic->availableThemes ();
}

void 
Ut_ThemeCellCreator::cleanupTestCase()
{
    delete m_App;
    delete m_ThemeBusinessLogic;
}

void 
Ut_ThemeCellCreator::testCreateCell ()
{
    ThemeListModel    *model;
    QModelIndex        index;
    MWidgetRecycler    recycler;
    int                rows;
    MWidget           *cell;
    ThemeCellCreator  *cellCreator;
    MAdvancedListItem *item;

    model = new ThemeListModel;
    model->setThemeList (m_ThemeDescList);
    cellCreator = new ThemeCellCreator;

    rows = model->rowCount (index);

    SYS_DEBUG ("*** rows    = %d", rows);

    for (int n = 0; n < rows; ++n) {
        index = model->index (n, 0);
        SYS_DEBUG ("*** n       = %d", n);

        cell = cellCreator->createCell (index, recycler);
        item = qobject_cast<MAdvancedListItem *> (cell);

        QVERIFY (item);
        SYS_DEBUG ("+++ title    = %s", SYS_STR(item->title()));
        SYS_DEBUG ("+++ icon     = %s", SYS_STR(item->imageWidget()->image()));
        QVERIFY (!item->title().isEmpty());
        QVERIFY (!item->imageWidget()->image().isEmpty());
    }
}

QTEST_APPLESS_MAIN(Ut_ThemeCellCreator)

