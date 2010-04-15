/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <MContainer>

class MLabel;
class MButton;
class MSlider;
class MImageWidget;

/*!
 * A widget that shows and controls the basic properties of a profile: the mute
 * state, the volume level and the vibration on/off property.
 */
class ProfileContainer: public MContainer
{
    Q_OBJECT

public:
    ProfileContainer(
            int            id, 
            const QString &title, 
            int            level, 
            bool           vibra, 
            MWidget     *parent = 0);
    ~ProfileContainer ();

    void setLevel (int value);
    void setVibration (bool enabled);
    int id ();
    void retranslateUi ();

signals:
    void sliderValueChanged (int newValue);
    void vibrationChanged (bool enabled);

private slots:
    void slotSliderValueChanged (int newValue); 
    void slotVibrationSwitchToggled (bool toggle);
    
private:
    void setLayout();

private:
    MSlider         *m_Slider;
    MButton         *m_VibrationSwitch;
    MLabel          *m_VibrationLabel;
    MLabel          *m_OnOffLabel;
    MLabel          *m_NoRingLabel;
    MImageWidget    *m_Img;
    int                m_Level;
    const int          m_ProfileId;
};

#endif
