/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <MTheme>
#include <MGConfItem>

//#define DEBUG
#include "../debug.h"

static const QString themeDirName ("/usr/share/themes");

/*!
 * Returns the code name of the current theme. This code name can be used as a
 * string ID to the GConf database.
 */
QString
ThemeBusinessLogic::currentThemeCodeName () const
{
    MTheme *theme = MTheme::instance();

    Q_ASSERT (theme != 0);

    return theme->currentTheme();
}

/*!
 * Returns the official name of the current theme. This name can be used as a UI
 * string.
 */
QString
ThemeBusinessLogic::currentThemeName () const
{
    QString codeName = currentThemeCodeName();
    QList<ThemeDescriptor *> list = availableThemes ();
    QString retval;

    foreach (ThemeDescriptor *descr, list) {
        if (descr->codeName() == codeName)
            retval = descr->name();

        delete descr;
    }

    return retval;
}

/*!
 * Returns all the available themes. Please note, that some of these themes
 * might be hidden/invisible.
 */
QList<ThemeDescriptor *>
ThemeBusinessLogic::availableThemes () const
{
    QList<ThemeDescriptor *> retval;
    QDir themeDir (themeDirName);

    foreach (QString themeFile, themeDir.entryList (QDir::Dirs)) {
        ThemeDescriptor *descr;

        if (themeFile == "." || 
                themeFile == "..")
            continue;
        
        descr = new ThemeDescriptor (
                themeDirName + "/" + themeFile,
                themeFile);

        if (descr->isValid())
            retval << descr;
        else
            delete descr;
    }

    return retval;
}

/*!
 * This function can be used to change the current theme.
 */
void
ThemeBusinessLogic::changeTheme (
        QString themeCodeName)
{
    SYS_DEBUG ("Activating theme '%s'", SYS_STR(themeCodeName));
    
    MGConfItem  gconfItem ("/M/theme/name");
    gconfItem.set (themeCodeName);
    emit themeChanged (themeCodeName);
}

