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
#include "wallpaperworkerthread.h"

#define DEBUG
#define WARNING
#include "../debug.h"

WallpaperWorkerThread::WallpaperWorkerThread (
        QPixmap       &pixmap, 
        const QString &fileName) :
    m_Pixmap (pixmap),
    m_OutputFileName (fileName),
    m_Success (false)
{
}


void
WallpaperWorkerThread::run ()
{
    SYS_DEBUG ("*** m_Pixmap         = %dx%d", 
            m_Pixmap.width(), m_Pixmap.height());
    SYS_DEBUG ("*** m_OutputFileName = %s", SYS_STR(m_OutputFileName));

    if (!m_OutputFileName.isEmpty() && 
            m_Pixmap.width() > 0 && m_Pixmap.height() > 0)
        runSavePixmap ();
}

void
WallpaperWorkerThread::runSavePixmap ()
{
    m_Success = m_Pixmap.save (m_OutputFileName);

}

bool 
WallpaperWorkerThread::success () const
{
    return m_Success;
}

