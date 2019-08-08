#!/bin/sh

# cd ./src/vm/
# ./compile_vm.sh

# cd ..
# cd ./string/
# ./compile_string.sh

# cd ..
# ./compile.sh

# cd ..


# Before running, make sure that required libraries exist in LD_LIBRARY_PATH
# ALso make them accessible with export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/local/onigmo/lib

# For debugging lexer and parser
# Switch parse.y global variable, int yydebug = 1; /* 0 : no debug, 1: debug */
# yacc -v outputs output.y 

# pass user defined optinos to makefile or compiler. -D option passes the following variable. -DDEBUG means that DEBUG macro variable will be defined when compilation.


NO_COLOR='\033[0m'
LIGHT_RED='\033[1;31m'
Yellow='\033[1;33m'
Green='\033[1;32m'
LIGHT_BLUE='\033[1;34m'
# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37

echo -e "${Green}For debug purpose, pass -DDEBUG to 'make build' and run the executable with -v option.${NO_COLOR}"

g++ -std=c++11 -Wall -o myparsercpp main.cpp -I../ -I../string -I../simple_re -Ionigmo_build/include -Igetopt_port/ -lm -L../ -lsailr -Lonigmo_build/lib -lonigmo




