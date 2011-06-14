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
#ifndef UT_DISPLAYBRIEF_H
#define UT_DISPLAYBRIEF_H

#include <QtTest/QtTest>
#include <QObject>
#include "brightnessbrief.h"


class DisplayBrief;
class MApplication;
class DisplayApplet;


class Ut_DisplayBrief : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testConstructWidget ();
    void testConstructBrief();
    void testValue();
    void testMinValue();
    void testMaxValue();
    void testSetValue();

private:
    MApplication              *m_App;
    DisplayApplet             *m_Applet;
    BrightnessBrief           *brief1;
};

#endif

