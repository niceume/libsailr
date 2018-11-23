#ifndef SAILR_H
#define SAILR_H

typedef void ptr_table_object;
typedef void ptr_record_object;
typedef void parser_state_object;
typedef void vm_inst_object;
typedef void vm_stack_object;
typedef void string_type_object;
typedef void parser_state_object;

ptr_table_object*     sailr_ptr_table_init();
parser_state_object*  sailr_new_parser_state(char* , ptr_table_object* );
int                   sailr_construct_parser (const char* code , parser_state_object* ps);
void                  sailr_tree_dump( parser_state_object* ps ); 
int                   sailr_tree_free( parser_state_object* ps ); 
vm_inst_object*       sailr_gen_code( parser_state_object* ps , ptr_table_object*); // VM Code is generated.
vm_inst_object*       sailr_vm_inst_list_to_code( vm_inst_object* );
int                   sailr_vm_inst_list_size( vm_inst_object* );
void                  sailr_vm_inst_list_show( vm_inst_object* );
vm_stack_object*      sailr_vm_stack_init();
void                  sailr_vm_exec_code(vm_inst_object*, int, ptr_table_object*, vm_stack_object*);

ptr_record_object* sailr_ptr_table_create_int_from_ptr(ptr_table_object** table, char* key, int** pp);
ptr_record_object* sailr_ptr_table_create_double_from_ptr(ptr_table_object** table, char* key, double** pp);
ptr_record_object* sailr_ptr_table_create_string_from_ptr(ptr_table_object** table, char* key, string_type_object** pp);
ptr_record_object* sailr_ptr_table_create_anonym_string(ptr_table_object** table, const char* str);


int sailr_ptr_table_update_int(ptr_table_object** table, char* key, int ival);
int sailr_ptr_table_update_double(ptr_table_object** table, char* key, double dval);
int sailr_ptr_table_update_string(ptr_table_object** table, char* key, string_type_object** str);

void                  sailr_ptr_table_show_all(ptr_table_object** table);

char** sailr_varnames(parser_state_object* );
char** sailr_rhs_varnames(parser_state_object* );
char** sailr_lhs_varnames(parser_state_object* );
int sailr_varnames_num(parser_state_object* psobj);
int sailr_rhs_varnames_num(parser_state_object* psobj);
int sailr_lhs_varnames_num(parser_state_object* psobj);

#endif



