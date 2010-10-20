/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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
#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h"

class MButton;
class MLabel;
class MLayout;
class MLinearLayoutPolicy;
class MContainer;
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
    void charging(int animation_rate);
    void chargeComplete ();

private:
    void updatePSMButton ();
    void retranslateUi ();
    MButton                 *m_PSMAutoButton;
    MContainer              *activationLevelLabelContainer;
    BatteryBusinessLogic    *m_logic;
    bool                     m_UILocked;
    BatteryImage            *batteryImage;
    MButton                 *PSMButton;
    MLinearLayoutPolicy     *m_MainLayout;
    // true means we are believed to be in power save mode
    bool                     m_PSMButtonToggle;
    SliderContainer         *sliderContainer;
    PercentageContainer     *remainingCapacityContainer;
};

#endif // BATTERYWIDGET_H
