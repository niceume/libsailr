gcc -o vm_assign.o -c vm_assign.c -I../
echo "vm_assign.o is created. \n"

gcc -o vm_calc.o -c vm_calc.c -lm -I../
echo "vm_calc.o is created. \n"

gcc -o vm_cmd.o -c vm_cmd.c 
echo "vm_code.o is created. \n"

gcc -o vm_code.o -c vm_code.c 
echo "vm_code.o is created. \n"

gcc -o vm_stack.o -c vm_stack.c -I../
echo "vm_stack.o is created. \n"

gcc -o vm.o -c vm.c -I../
echo "vm.o is created. \n"
# gcc -o vm.exe vm.c vm_assign.o vm_calc.o vm_code.o vm_stack.o vm_cmd.o ../ptr_table.o ../struct_string.o -I../
# echo "VM is created.\n"
