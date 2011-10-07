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

#include "logowidget.h"

#define DEBUG
#define WARNING
#include "../debug.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(LogoWidget)

LogoWidget::LogoWidget (
        QGraphicsWidget *parent) :
    MImageWidget (parent),
    m_LastTime (0),
    m_StateCounter (0)
{
    #ifndef MEEGO
    setImage ("icon-l-about-nokia-logo");
    #else
    setImage ("icon-l-about-meego-logo");
    #endif

    //on meego we set the logo with diffrent ID:
    setObjectName ("AboutAppletLogoImage");    

    //grabGesture (Qt::TapGesture, Qt::GestureFlags());
}

bool 
LogoWidget::event (
        QEvent* event)
{
    time_t t;
    bool   retval = false;

    time (&t);

    /*
     * If we already at the end-state.
     */
    if (m_StateCounter == 4)
        goto finalize;

    /*
     * If the user is too slow...
     */
    if (m_LastTime > 0 && t - m_LastTime > 2) {
        goto finalize_fail;
    }

    if (event->type() != QEvent::GraphicsSceneMousePress &&
            event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        goto finalize;

    if (m_StateCounter == 0 && 
            event->type() != QEvent::GraphicsSceneMousePress)
        goto finalize_fail;
    else if (m_StateCounter == 1 && 
            event->type() != QEvent::GraphicsSceneMousePress)
        goto finalize_fail;
    else if (m_StateCounter == 2 && 
            event->type() != QEvent::GraphicsSceneMouseDoubleClick)
        goto finalize_fail;
    else if (m_StateCounter == 3 && 
            event->type() != QEvent::GraphicsSceneMousePress)
        goto finalize_fail;

    ++m_StateCounter;
    m_LastTime = t;

    if (m_StateCounter == 4)
        turnedOn ();

finalize:
    SYS_DEBUG ("succ event type = %d time = %d counter = %d", 
        (int) event->type(),
        (int) t,
        m_StateCounter);
    return retval;

finalize_fail:
    m_LastTime     = 0;
    m_StateCounter = 0;
    SYS_DEBUG ("fail event type = %d time = %d counter = %d", 
        (int) event->type(),
        (int) t,
        m_StateCounter);
    return retval;
}

void
LogoWidget::turnedOn ()
{
    SYS_WARNING ("Eastern eggs turned on.");
}
