#include <stdio.h>
#include <stdbool.h>
#include "ptr_table.h"

// Using information of vm_code
// VM does
// 1. Stack manipulation
// 2. Calculation

#include "vm_cmd.h"
#include "vm_code.h"
#include "vm_stack.h"
#include "vm_calc.h"
#include "vm_assign.h"

int vm_run_inst (vm_inst* , ptr_table* , vm_stack* );

int
vm_exec_code( vm_inst* code , int num_insts , ptr_table* table , vm_stack* vmstack)
{
	int idx = 0;
	int move_forward = 0;	
	stack_item* top_item;
	vm_inst* inst;

	for(  ; idx < num_insts ; ++idx ){
		printf("----Current Stack----\n");
		vm_stack_display_all(vmstack);

		inst = &(code[idx]);
		if(inst->cmd == VM_JMP){
			move_forward = vm_code_jmp(code, idx, inst->label , num_insts);
			printf("VM_JMP: move forward by %d .\n", move_forward);
			idx = idx + move_forward ;
		}else if(inst->cmd == VM_FJMP){
			top_item = &( vmstack->stack[vmstack->sp] );
			if( (top_item->type == BOOLEAN) && (top_item->boolean == false) ){
				move_forward = vm_code_jmp(code, idx, inst->label , num_insts);
				idx = idx + move_forward ;

				//The top item on stack should be removed.
				vmstack->sp = vmstack->sp - 1;
			} else if( (top_item->type == BOOLEAN) && (top_item->boolean == true) ){
				printf("Top item of the current stack is boolean. and it says 'true' \n");
				//The top item on stack should be removed.
				vmstack->sp = vmstack->sp - 1;

			} else {
				printf("Top item of the current stack is not boolean... \n");
			}
		}else {
			printf("=> RUN INSTRUCTION: %s \n", vm_cmd_to_string(inst->cmd) );
			vm_run_inst(inst, table , vmstack);
		}
		printf("idx : %d %d (max %d)\n", idx, inst->cmd, num_insts);
	}
}

int
vm_run_inst (vm_inst* inst, ptr_table* table, vm_stack* vmstack )
{
	int move_forward = 0;
	switch (inst->cmd){
	case VM_PUSH_IVAL:
		vm_stack_push_ival(vmstack, inst->ival);
		break;
	case VM_PUSH_DVAL:
		vm_stack_push_dval(vmstack, inst->dval);
		break;
	case VM_PUSH_PP_IVAL:
		vm_stack_push_pp_ival(vmstack, &table, inst->ptr_key);
		break;
	case VM_PUSH_PP_DVAL:
		vm_stack_push_pp_dval(vmstack, &table, inst->ptr_key);
		break;
	case VM_PUSH_PP_STR:
		vm_stack_push_pp_str(vmstack, &table, inst->ptr_key);
		break;
	case VM_POP:
		vm_stack_pop(vmstack);
		break;
	case VM_FJMP:
	case VM_JMP:
		printf("This code should never be run. ");
		break;
	case VM_END:
		vm_stack_end(vmstack);
		break;
	case VM_DISP:
		vm_stack_display_item(vmstack, vmstack->sp);
		break;
	case VM_STO:
		vm_stack_store_val(vmstack);
		break;
	case VM_FCALL:
		// vm_stack_fcall(vmstack);
		break;
	case VM_ADDX:
		vm_calc_addx(vmstack);
		break;
	case VM_MULX:
		vm_calc_mulx(vmstack);
		break;
	case VM_SUBX:
		vm_calc_subx(vmstack);
		break;
	case VM_DIVX:
		vm_calc_divx(vmstack);
		break;
	case VM_POWX:
		vm_calc_powx(vmstack);
		break;
	case VM_FAC:
		vm_calc_factorial(vmstack);
		break;
	case VM_AND:
		vm_calc_and(vmstack);
		break;
	case VM_OR:
		vm_calc_or(vmstack);
		break;
	case VM_EQ:
		vm_calc_eq(vmstack);
		break;
	case VM_NEQ:
		vm_calc_neq(vmstack);
		break;
	case VM_LT:
		vm_calc_lt(vmstack);
		break;
	case VM_LE:
		vm_calc_le(vmstack);
		break;
	case VM_GT:
		vm_calc_gt(vmstack);
		break;
	case VM_GE:
		vm_calc_ge(vmstack);
		break;
	case VM_NEG:
		vm_calc_neg(vmstack);
		break;
	case VM_LABEL:
		// Do nothing.
		break;
	default:
		printf("ERROR: undefined VM command specified. \n");
		break;
	}

	return move_forward;
}

// Sample Code
/*
vm_code vm_code1 = {
    { .cmd = VM_PUSH_PP_IVAL, .ptr_key = "x" },
    { .cmd = VM_PUSH_IVAL, .ival = 11 },
    { .cmd = VM_PUSH_IVAL, .ival = 22 },
    { .cmd = VM_ADDX },
	{ .cmd = VM_DISP },
    { .cmd = VM_STO },
    { .cmd = VM_END }
};


int main(int args, char** argv){
	vm_stack* vmstack = vm_stack_init();
	ptr_table* table = ptr_table_init();
	int* address_for_x = (int *)malloc(sizeof(int));
	*address_for_x = 0;
	ptr_table_add( &table, "x", (void**) &address_for_x, PTR_INT, GC_NO);
	printf("---- Show ptr_table ----\n");
	ptr_table_show_all(&table);
	printf("\n");

	vm_exec_code(vm_code1, sizeof(vm_code1)/sizeof(vm_code1[0]), table , vmstack);
	printf("\n");
	printf("---- Show ptr_table ----\n");
	ptr_table_show_all(&table);
	printf("\n");

	free(address_for_x);
	return 0;
}
*/

