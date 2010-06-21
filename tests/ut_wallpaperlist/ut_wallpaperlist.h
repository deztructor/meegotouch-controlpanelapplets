/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_WALLPAPERLIST_H
#define UT_WALLPAPERLIST_H

#include <QtTest/QtTest>
#include <QObject>

#include "wallpaperdescriptor.h"

class MApplication;
class WallpaperBusinessLogic;
class WallpaperList;
class WallpaperDescriptor;

class SignalSink : public QObject
{
    Q_OBJECT
public:
    SignalSink ();

public slots:
    void imageActivated (WallpaperDescriptor *desc);

public:
    WallpaperDescriptor   *m_Desc;
};

class Ut_WallpaperList : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testItemClicked ();
    void testShowHide ();
    
private:
    MApplication              *m_App;
    WallpaperBusinessLogic    *m_BusinessLogic;
    WallpaperList             *m_List;
    SignalSink                 m_Sink;
};

#endif


