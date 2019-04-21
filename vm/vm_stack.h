#ifndef VM_STACK_H
#define VM_STACK_H

#include <stdbool.h>

#include "vm_code.h"
#include "../ptr_table.h"
#include "../simple_re/simple_re.h"

// Stack type and name

#define VM_STACK_ITEM_NAME_TABLE \
	Y(IVAL, "IVAL") /* The stack is holding an int. */ \
	Y(DVAL, "DVAL") /* The stack is holding a double. */ \
	Y(BOOLEAN, "BOOLENAN") \
	Y(PP_IVAL, "PP_IVAL") \
	Y(PP_DVAL, "PP_DVAL") \
	Y(PP_STR, "PP_STR") \
	Y(PP_REXP, "PP_REXP") \
	Y(NULL_ITEM, "NULL_ITEM")

#define Y(a, b) a,
enum _ItemType {
  VM_STACK_ITEM_NAME_TABLE
};
#undef Y

typedef enum _ItemType ItemType;

char* display_item_type(ItemType type);


// Stack structure for VM

struct _stack_item {
	ItemType type ; 
	union {
		int ival;
		double dval;
		bool boolean;
		int** pp_ival;
		double** pp_dval;
		string_object** pp_str; 
		simple_re** pp_rexp;
		void* ptr;
	};
    ptr_record* p_record;
};
typedef struct _stack_item stack_item ;

#define MAXSTACKSIZE 1000

struct _vm_stack{
	int sp ; 
	stack_item stack[ MAXSTACKSIZE ];
};
typedef struct _vm_stack vm_stack;


// Function Prototypes 

// public
// int run_vm ( vm_stack* , vm_code* , ptr_table* ); Deprecated

// private
vm_stack* vm_stack_init();
int vm_stack_push_item( vm_stack* , stack_item* );
int vm_stack_push_ival( vm_stack* , int );
int vm_stack_push_dval( vm_stack* , double );
int vm_stack_push_pp_ival( vm_stack* , ptr_table**, char* );
int vm_stack_push_pp_dval( vm_stack* , ptr_table**, char* );
int vm_stack_push_pp_num( vm_stack* , ptr_table**, char* );
int vm_stack_push_pp_str( vm_stack* , ptr_table**, char* );
int vm_stack_push_pp_rexp( vm_stack* , ptr_table**, char* );
int vm_stack_push_null( vm_stack* , ptr_table**, char* );
int vm_stack_push_corresp_item( vm_stack* , ptr_table** , char* );

int vm_stack_fcall( vm_stack* , char* , int , ptr_table** );

stack_item* vm_stack_pop( vm_stack* );

void vm_stack_display_item( vm_stack*, int );
void vm_stack_display_all( vm_stack* );
int vm_stack_end( vm_stack* );
int vm_stack_is_full( vm_stack* );
int vm_stack_is_empty( vm_stack* );
int vm_stack_size( vm_stack* );
int vm_stack_free( vm_stack* );


stack_item* vm_stack_top( vm_stack*);
stack_item* vm_stack_second( vm_stack* );
stack_item* vm_stack_third( vm_stack* );
stack_item* vm_stack_nth( vm_stack*, int );

#endif

