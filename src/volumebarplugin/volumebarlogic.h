#ifndef VOLUMEBARLOGIC_H
#define VOLUMEBARLOGIC_H

#include <dbus/dbus.h>
#include <dbusconnectioneventloop.h>
#include <QObject>

class VolumeBarLogic : public QObject
{
    Q_OBJECT

    public:
        VolumeBarLogic ();
        ~VolumeBarLogic ();

        void setVolume (quint32 value);
        quint32 getVolume ();
        quint32 getMaxVolume ();

    public slots:
        void stepsUpdated (quint32 value, quint32 maxvalue);

    signals:
        void volumeChanged (quint32 value, quint32 maxvalue);

    private:
        void initValues ();
        void addSignalMatch ();

        DBusConnection          *m_dbus_conn;
        DBUSConnectionEventLoop *m_eventloop;

        quint32     m_currentvolume;
        quint32     m_currentmax;
};

#endif

