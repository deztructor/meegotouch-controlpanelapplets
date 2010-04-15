#ifndef BATTERYPLUGIN_H
#define BATTERYPLUGIN_H

#include <MStatusIndicatorMenuPluginInterface>
#include <QObject>

class BatteryPlugin : public QObject, public MStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuPluginInterface)

public:
    // Methods derived from MStatusIndicatorMenuPlugin
    virtual MWidget *constructWidget (
        MStatusIndicatorMenuInterface &statusIndicatorMenu);
};

#endif // BATTERYPLUGIN_H
