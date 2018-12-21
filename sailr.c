#include "sailr.h"
#include "ptr_table.h"
#include "var_hash.h"
#include "parser_state.h"
#include "common_string.h"
#include "node.h"
#include "tree_dump.h"
#include "tree_free.h"
#include "gen_code.h"
#include "gen_code_util.h"
#include "vm_code.h"
#include "vm_stack.h"
#include "vm.h"
#include "y.tab.h"

ptr_table_object*
sailr_ptr_table_init()
{
	return (ptr_table_object*) ptr_table_init();
}

parser_state_object*
sailr_new_parser_state(char* fname, ptr_table_object* table)
{
	return (parser_state_object*) new_parser_state(fname, (ptr_table*) table);
}

int
sailr_construct_parser (const char* code , parser_state_object* ps)
{
	yy_scan_string(code);
	int result = yyparse((parser_state*) ps);
    yylex_destroy();
	return result;
}

void
sailr_tree_dump( parser_state_object* ps )
{
	tree_dump( ((parser_state*) ps)->tree, 0);
}

int
sailr_tree_free(parser_state_object* ps)
{
	tree_free( ((parser_state*) ps)->tree, 0);
}

vm_inst_object*
sailr_gen_code( parser_state_object* ps , ptr_table_object* table)
{
	vm_inst* result = gen_code( ((parser_state*) ps)->tree, (ptr_table*) table );
	return (vm_inst_object*) result; 
}

vm_stack_object*
sailr_vm_stack_init()
{
	return (vm_stack_object*) vm_stack_init();
}

vm_inst_object*
sailr_vm_inst_list_to_code( vm_inst_object* insts)
{
	return (vm_inst_object*) vm_inst_list_to_code((vm_inst_list*) insts);
}


int
sailr_vm_inst_list_size( vm_inst_object* insts)
{
	return vm_inst_list_size((vm_inst_list*) insts);
}

void
sailr_vm_inst_list_show( vm_inst_object* insts )
{
	vm_inst* inst = (vm_inst*) insts;
	do{
		vm_inst_display(inst);		
		inst = inst->next;
	}while(inst != NULL);
}

void
sailr_vm_exec_code( vm_inst_object* code , int num_insts , ptr_table_object* table , vm_stack_object* vmstack)
{
	vm_exec_code((vm_inst*)code, num_insts, (ptr_table*)table, (vm_stack*)vmstack);
}

ptr_record_object*
sailr_ptr_table_create_int_from_ptr(ptr_table_object** table, char* key, int** pp)
{
	return (ptr_record_object*) ptr_table_create_int_from_ptr((ptr_table**)table, key, pp);
}

ptr_record_object*
sailr_ptr_table_create_double_from_ptr(ptr_table_object** table, char* key, double** pp)
{
	return (ptr_record_object*) ptr_table_create_double_from_ptr((ptr_table**)table, key, pp);
}

ptr_record_object*
sailr_ptr_table_create_string_from_ptr(ptr_table_object** table, char* key, string_type_object** pp)
{
	return (ptr_record_object*) ptr_table_create_string_from_ptr((ptr_table**)table, key, (string_object**)pp);

}

ptr_record_object*
sailr_ptr_table_create_null(ptr_table_object** table, char* key)
{
	return (ptr_record_object*) ptr_table_create_null((ptr_table**)table, key);
}

ptr_record_object*
sailr_ptr_table_create_anonym_string(ptr_table_object** table, const char* str)
{
	string_object* new_str = string_new(str);
	return (ptr_record_object*) ptr_table_create_anonym_string((ptr_table**)table, &new_str);

}


string_type_object*
sailr_ptr_table_get_pp_string(ptr_table_object** table, char* key){
  return (string_type_object*) ptr_table_get_pp_string((ptr_table**)table, key);
}

const char*
sailr_ptr_table_get_char(ptr_table_object** table, char* key){
  return  ptr_table_get_char((ptr_table**)table, key);
}


string_type_object*
sailr_new_string(const char* str)
{
	string_object* new_str = string_new(str);
	return (string_type_object*) new_str;
}

int
sailr_ptr_table_update_int(ptr_table_object** table, char* key, int ival)
{
	return ptr_table_update_int((ptr_table**) table, key, ival);
}

int
sailr_ptr_table_update_double(ptr_table_object** table, char* key, double dval)
{
	return ptr_table_update_double((ptr_table**) table, key, dval);
}

int
sailr_ptr_table_update_string(ptr_table_object** table, char* key, string_type_object** str)
{
	return ptr_table_update_string((ptr_table**) table, key, (string_object**) str);
}

void
sailr_ptr_table_show_all(ptr_table_object** table)
{
	ptr_table_show_all((ptr_table**) table);
}

// Functions fro var_hash, varnames

char**
sailr_varnames(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_names(&(ps->vars));
}

char**
sailr_rhs_varnames(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_names(&(ps->rhsvars));
}

char**
sailr_lhs_varnames(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_names(&(ps->lhsvars));
}

int
sailr_varnames_num(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_size(&(ps->vars));
}

int
sailr_rhs_varnames_num(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_size(&(ps->rhsvars));
}

int
sailr_lhs_varnames_num(parser_state_object* psobj)
{
	parser_state* ps = (parser_state*) psobj;
	return var_hash_size(&(ps->lhsvars));
}


