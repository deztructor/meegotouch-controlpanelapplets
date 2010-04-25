/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>
#include <MTheme>

#define DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        MImageWidget (parent),
        m_timer (NULL),
        m_batteryLevel (0),
        m_iconCurrentSet (ICON_NORMAL)
{
    qRegisterMetaType<BatteryIconType> ();
    setImage ("icon-m-energy-management-battery-low");
}

void
BatteryImage::loadImages (BatteryIconType type)
{
  if (m_timer != 0)
      m_timer->stop (); // to avoid crashes...

  if (m_Images.isEmpty () == false)
  {
    if (m_iconCurrentSet == type)
        return; // Nothing to do...

    // Release the pixmaps
    foreach (const QPixmap *icon, m_Images)
        delete icon;
    m_Images.clear ();

  }

  QString ID = "battery";
  switch (type)
  {
    case ICON_POWERSAVE:
        ID = "powersave";
        break;
    case ICON_CHARGING:
        ID = "charging";
        break;
    default:
        break;
  }

  MTheme *theme = MTheme::instance ();

  m_Images <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1-verylow").arg (ID)) <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1-low").arg (ID)) <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "1") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "2") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "3") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "4") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "5") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "6") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "7") <<
      theme->pixmapCopy (QString ("icon-m-energy-management-%1").arg (ID) + "8");

  if (m_timer != 0)
      m_timer->start (); // to avoid crashes...
}

BatteryImage::~BatteryImage ()
{
    delete m_timer;
    m_timer = NULL;

    if (! m_Images.isEmpty ())
    {
        // Release the pixmaps
        foreach (const QPixmap *icon, m_Images)
            MTheme::instance ()->releasePixmap (icon);
        m_Images.clear ();
    }
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
BatteryImage::setIconSet (BatteryIconType type)
{
    m_iconCurrentSet = type;
}

void
BatteryImage::updateImage (bool charging)
{
    static int imageIndex = m_batteryLevel;

    if (charging)
    {
        imageIndex++;
    }
    else
    {
        imageIndex = m_batteryLevel;
    }

    if (charging)
        m_iconCurrentSet = ICON_CHARGING;
    else if (m_iconCurrentSet == ICON_CHARGING)
        m_iconCurrentSet = ICON_NORMAL;

    loadImages (m_iconCurrentSet);

    // To avoid crashes (when icons aren't available)
    if (m_Images.isEmpty ())
        return;

    // cumulative charging animation
    if (m_Images.size () <= imageIndex)
    {
        imageIndex = m_batteryLevel;
    }

    setPixmap (*(m_Images.at (imageIndex)));
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

    updateImage (false);
}

