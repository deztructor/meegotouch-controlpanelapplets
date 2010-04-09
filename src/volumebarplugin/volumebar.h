#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <DuiWidget>

class DuiSlider;
class DuiContainer;
class VolumeBarLogic;
class VolumeOverlay;
class DuiStatusIndicatorMenuInterface;

// For Hw-volume key handling
#include <qmkeys.h>
using namespace Maemo;

class VolumeBar : public DuiWidget
{
    Q_OBJECT

public:
    VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
               QGraphicsItem *parent = 0);
    ~VolumeBar ();

private slots:
    void sliderChanged (int val);
    void overlayChanged (int val);
    void volumeChanged (quint32 val, quint32 max);
    void hwKeyEvent (QmKeys::Key key, QmKeys::State state);

private:
    DuiSlider       *m_bar;
    VolumeBarLogic  *m_logic;
    VolumeOverlay   *m_overlay;
    Maemo::QmKeys   *m_hwkeys;
};

#endif
