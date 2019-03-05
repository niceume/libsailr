// This function file will be moved to another directory.
// 

#include "c_funcs.h"
#include "vm_item_pp2val.h"
#include "vm_stack.h"
#include "common_string.h"
#include "simple_re.h"

#include <stdio.h>

int
sailr_c_func_print(vm_stack* vmstack, int num_args)
{
	if(num_args <= 0 ){
		printf("ERROR: print function should have one argument, but has %d \n", num_args );
	}

	stack_item* temp_item;
	int idx;
	string_object* new_str = string_new("");
	string_object* new_str2;
	for(idx = num_args ; idx >= 1 ; --idx ){
		temp_item = vm_stack_nth( vmstack , idx );
		stack_item_pp2value(temp_item);
		if(temp_item->type == PP_STR){
			/* Use the item as it is. */
			new_str2 = string_concat(new_str, *(temp_item->pp_str));
			string_free(new_str);
			new_str = new_str2;
		} else if (temp_item->type == IVAL) {
			printf("ERROR: Unsupported types are specified");	
		} else if (temp_item->type == DVAL) {
			printf("ERROR: Unsupported types are specified");
		} else if (temp_item->type == NULL_ITEM) {
			/* Nothing to be added. */
		} else{
			printf("ERROR: Unsupported types are specified");
		}
	}
	printf("%s", string_read(new_str));
	vmstack->sp = vmstack->sp  - num_args;
}

