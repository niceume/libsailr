# cd ./src/vm/
# ./compile_vm.sh

# cd ..
# cd ./string/
# ./compile_string.sh

# cd ..
# ./compile.sh

# cd ..


make clean --directory ../  ONIG_INCLUDE_DIR=dev_env/onig/include
make build --directory ../  ONIG_INCLUDE_DIR=dev_env/onig/include 

g++ -std=c++11 -Wall -o myparsercpp main.cpp -I../ -I../string -I../simple_re -Ionig/include -lm -L../ -lsailr -Lonigmo/lib -lonigmo

