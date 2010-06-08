TEMPLATE = subdirs

contains(BUILD_FEATURES,coverage) {
SUBDIRS = $$system(ls -1d ut_*/)
} else {
SUBDIRS = $$system(ls -1d ut_*/ ft_*/)
}

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml **/*.log coverage.out

coverage.target = coverage
coverage.depends = clean
coverage.CONFIG = recursive
QMAKE_EXTRA_TARGETS += coverage

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/systemui-applets-tests
support_files.CONFIG += no_check_exist

INSTALLS += support_files
