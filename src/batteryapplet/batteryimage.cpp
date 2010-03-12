/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>
#include <DuiTheme>

#undef DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        DuiImageWidget (parent),
        m_timer (NULL),
        m_batteryLevel (0)
{
    /*
     * We have to show something even if we get no signals from DBus. FIXME:
     * maybe this is not the right image, but it is the only one works now.
     */
    setImage ("icon-m-energy-management-battery-62");
}

void
BatteryImage::loadImages (bool charging)
{
#if 0
  // These icons are invisible :-S

  if (charging && m_ChargingImages.isEmpty ())
  {
    DuiTheme *theme = DuiTheme::instance ();

    m_ChargingImages << 
        theme->pixmap (QString ("icon-s-status-battery-verylow")) <<
        theme->pixmap (QString ("icon-s-status-battery-low")) <<
        theme->pixmap (QString ("icon-s-status-battery13")) << 
        theme->pixmap (QString ("icon-s-status-battery25")) << 
        theme->pixmap (QString ("icon-s-status-battery38")) << 
        theme->pixmap (QString ("icon-s-status-battery50")) << 
        theme->pixmap (QString ("icon-s-status-battery62")) << 
        theme->pixmap (QString ("icon-s-status-battery75")) << 
        theme->pixmap (QString ("icon-s-status-battery88")) << 
        theme->pixmap (QString ("icon-s-status-battery100"));
  }
  else
#else
  Q_UNUSED(charging);
#endif
  if (m_Images.isEmpty ())
  {
    DuiTheme *theme = DuiTheme::instance ();

    m_Images << 
        theme->pixmap (QString ("icon-m-energy-management-battery-verylow")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery-low")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery13")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery25")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery38")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery50")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery62")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery75")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery88")) << 
        theme->pixmap (QString ("icon-m-energy-management-battery100"));
  }
}

BatteryImage::~BatteryImage ()
{
    delete m_timer;
    m_timer = NULL;

    if (! m_Images.isEmpty ())
    {
        // Release the pixmaps
        foreach (const QPixmap *icon, m_Images)
            DuiTheme::instance ()->releasePixmap (icon);
        m_Images.clear ();
    }
#if 0
    if (! m_ChargingImages.isEmpty ())
    {   // Release the pixmaps
        foreach (const QPixmap *icon, m_ChargingImages)
            DuiTheme::instance ()->releasePixmap (icon);
        m_ChargingImages.clear ();
    }
#endif
}

void 
BatteryImage::updateBatteryLevel (int level)
{
    SYS_DEBUG ("level = %d", level);

    m_batteryLevel = level;
    if (m_timer == NULL)
        updateImage (false);
}

void 
BatteryImage::updateImage (bool charging)
{
    SYS_DEBUG ("charging = %s", charging ? "true" : "false");

    static int imageIndex = m_batteryLevel;

    if (charging)
    {
        imageIndex++;
    }
    else
    {
        imageIndex = m_batteryLevel;
    }

    loadImages (charging);

    // To avoid crashes (when icons aren't available)
    if (m_Images.isEmpty ())
        return;

#if 0
    int imageCount = charging ?
                     m_ChargingImages.size () :
                     m_Images.size ();

    if (imageCount <= imageIndex)
       imageIndex = 0;

    if (charging)
        setPixmap (*(m_ChargingImages.at (imageIndex)));
    else
        setPixmap (*(m_Images.at (imageIndex)));
#else
    if (m_Images.size () <= imageIndex)
        imageIndex = 0;

    setPixmap (*(m_Images.at (imageIndex)));
#endif
}

void
BatteryImage::startCharging (
		int rate)
{
    if (rate < 0) //USB 100mA
        return;

    if (m_timer == NULL) {
        m_timer = new QTimer (this);
        connect (m_timer, SIGNAL (timeout ()),
                 this, SLOT (updateImage ()));
    }

    m_timer->start (rate);
}

void
BatteryImage::stopCharging ()
{
    if (m_timer != NULL) {
        m_timer->stop ();
        delete m_timer;
        m_timer = NULL;
    }
}

