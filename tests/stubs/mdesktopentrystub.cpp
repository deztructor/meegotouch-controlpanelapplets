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
#include "mdesktopentrystub.h"

#include <QString>
#include <QStringList>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * We have a file system here! ;)
 */
static bool         fileSystemInitialized = false;
static QStringList  fileSystemFiles;


void
initFileSystem ()
{
    if (fileSystemInitialized)
        return;

    SYS_DEBUG ("Initializing file system.");
    fileSystemFiles <<
        "/usr/share/themes/plankton/index.theme" <<
        "/usr/share/themes/base/index.theme" <<
        "/usr/share/themes/devel/index.theme" <<
        "/usr/share/themes/dui/index.theme";
}

MDesktopEntry::DesktopID
getDesktopID (
        const QString &filePath)
{
    MDesktopEntry::DesktopID retval;

    initFileSystem ();

    retval = (MDesktopEntry::DesktopID) fileSystemFiles.indexOf (filePath);
    SYS_DEBUG ("*** filePath = %s", SYS_STR(filePath));
    SYS_DEBUG ("*** retval   = %d", retval);

    switch (retval) {
        case MDesktopEntry::DesktopFileMissing:
            SYS_DEBUG ("MDesktopEntry::DesktopFileMissing");
            break;

        case MDesktopEntry::DesktopFilePerfect:
            SYS_DEBUG ("MDesktopEntry::DesktopFilePerfect");
            break;

        case MDesktopEntry::DesktopFileHidden:
            SYS_DEBUG ("MDesktopEntry::DesktopFileHidden");
            break;
        
        case MDesktopEntry::DesktopFileEmpty:
            SYS_DEBUG ("MDesktopEntry::DesktopFileEmpty");
            break;

        default:
            SYS_DEBUG ("Unhandled case");
    }
    
    return retval;
}

/******************************************************************************
 * MDesktopEntry implementation.
 */
MDesktopEntry::MDesktopEntry (
        const QString &filePath)
{
    initFileSystem ();

    SYS_DEBUG ("*** filePath = %s", SYS_STR(filePath));
    m_FilePath = filePath;
}


bool
MDesktopEntry::isValid ()
{
    DesktopID id;

    id = getDesktopID (m_FilePath);
    SYS_DEBUG ("*** filePath = %s", SYS_STR(m_FilePath));
    SYS_DEBUG ("*** id       = %d", id);

    switch (id) {
        case DesktopFileMissing:
            return false;

        case DesktopFilePerfect:
        case DesktopFileHidden:
        case DesktopFileEmpty:
            return true;
    }

    return false;
}

QString 
MDesktopEntry::value (
        const QString &key) const
{
    QString value;

    SYS_DEBUG ("*** getting %s", SYS_STR(key));
    if (key == "Desktop Entry/Type")
        value = getValueForType (); 
    else if (key == "Desktop Entry/Name")
        value = getValueForName (); 
    else if (key == "X-MeeGoTouch-Metatheme/X-Icon")
        value = getValueForIcon (); 
    else if (key == "X-MeeGoTouch-Metatheme/X-Visible")
        value = getValueForVisible (); 
    else {
        SYS_WARNING ("Key not suported: %s", SYS_STR(key));
    }

    return value;
}

/******************************************************************************
 * Private functions.
 */
QString 
MDesktopEntry::getValueForType () const
{
    QString retval;
    DesktopID id;

    id = getDesktopID (m_FilePath);
    
    switch (id) {
        case DesktopFileMissing:
            // The empty string
            break;

        case DesktopFilePerfect:
        case DesktopFileHidden:
            retval = "X-MeeGoTouch-Metatheme";
            break;
        
        case DesktopFileEmpty:
            // All strings are empty
            break;
    }

    SYS_DEBUG ("Returning %s", SYS_STR(retval));
    return retval;
}

QString 
MDesktopEntry::getValueForName () const
{
    QString retval;
    DesktopID id;

    id = getDesktopID (m_FilePath);
    
    switch (id) {
        case DesktopFileMissing:
            // The empty string
            break;

        case DesktopFilePerfect:
            retval = NAMEDesktopFilePerfect;
            break;
        
        case DesktopFileHidden:
            retval = NAMEDesktopFileHidden;
            break;
        
        case DesktopFileEmpty:
            // All strings are empty
            break;
    }

    SYS_DEBUG ("Returning %s", SYS_STR(retval));
    return retval;
}

QString 
MDesktopEntry::getValueForIcon () const
{
    QString retval;
    DesktopID id;

    id = getDesktopID (m_FilePath);
    
    switch (id) {
        case MDesktopEntry::DesktopFileMissing:
            // The empty string
            break;

        case MDesktopEntry::DesktopFilePerfect:
            retval = ICONDesktopFilePerfect; 
            break;

        case MDesktopEntry::DesktopFileHidden:
            retval = ICONDesktopFileHidden;
            break;
        
        case MDesktopEntry::DesktopFileEmpty:
            // All strings are empty
            break;

        default:
            SYS_DEBUG ("Unhandled case");
    }

    SYS_DEBUG ("Returning %d/%s", id, SYS_STR(retval));
    return retval;
}

QString 
MDesktopEntry::getValueForVisible () const
{
    QString retval;
    DesktopID id;

    id = getDesktopID (m_FilePath);
    
    switch (id) {
        case MDesktopEntry::DesktopFileMissing:
            // The empty string
            break;

        case MDesktopEntry::DesktopFilePerfect:
            retval = "true";
            break;
        
        case MDesktopEntry::DesktopFileHidden:
            retval = "false";
            break;
        
        case MDesktopEntry::DesktopFileEmpty:
            // All strings are empty
            break;
    }

    SYS_DEBUG ("Returning %s", SYS_STR(retval));
    return retval;
}
