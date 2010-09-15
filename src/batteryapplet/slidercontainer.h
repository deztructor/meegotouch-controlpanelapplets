/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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

private slots:
    void PSMAutoButtonToggled (bool toggle);
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
    void    toggleSliderExistence (bool toggle);
    void    updateSliderValueLabel ();
};

#endif // SLIDERCONTAINER_H
