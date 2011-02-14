QMAKE_LFLAGS_RPATH += -Wl
QMAKE_LFLAGS += -Wl,--as-needed

include(common.pri)
include(check.pri)
