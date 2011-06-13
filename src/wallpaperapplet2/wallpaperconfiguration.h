/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/
#ifndef WALLPAPER_CONFIGURATION_H
#define WALLPAPER_CONFIGURATION_H

#include <QString>

namespace Wallpaper 
{
    const bool useSheets (true);
    const bool useFullScreen (true);
    const bool supportEdit (true);
    const bool currentWallpaperAtTop (true);

    const int maxThumbnails = 100;
    const int nForwardThumbnails = 25;
    const int nBackwardThumbnails = 25;

    const QString CurrentPortraitKey (
            "/desktop/meego/background/portrait/picture_filename");

    const QString OriginalPortraitKey (
            "/desktop/meego/background/portrait/original_filename");

    const QString themeNameKey ("/meegotouch/theme/name");

    const QString themeImagePath (
            "/usr/share/themes/%1/meegotouch/images/backgrounds/%2.jpg");
    
    //const QString themeImagePath (
    //        "/usr/share/themes/%1/meegotouch/proto/%2.jpg");

    const QString ImagesDir ("~/MyDocs/.wallpapers");
    const QString ImagesSaveDir ("~/.wallpapers");
    const QString OutImgExt (".png");

    const QString DefaultThumbnailFlavor ("grid");
};

#endif
