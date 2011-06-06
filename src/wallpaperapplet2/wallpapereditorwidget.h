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
#ifndef WALLPAPEREDITORWIDGET_H
#define WALLPAPEREDITORWIDGET_H

#include "wallpaperviewwidget.h"
#include "wallpapereditorwidgetstyle.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperitrans.h"

#include <QPointer>
#include <MPhysics2DPanning>
#include <meegocontrolexport.h>
#include <dcpwidget.h>
#include <MStylableWidget>
#include <QPropertyAnimation>

class MContainer;
class MLinearLayoutPolicy;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class MAction;
class QSize;
class QPixmap;
class QImage;
class QPointF;
class QGestureEvent;

/*!
 *
 */
class MC_EXPORT WallpaperEditorWidget : public WallpaperViewWidget
{
    Q_OBJECT

public:
    WallpaperEditorWidget (
            WallpaperBusinessLogic *wallpaperBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~WallpaperEditorWidget ();


    virtual bool back ();
    virtual void saveImage ();

protected:

#ifndef NO_EDITING
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent (QGraphicsSceneWheelEvent *event);

    void panGestureEvent (
            QGestureEvent *event, 
            QPanGesture   *panGesture);

    virtual void pinchGestureEvent (
            QGestureEvent *event, 
            QPinchGesture *gesture);

    virtual void pinchGestureStarted (
            QGestureEvent *event, 
            QPinchGesture *gesture);

    virtual void pinchGestureUpdate (
            QGestureEvent *event, 
            QPinchGesture *gesture);
    
    virtual void pinchGestureEnded (
            QGestureEvent *event, 
            QPinchGesture *gesture);
#endif


    
    protected slots:
        virtual void initialize (QuillImage &image, QSize size);
        virtual void slotDoneActivated ();
        virtual void slotCancelActivated ();
        virtual void redrawImage ();

    private slots:
    void queueRedrawImage ();
    void orientationChanged (M::Orientation orientation);
    void panningPhysicsPositionChanged (const QPointF &position);
    void panningPhysicsPanningStopped ();
    void scalePhysicsPositionChanged(const QPointF &position);
    void scalePhysicsPanningStopped ();
        void rotateAnimationFinished ();

    protected:
        virtual int imageX () const;
        virtual int imageY () const;

    private:
        M_STYLABLE_WIDGET(WallpaperEditorWidgetStyle)
        void setupPanningPhysics ();    
        void gestureWorkaround (QPointF *point);
        bool supportsLandscape () const;
        bool supportsPortrait () const;
 
private:
    QImage                m_bgLandscape;
    QImage                m_bgPortrait;

    bool                  m_NoTitlebar;
    bool                  m_OrientationLocked;
    M::Orientation        m_Orientation;

    QPointF               m_LastClick;
    QPointF               m_UserOffset;
    QPointF               m_ImageFixpoint;

    WallpaperITrans       m_LandscapeTrans;
    WallpaperITrans       m_PortraitTrans;
    
    qreal                 m_OriginalScaleFactor;
    bool                  m_PinchOngoing;
    bool                  m_PanOngoing;
    bool                  m_HasPendingRedraw;
    MPhysics2DPanning    *m_Physics;
    MPhysics2DPanning    *m_ScalePhysics;
        QPropertyAnimation    m_RotateAnimation;
};

#endif
