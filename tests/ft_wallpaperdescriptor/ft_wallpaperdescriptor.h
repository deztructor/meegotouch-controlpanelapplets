/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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

