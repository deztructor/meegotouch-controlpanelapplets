/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
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
#include "ut_trackerconnection.h"

#include <QString>
#include <QStringList>
#include "trackerconnection.h"

#define DEBUG 
#define WARNING
#include "../debug.h"

void
Ut_TrackerConnection::initTestCase ()
{
    SYS_DEBUG ("");
}

void
Ut_TrackerConnection::cleanupTestCase ()
{
    SYS_DEBUG ("");
}

void
Ut_TrackerConnection::init ()
{
    SYS_DEBUG ("");
}

void
Ut_TrackerConnection::cleanup ()
{
    SYS_DEBUG ("");
}

void
Ut_TrackerConnection::testConstructDestruct ()
{
    TrackerConnection *tc = 0;

    tc = TrackerConnection::instance ();

    QVERIFY (tc);
    QCOMPARE (tc, tc->s_Instance);

    delete tc;

    QVERIFY (! tc->s_Instance);
}

QTEST_MAIN(Ut_TrackerConnection)

