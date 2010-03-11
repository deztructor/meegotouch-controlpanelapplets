#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <DuiWidget>

class DuiSlider;
class DuiContainer;
class VolumeBarLogic;
class DuiStatusIndicatorMenuInterface;

class VolumeBar : public DuiWidget
{
    Q_OBJECT

public:
    VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
               QGraphicsItem *parent = 0);
    ~VolumeBar ();

private slots:
    void sliderChanged (int val);
    void volumeChanged (int val, int max);

private:
    DuiSlider       *m_bar;
    VolumeBarLogic  *m_logic;
};

#endif
