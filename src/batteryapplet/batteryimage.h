/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYIMAGE_H
#define BATTERYIMAGE_H

#include <QObject>
#include <MImageWidget>
#include <QMetaType>

class QTimer;
class QStringList;

class BatteryImage : public MImageWidget
{
    Q_OBJECT

    typedef enum {
        ICON_NORMAL = 0,
        ICON_POWERSAVE,
        ICON_CHARGING
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
    void  maybeStartTimer();
    const QPixmap *getPixmap (const QString &name);
    void loadImages (BatteryImage::BatteryIconType type);
    
private:
    bool                    m_PowerSave;
    int                     m_ChargingSpeed;
    QTimer                 *m_timer;
    int                     m_batteryLevel;
    QList<const QPixmap *>  m_Images;

    BatteryIconType         m_iconCurrentSet;
};

#endif // BATTERYIMAGE_H
