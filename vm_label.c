#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vm_label.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MAX_LABEL_DIGITS 10
#define LABEL_FORMAT "L%0" STR( MAX_LABEL_DIGITS ) "d"

static int vm_label_counter = 0;
static char* cur_vm_label;

char* new_vm_label(){
	char* new_label = (char *)malloc((MAX_LABEL_DIGITS + 2) * sizeof(char)); 

	vm_label_counter = vm_label_counter + 1 ; // increment label counter.

	if(vm_label_counter <= (pow(10, MAX_LABEL_DIGITS ) - 1)){
        char* label_format = LABEL_FORMAT ;
		// printf("%s \n", label_format );
		sprintf(new_label, label_format , vm_label_counter); 
		// No need to call new_label[ MAX_LABEL_DIGITS + 1] = '\0';
	}else{
		printf("Compiler exhausted counts for label creation. \n");
		exit(1);
	}
	char* prev_vm_label = cur_vm_label; 
    cur_vm_label = new_label;
	return new_label;
}

char* current_vm_label(){
    return cur_vm_label;
}

//void free_vm_label(){
//	free(cur_vm_label);
//}

/*
int main(int argc, char** argv){
	char* temp1;
	temp1 = new_vm_label();
	printf("%s => len: %d \n", temp1 , strlen(temp1) );
	printf("Current label: %s\n", current_vm_label());
	char* temp2;
	temp2 = new_vm_label();
	printf("%s => len: %d \n", temp2 , strlen(temp2) );	
	printf("Current label: %s\n", current_vm_label());
	char* temp3;
	temp3 = new_vm_label();
	printf("%s => len: %d \n", temp3 , strlen(temp3) );	
	printf("Current label: %s\n", current_vm_label());
	free_vm_label();
	return 0;
}
*/

