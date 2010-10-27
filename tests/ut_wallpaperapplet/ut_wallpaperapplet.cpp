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

#include "ut_wallpaperapplet.h"
#include "mdesktopentry.h"
#include "wallpaperapplet.h"
#include "wallpaperwidget.h"
#include "wallpapereditorwidget.h"
#include "wallpaperbrief.h"

#include <dcpwidgettypes.h>

#include <MApplication>
#include <MAction>

#include <QVector>

#define DEBUG
#include "../../src/debug.h"


/******************************************************************************
 * Ut_WallpaperApplet implementation. 
 */
void 
Ut_WallpaperApplet::init()
{
}

void 
Ut_WallpaperApplet::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WallpaperApplet";

void 
Ut_WallpaperApplet::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Applet = new WallpaperApplet;
    
    QVERIFY (!m_Applet->m_MainWidget);
    m_Applet->init ();
    
    /*
     * Testing if the widget is not created yet.
     */
    QVERIFY (!m_Applet->m_MainWidget);
}

void 
Ut_WallpaperApplet::cleanupTestCase()
{
    delete m_Applet;
    m_App->deleteLater ();
}

void 
Ut_WallpaperApplet::testTitle ()
{
    QString title = m_Applet->title();
    QVERIFY (!title.isEmpty());
}

void
Ut_WallpaperApplet::testConstructWidget ()
{
    DcpWidget *widget, *editorWidget;
    bool       backAccepted;

    /*
     * Testing if the applet creates a widget the first time.
     */
    widget = m_Applet->constructWidget (WallpaperApplet::MainWidget);
    QVERIFY (widget);
    QVERIFY (m_Applet->m_MainWidget == widget);
    
    editorWidget = m_Applet->constructWidget (WallpaperApplet::EditorWidget);
    QVERIFY (editorWidget);
    QVERIFY (m_Applet->m_EditorWidget == editorWidget);
    
    /*
     * Testing if the widget accepts the back. Our applets always accept back.
     */
    backAccepted = widget->back();
    QVERIFY (backAccepted);

    backAccepted = editorWidget->back();
    QVERIFY (backAccepted);

    /*
     * Testing if the applet knows about the destruction of the widget.
     */
    delete editorWidget;
    QVERIFY (!m_Applet->m_EditorWidget);

    delete widget;
    QVERIFY (!m_Applet->m_MainWidget);

    /*
     * For an invalid window ID we expect the null pointer. ;)
     */
    QVERIFY (!m_Applet->constructWidget(WallpaperApplet::EditorWidget + 1));
}

void 
Ut_WallpaperApplet::testMenuItems ()
{
    QVector<MAction*> items = m_Applet->viewMenuItems ();
    
    QVERIFY (items.size() == 0);
}

void 
Ut_WallpaperApplet::testConstructbrief ()
{
    DcpBrief *brief1 = m_Applet->constructBrief(0);
    QString   valueText;
    QString   imageFilename;
    QString   iconName;

    QVERIFY (brief1);
    QVERIFY (brief1->widgetTypeID() == DcpWidgetType::Image);
    
    valueText = brief1->valueText();
    /*
     * FIXME: Actually we now check the GConf database, so we need to enhance
     * this test with a GConf stub.
     */
    //QVERIFY (valueText == WALLPAPER_NAME);
    imageFilename = brief1->image ();
    iconName = brief1->icon ();
    SYS_DEBUG ("*** valueText     = %s", SYS_STR(valueText));
    SYS_DEBUG ("*** imageFilename = %s", SYS_STR(imageFilename));
    SYS_DEBUG ("*** iconName      = %s", SYS_STR(iconName));
    QVERIFY (!imageFilename.isEmpty() || !iconName.isEmpty());

    delete brief1;
}


QTEST_APPLESS_MAIN(Ut_WallpaperApplet)


