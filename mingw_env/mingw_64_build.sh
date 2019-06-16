#!/bin/sh

echo "Building in mingw 64bit build directory"

make --directory build/64/ -f Makefile_mingw_64 build ONIG_INCLUDE_DIR=../../onigmo/64/include ONIG_LIB_DIR=../../onigmo/64/lib CC_USER_DEFINES="-DDEBUG"


