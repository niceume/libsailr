#include <string.h>
#include <stdio.h>
#include "common_string.h"
#include "vm_stack.h"
#include "vm_assign.h"

int
vm_stack_store_val(vm_stack* vmstack)
{

	stack_item* lvalue = vm_stack_second(vmstack);
	stack_item* rvalue = vm_stack_top(vmstack);

	if( (lvalue->type != PP_IVAL ) && (lvalue->type != PP_DVAL ) && (lvalue->type != PP_STR ))
		printf("lvalue should be pointer to pointer.\n");

	double tmp_dbl;
	int tmp_int; 
	char* tmp_str;
	string_object* tmp_s_str;

	switch (rvalue->type){
		case IVAL:
		if(lvalue->type == PP_IVAL){
			printf("Assign int rvalue to lvalue pointer to pointer.\n");
			memcpy( *(lvalue->pp_ival), &(rvalue->ival), sizeof(int));
			printf("Value: %d, Pointer: %p \n", **(lvalue->pp_ival), *(lvalue->pp_ival) );
		}else if(lvalue->type == PP_DVAL){
			tmp_dbl = (double) (rvalue->ival);
			memcpy( *(lvalue->pp_dval), &tmp_dbl, sizeof(double));
		}else if(lvalue->type == PP_STR){
			sprintf( tmp_str , "%d" , rvalue->ival);
			tmp_s_str = string_new(tmp_str );
			string_free(*(lvalue->pp_str));
			*(lvalue->pp_str) = tmp_s_str;
		}else{
			printf("unintended lvalue type.\n");
			return 0;
		}
		break;

		case DVAL:
		if(lvalue->type == PP_IVAL){
			printf("CAUTION: double is caseted into int.\n");
			tmp_int = (int) (rvalue->dval);
			memcpy( *(lvalue->pp_ival), &tmp_int, sizeof(double));
		}else if(lvalue->type == PP_DVAL){
			memcpy( *(lvalue->pp_dval), &(rvalue->dval), sizeof(double));
		}else if(lvalue->type == PP_STR){
			sprintf( tmp_str , "%d" , rvalue->dval);
			tmp_s_str = string_new(tmp_str);
			string_free(*(lvalue->pp_str));
			*(lvalue->pp_str) = tmp_s_str;
		}else{
			printf("unintended lvalue type.\n");
			return 0;
		}
			
		break;

		case PP_STR:
		if(lvalue->type == PP_STR){
			string_free(*(lvalue->pp_str));
			*(lvalue->pp_str) = *(rvalue->pp_str);
		}else{
			printf("unintended lvalue type.\n");
		}
		break;
	}

//	printf("%d\n", vmstack->sp);
	vmstack->sp = ((vmstack->sp) - 2);
//	printf("%d\n ", vmstack->sp);
	return 1; 

}
