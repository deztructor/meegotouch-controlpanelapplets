#ifndef _VOLUMEOVERLAY_H
#define _VOLUMEOVERLAY_H

#include <DuiOverlay>
#include <QObject>

class DuiSlider;
class QTimer;

class VolumeOverlay : public DuiOverlay
{
  Q_OBJECT

  public:
    VolumeOverlay (QGraphicsItem *parent = 0);
    ~VolumeOverlay ();

  signals:
    void VolumeChanged (int val);

  public slots:
    void UpdateVolume (int val, int max);

  private slots:
    void updateSliderIcon (int val);
    void hideMe ();

  private:
    QTimer          *m_timer;
    DuiSlider       *m_slider;

    void constructUi ();
};

#endif
