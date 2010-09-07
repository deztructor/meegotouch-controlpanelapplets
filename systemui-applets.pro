# ####################################################################
# System-UI applets main projectfile
# ####################################################################

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
                   -t \"SystemUI Applets Coverage Report\" \
                tests/ut_*/selected.cov
}

