/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYIMAGE_H
#define BATTERYIMAGE_H

#include <QObject>
#include <MImageWidget>
#include <QMetaType>

class QTimer;
class QStringList;

/*!
 * An animated icon that shows the state of the battery. The icon has free
 * dictinct states:
 * (1) Using the battery.
 * (2) Charging the battery.
 * (3) Power save mode.
 *
 * The state however is controlled by several attributes:
 * (1) Charging boolean
 * (2) PSM enabled boolean
 *
 * And thus we have four combinations. here is a map that this class implements:
 * 
 * Charg.       PSM.       State
 *   n           n         (1) Using the battery.
 *   n           y         (3) Power save mode.
 *   y           n         (2) Charging the battery.
 *   y           y         (2) Charging the battery.
 *
 * This is because indicating the charging is more important than indicating the
 * power save mode, although we could anymate the power save mode icon too.
 */
class BatteryImage : public MImageWidget
{
    Q_OBJECT

    typedef enum {
        ICON_NORMAL = 0,
        ICON_POWERSAVE,
        ICON_CHARGING,
        IconTypeUnset,
    } BatteryIconType;


public:
    BatteryImage (QGraphicsItem *parent = 0);
    ~BatteryImage ();
    bool charging () const;

public slots:
    void updateBatteryLevel (int level);

    void startCharging (int rate);
    void stopCharging ();
    void setPSMValue (bool PSMEnabled);

private slots:
    void updateImage ();

private:
    void  setIconSet ();

    void  stopTimer();
    bool  maybeStartTimer();
    const QPixmap *getPixmap (const QString &name);
    void loadImages (BatteryImage::BatteryIconType type);
    
private:
    bool                    m_PowerSave;
    int                     m_ChargingSpeed;
    QTimer                 *m_timer;
    int                     m_batteryLevel;
    QList<const QPixmap *>  m_Images;
    BatteryIconType         m_iconCurrentSet;
    #ifdef UNIT_TEST
    friend class Ut_BatteryImage;
    #endif
};

#endif
