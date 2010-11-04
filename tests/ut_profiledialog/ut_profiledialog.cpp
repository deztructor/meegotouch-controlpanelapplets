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
#include "ut_profiledialog.h"

#include <MApplication>
#include <MDialog>
#include <MListItem>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Ut_ProfileDialog implementation. 
 */
void
Ut_ProfileDialog::init ()
{
}

void
Ut_ProfileDialog::cleanup ()
{
}

static int argc = 1;
static char *argv[] = { (char*) "./ut_profiledialog", NULL };

void
Ut_ProfileDialog::initTestCase ()
{
    app = new MApplication (argc, argv);
    dataIf = new ProfileDataInterface;
    profileDialog = new ProfileDialog (dataIf);
}

void
Ut_ProfileDialog::cleanupTestCase ()
{
    delete profileDialog;
    delete dataIf;

    app->deleteLater ();
}

void
Ut_ProfileDialog::testExec ()
{
    QTimer::singleShot (200, profileDialog, SLOT (accept ()));

    profileDialog->exec ();

    // check whether profiles are available in the menu
    QCOMPARE (profileDialog->profileList.values().count(), 4);
}

void
Ut_ProfileDialog::testSelect ()
{
    QSignalSpy spy (profileDialog, SIGNAL (profileChanged (int)));

    // pick an item... [here we're pick the last one]
    MListItem *anItem = profileDialog->profileList.values().last();
    // get the corresponding profile id
    int itemId = profileDialog->profileList.key (anItem);

    anItem->click ();

    QTest::qWait (50);

    QCOMPARE (spy.count (), 1);
    QList<QVariant> args = spy.takeFirst ();
    // check whether the clicked item is selected
    QCOMPARE (args.at (0).toInt (), itemId);
}

QTEST_APPLESS_MAIN(Ut_ProfileDialog)

