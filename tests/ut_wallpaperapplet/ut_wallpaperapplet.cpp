/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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
    
    QVERIFY (items.size() == 1);
}

void 
Ut_WallpaperApplet::testConstructbrief ()
{
    DcpBrief *brief1 = m_Applet->constructBrief(0);
    QString   valueText;
    QString   imageFilename;

    QVERIFY (brief1);
    QVERIFY (brief1->widgetTypeID() == DcpWidgetType::Image);
    
    valueText = brief1->valueText();
    QVERIFY (valueText == WALLPAPER_NAME);
    
    imageFilename = brief1->image();
    QVERIFY (!imageFilename.isEmpty());
    SYS_DEBUG ("*** imageFilename = %s", SYS_STR(imageFilename));

    delete brief1;
}


QTEST_APPLESS_MAIN(Ut_WallpaperApplet)


