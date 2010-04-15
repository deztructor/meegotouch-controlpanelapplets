#include "batteryplugin.h"
#include "battery.h"

Q_EXPORT_PLUGIN2(battery, BatteryPlugin);

MWidget*
BatteryPlugin::constructWidget (
    MStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    return new Battery (statusIndicatorMenu);
}
