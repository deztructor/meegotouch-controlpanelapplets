shell_scripts.commands += tests/gen-tests-xml.sh > tests/tests.xml
shell_scripts.files += tests/tests.xml
shell_scripts.path += /usr/share/systemui-applets-tests
shell_scripts.CONFIG += no_check_exist

INSTALLS    += shell_scripts
