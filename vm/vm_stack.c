#include "vm_stack.h"
#include "ptr_table.h"
#include "common_string.h"
#include "simple_re.h"
#include "vm_call_func.h"
#include <stdio.h>
#include "helper.h"

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
	item->type = INFO_ITEM;
	item->p_vm_stack_info = (vm_stack_info*)malloc(sizeof(vm_stack_info));
	item->p_vm_stack_info->characterEncoding = DEFAULT_VM_CHARACTER_ENCODING;
	item->p_vm_stack_info->max_size = MAXSTACKSIZE;

	memcpy( &(stack->stack[stack->sp]), item, sizeof(stack_item));
	return stack;
}

int
vm_stack_set_encoding(vm_stack* vmstack, const char* encoding)
{	
    vmstack->stack[0].p_vm_stack_info->characterEncoding = encoding;
	return 1;
}

const char*
vm_stack_get_encoding(vm_stack* vmstack)
{
    return (vmstack->stack[0].p_vm_stack_info->characterEncoding);
}


int
vm_stack_push_item( vm_stack* stack, stack_item* item )
{
	stack->sp = stack->sp + 1;
	memcpy( &(stack->stack[stack->sp]), item, sizeof(stack_item));

	if (vm_stack_is_full(stack)) {
		printf("ERROR: The stack is full.\n");
		return 0;
	}
	return 1;
}

int
vm_stack_push_ival( vm_stack* stack , int num)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ IVAL, {.ival = num} , JUST_A_VALUE },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	return 1;
}

int
vm_stack_push_dval( vm_stack* stack , double num)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ DVAL, {.dval = num} , JUST_A_VALUE },
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
	DEBUG_PRINT("push new_stack_item: pointer to pointer to %d \n", **(new_stack_item->pp_ival) );
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
	DEBUG_PRINT("push new_stack_item: pointer to pointer to %f \n", **(new_stack_item->pp_dval) );
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
        printf("ERROR: For PUSH_PP_NUM instruction, types on pointer table should be PTR_INT or PTR_DBL.\n");
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
}

int
vm_stack_push_temp_pp_str( vm_stack* stack , string_object** pp_str)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ PP_STR, {.pp_str = pp_str}, TEMP_OBJECT },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
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
}

int
vm_stack_push_corresp_item( vm_stack* stack , ptr_table** table, char* ptr_key)
{
	ptr_record* record = ptr_table_find(table, ptr_key);
	if(record->type == PTR_NULL ){
		vm_stack_push_null( stack, table, ptr_key);
	}else if(record->type == PTR_INT ){
		vm_stack_push_pp_ival( stack, table, ptr_key);
	}else if(record->type == PTR_DBL ){
		vm_stack_push_pp_dval( stack, table, ptr_key);
	}else if(record->type == PTR_STR ){
		vm_stack_push_pp_str( stack, table, ptr_key);
	}else if(record->type == PTR_REXP ){
		vm_stack_push_pp_rexp( stack, table, ptr_key);
	}else{
		// Boolean is not on ptr table. vm_stack_push_boolean is not required in this function.
		printf("ERROR: ptr_table holds unknown type for variable, %s\n", ptr_key);
	}
}

int
vm_stack_push_boolean( vm_stack* stack, bool boolean)
{
	stack_item* new_stack_item = (stack_item*)malloc(sizeof(stack_item));
	memcpy(new_stack_item,
		&(stack_item const){ BOOLEAN, {.boolean = boolean} , NOT_ON_PTR_TABLE },
		sizeof(stack_item));
	vm_stack_push_item(stack, new_stack_item);
	return 1;
}

int
vm_stack_fcall( vm_stack* vmstack, char* fname , int num_args, ptr_table** table)
{
	DEBUG_PRINT("Function name is %s\n", fname);
	call_func(vmstack, fname, num_args, table);
}


stack_item*
vm_stack_pop( vm_stack* vmstack )
{
	if (vm_stack_is_empty(vmstack)){
		printf("ERROR: The stack is empty.\n");
		return 0;
	}
	stack_item* current_item_ptr = &(vmstack->stack[vmstack->sp]) ;
	vmstack->sp = vmstack->sp - 1;
	return current_item_ptr ;
}

int
vm_stack_clean_top(vm_stack* vmstack)
{
	stack_item top_item = vmstack->stack[vmstack->sp];
	switch( top_item.type ){
	case IVAL:
		top_item.ival = 0;
		break;
	case DVAL:
		top_item.dval = 0.0;
		break;
	case BOOLEAN:
		top_item.boolean = false;
		break;
	case PP_IVAL:
		if( vm_stack_item_is_temp( &top_item )){
			free(*(top_item.pp_ival));
			free(top_item.pp_ival);
			printf("ERROR: This case should not be executed. (PP_IVAL)");
		}
		top_item.pp_ival = NULL;
		break;
	case PP_DVAL:
		if( vm_stack_item_is_temp( &top_item )){
			free(*(top_item.pp_dval));
			free(top_item.pp_dval);
			printf("ERROR: This case should not be executed. (PP_DVAL)\n");
		}
		top_item.pp_dval = NULL;
		break;
	case PP_STR:
		if( vm_stack_item_is_temp( &top_item )){ 
			string_free(*(top_item.pp_str));
			free(top_item.pp_str);
		}
		top_item.pp_str = NULL;
		break;
	case PP_REXP:
		if( vm_stack_item_is_temp( &top_item )){
			simple_re_free(*(top_item.pp_rexp));
			free(top_item.pp_rexp);
			printf("ERROR: This case should not be executed. (PP_REXP)\n");
		}
		top_item.pp_rexp = NULL;
		break;
	case NULL_ITEM:
		top_item.p_record = NULL;
		break;
	case VOID_ITEM:
		break;
	case INFO_ITEM:
		free(top_item.p_vm_stack_info);
		break;
	}
	top_item.type = VOID_ITEM;
	top_item.p_record = NULL;
	DEBUG_PRINT("clean %s\n", display_item_type(top_item.type));
}

int
vm_stack_clean_and_pop( vm_stack* vmstack, int n)
{
	if (vm_stack_is_empty(vmstack)){
		printf("ERROR: The stack is empty.\n");
		return 0;
	}
	int idx;
	for( idx = 0 ; idx < n ; idx++ ){
		vm_stack_clean_top(vmstack);
		vm_stack_pop(vmstack);
	}
	return 1;
}

int
vm_stack_clean_items_from_zero_to_top( vm_stack* vmstack )
{
	int idx = vmstack->sp;
	while(idx >= 0){
		vm_stack_clean_top( vmstack );
		idx = idx - 1 ;
	}
	return 1;
}

bool
vm_stack_item_is_temp( stack_item* item )
{
	switch(item->type){
	case IVAL:
	case DVAL:
	case BOOLEAN:
	case NULL_ITEM:
		break;
	case PP_IVAL:
	case PP_DVAL:
	case PP_REXP:
		if(item->p_record == NULL){
			printf("ERROR: This branch is not supposed to be executed, but must be temporary object.");
			return true;
		}
		break;
	case PP_STR:
		if(item->p_record == NULL){
			DEBUG_PRINT("This string object is temporary object.\n");
			return true;
		}
		break;
	default:
		printf("ERROR: Unsuppored type.");
		break;
	}
	
	return false;
}

void
vm_stack_display_item(vm_stack* vmstack, int idx)
{
	if(idx < 0 )
		printf("ERROR: idx does not allow negative values. \n");
	if(idx > (vmstack->sp) )
		printf("ERROR: idx specifieed is over stack pointer. \n");

	stack_item* stack = vmstack->stack;
	switch (stack[idx].type)
	{
	case IVAL:
		DEBUG_PRINT("%04d \t%s %d\n", idx, display_item_type(stack[idx].type), stack[idx].ival );
		break;
	case DVAL:
		DEBUG_PRINT("%04d \t%s %lf\n", idx, display_item_type(stack[idx].type), stack[idx].dval );
		break;
	case PP_IVAL:
		DEBUG_PRINT("%04d \t%s \t%p \t%d\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_ival), **(stack[idx].pp_ival));
		break;
	case PP_DVAL:
		DEBUG_PRINT("%04d \t%s \t%p \t%lf\n", idx, display_item_type(stack[idx].type), *(stack[idx].pp_dval), **(stack[idx].pp_dval));
		break;
	case PP_STR:
		DEBUG_PRINT("%04d", idx);
		DEBUG_PRINT("\t%s", display_item_type(stack[idx].type));
		DEBUG_PRINT("\t%p (address of ptr to str)", (stack[idx].pp_str));
		DEBUG_PRINT("\t%p (address of str)", *(stack[idx].pp_str));
		DEBUG_PRINT("\t%s\n", string_read(*(stack[idx].pp_str)));
		break;
	case PP_REXP:
		DEBUG_PRINT("%04d", idx);
		DEBUG_PRINT("\t%s", display_item_type(stack[idx].type));
		DEBUG_PRINT("\t%p (address of ptr to rexp)", (stack[idx].pp_rexp));
		DEBUG_PRINT("\t%p (address of rexp)", *(stack[idx].pp_rexp));
		DEBUG_PRINT("\t%s\n", simple_re_read_pattern(*(stack[idx].pp_rexp)));
		break;
	case BOOLEAN:
		DEBUG_PRINT("%04d \t%s \t%d \n", idx, display_item_type(stack[idx].type), stack[idx].boolean );
		break;
	case NULL_ITEM:
		DEBUG_PRINT("%04d \t%s %p (address on ptr_table) \n", idx, display_item_type(stack[idx].type), ((ptr_record*) stack[idx].p_record)->address );
		break;
	case VOID_ITEM:
		DEBUG_PRINT("%04d This VOID_ITEM should not be displayed. This index should already be out of stack pointer.\n", idx);
		break;
	case INFO_ITEM:
		DEBUG_PRINT("%04d This INFO_ITEM should not be displayed. Only for internal use.\n", idx);
		break;
	}
}

void
vm_stack_display_all(vm_stack* vmstack)
{
	int idx = vmstack->sp;
	while(idx > 0){
		vm_stack_display_item( vmstack, idx );
		idx = idx - 1 ;
	}
}

int
vm_stack_end( vm_stack* vmstack ){
	if (vmstack->sp > 0 ) {
		DEBUG_PRINT("CAUTION: There are some items left on virtual stack machine.\n");
		vm_stack_display_all(vmstack);
	}
	vm_stack_clean_items_from_zero_to_top(vmstack);
	vm_stack_free(vmstack);
	DEBUG_PRINT("Virtual machine is disallocated. \n");
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
	} else if (vmstack->sp > 0 ){
		return 0; // False
	} else{
		printf("ERROR: vmstack->sp is negative value.\n");
		return -1;
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
		printf("ERROR: The item below top is NULL. ");
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
		printf("ERROR: The item second below top is NULL. ");
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
		printf("ERROR: The item nth (%d) below top is NULL. ", nth);
		return NULL;
	} else {
	  return &(vmstack->stack[idx]);
	}
}

