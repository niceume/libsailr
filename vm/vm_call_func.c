#include "vm_call_func.h"
#include "func/c_functions/c_funcs.h"
#include "vm_stack.h"
#include <string.h>
#include <stdio.h>

#define FUNC_NAME_IS( a , b ) ( strcmp( ( a ) , ( b ) ) ==  0 )

int
call_func( vm_stack* vmstack, char* fname, int num_args )
{
	if( FUNC_NAME_IS(fname, "print")){
		sailr_c_func_print(vmstack, num_args);
	}else{
		printf("Corresponding C functions cannot be found. Try to choose from Lua functions \n");
		printf("Lua function is not implemented yet. \n");
	}
}
