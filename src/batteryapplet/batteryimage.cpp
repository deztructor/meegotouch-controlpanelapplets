/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>
#include <QStringList>
#include <MTheme>

//#define DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        MImageWidget (parent),
        m_PowerSave (false),
        m_ChargingSpeed (0),
        m_timer (NULL),
        m_batteryLevel (0),
        m_iconCurrentSet (IconTypeUnset)
{
    setIconSet ();
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
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("level = %d", level);
    m_batteryLevel = level;
    updateImage ();
    SYS_DEBUG ("***************************************************");
}

void
BatteryImage::setIconSet ()
{
    BatteryImage::BatteryIconType type;

    type = ICON_NORMAL;
    if (charging()) {
        type = ICON_CHARGING;
    } else if (m_PowerSave) {
        type = ICON_POWERSAVE;
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

/*!
 * \returns true if the image in charging state
 */
bool 
BatteryImage::charging () const
{
    return m_ChargingSpeed > 0 /*&& !m_PowerSave*/;
}

void
BatteryImage::startCharging (
		int rate)
{
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** rate = %d", rate);
    m_ChargingSpeed = rate;
    setIconSet ();
    maybeStartTimer ();
    SYS_DEBUG ("***************************************************");
}

void
BatteryImage::stopCharging ()
{
    SYS_DEBUG ("***************************************************");
    m_ChargingSpeed = 0;
    setIconSet ();
    maybeStartTimer ();
    SYS_DEBUG ("***************************************************");
}

void
BatteryImage::setPSMValue (
        bool    PSMEnabled)
{
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** PSMEnabled = %s", SYS_BOOL(PSMEnabled));
    m_PowerSave = PSMEnabled;
    setIconSet ();
    maybeStartTimer ();
    SYS_DEBUG ("***************************************************");
}


void
BatteryImage::stopTimer ()
{
    if (m_timer && m_timer->isActive()) {
        SYS_DEBUG ("STOPPING TIMER");
        m_timer->stop();
    }
}

/*!
 * \return true if the animation is ongoing.
 *
 * Start the animation if the state of the image requires it. Updates the icon
 * once if not.
 */
bool
BatteryImage::maybeStartTimer ()
{
    if (!charging()) {
        updateImage ();
        return false;
    }

    if (!m_timer) {
        SYS_DEBUG("Creating timer.");
        m_timer = new QTimer (this);
        connect (m_timer, SIGNAL(timeout()),
                this, SLOT(updateImage()));
    } else {
        stopTimer();
    }

    updateImage ();
    SYS_DEBUG ("STARTING TIMER");
    m_timer->start (m_ChargingSpeed);
    return true;
}

const QPixmap *
BatteryImage::getPixmap (
        const QString &name)
{
    const QPixmap  *pixmap;

    SYS_DEBUG ("*** name   = %s", SYS_STR(name));
    pixmap = MTheme::pixmapCopy (name);
    
    #ifdef DEBUG
    if (pixmap->width() < 48 ||
            pixmap->height() < 48) {
        SYS_WARNING ("Pixmap not loaded? Size: %dx%d", 
                pixmap->width(), pixmap->height());
    }
    #endif
    return pixmap;
}


