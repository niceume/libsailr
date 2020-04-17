#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vm_label.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define CAT3_HELPER( x, y, z ) x ## y ## z 
#define CAT3( x, y, z)  CAT3_HELPER( x, y, z )

#define MAX_LABEL_DIGITS 10
#define LABEL_FORMAT CAT3( L%0 , MAX_LABEL_DIGITS , u )

static unsigned int vm_label_counter = 0;
static char* cur_vm_label;

char* new_vm_label(){
	char* new_label = (char *)malloc((MAX_LABEL_DIGITS + 2) * sizeof(char)); 

	vm_label_counter = vm_label_counter + 1 ; // increment label counter.

	if(vm_label_counter <= (pow(10, MAX_LABEL_DIGITS ) - 1)){
        char* label_format = STR( LABEL_FORMAT ) ;
//		printf("---------LABEL FORMAT-----------\n");
//		printf("%s\n", label_format);
//		printf("%s \n", label_format );
		snprintf(new_label, (MAX_LABEL_DIGITS + 2) * sizeof(char) , label_format, vm_label_counter); 
//		printf("%s\n", new_label);
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

