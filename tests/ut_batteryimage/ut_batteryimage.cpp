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
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
    }
    
    image->loadImages (BatteryImage::ICON_POWERSAVE);
    QVERIFY (image->m_Images.size() == 10);
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
    }
    
    image->loadImages (BatteryImage::ICON_CHARGING);
    QVERIFY (image->m_Images.size() == 10);
    for (int n = 0; n < 10; ++n) {
        const QPixmap  *pixmap;
        pixmap = image->m_Images[n];
        SYS_DEBUG ("image[%d] size is %d, %d", n, 
                pixmap->width(), pixmap->height());
    }

    delete image;
}

QTEST_APPLESS_MAIN (Ut_BatteryImage);

