QMAKE_LFLAGS_RPATH = -Wl

#TEST_PKG_NAME = meegotouch-controlpanelapplets-tests
TEST_PKG_NAME = systemui-applets-tests

include(check.pri)
include(coverage.pri)
include(../localconfig.pri)
