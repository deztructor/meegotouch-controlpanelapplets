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
#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <QObject>
#include <QPluginLoader>
#include <QtTest/QtTest>
#include <mapplication.h>
#include <mstatusindicatormenuextensioninterface.h>

class MApplicationWindow;

class Ft_PluginLoader : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testprofileplugin ();

private:
    MApplication                    *m_app;
    MApplicationWindow              *m_window;
    MStatusIndicatorMenuInterface   *m_smstub;

    void    DoPluginTest (const QString &soname);
};

#endif
