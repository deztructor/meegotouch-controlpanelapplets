#
# Translation generation project include file
# CATALOGNAME must be the applet source dir name
#

# To sense whether this is a meego.com based build
CONFIG += duicontrolpanel

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands = $$system(true)
}

isEmpty(CATALOGNAME) {
    warning($$PWD does not set the CATALOGNAME!)
}

contains(DEFINES, MEEGO) {
# hack around the meegotouch_translations.prf, if this isn't empty
# then the generated translations will not be prefixed with '\ !!'
  LRELEASE_OPTIONS = -idbased
}

SOURCEDIR = $$PWD/$$CATALOGNAME
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/meegotouch
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

