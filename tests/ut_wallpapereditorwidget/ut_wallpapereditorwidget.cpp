/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpapereditorwidget.h"
#include "wallpaperbusinesslogic.h"
#include "wallpapereditorwidget.h"
#include "wallpapercurrentdescriptor.h"

#include <MApplication>
#include <QList>

#define DEBUG
#include "../../src/debug.h"


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

void 
Ut_WallpaperEditorWidget::testCreateContent ()
{
    /*
     * FIXME: I'm working here, this test is not finalized.
     */
    createObjects ();
    QVERIFY (!m_Widget->m_InfoHeader);
    QVERIFY (!m_Widget->m_DoneAction);
    QVERIFY (m_Widget->back());
    QVERIFY (!m_Widget->pagePans());
    dropObjects ();

    createObjects (DescriptorCurrent);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    m_Widget->orientationChanged (M::Landscape);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    m_Widget->orientationChanged (M::Portrait);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
    dropObjects ();

    createObjects (DescriptorNotCurrent);
    SYS_DEBUG ("imageDX() = %d", m_Widget->imageDX());
    SYS_DEBUG ("imageDY() = %d", m_Widget->imageDY());
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

    m_Widget = 0;
    m_BusinessLogic = 0;
}

QTEST_APPLESS_MAIN(Ut_WallpaperEditorWidget)



