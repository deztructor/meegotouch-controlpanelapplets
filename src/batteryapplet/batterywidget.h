/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h"

class MButton;
class MLabel;
class MLayout;
class BatteryBusinessLogic;
class BatteryImage;
class SliderContainer;
class PercentageContainer;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT

public:
    BatteryWidget (QGraphicsWidget *parent = 0);
    ~BatteryWidget ();
    bool back ();

protected:
    void initWidget();

private slots:
    void PSMButtonReleased();
    void remainingBatteryCapacityReceived (const int value);
    void PSMValueReceived (bool PSMEnabled);
    void PSMAutoToggled (bool PSMAutoEnabled);
    void chargindReceived(int animation_rate);

private:
    void updatePSMButton ();
    void retranslateUi ();

    BatteryBusinessLogic    *m_logic;
    bool                     m_UILocked;
    BatteryImage            *batteryImage;
    MButton                 *PSMButton;
    MLayout                 *m_MainLayout;
    // true means we are believed to be in power save mode
    bool                     m_PSMButtonToggle;
    SliderContainer         *sliderContainer;
    PercentageContainer     *remainingCapacityContainer;
};

#endif // BATTERYWIDGET_H
