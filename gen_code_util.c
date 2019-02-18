#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gen_code_util.h"
#include "vm_code.h"

vm_inst*
new_vm_inst_command( VM_CMD cmd )
{
	vm_inst* temp_inst = (vm_inst*)malloc(sizeof(vm_inst));
	temp_inst->cmd = cmd;
	temp_inst->prev = NULL;
	temp_inst->next = NULL;
	temp_inst->last = temp_inst;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_ival( int ival )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_IVAL );
	temp_inst->ival = ival;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_dval( double dval )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_DVAL );
	temp_inst->dval = dval;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_pp_ival( char* ptr_key )
{
//	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_IVAL );
    vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_NUM );
	temp_inst->ptr_key = ptr_key;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_pp_dval( char* ptr_key )
{
//	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_DVAL );
    vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_NUM );
	temp_inst->ptr_key = ptr_key;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_pp_str( char* ptr_key )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_STR );
	temp_inst->ptr_key = ptr_key;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_pp_rexp( char* ptr_key )
{
	printf("Going to generate VM code for REXP\n");
	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_PP_REXP );
	temp_inst->ptr_key = ptr_key;
	return temp_inst;
}

vm_inst*
new_vm_inst_push_null( char* ptr_key )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_PUSH_NULL );
	temp_inst->ptr_key = ptr_key;
	return temp_inst;
}

vm_inst*
new_vm_inst_label( char* label )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_LABEL );
	temp_inst->label = label;
	return temp_inst;
}


vm_inst* 
new_vm_inst_fjmp( char* label )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_FJMP );
	temp_inst->label = label;
	return temp_inst;
}


vm_inst*
new_vm_inst_jmp( char* label )
{
	vm_inst* temp_inst = new_vm_inst_command( VM_JMP );
	temp_inst->label = label;
	return temp_inst;
}


vm_inst*
vm_inst_list_last( vm_inst_list* list )
{
	
	vm_inst* last_inst = list->last ;
	return last_inst;
}


vm_inst*
vm_inst_list_reach_last( vm_inst_list* list )
{
	vm_inst* curr_inst = list;
	while(curr_inst->next != NULL)
	{
		curr_inst = curr_inst->next;
	}
	return curr_inst;
}

vm_inst*
vm_inst_list_get(vm_inst_list* list , int zero_indexed)
{
	int idx = zero_indexed;
	vm_inst* curr_inst = list;
	do{
		if(idx != 0){
			idx = idx - 1;
			curr_inst = curr_inst->next;
		} else {
			return curr_inst;
		}
	}while(curr_inst->next != NULL);

	printf("index is out of bound. The returned inst is the last one.\n");
	return curr_inst;
}

int
vm_inst_list_size(vm_inst_list* list )
{
	int size = 1;
	vm_inst* curr_inst = list;
	while(curr_inst->next != NULL)
	{
		curr_inst = curr_inst->next;
		size = size + 1;
	}
	return size;
}

vm_inst_list*
vm_inst_list_cat( vm_inst_list* list1, vm_inst_list* list2 )
{
	vm_inst* list1_last = vm_inst_list_last(list1);

	list1_last->next = list2;
	list2->prev = list1_last;

	list1->last = vm_inst_list_last(list2);

	return list1;
}

void
vm_inst_list_display_all( vm_inst_list* list )
{
	char* curr_cmd;
	vm_inst* curr_inst;
	curr_inst = list;
    printf("At this point, VM instructions just holds 'key name' for ptr_table record.\n");
    printf("For values, VM instructions holds values themselves.\n");
	do{
		vm_inst_display( curr_inst );
		curr_inst = curr_inst->next;
	}while(curr_inst != NULL);
}

/*
vm_inst_list*
vm_inst_list_insert( vm_inst_list* list1, int idx, vm_inst_list* list2)
{
	if(idx <= 0 )
		printf("idx should be larger than zero.\n");
	if(idx > (vm_inst_list_size(list1) - 1) )
		printf("idx is out of bound.\n");

	vm_inst* list1_inst_before_idx = vm_inst_list_get(list1, idx - 1);
	vm_inst* list1_inst_at_idx = vm_inst_list_get(list1, idx );
	vm_inst* list2_head = list2;
	vm_inst* list2_tail = vm_inst_list_tail( list2 );

	list1_inst_before_idx->next = list2_head;
	list2_head->prev = list1_inst_before_idx;

	list1_inst_at_idx->prev = list2_tail;
	list2_tail->next = list1_inst_at_idx;

	return list1;
}
*/

int
vm_inst_list_free( vm_inst_list* inst_list )
{
	vm_inst* curr_inst = inst_list;
	vm_inst* next_inst = curr_inst->next;
	vm_inst_free( curr_inst );
	printf("Free vm_inst \n");
	if(next_inst != NULL){
		vm_inst_list_free(next_inst);
	}
}

vm_inst*
vm_inst_list_to_code( vm_inst_list* list)
{
	int size = vm_inst_list_size(list);
	vm_inst* vm_code_start = (vm_inst*)malloc(sizeof(vm_inst) * size);
	vm_inst* vm_code_ptr = vm_code_start;
	vm_inst* vm_inst_list_ptr = list;
	int idx;
	for( idx = 0 ; idx < size ; idx = idx + 1){
		memcpy( vm_code_ptr, vm_inst_list_ptr, sizeof(vm_inst) );
		vm_inst_list_ptr = vm_inst_list_ptr->next;
		vm_code_ptr = vm_code_ptr + 1;
	}
	return vm_code_start;
}


// main function to check behavior 
// gcc gen_code_util.c vm/vm_code.o vm/vm_cmd.o -Ivm

/*
#define cat_insts(a, b) vm_inst_list_cat(a, b) 

int
main(int argc, char** argv)
{
	vm_inst* list = new_vm_inst_push_pp_ival("x");
	cat_insts( list, new_vm_inst_push_ival(11));
	cat_insts( list, new_vm_inst_push_ival(22));
	cat_insts( list, new_vm_inst_command(VM_ADDX));
	cat_insts( list, new_vm_inst_command(VM_DISP));
	cat_insts( list, new_vm_inst_command(VM_STO));
	cat_insts( list, new_vm_inst_command(VM_END));

	printf("---Show all---\n");
	vm_inst_list_show_all( list );
	printf("---Free all---\n");
	vm_inst_list_free( list );
}
*/

