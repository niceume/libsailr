#!/bin/sh

./mingw_64_clean.sh

echo "Copying sailr src into mingw 64bit build directory"
cp ../*.c build/64/
cp ../*.h build/64/
cp ../*.y build/64/
cp ../*.l build/64/
cp -R ../simple_re   build/64/
cp -R ../string      build/64/
cp -R ../simple_date build/64/
cp -R ../vm          build/64/
cp Makefile_mingw_64  build/64/
