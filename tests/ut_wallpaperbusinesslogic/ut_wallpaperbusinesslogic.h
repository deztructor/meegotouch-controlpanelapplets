/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef UT_WALLPAPERBUSINESSLOGIC_H
#define UT_WALLPAPERBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class WallpaperBusinessLogic;

class Ut_WallpaperBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testGConfItems ();
    void testDirPath ();
    void testEditedImage ();
    void testAvailableWallpapers ();
    void testITrans ();

private:
    WallpaperBusinessLogic    *m_Api;
    MApplication              *m_App;
};

#endif
