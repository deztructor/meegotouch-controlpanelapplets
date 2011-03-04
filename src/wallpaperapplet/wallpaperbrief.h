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
#ifndef WALLPAPERBRIEF_H
#define WALLPAPERBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "wallpaperbusinesslogic.h"


class WallpaperBrief: public DcpBrief
{
    Q_OBJECT

public:
#if 0
    WallpaperBrief (WallpaperBusinessLogic *businessLogic);
#endif

    virtual int widgetTypeID() const;
#if 0
    /*
     * Had to remove the whole stuff: see NB#230415 for details.
     */
    virtual QString valueText() const;
    virtual QString icon () const;
    virtual QString image () const;

private:
    QPointer<WallpaperBusinessLogic> m_WallpaperBusinessLogic;
#endif
};

#endif
