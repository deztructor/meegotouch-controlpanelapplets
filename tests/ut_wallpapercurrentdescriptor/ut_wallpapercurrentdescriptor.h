/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef UT_WALLPAPERCURRENTDESCRIPTOR_H
#define UT_WALLPAPERCURRENTDESCRIPTOR_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class WallpaperDescriptor;
class WallpaperCurrentDescriptor;
class QUrl;
class QPixmap;
class QString;

class SignalSink : public QObject
{
Q_OBJECT

public:
    SignalSink ();

    void reset ();
    
signals:
    void thumbnailReady (
            QUrl         fileUri, 
            QUrl         thumbnailUri, 
            QPixmap      pixmap, 
            QString      flavor);
    void thumbnailError (
            QString      message,
            QUrl         url);
    void thumbnailLoadingFinished (
            int          left);

public slots:
    void thumbnailLoaded (WallpaperDescriptor *desc);

public:
    bool   m_ThumbnailLoaded;
};

class Ut_WallpaperCurrentDescriptor : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testDefaults ();
    void testFromDesktopFileInvalid ();
    void testFromDesktopFile ();
    void testFromFileNames ();

private:
    void createDescriptor ();
    void dropDescriptor ();
    QString wallpaperDesktopFileName ();
    MApplication                  *m_App;
    SignalSink                     m_SignalSink;
    WallpaperCurrentDescriptor    *m_Desc;
};

#endif


