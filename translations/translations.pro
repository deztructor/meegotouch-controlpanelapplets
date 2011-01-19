
contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands = $$system(true)
}

LANGUAGES =
CATALOGNAME = meegotouchcp-applets
SOURCEDIR = $$PWD/../src/displayapplet \
            $$PWD/../src/batteryapplet \
            $$PWD/../src/profileapplet \
            $$PWD/../src/profileplugin \
            $$PWD/../src/usbapplet \
            $$PWD/../src/offlineapplet \
            $$PWD/../src/themeapplet \
            $$PWD/../src/wallpaperapplet \
            $$PWD/../src/resetapplet \
            $$PWD/../src/warrantyapplet \ 
            $$PWD/../src/soundsettingsapplet \
            $$PWD/../src/aboutapplet 

TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/meegotouch
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

