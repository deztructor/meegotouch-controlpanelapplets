#ifndef VOLUMEBARLOGIC_H
#define VOLUMEBARLOGIC_H

#include <QObject>
#include <QDBusInterface>

class VolumeBarLogic
{
    Q_OBJECT

    public:
        VolumeBarLogic ();
        ~VolumeBarLogic ();

        void setVolume (int value);
        int  getVolume ();
        int  getMaxVolume ();

    signals:
        void volumeChanged (int value, int maxvalue);

    private:
        QDBusInterface  *m_if;

        int     m_currentvolume;
        int     m_currentmax;
};

#endif
