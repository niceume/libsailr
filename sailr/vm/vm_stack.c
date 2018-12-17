#include "vm_stack.h"
#include "ptr_table.h"
#include "common_string.h"
#include <stdio.h>

char* ItemTypeDisplay[] = {
	"IVAL",
	"DVAL",
	"BOOLEAN",
	"PP_IVAL",
	"PP_DVAL",
	"PP_STR",
	"NULLPTR"
};

// private
vm_stack*
vm_stack_init()
{
	vm_stack* stack = (vm_stack*)malloc(sizeof(vm_stack));
	stack->sp = 0;

	stack_item* item = (stack_item*)malloc(sizeof(stack_item));
	item->type = NULLPTR;
	item->ptr = NULL;

	memcpy( &(stack->stack[stack->sp]), item, sizeof(stack_item));
	return stack;
}


int
vm_stack_push_item( vm_stack* stack, stack_item* item )
{
	stack->sp = stack->sp + 1;
	memcpy( &(stack->stack[stack->sp]), item, sizeof(stack_item));

	if (vm_stack_is_full(stack)) {
		printf("The stack is full.\n");
		return 0;
	}
	return 1;
}

int
vm_stack_push_ival( vm_stack* stack , int num)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ IVAL, {.ival = num} },
		sizeof(stack_item));
	printf("new_stack_item: ival %d \n", new_stack_item->ival );
	vm_stack_push_item(stack, new_stack_item);
	return 1;
}

int
vm_stack_push_dval( vm_stack* stack , double num)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ DVAL, {.dval = num} },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	return 1;
}


int
vm_stack_push_pp_ival( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	int** pp_ival = (int**) &(record->address);

	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_IVAL, {.pp_ival = pp_ival} },
		sizeof(stack_item));
	printf("new_stack_item: pointer to pointer to %d \n", **(new_stack_item->pp_ival) );
	vm_stack_push_item(stack, new_stack_item);
}

int
vm_stack_push_pp_dval( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	double** pp_dval = (double**) &(record->address);
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_DVAL, {.pp_dval = pp_dval} },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
}

int
vm_stack_push_pp_str( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	string_object** pp_str = (string_object**) &(record->address);
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_STR, {.pp_str = pp_str} },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	vm_stack_display_item(stack, stack->sp);
}

stack_item*
vm_stack_pop( vm_stack* vmstack )
{
	if (vm_stack_is_empty(vmstack)){
		printf("The stack is empty.\n");
		return 0;
	}
	stack_item* current_item_ptr = &(vmstack->stack[vmstack->sp]) ;
	vmstack->sp = vmstack->sp - 1;
	return current_item_ptr ;
}


void
vm_stack_display_item(vm_stack* vmstack, int idx)
{
	if(idx < 0 )
		printf("idx does not allow negative values. \n");
	if(idx > (vmstack->sp) )
		printf("idx specifieed is over stack pointer. \n");

	stack_item* stack = vmstack->stack;
	switch (stack[idx].type)
	{
	case IVAL:
		printf("%04d \t%s %d\n", idx, ItemTypeDisplay[stack[idx].type], stack[idx].ival );
		break;
	case DVAL:
		printf("%04d \t%s %lf\n", idx, ItemTypeDisplay[stack[idx].type], stack[idx].dval );
		break;
	case PP_IVAL:
		printf("%04d \t%s \t%p \t%d\n", idx, ItemTypeDisplay[stack[idx].type], *(stack[idx].pp_ival), **(stack[idx].pp_ival));
		break;
	case PP_DVAL:
		printf("%04d \t%s \t%p \t%lf\n", idx, ItemTypeDisplay[stack[idx].type], *(stack[idx].pp_dval), **(stack[idx].pp_dval));
		break;
	case PP_STR:
//		printf("SHOWING STRING of %p. \n", stack[idx].pp_str);
		printf("%04d \t%s \t%p \t%s\n", idx, ItemTypeDisplay[stack[idx].type], *(stack[idx].pp_str), string_read(*(stack[idx].pp_str)));
		break;
	case BOOLEAN:
		printf("%04d \t%s \t%d \n", idx, ItemTypeDisplay[stack[idx].type], stack[idx].boolean );
		break;
	case NULLPTR:
		printf("%04d \t%s %p\n", idx, ItemTypeDisplay[stack[idx].type], stack[idx].ptr );
		break;
	}
}

void
vm_stack_display_all(vm_stack* vmstack)
{
	printf("Current stack size (sp): %d \n", vmstack->sp);
	int idx = vmstack->sp;
	while(idx > 0){
		vm_stack_display_item( vmstack, idx );
		idx = idx - 1 ;
	}
}

int
vm_stack_end( vm_stack* vmstack ){
	if (vmstack->sp > 0 ) {
		printf("There are some items left on virtual stack machine.\n");
		vm_stack_display_all(vmstack);
	}
	printf("Virtual machine is disallocated. \n");
	vm_stack_free(vmstack);
}

int
vm_stack_is_full( vm_stack* vmstack )
{
	if (vmstack->sp == MAXSTACKSIZE ){
		return 1; // True
	} else {
		return 0; // False
	}
}

int
vm_stack_is_empty( vm_stack* vmstack )
{
	if (vmstack->sp == 0 ){
		return 1; // True
	} else {
		return 0; // False
	}
}

int
vm_stack_size( vm_stack* vmstack )
{
	return vmstack->sp ;
}

int
vm_stack_free( vm_stack* vmstack )
{
	free(vmstack) ;
}

stack_item*
vm_stack_top( vm_stack* vmstack )
{
	return &(vmstack->stack[vmstack->sp]);
}

stack_item*
vm_stack_second( vm_stack* vmstack )
{
	int idx = vmstack->sp - 1 ;
	if (idx <= 0) {
		printf("The item below top is NULL. ");
		return NULL;
	} else {
	  return &(vmstack->stack[idx]);
	}
}

stack_item*
vm_stack_third( vm_stack* vmstack )
{
	int idx = vmstack->sp - 2 ;
	if (idx <= 0) {
		printf("The item second below top is NULL. ");
		return NULL;
	} else {
	  return &(vmstack->stack[idx]);
	}
}

