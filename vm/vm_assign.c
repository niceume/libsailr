#include <string.h>
#include <stdio.h>
#include "common_string.h"
#include "vm_stack.h"
#include "vm_assign.h"
#include "../ptr_table.h"

int
vm_stack_store_val(vm_stack* vmstack)
{
	stack_item* lvalue = vm_stack_second(vmstack);
	stack_item* rvalue = vm_stack_top(vmstack);
	
	ptr_record* left_record;
	if( (lvalue->type != NULL_ITEM) && (lvalue->type != PP_IVAL ) && (lvalue->type != PP_DVAL ) && (lvalue->type != PP_STR )){
		printf("ERROR: lvalue should be pointer to pointer.\n");
	} else {
		left_record = (ptr_record*)lvalue->p_record;
	}

	double tmp_dbl;
	int tmp_int; 
	char* tmp_str;
	string_object* tmp_s_str;

	if(lvalue->type == NULL_ITEM){
		if(left_record->type == PTR_NULL){
			if(rvalue->type == IVAL){
				// change type from PTR_NULL to PTR_INT on ptr_table.
				left_record->type = PTR_INT;
				left_record->address = malloc(sizeof(int));
				left_record->gc = GC_YES;
				left_record->ex_type = PTR_DBL;
				left_record->ex_addr = malloc(sizeof(double));
				left_record->ex_gc = GC_YES;
				// assign value to the newly allocated memory.
				memcpy( left_record->address, &(rvalue->ival), sizeof(int));
				*((int*)left_record->ex_addr) = 0;
			}else if( rvalue->type == DVAL){
				// change type from PTR_NULL to PTR_DBL on ptr_table.
				left_record->type = PTR_DBL;
				left_record->address = malloc(sizeof(double));
				left_record->gc = GC_YES;
				left_record->ex_type = PTR_INT;
				left_record->ex_addr = malloc(sizeof(int));
				left_record->ex_gc = GC_YES;
				// assign value to the newly allocated memory.
				memcpy( left_record->address, &(rvalue->dval), sizeof(double));
				*((double*)left_record->ex_addr) = 0.0;
			}else if( rvalue->type == PP_STR){
				// change type from PTR_NULL to PTR_STR on ptr_table.
				left_record->type = PTR_STR;
				left_record->address = (void*) string_new(string_read((string_object*) *(rvalue->pp_str)));
				left_record->gc = GC_YES;
				// ToDo: Do I need to free the string on rvalue ??
				// ToDo: I think to answer this, we need to know the GC flag of the record corresponding to this stack item.
			}else if( rvalue->type == PP_REXP){
				// change type from PTR_NULL to PTR_STR on ptr_table.
				left_record->type = PTR_REXP;
				left_record->address = (void*) *(rvalue->pp_rexp) ;
				left_record->gc = GC_YES;
			}else{
				// ToDo: ERROR
            }
		}else if(left_record->type == PTR_INT){ // Type compatible
			if(rvalue->type == IVAL){
				// continue to be left_record->type == PTR_INT
				left_record->gc = GC_YES;
				*((int*)left_record->address) = rvalue->ival;
			}else if( rvalue->type == DVAL){ // Type mismatch
				ptr_record_swap_addresses(left_record); // Main address type is now PTR_DBL
				left_record->gc = GC_YES;
				*((double*)left_record->address) = rvalue->dval;
			}else {
				printf("ERROR: Invalid");
			}
		}else if(left_record->type == PTR_DBL){
			if(rvalue->type == IVAL){// Type mismatch
				ptr_record_swap_addresses(left_record); // Main address type is now PTR_INT
				left_record->gc = GC_YES;
				*((int*)left_record->address) = rvalue->ival;
			}else if( rvalue->type == DVAL){ // Type compatible
				// continue to be left_record->type == PTR_DBL
				left_record->gc = GC_YES;
				*((double*)left_record->address) = rvalue->dval;
			}else {
				printf("ERROR: Invalid");
			}
		}else if(left_record->type == PTR_STR){
			if(rvalue->type == PP_STR){
				// ToDo: Do I really need to create new string object here??
				left_record->address = (void*) string_new(string_read((string_object*) *(rvalue->pp_str)));
				left_record->gc = GC_YES;
				// ToDo: Do I need to free the string on rvalue ??
				// ToDo: I think to answer this, we need to know the GC flag of the record corresponding to this stack item.
			}else {
				printf("ERROR: Invalid");
			}
		}
	}else if(lvalue->type == PP_IVAL){
		if(rvalue->type == IVAL){  // Type compatible
			printf("Assign int rvalue to lvalue pointer to pointer.\n");
			memcpy( *(lvalue->pp_ival), &(rvalue->ival), sizeof(int));
			printf("Value: %d, Pointer: %p \n", **(lvalue->pp_ival), *(lvalue->pp_ival) );
		}else if(rvalue->type == DVAL){  // Type mismatch
			printf("NOTE: LHS is int, RHS is double. LHS variable becomes double. \n");
			// ToDo: Don't I need to free original value?
			ptr_record_swap_addresses(left_record); // Main address type is now PTR_DBL
			memcpy( *(lvalue->pp_dval), &(rvalue->dval), sizeof(double));
		}else{
			printf("LHS is PP_IVAL and RHS is type, %d.", rvalue->type );
		}
	}else if(lvalue->type == PP_DVAL){
		if(rvalue->type == IVAL){  // Type mismatch
			// ToDo: Don't I need to free original value?
			ptr_record_swap_addresses(left_record); // Main address type is now PTR_INT
			memcpy( *(lvalue->pp_ival), &(rvalue->ival), sizeof(int));
		}else if(rvalue->type == DVAL){  // Type compatible
			printf("Assign double rvalue to lvalue pointer to pointer.\n");
			memcpy( *(lvalue->pp_dval), &(rvalue->dval), sizeof(double));
			printf("Value: %f, Pointer: %p \n", **(lvalue->pp_dval), *(lvalue->pp_dval) );
		}else{
			printf("LHS is PP_IVAL and RHS is type, %d.", rvalue->type );
		}
	}else if(lvalue->type == PP_STR){
		if(rvalue->type == IVAL){ // Mismatch
			sprintf( tmp_str , "%d" , rvalue->ival);
			tmp_s_str = string_new(tmp_str );
			string_free(*(lvalue->pp_str));  // ToDo: Really need free?
			*(lvalue->pp_str) = tmp_s_str;
		}else if(rvalue->type == DVAL){
			sprintf( tmp_str , "%f" , rvalue->dval);
			tmp_s_str = string_new(tmp_str );
			string_free(*(lvalue->pp_str));  // ToDo: Really need free?
			*(lvalue->pp_str) = tmp_s_str;
		}else if(rvalue->type == PP_STR){
			printf("ASSING STRING VALUE");
			string_free(*(lvalue->pp_str));
			printf("LVALUE: pointer to pointer to string.\n");
			printf("LVALUE: String Value %s \n",string_read( *( lvalue->pp_str)));
			printf("LVALUE: pointer to string %p \n", *( lvalue->pp_str) );
			printf("LVALUE: pointer to pointer to string %p \n" , ( lvalue->pp_str));

			printf("RVALUE: pointer to pointer to string.\n");
			printf("RVALUE: String Value %s \n",string_read( *( rvalue->pp_str)));
			printf("RVALUE: pointer to string %p \n", *( rvalue->pp_str) );
			printf("RVALUE: pointer to pointer to string %p \n" , ( rvalue->pp_str));
			(*(lvalue->pp_str)) = *(rvalue->pp_str);

			printf("RVALUE is assigned to LVALUE");
			printf("LVALUE: pointer to pointer to string.\n");
			printf("LVALUE: String Value %s \n",string_read( *( lvalue->pp_str)));
			printf("LVALUE: pointer to string %p \n", *( lvalue->pp_str) );
			printf("LVALUE: pointer to pointer to string %p \n" , ( lvalue->pp_str));
		}else{
			printf("LHS is PP_IVAL and RHS is type, %d.", rvalue->type );
		}
	}

	vmstack->sp = ((vmstack->sp) - 2);
	return 1;
}


