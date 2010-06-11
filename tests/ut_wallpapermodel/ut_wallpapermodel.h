/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_WALLPAPERMODEL_H
#define UT_WALLPAPERMODEL_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class WallpaperBusinessLogic;
class WallpaperModel;
class WallpaperDescriptor;

class Ut_WallpaperModel : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testRowsColumns ();
    void testData ();

private:
    bool isWallpaperDescriptorValid (WallpaperDescriptor *desc);

    MApplication              *m_App;
    WallpaperBusinessLogic    *m_BusinessLogic;
    WallpaperModel            *m_Model;
};

#endif

