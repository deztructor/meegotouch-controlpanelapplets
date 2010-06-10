#! /bin/sh

LCOV=`which lcov`

if test "x$LCOV" = x; then
    echo "You need to install lcov to get actual reports!";
    echo "See http://ltp.sf.net/coverage/lcov.php";
    echo "!!! ATTENTION !!!"
    echo "The current lcov-1.8 contains some silly bug.";
    echo "Please, use a newer one or the cvs version."
    exit 1;
fi

if test "x$SBOX_USE_CCACHE" = xyes; then
	export SBOX_USE_CCACHE=no
fi

if [ -f Makefile ]; then
    make distclean
fi

rm -fr ./coverage/*

qmake DCP_BUILD_FEATURES=coverage
make clean
make coverage
ls -lh 
./coverage/index.html

