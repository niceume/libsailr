#include "vm_call_func.h"
#include "func/c_func/c_func.h"
#include "vm_stack.h"
#include <string.h>
#include <stdio.h>

#define FUNC_NAME_IS( a , b ) ( strcmp( ( a ) , ( b ) ) ==  0 )

int
call_func( vm_stack* vmstack, char* fname, int num_args, ptr_table** table )
{
    if(FUNC_NAME_IS(fname, "print")){
        sailr_func_print(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "num_to_str")){
        sailr_func_num_to_str(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_strip")){
        sailr_func_str_strip(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_lstrip")){
        sailr_func_str_lstrip(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_rstrip")){
        sailr_func_str_rstrip(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_concat")){
        sailr_func_str_concat(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_repeat")){
        sailr_func_str_repeat(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_subset")){
        sailr_func_str_subset(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "str_to_num")){
        sailr_func_str_to_num(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "rexp_matched")){
        sailr_func_rexp_matched(vmstack, num_args, table);
    }else if(FUNC_NAME_IS(fname, "date_ymd")){
        sailr_func_date_ymd(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "date_ym_weekday_nth")){
        sailr_func_date_ym_weekday_nth(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "date_add_n_years")){
        sailr_func_date_add_n_years(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "date_add_n_months")){
        sailr_func_date_add_n_months(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "date_add_n_days")){
        sailr_func_date_add_n_days(vmstack, num_args);
    }else if(FUNC_NAME_IS(fname, "date_format")){
        sailr_func_date_format(vmstack, num_args, table);
	}else{
        printf("Function, %s , cannot be found. \n", fname );
    }
}
