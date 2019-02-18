#include "simple_re.h"
#include "common_string.h"
#include "vm_item_pp2val.h"
#include <stdio.h>

#include "../ptr_table.h"

int
stack_item_pp2value(stack_item* item)
{
    ptr_record* null_record ;
	switch(item->type){
		case PP_IVAL:
			item->type = IVAL;
			int tmp_int = **(item->pp_ival) ;
			item->ival = tmp_int;
			break;
		case PP_DVAL:
			item->type = DVAL;
			double tmp_double = **(item->pp_dval) ;
			item->dval = tmp_double;
			break;
		case IVAL:
			printf("This item is already a value.\n");
			break;
		case DVAL:
			printf("This item is already a value.\n");
			break;
        case NULL_ITEM:
            null_record = (ptr_record*) item->p_record;
            if(null_record->type == PTR_NULL){
                printf("ERROR: The variable, %s, should not be null. ", null_record->key );
                printf("Variable of null value cannot be used for calculation. \n");
            }else{
                if(null_record->type == PTR_INT){
			        item->type = IVAL;
			        int tmp_int = *((int*)(null_record->address)) ;
			        item->ival = tmp_int;
					printf("NULL_ITEM is converted to IVAL.\n");
                }else if(null_record->type == PTR_DBL){
			        item->type = DVAL;
			        double tmp_double = *((double*)(null_record->address)) ;
			        item->dval = tmp_double;
					printf("NULL_ITEM is converted to DVAL.\n");
                }else if(null_record->type == PTR_STR){
			        item->type = PP_STR;
			        string_object** tmp_pp_str = (string_object**) &( null_record->address) ;
			        item->pp_str = tmp_pp_str;
					printf("NULL_ITEM is converted to PP_STR.\n");
                }else if(null_record->type == PTR_REXP){
			        item->type = PP_REXP;
			        simple_re** tmp_pp_rexp = (simple_re**) &(null_record->address);
			        item->pp_rexp = tmp_pp_rexp;
					printf("NULL_ITEM is converted to PP_REXP.\n");
                }else{
                    printf("ERROR: NULL_ITEM points to a ptr_record with unintended type: %s", null_record->key );
                }
            }
			break;
		default:
			printf("THis item is not PP_IVAL, PP_DVAL, IVAL, DVAL or NULL_ITEM. Need not to be converted. \n");
			return 0;
	}
	return 1;
}

