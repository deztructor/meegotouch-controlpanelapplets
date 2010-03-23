LANGUAGES =
CATALOGNAME = systemui-applets
SOURCEDIR = $$PWD/../src/displayapplet \
            $$PWD/../src/batteryapplet \
            $$PWD/../src/batteryplugin \
            $$PWD/../src/profileapplet \
            $$PWD/../src/profileplugin \
            $$PWD/../src/usbapplet \
            $$PWD/../src/volumebarplugin \
            $$PWD/../src/themeapplet
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/dui
include(translations.pri)
