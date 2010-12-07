include(../common_top.pri)

TEMPLATE = app
QT += testlib
SRCDIR = ../../src
INCLUDEPATH += $$SRCDIR

CONFIG += \
    meegotouch \
    duicontrolpanel \
    silent

TARGET = ft_applets
target.path = /usr/lib/$$TEST_PKG_NAME

include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)
# ^ this defines APPLET_LIBS

SOURCES += \
    $$SRCDIR/debug.cpp \
    ft_applets.cpp

HEADERS += \
    $$SRCDIR/debug.h \
    ft_applets.h

INSTALLS += target
