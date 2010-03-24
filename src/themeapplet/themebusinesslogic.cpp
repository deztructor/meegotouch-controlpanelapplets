/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebusinesslogic.h"

#include <QString>
#include <QStringList>
#include <DuiTheme>

#define DEBUG
#include "../debug.h"

#define USE_TEST_DATA

ThemeBusinessLogic::ThemeBusinessLogic ()
{
}

ThemeBusinessLogic::~ThemeBusinessLogic ()
{
}


QString
ThemeBusinessLogic::currentThemeName () const
{
    DuiTheme *theme = DuiTheme::instance();

    Q_ASSERT (theme != 0);

    return theme->currentTheme();
}

QStringList
ThemeBusinessLogic::availableThemes () const
{
    DuiTheme *theme = DuiTheme::instance();

    Q_ASSERT (theme != 0);

    QStringList retval = theme->findAvailableThemes();
    #ifdef USE_TEST_DATA
    retval <<
        "devel" <<
        "Test theme 1" <<
        "Test theme 2" <<
        "Test theme 3" <<
        "Test theme 4";
    #endif

    return retval;
}

void
ThemeBusinessLogic::changeTheme (
        QString themeId)
{
    SYS_DEBUG ("Activating theme '%s'", SYS_STR(themeId));
    DuiTheme *theme = DuiTheme::instance();

    Q_ASSERT (theme != 0);

    theme->changeTheme(themeId);

    emit themeChanged (themeId);
}


