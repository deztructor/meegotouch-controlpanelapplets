#ifndef _VOLUMEOVERLAY_H
#define _VOLUMEOVERLAY_H

#include <DuiOverlay>
#include <QObject>
#include <QTimer>

class VolumeOverlay : public DuiOverlay
{
  Q_OBJECT

  public:
    VolumeOverlay (QGraphicsItem *parent = 0);
    ~VolumeOverlay ();
#if 0
    virtual QSizeF sizeHint (Qt::SizeHint which,
                             const QSizeF &constraint = QSizeF ()) const;
#endif

  public slots:
    void UpdateVolume (int percentage);

  private slots:
    void hideMe ();

  private:
    QTimer      *m_timer;

};

#endif
