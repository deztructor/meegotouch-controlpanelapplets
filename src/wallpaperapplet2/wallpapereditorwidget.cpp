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
#include "wallpaperutils.h"

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

//#define DEBUG
#define WARNING
#include "../debug.h"

static const int ExtraMargin = 0;
static const int TitleBarHeight = 60;
static const int alwaysVisibleFromImage = 15;

static const qreal scaleLowerLimit = 20.0;
static const qreal scaleUpperLimit = 400.0;
static const qreal rotationMin = -0.0;
static const qreal rotationMax = +720.0;
static const qreal rotationDelta = 360.0;
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
    SYS_WARNING ("----------------------------------------------");
    MWindow *win = MApplication::activeWindow ();

    setObjectName ("WallpaperEditorWidget");
    /*
     * Creating the MPhysics2DPanning object. These values are copyed from the 
     * mpannablewidgetstyle.css CSS file.
     */
    m_Physics = new MPhysics2DPanning (this);
    m_Physics->setPanDirection (Qt::Vertical | Qt::Horizontal);
    m_Physics->setEnabled(true);
  
#if 0
    SYS_DEBUG ("*** PointerSpringK  = %g", style()->pointerSpringK());
    SYS_DEBUG ("*** Friction        = %g", style()->friction());
    SYS_DEBUG ("*** SlidingFriction = %g", style()->slidingFriction());
    SYS_DEBUG ("*** BorderSpringK   = %g", style()->borderSpringK());
    SYS_DEBUG ("*** BorderFriction  = %g", style()->borderFriction());
    SYS_DEBUG ("*** maximumvelocity = %g", style()->maximumVelocity());
#endif

    m_Physics->setPointerSpringK  (style()->pointerSpringK());
    m_Physics->setFriction        (style()->friction());
    m_Physics->setSlidingFriction (style()->slidingFriction());
    m_Physics->setBorderSpringK   (style()->borderSpringK());
    m_Physics->setBorderFriction  (style()->borderFriction());
    m_Physics->setMaximumVelocity (style()->maximumVelocity());

    connect (m_Physics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(panningPhysicsPositionChanged(const QPointF &)));
    connect (m_Physics, SIGNAL(panningStopped()),
            this, SLOT(panningPhysicsPanningStopped()));

    /*
     *
     */
    m_ScalePhysics = new MPhysics2DPanning (this);
    m_ScalePhysics->setPanDirection (Qt::Vertical | Qt::Horizontal);
    m_ScalePhysics->setEnabled(true);
    
    m_ScalePhysics->setPointerSpringK  (style()->pointerSpringK());
    m_ScalePhysics->setFriction        (style()->friction());
    m_ScalePhysics->setSlidingFriction (style()->slidingFriction());
    m_ScalePhysics->setBorderSpringK   (style()->borderSpringK());
    m_ScalePhysics->setBorderFriction  (style()->borderFriction());
    m_ScalePhysics->setMaximumVelocity (style()->maximumVelocity());
    m_ScalePhysics->setRange (
            QRectF(rotationMin, scaleLowerLimit, rotationMax, scaleUpperLimit));
    m_ScalePhysics->setPosition (QPointF(rotationDelta, 100.0));

    connect (m_ScalePhysics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(scalePhysicsPositionChanged(const QPointF &)));
    connect (m_ScalePhysics, SIGNAL(panningStopped()),
            this, SLOT(scalePhysicsPanningStopped()));

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

    connect (&m_RotateAnimation, SIGNAL(finished()),
            this, SLOT(rotateAnimationFinished()));
}

/*!
 * Destructor for WallpaperEditorWidget
 */
WallpaperEditorWidget::~WallpaperEditorWidget ()
{
}

void
WallpaperEditorWidget::initialize (
        QuillImage   &image,
        QSize         size)
{
    WallpaperViewWidget::initialize (image, size);
    setupPanningPhysics ();
}

void 
WallpaperEditorWidget::scalePhysicsPositionChanged(
        const QPointF    &position)
{
    SYS_DEBUG ("*** position = %s", SYS_POINTF(position));

    qreal scalefactor = position.y() / 100.0;

    if (scalefactor < 0.05)
        scalefactor = 0.05;

    m_Trans.setScale (scalefactor);
    m_Trans.setRotation (position.x() - rotationDelta);

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
    //SYS_DEBUG ("");
    redrawImage ();
}

void 
WallpaperEditorWidget::scalePhysicsPanningStopped ()
{
    qreal rotation  = m_Trans.rotation();
    qreal rRotation = 0.0;

    for (qreal rounded = -360.0; rounded <= 360.0; rounded += 90.0) {
        if (rotation >= rounded - 45.0 && rotation <= rounded + 45.0) {
            rRotation = rounded;
            break;
        }
    }

    SYS_DEBUG ("*** rotation = %g", rotation);
    SYS_DEBUG ("*** rRotation = %g", rRotation);

    if (rotation != rRotation) {
        m_RotateAnimation.setTargetObject (m_ImageWidget);
        m_RotateAnimation.setPropertyName ("rotation");
        m_RotateAnimation.setEasingCurve(QEasingCurve::InOutCubic);
        m_RotateAnimation.setStartValue (rotation);
        m_RotateAnimation.setEndValue (rRotation);
        m_RotateAnimation.setDuration (500.0);
        m_RotateAnimation.start();
    }


//    redrawImage ();
}

void
WallpaperEditorWidget::rotateAnimationFinished ()
{
    qreal rotation = m_ImageWidget->rotation();

    SYS_DEBUG ("");

    if (rotation == 360.0 || rotation == -360.0) {
        rotation = 0.0;
        m_ImageWidget->setRotation (rotation);
    }

    m_Trans.setRotation(rotation);
    m_ScalePhysics->setPosition (
            QPointF(rotation + rotationDelta, m_Trans.scale() * 100.0));
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

    if (m_Saving) {
        SYS_DEBUG ("Saving is being performed.");
        return;
    }

    /*
     * We ignore the button press while the image is moving.
     */
    if (m_Physics->inMotion())
        m_Physics->stop();

    if (m_ScalePhysics->inMotion()) 
        m_ScalePhysics->stop();

    WallpaperViewWidget::slotDoneActivated ();
}

/*!
 * A slot for handling the 'Cancel' button presses
 */
void
WallpaperEditorWidget::slotCancelActivated ()
{
    SYS_DEBUG ("");
    if (m_Saving) {
        SYS_DEBUG ("Saving is being performed.");
        return;
    }


    /*
     * We ignore the button press while the image is moving.
     */
    if (m_Physics->inMotion())
        m_Physics->stop();

    if (m_ScalePhysics->inMotion()) 
        m_ScalePhysics->stop();

    WallpaperViewWidget::slotCancelActivated ();
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
    WallpaperViewWidget::redrawImage ();
    //update();
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
void 
WallpaperEditorWidget::wheelEvent (
        QGraphicsSceneWheelEvent *event)
{
    bool     ctrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
    
    SYS_DEBUG ("ctrl = %s", SYS_BOOL(ctrl));

    /*
     * We might auto-rotate. If we do we don't accept nothing.
     */
    if (m_RotateAnimation.state() == QAbstractAnimation::Running)
        return;


    m_Physics->stop ();
    m_ScalePhysics->pointerPress(QPointF());
    
    if (!ctrl) {
        m_ScalePhysics->pointerMove (QPointF(0.0,  event->delta() / 100.0));
    } else {
        m_ScalePhysics->pointerMove (QPointF(event->delta() / 100.0, 0.0));
    }

    m_ScalePhysics->pointerRelease();
}


void
WallpaperEditorWidget::mousePressEvent (
        QGraphicsSceneMouseEvent *event)
{
    m_Physics->stop();
}

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
        
        case Qt::NoGesture:
            SYS_WARNING ("I dont know what to do when Qt::NoGesture");
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
    /*
     * We might auto-rotate. If we do we don't accept nothing.
     */
    if (m_RotateAnimation.state() == QAbstractAnimation::Running)
        return;
    
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
     * 
     */
    qreal scalex = gesture->rotationAngle();
    qreal scaley =
        m_OriginalScaleFactor - 
        (gesture->totalScaleFactor() * m_OriginalScaleFactor);
    
    m_ScalePhysics->pointerMove(
            QPointF(scalex - rotationDelta, scaley * 100.0));
    
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
            break;

        case Qt::NoGesture:
            SYS_WARNING ("I dont know what to do when Qt::NoGesture");
            break;
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
    /*
     * The widget's geometry is disturbed by the transparent toolbar, 
     * but the expectedsize is stable.
     */
    QSize   geom = m_Trans.expectedSize();
    qreal   left, top;
    qreal   width, height;
    int     imagedx, imagedy;

    if (m_Trans.rotation() == 90.0 || m_Trans.rotation() == -90.0) {
        imagedx = imageDY();
        imagedy = imageDX();
    } else {
        imagedx = imageDX();
        imagedy = imageDY();
    }

    if (geom.height() >= imagedy) {
        top    = 0.0;
        top    -= m_Trans.offset().y();
        height = geom.height() - imagedy;
    } else {
        top     = geom.height() - imagedy;
        top    -= m_Trans.offset().y();

        height  = -1.0 * top;
        height -= m_Trans.offset().y();
    }
    
    if (geom.width() >= imagedx) {
        left    = 0.0;
        left   -= m_Trans.offset().x();
        width   = geom.width() - imagedx;
    } else {
        left    = geom.width() - imagedx;
        left   -= m_Trans.offset().x();

        width   = -1.0 * left;
        width -= m_Trans.offset().x();
    }

    //SYS_DEBUG ("%g, %g (%gx%g)", left, top, width, height);
    m_Physics->setRange (QRectF(left, top, width, height));
}

