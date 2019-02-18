#include "vm_rexp.h"
#include "simple_re.h"
#include "common_string.h"
#include "vm_item_pp2val.h"
#include <stdio.h>
#include <stdbool.h>

int
vm_rexp_match(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	stack_item_pp2value( top_item );
	stack_item_pp2value( sec_item );

	stack_item* str_item ;
	stack_item* rexp_item ;

	string_object* str_obj;
	simple_re* rexp_obj;
	int matched_pos;
	bool result_bool;

	if( (top_item->type == PP_STR ) && (sec_item->type == PP_REXP)){
		str_item = top_item;
		rexp_item = sec_item;
	} else if ( (top_item->type == PP_REXP ) && (sec_item->type == PP_STR)){
		str_item = sec_item;
		rexp_item = top_item;
	} else {
		printf("ERROR: REXP_MATCH should have REXP and STR on each side respectively.\n");
	}

	str_obj = *(str_item->pp_str);
	rexp_obj = *(rexp_item->pp_rexp);
	matched_pos = simple_re_match( rexp_obj , string_read(str_obj));

	if(matched_pos > 0 ){
		result_bool = true;
	}else{
		result_bool = false;
	}

	vmstack->sp = vmstack->sp - 1; 
	sec_item->type = BOOLEAN ;
	sec_item->boolean = result_bool;
}

	




