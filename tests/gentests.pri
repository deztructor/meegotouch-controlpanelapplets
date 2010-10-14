include(common_top.pri)

shell_scripts.commands += tests/gen-tests-xml.sh > tests/tests.xml
shell_scripts.files += tests/tests.xml
shell_scripts.path += /usr/share/$$TEST_PKG_NAME
shell_scripts.CONFIG += no_check_exist

INSTALLS    += shell_scripts
