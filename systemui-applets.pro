# ####################################################################
# System-UI applets main projectfile
# ####################################################################

TEMPLATE = subdirs
SUBDIRS = src \
          tests \
          translations

include(tests/gentests.pri)

QMAKE_CLEAN += configure-stamp build-stamp
