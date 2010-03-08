TEMPLATE = app
QT += testlib

CONFIG += dui duistatusindicatormenu silent

TARGET = ft_statusmenuplugins
target.path = /usr/lib/systemui-applets-tests

SOURCES += \
    appletloader.cpp \
    StatusIndicatorMenuStub.cpp

HEADERS += \
    appletloader.h

INSTALLS += target
