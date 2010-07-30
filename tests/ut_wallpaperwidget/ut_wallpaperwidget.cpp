/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpaperwidget.h"
#include "wallpaperwidget.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpapermodel.h"

#include <MApplication>

//#define DEBUG
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
    QTest::qWait (500);

    QVERIFY (m_Widget->m_WallpaperBusinessLogic->editedImage() == curr);
    QVERIFY (m_Sink.m_ChangeWidgetCame);
    QVERIFY (m_Sink.m_WidgetID == 1);
}

void
Ut_WallpaperWidget::testGalleryImageSelected()
{
    m_Widget->galleryImageSelected("file:///nodir/NoSuchFile.png");
    // The loading of the image is happening in a separate thread, we need to
    // give a chance to be executed.
    QTest::qWait (500);
    QVERIFY(m_Sink.m_WallpaperImageEditRequestedCame);
}

void
Ut_WallpaperWidget::testImageBrowserDismissed()
{
    m_Widget->imageBrowserDismissed();
    QVERIFY (m_Widget->m_noImageBrowser);
}

QTEST_APPLESS_MAIN(Ut_WallpaperWidget)

