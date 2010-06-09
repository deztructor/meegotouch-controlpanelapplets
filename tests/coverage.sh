#! /bin/bash

make clean
qmake BUILD_FEATURES=coverage
make

lcov --zerocounters --directory /scratchbox/ccache/
lcov --capture --directory /scratchbox/ccache/ --output-file trace_args.info --test-name mytest 

#--ignore-errors source
