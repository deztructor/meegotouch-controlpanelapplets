include(../coverage.pri)
include(../../localconfig.pri)
include(../../shared.pri)

TEMPLATE = lib

include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)

CONFIG +=           \
    plugin          \
    gui             \
    meegotouchcore  \
    link_pkgconfig  \
    duicontrolpanel \
    mobility

MOBILITY +=         \
    systeminfo

PKGCONFIG += dbus-1 profile gconf-2.0 gstreamer-0.10

contains(DEFINES, HAVE_LIBRESOURCEQT) {
    PKGCONFIG += libresourceqt1
}

contains(DEFINES, HAVE_QTSPARQL) {
    CONFIG += qtsparql-tracker-live
}

contains(DEFINES, HAVE_CONTENT_MANAGER) {
    CONFIG += contentmanager
}

QT += dbus

contains(cov, true) { 
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS =                      \
    ../debug.h                 \
    drilldownitem.h            \
    trackerconnection.h        \
    profilebackend.h           \
    profiledatainterface.h     \
    alerttone.h                \
    alerttoneappletmaps.h      \
    profilecontainer.h         \
    alerttoneappletwidget.h    \
    alerttonebrowser.h         \
    alerttonedefaults.h        \
    alerttonedefaultsmodel.h   \
    alerttonepreview.h         \
    alerttonetoplevel.h        \
    alerttonevolume.h          \
    alerttonewidget.h          \
    gconfstringcombo.h         \
    profileintcombo.h          \
    qgconfdirmanager.h         \
    qgconfvalue.h              \
    qprofilevalue.h            \
    qtrackedvariant.h          \
    soundsettingsapplet.h      \
    static.h                   \
    $$(NULL)

SOURCES =                      \
    ../debug.cpp               \
    drilldownitem.cpp          \
    trackerconnection.cpp      \
    profilebackend.cpp         \
    profiledatainterface.cpp   \
    alerttone.cpp              \
    alerttoneappletmaps.cpp    \
    profilecontainer.cpp       \
    alerttoneappletwidget.cpp  \
    alerttonebrowser.cpp       \
    alerttonedefaults.cpp      \
    alerttonedefaultsmodel.cpp \
    alerttonepreview.cpp       \
    alerttonetoplevel.cpp      \
    alerttonevolume.cpp        \
    alerttonewidget.cpp        \
    gconfstringcombo.cpp       \
    profileintcombo.cpp        \
    qgconfdirmanager.cpp       \
    qgconfvalue.cpp            \
    qprofilevalue.cpp          \
    qtrackedvariant.cpp        \
    soundsettingsapplet.cpp    \
    $$(NULL)

DESTDIR = lib

css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/libsoundsettingsapplet/style
css.files = libsoundsettingsapplet.css

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/$$DCP_DESKTOP_DIR

target.path += $$(DEBIAN_DESTDIR)/$$DCP_APPLET_DIR

INSTALLS += \
    target  \
    desktop \
    css

