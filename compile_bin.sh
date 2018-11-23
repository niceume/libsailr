cd ./sailr/vm/
./compile_vm.sh

cd ..
cd ./string/
./compile_string.sh

cd ..
./compile.sh

cd ..

gcc -Wall -o myparser.o -c main.c -Ivm -Isailr/ -Isailr/vm  -Isailr/string

echo "myparser.o is created"

gcc -g -o myparser myparser.o sailr/parse.o sailr/lex.o sailr/tree_dump.o sailr/tree_free.o sailr/node.o sailr/gen_code.o sailr/gen_code_util.o sailr/parser_state.o sailr/ptr_table.o sailr/vm_label.o  sailr/string/common_string.o  sailr/string/struct_string.o sailr/string/cpp_string.o sailr/vm/vm.o sailr/vm/vm_assign.o sailr/vm/vm_calc.o sailr/vm/vm_cmd.o sailr/vm/vm_code.o sailr/vm/vm_stack.o -lm -lstdc++

