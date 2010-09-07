/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

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

#ifndef DISPLAYBUSINESSLOGIC_H
#define DISPLAYBUSINESSLOGIC_H

#ifdef HAVE_QMSYSTEM
#  include <qmdisplaystate.h>
using namespace Maemo;
#endif

#include <QObject>

class MGConfItem;

/*!
 * Implements the business logic that needed to handle the display settings.
 * This class is using the QmSystem library when it is available and uses GConf
 * database items when QmSystem is not found. The aim of using the QmSystem is
 * simply to hide the implementation details (that is the GConf keys) sdo that
 * we can create an independent implementation.
 *
 * Please note that not all the GConf keys are implemented yet.
 */
class DisplayBusinessLogic : public QObject
{
    Q_OBJECT

public:
    DisplayBusinessLogic (QObject* parent = 0);
    virtual ~DisplayBusinessLogic();

    QList<int> brightnessValues();
    int selectedBrightnessValueIndex ();
    int selectedBrightnessValue ();
    QList<int> screenLightsValues();
    int selectedScreenLightsValue();
    bool blankInhibitValue();

public slots:
    void setBrightnessValue(int value);
    void setScreenLightTimeouts (int index);
    void setBlankInhibitValue(bool value);

private: 
    #ifdef HAVE_QMSYSTEM
    QmDisplayState *m_Display;
    #else
    MGConfItem     *m_MaxDisplayBrightness;
    MGConfItem     *m_CurrentBrightness;
    #endif
    MGConfItem     *m_possibleDimValues;
    #ifdef UNIT_TEST
    friend class Ut_DisplayBusinessLogic;
    #endif
};

#endif
