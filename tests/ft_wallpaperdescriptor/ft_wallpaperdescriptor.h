/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef FT_WALLPAPERDESCRIPTOR_H
#define FT_WALLPAPERDESCRIPTOR_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class WallpaperDescriptor;
class QUrl;
class QPixmap;
class QString;

class SignalSink : public QObject
{
Q_OBJECT

public:
    SignalSink ();

    void reset ();
    
public slots:
    void thumbnailLoaded (WallpaperDescriptor *desc);

public:
    bool   m_ThumbnailLoaded;
};

class Ft_WallpaperDescriptor : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testCacheUncache ();
    void testThumbnailing ();

private:
    void createDescriptor ();
    void dropDescriptor ();

    MApplication              *m_App;
    SignalSink                 m_SignalSink;
    WallpaperDescriptor       *m_Desc;
};

#endif

