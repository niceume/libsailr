#!/bin/sh

echo "Cleaning mingw 64bit build directory"

make --directory build/64/ -f Makefile_mingw_64  clean
