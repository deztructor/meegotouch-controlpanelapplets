/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MGConfItem>
#include <MTheme>

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>

#define DEBUG
#define WARNING
#include "../debug.h"

#if !defined(UNIT_TEST) || defined(FUNCTIONAL_TEST)
#  define entryList(themeDir, filters) \
      themeDir.entryList (filters)
#else
#  include "qdirstub.h"
#endif


// The directory where all the available themes are installed.
static const QString themeDirName (MEEGO_THEMEDIR);

// The GConf key where meegotouch expects us to place the theme name.
static const QString themeGConfKey ("/meegotouch/theme/name");

ThemeBusinessLogic::ThemeBusinessLogic ()
{
    connect (MTheme::instance(), SIGNAL(themeChangeCompleted()),
            this, SLOT(themeChangeCompleted()));
}

/*!
 * Returns the code name of the current theme. This code name can be used as a
 * string ID to the GConf database.
 */
QString
ThemeBusinessLogic::currentThemeCodeName ()
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
ThemeBusinessLogic::currentThemeName ()
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
 * Returns the official name of the current theme. This name can be used as a UI
 * string.
 */
QString
ThemeBusinessLogic::currentThemeIconName ()
{
    QString codeName = currentThemeCodeName();
    QList<ThemeDescriptor *> list = availableThemes ();
    QString retval;

    foreach (ThemeDescriptor *descr, list) {
        if (descr->codeName() == codeName)
            retval = descr->iconName();

        delete descr;
    }

    return retval;
}

/*!
 * Returns all the available themes. 
 * Invisible themes are filtered out.
 */
QList<ThemeDescriptor *>
ThemeBusinessLogic::availableThemes ()
{
    QList<ThemeDescriptor *> retval;
    QDir themeDir (themeDirName);

    SYS_DEBUG ("------------------------------------------------");
    foreach (QString themeFile, 
            entryList(themeDir, (QDir::Dirs | QDir::NoDotAndDotDot))) {
        ThemeDescriptor *descr;

        SYS_DEBUG ("themeFile = %s", SYS_STR(themeFile));
        descr = new ThemeDescriptor (
                this,
                themeDirName + QDir::separator() + themeFile,
                themeFile);

        if (descr->isValid() && descr->isVisible())
            retval << descr;
        else
            delete descr;
    }
    SYS_DEBUG ("------------------------------------------------");

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
    emit themeChangeStarted (themeCodeName);
    
    MGConfItem  gconfItem (themeGConfKey);
    gconfItem.set (themeCodeName);
}

void 
ThemeBusinessLogic::themeChangeCompleted ()
{
    QString     themeCodeName = currentThemeCodeName ();

    SYS_DEBUG ("Theme changed to %s", SYS_STR(themeCodeName));
    emit themeChanged (themeCodeName);
}
