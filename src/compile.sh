g++ -o cpp_string.o -c cpp_string.cpp

yacc -d -v parse.y  # yacc creates y.tab.c. -d option Creates y.tab.h. -v option creates y.output.
gcc -o parse.o -c y.tab.c -DYYDEBUG=1

lex -olex.yy.c --debug  lex.l 
gcc -o lex.o -c lex.yy.c

gcc -o tree_dump.o -c tree_dump.c 
gcc -o tree_free.o -c tree_free.c 
gcc -o node.o -c node.c 
gcc -o struct_string.o -c struct_string.c 
gcc -o parser_state.o -c parser_state.c 
gcc -o ptr_table.o -c ptr_table.c 
gcc -o gen_code.o -c gen_code.c -Ivm
gcc -o gen_code_util.o -c gen_code_util.c -Ivm
gcc -o vm_label.o -c vm_label.c -lm 
gcc -o gen_code_util.o -c gen_code_util.c -Ivm

gcc -o myparser.o -c main.c -Ivm -I./

gcc -g -o myparser myparser.o parse.o lex.o tree_dump.o tree_free.o node.o gen_code.o gen_code_util.o struct_string.o parser_state.o ptr_table.o vm_label.o vm/vm.o vm/vm_assign.o vm/vm_calc.o vm/vm_cmd.o vm/vm_code.o vm/vm_stack.o -lm -Ivm -I..

echo "myparser is created.  Enjoy!! \n"



