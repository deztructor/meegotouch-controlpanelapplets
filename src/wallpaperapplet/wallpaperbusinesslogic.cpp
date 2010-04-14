/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>
#include <DuiTheme>
#include <DuiGConfItem>

#define DEBUG
#include "../debug.h"

static const QString PortraitKey = "/desktop/standard/background/portrait";
static const QString LandscapeKey = "/desktop/standard/background/landscape";
static const QString PictureKey = "/picture_filename";

WallpaperBusinessLogic::WallpaperBusinessLogic()
{
    m_LandscapeGConfItem = new DuiGConfItem (LandscapeKey + PictureKey);
    m_PortraitGConfItem = new DuiGConfItem (PortraitKey + PictureKey);
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
        WallpaperDescriptor &desc)
{
    Q_ASSERT (m_PortraitGConfItem != 0);
    Q_ASSERT (m_LandscapeGConfItem != 0);

    SYS_DEBUG ("*** filename = %s", SYS_STR(desc.filename()));
    m_PortraitGConfItem->set (desc.filename());
    m_LandscapeGConfItem->set (desc.filename());
}

QStringList
WallpaperBusinessLogic::availableWallpapers () const
{
    QStringList list;

    list <<
        "/usr/share/themes/base/dui/images/duiapplicationpage-background.png" <<
        "/usr/share/themes/plankton/dui/images/duiapplicationpage-background.png" <<
        "/usr/share/themes/plankton/dui/images/duiapplicationpage-portrait-background.png" <<
        "/usr/share/themes/plankton/dui/images/duiapplicationpage-portrait-background.png";

    return list;
}
