TEMPLATE = app
TARGET = page_test
CONFIG += dui # Requires libdui to be installed
QMAKE_CXXFLAGS += -g

# Input
SOURCES += main.cpp
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
HEADERS += main.h
