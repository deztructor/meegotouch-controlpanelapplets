/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpapereditorwidget.h"
#include "wallpaperbusinesslogic.h"
#include "wallpapereditorwidget.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpapergconf.h"

/*
 * We are reading data from this stub, we have some definition there we can use
 * in the test. This way the stub might be changed and these tests will
 * hopefully not fail.
 */
#include "mdesktopentry.h"

#include <MApplication>
#include <QList>
#include <QGraphicsSceneEvent>
#include <MGConfItem>

//#define DEBUG
#include "../../src/debug.h"

#define UNKNOWN_BORDER_X 10
#define UNKNOWN_BORDER_Y 10
#define TITLEBAR_HEIGHT 60

QVariant
MGConfItem::value () const
{
    QString retval;

    SYS_DEBUG ("*** key() = %s", SYS_STR(key()));

    if (key() == WALLPAPER_LANDSCAPE_KEY) {
        retval = WALLPAPER_LANDSCAPE_EDITEDFILE;
        goto return_string;
    }

    if (key() == WALLPAPER_PORTRAIT_KEY) {
        retval = WALLPAPER_PORTRAIT_EDITEDFILE;
        goto return_string;
    }

    return QVariant();

return_string:
    return QVariant (retval);
}

/******************************************************************************
 * Ut_WallpaperEditorWidget implementation. 
 */
void 
Ut_WallpaperEditorWidget::init()
{
}

void 
Ut_WallpaperEditorWidget::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_WallpaperEditorWidget";

void 
Ut_WallpaperEditorWidget::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = 0; 
    m_Widget = 0; 
}

void 
Ut_WallpaperEditorWidget::cleanupTestCase()
{
    dropObjects ();
    m_App->deleteLater ();
}

/*
 * We crate the widget with no wallpaperdescriptor set to be under editing. In
 * this case the widget will not be able to do anything. In normal use this
 * scenario can not happen, but we need to test it anyway. 
 */
void 
Ut_WallpaperEditorWidget::testCreateContentFail ()
{
    createObjects ();
    QVERIFY (!m_Widget->m_InfoHeader);
    QVERIFY (!m_Widget->m_DoneAction);
    QVERIFY (m_Widget->back());
    QVERIFY (!m_Widget->pagePans());
    dropObjects ();
}

void 
Ut_WallpaperEditorWidget::testCreateContentCurrent ()
{
    /*
     * We crate the editor widget with the current wallpaper descriptor. The
     * data is coming from the mdesktopentry stub.
     */
    createObjects (DescriptorCurrent);

    /*
     * Checking the offsets here.
     */
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    // Checking the offset if they are properly calculated. Please note that the
    // -10 is because of the unknown border (should be fixed) and the
    // -70 is because of the window titlebar height (should not be literal).
    QVERIFY (m_Widget->imageDX() == 
            WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() == 
            WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y - 
            TITLEBAR_HEIGHT);

    /*
     * We send the same orientation again. The editor widget should ignore this
     * one, it sets the orientation that is already set.
     */
    m_Widget->orientationChanged (M::Landscape);
    // The offsets sould remain the same.
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    QVERIFY (m_Widget->imageDX() == 
            WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() == 
            WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y 
            - TITLEBAR_HEIGHT);

    /*
     * Changing the orientation, checking if the offset is handled properly.
     */
    m_Widget->orientationChanged (M::Portrait);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    QVERIFY (m_Widget->imageDX() == 
            WALLPAPER_PORTRAIT_HOROFFSET_NUM - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() == 
            WALLPAPER_PORTRAIT_VERTOFFSET_NUM - UNKNOWN_BORDER_Y 
            - TITLEBAR_HEIGHT);
    
    /*
     * Then turning to landscape again to test the change now that we are in
     * portrait mode. 
     */
    m_Widget->orientationChanged (M::Landscape);
    // The offsets sould remain the same.
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    QVERIFY (m_Widget->imageDX() == 
            WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() == 
            WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y 
            - TITLEBAR_HEIGHT);

    /*
     * Destroying the widget.
     */
    dropObjects ();
}

void 
Ut_WallpaperEditorWidget::testCreateContentNonCurrent ()
{
    createObjects (DescriptorNotCurrent);
    /* 
     * Checking the offsets. Please check the comments in 
     * testCreateContentCurrent() for details.
     */
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    QVERIFY (m_Widget->imageDX() ==  - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() ==  -TITLEBAR_HEIGHT - UNKNOWN_BORDER_Y);

    /*
     * Changing the orientation, checking if the offset is handled properly.
     */
    m_Widget->orientationChanged (M::Portrait);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    QVERIFY (m_Widget->imageDX() == - UNKNOWN_BORDER_X);
    QVERIFY (m_Widget->imageDY() == -TITLEBAR_HEIGHT - UNKNOWN_BORDER_Y);

    dropObjects ();
}

void 
Ut_WallpaperEditorWidget::testMouseEvents ()
{
    QGraphicsSceneMouseEvent *pressEvent;
    QGraphicsSceneMouseEvent *moveEvent;
    QGraphicsSceneMouseEvent *releaseEvent;
    QPointF                   pressAt (10, 10);
    QPointF                   moveToRel (12, 13);
    QPointF                   moveTo (
            pressAt.x() + moveToRel.x(), 
            pressAt.y() + moveToRel.y() + TITLEBAR_HEIGHT);

    /*
     * We use the current desciptor to test the mouse events.
     */
    createObjects (DescriptorCurrent);

    /*
     *
     */
    pressEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMousePress);
    pressEvent->setPos (pressAt);
    
    moveEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos (moveTo);

    releaseEvent = new QGraphicsSceneMouseEvent (
            QEvent::GraphicsSceneMouseRelease);

    /*
     * Sending the mouse press event. This should turn off the titlebar of the
     * window and modify the image offsets accordingly. 
     */
    QVERIFY (m_Widget->m_NoTitlebar == false);
    if (m_Widget->m_Orientation == M::Landscape) {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X);
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y 
                - TITLEBAR_HEIGHT);
    } else {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_PORTRAIT_HOROFFSET_NUM - UNKNOWN_BORDER_X);
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_PORTRAIT_VERTOFFSET_NUM - UNKNOWN_BORDER_Y 
                - TITLEBAR_HEIGHT);
    }
    //
    // Sending the mouse press event here.
    //
    m_Widget->mousePressEvent (pressEvent);
    // Checking the changed.
    QVERIFY (m_Widget->m_LastClick == 
            QPointF (pressAt.x(), pressAt.y() + TITLEBAR_HEIGHT));
    QVERIFY (m_Widget->m_NoTitlebar == true);
    if (m_Widget->m_Orientation == M::Landscape) {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X);
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y);
    } else {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_PORTRAIT_HOROFFSET_NUM - UNKNOWN_BORDER_X);
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_PORTRAIT_VERTOFFSET_NUM - UNKNOWN_BORDER_Y);
    }

    /*
     * Sending a mouse move event, dragging the image around the widget.
     */
    m_Widget->mouseMoveEvent (moveEvent);
    
    QVERIFY (m_Widget->m_NoTitlebar == true);
    if (m_Widget->m_Orientation == M::Landscape) {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X +
                moveToRel.x());
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y + 
                moveToRel.y());
    } else {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_PORTRAIT_HOROFFSET_NUM - UNKNOWN_BORDER_X +
                moveToRel.x());
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_PORTRAIT_VERTOFFSET_NUM - UNKNOWN_BORDER_Y + 
                moveToRel.y());
    }

    /*
     * Sending the relase event. This should only change the image coordinates
     * in order to compensate the titlebar change; when the mouse button is
     * released the titlebar is shown again.
     */
    m_Widget->mouseReleaseEvent (releaseEvent);
    QVERIFY (m_Widget->m_NoTitlebar == false);
    if (m_Widget->m_Orientation == M::Landscape) {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_LANDSCAPE_HOROFFSET_NUM - UNKNOWN_BORDER_X +
                moveToRel.x());
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_LANDSCAPE_VERTOFFSET_NUM - UNKNOWN_BORDER_Y -
                TITLEBAR_HEIGHT + moveToRel.y());
    } else {
        QVERIFY (m_Widget->imageDX() == 
                WALLPAPER_PORTRAIT_HOROFFSET_NUM - UNKNOWN_BORDER_X +
                moveToRel.x());
        QVERIFY (m_Widget->imageDY() == 
                WALLPAPER_PORTRAIT_VERTOFFSET_NUM - UNKNOWN_BORDER_Y -
                TITLEBAR_HEIGHT + moveToRel.y());
    }

    /*
     * Cleaning up the resources.
     */
    delete pressEvent;
    delete moveEvent;
    delete releaseEvent;
    dropObjects ();
}

/******************************************************************************
 * Low level helper functions.
 */
void 
Ut_WallpaperEditorWidget::createObjects (
       DescriptorType descType)
{
    QList<WallpaperDescriptor *> descriptors;
    
    m_BusinessLogic = new WallpaperBusinessLogic;
    switch (descType) {
        case DescriptorNone:
            // No image descriptor will be used.
            break;

        case DescriptorCurrent:
            // The current wallpaper will be edited.
            m_BusinessLogic->setEditedImage (
                    WallpaperCurrentDescriptor::instance());
            break;

        case DescriptorNotCurrent:
            // The not-current image will be edited. The index 0 contains the
            // current image descriptor.
            descriptors = m_BusinessLogic->availableWallpapers ();
            m_BusinessLogic->setEditedImage (descriptors[1]);
            break;
    }

    m_Widget = new WallpaperEditorWidget (m_BusinessLogic);
    QTest::qWait (100);
}

void 
Ut_WallpaperEditorWidget::dropObjects ()
{
    if (m_Widget)
        delete m_Widget;
    if (m_BusinessLogic)
        delete m_BusinessLogic;

    delete WallpaperCurrentDescriptor::instance();
    m_Widget = 0;
    m_BusinessLogic = 0;
}

QTEST_APPLESS_MAIN(Ut_WallpaperEditorWidget)



