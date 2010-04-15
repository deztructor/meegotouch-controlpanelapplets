#ifndef _VOLUMEOVERLAY_H
#define _VOLUMEOVERLAY_H

#include <MOverlay>
#include <QObject>

class MSlider;
class QTimer;

class VolumeOverlay : public MOverlay
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
    MSlider       *m_slider;

    void constructUi ();
};

#endif