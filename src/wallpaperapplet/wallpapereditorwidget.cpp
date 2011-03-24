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
#include "wallpapereditorwidget.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperinfoheader.h"

#include <QStyle>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MAction>
#include <MApplicationWindow>
#include <MApplication>
#include <MApplicationPage>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(WallpaperEditorWidget)

static const qreal ScaleLowerLimit = 0.15;

//#define DEBUG
#define WARNING
#include "../debug.h"

static const int ExtraMargin = 10;
static const int TitleBarHeight = 60;

/*!
 * WallpaperEditorWidget constructor
 *
 * \param wallpaperBusinessLogic An WallpaperBusinessLogic instance
 * \param parent The parent widget (if any)
 */
WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    DcpWidget (parent),
    m_WallpaperBusinessLogic (wallpaperBusinessLogic),
    m_InfoHeader (0),
    m_DoneAction (0),
    m_CancelAction (0),
    m_NoTitlebar (false),
    m_OrientationLocked (false),
    m_PinchOngoing (false),
    m_MotionOngoing (false),
    m_HasPendingRedraw (false),
    m_Physics (0)
{
    MWindow *win = MApplication::activeWindow ();
    
#ifndef LIBMEEGOCONTROL
    /*
     * FIXME: With the new demo program this will not work. This must be a bug
     * in the libMeegoTouch!
     * NOTE: It also freezes the controlpanel sometimes, so I had to completely
     * remove.
     */
    //if (win)
    //    win->showFullScreen();
#endif

#if 0
    /*
     * This one unfortunately will not turn to full-screen at all.
     */
    if (win) {
        Qt::WindowStates state = win->windowState();
        win->setWindowState(state | Qt::WindowFullScreen);
    }
#endif

    setObjectName ("WallpaperEditorWidget");
    /*
     * Creating the MPhysics2DPanning object.
     */
    m_Physics = new MPhysics2DPanning (0);
    m_Physics->setPanDirection (Qt::Vertical | Qt::Horizontal);
    //m_Physics->setPanDirection (Qt::Horizontal);
    m_Physics->setPosition (QPointF(100.0, 100.0));
    //m_Physics->setRange (QRectF(-000.0, -000.0, +100.0, +100.0));
    m_Physics->setEnabled(true);
    
    m_Physics->setPointerSpringK(0.25);
    m_Physics->setFriction(0.6);
    m_Physics->setSlidingFriction(0.10);
    m_Physics->setBorderSpringK(0.02);
    m_Physics->setBorderFriction(0.15);
    m_Physics->setMaximumVelocity(70);

    connect (m_Physics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(panningPhysicsPositionChanged(const QPointF &)));
    connect (m_Physics, SIGNAL(panningStopped()),
            this, SLOT(panningPhysicsPanningStopped()));

    /*
     *
     */
    QTimer::singleShot (0, this, SLOT(createContent()));

    if (win) {
        m_Orientation = win->orientation();
        m_OrientationLocked = win->isOrientationLocked ();
        connect (win, SIGNAL(orientationChanged(M::Orientation)),
                this, SLOT(orientationChanged(M::Orientation)));
    } else {
        SYS_WARNING ("We have a problem: We don't know the orientation! "
                    "Only in tests though...");
        m_Orientation = M::Landscape;
    }
}

/*!
 * Destructor for WallpaperEditorWidget
 */
WallpaperEditorWidget::~WallpaperEditorWidget ()
{
}

/*!
 * Paint method for WallpaperEditorWidget, used for 
 * drawing the edited image actual state
 */
void
WallpaperEditorWidget::paint (
        QPainter                          *painter,
        const QStyleOptionGraphicsItem    *option,
        QWidget                           *widget)
{
#if 0
    bool portrait = (geometry().height() > geometry().width());
#endif

    painter->fillRect (
            -ExtraMargin, -ExtraMargin, 
            m_Trans.expectedWidth (),
            m_Trans.expectedHeight (),
            QColor ("black"));
#if 0
    if (portrait) {
        painter->drawImage (
                QRect (imageX(), imageY(), imageDX(), imageDY()),
                m_bgPortrait);
    } else if (!portrait) {
        painter->drawImage (
                QRect (imageX(), imageY(), imageDX(), imageDY()),
                m_bgLandscape);
    }
#else
        painter->drawImage (
                QRect (imageX(), imageY(), imageDX(), imageDY()),
                m_bgPortrait);
#endif
    MWidget::paint (painter, option, widget);
}

/*!
 * Method for creating the editor-widget contents
 */
void
WallpaperEditorWidget::createContent ()
{
    MWindow             *win = MApplication::activeWindow ();
    WallpaperDescriptor *desc;

    SYS_DEBUG ("");

    desc = m_WallpaperBusinessLogic->editedImage();
    if (!desc) {
        SYS_WARNING ("There is no editedimage.");
        return;
    }

    if (desc->isCurrent()) {
        WallpaperCurrentDescriptor *cdesc;
       
        cdesc = qobject_cast<WallpaperCurrentDescriptor*> (desc);
        if (!desc) {
            SYS_WARNING ("This is the current wallpaper, but its type is"
                    "not <WallpaperCurrentDescriptor>");
            goto not_current_wallpaper;
        }

        SYS_DEBUG ("This is the current image.");
        SYS_DEBUG ("*** orig landscape       = %s", 
                SYS_STR(cdesc->originalImageFile (M::Landscape)));
        SYS_DEBUG ("*** orig portrait        = %s", 
                SYS_STR(cdesc->originalImageFile (M::Portrait)));


        m_LandscapeTrans = cdesc->iTrans (M::Landscape);
        m_PortraitTrans = cdesc->iTrans (M::Portrait);

        /*
         * Initialization of the image transformations for the current
         * wallpaper. 
         */
        m_PortraitTrans.setExpectedSize (win->visibleSceneSize (M::Portrait));
        m_PortraitTrans.setOrientation (M::Portrait);
        m_LandscapeTrans.setExpectedSize (win->visibleSceneSize (M::Landscape));
        m_LandscapeTrans.setOrientation (M::Landscape);

        /*
         * FIXME: We are using the original pixmap here, but we could make this
         * be more generic, and use the original only when it differs from the
         * edited, then we could move it to the WallpaperDescriptor class...
         */
        if (supportsLandscape())
            m_bgLandscape = cdesc->image (
                    WallpaperDescriptor::OriginalLandscape);

        if (supportsPortrait())
            m_bgPortrait = cdesc->image (
                    WallpaperDescriptor::OriginalPortrait);
        /*
         * Need to copy back...
         */
        m_Trans = m_Orientation == M::Portrait ? 
            m_PortraitTrans : m_LandscapeTrans;

        goto finalize;
    }

not_current_wallpaper:
    /*
     * Initializing image transformations when editing an image that is not the
     * current image.
     */
    m_PortraitTrans.setExpectedSize (win->visibleSceneSize (M::Portrait));
    m_PortraitTrans.setOrientation (M::Portrait);
    m_LandscapeTrans.setExpectedSize (win->visibleSceneSize (M::Landscape));
    m_LandscapeTrans.setOrientation (M::Landscape);

    /*
     * Need to copy back.
     */
    m_Trans = m_Orientation == M::Portrait ? 
        m_PortraitTrans : m_LandscapeTrans;

    /*
     * Here is what we do when this is not the current image.  If the image is
     * very big the handling might be slow, so we scale it down.
     */
    if (supportsLandscape())
        m_bgLandscape = desc->image (
                WallpaperDescriptor::Landscape);

    if (supportsPortrait())
        m_bgPortrait = desc->image (
               WallpaperDescriptor::Portrait);


finalize:
    SYS_DEBUG ("*** m_bgLandscape is %dx%d", 
            m_bgLandscape.width(),
            m_bgLandscape.height());
    SYS_DEBUG ("*** landscape offset = %d, %d", 
            m_LandscapeTrans.x(),
            m_LandscapeTrans.y());
    SYS_DEBUG ("*** m_bgPortrait  is %dx%d", 
            m_bgPortrait.width(),
            m_bgPortrait.height());
    SYS_DEBUG ("*** portrait offset = %d, %d", 
            m_PortraitTrans.x(),
            m_PortraitTrans.y());

    this->setMinimumSize (m_Trans.expectedSize());

    /*
     *
     */
    setupPanningPhysics ();

    /*
     * Enabling two finger gestures.
     */
    //setAcceptTouchEvents(true);
    grabGesture (Qt::PinchGesture, Qt::GestureFlags());
    grabGesture (Qt::PanGesture, Qt::GestureFlags());

    createWidgets ();
    redrawImage ();
}

/*!
 * Method for create wallpaper-editor internal widgets
 */
void
WallpaperEditorWidget::createWidgets ()
{
    SYS_DEBUG ("");
    setContentsMargins (0, 0, 0, 0);
    setWindowFrameMargins (0.0, 0.0, 0.0, 0.0);
    
    #ifdef USE_INFOHEADER
    MLayout               *layout;
    MLinearLayoutPolicy   *policy;
    /*
     *
     */
    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    layout->setContentsMargins (0, 0, 0, 0);

    /*
     * The information header.
     */
    SYS_DEBUG ("Creating WallpaperInfoHeader");
    m_InfoHeader = new WallpaperInfoHeader;
    m_InfoHeader->setObjectName ("WallpaperInfoHeader");
    policy->addItem (m_InfoHeader);

    policy->addStretch ();

    setLayout (layout);
    #endif

}

void 
WallpaperEditorWidget::panningPhysicsPositionChanged(
        const QPointF    &position)
{
#if 0
    SYS_WARNING ("position at %g, %g", position.x(), position.y());
    SYS_WARNING ("inMotion = %s", SYS_BOOL(m_Physics->inMotion()));
    SYS_WARNING ("enabled  = %s", SYS_BOOL(m_Physics->enabled()));
#endif

    //gestureWorkaround (&position);
    m_UserOffset = position;
    //queueRedrawImage ();
    redrawImage ();
    m_Physics->setPosition (position);
}

void 
WallpaperEditorWidget::panningPhysicsPanningStopped ()
{
    //SYS_WARNING ("");
    //queueRedrawImage ();
    redrawImage ();
}

/*!
 * This virtual method is executed when we already have an MApplicationPage as
 * parent. We initialize the page here.
 */
void
WallpaperEditorWidget::polishEvent ()
{
    QGraphicsWidget  *parent;
    MApplicationPage *page = 0;

    /*
     * Just a protection about double adding the actions.
     */
    if (m_DoneAction)
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

    if (!page)
        return;

    page->setPannable (false);
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

/*!
 * This slot is called when the user activates the 'done' action and so we have
 * to store the settings.
 */
void
WallpaperEditorWidget::slotDoneActivated ()
{
    WallpaperITrans   *ltrans, *ptrans;
    MWindow           *win;

    SYS_DEBUG ("");
    ltrans = m_Trans.orientation() == M::Landscape ?
        &m_Trans : &m_LandscapeTrans;
    ptrans = m_Trans.orientation() == M::Portrait ?
        &m_Trans : &m_PortraitTrans;

    /*
     * Here we save the settings.
     */
    m_WallpaperBusinessLogic->setBackground (ltrans, ptrans);

    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    /*
     * We are done with the editing, let's page back to the view where we have
     * the list.
     */
    SYS_DEBUG ("Calling changeWidget()");
    emit doneClicked ();
    changeWidget (0);

    /*
     *
     */
    m_WallpaperBusinessLogic->setEditedImage (0);
}

/*!
 * A slot for handling the 'Cancel' button presses
 */
void
WallpaperEditorWidget::slotCancelActivated ()
{
    emit cancelClicked ();
    emit closePage ();
    /*
     *
     */
    m_WallpaperBusinessLogic->setEditedImage (0);
}

/*!
 * Re-implemented method for setting back the non-fullscreen window-state
 */
bool 
WallpaperEditorWidget::back ()
{
    MWindow *win;

    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    SYS_DEBUG ("");

    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    return DcpWidget::back();
}

bool 
WallpaperEditorWidget::pagePans () const
{
    return false;
}

/******************************************************************************
 * Private low level functions.
 */

/*!
 * \returns The offset correction value for the new state.
 * \param show If the titlebar should be shown or not.
 *
 */
QPointF
WallpaperEditorWidget::toggleTitlebars (
        bool       show)
{
    MApplicationPage  *currentPage;
    #if 0
    /*
     * This mapToScene() is sensitive to the screen rotation.
     */
    QPointF r;
    r = mapToScene (0, 0);
    SYS_WARNING ("r = %g, %g", r.x(), r.y());
    #endif
    /*
     * If the requested state is the current state we don't need to hide/show
     * anything, we just return the correction value.
     */
    if (show == !m_NoTitlebar)
        goto finalize;

    /*
     * We do the showing/hiding here.
     */
    if (MApplication::activeApplicationWindow())
        currentPage = MApplication::activeApplicationWindow()->currentPage();
    else
        currentPage = 0;

    if (currentPage) {
        if (show) {
            SYS_DEBUG ("Showing titlebar");
            currentPage->setComponentsDisplayMode (
                    MApplicationPage::NavigationBar,
                    MApplicationPageModel::Show); 
            #ifdef USE_INFOHEADER
            m_InfoHeader->show ();
            #endif
        } else {
            SYS_DEBUG ("Hiding titlebar");
            currentPage->setComponentsDisplayMode (
                    MApplicationPage::NavigationBar, 
                    MApplicationPageModel::Hide);
            #ifdef USE_INFOHEADER
            m_InfoHeader->hide ();
            #endif
        }
    }

    m_NoTitlebar = !show;

finalize:
    /*
     * To return the correction value.
     */
    if (!show)
        return QPointF (0, TitleBarHeight);

    return QPointF (0, 0);
}

/*!
 * Returns the X offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageX () const
{
    int retval = 0;

    retval -= ExtraMargin;
    retval += m_UserOffset.x();
    retval += m_Trans.x();

    //SYS_DEBUG ("returning %d", retval);
    return retval;
}

/*!
 * Returns the Y offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageY () const
{
    int            retval = 0;
#if 0
    bool           portrait = (geometry().height() > geometry().width());
    QPointF        r;
    r = mapToScene (0.0, 0.0);

    SYS_WARNING ("*** portrait = %s", SYS_BOOL(portrait));
    SYS_WARNING ("*** origo    = %g, %g", r.x(), r.y());
    retval -= r.y(); 
#endif
    retval += m_UserOffset.y();
    retval += m_Trans.y();

    //SYS_DEBUG ("returning %d", retval);
    return retval;
}


int
WallpaperEditorWidget::imageDX () const
{
    bool portrait = (geometry().height() > geometry().width());

    return portrait ? 
        m_Trans * m_bgPortrait.width() : m_Trans * m_bgLandscape.width();
}

int
WallpaperEditorWidget::imageDY () const
{
    bool portrait = (geometry().height() > geometry().width());

    return portrait ? 
        m_Trans * m_bgPortrait.height() : m_Trans * m_bgLandscape.height();
}

void 
WallpaperEditorWidget::queueRedrawImage ()
{
    if (m_HasPendingRedraw) {
        //SYS_DEBUG ("Dropping...");
        return;
    }

    m_HasPendingRedraw = true;
    QTimer::singleShot(50, this, SLOT(redrawImage ()));
}

void 
WallpaperEditorWidget::redrawImage ()
{
    /*
     * We need to update the current page and not just this widget because of
     * those damn extra margins coming from somewhere.
     */
    //SYS_DEBUG ("Drawing...");
    m_HasPendingRedraw = false;
    if (MApplication::activeApplicationWindow())
        MApplication::activeApplicationWindow()->currentPage()->update();
}

void 
WallpaperEditorWidget::orientationChanged (
        M::Orientation orientation)
{
    if (m_Orientation == orientation) {
        SYS_WARNING ("This is the old orientation?!");
        return;
    }

    switch (orientation) {
        case M::Portrait:
            SYS_DEBUG ("Turned to portrait");
            m_LandscapeTrans = m_Trans;
            m_Trans = m_PortraitTrans;
            this->setMinimumSize (m_Trans.expectedSize());
            break;

        case M::Landscape:
            SYS_DEBUG ("Turned to landscape");
            m_PortraitTrans = m_Trans;
            m_Trans = m_LandscapeTrans;
            this->setMinimumSize (m_Trans.expectedSize());
            break;
    }
    
    m_Orientation = orientation;
    redrawImage ();
}

/******************************************************************************
 * Stuff for the normal mouse events.
 */
void 
WallpaperEditorWidget::wheelEvent (
        QGraphicsSceneWheelEvent *event)
{
    m_Trans.modScale (event->delta());
    setupPanningPhysics ();
    queueRedrawImage ();
}

#if 0
void 
WallpaperEditorWidget::mouseMoveEvent (
        QGraphicsSceneMouseEvent *event)
{
    if (m_PinchOngoing)
        return;

    #if 0 
    QPointF testPos = event->pos();
    QPointF mapp = mapToScene (testPos);
    SYS_DEBUG ("*** pos    = %g, %g", testPos.x(), testPos.y());
    SYS_DEBUG ("*** mapped = %g, %g", mapp.x(), mapp.y());
    #endif
    /*
     * If the tap happened outside the image we might still start moving the
     * image when the motion enters the image.
     */
    if (!m_MotionOngoing) {
        QPointF  position;

        position = event->pos();
        if (position.x() < imageX() ||
            position.y() < imageY() ||
            position.x() > imageX() + imageDX() ||
            position.y() > imageY() + imageDY()) {
            
            SYS_DEBUG ("Rejected... %g, %g not in %d, %d - %d, %d",
                position.x(), position.y(),
                imageX(), imageY(), 
                imageX() + imageDX(), 
                imageY() + imageDY());

            return;
        }

        m_MotionOngoing = true;
        m_LastClick = event->pos();
        return;
    }

    m_UserOffset = event->pos() - m_LastClick;
    queueRedrawImage ();
}
#endif
void
WallpaperEditorWidget::retranslateUi()
{
    if (m_DoneAction)
        //% "Save"
        m_DoneAction->setText (qtTrId("qtn_comm_save"));

    if (m_CancelAction)
        //% "Cancel"
        m_CancelAction->setText (qtTrId("qtn_comm_cancel"));
}

void
WallpaperEditorWidget::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    m_Physics->stop();
}


#if 0
void
WallpaperEditorWidget::mouseReleaseEvent (
        QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);

    SYS_DEBUG ("");
    if (!m_MotionOngoing) {
        toggleTitlebars (true);
        return;
    }

    SYS_DEBUG ("Finalizing something...");
    m_MotionOngoing = false;    
    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    toggleTitlebars (true);
}
#endif
/*******************************************************************************
 * Stuff for the two finger gestures.
 */
void 
WallpaperEditorWidget::panGestureEvent (
        QGestureEvent *event, 
        QPanGesture   *panGesture)
{
    //SYS_WARNING ("");
    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = 
        panGesture->offset() * itemTransform - 
        QPointF(itemTransform.dx(),itemTransform.dy());

    //gestureWorkaround (&point);

    switch (panGesture->state()) {
        case Qt::GestureStarted:
            m_Physics->pointerPress(QPointF());
        case Qt::GestureUpdated:
            m_Physics->pointerMove(-itemSpaceOffset);

        break;

        case Qt::GestureFinished:
        case Qt::GestureCanceled:
            m_Physics->pointerRelease();
        break;
    }

    event->accept (panGesture);
}


void 
WallpaperEditorWidget::pinchGestureStarted (
        QGestureEvent *event, 
        QPinchGesture *gesture)
{
    QPointF   centerPoint;

    Q_UNUSED (event);

    SYS_DEBUG ("Gesture started");
    if (m_PinchOngoing) {
        SYS_WARNING ("But gesture is not finished yet!");
        return;
    }

    /*
     *
     */
    centerPoint = gesture->centerPoint();
    gestureWorkaround (&centerPoint);

    if (centerPoint.x() < imageX() ||
            centerPoint.y() < imageY() ||
            centerPoint.x() > imageX() + imageDX() ||
            centerPoint.y() > imageY() + imageDY()) {
        SYS_DEBUG ("Rejected... %g, %g not in %d, %d - %d, %d",
                centerPoint.x(), centerPoint.y(),
                imageX(), imageY(), 
                imageX() + imageDX(), 
                imageY() + imageDY());
        return;
    }

    m_OriginalScaleFactor = m_Trans.scale();
    m_LastClick = centerPoint;
    m_PinchOngoing = true;

    if (!m_NoTitlebar) {
        m_LastClick += toggleTitlebars (true);
    }

    m_ImageFixpoint = QPointF (
            (centerPoint.x() - m_Trans.x()) / m_Trans.scale(),
            (centerPoint.y() - m_Trans.y()) / m_Trans.scale());

    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureUpdate (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    QPointF  centerPoint;
    qreal    newScale;

    Q_UNUSED (event);

    if (!m_PinchOngoing)
        return;

    //SYS_DEBUG ("Gesture update");
    /*
     *
     */
    centerPoint = gesture->centerPoint();
    gestureWorkaround (&centerPoint);

    /*
     * Updating the scale, we have a lower limit for this.
     */
    newScale = gesture->totalScaleFactor() * m_OriginalScaleFactor;
    if (newScale < ScaleLowerLimit)
        newScale = ScaleLowerLimit;
    m_Trans.setScale (newScale);

    m_UserOffset = QPointF (
            centerPoint.x() - m_ImageFixpoint.x() * m_Trans.scale() - m_Trans.x(),
            centerPoint.y() - m_ImageFixpoint.y() * m_Trans.scale() - m_Trans.y());
        
    event->accept(gesture);

    setupPanningPhysics ();

    /*
     * No frame drop here: the pinch gesture is much better this way...
     */
    redrawImage ();
}

void 
WallpaperEditorWidget::pinchGestureEnded (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    Q_UNUSED (event);
    SYS_DEBUG ("Gesture finished");

    if (!m_PinchOngoing)
        return;

    m_PinchOngoing = false;
    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    toggleTitlebars (true);

    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    Q_UNUSED (event);

    if (gesture->state() == Qt::GestureStarted) {
        pinchGestureStarted (event, gesture);
    } else if (gesture->state() == Qt::GestureFinished) {
        pinchGestureEnded (event, gesture);
    } else {
        pinchGestureUpdate (event, gesture);
    }
}

/*
 * The pinch gesture event coordinate system is not the same as the motion event
 * event coordinate system in portrait mode. This hackish workaround will
 * transform the coordinates if necessary.
 */
void 
WallpaperEditorWidget::gestureWorkaround (
        QPointF *point)
{
    bool portrait = (geometry().height() > geometry().width());

    if (portrait) {
        QPointF tmp (geometry().width() - point->y(), point->x());
        *point = tmp;
    }
}

bool
WallpaperEditorWidget::supportsLandscape () const
{
    return !m_OrientationLocked || m_Orientation == M::Landscape;
}

bool
WallpaperEditorWidget::supportsPortrait () const
{
    return !m_OrientationLocked || m_Orientation == M::Portrait;
}

void 
WallpaperEditorWidget::setupPanningPhysics ()
{
    qreal  left, top;
    qreal  right, bottom;

    left = -0.90 * imageDX();
    top  = -0.90 * imageDY();
    right = 0.10 * imageDX();
    bottom = 0.10 * imageDY();

    m_Physics->setRange (
            QRectF(left, top, 
                m_Trans.expectedSize().width() - right - left,
                m_Trans.expectedSize().height() - bottom - top));
}

