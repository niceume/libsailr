#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm_code.h"

void
vm_inst_display( vm_inst* inst )
{
	char* cmd_str = vm_cmd_to_string(inst->cmd);
	switch(inst->cmd){
	case VM_PUSH_IVAL:
		printf("CMD:%s\t ARG:.ival=%d\n", cmd_str, inst->ival);
		break;
	case VM_PUSH_DVAL:
		printf("CMD:%s\t ARG:.dval=%f\n", cmd_str, inst->dval);
		break;
	case VM_PUSH_PP_IVAL:
	case VM_PUSH_PP_DVAL:
	case VM_PUSH_PP_STR:
		printf("CMD:%s\t ARG:.ptr_key=%s\n", cmd_str, inst->ptr_key);
		break;
	case VM_LABEL:
		printf("CMD:%s\t ARG:.label=%s\n", cmd_str, inst->label);
		break;
	case VM_JMP:
	case VM_FJMP:
		printf("CMD:%s\t ARG:.label=%s\n", cmd_str, inst->label);
		break;
	default:
		printf("CMD:%s\n", cmd_str);
		break;
	}
}

int
vm_inst_free( vm_inst* inst )
{
	free(inst);
}


int
vm_code_jmp(vm_code code, int idx, char* label, int max_line)
{
	vm_inst inst;
	int start_idx = idx ;
	for( idx = idx + 1 ; idx <= max_line ; ++idx ){
		inst = code[idx];
		if((inst.cmd == VM_LABEL) && (strcmp( inst.label, label) == 0 )){
			printf("%s\n", inst.label);
			printf("%s\n", label);
			return (idx - start_idx - 1);  // return num of steps to move forward. (idx will be incremented one more.)
		}
	}
	printf( "The label to jmp to could not be found in VM code.\n" );
	return -1 ;
}

// fjmp code is implemented at vm.c level.

void
vm_code_display_all( vm_code* code )
{

}
