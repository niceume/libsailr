#include "vm_stack.h"
#include "ptr_table.h"
#include "common_string.h"
#include "simple_re.h"
#include "vm_call_func.h"
#include <stdio.h>

#define Y(a, b) b,
char *vm_stack_item_name[] = {
  VM_STACK_ITEM_NAME_TABLE
};
#undef Y

char*
display_item_type(ItemType type)
{
	return vm_stack_item_name[ type ];
}

// private
vm_stack*
vm_stack_init()
{
	vm_stack* stack = (vm_stack*)malloc(sizeof(vm_stack));
	stack->sp = 0;

	stack_item* item = (stack_item*)malloc(sizeof(stack_item));
	item->type = NULL_ITEM;
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
		&(stack_item const){ IVAL, {.ival = num} , NULL },
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
		&(stack_item const){ DVAL, {.dval = num} , NULL },
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
		&(stack_item const){ PP_IVAL, {.pp_ival = pp_ival}, record },
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
		&(stack_item const){ PP_DVAL, {.pp_dval = pp_dval}, record },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
}

int
vm_stack_push_pp_num( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
    if(record->type == PTR_INT){
        vm_stack_push_pp_ival(stack, table, ptr_key);
    } else if (record->type == PTR_DBL){
        vm_stack_push_pp_dval(stack, table, ptr_key);
    } else {
        printf("ERROR: For records corresponding to instructions of PUSH_PP_NUM, ");
        printf("types should be PTR_INT or PTR_DBL.\n");
    }
}

int
vm_stack_push_pp_str( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	string_object** pp_str = (string_object**) &(record->address);
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_STR, {.pp_str = pp_str}, record },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	vm_stack_display_item(stack, stack->sp);
}

int
vm_stack_push_pp_rexp( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	simple_re** pp_rexp = (simple_re**) &(record->address);
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_REXP, {.pp_rexp = pp_rexp}, record },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	vm_stack_display_item(stack, stack->sp);
}

int
vm_stack_push_null( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ NULL_ITEM, {.ptr = NULL}, record },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	vm_stack_display_item(stack, stack->sp);
}

int
vm_stack_fcall( vm_stack* vmstack, char* fname , int num_args)
{
	call_func(vmstack, fname, num_args);
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
		printf("%04d \t%s %d\n", idx, display_item_type(stack[idx].type), stack[idx].ival );
		break;
	case DVAL:
		printf("%04d \t%s %lf\n", idx, display_item_type(stack[idx].type), stack[idx].dval );
		break;
	case PP_IVAL:
		printf("%04d \t%s \t%p \t%d\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_ival), **(stack[idx].pp_ival));
		break;
	case PP_DVAL:
		printf("%04d \t%s \t%p \t%lf\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_dval), **(stack[idx].pp_dval));
		break;
	case PP_STR:
		printf("%04d \t%s \t%p (ptr to str) \t%s\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_str), string_read(*(stack[idx].pp_str)));
		break;
	case PP_REXP:
		printf("%04d \t%s \t%p (ptr to rexp) \t%s\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_rexp), simple_re_read_pattern(*(stack[idx].pp_rexp)));
		break;
	case BOOLEAN:
		printf("%04d \t%s \t%d \n", idx, display_item_type(stack[idx].type), stack[idx].boolean );
		break;
	case NULL_ITEM:
		printf("%04d \t%s %p (address on ptr_table) \n", idx, display_item_type(stack[idx].type), ((ptr_record*) stack[idx].p_record)->address );
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

stack_item*
vm_stack_nth( vm_stack* vmstack , int nth)
{
	int idx = vmstack->sp - (nth - 1 ) ;
	if (idx <= 0) {
		printf("The item nth (%d) below top is NULL. ", nth);
		return NULL;
	} else {
	  return &(vmstack->stack[idx]);
	}
}

