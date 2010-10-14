include(../common_top.pri)

TEMPLATE = app
QT += testlib

CONFIG += \
    meegotouch \
    system-ui \
    silent

TARGET = ft_statusmenuplugins
target.path = /usr/lib/$$TEST_PKG_NAME

SOURCES += \
    pluginloader.cpp \
    StatusIndicatorMenuStub.cpp

HEADERS += \
    pluginloader.h \
    StatusIndicatorMenuStub.h

INSTALLS += target
