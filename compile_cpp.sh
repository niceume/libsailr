# cd ./src/vm/
# ./compile_vm.sh

# cd ..
# cd ./string/
# ./compile_string.sh

# cd ..
# ./compile.sh

# cd ..

make build --directory sailr
g++ -std=c++11 -Wall -o myparsercpp main.cpp -Isailr/ -Isrc/string -lm -Lsailr -lsailr 

