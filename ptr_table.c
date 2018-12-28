#include "ptr_table.h"
#include <stdio.h>
#include <string.h>
#include "common_string.h"

static int str_counter = 0;

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
    		ptr_table_update( result, *address, type, gc );
        }else{
    		ptr_table_update( result, NULL, type, gc );
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
  printf("The pointer referred is : %p", ptr_address);
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

/*
ptr_record*
ptr_table_create_anonym_struct_string(ptr_table** table, struct_string** str)
{
	char* new_key;
	new_key = create_new_str_key();
	ptr_record* new_ptr_record;
	new_ptr_record = ptr_table_add(table, new_key, (void**)str, PTR_STR, GC_YES);
	return new_ptr_record ;
}
*/

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
	return ptr_table_create_string(table, key, strptr);
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

ptr_record*
ptr_table_create_null(ptr_table** table, char* key)
{
	ptr_record* result = NULL;
    void** ppv = NULL;
	result = ptr_table_add(table, key, ppv, PTR_NULL, GC_NO);
    printf("void pointer \n");
	return result;
}

int
ptr_table_del(ptr_table** table, char* key)
{
	ptr_record* to_be_deleted = ptr_table_find(table, key);
	if (to_be_deleted != NULL){
		HASH_DEL( *table, to_be_deleted);
		ptr_record_free(to_be_deleted);
		return 1;
	} else {
		printf("Cannot find record to be deleted.\n");
		return 0;
	}
}

int
ptr_record_free(ptr_record* pr)
{
	if(pr->gc == GC_YES)
		free(pr->address);

	free(pr);
}

PtrType
ptr_table_get_type(ptr_table** table, char* key)
{
	ptr_record* temp = ptr_table_find(table, key);
	return temp->type;
}

PtrType
ptr_record_get_type(ptr_record* record)
{
	return record->type;
}

void**
ptr_table_get_pptr(ptr_table** table, char* key)
{
	ptr_record* temp = ptr_table_find(table, key);
	void** temp_pptr = (void**) &(temp->address);
	return temp_pptr;
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
ptr_table_update(ptr_record* pr, void* address, PtrType type, GCReq gc )
{
	pr->address = address;
	pr->type = type;
	pr->gc = gc;
	return 1;
}


void
ptr_table_show_all(ptr_table** table)
{
    ptr_record *pr;
    for( pr = *table ; pr != NULL; pr=(ptr_record *)pr->hh.next) {
		if(pr->type == PTR_INT){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%d\t (ADR:%p\t TYPE:%d\t GC:%d\t VAL:%lf) \n", 
			pr->key, pr->address, pr->type, pr->gc, *((int*)(pr->address)),
			pr->ex_addr, pr->ex_type, pr->ex_gc, *((double*)(pr->ex_addr)) );
		}else if(pr->type == PTR_DBL){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%lf\t (ADR:%p\t TYPE:%d\t GC:%d\t VAL:%d)\n", 
			pr->key, pr->address, pr->type, pr->gc, *((double*)(pr->address)),
			pr->ex_addr, pr->ex_type, pr->ex_gc, *((int*)(pr->ex_addr)));
		}else if(pr->type == PTR_STR){
        	printf("KEY:%s\t ADR:%p\t TYPE:%d\t GC:%d\t VAL:%s\t (ADR:%p)\n", 
			pr->key, pr->address, pr->type, pr->gc, string_read((string_object*)(pr->address)),
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

	ptr_table_del(&table, "toshi");
	printf("\n");
	ptr_table_show_all(&table);

	ptr_table_del(&table, "STR000000000001");
	printf("\n");
	ptr_table_show_all(&table);	
}
*/



