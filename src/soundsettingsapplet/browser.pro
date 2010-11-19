TEMPLATE = app
CONFIG +=           \
    plugin          \
    gui             \
    meegotouch      \
    debug           \
    link_pkgconfig  \
    $$(NULL)

PKGCONFIG += meegotouch profile gstreamer-0.10 ContentManager

INCLUDEPATH += /usr/include/qt4/dui

DEFINES += RUN_STANDALONE

LIBS += -lqttracker

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS =                \
    qtrackedvariant.h \
		qprofilevalue.h \
		alerttoneappletmaps.h \
    alerttonebrowser.h \
		alerttonetoplevel.h \
		alerttone.h \
		alerttonedefaults.h \
		alerttonepreview.h \
		static.h \
    $$(NULL)

SOURCES =                  \
    qtrackedvariant.cpp \
		qprofilevalue.cpp \
		alerttoneappletmaps.cpp \
    alerttonebrowser.cpp \
		alerttonetoplevel.cpp \
		alerttone.cpp \
		alerttonedefaults.cpp \
		alerttonepreview.cpp \
		static.cpp \
    $$(NULL)

INSTALLS += \
    target  \
		$$(NULL)
