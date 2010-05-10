include(../coverage.pri)
TEMPLATE = app
QT += testlib

CONFIG += meegotouch duicontrolpanel silent

TARGET = ft_applets
target.path = /usr/lib/systemui-applets-tests

SOURCES += \
    appletloader.cpp

HEADERS += \
    appletloader.h

INSTALLS += target
