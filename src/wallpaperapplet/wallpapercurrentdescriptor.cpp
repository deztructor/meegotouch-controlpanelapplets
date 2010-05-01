/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpapercurrentdescriptor.h"

#include <QPointF>
#include <QFileInfo>
#include <MDesktopEntry>

//#define DEBUG
#include "../debug.h"

WallpaperCurrentDescriptor *WallpaperCurrentDescriptor::sm_Instance = 0;

static const QString mainGroupKey = "Desktop Entry";
static const QString landscapeGroupKey = "DCP Landscape Wallpaper";
static const QString portraitGroupKey = "DCP Portrait Wallpaper";

static const QString nameKey = "Name";
static const QString versionKey = "Version";
static const QString originalFilenameKey = "OriginalFile";
static const QString editedFilenameKey = "EditedFile";
static const QString mimeTypeKey = "MimeType";
static const QString horOffsetKey = "HorOffset";
static const QString vertOffsetKey = "VertOffset";
static const QString scaleKey = "Scale";

WallpaperCurrentDescriptor *
WallpaperCurrentDescriptor::instance ()
{
    if (!WallpaperCurrentDescriptor::sm_Instance)
        WallpaperCurrentDescriptor::sm_Instance = 
            new WallpaperCurrentDescriptor;

    return WallpaperCurrentDescriptor::sm_Instance;
}

WallpaperCurrentDescriptor::WallpaperCurrentDescriptor () :
    m_DesktopEntry (0),
    m_Version (0)
{
    m_LandscapeTrans.setOrientation (M::Landscape);
    m_PortraitTrans.setOrientation (M::Portrait);
}

bool
WallpaperCurrentDescriptor::isCurrent () const
{
    return true;
}

int
WallpaperCurrentDescriptor::version () const 
{
    return m_Version;
}

bool 
WallpaperCurrentDescriptor::setFromDestopFile (
        QString desktopFileName)
{
    QString        value;
    qreal          rval;
    bool           retval = false;

    SYS_DEBUG ("*** desktopFileName = %s", SYS_STR(desktopFileName));
    m_DesktopEntry = new MDesktopEntry (desktopFileName);

    if (!m_DesktopEntry->isValid()) {
        goto finalize;
    }

    /*
     * The edited image file name... FIXME: This is actually depends on the
     * orientation.
     * We should compare these with the values stored in the gconf database.
     */
    if (!getValue(landscapeGroupKey, editedFilenameKey, value)) {
        goto finalize;
    }
    m_landscapeEditedFile = value;
    setFilename (value);
    setUrl ("file://" + value);
   
    /*
     * The portrait edited file name. 
     * We should compare these with the values stored in the gconf database.
     */
    if (!getValue(portraitGroupKey, editedFilenameKey, value)) {
        goto finalize;
    }
    m_portraitEditedFile = value;

    /*
     * MimeType. FIXME: This should not depend on the orientation?
     */
    if (getValue(landscapeGroupKey, mimeTypeKey, value)) {
        setMimeType (value);
    }

    /*
     * The name. This actually is all right.
     */
    if (getValue(mainGroupKey, nameKey, value)) {
        setTitle (value);
    }
    
    /*
     * The version number.
     */
    if (getValue(mainGroupKey, versionKey, &rval))
        m_Version = (int) rval;

    getValue (landscapeGroupKey, originalFilenameKey, m_LandscapeOriginalFile);
    getValue (portraitGroupKey, originalFilenameKey, m_PortraitOriginalFile);
    getValue (landscapeGroupKey, m_LandscapeTrans);
    getValue (portraitGroupKey, m_PortraitTrans);

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
        #if 0
        SYS_DEBUG ("key = %s/%s value = %s",
                SYS_STR (group),
                SYS_STR (key),
                SYS_STR (value));
        #endif
        retval = true;
    }

    return retval;
}

bool 
WallpaperCurrentDescriptor::getValue (
            const QString   &group,
            WallpaperITrans &value)
{
    bool   retval;
    qreal  rval1, rval2;

    retval = getValue (group, horOffsetKey, &rval1);
    if (!retval) {
        SYS_WARNING ("Key not found %s/%s", 
                SYS_STR(group), 
                SYS_STR(horOffsetKey));
        return retval;
    }
    
    retval = getValue (group, vertOffsetKey, &rval1);
    if (!retval) {
        SYS_WARNING ("Key not found %s/%s", 
                SYS_STR(group), 
                SYS_STR(vertOffsetKey));
        return retval;
    }
    value.setOffset (QPointF(rval1, rval2));
    
    retval = getValue (group, scaleKey, &rval1);
    if (!retval) {
        SYS_WARNING ("Key not found %s/%s", 
                SYS_STR(group), 
                SYS_STR(scaleKey));
        return retval;
    }
    value.setScale (rval1);

    return retval;
}

bool
WallpaperCurrentDescriptor::getValue (
            const QString &group,
            const QString &key, 
            qreal         *value)
{
    QString sValue;

    if (!getValue(group, key, sValue)) {
        *value = 0.0;
        return false;
    }

    *value = sValue.toDouble();
    return true;
}

WallpaperITrans 
WallpaperCurrentDescriptor::iTrans (
        M::Orientation orientation) const
{
    switch (orientation) {
        case M::Portrait:
            return m_PortraitTrans;

        case M::Landscape:
            return m_LandscapeTrans;
    }

    SYS_WARNING ("Unknown orientation: %d", orientation);
    return m_LandscapeTrans;
}

QString 
WallpaperCurrentDescriptor::originalImageFile (
        M::Orientation orientation) const
{
    switch (orientation) {
        case M::Portrait:
            return m_PortraitOriginalFile;

        case M::Landscape:
            return m_LandscapeOriginalFile;
    }

    SYS_WARNING ("Unknown orientation: %d", orientation);
    return m_LandscapeOriginalFile;
}


QString
WallpaperCurrentDescriptor::suggestedOutputFilename (
        M::Orientation orientation) const
{
    QFileInfo fileInfo (originalImageFile(orientation));
    QString   retval;

    switch (orientation) {
        case M::Landscape:
            retval = "-landscape.";
            break;

        case M::Portrait:
            retval = "-portrait.";
            break;
    }

    SYS_WARNING ("*** original image = %s", 
            SYS_STR (originalImageFile(orientation)));
    retval = fileInfo.baseName() + retval + 
        QString::number(version()) + "." + extension();

    return retval;
}

QString
WallpaperCurrentDescriptor::editedFilename (
        M::Orientation orientation) const
{
    switch (orientation) {
        case M::Landscape:
            return m_landscapeEditedFile;

        case M::Portrait:
            return m_portraitEditedFile;
    }

    return QString();
}

