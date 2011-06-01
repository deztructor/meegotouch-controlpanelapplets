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
****************************************************************************/
#ifndef WALLPAPERVIEWWIDGETSTYLE_H
#define WALLPAPERVIEWWIDGETSTYLE_H

#include <mwidgetstyle.h>
#include <QColor>

class M_CORE_EXPORT WallpaperViewWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(WallpaperViewWidgetStyle)

    M_STYLE_ATTRIBUTE(QColor, imageBackgroundColor,    ImageBackgroundColor)
};

class M_CORE_EXPORT WallpaperViewWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(WallpaperViewWidgetStyle)
};


#endif
