#ifndef VOLUMEBARPLUGIN_H
#define VOLUMEBARPLUGIN_H

#include <MStatusIndicatorMenuPluginInterface>
#include <QObject>

class VolumeBarPlugin : public QObject, public MStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuPluginInterface)

public:
    MWidget *constructWidget (
        DuiStatusIndicatorMenuInterface &statusIndicatorMenu);
};


#endif

