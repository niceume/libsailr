# cd ./src/vm/
# ./compile_vm.sh

# cd ..
# cd ./string/
# ./compile_string.sh

# cd ..
# ./compile.sh

# cd ..

make build --directory ../
g++ -std=c++11 -Wall -o myparsercpp main.cpp -I../ -I../string -lm -L../ -lsailr

