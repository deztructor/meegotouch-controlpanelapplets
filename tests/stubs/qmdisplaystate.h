/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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

#ifndef QMDISPLAYSTATE_H
#define QMDISPLAYSTATE_H
#define QMDISPLAYSTATE_STUB_H

#ifndef UNIT_TEST
#  error "This header should be only used in unit tests."
#endif

#include <QObject>

namespace Maemo {


class QmDisplayState : public QObject 
{
Q_OBJECT

public:
    QmDisplayState ();
    ~QmDisplayState ();

    enum DisplayState
    {
        Off = -1,   /**< Display is off */
        Dimmed = 0, /**< Display is dimmed */
        On = 1      /**< Display is on */
    };

    bool set(DisplayState state);
    int getMaxDisplayBrightnessValue();
    int getDisplayBrightnessValue();
    int getDisplayDimTimeout();
    bool getBlankingWhenCharging();
    void setDisplayBrightnessValue(int brightness);
    void setDisplayDimTimeout(int timeout);
    void setDisplayBlankTimeout(int timeout);
    void setBlankingWhenCharging(bool blanking);

private:
    DisplayState m_State;
    int   m_DisplayBrightnessValue;
    int   m_DisplayDimTimeout;
    int   m_DisplayBlankTimeout;
    bool  m_BlankingWhenCharging;
};
}
#endif

