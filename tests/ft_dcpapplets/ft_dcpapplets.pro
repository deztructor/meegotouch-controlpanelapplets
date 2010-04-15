TEMPLATE = app
QT += testlib

CONFIG += m mcontrolpanel silent
LIBS += -lmcontrolpanel

TARGET = ft_dcpapplets
target.path = /usr/lib/systemui-applets-tests

SOURCES += \
    appletloader.cpp

HEADERS += \
    appletloader.h

INSTALLS += target
