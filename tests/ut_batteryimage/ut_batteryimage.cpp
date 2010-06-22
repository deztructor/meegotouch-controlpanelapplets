/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui-applets.
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
#include "ut_batteryimage.h"

#include "batteryimage.h"

#include <MApplication>

#define DEBUG
#include "../debug.h"

/*
 * These are run before and after every test case
 */
void Ut_BatteryImage::init ()
{
}

void Ut_BatteryImage::cleanup ()
{
}

static int argc = 1;
static char *app_name = (char*) "./Ut_BatteryImage";

void Ut_BatteryImage::initTestCase ()
{
    SYS_DEBUG ("---->");
    m_App = new MApplication (argc, &app_name);
}

void
Ut_BatteryImage::cleanupTestCase ()
{
    m_App->deleteLater ();
}

void 
Ut_BatteryImage::testLoadImages ()
{
    BatteryImage *image = new BatteryImage;

    image->loadImages (BatteryImage::ICON_NORMAL);
    QVERIFY (image->m_Images.size() == 10);
    // We need to wait until the theme daemon loads the icon.
    QTest::qWait (1000);
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
        QVERIFY (pixmap->width () >= 48);
        QVERIFY (pixmap->height () >= 48);
    }
    
    image->loadImages (BatteryImage::ICON_POWERSAVE);
    QVERIFY (image->m_Images.size() == 10);
    // We need to wait until the theme daemon loads the icon.
    QTest::qWait (1000);
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
        QVERIFY (pixmap->width () >= 48);
        QVERIFY (pixmap->height () >= 48);
    }
    
    image->loadImages (BatteryImage::ICON_CHARGING);
    QVERIFY (image->m_Images.size() == 10);
    // We need to wait until the theme daemon loads the icon.
    QTest::qWait (1000);
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
        QVERIFY (pixmap->width () >= 48);
        QVERIFY (pixmap->height () >= 48);
    }

    delete image;
}

void 
Ut_BatteryImage::testAnimation ()
{
    BatteryImage *image = new BatteryImage;
    bool          indexUsed[10];
    int           level;

    /*
     * Creating the image that shows the non-charging non-psm state and testing
     * if it follows the battery level.
     */
    image->stopCharging ();
    QVERIFY (image->m_ChargingSpeed == 0);
    QVERIFY (!animationIsOngoing(image));
    QVERIFY (image->m_iconCurrentSet == BatteryImage::ICON_NORMAL);

    for (int n = 9; n >= 0; --n) {
        image->updateBatteryLevel (n);
        QVERIFY (image->m_batteryLevel == n);
        QVERIFY (image->m_ImageIndex == n);
        QVERIFY (image->m_iconCurrentSet == BatteryImage::ICON_NORMAL);
    }

    /*
     * Checking the battery animation from level 0 to level 9
     */
    image->updateBatteryLevel (0);
    image->startCharging(10);
    QVERIFY (image->m_ChargingSpeed == 10);
    QVERIFY (animationIsOngoing(image));
    QVERIFY (image->m_iconCurrentSet == BatteryImage::ICON_CHARGING);

    // rsetting array
    for (int n = 0; n < 10; ++n)
        indexUsed[n] = false;
    // Gathering data
    for (int w = 0; w < 30; ++w) {
        indexUsed[image->m_ImageIndex] = true;
        QTest::qWait (9);
    }
    // Checking the values.
    for (int n = 0; n < 10; ++n) {
        QVERIFY (indexUsed[n]);
    }
    
    /*
     * Checking the cumulative charging indicator. It is a bit tricky.
     */
    for (level = 1; level < 5; ++level) {
        image->updateBatteryLevel (level);
        // Resetting the array
        for (int n = 0; n < 10; ++n)
            indexUsed[n] = false;
        // Gathering data
        for (int w = 0; w < 30; ++w) {
            indexUsed[image->m_ImageIndex] = true;
            QTest::qWait (9);
        }

        // checking the values.
        for (int n = 0; n < 10; ++n) {
            int shouldBeTrue;

            shouldBeTrue = n >= level;
            QVERIFY (indexUsed[n] == shouldBeTrue);
        }
    }

    /*
     * Checking the power save mode, with and without charging. When the
     * charging is ongoing the image will show the charging even in powersave
     * mode.
     */
    image->setPSMValue (true);
    image->updateBatteryLevel (2);
    QVERIFY (animationIsOngoing(image));
    QVERIFY (image->m_iconCurrentSet == BatteryImage::ICON_CHARGING);

    image->stopCharging ();
    QVERIFY (!animationIsOngoing(image));
    QVERIFY (image->m_iconCurrentSet == BatteryImage::ICON_POWERSAVE);
    QVERIFY (image->m_ImageIndex == 2);

    delete image;
}

/******************************************************************************
 * Low level helper functions.
 */
bool 
Ut_BatteryImage::animationIsOngoing (
        BatteryImage *image)
{
    if (!image->m_timer)
        return false;

    return image->m_timer->isActive ();
}

QTEST_APPLESS_MAIN (Ut_BatteryImage);

