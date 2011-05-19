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
#include "wallpaperdescriptor_p.h"

#define DEBUG
#define WARNING
#include "../debug.h"

WallpaperDescriptorPrivate::WallpaperDescriptorPrivate () :
    m_ReferenceCounter (1)
{
}

WallpaperDescriptorPrivate::WallpaperDescriptorPrivate (
        const QString &filePath) :
    m_ReferenceCounter (1),
    m_FilePath (filePath)
{
}


void
WallpaperDescriptorPrivate::ref ()
{
    ++m_ReferenceCounter;
}

int 
WallpaperDescriptorPrivate::unRef ()
{
    return --m_ReferenceCounter;
}

/*********************************************************************************
 * Standard set/get methods.
 */
void 
WallpaperDescriptorPrivate::setFilePath (
        const QString &filePath)
{
    m_FilePath = filePath;
}
        
QString 
WallpaperDescriptorPrivate::filePath () const
{
    return m_FilePath;
}

