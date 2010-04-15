/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <MTheme>
#include <MGConfItem>

#include <QProcessEnvironment>
#include <QDir>

#define DEBUG
#include "../debug.h"

static const QString PortraitKey = "/desktop/standard/background/portrait";
static const QString LandscapeKey = "/desktop/standard/background/landscape";
static const QString PictureKey = "/picture_filename";

WallpaperBusinessLogic::WallpaperBusinessLogic()
{
    m_LandscapeGConfItem = new MGConfItem (LandscapeKey + PictureKey);
    m_PortraitGConfItem = new MGConfItem (PortraitKey + PictureKey);
    m_EditedImage = 0;
}

WallpaperBusinessLogic::~WallpaperBusinessLogic()
{
    delete m_LandscapeGConfItem;
    delete m_PortraitGConfItem;
}


bool
WallpaperBusinessLogic::hasWallpaperFileName (
        bool     portrait)
{
    QString filename = WallpaperFileName(portrait);

    return !filename.isEmpty();
}

QString
WallpaperBusinessLogic::WallpaperFileName (
        bool     portrait)
{
    return portrait ? 
        m_PortraitGConfItem->value().toString() :
        m_LandscapeGConfItem->value().toString();
}

WallpaperDescriptor
WallpaperBusinessLogic::Wallpaper (
        bool portrait)
{
    QString filename = WallpaperFileName (portrait);
    return WallpaperDescriptor (filename);
}

void
WallpaperBusinessLogic::setBackground (
        WallpaperDescriptor *desc)
{
    if (desc == 0)
        desc = m_EditedImage;

    Q_ASSERT (m_PortraitGConfItem != 0);
    Q_ASSERT (m_LandscapeGConfItem != 0);

    SYS_DEBUG ("*** filename = %s", SYS_STR(desc->filename()));
    m_PortraitGConfItem->set (desc->filename());
    m_LandscapeGConfItem->set (desc->filename());
}

/*!
 * Returns a list of filenames (full path filenames to be precise) of the
 * available wallpapers. These are the raw image files, before they were edited.
 *
 * FIXME: This function needs some polishing.
 */
QStringList
WallpaperBusinessLogic::availableWallpapers () const
{
    QStringList list;
    list <<
        "/usr/share/themes/base/m/images/mapplicationpage-background.png" <<
        "/usr/share/themes/plankton/m/images/mapplicationpage-background.png" <<
        "/usr/share/themes/plankton/m/images/mapplicationpage-portrait-background.png" <<
        "/usr/share/themes/plankton/m/images/mapplicationpage-portrait-background.png";

    /*
     * FIXME:
     * I have no idea how we sould get the available images, this will do as a
     * test.
     */
    QString             home = getenv("HOME");
    QString 	        dirname = home + "/MyDocs";
    QStringList         filterList;


    filterList << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG" << "*.png" <<
        "*.PNG";

    SYS_DEBUG ("*** dirname = %s", SYS_STR(dirname));

    QDir imageDir (dirname);
    foreach (QString imageFile, imageDir.entryList(filterList, QDir::Files)) {
        list << (dirname + "/" + imageFile);
    }

    return list;
}

void
WallpaperBusinessLogic::setEditedImage (
        WallpaperDescriptor  *desc)
{
    SYS_DEBUG ("*** desc = %s", SYS_STR(desc->filename()));
    m_EditedImage = desc;
}

WallpaperDescriptor *
WallpaperBusinessLogic::editedImage ()
{
    SYS_DEBUG ("*** m_EditedImage = %s", SYS_STR(m_EditedImage->filename()));
    return m_EditedImage;
}
