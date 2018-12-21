#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vm_calc.h"
#include "common_string.h"
#include "../ptr_table.h"


// Helper functions --------------
int
int_mul( int x, int y)
{
	return (x * y);
}

double
dbl_mul( double x, double y)
{
	return (x * y) ;
}

int
int_sub( int x, int y)
{
	return (x - y);
}

double
dbl_sub( double x, double y)
{
	return (x - y) ;
}

int
int_pow( int x, int y)
{
	double result = pow((double)x, (double)y);
	// TODO Check range of int ?
	int int_result = (int) result;
	return int_result;
}

double
dbl_pow( double x, double y)
{
	double result = pow(x, y);
	// TODO Check range of double ?
	return result;
}

int
int_mod( int x, int y)
{
	return ( x % y );
}

double
dbl_mod( double x, double y)
{
	double result = modf( x , &y );
	return  result;
}

int
divisible_int_div( int x, int y )
{
	int result = x / y;
	return result;
}

double
int_div( int x, int y )
{
	double result;
	printf("x: %lf\n", (double) x );
	printf("y: %lf\n", (double) y );
	result = ((double) x) / ((double) y);
	printf("%lf\n",result);
	return result;
}

double
dbl_div( double x, double y )
{
	double result = x / y;
	return result;
}

int
int_factorial( int x )
{
	if( x < 0 )
		return -1;

	int result;
	if( x == 1){
		result = 1;
	}else{
		result = x * int_factorial( x - 1 );
	}
	// TODO Check range of int ?
	return result ;
}


bool
item_is_num (stack_item* item)
{
	if((item->type == IVAL) || (item->type == DVAL) || (item->type == PP_IVAL) || (item->type == PP_DVAL) )
		return true;
	else
		return false;
}

bool
item_is_str (stack_item* item)
{
	if( item->type == PP_STR) 
		return true;
	else
		return false;
}

bool
item_is_bool (stack_item* item)
{
	if( item->type == BOOLEAN) 
		return true;
	else
		return false;
}

// -------------------------------


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
                }else if(null_record->type == PTR_DBL){
			        item->type = DVAL;
			        double tmp_double = *((double*)(null_record->address)) ;
			        item->dval = tmp_double;
                }else if(null_record->type == PTR_STR){
			        item->type = PP_STR;
			        string_object* tmp_str = ((string_object*)(null_record->address)) ;
			        item->pp_str = &tmp_str;
                }else{
                    printf("ERROR: NULL_ITEM points to a ptr_record with unintended type: %s", null_record->key );
                }
            }
		default:
			printf("THis item is not PP_IVAL, PP_DVAL, IVAL or DVAL. \n");
			return 0;
	}
	return 1;
}


int
vm_calc_addx(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	stack_item_pp2value( top_item );
	stack_item_pp2value( sec_item );

	string_object* str1;
	string_object* str2;
	string_object** new_pp_str;

	int result_ival;
	double result_dval;
	// Add calcuation
	// For ivals
	if((top_item->type == IVAL) && (sec_item->type == IVAL)){
		result_ival = sec_item->ival + top_item->ival ;
		vmstack->sp = vmstack->sp - 1; 
		sec_item->type = IVAL;
		sec_item->ival = result_ival;
	} else {
		// For dvals
		if((top_item->type == IVAL) && (sec_item->type == DVAL)){
			result_dval = sec_item->dval + (double)top_item->ival ;
		}else if((top_item->type == DVAL) && (sec_item->type == IVAL)){
			result_dval = (double)sec_item->ival + top_item->dval ;
		}else if((top_item->type == DVAL) && (sec_item->type == DVAL)){
			result_dval = sec_item->dval + top_item->dval ;
		}else if((top_item->type == PP_STR ) && (sec_item->type == PP_STR )){
			printf("Pointer to pointer: %p \n", sec_item->pp_str);
			str1 = *(sec_item->pp_str);
			str2 = *(top_item->pp_str);
			new_pp_str =  string_ptr_concat( str1, str2 );
			sec_item->pp_str = new_pp_str;

			vmstack->sp = vmstack->sp - 1; 
			printf("Pointer : %p \n", new_pp_str);
			printf("Pointer to pointer: %p \n", sec_item->pp_str);
			printf("Pointer : %p \n", *(sec_item->pp_str));
			return 1;
		}else{
			printf("ADDX should be applied to 'num and num' or 'str and str' on stack.\n");
			return 0;
		}
		vmstack->sp = vmstack->sp - 1; 
		sec_item->type = DVAL;
		sec_item->dval = result_dval;
	}
	return 1;
}

#define DEFINE_BINARY_OPERATOR( INTFUNC , DBLFUNC ) \
({ \
	int sp = vmstack->sp; \
	stack_item* stack = vmstack->stack; \
	stack_item* top_item = vm_stack_top(vmstack); \
	stack_item* sec_item = vm_stack_second(vmstack); \
	stack_item_pp2value( top_item ); \
	stack_item_pp2value( sec_item ); \
 \
	int result_ival; \
	double result_dval; \
	/* OP calcuation*/ \
	/* For ivals*/ \
	if((top_item->type == IVAL) && (sec_item->type == IVAL)){ \
		result_ival = INTFUNC ( sec_item->ival , top_item->ival ) ; \
		vmstack->sp = vmstack->sp - 1;  \
		sec_item->type = IVAL; \
		sec_item->ival = result_ival; \
	} else { \
		/* For dvals */ \
		if((top_item->type == IVAL) && (sec_item->type == DVAL)){ \
			result_dval = DBLFUNC ( sec_item->dval , (double)top_item->ival ) ; \
		}else if((top_item->type == DVAL) && (sec_item->type == IVAL)){ \
			result_dval = DBLFUNC ( (double)sec_item->ival , top_item->dval ) ; \
		}else if((top_item->type == DVAL) && (sec_item->type == DVAL)){ \
			result_dval = DBLFUNC ( sec_item->dval , top_item->dval ) ; \
		}else{ \
			printf("This VM_CMD should be applied to num and num on stack.\n"); \
			return 0; \
		} \
		vmstack->sp = vmstack->sp - 1;  \
		sec_item->type = DVAL; \
		sec_item->dval = result_dval; \
	} \
	return 1; \
})

int
vm_calc_mulx(vm_stack* vmstack)
{
  DEFINE_BINARY_OPERATOR( int_mul, dbl_mul );
}

int
vm_calc_subx(vm_stack* vmstack)
{
  DEFINE_BINARY_OPERATOR( int_sub, dbl_sub );
}

int
vm_calc_powx(vm_stack* vmstack)
{
  DEFINE_BINARY_OPERATOR( int_pow, dbl_pow );
}

int
vm_calc_modx(vm_stack* vmstack)
{
  DEFINE_BINARY_OPERATOR( int_mod, dbl_mod );
}


int
vm_calc_divx(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	stack_item_pp2value( top_item );
	stack_item_pp2value( sec_item );

	int result_ival;
	double result_dval;
	// Add calcuation
	// For ivals
	if((top_item->type == IVAL) && (sec_item->type == IVAL)){
		if( sec_item->ival % top_item->ival == 0 ){
			printf("DIVISABLE \n ");
			result_ival = divisible_int_div( sec_item->ival , top_item->ival) ;
			vmstack->sp = vmstack->sp - 1; 
			sec_item->type = IVAL;
			sec_item->ival = result_ival;
		}else{
			printf("NOT DIVISABLE \n");
			printf("sec: %d\n", sec_item->ival);
			printf("top: %d\n", top_item->ival);

			result_dval = int_div( sec_item->ival, top_item->ival ) ;
			vmstack->sp = vmstack->sp - 1; 
			sec_item->type = DVAL;
			sec_item->dval = result_dval;
		}
	} else {
		// For dvals
		if((top_item->type == IVAL) && (sec_item->type == DVAL)){
			result_dval = dbl_div( sec_item->dval , (double)top_item->ival ) ;
		}else if((top_item->type == DVAL) && (sec_item->type == IVAL)){
			result_dval = dbl_div( (double)sec_item->ival , top_item->dval ) ;
		}else if((top_item->type == DVAL) && (sec_item->type == DVAL)){
			result_dval = dbl_div( sec_item->dval , top_item->dval ) ;
		}else{
			printf("DIVX should be applied to num and num on stack.\n");
			return 0;
		}
		vmstack->sp = vmstack->sp - 1; 
		sec_item->type = DVAL;
		sec_item->dval = result_dval;
	}
	return 1;
}

// factorial (sigle operator)
int
vm_calc_factorial(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item_pp2value( top_item );

	int result_ival;

	// Add calcuation
	// For ivals
	if(top_item->type == IVAL){
			result_ival = int_factorial( top_item->ival) ;
			vmstack->sp = vmstack->sp - 0; 
			top_item->type = IVAL;
			top_item->ival = result_ival;		
	} else if(top_item->type == DVAL) {
		// For dval
			result_ival = int_factorial( (int) top_item->dval );
			vmstack->sp = vmstack->sp - 0; 
			top_item->type = IVAL;
			top_item->ival = result_ival;		
	} else {
		printf("FACT should be applied to num and num on stack.\n");
		return 0;
	}
	return 1;
}

// Logical Calculations

int
vm_calc_and(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	bool result_bool;

	if( (top_item->type == BOOLEAN) && (sec_item->type == BOOLEAN)){
		result_bool = ( ( sec_item->boolean ) && ( top_item->boolean )) ;
		vmstack->sp = vmstack->sp - 1; 
		sec_item->type = BOOLEAN ;
		sec_item->boolean = result_bool;
	}else{
		printf("AND should be applied to boolean and boolean.\n");
		return 0;
	}
	return 1;
}

int
vm_calc_or(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	bool result_bool;

	if( (top_item->type == BOOLEAN) && (sec_item->type == BOOLEAN)){
		result_bool = ( ( sec_item->boolean ) || ( top_item->boolean )) ;
		vmstack->sp = vmstack->sp - 1; 
		sec_item->type = BOOLEAN ;
		sec_item->boolean = result_bool;
	}else{
		printf("AND should be applied to boolean and boolean.\n");
		return 0;
	}
	return 1;
}

int
vm_calc_eq(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	stack_item* sec_item = vm_stack_second(vmstack);
	bool result_bool;

	if( item_is_num(sec_item) && item_is_num(top_item)){
		stack_item_pp2value( top_item );
		stack_item_pp2value( sec_item );
		if((top_item->type == IVAL) && (sec_item->type == IVAL)){
			result_bool = ( sec_item->ival == top_item->ival ) ;
		}else if((top_item->type == IVAL) && (sec_item->type == DVAL)){
			result_bool = ( sec_item->dval == top_item->ival ) ;
		}else if((top_item->type == DVAL) && (sec_item->type == IVAL)){
			result_bool = ( sec_item->ival == top_item->dval ) ;
		}else if((top_item->type == DVAL) && (sec_item->type == DVAL)){
			result_bool = ( sec_item->dval == top_item->dval ) ;
		}
	}else if(item_is_str(sec_item) && item_is_str(top_item)){
		result_bool = string_compare( *(sec_item->pp_str), *(top_item->pp_str) ) ;
		result_bool;
	}else{
		printf("Types are invalied for VM_EQ command.\n");
		return 0;
	}
	vmstack->sp = vmstack->sp - 1; 
	sec_item->type = BOOLEAN ;
	sec_item->boolean = result_bool;
	return 1;
}


#define DEFINE_LOGICAL_OPERATOR( OP ) \
({ \
	int sp = vmstack->sp; \
	stack_item* stack = vmstack->stack; \
	stack_item* top_item = vm_stack_top(vmstack); \
	stack_item* sec_item = vm_stack_second(vmstack); \
	bool result_bool; \
 \
	if( item_is_num(sec_item) && item_is_num(top_item)){ \
		stack_item_pp2value( top_item ); \
		stack_item_pp2value( sec_item ); \
		if((top_item->type == IVAL) && (sec_item->type == IVAL)){ \
			result_bool = ( sec_item->ival OP top_item->ival ) ; \
		}else if((top_item->type == IVAL) && (sec_item->type == DVAL)){ \
			result_bool = ( sec_item->dval OP top_item->ival ) ; \
		}else if((top_item->type == DVAL) && (sec_item->type == IVAL)){ \
			result_bool = ( sec_item->ival OP top_item->dval ) ; \
		}else if((top_item->type == DVAL) && (sec_item->type == DVAL)){ \
			result_bool = ( sec_item->dval OP top_item->dval ) ; \
		} \
	}else if(item_is_str(sec_item) && item_is_str(top_item)){ \
		printf("String is not supported for OP calculation.\n"); \
		return 0; \
	}else{ \
		printf("Types are invalied for OP calculation.\n"); \
		return 0; \
	} \
	vmstack->sp = vmstack->sp - 1;  \
	sec_item->type = BOOLEAN ; \
	sec_item->boolean = result_bool; \
	return 1; \
})

int
vm_calc_neq(vm_stack* vmstack)
{
	DEFINE_LOGICAL_OPERATOR( != ) ;
}

int
vm_calc_gt(vm_stack* vmstack)
{
	DEFINE_LOGICAL_OPERATOR( > ) ;
}

int
vm_calc_lt(vm_stack* vmstack)
{
	DEFINE_LOGICAL_OPERATOR( < ) ;
}

int
vm_calc_ge(vm_stack* vmstack)
{
	DEFINE_LOGICAL_OPERATOR( >= ) ;
}

int
vm_calc_le(vm_stack* vmstack)
{
	DEFINE_LOGICAL_OPERATOR( <= ) ;
}


int
vm_calc_neg(vm_stack* vmstack)
{
	int sp = vmstack->sp;
	stack_item* stack = vmstack->stack;
	stack_item* top_item = vm_stack_top(vmstack);
	bool result_bool;

	if( item_is_bool(top_item)){
		result_bool = ( ! top_item->boolean );
		vmstack->sp = vmstack->sp - 0; 
		top_item->type = BOOLEAN ;
		top_item->boolean = result_bool;
	}else{
		printf("Type is invalied for VM_NEG command.\n");
		return 0;
	}
	return 1;
}












