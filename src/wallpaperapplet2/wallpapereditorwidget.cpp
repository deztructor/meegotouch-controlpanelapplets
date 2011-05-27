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

#include <QStyle>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QPixmap>
#include <QImage>
#include <QTimer>

#ifndef NO_EDITING
#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>
#endif

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MAction>
#include <MApplicationWindow>
#include <MApplication>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(WallpaperEditorWidget)

static const qreal ScaleLowerLimit = 0.15;

#define DEBUG
#define WARNING
#include "../debug.h"

static const int ExtraMargin = 0;
static const int TitleBarHeight = 60;
static const int alwaysVisibleFromImage = 15;
static const qreal scaleLowerLimit = 20.0;
static const qreal scaleUpperLimit = 400.0;

/*!
 * WallpaperEditorWidget constructor
 *
 * \param wallpaperBusinessLogic An WallpaperBusinessLogic instance
 * \param parent The parent widget (if any)
 */
WallpaperEditorWidget::WallpaperEditorWidget (
        WallpaperBusinessLogic *wallpaperBusinessLogic, 
        QGraphicsWidget        *parent) :
    WallpaperViewWidget (wallpaperBusinessLogic, parent),
    m_NoTitlebar (false),
    m_OrientationLocked (false),
    m_PinchOngoing (false),
    m_PanOngoing (false),
    m_HasPendingRedraw (false),
    m_Physics (0)
{
//    WallpaperEditorWidgetStyle *s = (WallpaperEditorWidgetStyle *)
//        (style());

    SYS_WARNING ("----------------------------------------------");
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
     * Creating the MPhysics2DPanning object. These values are copyed from the 
     * mpannablewidgetstyle.css CSS file.
     */
    m_Physics = new MPhysics2DPanning (this);
    m_Physics->setPanDirection (Qt::Vertical | Qt::Horizontal);
    m_Physics->setEnabled(true);
  
#if 0
    SYS_WARNING ("*** PointerSpringK  = %g", style()->pointerSpringK());
    SYS_WARNING ("*** Friction        = %g", style()->friction());
    SYS_WARNING ("*** SlidingFriction = %g", style()->slidingFriction());
    SYS_WARNING ("*** BorderSpringK   = %g", style()->borderSpringK());
    SYS_WARNING ("*** BorderFriction  = %g", style()->borderFriction());
    SYS_WARNING ("*** maximumvelocity = %g", style()->maximumVelocity());
#endif

    m_Physics->setPointerSpringK (style()->pointerSpringK());
    m_Physics->setFriction (style()->friction());
    m_Physics->setSlidingFriction (style()->slidingFriction());
    m_Physics->setBorderSpringK (style()->borderSpringK());
    m_Physics->setBorderFriction (style()->borderFriction());
    m_Physics->setMaximumVelocity (style()->maximumVelocity());

    connect (m_Physics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(panningPhysicsPositionChanged(const QPointF &)));
    connect (m_Physics, SIGNAL(panningStopped()),
            this, SLOT(panningPhysicsPanningStopped()));

    /*
     *
     */
    m_ScalePhysics = new MPhysics2DPanning (this);
    m_ScalePhysics->setPanDirection (Qt::Vertical);
    m_ScalePhysics->setEnabled(true);
    
    m_ScalePhysics->setPointerSpringK(0.25);
    m_ScalePhysics->setFriction(0.6);
    m_ScalePhysics->setSlidingFriction(0.20);
    m_ScalePhysics->setBorderSpringK(0.02);
    m_ScalePhysics->setBorderFriction(0.25);
    m_ScalePhysics->setMaximumVelocity(70);
    m_ScalePhysics->setRange (
            QRectF(0.0, scaleLowerLimit, 0.0, scaleUpperLimit));
    m_ScalePhysics->setPosition (QPointF(0.0, 100.0));
    connect (m_ScalePhysics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(scalePhysicsPositionChanged(const QPointF &)));

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

    /*
     * Enabling two finger gestures.
     */
    grabGesture (Qt::PinchGesture, Qt::GestureFlags());
    grabGesture (Qt::PanGesture, Qt::GestureFlags());
}

/*!
 * Destructor for WallpaperEditorWidget
 */
WallpaperEditorWidget::~WallpaperEditorWidget ()
{
    SYS_WARNING ("");
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
    WallpaperViewWidget::paint (painter, option, widget);
}

/*!
 * Method for creating the editor-widget contents
 */
void
WallpaperEditorWidget::createContent ()
{
    SYS_DEBUG ("");
    WallpaperViewWidget::createContent ();
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
}

void 
WallpaperEditorWidget::scalePhysicsPositionChanged(
        const QPointF    &position)
{
    SYS_DEBUG ("");
    qreal scalefactor = position.y() / 100.0;

    if (scalefactor < 0.05)
        scalefactor = 0.05;

    SYS_DEBUG ("position    %g, %g", position.x(), position.y());
    SYS_DEBUG ("scalefactor %g", scalefactor);
    m_Trans.setScale (scalefactor);
    setupPanningPhysics ();
    queueRedrawImage ();
}

void 
WallpaperEditorWidget::panningPhysicsPositionChanged(
        const QPointF    &position)
{
    m_UserOffset = position;
    redrawImage ();
}

void 
WallpaperEditorWidget::panningPhysicsPanningStopped ()
{
    redrawImage ();
}

/*!
 * This virtual method is executed when we already have an MApplicationPage as
 * parent. We initialize the page here.
 */

void
WallpaperEditorWidget::saveImage ()
{
    SYS_DEBUG ("");
    WallpaperITrans   *ltrans, *ptrans;

    m_Trans += m_UserOffset;
    m_UserOffset = QPointF();
    

    ltrans = m_Trans.orientation() == M::Landscape ?
        &m_Trans : &m_LandscapeTrans;
    ptrans = m_Trans.orientation() == M::Portrait ?
        &m_Trans : &m_PortraitTrans;


    WallpaperViewWidget::saveImage ();
}

/*!
 * This slot is called when the user activates the 'done' action and so we have
 * to store the settings.
 */
void
WallpaperEditorWidget::slotDoneActivated ()
{
    SYS_DEBUG ("");
    MWindow  *win;

    /*
     * We ignore the button press while the image is moving.
     */
    if (m_Physics->inMotion())
        m_Physics->stop();

    if (m_ScalePhysics->inMotion()) 
        m_ScalePhysics->stop();

    saveImage ();

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

    SYS_WARNING ("UNIMPLEMENTED");
}

/*!
 * A slot for handling the 'Cancel' button presses
 */
void
WallpaperEditorWidget::slotCancelActivated ()
{
    SYS_DEBUG ("");
    /*
     * We ignore the button press while the image is moving.
     */
    if (m_Physics->inMotion())
        m_Physics->stop();

    if (m_ScalePhysics->inMotion()) 
        m_ScalePhysics->stop();

    emit cancelClicked ();
    emit closePage ();

    /*
     *
     */
    m_BusinessLogic->endEdit ();
}

/*!
 * Re-implemented method for setting back the non-fullscreen window-state
 */
bool 
WallpaperEditorWidget::back ()
{
    SYS_DEBUG ("");
    MWindow *win;

    /*
     * Turning back from fullscreen. This could be done in the destructor, but
     * that ends up with a segfault in the Qt...
     */
    SYS_DEBUG ("");

    win = MApplication::activeWindow ();
    if (win)
        win->showNormal();

    return DcpStylableWidget::back();
}

/******************************************************************************
 * Private low level functions.
 */


/*!
 * Returns the X offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageX () const
{
    int retval = WallpaperViewWidget::imageX();

    retval += m_UserOffset.x();
    
    //SYS_DEBUG ("*** retval = %d", retval);
    return retval;
}

/*!
 * Returns the Y offset where the image should be painted inside the widget. 
 */
int
WallpaperEditorWidget::imageY () const
{
    int retval = WallpaperViewWidget::imageY ();
    
    retval += m_UserOffset.y();

    //SYS_DEBUG ("*** retval = %d", retval);
    return retval;
}


void 
WallpaperEditorWidget::queueRedrawImage ()
{
    SYS_DEBUG ("");
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
    m_HasPendingRedraw = false;
    update();
#if 0
    /*
     * We need to update the current page and not just this widget because of
     * those damn extra margins coming from somewhere.
     */
    //SYS_DEBUG ("Drawing...");
    m_HasPendingRedraw = false;
    if (MApplication::activeApplicationWindow())
        MApplication::activeApplicationWindow()->currentPage()->update();
#endif
}

void 
WallpaperEditorWidget::orientationChanged (
        M::Orientation orientation)
{
    SYS_DEBUG ("");
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
#ifndef NO_EDITING
void 
WallpaperEditorWidget::wheelEvent (
        QGraphicsSceneWheelEvent *event)
{
    SYS_DEBUG ("");
    m_Physics->stop ();
    m_ScalePhysics->pointerPress(QPointF());

    m_ScalePhysics->pointerMove (QPointF(0.0,  event->delta() / 100.0));
    m_ScalePhysics->pointerRelease();
}
#endif


#ifndef NO_EDITING
void
WallpaperEditorWidget::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    SYS_DEBUG ("");
    m_Physics->stop();
}
#endif

#ifndef NO_EDITING
/*******************************************************************************
 * Stuff for the two finger gestures.
 */
void 
WallpaperEditorWidget::panGestureEvent (
        QGestureEvent *event, 
        QPanGesture   *panGesture)
{
    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = 
        panGesture->offset() * itemTransform - 
        QPointF(itemTransform.dx(),itemTransform.dy());

    if (m_PinchOngoing)
        return;

    switch (panGesture->state()) {
        case Qt::GestureStarted:
            m_PanOngoing = true;
            m_Physics->pointerPress(QPointF());
            m_Physics->pointerMove(-itemSpaceOffset);
            break;

        case Qt::GestureUpdated:
            m_Physics->pointerMove(-itemSpaceOffset);
        break;

        case Qt::GestureFinished:
            m_Physics->pointerRelease();
            m_PanOngoing = false;
            break;

        case Qt::GestureCanceled:
            m_Physics->pointerRelease();
            m_PanOngoing = false;
        break;
    }

    event->accept (panGesture);
}


void 
WallpaperEditorWidget::pinchGestureStarted (
        QGestureEvent *event, 
        QPinchGesture *gesture)
{
    SYS_DEBUG ("");
    if (m_PanOngoing) {
        m_Physics->pointerRelease();
        m_PanOngoing = false;
        return;
    }

    m_OriginalScaleFactor = m_Trans.scale();
    qreal startFrom = m_OriginalScaleFactor * 100.0;

    SYS_DEBUG ("m_ScalePhysics->pointerPress (0.0, %g)", startFrom);
    m_ScalePhysics->pointerPress(QPointF());
    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureUpdate (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    SYS_DEBUG ("");
    if (m_PanOngoing) {
        m_Physics->pointerRelease();
        m_PanOngoing = false;
        return;
    }
    /*
     * No frame drop here: the pinch gesture is much better this way...
     */
    qreal scaley =
        m_OriginalScaleFactor - 
        (gesture->totalScaleFactor() * m_OriginalScaleFactor);

    m_ScalePhysics->pointerMove(
            QPointF(0.0, scaley * 100.0));
    
    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureEnded (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    SYS_DEBUG ("");
    if (m_PanOngoing) {
        m_Physics->pointerRelease();
        m_PanOngoing = false;
        return;
    }

    m_ScalePhysics->pointerRelease ();
    event->accept(gesture);
}

void 
WallpaperEditorWidget::pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *gesture)
{
    SYS_DEBUG ("");
    Q_UNUSED (event);

    switch (gesture->state()) {
        case Qt::GestureStarted:
            m_PinchOngoing = true;
            pinchGestureStarted (event, gesture);
            break;

        case Qt::GestureFinished:
        case Qt::GestureCanceled:
            pinchGestureEnded (event, gesture);
            m_PinchOngoing = false;
            break;

        case Qt::GestureUpdated:
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
    SYS_DEBUG ("");
    bool portrait = (geometry().height() > geometry().width());

    if (portrait) {
        QPointF tmp (geometry().width() - point->y(), point->x());
        *point = tmp;
    }
}
#endif

bool
WallpaperEditorWidget::supportsLandscape () const
{
    SYS_DEBUG ("");
    return !m_OrientationLocked || m_Orientation == M::Landscape;
}

bool
WallpaperEditorWidget::supportsPortrait () const
{
    SYS_DEBUG ("");
    return !m_OrientationLocked || m_Orientation == M::Portrait;
}

void 
WallpaperEditorWidget::setupPanningPhysics ()
{
    SYS_DEBUG ("");
    /*
     * The widget's geometry is disturbed by the transparent toolbar, 
     * but the expectedsize is stable.
     */
    QSize   geom = m_Trans.expectedSize();
    qreal   left, top;
    qreal   width, height;
   

    if (geom.height() >= imageDY()) {
        top    = 0.0;
        height = geom.height() - imageDY();
    } else {
        top    = geom.height() - imageDY();
        height = -1.0 * top;
    }
    
    if (geom.width() >= imageDX()) {
        left    = 0.0;
        width   = geom.width() - imageDX();
    } else {
        left    = geom.width() - imageDX();
        width   = -1.0 * left;
    }

    m_Physics->setRange (QRectF(left, top, width, height));
}

