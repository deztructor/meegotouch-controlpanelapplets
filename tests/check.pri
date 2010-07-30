QMAKE_STRIP = echo

CONFIG += link_prl
QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = @LD_LIBRARY_PATH=../../lib:$$(LD_LIBRARY_PATH) ./$$TARGET

