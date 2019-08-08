#!/bin/sh

DEV_DIR="`pwd`"
DEV_ONIGMO_INSTALL="${DEV_DIR}/onigmo_build"
NO_COLOR='\033[0m'
Green='\033[1;32m'

if [ ! -f "${DEV_ONIGMO_INSTALL}/lib/libonigmo.a" ];then
  echo -e "${Green} libonigmo.a does not exist in dev_env/onigmo_build/lib ${NO_COLOR} \n"
  echo -e "${Green} start onigmo build and install ${NO_COLOR} \n"

  cd onigmo_src/; 
  ./autogen.sh; ./configure --prefix=${DEV_ONIGMO_INSTALL}; make; make install;
  cd ..; 

  rm -f ${DEV_ONIGMO_INSTALL}/lib/onigmo.dll
  rm -f ${DEV_ONIGMO_INSTALL}/lib/libonigmo.dll.a
  rm -f ${DEV_ONIGMO_INSTALL}/lib/libonigmo.la
  rm -f ${DEV_ONIGMO_INSTALL}/lib/onigmo.so
else
  echo -e "${Green} libonigmo.a already exists ${NO_COLOR} \n"
fi


