/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <DuiTheme>
#include <DuiGConfItem>

#define DEBUG
#include "../debug.h"

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

/*
 * /usr/share/themes
 */
QList<ThemeDescriptor *>
ThemeBusinessLogic::availableThemes () const
{
    QList<ThemeDescriptor *> retval;
    
    #if 0
    /*
     * This is a nice try, but it is not implemented in the libdui and I was
     * tolt it is not going to be either.
     */
    DuiTheme *theme = DuiTheme::instance();
    Q_ASSERT (theme != 0);
    retval = theme->findAvailableThemes();
    #endif

    QDir themeDir ("/usr/share/themes");
    foreach (QString themeFile, themeDir.entryList (QDir::Dirs)) {
        ThemeDescriptor *descr;

        if (themeFile == "." || 
                themeFile == "..")
            continue;
        
        descr = new ThemeDescriptor (
                "/usr/share/themes/" + themeFile,
                themeFile);

        if (descr->isValid())
            retval << descr;
        else
            delete descr;
    }

    return retval;
}

void
ThemeBusinessLogic::changeTheme (
        QString themeId)
{
    SYS_DEBUG ("Activating theme '%s'", SYS_STR(themeId));
    DuiTheme *theme = DuiTheme::instance();

    Q_ASSERT (theme != 0);

    #if 0
    theme->changeTheme(themeId);
    #else
    DuiGConfItem  gconfItem ("/Dui/theme/name");
    gconfItem.set (themeId);
    #endif
    emit themeChanged (themeId);
}


