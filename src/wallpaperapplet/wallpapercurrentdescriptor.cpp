/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapercurrentdescriptor.h"

#include <MDesktopEntry>

#define DEBUG
#include "../debug.h"

WallpaperCurrentDescriptor *WallpaperCurrentDescriptor::sm_Instance = 0;

static const QString mainGroupKey = "Desktop Entry";
static const QString landscapeGroupKey = "DCP Landscape Wallpaper";

static const QString nameKey = "Name";
static const QString originalFilenameKey = "OriginalFile";
static const QString editedFilenameKey = "EditedFile";
static const QString mimeTypeKey = "MimeType";


WallpaperCurrentDescriptor *
WallpaperCurrentDescriptor::instance ()
{
    if (!WallpaperCurrentDescriptor::sm_Instance)
        WallpaperCurrentDescriptor::sm_Instance = 
            new WallpaperCurrentDescriptor;

    return WallpaperCurrentDescriptor::sm_Instance;
}

WallpaperCurrentDescriptor::WallpaperCurrentDescriptor () :
    m_DesktopEntry (0)
{
}

bool
WallpaperCurrentDescriptor::isCurrent () const
{
    return true;
}

bool 
WallpaperCurrentDescriptor::setFromDestopFile (
        QString desktopFileName)
{
    QString        value;
    bool           retval = false;

    SYS_DEBUG ("*** desktopFileName = %s", SYS_STR(desktopFileName));
    m_DesktopEntry = new MDesktopEntry (desktopFileName);

    if (!m_DesktopEntry->isValid()) {
        goto finalize;
    }

    if (!getValue(landscapeGroupKey, editedFilenameKey, value)) {
        goto finalize;
    }
    setFilename (value);
    setUrl ("file://" + value);
    
    if (getValue(landscapeGroupKey, mimeTypeKey, value)) {
        setMimeType (value);
    }

    if (getValue(mainGroupKey, nameKey, value)) {
        setTitle (value);
    }


    retval = true;

finalize:
    delete m_DesktopEntry;
    m_DesktopEntry = 0;
    return retval;
}

bool 
WallpaperCurrentDescriptor::getValue (
        const QString  &group,
        const QString  &key, 
        QString        &value)
{
    QString  fullKey = group + "/" + key;
    bool     retval = false;

    if (!m_DesktopEntry) {
        SYS_WARNING ("The m_DesktopEntry is 0.");
        return retval;
    }

    value = m_DesktopEntry->value (fullKey);
    if (value.isEmpty()) {
        SYS_WARNING ("The key %s is not set.", SYS_STR(fullKey));
    } else {
        retval = true;
    }

    return retval;
}
