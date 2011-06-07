/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "wallpaperviewwidget.h"
#include "wallpaperconfiguration.h"
#include "wallpaperutils.h"

#include <QTimer>
#include <MWindow>
#include <MApplication>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MPositionIndicator>

#define DEBUG
#define WARNING
#include "../debug.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(WallpaperViewWidget)

/******************************************************************************
 *
 */
WallpaperViewWidget::WallpaperViewWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpStylableWidget (parent),
    m_BusinessLogic (wallpaperBusinessLogic),
    m_BgColor ("black"),
    m_Saving (false),
    m_Initialized (false),
    m_PageRealized (false),
    m_ImageWidget (0)
{
    connect (m_BusinessLogic, SIGNAL(wallpaperSaved()),
            this, SLOT(wallpaperSaved()));
    connect (m_BusinessLogic, SIGNAL(wallpaperLoaded (QuillImage, QSize)),
            this, SLOT(wallpaperLoaded (QuillImage, QSize)));
}

WallpaperViewWidget::~WallpaperViewWidget ()
{
    if (Wallpaper::useFullScreen) {
        MWindow *win = MApplication::activeWindow ();

        if (win)
            win->showNormal ();
    }
}

void
WallpaperViewWidget::saveImage ()
{
    if (Wallpaper::supportEdit || m_Trans.expectedSize() != m_OriginalSize) {
        SYS_DEBUG ("*** expectedSize = %s", SYS_SIZE(m_Trans.expectedSize()));
        SYS_DEBUG ("*** offset       = %s", SYS_POINTF(m_Trans.offset()));
        SYS_DEBUG ("*** scale        = %g", m_Trans.scale());
        SYS_DEBUG ("*** rotation     = %g", m_Trans.rotation());
        QPixmap pixmap = generatePixmap (m_Trans);

        m_Saving = true;
        m_BusinessLogic->setWallpaper (pixmap);
    } else {
        m_Saving = true;
        m_BusinessLogic->setWallpaper ();
    }

    /*
     * Notifying the business logic about the editing ended. It is important,
     * otherwise the businesslogic will reject the next edit start requests.
     */
    m_BusinessLogic->endEdit ();
}

void
WallpaperViewWidget::dropImage ()
{
    /*
     * Notifying the business logic about the editing ended. It is important,
     * otherwise the businesslogic will reject the next edit start requests.
     */
    m_BusinessLogic->endEdit ();    
}

void
WallpaperViewWidget::initialize (
        QuillImage   &image,
        QSize         size)
{
    WallpaperDescriptor  desc = m_BusinessLogic->editedImage ();
    QSize                sceneSize;
    int                  xMarg = 0;
    int                  yMarg = 0;

    SYS_DEBUG ("");
    if (m_Initialized) {
        SYS_WARNING ("Already initialized.");
        return;
    }

    if (desc.isNull()) {
        SYS_WARNING ("No editted image, giving up.");
        return;
    }
    
    m_Image        = image;
    m_OriginalSize = size;
    sceneSize = m_BusinessLogic->sceneSize ();
    m_Trans.setExpectedSize (sceneSize);
    m_Trans.setOrientation (M::Portrait);
    
    /*
     * The margins can be positive or negative, so that the image is always
     * centered at first.
     */
    xMarg = (sceneSize.width() - m_Image.width()) / 2;
    yMarg = (sceneSize.height() - m_Image.height()) / 2;

    m_Trans.setOffset (QPointF(xMarg, yMarg));
    this->setMinimumSize (m_Trans.expectedSize());
    
    SYS_WARNING ("sceneSize = %dx%d", sceneSize.width(), sceneSize.height());
    SYS_WARNING ("imageSize = %dx%d", m_Image.width(), m_Image.height());
    SYS_WARNING ("offset    = %d, %d", xMarg, yMarg);

    m_ImageWidget = new MImageWidget(&m_Image, this);
    //m_ImageWidget->setPos (m_Trans.offset());
    //redrawImage ();

    m_Initialized = true;
}

bool 
WallpaperViewWidget::pagePans () const
{
    return false;
}

#if 0
void
WallpaperViewWidget::paint (
        QPainter                          *painter,
        const QStyleOptionGraphicsItem    *option,
        QWidget                           *widget)
{
    MWidget::paint (painter, option, widget);
    
    painter->fillRect (
            0, 0, 
            m_Trans.expectedWidth (),
            m_Trans.expectedHeight (),
            m_BgColor);

    painter->drawImage (
            QRect (imageX(), imageY(), imageDX(), imageDY()),
            m_Image);
}
#endif

void
WallpaperViewWidget::polishEvent ()
{
    SYS_DEBUG ("");
    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;

    /*
     * Just a protection about double adding the actions.
     */
    if (m_PageRealized)
        return;
    
    /*
     * We need to find the MApplicationPage among our parents.
     */
    parent = parentWidget();
    while (parent) {
        page = qobject_cast <MApplicationPage *>(parent);
        if (page)
            break;
        parent = parent->parentWidget();
    }
    m_PageRealized = true;

    /*
     * With or without the page we can turn the window fullscreen.
     */
    if (Wallpaper::useFullScreen) {
        MWindow *win = MApplication::activeWindow ();

        if (win)
            win->showFullScreen();
    }

    if (!page)
        return;

    page->setPannable (false);
    page->pannableViewport()->positionIndicator()->hide();
    /**************************************************************************
     * Hiding the home button and the escape button from the page. 
     */
    page->setComponentsDisplayMode (
            MApplicationPage::EscapeButton,
            MApplicationPageModel::Hide);
    page->setComponentsDisplayMode (
            MApplicationPage::HomeButton,
            MApplicationPageModel::Hide);

    //% "Save"
    m_DoneAction = new MAction(qtTrId("qtn_comm_save"), this);
    m_DoneAction->setLocation(MAction::ToolBarLocation);
    page->addAction(m_DoneAction);
    connect(m_DoneAction, SIGNAL(triggered()), this, SLOT(slotDoneActivated()));

    //% "Cancel"
    m_CancelAction = new MAction(qtTrId("qtn_comm_cancel"), this);
    m_CancelAction->setLocation(MAction::ToolBarLocation);
    page->addAction(m_CancelAction);
    connect(m_CancelAction, SIGNAL(triggered()), this, SLOT(slotCancelActivated()));
}

int
WallpaperViewWidget::imageX () const
{
    int retval = 0;

    retval += m_Trans.x();
    return retval;
}

int
WallpaperViewWidget::imageY () const
{
    int retval = 0;

    retval += m_Trans.y();

    // FIXME: testing
    //retval -= 110;
    return retval;
}

int
WallpaperViewWidget::imageDX () const
{
    return m_Trans.scale() * m_Image.width();
}

int
WallpaperViewWidget::imageDY () const
{
    return m_Trans.scale() * m_Image.height();
}

QPixmap
WallpaperViewWidget::generatePixmap (
        const WallpaperITrans  &transformations)
{
    QImage    transformed = transformedImage();
    QPixmap   retval (transformations.expectedSize());
    QPainter  painter (&retval);
    QPointF   offset (
            imageX() - transformed.width() / 2, 
            imageY() - transformed.height() / 2);

    retval.fill (m_BgColor);
    painter.drawImage (offset, transformed);

    return retval;
}

    
void 
WallpaperViewWidget::slotDoneActivated ()
{
    SYS_DEBUG ("");

    if (m_Saving) {
        SYS_DEBUG ("Saving is being performed.");
        return;
    }
        
    saveImage ();
}
 
void 
WallpaperViewWidget::slotCancelActivated ()
{
    SYS_DEBUG ("");

    if (m_Saving) {
        SYS_DEBUG ("Saving is being performed.");
        return;
    }

    /*
     * Amitting a signal to show the cancel is clicked. Then emitting a signal
     * to close the application page (this slot is only called when the sheet is
     * not used). Then we notify the business logic about the edit operation is
     * finished.
     */
    emit cancelClicked ();
    emit closePage ();
    m_BusinessLogic->endEdit ();
}

void 
WallpaperViewWidget::wallpaperSaved ()
{
    SYS_DEBUG ("");

    if (!m_Saving) {
        SYS_WARNING ("We didn't request any saving.");
        return;
    }

    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    MWindow  *win;
    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    if (!Wallpaper::useSheets) {
        /*
         * We are done with the editing, let's page back to the view where we
         * have the list.
         */
        //changeWidget (0);
        emit closePage ();
    }
    
    m_BusinessLogic->endEdit ();
    m_Saving = false;
}


void 
WallpaperViewWidget::wallpaperLoaded (
        QuillImage    image, 
        QSize         originalSize)
{
    SYS_DEBUG ("");

    if (!m_Initialized) {
        initialize (image, originalSize);
    } else {
        SYS_WARNING ("Progressive loading not implemented.");
    }
}

void 
WallpaperViewWidget::redrawImage ()
{
    QSize   imageSize = imageVisualSize ();
#if 1
    QPointF offset (
            imageX() - m_ImageWidget->size().width() / 2.0, 
            imageY() - m_ImageWidget->size().height() / 2.0);
#else
    QPointF offset (0, 0);
#endif

    m_ImageWidget->setTransformOriginPoint(
            m_ImageWidget->size().width() / 2.0, 
            m_ImageWidget->size().height() / 2.0);

    m_ImageWidget->setScale (m_Trans.scale());
    
    //SYS_DEBUG ("*** myxy   (%d, %d)", imageX(), imageY()); 
    //SYS_DEBUG ("*** setPos (%s)", SYS_POINTF(offset)); 
    m_ImageWidget->setPos (offset);
    m_ImageWidget->setRotation (m_Trans.rotation());
}

QSize
WallpaperViewWidget::imageVisualSize (qreal scale)
{
    qreal realdx, realdy;
    QSize  retval;

    #if 1
    SYS_DEBUG ("*** rotation    = %g", m_Trans.rotation());
    SYS_DEBUG ("*** scale       = %g", m_Trans.scale());
    SYS_DEBUG ("*** initialized = %s", SYS_BOOL(m_Initialized));
    #endif
    if (m_Trans.rotation() == 90.0 || m_Trans.rotation() == -90.0) {
        realdx = m_Image.height(); 
        realdy = m_Image.width(); 
    } else {
        realdy = m_Image.height();
        realdx = m_Image.width();
    }

    retval = QSize (realdx * scale, realdy * scale);
    return retval;
}

QImage
WallpaperViewWidget::transformedImage ()
{
    QSize  visualSize (imageVisualSize(m_Trans.scale()));
    QImage retval (visualSize, QImage::Format_RGB16);
    QPainter  painter (&retval);
    QRectF rect;

    retval.fill (QColor("#ffffff").rgb());
    painter.translate(
            retval.width() / 2, 
            retval.height() / 2); 
    painter.rotate(m_Trans.rotation());
   
    if (m_Trans.rotation() == 90.0 || m_Trans.rotation() == -90.0)
        rect = QRectF (
                -retval.height() / 2, 
                -retval.width() / 2, 
                visualSize.height(),
                visualSize.width());
    else
        rect = QRectF (
                -retval.width() / 2, 
                -retval.height() / 2, 
                visualSize.width(),
                visualSize.height());


    painter.drawImage (rect, m_Image);
    return retval;
}
