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

	if( (lvalue->type != NULL_ITEM) && (lvalue->type != PP_IVAL ) && (lvalue->type != PP_DVAL ) && (lvalue->type != PP_STR ))
		printf("ERROR: lvalue should be pointer to pointer.\n");

    ptr_record* left_record;
    if( lvalue->type == NULL_ITEM ){
        left_record = (ptr_record*)lvalue->p_record;
        if(left_record->type == PTR_NULL){
    	    switch (rvalue->type){
    		    case IVAL:
                // change type from PTR_NULL to PTR_INT on ptr_table.
                left_record->type = PTR_INT;
                // allocate memory for integer. ptr_record points to the address.
                left_record->address = malloc(sizeof(int));
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
                memcpy( left_record->address, &(rvalue->ival), sizeof(int)); 
    		    break;
    		    case DVAL:
                // change type from PTR_NULL to PTR_DBL on ptr_table.
                left_record->type = PTR_DBL;
                // allocate memory for integer. ptr_record points to the address.
                left_record->address = malloc(sizeof(double));
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
                memcpy( left_record->address, &(rvalue->dval), sizeof(double));
    		    break;
    		    case PP_STR:
                // change type from PTR_NULL to PTR_STR on ptr_table.
                left_record->type = PTR_STR;
                left_record->gc = GC_YES;
                // assign address of right string to the ptr_record.
                left_record->address = *(rvalue->pp_str);        
    		    break;
                default:
                break;
            }
	        vmstack->sp = ((vmstack->sp) - 2);
            return 1;
        }else{
            printf("lvalue is NULL_ITEM, but its corresponding item on ptr_table has type. \n");
            // Move to next if.
        }
    }


	double tmp_dbl;
	int tmp_int; 
	char* tmp_str;
	string_object* tmp_s_str;
    
    
	switch (rvalue->type){
		case IVAL:
		if(lvalue->type == PP_IVAL ){
			printf("Assign int rvalue to lvalue pointer to pointer.\n");
			memcpy( *(lvalue->pp_ival), &(rvalue->ival), sizeof(int));
			printf("Value: %d, Pointer: %p \n", **(lvalue->pp_ival), *(lvalue->pp_ival) );
		}else if(lvalue->type == PP_DVAL){  // MISMATCH
			tmp_dbl = (double) (rvalue->ival);
			memcpy( *(lvalue->pp_dval), &tmp_dbl, sizeof(double));
		}else if(lvalue->type == PP_STR){  // MISMATCH
			sprintf( tmp_str , "%d" , rvalue->ival);
			tmp_s_str = string_new(tmp_str );
			string_free(*(lvalue->pp_str));
			*(lvalue->pp_str) = tmp_s_str;
        }else if(lvalue->type = NULL_ITEM){
            left_record = (ptr_record*)lvalue->p_record;
            if(left_record->type == PTR_INT){
                // left_record->type continues to be PTR_INT.
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
                *((int*)left_record->address) = rvalue->ival;
            }else if(left_record->type == PTR_DBL){ // MISMATCH
                // left_record->type continues to be PTR_DBL.
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
                tmp_dbl = (double)(rvalue->ival);
                *((double*)left_record->address) = tmp_dbl;   
            }else{
                printf("lvalue type on stack is null.");
                printf("Mismatch rvalue with lvalue on ptr_table.\n");
            }
		}else{
			printf("unintended lvalue type for right IVAL.\n");
			return 0;
		}
		break;

		case DVAL:
		if(lvalue->type == PP_IVAL){  // MISMATCH
			printf("CAUTION: double is caseted into int.\n");
			tmp_int = (int) (rvalue->dval);
			memcpy( *(lvalue->pp_ival), &tmp_int, sizeof(double));
		}else if(lvalue->type == PP_DVAL){
			memcpy( *(lvalue->pp_dval), &(rvalue->dval), sizeof(double));
		}else if(lvalue->type == PP_STR){  // MISMATCH
			sprintf( tmp_str , "%f" , rvalue->dval);
			tmp_s_str = string_new(tmp_str);
			string_free(*(lvalue->pp_str));
			*(lvalue->pp_str) = tmp_s_str;
        }else if(lvalue->type = NULL_ITEM){
            left_record = (ptr_record*)lvalue->p_record;
            if(left_record->type == PTR_INT){ // MISMATCH
		    	printf("CAUTION: double is caseted into int.\n");
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
			    tmp_int = (int) (rvalue->dval);
                *((int*)left_record->address) = tmp_int;
            }else if(left_record->type == PTR_DBL){
                // left record->type continues to be PTR_DBL.
                left_record->gc = GC_YES;
                // assign value to the newly allocated memory.
                *((double*)left_record->address) = rvalue->dval;
            }else{
                printf("lvalue type on stack is null.");
                printf("Mismatch rvalue with lvalue on ptr_table.\n");
            }
		}else{
			printf("unintended lvalue type for right DVAL.\n");
			return 0;
		}
		break;

		case PP_STR:
		if(lvalue->type == PP_STR){
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
        }else if(lvalue->type == NULL_ITEM){
            left_record = (ptr_record*)lvalue->p_record;
            if(left_record->type == PTR_STR){
                left_record->gc = GC_YES;
                // assign address of right string to the ptr_record.
                left_record->address = *(rvalue->pp_str);   
            }else{
                printf("lvalue type on stack is null.");
                printf("Mismatch rvalue with lvalue on ptr_table.\n");
            }
		}else{
			printf("unintended lvalue type for right PP_STR.\n");
		}
		break;
	}

//	printf("%d\n", vmstack->sp);
	vmstack->sp = ((vmstack->sp) - 2);
//	printf("%d\n ", vmstack->sp);
	return 1; 

}
