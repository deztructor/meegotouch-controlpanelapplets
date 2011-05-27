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
#ifndef WALLPAPEREDITORWIDGETSTYLE_H
#define WALLPAPEREDITORWIDGETSTYLE_H

#include <mwidgetstyle.h>

class WallpaperEditorWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(WallpaperEditorWidgetStyle)

    M_STYLE_ATTRIBUTE(qreal, pointerSpringK,    PointerSpringK)
    M_STYLE_ATTRIBUTE(qreal, friction,          Friction)
    M_STYLE_ATTRIBUTE(qreal, slidingFriction,   SlidingFriction)
    M_STYLE_ATTRIBUTE(qreal, borderSpringK,     BorderSpringK)
    M_STYLE_ATTRIBUTE(qreal, borderFriction,    BorderFriction)
    M_STYLE_ATTRIBUTE(qreal, maximumVelocity,   MaximumVelocity)
};

class WallpaperEditorWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(WallpaperEditorWidgetStyle)
};


#endif
