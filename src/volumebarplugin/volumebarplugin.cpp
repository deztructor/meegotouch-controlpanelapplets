#include "volumebarplugin.h"
#include "volumebar.h"

#include <MTheme>

#define CSS_DIR "/usr/share/themes/base/m/sysuid/"

Q_EXPORT_PLUGIN2(volumebar, VolumeBarPlugin);

MWidget*
VolumeBarPlugin::constructWidget (
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    MTheme::loadCSS (CSS_DIR "volumebar.css");

    return new VolumeBar (statusIndicatorMenu);
}
