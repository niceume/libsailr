#include <stdio.h>
#include <string.h>
#include "struct_string.h"
#include "node.h"
#include "vm_cmd.h"
#include "vm_label.h"
#include "gen_code_util.h"

#define NEW_STRUCT_STRING( STR ) new_struct_string( STR, strlen(STR) )

/* Helper Functions ---------------- */

vm_inst*
gen_code_stmt(vm_inst* code1, vm_inst* code2)  // OK
{
	vm_inst* code;
	code = vm_inst_list_cat( code1, code2 );
	return code;
}

vm_inst*
gen_code_stmt_nosib(vm_inst* code1)  // OK. Add VM_END instruction.
{
	vm_inst* code;
	code = code1;
	return code;
}

vm_inst* 
gen_code_int(TreeNode* nd)  // Terminal OK
{
	int ival = nd->e1.ival;
	vm_inst* code = new_vm_inst_push_ival( ival );
	return code; 
}

vm_inst*
gen_code_double(TreeNode* nd)  // Terminal OK
{
	double dval = nd->e1.dval;
	vm_inst* code = new_vm_inst_push_dval( dval );
	return code; 
}

vm_inst*
gen_code_str(TreeNode* nd )  // Terminal OK
{
	char* key = nd->e1.str_key;
	vm_inst* code = new_vm_inst_push_pp_str(key);
	return code; 
}

vm_inst* 
gen_code_ident(TreeNode* nd, ptr_table* table)  // Terminal OK
{
	vm_inst* code;
	char* id_name = nd->e1.id;
	ptr_record* record = ptr_table_find( &table, id_name );
	if(record->type == PTR_INT){
		code = new_vm_inst_push_pp_ival(id_name);
	} else if (record->type == PTR_DBL){
		code = new_vm_inst_push_pp_dval(id_name);
	} else if (record->type == PTR_STR){
		code = new_vm_inst_push_pp_str(id_name);
	} else {
		printf("Inappropriate type is specified for varialbe. \n");
	}
	return code; 
}

/*
struct_string*
gen_code_fcall(struct_string* func_ident, struct_string* args )
{
	struct_string* code;
    // ToDo: check whether function specified is available or not.
    // ToDo: check whether args are valid or not.
	struct_string* par_start = new_struct_string( "( " , "( " );
	struct_string* par_end   = new_struct_string( " )" ,  " )" );
	code = cat_struct_strings(4, func_ident, par_start, args, par_end );
	return code; 
}

struct_string*
gen_code_farg(struct_string* code1, struct_string* code2 ){
	struct_string* code;
	struct_string* comma = new_struct_string( ", " , ", " );
	code = cat_struct_strings(3, code1, comma, code2);
	return code;
}

struct_string*
gen_code_farg_nosib(struct_string* code1){
	struct_string* code;
	code = code1;
	return code;
}
*/


VM_CMD
convert_op(char* op_name)
{
	if ( strcmp( op_name, "PLUS") == 0 ) {
		return VM_ADDX;
	} else if ( strcmp( op_name, "SUB") == 0 ) {
		return VM_SUBX;
	} else if ( strcmp( op_name, "MULT") == 0 ) {
		return VM_MULX;
	} else if ( strcmp( op_name, "DIV") == 0 ) {
		return VM_DIVX;
	} else if ( strcmp( op_name, "MOD") == 0 ) {
		return VM_MODX;
	} else if ( strcmp( op_name, "POWER") == 0 ) {
		return VM_POWX;
	} else if ( strcmp( op_name, "FACTOR") == 0 ) {
		return VM_FAC;
	} else if ( strcmp( op_name, "AND") == 0 ) {
		return VM_AND;
	} else if ( strcmp( op_name, "OR") == 0 ) {
		return VM_OR;
	} else if ( strcmp( op_name, "EQ") == 0 ) {
		return VM_EQ;
	} else if ( strcmp( op_name, "NEQ") == 0 ) {
		return VM_NEQ;
	} else if ( strcmp( op_name, "GT") == 0 ) {
		return VM_GT;
	} else if ( strcmp( op_name, "LT") == 0 ) {
		return VM_LT;
	} else if ( strcmp( op_name, "GE") == 0 ) {
		return VM_GE;
	} else if ( strcmp( op_name, "LE") == 0 ) {
		return VM_LE;
	} else if ( strcmp( op_name, "NEG") == 0 ) {
		return VM_NEG;
	} else {
		printf("ERROR:node op has undefined oprator!!\n");
	}
}

vm_inst*
gen_code_op(VM_CMD cmd, vm_inst* code1, vm_inst* code2) // OK operator
{
	vm_inst* code;
	code = vm_inst_list_cat(code1, code2);

	vm_inst* op_code;
	op_code = new_vm_inst_command(cmd);

	code = vm_inst_list_cat(code, op_code);
	return code;
}

vm_inst*
gen_code_unitary_op(VM_CMD cmd, vm_inst* code1) // OK operator
{
	vm_inst* code;
	vm_inst* op_code;

	op_code = new_vm_inst_command(cmd);

	code = vm_inst_list_cat(code1, op_code);
	return code;
}

vm_inst*
gen_code_let(vm_inst* code1, vm_inst* code2){
	vm_inst* code;
	vm_inst* let_code;
	let_code = new_vm_inst_command(VM_STO);
	code = vm_inst_list_cat(code1, code2);
	code = vm_inst_list_cat(code, let_code);
	return code;
}


/* END Helper Functions ---------------- */



/* Main Logic */

vm_inst* gen_code(TreeNode* nd, ptr_table* table){
  vm_inst* tmp_code1 = NULL;
  vm_inst* tmp_code2 = NULL;
  vm_inst* tmp_code3 = NULL;
  vm_inst* nd_code = NULL;
  VM_CMD cmd = 0;

  switch (nd->type){
  case NODE_PRGM:
    tmp_code1 = gen_code(nd->e1.nd, table);
    vm_inst* termin_code = new_vm_inst_command(VM_END);
    nd_code = vm_inst_list_cat( tmp_code1 , termin_code );
	return nd_code;
	break;

  case NODE_STMT:
    tmp_code1 = gen_code(nd->e1.nd, table);
    if(nd->e3.sibling != NULL){
		tmp_code3 = gen_code(nd->e3.sibling, table);
		nd_code = gen_code_stmt(tmp_code1, tmp_code3);
	}else{
		nd_code = gen_code_stmt_nosib(tmp_code1);
	}
    return nd_code;
    break;

  case NODE_INT:  // terminal node
	nd_code = gen_code_int(nd);
    return nd_code;
    break;

  case NODE_DBL:  // terminal node
    nd_code = gen_code_double(nd);
    return nd_code;
    break;

  case NODE_STR:  // terminal node
    nd_code = gen_code_str(nd);
    return nd_code;
    break;

  case NODE_IDENT:  // terminal node
    nd_code = gen_code_ident(nd, table);
    return nd_code;
    break;

  case NODE_OP:
    cmd = convert_op(nd->e1.op);
    tmp_code2 = gen_code(nd->e2.nd, table);
    tmp_code3 = gen_code(nd->e3.nd, table);
    nd_code = gen_code_op( cmd , tmp_code2, tmp_code3 );
	return nd_code;
    break;

  case NODE_UNIOP:
    cmd = convert_op(nd->e1.op);
    tmp_code2 = gen_code(nd->e2.nd, table);
    nd_code = gen_code_unitary_op( cmd , tmp_code2 );
	return nd_code;
	break;

  case NODE_LET:
    tmp_code1 = gen_code(nd->e1.nd, table);
    tmp_code2 = gen_code(nd->e2.nd, table);
    nd_code = gen_code_let( tmp_code1, tmp_code2 );
	return nd_code;
    break;


/*
  case NODE_FCALL:
    tmp_code1 = gen_code( nd->e1.nd , table) ;
	if(nd->e3.sibling != NULL){
      tmp_code3 = gen_code(nd->e3.sibling, table);
    } else {
      tmp_code3 = new_struct_string("",0);
    }
    nd_code = gen_code_fcall( tmp_code1, tmp_code3);
    return nd_code;
    break;

  case NODE_FARG:
	tmp_code1 = gen_code(nd->e1.nd, table);

    if(nd->e3.sibling != NULL){
		tmp_code3 = gen_code_farg(nd->e3.sibling);
		nd_code = gen_code_farg(tmp_code1, tmp_code3);
	} else {
		nd_code = gen_code_farg_nosib(tmp_code1);
	}
	return nd_code;
	break;
*/


  case NODE_IF:
    tmp_code1 = gen_code(nd->e1.nd, table);
    nd_code = tmp_code1;

	char* label_L1 = new_vm_label();
	char* label_L2 = new_vm_label();

	// fjmp L1
	// Always generate code
	nd_code = vm_inst_list_cat( nd_code, new_vm_inst_fjmp(label_L1));

	// Code for "then node"
	if(nd->e2.nd != NULL){
	    tmp_code2 = gen_code (nd->e2.nd, table);
		nd_code = vm_inst_list_cat( nd_code, tmp_code2 );
	}

	// If e3 exists. jmp L2
	if(nd->e3.nd != NULL){ 
	    nd_code = vm_inst_list_cat( nd_code, new_vm_inst_jmp(label_L2) );
	}

	// label L1
	// Always generate 
	nd_code = vm_inst_list_cat( nd_code, new_vm_inst_label(label_L1));

	// If e3 exists
	// Code for "else node"
	if(nd->e3.nd != NULL){
	    tmp_code3 = gen_code (nd->e3.nd, table);
		nd_code = vm_inst_list_cat( nd_code, tmp_code3 );
	}

	// If e3 exists
	if(nd->e3.nd != NULL){ // If e3 exists.
	    nd_code = vm_inst_list_cat( nd_code, new_vm_inst_label(label_L2));
	}

	return nd_code;
    break;

  }
}

