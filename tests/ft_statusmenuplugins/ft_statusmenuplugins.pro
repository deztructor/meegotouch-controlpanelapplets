include(../coverage.pri)
include(../check.pri)

TEMPLATE = app
QT += testlib

CONFIG += \
    meegotouch \
    system-ui \
    silent

TARGET = ft_statusmenuplugins
target.path = /usr/lib/systemui-applets-tests

SOURCES += \
    pluginloader.cpp \
    StatusIndicatorMenuStub.cpp

HEADERS += \
    pluginloader.h \
    StatusIndicatorMenuStub.h

INSTALLS += target
