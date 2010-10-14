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

#ifndef UT_WALLPAPERWIDGET_H
#define UT_WALLPAPERWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

#include "wallpaperdescriptor.h"

class MApplication;
class WallpaperBusinessLogic;
class WallpaperWidget;
class WallpaperDescriptor;

class SignalSink : public QObject
{
    Q_OBJECT
public:
    SignalSink ();

    void reset();

public slots:
    void changeWidget (int widgetId);
    void imageEditRequested ();

public:
    bool   m_ChangeWidgetCame;
    int    m_WidgetID;
    bool   m_WallpaperImageEditRequestedCame;
};

class Ut_WallpaperWidget : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testImageActivated ();
   
    #ifdef HAVE_CONTENT_MANAGER 
    void testGalleryImageSelected ();
    #endif

private:
    MApplication              *m_App;
    WallpaperBusinessLogic    *m_BusinessLogic;
    WallpaperWidget           *m_Widget;
    SignalSink                 m_Sink;
};

#endif



