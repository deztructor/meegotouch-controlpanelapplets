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
#include <MApplication>
#include <MApplicationWindow>

class Ft_AppletLoader : public QObject
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

private:
    MApplication            *m_app;
    MApplicationWindow      *m_window;

    void    DoAppletTest (const char *desktopfile, bool hasBrief);
};

#endif
