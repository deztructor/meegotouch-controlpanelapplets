TEMPLATE = subdirs
SUBDIRS = \
     ut_template

QMAKE_STRIP = echo
#include(shell.pri)
#include(runtests.pri)

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/systemui-applets-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
