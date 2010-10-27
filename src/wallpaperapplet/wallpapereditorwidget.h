/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QPointer>
#include "dcpwidget.h"

#include "wallpaperbusinesslogic.h"
#include "wallpaperitrans.h"

class MContainer;
class MLinearLayoutPolicy;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class MButton;
class QSize;
class QPixmap;
class QPointF;
class QGestureEvent;
class WallpaperInfoHeader;

/*!
 * Problem: During the one finger move the image lags behind the finger.
 * Solution: Implemented framedropping, the movement is faster now.
 *
 * Problem: While resizing the image with the pinch gesture the image moves.
 * Solution: The image is moved during the gesture so the point that was under
 *   the midpoint remains under the midpoint even when the midpoint is moved
 *   (moving while resizing).
 *
 * Problem: User can use one finger to move the image outside of the screen.
 * Solution: The one finger moving of the image can be performed only when
 *   started from inside the image. This way the image is always under the
 *   finger, so it can not be moved outside the screen.
 * 
 * Problem: Two finger resizing can move the image outside the screen.
 * Solution: The pinch gesture is accepted only if originated with the midpoint
 *   inside the image. 
 *
 * Problem: The image can be resized so small, it is not possible to magnify it.
 * Solution: We can not let the user resize the image without the midsection
 *   being outside the image, so we imeplement a lower limit for the scale
 *   factor.
 * 
 * Problem: Image can moved under the titlebar impossible to pick it up.
 * Solution: (1) tap on the black area (2) move your finger onto the image to
 *   pick it up.
 *
 * Problem: Portrait image editing is not as expected.
 * Solution: We had to add a workaround code because the pinch gesture
 *   coordinate system is not the same as in the motion events. I added this
 *   point as a reminder to test the applet in portrait mode also.
 */
class WallpaperEditorWidget : public DcpWidget
{
    Q_OBJECT

public:
    WallpaperEditorWidget (
            WallpaperBusinessLogic *wallpaperBusinessLogic, 
            QGraphicsWidget        *parent = 0);
    ~WallpaperEditorWidget ();

    virtual bool pagePans () const;
    virtual bool back ();
    virtual void paint (
            QPainter                        *painter,
            const QStyleOptionGraphicsItem  *option,
            QWidget                         *widget = 0);

protected:
    virtual void polishEvent ();
    virtual void retranslateUi();
    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent (QGraphicsSceneWheelEvent *event);

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

private slots:
    void redrawImage ();
    void queueRedrawImage ();
    void orientationChanged (M::Orientation orientation);
    void slotDoneActivated ();
    void slotCancelActivated ();
    void createContent ();
    void createWidgets ();

private:
    QPointF toggleTitlebars (bool show);
    int  imageX () const;
    int  imageY () const;
    int  imageDX () const;
    int  imageDY () const;
    void gestureWorkaround (QPointF *point);

private:
    QPointer<WallpaperBusinessLogic>  m_WallpaperBusinessLogic;

    WallpaperInfoHeader  *m_InfoHeader;
    QPixmap               m_bgLandscape;
    QPixmap               m_bgPortrait;
    MButton              *m_DoneButton;
    MButton              *m_CancelButton;
    bool                  m_NoTitlebar;
    M::Orientation        m_Orientation;

    QPointF               m_LastClick;
    QPointF               m_UserOffset;
    QPointF               m_ImageFixpoint;

    WallpaperITrans       m_Trans;
    WallpaperITrans       m_LandscapeTrans;
    WallpaperITrans       m_PortraitTrans;
    
    qreal                 m_OriginalScaleFactor;
    bool                  m_PinchOngoing;
    bool                  m_MotionOngoing;
    bool                  m_HasPendingRedraw;
    #ifdef UNIT_TEST
    friend class Ut_WallpaperEditorWidget;
    #endif
};

#endif
