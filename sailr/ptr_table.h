// ptr_table stores address information for variables and strings. 
// Virtual stack machine does not directly manipulate these information on its stack.

#ifndef PTR_TABLE_H
#define PTR_TABLE_H

#include "uthash.h"
#include "common_string.h"

#define MAX_KEY_LEN 511

enum _PtrType{
	PTR_INT,
	PTR_DBL,
//	PTR_STRUCTSTR,
	PTR_STR,
//	PTR_OBJ,
	PTR_NULL
};
typedef enum _PtrType PtrType ;

enum _GCReq {
	GC_NO,
	GC_YES
};
typedef enum _GCReq GCReq ;
// GC_NO: 0, GC_YES: 1

struct _ptr_record {
	char key[ MAX_KEY_LEN ];  /* Usually this should be a variable name. For string, temporary name is generated. */
	void* address;  /* value1 */
	PtrType type; /* value2 */
	GCReq gc; /* value3: whether garbage collected. 0: GC_NO ,  1: GC_YES */
	UT_hash_handle hh; /* This macro makes this structure hashable */
};
typedef struct _ptr_record ptr_record ;
typedef ptr_record ptr_table;


ptr_table*  ptr_table_init();
ptr_record* ptr_table_add(ptr_table** table, char* key, void** address, PtrType type, GCReq gc);

ptr_record* ptr_table_create_int(ptr_table** table, char* key, int ival);
ptr_record* ptr_table_create_int_from_ptr(ptr_table** table, char* key, int** iptr);
int ptr_table_update_int(ptr_table** table, char* key, int ival);

ptr_record* ptr_table_create_double(ptr_table** table, char* key, double dval);
ptr_record* ptr_table_create_double_from_ptr(ptr_table** table, char* key, double** dptr);
int ptr_table_update_double(ptr_table** table, char* key, double dval);

/*
ptr_record* ptr_table_create_anonym_struct_string(ptr_table** table, struct_string** strptr);
*/

ptr_record* ptr_table_create_anonym_string(ptr_table** table, string_object** strptr);
ptr_record* ptr_table_create_string(ptr_table** table, char* key, string_object** strptr);
ptr_record* ptr_table_create_string_from_ptr(ptr_table** table, char* key, string_object** strptr);
int ptr_table_update_stirng(ptr_table**, char* key, string_object** strptr);

string_object* ptr_table_get_pp_string(ptr_table** table, char* key);
const char* ptr_table_get_char(ptr_table** table, char* key);

int ptr_table_update_string(ptr_table** , char* , string_object** );
ptr_record* ptr_table_create_null(ptr_table** table, char* key);
int ptr_table_del(ptr_table** table, char* key);
int ptr_record_free(ptr_record* );
int ptr_table_free(ptr_table**);
void ptr_table_show_all(ptr_table**);
PtrType ptr_table_type_of(ptr_table** table, char* key);


// private
ptr_record* ptr_table_find(ptr_table** table, char* key);
ptr_record* ptr_table_insert(ptr_table** table, ptr_record* pr);
int ptr_table_update(ptr_record* pr, void* address, PtrType type, GCReq gc);

#endif /* PTR_TABLE_H */
