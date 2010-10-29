/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "ut_wallpaperlist.h"
#include "wallpaperlist.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpapermodel.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * SignalSink implementation. 
 */
SignalSink::SignalSink () :
    m_Desc ()
{
}

void 
SignalSink::imageActivated (
        WallpaperDescriptor *desc)
{
    SYS_DEBUG ("");
    m_Desc = desc;
}

/******************************************************************************
 * Ut_WallpaperList implementation. 
 */
void 
Ut_WallpaperList::init()
{
    bool connectSuccess;

    m_List = new WallpaperList (m_BusinessLogic);
    connectSuccess = connect (
            m_List, SIGNAL(imageActivated(WallpaperDescriptor *)),
            &m_Sink, SLOT(imageActivated(WallpaperDescriptor *)));
    QVERIFY(connectSuccess);

    m_List->setDataSourceType (WallpaperList::DataSourceLocal);
    QVERIFY (m_List->m_DataSourceType == WallpaperList::DataSourceLocal);
    QVERIFY (m_List->m_Model);
}

void 
Ut_WallpaperList::cleanup()
{
    delete m_List;
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WallpaperList";

void 
Ut_WallpaperList::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic; 
}

void 
Ut_WallpaperList::cleanupTestCase()
{
    delete m_BusinessLogic;
    m_App->deleteLater ();
}

void 
Ut_WallpaperList::testItemClicked ()
{
    QModelIndex          index;
    
    index = m_List->m_Model->index (0, 0);
    QVERIFY (index.isValid ());

    m_List->slotItemClicked (index);
    QVERIFY (m_Sink.m_Desc);
}

void 
Ut_WallpaperList::testShowHide ()
{
    m_List->showEvent (0);
    QTest::qWait (300);
    
    // FIXME: We should find a way to test the loading of the thumbnails here.
    // Some code was removed trom the tested unit, so this method became empty.

    m_List->hideEvent (0);
    // Well, maybe the stopLoadingImages should be checked too?
}


QTEST_APPLESS_MAIN(Ut_WallpaperList)



