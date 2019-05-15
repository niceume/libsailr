#include "ptr_table.h"
#include <stdio.h>
#include <string.h>
#include "common_string.h"
#include "simple_re.h"

static int str_counter = 0;
static int rexp_counter = 0;

ptr_table*
ptr_table_init (){
	ptr_table *table = NULL;
	ptr_record* new_ptr_record;
	new_ptr_record = (ptr_record *)malloc(sizeof(ptr_record));
	strncpy( new_ptr_record->key , "_HEAD_OF_UTHASH_", MAX_KEY_LEN) ; 
	new_ptr_record->address = (void*) NULL;
	new_ptr_record->type = PTR_NULL;
	new_ptr_record->gc = GC_NO;
	new_ptr_record->ex_addr = (void*) NULL;
	new_ptr_record->ex_type = PTR_NULL;
	new_ptr_record->ex_gc = GC_NO;
	ptr_table_insert(&table, new_ptr_record );
	return table;
}

// type specified should be PTR_INT, PTR_DBL, PTR_STR or PTR_NULL
ptr_record*
ptr_table_add (ptr_table** table, char* key, void** address, PtrType type, GCReq gc )
{
	ptr_record * result = NULL;
	result = ptr_table_find (table, key);
	if(result == NULL){
		// Create new key/value.
		ptr_record* new_ptr_record;
		new_ptr_record = (ptr_record *)malloc(sizeof(ptr_record));
		strncpy( new_ptr_record->key , key, MAX_KEY_LEN ) ;
        if(type != PTR_NULL){
    		new_ptr_record->address = *address;
        }else{
            new_ptr_record->address = NULL;
        }
		new_ptr_record->type = type;
		new_ptr_record->gc = gc ; 
		// printf("%s\t %p\t \n", new_ptr_record->key, new_ptr_record->address);
		ptr_table_insert( table, new_ptr_record );
		// printf("%s\t %p\t \n", table->key, table->address);
		result = new_ptr_record;
	} else {
        if(type != PTR_NULL){
    		ptr_record_update( result, *address, type, gc );
        }else{
    		ptr_record_update( result, NULL, type, gc );
        }

	}
	return result;
}

ptr_record*
ptr_table_create_int(ptr_table** table, char* key, int ival)
{
	ptr_record* result = NULL;
	int* new = (int*)malloc(sizeof(int));
	*new = ival;
	result = ptr_table_add(table, key, (void**) &new, PTR_INT, GC_YES);
	return result;
}

ptr_record*
ptr_table_create_int_from_ptr(ptr_table** table, char* key, int** iptr, double** dptr)
{
	ptr_record* result = NULL;
	result = ptr_table_add(table, key, (void**) iptr, PTR_INT, GC_NO);
	ptr_record_update_extra_address(result, (void**) dptr, PTR_DBL, GC_NO);
	return result;
}

int
ptr_table_update_int(ptr_table** table, char* key, int ival)
{
	ptr_record* result = ptr_table_find(table, key);
	memcpy((int*) result->address , &ival, sizeof(int));
	return 1;
}

string_object*
ptr_table_get_pp_string(ptr_table** table, char* key)
{
  ptr_record* result = ptr_table_find(table, key);
  void* ptr_address = result->address;
  return ptr_address ;
}

const char*
ptr_table_read_string(ptr_table** table, char* key)
{
  ptr_record* result = ptr_table_find(table, key);
  void* ptr_address = result->address;
//  printf("The pointer referred is : %p", ptr_address);
  return string_read ((string_object*) ptr_address);
}

ptr_record*
ptr_table_create_double(ptr_table** table, char* key, double dval)
{
	ptr_record* result = NULL;
	double* new = (double*)malloc(sizeof(double));
	*new = dval;
	result = ptr_table_add(table, key, (void**) &new, PTR_DBL, GC_YES);
	return result;
}

ptr_record*
ptr_table_create_double_from_ptr(ptr_table** table, char* key, double** dptr, int** iptr)
{
	ptr_record* result = NULL;
	result = ptr_table_add(table, key, (void**) dptr, PTR_DBL, GC_NO);
	ptr_record_update_extra_address(result, (void**) iptr, PTR_INT, GC_NO);
	return result;
}

int
ptr_table_update_double(ptr_table** table, char* key, double dval)
{
	ptr_record* result = ptr_table_find(table, key);
	memcpy((double*) result->address , &dval, sizeof(double));
	return 1;
}

int
ptr_record_update_extra_address(ptr_record* pr, void** ptr_ex_addr, PtrType ex_type, GCReq ex_gc )
{
	pr->ex_addr = *ptr_ex_addr;
	pr->ex_type = ex_type;
	pr->ex_gc = ex_gc;
	return 1;
}

int
ptr_record_swap_addresses(ptr_record* pr)
{
	void* temp_address;
	PtrType temp_type;
	GCReq temp_gc;

	temp_address = pr->address;
	temp_type = pr->type;
	temp_gc = pr->gc;

	pr->address = pr->ex_addr;
	pr->type = pr->ex_type;
	pr->gc = pr->ex_gc;

	pr->ex_addr = temp_address;
	pr->ex_type = temp_type;
	pr->ex_gc = temp_gc;
	return 1;
}


char*
create_new_str_key(){
	char* new_str = (char *)malloc(sizeof(char)*16);
	str_counter = str_counter + 1 ;
    char* prefix = (char*) "STR%012d";
	sprintf(new_str, prefix , str_counter);
	return new_str ; 
}

char*
create_new_rexp_key(){
	char* new_str = (char *)malloc(sizeof(char)*16);
	rexp_counter = rexp_counter + 1 ;
    char* prefix = (char*) "REXP%011d";
	sprintf(new_str, prefix , rexp_counter);
	return new_str ; 
}


ptr_record*
ptr_table_create_anonym_string(ptr_table** table, string_object** strptr)
{
	char* new_key;
	new_key = create_new_str_key();
	ptr_record* new_ptr_record;
	new_ptr_record = ptr_table_add(table, new_key, (void**)strptr, PTR_STR, GC_YES);
	return new_ptr_record ;
}

ptr_record*
ptr_table_create_string(ptr_table** table, char* key, string_object** strptr)
{
	ptr_record* new_ptr_record;
	new_ptr_record = ptr_table_add(table, key, (void**)strptr, PTR_STR, GC_NO);
	return new_ptr_record ;
}

ptr_record*
ptr_table_create_string_from_ptr(ptr_table** table, char* key, string_object** strptr)
{
	ptr_record* result = ptr_table_create_string(table, key, strptr);
	return result;
}

int
ptr_table_update_string(ptr_table** table, char* key, string_object** strptr)
{
    ptr_record* to_be_updated = ptr_table_find(table, key);
    if(to_be_updated->type != PTR_STR)
        printf("ERROR: Record with non-string is trying to be updated with string.");

    if(to_be_updated->gc == GC_YES)
        free(to_be_updated->address);
    
    to_be_updated->address = *strptr;
}

int
ptr_record_update_string(ptr_record* pr , string_object** pp_str, GCReq gc)
{
	pr->address = *pp_str;
	pr->gc = gc;
}

ptr_record*
ptr_table_create_anonym_rexp(ptr_table** table, const char* pattern, const char* enc)
{
	char* new_key;
	new_key = create_new_rexp_key();
	simple_re* new_re;
	new_re = simple_re_compile( pattern, enc );
	ptr_record* new_ptr_record;
	new_ptr_record = ptr_table_add(table, new_key, (void**) &new_re, PTR_REXP, GC_YES);
	return new_ptr_record ;
}

ptr_record*
ptr_table_create_null(ptr_table** table, char* key)
{
	ptr_record* result = NULL;
    void** ppv = NULL;
	result = ptr_table_add(table, key, ppv, PTR_NULL, GC_NO);
	return result;
}

int
ptr_table_del_record(ptr_table** table, char* key)
{
	ptr_record* to_be_deleted = ptr_table_find(table, key);
	if (to_be_deleted != NULL){
		HASH_DEL( *table, to_be_deleted);  /* Just removed from hash. (Structure exists) */
		ptr_record_free(to_be_deleted);  /* Free structure and momory pointed by address */
		return 1;
	} else {
		printf("Cannot find record to be deleted.\n");
		return 0;
	}
}

int
ptr_record_free_gc_required_memory(ptr_record* pr)
{
//	ptr_record_show(pr);
	if(pr->gc == GC_YES){
		switch( pr->type ){	
			case PTR_INT:
			case PTR_DBL:
				free(pr->address);
				break;
			case PTR_STR:
				string_free((string_object*)pr->address);
				break;
			case PTR_REXP:
				simple_re_free((simple_re*)pr->address);
				break;
			case PTR_NULL:
				// Nothing to be freed
				break;
			default:
				free(pr->address);
				break;
		}
		pr->address = NULL;
	}
}

int
ptr_record_free_struct(ptr_record* pr)
{
	free(pr);
}

int
ptr_record_free(ptr_record* pr)
{
	ptr_record_free_gc_required_memory(pr);  /* Free memory pointed by address. */
	ptr_record_free_struct(pr); /* Free this record itself. */
}

PtrType
ptr_table_get_type(ptr_table** table, char* key)
{
	ptr_record* temp = ptr_table_find(table, key);
	return temp->type;
}

int
ptr_record_is_ptr_null(ptr_table** table, char* key)
{
	PtrType ptrtype = ptr_table_get_type(table, key);
	if(ptrtype == PTR_NULL){
		return 1;
	} else {
		return 0;
	}
}

void**
ptr_table_get_pptr(ptr_table** table, char* key)
{
	ptr_record* temp = ptr_table_find(table, key);
	void** temp_pptr = (void**) &(temp->address);
	return temp_pptr;
}

int
ptr_table_del_records_except(ptr_table** table, char** keys, int key_num )
{
	/* keys is array of pointers to chars. */
	ptr_record *current_record;
	ptr_record *temp_record;
	char* current_record_key;
	char* key_name;
	int idx;
	int matched;

	for( idx = 0; idx < key_num ; ++idx ){
		key_name = (char*) keys[idx];
		printf("* %s\n", key_name);
	}

	for(current_record = *table; current_record != NULL; current_record=temp_record) {
		current_record_key = current_record->key;
		temp_record = current_record->hh.next;
		matched = 0;
		for( idx = 0; idx < key_num ; ++idx ){
			key_name = (char*) keys[idx];
			if(strcmp( current_record_key , key_name ) == 0){ /*matched*/
				matched = 1;
			}
		}
		if(matched == 1){
			/* Don't delete the curent record. */
		}else{
			/* Delete the current record. */
    		HASH_DEL( *table , current_record);  /* Just removed from hash. (Structure exists) */
			ptr_record_free( current_record );  /* Free structure and momory pointed by address */
		}
	}
}

int
ptr_table_del_all(ptr_table** table)
{
  ptr_record *current_record;
  ptr_record *temp_record;

  for(current_record = *table; current_record != NULL; current_record=temp_record) {
    temp_record = current_record->hh.next;
    HASH_DEL( *table , current_record);  /* Just removed from hash. (Structure exists) */
	ptr_record_free( current_record );  /* Free structure and momory pointed by address */
  }
}

void
ptr_table_show_all(ptr_table** table)
{
    ptr_record *pr;
    for( pr = *table ; pr != NULL; pr=(ptr_record *)pr->hh.next) {
		ptr_record_show(pr);
    }
}

void
ptr_record_show(ptr_record* pr)
{
		if(pr->type == PTR_INT){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%d\t (ADR:%p\t TYPE:%d\t GC:%d\t VAL:%lf) \n", 
			pr->key, pr->address, pr->type, pr->gc, *((int*)(pr->address)),
			pr->ex_addr, pr->ex_type, pr->ex_gc, *((double*)(pr->ex_addr)) );
		}else if(pr->type == PTR_DBL){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%lf\t (ADR:%p\t TYPE:%d\t GC:%d\t VAL:%d)\n", 
			pr->key, pr->address, pr->type, pr->gc, *((double*)(pr->address)),
			pr->ex_addr, pr->ex_type, pr->ex_gc, *((int*)(pr->ex_addr)));
		}else if(pr->type == PTR_STR){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%s\t (ADR:%p --Extra space is not used for string--)\n", 
			pr->key, pr->address, pr->type, pr->gc, string_read((string_object*)(pr->address)),
			pr->ex_addr );
		}else if(pr->type == PTR_REXP){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%s\t (ADR:%p)\n", 
			pr->key, pr->address, pr->type, pr->gc, simple_re_read_pattern((simple_re*)(pr->address)),
			pr->ex_addr );
		}else if(pr->type == PTR_NULL){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t (ADR:%p) \n", 
			pr->key, pr->address, pr->type, pr->gc,
			pr->ex_addr);
		}else{
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t (ADR:%p) \n", 
			pr->key, pr->address, pr->type, pr->gc,
			pr->ex_addr);
		}
}



// private
ptr_record*
ptr_table_find(ptr_table** table, char* key)
{
	ptr_record* temp;
	HASH_FIND_STR(*table, key, temp);
	return temp;
}

ptr_record*
ptr_table_insert(ptr_table** table, ptr_record* new_ptr_record)
{
	HASH_ADD_STR(*table, key , new_ptr_record );
	return new_ptr_record;
}

int
ptr_record_update(ptr_record* pr, void* address, PtrType type, GCReq gc )
{
	pr->address = address;
	pr->type = type;
	pr->gc = gc;
	return 1;
}

/*
int main(int argc, char** argv){
	struct _person {
		char* name;
		int age;
	};
	typedef struct _person person;

	person* ind1 = (person*)malloc(sizeof(person));
	ind1->name = "toshi";
	ind1->age = 35;
	struct_string* hello = new_struct_string( "Hello", strlen("Hello"));

	ptr_table* table;
	table = ptr_table_init();
	// printf("table\t %p \n");
	
	ptr_table_add(&table, ind1->name, (void**)&ind1, PTR_OBJ, GC_YES);
	printf("toshi\t %p \n", ind1);
	// printf("table\t %p \n");

	ptr_table_add_anonym_str(&table, &hello);
	printf("hello\t %p \n", hello);
	// printf("table\t %p \n");

	printf("\n");
	ptr_table_show_all(&table);

	ptr_table_del_record(&table, "toshi");
	printf("\n");
	ptr_table_show_all(&table);

	ptr_table_del_record(&table, "STR000000000001");
	printf("\n");
	ptr_table_show_all(&table);	
}
*/



