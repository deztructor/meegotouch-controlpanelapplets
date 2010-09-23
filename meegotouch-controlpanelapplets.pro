# ##########################################################################
# MeeGo Touch Controlpanel applets (ex. system-ui-applets) main projectfile
# ##########################################################################

system(./configure)

TEMPLATE = subdirs
SUBDIRS = \
	src \
	tests \
	translations

QMAKE_CLEAN += \
	configure-stamp \
	build-stamp \
	localconfig.pri

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.depends = src/Makefile
	coverage.commands = \
                cd tests && make coverage && cd .. \
                && genhtml --no-branch-coverage --legend -o coverage/ \
                   -t \"MeeGo Touch Controlpanel Applets Coverage Report\" \
                tests/ut_*/selected.cov
}

# DOXYGEN_BIN=$$findFile(doxygen)
DOXYGEN_BIN=doxygen

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH"
} else {
    doc.commands = @$${DOXYGEN_BIN} Doxyfile ;
}
doc.files += $$OUT_PWD/html/*
doc.path += /usr/share/libmeegocontrol-doc
doc.CONFIG += no_check_exist
doc.depends = FORCE

INSTALLS += \
    doc

