#include "simple_re.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	
	simple_re* re = simple_re_compile("^(\\S)(.)+",  "UTF-8");

	printf("Compilation finished. \n");	

//	const char* string = "こんにちは World";
	const char* string = "Hello World";
	simple_re* ptr_to_store;
	int result = simple_re_match( re , string , &ptr_to_store);
	printf("Matching finished. \n");

	int num;
	char* str;
	int idx;
	if(result == 1 ){
		printf("success match found. \n");
		printf("Showing ... \n");
		
		num = simple_re_matched_group_num(ptr_to_store);
		for(idx = 0; idx < num ; ++idx){
			str = simple_re_matched_str( ptr_to_store , idx );
			if(idx == 0){
				printf("Whole Matched : %s \n", str );
			} else {
				printf("Matched Group %d : %s \n", idx, str );
			}
			free(str);
		}
	}else{
		printf("No match. %d \n", result);
	}
	simple_re_free(re);
}
