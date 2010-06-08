/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ft_wallpapermodel.h"

#include "wallpaperbusinesslogic.h"
#include "wallpapermodel.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperitrans.h"

#include <QPixmap>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * SignalSink implementation.
 */
SignalSink::SignalSink ()
{
    reset ();
}

void 
SignalSink::reset ()
{
    m_WallpaperChangedCame = false;
}

void 
SignalSink::wallpaperChanged ()
{
    SYS_DEBUG ("");
    m_WallpaperChangedCame = true;
}

/******************************************************************************
 * Ft_WallpaperModel implementation. 
 */
void 
Ft_WallpaperModel::init()
{
}

void 
Ft_WallpaperModel::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ft_WallpaperModel";

void 
Ft_WallpaperModel::initTestCase()
{
    bool connectSuccess;

    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic;
    m_Model = new WallpaperModel (m_BusinessLogic);

    connectSuccess = connect (
            m_BusinessLogic, SIGNAL (wallpaperChanged()),
            &m_SignalSink, SLOT (wallpaperChanged()));
    QVERIFY (connectSuccess);
}

void 
Ft_WallpaperModel::cleanupTestCase()
{
    delete m_Model;
    delete m_BusinessLogic;

    m_App->deleteLater ();
}

void 
Ft_WallpaperModel::testRowsColumns ()
{
    QModelIndex    index;
    int            columns = m_Model->columnCount (index);
    int            rows = m_Model->rowCount (index);

    SYS_DEBUG ("*** columns = %d", columns);
    SYS_DEBUG ("*** rows    = %d", rows);

    QVERIFY (columns == 1);
    QVERIFY (rows >= 1);
}



QTEST_APPLESS_MAIN(Ft_WallpaperModel)



