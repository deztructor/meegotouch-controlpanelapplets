/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "themebusinesslogic.h"
#include "themedescriptor.h"

#include <MGConfItem>
#include <MTheme>
#include <QDBusConnection>
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


ThemeBusinessLogic::~ThemeBusinessLogic ()
{
    stopDBusAdaptor ();
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
    QString                   codeName = currentThemeCodeName();
    QString                   retval;

    availableThemes();
    foreach (ThemeDescriptor *descr, m_AvailableThemes) {
        if (descr->codeName() == codeName)
            retval = descr->name();
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
    QString retval;

    availableThemes ();
    foreach (ThemeDescriptor *descr, m_AvailableThemes) {
        if (descr->codeName() == codeName)
            retval = descr->iconName();
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
    QDir themeDir (themeDirName);

    /*
     * If we already got the list we are not going to read the filesystem again.
     */
    if (m_AvailableThemes.size() > 0) {
        return m_AvailableThemes;
    }

    /*
     * Here we do the actual reading.
     */
    foreach (QString themeFile, 
            entryList(themeDir, (QDir::Dirs | QDir::NoDotAndDotDot))) {
        ThemeDescriptor *descr;

        descr = new ThemeDescriptor (
                this,
                themeDirName + QDir::separator() + themeFile,
                themeFile);

        if (descr->isValid() && descr->isVisible()) {
            SYS_DEBUG ("themeFile = %s", SYS_STR(themeFile));
            m_AvailableThemes << descr;
        } else {
            delete descr;
        }
    }

    return m_AvailableThemes;
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

void
ThemeBusinessLogic::startupDBusAdaptor ()
{
    SYS_DEBUG ("Registering:");
    SYS_DEBUG ("*** service = %s", THEME_DBUS_SERVICE);
    SYS_DEBUG ("*** path    = %s", THEME_DBUS_PATH);

    if (m_ThemeBusinesslogicAdaptor)
        return;

    QDBusConnection bus = QDBusConnection::sessionBus ();
    if (!bus.registerService (THEME_DBUS_SERVICE)) {
        SYS_WARNING ("Failed to register dbus service");
    }

    if (!bus.registerObject (THEME_DBUS_PATH, this)) {
        SYS_WARNING ("Failed to register dbus object");
    }

    m_ThemeBusinesslogicAdaptor = new ThemeBusinessLogicAdaptor (this, this);
}
    
void
ThemeBusinessLogic::stopDBusAdaptor ()
{
    SYS_DEBUG ("");

    if (!m_ThemeBusinesslogicAdaptor)
        return;

    QDBusConnection bus = QDBusConnection::sessionBus ();
    if (!bus.unregisterService (THEME_DBUS_SERVICE)) {
        SYS_WARNING ("Failed to unregister dbus service");
    }

    bus.unregisterObject (THEME_DBUS_PATH);

    delete m_ThemeBusinesslogicAdaptor;
}


/*!
 * \param themeCodeName The code name of the theme for which we need the
 *   screenshot.
 * \param orientation Every theme should be two screenshots.
 * \returns The preview screenshot image file name for a given theme.
 *
 * Please note, that the preview can not handled through the normal MTheme
 * support, because we need the image _before_ the theme is set to be current.
 */
QString 
ThemeBusinessLogic::themePreviewFileName (
        const QString  &themeCodeName,
        M::Orientation  orientation) const
{
    QString oString;

    switch (orientation) {
        case M::Portrait:
            oString = "portrait";
            break;

        case M::Landscape:
            oString = "landscape";
    }

    return themeDirName + QDir::separator() + themeCodeName + 
        "/meegotouch/images/meegotouch-theme-preview-" + oString + ".jpg";
}

