
contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands = $$system(true)
}

LANGUAGES =
CATALOGNAME = meegotouchcp-applets
SOURCEDIR = $$PWD/../src/displayapplet \
            $$PWD/../src/batteryapplet \
            $$PWD/../src/profileapplet \
            $$PWD/../src/usbapplet \
            $$PWD/../src/offlineapplet \
            $$PWD/../src/themeapplet \
            $$PWD/../src/wallpaperapplet \
            $$PWD/../src/resetapplet \
            $$PWD/../src/warrantyapplet \ 
            $$PWD/../src/soundsettingsapplet \
            $$PWD/../src/aboutapplet 

source.files=meegotouchcp-applets.ts
source.depends=meegotouchcp-applets.ts
source.path=/usr/share/doc/systemui-applets-l10n-engineering-english/
source.CONFIG+=no_check_exist
QMAKE_EXTRA_TARGET += source

PRE_TARGETDEPS += $${source.files}
INSTALLS += source

TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/meegotouch
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

