# ##########################################################################
# MeeGo Touch Controlpanel applets (ex. system-ui-applets) main projectfile
# ##########################################################################

system(./configure)
include(shared.pri)
TEMPLATE = subdirs
addSubDirs(src) 
addSubDirs(tests)
addSubDirs(translations)

CONFIG(docs) {
    addSubDirs(doc)
}

QMAKE_CLEAN += \
	configure-stamp \
	build-stamp
# this causes slowdown, and too much warnings:
#	localconfig.pri

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.depends = src/Makefile
	coverage.commands = \
                cd tests && make coverage && cd .. \
                && genhtml --no-branch-coverage --legend -o coverage/ \
                   -t \"MeeGo Touch Controlpanel Applets Coverage Report\" \
                tests/ut_*/selected.cov
}
