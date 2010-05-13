/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>
#include <QStringList>
#include <MTheme>

#define DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        MImageWidget (parent),
        m_PowerSave (false),
        m_ChargingSpeed (0),
        m_timer (NULL),
        m_batteryLevel (0),
        m_iconCurrentSet (ICON_NORMAL)
{
    setImage ("icon-m-energy-management-battery-low");
}

void
BatteryImage::loadImages (
        BatteryImage::BatteryIconType type)
{
    if (!m_Images.isEmpty ()) {
        // Release the pixmaps
        foreach (const QPixmap *icon, m_Images)
            delete icon;
        
        m_Images.clear ();
    }

    QString ID = "battery";
    switch (type) {
        case ICON_POWERSAVE:
            ID = "powersave";
            break;

        case ICON_CHARGING:
            ID = "charging";
            break;

        default:
            break;
    }

  m_Images <<
      getPixmap (QString ("icon-m-energy-management-%1-verylow").arg (ID)) <<
      getPixmap (QString ("icon-m-energy-management-%1-low").arg (ID)) <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "1") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "2") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "3") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "4") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "5") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "6") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "7") <<
      getPixmap (QString ("icon-m-energy-management-%1").arg (ID) + "8");
    
    m_iconCurrentSet = type;
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

    if (!charging())
        updateImage ();
}

void
BatteryImage::setIconSet ()
{
    BatteryImage::BatteryIconType type;

    type = ICON_NORMAL;
    if (m_PowerSave) {
        type = ICON_POWERSAVE;
    } else if (charging()) {
        type = ICON_CHARGING;
    }

    if (m_iconCurrentSet != type) {
        SYS_DEBUG ("*** new type = %d", type);
        stopTimer ();
        loadImages (type);
        maybeStartTimer ();
    }
}

void
BatteryImage::updateImage ()
{
    static int imageIndex = m_batteryLevel;

    //SYS_DEBUG ("*** m_batteryLevel = %d", m_batteryLevel);
    /*
     * When the battery is charing we animate when it is not we simply show the
     * battery level.
     */
    if (charging()) {
        imageIndex++;
        // Cumulative charging indicator
        if (imageIndex >= m_Images.size ())
            imageIndex = m_batteryLevel;
    } else {
        imageIndex = m_batteryLevel;
    }

    //SYS_DEBUG ("*** image at   = %p", m_Images[imageIndex]);
    /*
     * A last check...
     */
    if (imageIndex < 0 || imageIndex >= m_Images.size()) {
        SYS_WARNING ("Wrong index %d, should be between 0 and %d",
                imageIndex, m_Images.size());
        return;
    }

    SYS_DEBUG ("*** imageIndex = %d", imageIndex);
    setPixmap (*(m_Images.at (imageIndex)));

    // When the battery is almost loaded we can not animate, so I hacked
    // this here.
    if (charging() && 
            imageIndex + 1 == m_Images.size () && 
            m_batteryLevel + 1 == m_Images.size ())
        imageIndex = 7;
}

bool 
BatteryImage::charging () const
{
    return m_ChargingSpeed > 0 && !m_PowerSave;
}

void
BatteryImage::startCharging (
		int rate)
{
    SYS_DEBUG ("*** rate = %d", rate);
    m_ChargingSpeed = rate;
    setIconSet ();
    maybeStartTimer ();
}

void
BatteryImage::stopCharging ()
{
    m_ChargingSpeed = 0;
    setIconSet ();
    updateImage ();
}

void
BatteryImage::setPSMValue (
        bool    PSMEnabled)
{
    SYS_DEBUG ("*** PSMEnabled = %s", SYS_BOOL(PSMEnabled));
    m_PowerSave = PSMEnabled;
    setIconSet ();
}


void
BatteryImage::stopTimer ()
{
    if (m_timer && m_timer->isActive()) {
        SYS_DEBUG ("Stopping timer");
        m_timer->stop();
    }
}

void
BatteryImage::maybeStartTimer ()
{
    if (m_ChargingSpeed <= 0) {
        SYS_WARNING ("Charging speed not set.");
        return;
    }

    if (!m_timer) {
        SYS_DEBUG("Creating timer.");
        m_timer = new QTimer (this);
        connect (m_timer, SIGNAL(timeout()),
                this, SLOT(updateImage()));
    } else {
        stopTimer();
    }

    m_timer->start (m_ChargingSpeed);
}

const QPixmap *
BatteryImage::getPixmap (
        const QString &name)
{
    const QPixmap  *pixmap;

    SYS_DEBUG ("*** name   = %s", SYS_STR(name));
    pixmap = MTheme::pixmapCopy (name);

    SYS_DEBUG ("*** pixmap = %d, %d", pixmap->width(), pixmap->height());
    return pixmap;
}


