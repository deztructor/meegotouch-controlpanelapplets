/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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

#ifndef __MDESKTOPENTRYSTUB_H__
#define __MDESKTOPENTRYSTUB_H__

#include <QObject>
#include <QFlags>

/*
 * We return an invalid MDesktopEntry for this. Actually any values that are not
 * enlisted below would suffice.
 */
#define DIRNAMEDesktopFileMissing  "/nosuchfile"
#define CODENAMEDesktopFileMissing "nosuchentry"

/*
 * With this we return strings for every keys.
 */
#define DIRNAMEDesktopFilePerfect  "/usr/share/themes/plankton"
#define CODENAMEDesktopFilePerfect "plankton"
#define NAMEDesktopFilePerfect "DesktopFilePerfectName"
#define ICONDesktopFilePerfect "DesktopFilePerfectIcon"

/*
 * For these we return strings that represent a hidden theme; all strings are
 * valid, but the theme is hidden.
 */
#define DIRNAMEDesktopFileHidden  "/usr/share/themes/base"
#define CODENAMEDesktopFileHidden "base"
#define NAMEDesktopFileHidden "DesktopFileHiddenName"
#define ICONDesktopFileHidden "DesktopFileHiddenIcon"

/*
 * For these all the keys will hold no value, we return the empty string.
 */
#define DIRNAMEDesktopFileEmpty  "/usr/share/themes/dui"
#define CODENAMEDesktopFileEmpty "dui"

class QString;

/*!
 * This file contains the definition for a simulated MDesktopentry database that
 * holds simulated values for some desktop files.
 */
class MDesktopEntry: public QObject 
{
Q_OBJECT
public:
    MDesktopEntry (const QString &filePath);

    bool isValid ();

    typedef enum {
        DesktopFileMissing = -1,
        DesktopFilePerfect = 0,
        DesktopFileHidden  = 1,
        DesktopFileEmpty   = 2,
    } DesktopID;

    //QString value (const QString &group, const QString &key) const;
    QString value (const QString &key) const;

private:
    QString getValueForType () const;
    QString getValueForName () const;
    QString getValueForIcon () const;
    QString getValueForVisible () const;

    QString   m_FilePath;
};

#endif

