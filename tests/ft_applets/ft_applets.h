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
#ifndef APPLET_LOADER_H
#define APPLET_LOADER_H

#include <QObject>
#include <QtTest/QtTest>

class MApplication;
class MApplicationWindow;
class MApplicationPage;

class Ft_Applets : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testbatteryapplet ();
    void testdisplayapplet ();
    void testprofileapplet ();
    void testusbapplet ();
    void testresetapplet ();
    void testaboutapplet ();
    void testthemeapplet ();
    void testwallpaperapplet ();
    void testwarrantyapplet ();
    void testofflineapplet ();
    void testsoundsettingsapplet ();

private:
    void doAppletTest (const char *soName,
                       bool hasBrief = true,
                       bool hasView = true);

private:
    MApplication            *m_app;
    MApplicationWindow      *m_window;
};

#endif
