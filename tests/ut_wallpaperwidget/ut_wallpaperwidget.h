/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

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

    void testGalleryActivated ();
    void testGalleryImageSelected ();
    void testImageBrowserDismissed();

private:
    MApplication              *m_App;
    WallpaperBusinessLogic    *m_BusinessLogic;
    WallpaperWidget           *m_Widget;
    SignalSink                 m_Sink;
};

#endif



