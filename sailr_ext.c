#include "sailr_ext.h"
#include "vm/func/c_func/c_func_helper.h"

// External Functions

ext_func_hash_object*
sailr_ext_func_hash_init()
{
  return (ext_func_hash_object*) ext_func_hash_init();
}

void
sailr_ext_func_hash_add(ext_func_hash_object** hash, const char* fname, unsigned int num_args, int (* func)(arg_list*, unsigned int, vm_stack*))
{
  ext_func_hash_add((ext_func_hash**)hash, fname, num_args, func);
}

void
sailr_ext_func_hash_free(ext_func_hash_object** hash)
{
  ext_func_hash_free((ext_func_hash**)hash);
}

