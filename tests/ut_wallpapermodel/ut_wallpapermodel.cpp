/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpapermodel.h"
#include "wallpapermodel.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_WallpaperModel implementation. 
 */
void 
Ut_WallpaperModel::init()
{
}

void 
Ut_WallpaperModel::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WallpaperModel";

void 
Ut_WallpaperModel::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic;
    m_Model = new WallpaperModel (m_BusinessLogic);
}

void 
Ut_WallpaperModel::cleanupTestCase()
{
    delete m_BusinessLogic;
    delete m_Model;
    m_App->deleteLater ();
}



void 
Ut_WallpaperModel::testRowsColumns ()
{
    QModelIndex    index;
    int            columns = m_Model->columnCount (index);
    int            rows = m_Model->rowCount (index);

    SYS_DEBUG ("*** columns = %d", columns);
    SYS_DEBUG ("*** rows    = %d", rows);

    QVERIFY (columns == 1);
    //QVERIFY (rows >= 1);
}

void 
Ut_WallpaperModel::testData ()
{
    QModelIndex          index;
    QVariant             data;
    WallpaperDescriptor *desc;
    int            rows = m_Model->rowCount (index);

    for (int n = 0; n < rows; ++n) {
        index = m_Model->index (n, 0);
        data = index.data();
        desc = data.value<WallpaperDescriptor*>();

        #if 1
        SYS_DEBUG ("--- index at %d -----------------", n);
        SYS_DEBUG ("*** position = %d, %d", index.column(), index.row());
        SYS_DEBUG ("*** file     = %s", SYS_STR(desc->filename()));
        #endif
        
        QVERIFY (index.isValid ());
        QVERIFY (index.column() == 0);
        QVERIFY (index.row() == n);
        QVERIFY (index.model() == m_Model);
        QVERIFY (isWallpaperDescriptorValid(desc));
    }
}

bool
Ut_WallpaperModel::isWallpaperDescriptorValid (
        WallpaperDescriptor *desc)
{
    bool valid = true;

    if (desc == 0) {
        SYS_WARNING ("The pointer should not be NULL.");
        return false;
    }
    
    /*
     * Checking the filename
     */
    QString filename = desc->filename();
    if (filename.isEmpty())
        valid = false;
    else if (!filename.startsWith("/"))
        valid = false;
    else if (filename.endsWith("/"))
        valid = false;
    else if (filename.contains("*") || filename.contains("?"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid filename: %s", SYS_STR(filename));
        return false;
    }

    /*
     * Checking basename
     */
    QString basename = desc->basename();
    if (basename.isEmpty())
        valid = false;
    else if (basename.contains("/"))
        valid = false;
    else if (basename.contains("*") || filename.contains("?"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid basename: %s", SYS_STR(basename));
        return false;
    }

    /*
     * Checking the title.
     */
    QString title = desc->title();
    if (title.isEmpty())
        valid = false;
    
    if (!valid) {
        SYS_WARNING ("Invalid title: %s", SYS_STR(title));
        return false;
    }

    /*
     * Checking the MimeType
     */
    QString mimeType = desc->mimeType ();
    if (mimeType.isEmpty())
        valid = false;
    else if (!mimeType.startsWith("image/"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid mimeType = %s", SYS_STR(mimeType));
        return false;
    }

    return true;
}

QTEST_APPLESS_MAIN(Ut_WallpaperModel)


