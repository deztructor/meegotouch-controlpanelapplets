/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_themewidget.h"

#include "themeapplet.h"
#include "themewidget.h"
#include "themedescriptor.h"

#include <MApplication>
#include <MTheme>

#define DEBUG
#include "../../src/debug.h"

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
    /*
     * FIXME: First I have to stub the themedescriptor somehow.
     */
}

void 
Ut_ThemeWidget::cleanupTestCase()
{
    m_App->deleteLater ();
}


QTEST_APPLESS_MAIN(Ut_ThemeWidget)


