#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "var_hash.h"

var_hash*
var_hash_init ()
{
	var_hash* hash = NULL;
	var_elem* new_elem ;
	new_elem = (var_elem*) malloc(sizeof(var_elem));
	strncpy( new_elem->name , "_HEAD_OF_UTHASH_", MAX_KEY_LEN - 1) ; 
	new_elem->dummy = 1;
	
	var_hash_insert(&hash, new_elem);
	return hash;
}

var_elem*
var_hash_add_name (var_hash** hash, char* name)
{
	if (strlen(name) >= MAX_KEY_LEN - 1){
		printf("ERROR: Variable name is too long: %s \n", name);
	}

	var_elem* result = NULL;
	result = var_hash_find(hash, name);
	if(result == NULL){
		var_elem* new_elem = (var_elem*) malloc(sizeof(var_elem));
		strncpy( new_elem->name, name, MAX_KEY_LEN - 1) ;
		new_elem->dummy = 0;
		var_hash_insert(hash, new_elem);
		printf("%s added", new_elem->name);
		return new_elem;
	}else{
		return result;
	}
}

var_elem*
var_hash_insert (var_hash** hash, var_elem* new_elem )
{
	HASH_ADD_STR(*hash, name, new_elem );
	return new_elem ;
}

var_elem*
var_hash_find (var_hash** hash, char* name)
{
	var_elem* temp;
	HASH_FIND_STR(*hash, name, temp);
	return temp;
}

int
var_hash_size (var_hash** hash)
{
	int size = HASH_COUNT( *hash );
	return ( size - 1 );  // -1 because there's one dummy element.
}

char**
var_hash_names(var_hash** hash)
{
	int hash_size = var_hash_size(hash);
	printf("hash size: %d\n", hash_size);
	char** hash_names = (char**) malloc(hash_size * sizeof(char*));

	int idx = 0; 
	var_elem* elem;
	for( elem = *hash ; elem != NULL; elem = elem->hh.next) {
		if( (elem->dummy) != 1 ){
			char* new_str = (char*) malloc(sizeof(char) * MAX_KEY_LEN);
			strncpy( new_str, elem->name , MAX_KEY_LEN - 1);
        	hash_names[idx] = new_str ;
			idx = idx + 1;
		}
		if ( idx > hash_size ){
			printf("ERROR: hash size and real hash size mismatch.\n");
		}
    }
	return hash_names;
}

void var_hash_print_names(var_hash** hash)
{
	printf("Now printing....\n");
	char** names = var_hash_names(hash);
	int size = var_hash_size(hash);
	int idx;
	for(idx = 0; idx < size; ++idx){
		printf("%s \n", names[idx]);
	}
}

int
var_hash_free(var_hash** hash){
	var_elem* elem;
	var_elem* temp;

	HASH_ITER(hh, *hash, elem, temp) {
		printf("Delete %s\n", elem->name );
		free(elem);
	}
}

/*
int
main(int argc, char** argv)
{
	var_hash* hash = var_hash_init ();
	var_hash_add_name (&hash,  "Hello1");
	var_hash_add_name (&hash,  "Hello2");
	var_hash_print_names( &hash);
	var_hash_free(&hash);
}
*/
