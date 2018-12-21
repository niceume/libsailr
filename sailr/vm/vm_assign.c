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

    if( lvalue->type == NULL_ITEM ){
    ptr_record* left_record = (ptr_record*)lvalue->p_record;
	switch (rvalue->type){
		case IVAL:
        if(left_record->type == PTR_NULL){
            // change type from PTR_NULL to PTR_INT on ptr_table.
            left_record->type = PTR_INT;
            // allocate memory for integer. ptr_record points to the address.
            left_record->address = malloc(sizeof(int));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            memcpy( left_record->address, &(rvalue->ival), sizeof(int));
        } else if (left_record->type == PTR_INT){
            // left_record->type continues to be PTR_INT.
            free(left_record->address);
            left_record->address = malloc(sizeof(int));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            memcpy( left_record->address, &(rvalue->ival), sizeof(int));
        } else if (left_record->type == PTR_DBL){
            // left_record->type continues to be PTR_DBL.
            free(left_record->address);
            left_record->address = malloc(sizeof(double));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            double temp_dbl_from_rvalue = (double)(rvalue->ival);
            memcpy( left_record->address, &(temp_dbl_from_rvalue), sizeof(double));            
        } else { // e.g. left_record->type == PTR_STR
            printf("ERROR: this type cannot be assigned to this type.");
        }
		break;
		case DVAL:
        if(left_record->type == PTR_NULL){
            // change type from PTR_NULL to PTR_DBL on ptr_table.
            left_record->type = PTR_DBL;
            // allocate memory for integer. ptr_record points to the address.
            left_record->address = malloc(sizeof(double));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            memcpy( left_record->address, &(rvalue->dval), sizeof(double));
        }else if (left_record->type == PTR_INT){
            // change type from PTR_INT to PTR_DBL on ptr_table.
            left_record->type = PTR_DBL;
            free(left_record->address);
            left_record->address = malloc(sizeof(double));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            memcpy( left_record->address, &(rvalue->dval), sizeof(double));
        } else if (left_record->type == PTR_DBL){
            // left record->type continues to be PTR_DBL.
            free(left_record->address);
            left_record->address = malloc(sizeof(double));
            left_record->gc = GC_YES;
            // assign value to the newly allocated memory.
            memcpy( left_record->address, &(rvalue->dval), sizeof(double));
        } else { // e.g. left_record->type == PTR_STR
            printf("ERROR: this type cannot be assigned to this type.");
        }
		break;
		case PP_STR:
        // change type from PTR_NULL to PTR_STR on ptr_table.
        left_record->type = PTR_STR;
        left_record->gc = GC_YES;
        // assign address of right string to the ptr_record.
        left_record->address = *(rvalue->pp_str);
		break;
    }
	vmstack->sp = ((vmstack->sp) - 2);
    return 1;
    }

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
			sprintf( tmp_str , "%f" , rvalue->dval);
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
			printf("unintended lvalue type.\n");
		}
		break;
	}

//	printf("%d\n", vmstack->sp);
	vmstack->sp = ((vmstack->sp) - 2);
//	printf("%d\n ", vmstack->sp);
	return 1; 

}
