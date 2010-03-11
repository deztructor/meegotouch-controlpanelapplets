/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef SLIDERCONTAINER_H
#define SLIDERCONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiLabel;
class DuiSlider;
class DuiLinearLayoutPolicy; 

class SliderContainer : public DuiContainer
{
    Q_OBJECT

public:
    SliderContainer (DuiWidget *parent = 0);
    ~SliderContainer ();
    void retranslate ();

public slots:
    void initSlider (const QStringList &values);
    void updateSlider (const QString &value);
    void initPSMAutoButton (bool toggle);
    void PSMAutoDisabled ();

private slots:
    void PSMAutoButtonToggled (bool toggle);
    void sliderValueChanged (int value);

signals:
    void PSMAutoToggled (bool);
    void PSMThresholdValueChanged (QString);

private:
    DuiButton               *m_PSMAutoButton;
    DuiSlider               *m_PSMSlider;
    DuiLabel                *m_TextLabel;
    QStringList              m_SliderValues;
    int                      m_SliderValue;
    DuiLinearLayoutPolicy   *m_LayoutPolicy;

    void    setLayout();
    void    toggleSliderExistence (bool toggle);
};

#endif // SLIDERCONTAINER_H
