include(../common_top.pri)

TEMPLATE = app
QT += testlib

CONFIG += meegotouch duicontrolpanel silent

TARGET = ft_applets
target.path = /usr/lib/$$TEST_PKG_NAME

SOURCES += \
    appletloader.cpp

HEADERS += \
    appletloader.h

INSTALLS += target
