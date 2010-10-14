TEMPLATE = subdirs

include(common_top.pri)
#
# The coverage target will not build the functional tests.
#
contains(BUILD_FEATURES,coverage) {
	#
	# Sometimes it is good to disable all but one unit tests.
	#
        #SUBDIRS =  ut_themelistmodel
        #SUBDIRS =  $$system(ls -1d ut_wallpaper*/)
        SUBDIRS = $$system(./unit_tests.sh)
} else {
	SUBDIRS = $$system(ls -1d ut_*/ ft_*/)
}

# nocheck option disables the test building
contains(BUILD_FEATURES,nocheck) {
    message("*** Disabling unit/functional test building ***")
    SUBDIRS =
}

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_STRIP = echo
QMAKE_CLEAN += **/*.log.xml **/*.log 

contains(BUILD_FEATURES,coverage) {
  QMAKE_EXTRA_TARGETS += coverage
  coverage.target = coverage
  coverage.CONFIG = recursive
}

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/$$TEST_PKG_NAME
support_files.CONFIG += no_check_exist

dummy_file.commands += touch $$OUT_PWD/keep_this_dir
dummy_file.target = dummy_file
dummy_file.files += $$OUT_PWD/keep_this_dir
dummy_file.path = /usr/lib/$$TEST_PKG_NAME

testscript.files = cylontest.sh
testscript.path = /bin

INSTALLS += \
    support_files \
    dummy_file \
    testscript

