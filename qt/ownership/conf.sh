#!/bin/sh
if [ -e Makefile ]; then
    make distclean
fi
qmake "CONFIG+=debug_and_release" test.pro
