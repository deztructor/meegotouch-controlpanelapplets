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
#ifndef UT_PROFILEBUTTONS_H
#define UT_PROFILEBUTTONS_H

#include <QtTest/QtTest>
#include <QObject>

class ProfileButtons;
class MApplication;

class SignalSink : public QObject
{
Q_OBJECT
public slots:
    void profileSelected (int id);

signals:
    bool selectProfile (int id);
    friend class Ut_ProfileButtons;
};

class Ut_ProfileButtons : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testInit ();
    
private:
    MApplication              *m_App;
    ProfileButtons            *m_Buttons;
    SignalSink                 m_SignalSink;
};

#endif


