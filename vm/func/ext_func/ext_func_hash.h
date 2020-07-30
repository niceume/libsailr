#ifndef EXT_FUNC_HASH_H
#define EXT_FUNC_HASH_H

#include "uthash.h"
#include "vm/func/c_func/c_func_helper.h"
#include "vm/vm_stack.h"

#define MAX_FUNC_NAME_LEN 511

struct _ext_func_hash {
	char fname[MAX_FUNC_NAME_LEN];
	unsigned int num_args;
	int (* func)(arg_list*, unsigned int, vm_stack*);
	UT_hash_handle hh; /* This macro makes this structure hashable */
};
typedef struct _ext_func_hash ext_func_elem;
typedef ext_func_elem ext_func_hash;


ext_func_hash* ext_func_hash_init();
ext_func_elem* ext_func_hash_find (ext_func_hash** hash, const char* fname);
ext_func_elem* ext_func_hash_add( ext_func_hash** hash, const char* fname, unsigned int num_args, int (* func)(arg_list*, unsigned int, vm_stack*) );
void ext_func_hash_free( ext_func_hash** hash );

int ext_func_elem_apply( ext_func_elem* elem, vm_stack* vmstack);

// Private
ext_func_elem* ext_func_hash_insert (ext_func_hash** hash, ext_func_elem* new_elem );


#endif // EXT_FUNC_HASH_H
