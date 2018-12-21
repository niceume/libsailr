#ifndef VM_H
#define VM_H

#include "vm_code.h"
#include "../ptr_table.h"
#include "vm_stack.h"

int vm_exec_code( vm_inst* code , int num_insts , ptr_table* table , vm_stack* vmstack);

#endif /* VM_H */
