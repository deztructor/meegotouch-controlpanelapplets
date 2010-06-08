/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef FT_WALLPAPERBUSINESSLOGIC_H
#define FT_WALLPAPERBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class WallpaperBusinessLogic;

class SignalSink : public QObject
{
Q_OBJECT

public:
    SignalSink ();

    void reset ();
    
public slots:
    void wallpaperChanged ();

public:
    bool   m_WallpaperChangedCame;
};

class Ft_WallpaperBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testEditedImage ();
    void testCreateDirectory ();
    void testAvailableWallpapers ();
    void testCurrentWallpaper ();
    void testSetWallpapert ();

private:
    void testValidImages ();
    
    WallpaperBusinessLogic    *m_Api;
    MApplication              *m_App;
    SignalSink                 m_SignalSink;
};

#endif
