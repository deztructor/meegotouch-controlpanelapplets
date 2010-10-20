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
#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <MContainer>

class MButton;
class MLabel;
class MSlider;
class MLinearLayoutPolicy; 

class SliderContainer : public MContainer
{
    Q_OBJECT

public:
    SliderContainer (MWidget *parent = 0);
    ~SliderContainer ();
    void retranslate ();

public slots:
    void initSlider (const QStringList &values);
    void updateSlider (const int value);
    void initPSMAutoButton (bool toggle);
//    void PSMAutoButtonToggled (bool toggle);


private slots:
    void sliderValueChanged (int value);

signals:
    void PSMAutoToggled (bool);
    void PSMThresholdValueChanged (int);

private:
    MButton               *m_PSMAutoButton;
    MSlider               *m_PSMSlider;
    MLabel                *m_AutoPSMLabel;
    MLabel                *m_PsmValueLabel;
    QStringList            m_SliderValues;
    int                    m_SliderValue;
    MLinearLayoutPolicy   *m_LayoutPolicy;
    bool                   m_SliderExists;
    
    void    setLayout ();
//    void    toggleSliderExistence (bool toggle);
    void    updateSliderValueLabel ();
};

#endif // SLIDERCONTAINER_H
