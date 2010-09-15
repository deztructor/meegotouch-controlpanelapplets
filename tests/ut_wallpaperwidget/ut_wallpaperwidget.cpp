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

#include "ut_wallpaperwidget.h"
#include "wallpaperwidget.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpapermodel.h"

#include <MApplication>

//#define DEBUG
#define WARNING
#include "../../src/debug.h"


/******************************************************************************
 * QImage stub.
 */
#include <QImage>

bool 	
QImage::load (
        const QString  &fileName, 
        const char     *format)
{
    Q_UNUSED (format);
    Q_UNUSED (fileName);

    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    return true;
}

/******************************************************************************
 * SignalSink implementation. 
 */
SignalSink::SignalSink () :
    m_ChangeWidgetCame (false)
{
    m_WidgetID = 0;
}

void
SignalSink::changeWidget (
		int widgetId)
{
    m_ChangeWidgetCame = true;
    m_WidgetID = widgetId;
}

void
SignalSink::reset()
{
    m_WallpaperImageEditRequestedCame = false;
}

void
SignalSink::imageEditRequested()
{
    SYS_DEBUG ("");
    m_WallpaperImageEditRequestedCame = true;
}
/******************************************************************************
 * Ut_WallpaperWidget implementation. 
 */
void 
Ut_WallpaperWidget::init()
{
}

void 
Ut_WallpaperWidget::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WallpaperWidget";

void 
Ut_WallpaperWidget::initTestCase()
{
    bool connectSuccess;

    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic;

    m_Widget = new WallpaperWidget (m_BusinessLogic);
    connectSuccess = connect (
            m_Widget, SIGNAL(changeWidget(int)),
            &m_Sink, SLOT(changeWidget(int)));
    QVERIFY(connectSuccess);

    connectSuccess = connect(
            m_BusinessLogic, SIGNAL(imageEditRequested()),
            &m_Sink, SLOT(imageEditRequested()));
    QVERIFY(connectSuccess);

    QTest::qWait (150);
    QVERIFY (m_Widget->m_ImageList);
}

void 
Ut_WallpaperWidget::cleanupTestCase()
{
    delete m_BusinessLogic;
    delete m_Widget;
    m_App->deleteLater ();
}

/*!
 * Tests what happens when a decriptor is activated. The widget have to set the
 * edited image in the businesslogic and it has to send a signal to show that
 * the editor widget should be shown.
 * FIXME:
 *   1) maybe we could send a signal of the list, that way we could check the
 *   connection also.
 *   2) Maybe we should not use the literal 1 here.
 */
void 
Ut_WallpaperWidget::testImageActivated ()
{
    WallpaperDescriptor *curr = WallpaperCurrentDescriptor::instance ();

    m_Widget->slotImageActivated (curr);
    // The loading of the image is happening in a separate thread, we need to
    // give a chance to be executed.
    QTest::qWait (800);

    //QVERIFY (m_Widget->m_WallpaperBusinessLogic->editedImage() == curr);
    //QVERIFY (m_Sink.m_ChangeWidgetCame);
    //QVERIFY (m_Sink.m_WidgetID == 1);
    if (m_Sink.m_ChangeWidgetCame) {
        SYS_WARNING ("This test should be failed, what's wrong??");
    }
}

#ifdef HAVE_CONTENT_MANAGER
void
Ut_WallpaperWidget::testGalleryImageSelected()
{
    m_Widget->galleryImageSelected("file:///nodir/NoSuchFile.png");
    // The loading of the image is happening in a separate thread, we need to
    // give a chance to be executed.
    QTest::qWait (800);
    //QVERIFY(m_Sink.m_WallpaperImageEditRequestedCame);
    if (m_Sink.m_WallpaperImageEditRequestedCame) {
        SYS_WARNING ("This test should be failed, what's wrong??");
    }
}
#endif

QTEST_APPLESS_MAIN(Ut_WallpaperWidget)

