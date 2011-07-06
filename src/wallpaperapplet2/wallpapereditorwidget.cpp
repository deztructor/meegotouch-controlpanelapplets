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
#include <MWidgetStyle>

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
static const qreal rotationMin = -0.0;
static const qreal rotationMax = +720.0;
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
    m_OrientationLocked (false),
    m_PinchOngoing (false),
    m_PanOngoing (false),
    m_HasPendingRedraw (false),
    m_Physics (0),
    m_RotateAnimation (0)
{
    SYS_DEBUG ("start");
    MWindow *win = MApplication::activeWindow ();

    setObjectName ("WallpaperEditorWidget");
    /*
     *
     */
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
    SYS_DEBUG ("end");
}

/*!
 * Destructor for WallpaperEditorWidget
 */
WallpaperEditorWidget::~WallpaperEditorWidget ()
{
    SYS_DEBUG ("");
}

void 
WallpaperEditorWidget::applyStyle()
{
    SYS_DEBUG ("");
    if (m_Physics) {
        SYS_WARNING ("applyStyle already called.");
        return;
    }

    WallpaperViewWidget::applyStyle();

    /*
     * Creating the MPhysics2DPanning object. These values are copyed from the 
     * mpannablewidgetstyle.css CSS file.
     */
    m_Physics = new MPhysics2DPanning (this);
    m_Physics->setPanDirection (Qt::Vertical | Qt::Horizontal);
    m_Physics->setEnabled(true);
  
#if 1
    SYS_WARNING ("------------------------------------------------");
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
    SYS_WARNING ("m_ScalePhysics->setPosition (%g, %g)", 0.0, 100.0);
    m_ScalePhysics->setPosition (QPointF(0.0, 100.0));

    connect (m_ScalePhysics, SIGNAL(positionChanged(const QPointF &)),
            this, SLOT(scalePhysicsPositionChanged(const QPointF &)));
    connect (m_ScalePhysics, SIGNAL(panningStopped()),
            this, SLOT(scalePhysicsPanningStopped()));

}

void
WallpaperEditorWidget::initialize (
        QuillImage   &image,
        QSize         size)
{
    SYS_DEBUG ("");
    WallpaperViewWidget::initialize (image, size);
    setupPanningPhysics (true);
}

void 
WallpaperEditorWidget::panningPhysicsPositionChanged(
        const QPointF    &position)
{
    SYS_DEBUG ("panning -----------> %s", SYS_POINTF(position));
    m_UserOffset = position;
    redrawImage ();
}

void 
WallpaperEditorWidget::panningPhysicsPanningStopped ()
{
    SYS_DEBUG ("");
    redrawImage ();
}

void 
WallpaperEditorWidget::scalePhysicsPositionChanged(
        const QPointF    &position)
{
    SYS_DEBUG ("");
    qreal scalefactor = position.y() / 100.0;

    SYS_WARNING ("scaling  --------> %s", SYS_POINTF(position));
    if (scalefactor < 0.05)
        scalefactor = 0.05;

    m_Trans.setScale (scalefactor);
    m_Trans.setRotation (position.x());

    queueRedrawImage ();
}


void 
WallpaperEditorWidget::scalePhysicsPanningStopped ()
{
    SYS_DEBUG ("");
    qreal rotation  = m_Trans.rotation();
    qreal rRotation = 0.0;

    /*
     * The starting (current) rotation.
     */
    while (rotation > 360.0)
        rotation -= 360;
    while (rotation < -360)
        rotation += 360;
    m_Trans.setRotation (rotation);

    /*
     * Where we should go...
     */
    for (qreal rounded = -360.0; rounded <= 360.0; rounded += 90.0) {
        if (rotation >= rounded - 45.0 && rotation <= rounded + 45.0) {
            rRotation = rounded;
            break;
        }
    }
    
    SYS_DEBUG ("*** we are at  : %g", rotation);
    SYS_DEBUG ("*** rounded to : %g", rRotation);

    if (rotation != rRotation) {
        SYS_DEBUG ("NEED TO ANIMATE");
        m_RotateAnimation.setTargetObject (m_ImageWidget);
        m_RotateAnimation.setPropertyName ("rotation");
        m_RotateAnimation.setEasingCurve(QEasingCurve::InOutCubic);
        m_RotateAnimation.setStartValue (rotation);
        m_RotateAnimation.setEndValue (rRotation);
        m_RotateAnimation.setDuration (500.0);
        m_RotateAnimation.start();
    } else { 
        setupPanningPhysics ();
    }
    
#if 0
    if (m_ScalePhysics->enabled())
        m_ScalePhysics->setEnabled(false);
#endif
}

void
WallpaperEditorWidget::rotateAnimationFinished ()
{
    SYS_DEBUG ("");
    qreal rotation = m_ImageWidget->rotation();

    SYS_DEBUG ("");

    if (rotation == 360.0 || rotation == -360.0) {
        rotation = 0.0;
        m_ImageWidget->setRotation (rotation);
    }

    m_Trans.setRotation(rotation);

    QPointF position;
    position =  m_ScalePhysics->position ();
    SYS_DEBUG ("m_ScalePhysics->position() = %g, %g", position.x(), position.y());
    SYS_DEBUG ("m_ScalePhysics->setPosition(%g, %g)",
            rotation, m_Trans.scale() * 100.0);
    m_ScalePhysics->setPosition (QPointF(rotation, m_Trans.scale() * 100.0));
        
    setupPanningPhysics ();
}

/*!
 * This virtual method is executed when we already have an MApplicationPage as
 * parent. We initialize the page here.
 */

void
WallpaperEditorWidget::saveImage ()
{
    SYS_DEBUG ("");
 
    m_Trans.setOffset (m_UserOffset);
    
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
    SYS_DEBUG ("");
    int retval = 0.0; //WallpaperViewWidget::imageX();

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
    SYS_DEBUG ("");
    int retval = 0.0; //WallpaperViewWidget::imageY ();
    
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
    SYS_DEBUG ("");
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

/******************************************************************************
 * Stuff for the normal mouse events.
 */
void 
WallpaperEditorWidget::wheelEvent (
        QGraphicsSceneWheelEvent *event)
{
    SYS_DEBUG ("");
    bool     ctrl = QApplication::keyboardModifiers() & Qt::ControlModifier;
    
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
    SYS_DEBUG ("");
    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = 
        panGesture->offset() * itemTransform - 
        QPointF(itemTransform.dx(),itemTransform.dy());

    if (m_PinchOngoing || m_Saving)
        return;

    switch (panGesture->state()) {
        case Qt::GestureStarted:
            SYS_DEBUG ("Qt::GestureStarted");
            m_PanOngoing = true;
            m_Physics->pointerPress(QPointF());
            m_Physics->pointerMove(-itemSpaceOffset);
            break;

        case Qt::GestureUpdated:
            SYS_DEBUG ("Qt::GestureUpdated");
            SYS_DEBUG ("itemSpaceOffset = %s", SYS_POINTF(itemSpaceOffset));
            m_Physics->pointerMove(-itemSpaceOffset);
            break;

        case Qt::GestureFinished:
            SYS_DEBUG ("Qt::GestureFinished");
            m_Physics->pointerRelease();
            m_PanOngoing = false;
            break;

        case Qt::GestureCanceled:
            SYS_DEBUG ("Qt::GestureCanceled");
            m_Physics->pointerRelease();
            m_PanOngoing = false;
            break;
        
        case Qt::NoGesture:
            SYS_DEBUG ("Qt::NoGesture");
            SYS_WARNING ("I dont know what to do when Qt::NoGesture");
            break;
    }

    event->accept (panGesture);
}

void 
WallpaperEditorWidget::pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *pinchGesture)
{
    SYS_DEBUG ("");
    Q_UNUSED (event);

    if (m_Saving)
        return;

    if (m_PanOngoing) {
        SYS_WARNING ("Aborting panning?");
        m_Physics->pointerRelease();
        m_PanOngoing = false;
        return;
    }

    switch (pinchGesture->state()) {
        case Qt::GestureStarted:
            SYS_DEBUG ("Qt::GestureStarted");
            m_PinchOngoing = true;
            pinchGestureStarted (event, pinchGesture);
            break;

        case Qt::GestureFinished:
            SYS_DEBUG ("Qt::GestureFinished");
            pinchGestureEnded (event, pinchGesture);
            m_PinchOngoing = false;
            break;

        case Qt::GestureCanceled:
            SYS_DEBUG ("Qt::GestureCanceled");
            pinchGestureEnded (event, pinchGesture);
            m_PinchOngoing = false;
            break;

        case Qt::GestureUpdated:
            SYS_DEBUG ("Qt::GestureUpdated");
            pinchGestureUpdate (event, pinchGesture);
            break;

        case Qt::NoGesture:
            SYS_DEBUG ("Qt::NoGesture");
            SYS_WARNING ("I dont know what to do when Qt::NoGesture");
            break;
    }
    
    event->accept (pinchGesture);
}

void 
WallpaperEditorWidget::pinchGestureStarted (
        QGestureEvent *event, 
        QPinchGesture *pinchGesture)
{
    SYS_DEBUG ("");
    /*
     * We might auto-rotate. If we do we don't accept nothing.
     */
    if (m_RotateAnimation.state() == QAbstractAnimation::Running)
        return;
            
    QPointF pressAt (m_Trans.rotation(), m_Trans.scale() * 100.0);
            SYS_DEBUG ("m_ScalePhysics->pointerPress (%s)", SYS_POINTF(pressAt));
            m_ScalePhysics->pointerPress(pressAt);
}

void 
WallpaperEditorWidget::pinchGestureUpdate (
            QGestureEvent *event, 
            QPinchGesture *pinchGesture)
{
    /*
     * 
     */
    SYS_DEBUG ("*** rotation           = %g", m_Trans.rotation());
    SYS_DEBUG ("*** scale              = %g", m_Trans.scale());
    SYS_DEBUG ("*** rotationAngle      = %g", pinchGesture->rotationAngle());
    SYS_DEBUG ("*** lastRotationAngle  = %g", pinchGesture->lastRotationAngle());
    SYS_DEBUG ("*** scaleFactor        = %g", pinchGesture->scaleFactor());
    SYS_DEBUG ("*** lastScaleFactor    = %g", pinchGesture->lastScaleFactor());
#if 0
    if (!m_ScalePhysics->enabled()) {
        qreal scaleDiff = m_Trans.scale() - pinchGesture->scaleFactor();
        qreal rotationDiff = m_Trans.rotation() - pinchGesture->rotationAngle();
        bool  enabled = false;

        if (rotationDiff > 10.0 || rotationDiff < -10.0) {
            m_ScalePhysics->setPanDirection (Qt::Horizontal | Qt::Vertical);
            m_ScalePhysics->setEnabled(true);
            enabled = true;
        } else if (scaleDiff > 0.1 || scaleDiff < -0.1) {
            m_ScalePhysics->setPanDirection (Qt::Horizontal | Qt::Vertical);
            m_ScalePhysics->setEnabled(true);
            enabled = true;
        }
       
        if (enabled) {
            QPointF pressAt (m_Trans.rotation(), m_Trans.scale() * 100.0);
            SYS_DEBUG ("m_ScalePhysics->pointerPress (%s)", SYS_POINTF(pressAt));
            m_ScalePhysics->pointerPress(pressAt);
        }
    } 
#endif
    if (m_ScalePhysics->enabled()) {
        QPointF moveTo (
                -1.0 * pinchGesture->rotationAngle(),
                -1.0 * pinchGesture->scaleFactor() * 100.0);
    
        SYS_DEBUG ("m_ScalePhysics->pointerMove (%s)", SYS_POINTF(moveTo));
        m_ScalePhysics->pointerMove(moveTo);
    }
}

void 
WallpaperEditorWidget::pinchGestureEnded (
            QGestureEvent *event, 
            QPinchGesture *pinchGesture)
{
    SYS_DEBUG ("m_ScalePhysics->pointerRelease ()");
    m_ScalePhysics->pointerRelease ();
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

void 
WallpaperEditorWidget::setupPanningPhysics (
        bool movePh)
{
    SYS_DEBUG ("");
    /*
     * The widget's geometry is disturbed by the transparent toolbar, 
     * but the expectedsize is stable.
     */
    QSize   geom = m_Trans.expectedSize();
    QSize   imageSize;
    qreal   left, top;
    qreal   width, height;
    int     imagedx, imagedy;

    imageSize = imageVisualSize (m_Trans.scale());

    if (m_Trans.rotation() == 90.0 || m_Trans.rotation() == -90.0) {
        imagedx = imageDY();
        imagedy = imageDX();
    } else {
        imagedx = imageDX();
        imagedy = imageDY();
    }
    
    if (geom.height() >= imagedy) {
        top    = imageSize.height() / 2.0;
        //top    -= m_Trans.offset().y();

        height = geom.height() - imageSize.height();
    } else {
        top     = geom.height() - imageSize.height() / 2.0;
        //top    -= m_Trans.offset().y();

        height  =  imageSize.height() - geom.height();
    }

    if (geom.width() >= imagedx) {
        left    = imageSize.width() / 2.0;
        //left   -= m_Trans.offset().x();

        width = geom.width() - imageSize.width();
    } else {
        left    = geom.width() - imageSize.width() / 2.0;
        //left   -= m_Trans.offset().x();

        width   =  imageSize.width() - geom.width();
    }
   
    #if 0
    SYS_DEBUG ("*** imageSize = %s", SYS_SIZE(imageSize));
    SYS_DEBUG ("*** offset  = %g, %g",
            m_Trans.offset().x(), m_Trans.offset().y());
    SYS_DEBUG ("*** imaged. = %dx%d", imagedx, imagedy);
    SYS_DEBUG ("*** range   = %g, %g (%gx%g)", left, top, width, height);
    #endif

    m_Physics->setRange (QRectF(left, top, width, height));
   
    /*
     *
     */
    if (movePh) {
        qreal hMargin;
        qreal vMargin;

        if (width == 0.0) {
            hMargin = left;
        } else {
            hMargin = left + (width - left) / 2.0;
        }

        if (height == 0.0) {
            vMargin = top;
        } else {
            vMargin = top + (height - top) / 2.0;
        }

        SYS_DEBUG ("-> %g, %g", hMargin, vMargin);
        m_Physics->setPosition (QPointF(hMargin, vMargin));
    }
}

