/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpaperbusinesslogic.h"
#include "wallpaperbusinesslogic.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_WallpaperBusinessLogic implementation. 
 */
void 
Ut_WallpaperBusinessLogic::init()
{
}

void 
Ut_WallpaperBusinessLogic::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ut_WallpaperBusinessLogic";

void 
Ut_WallpaperBusinessLogic::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Api = new WallpaperBusinessLogic;
}

void 
Ut_WallpaperBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

void
Ut_WallpaperBusinessLogic::testSimpleQueries ()
{
#if 0
    /*
     * These are deprecated methods. Will write a real test soon.
     */
    QString thing;
    bool    bthing;

    thing = m_Api->WallpaperFileName ();
    qDebug() << "WallpaperFileName is " << thing;

    bthing = m_Api->hasWallpaperFileName();
    qDebug() << "hasWallpaperFileName is " << (bthing ? "yes" : "no");
#endif
}

QTEST_APPLESS_MAIN(Ut_WallpaperBusinessLogic)


