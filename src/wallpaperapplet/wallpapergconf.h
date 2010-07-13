/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
**
** This file is part of duicontrolpanel.
**
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef WALLPAPERGCONF_H
#define WALLPAPERGCONF_H

/*
 * GConf keys for the current wallpaper images, one for the portrait and one for
 * the lanscape version.
 */
#define WALLPAPER_PORTRAIT_KEY \
	"/desktop/meego/background/portrait/picture_filename"
#define WALLPAPER_LANDSCAPE_KEY \
	"/desktop/meego/background/landscape/picture_filename"

/*
 * GConf key for the images that are currently edited.
 */
#define WALLPAPER_EDITED_PORTRAIT_KEY \
	"/desktop/meego/background/portrait/edited_picture_filename"
#define WALLPAPER_EDITED_LANDSCAPE_KEY \
	"/desktop/meego/background/landscape/edited_picture_filename"

/*
 * The GConf key and the enum type for request codes.
 */
#define WALLPAPER_APPLET_REQUEST_CODE \
	"/desktop/meego/background/applet_request_code"

typedef enum {
    WallpaperRequestNone = 0,
    WallpaperRequestEdit
} WallpaperAppletRequestCode;

#endif
