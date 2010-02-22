LANGUAGES =
CATALOGNAME = systemui-applets
SOURCEDIR = $$PWD/../components/displayapplet \
            $$PWD/../components/batteryapplet \
            $$PWD/../components/batteryplugin \
            $$PWD/../components/profileapplet \
            $$PWD/../components/profileplugin \
            $$PWD/../components/usbapplet \
            $$PWD/../components/volumebarplugin
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/dui
include(translations.pri)
