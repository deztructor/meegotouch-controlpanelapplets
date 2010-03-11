/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>

#define DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        DuiImageWidget (parent),
        timer (NULL),
        batteryLevel (0)
{
    /*
     * We have to show something even if we get no signals from DBus. FIXME:
     * maybe this is not the right image, but it is the only one works now.
     */
    setImage ("icon-m-energy-management-battery-62");

    batteryImages << 
        QString ("icon-m-energy-management-battery-verylow") << 
        QString ("icon-m-energy-management-battery-low") << 
        QString ("icon-m-energy-management-battery13") << 
        QString ("icon-m-energy-management-battery25") << 
        QString ("icon-m-energy-management-battery38") << 
        QString ("icon-m-energy-management-battery50") << 
        QString ("icon-m-energy-management-battery62") << 
        QString ("icon-m-energy-management-battery75") << 
        QString ("icon-m-energy-management-battery88") << 
        QString ("icon-m-energy-management-battery100");

    batteryChargingImages << 
        QString ("icon-s-battery-verylow") <<
        QString ("icon-s-battery-low") <<
        QString ("icon-s-battery13") << 
        QString ("icon-s-battery25") << 
        QString ("icon-s-battery38") << 
        QString ("icon-s-battery50") << 
        QString ("icon-s-battery62") << 
        QString ("icon-s-battery75") << 
        QString ("icon-s-battery88") << 
        QString ("icon-s-battery100");
}

BatteryImage::~BatteryImage ()
{
    delete timer;
    timer = NULL;
}

void 
BatteryImage::updateBatteryLevel (int level)
{
    SYS_DEBUG ("level = %d", level);

    batteryLevel = level;
    if (timer == NULL)
        updateImage (false);
}

void 
BatteryImage::updateImage (bool charging)
{
#if 0
    static int chargingImageIndex = batteryLevel;
    if (charging) {
        if (chargingImageIndex >= batteryChargingImages.size ())
            chargingImageIndex = (batteryLevel > 1 ? batteryLevel : 2);
        SYS_DEBUG ("Charging index: %d", chargingImageIndex);
        setImage (batteryChargingImages.at (chargingImageIndex++));
    } else {
        SYS_DEBUG ("Normal index: %d", batteryLevel);
        setImage (batteryImages.at (batteryLevel));
        chargingImageIndex = batteryLevel;
    }
#else
    SYS_DEBUG ("charging = %s", charging ? "true" : "false");

    static int imageIndex = batteryLevel;

    if (charging)
    {
        imageIndex++;
    }
    else
    {
        imageIndex = batteryLevel;
    }

    if (batteryImages.size () <= imageIndex)
       imageIndex = 0;

    setImage (batteryImages.at (imageIndex)); 
#endif
}

void
BatteryImage::startCharging (
		int rate)
{
    if (rate < 0) //USB 100mA
        return;

    if (timer == NULL) {
        timer = new QTimer (this);
        connect (timer, SIGNAL (timeout ()),
                 this, SLOT (updateImage ()));
    }

    timer->setInterval (rate);
    timer->start ();
}

void
BatteryImage::stopCharging ()
{
    if (timer != NULL) {
        timer->stop ();
        delete timer;
        timer = NULL;
    }
}

