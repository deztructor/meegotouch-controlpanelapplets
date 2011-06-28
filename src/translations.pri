#
# Translation generation project include file
# CATALOGNAME must be the applet source dir name
#

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands = $$system(true)
}

isEmpty(CATALOGNAME) {
    warning($$PWD does not set the CATALOGNAME!)
}

SOURCEDIR = $$PWD/$$CATALOGNAME
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/meegotouch
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

