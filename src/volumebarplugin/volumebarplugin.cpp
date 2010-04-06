#include "volumebarplugin.h"
#include "volumebar.h"

#include <DuiTheme>

#define CSS_DIR "/usr/share/themes/base/dui/sysuid/"

Q_EXPORT_PLUGIN2(volumebar, VolumeBarPlugin);

DuiWidget*
VolumeBarPlugin::constructWidget (
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu)
{
    DuiTheme::loadCSS (CSS_DIR "volumebar.css");

    return new VolumeBar (statusIndicatorMenu);
}
