# ####################################################################
# System-UI applets main projectfile
# ####################################################################

TEMPLATE = subdirs
SUBDIRS = \
	src \
	tests \
	translations

QMAKE_CLEAN += \
	configure-stamp \
	build-stamp

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.depends = src/Makefile 
	coverage.commands = \
		cd tests && make coverage && cd .. \
                && genhtml --no-branch-coverage --legend -o coverage/ \
		tests/ut_*/selected.cov
}

