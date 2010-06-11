
contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands = $$system(true)
}

LANGUAGES =
CATALOGNAME = systemui-applets
SOURCEDIR = $$PWD/../src/displayapplet \
            $$PWD/../src/batteryapplet \
            $$PWD/../src/profileapplet \
            $$PWD/../src/profileplugin \
            $$PWD/../src/usbapplet \
            $$PWD/../src/themeapplet \
            $$PWD/../src/wallpaperapplet \
            $$PWD/../src/resetapplet \
            $$PWD/../src/warrantyapplet \ 
            $$PWD/../src/aboutapplet 

TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/meegotouch
include(translations.pri)
