/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebusinesslogic.h"

#include <QString>
#include <QStringList>
#include <DuiTheme>

#define DEBUG
#include "../debug.h"

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

    return theme->findAvailableThemes();
}

void
ThemeBusinessLogic::changeTheme (
        const QString &themeId)
{
    DuiTheme *theme = DuiTheme::instance();

    Q_ASSERT (theme != 0);

    theme->changeTheme(themeId);
}
