#include "sailr.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_arithmetic_test1( void );

void
test_arithmetic_add_tests(CU_pSuite testSuite)
{
	CU_add_test(testSuite, "test arithmetics", test_arithmetic_test1 );
}

void
test_arithmetic_test1( void )
{
	// Code 
	const char* code = " "
"pizza = 1 \n"
"size = 0.25 \n"
"pieces = pizza / size \n"
"friends = -1 + pizza / size \n"
" \n"
"factorial = 4! \n"
" \n"
"exp = 2 ^ 5 \n"
" \n"
"exp2 = 2 ** 5 \n"
" \n"
"exp3 = 2.2 ^ 3 \n"
" \n" ;

	// Parser Initialization
	ptr_table_object* table = sailr_ptr_table_init() ;
	parser_state_object* ps = sailr_new_parser_state ("souce from string literal", table);
	sailr_construct_parser( code, ps ); 


	// Add variables
	sailr_ptr_table_create_null(&table, (char*)"pizza" );
	sailr_ptr_table_create_null(&table, (char*)"size" );
	sailr_ptr_table_create_null(&table, (char*)"pieces" );
	sailr_ptr_table_create_null(&table, (char*)"friends" );
	sailr_ptr_table_create_null(&table, (char*)"factorial" );
	sailr_ptr_table_create_null(&table, (char*)"exp" );
	sailr_ptr_table_create_null(&table, (char*)"exp2" );
	sailr_ptr_table_create_null(&table, (char*)"exp3" );

	// Creating virtual machine codes
	vm_inst_object* inst_list = sailr_gen_code( ps, table); // VM Code is generated.
	vm_inst_object* vmcode = sailr_vm_inst_list_to_code(inst_list);
	int vmcode_size = sailr_vm_inst_list_size( inst_list);
	vm_stack_object* vmstack = sailr_vm_stack_init();

	// Run
	sailr_vm_exec_code(vmcode, vmcode_size , table , vmstack);

	// Assert
	// sailr_ptr_table_show_all(&table);

	char st_pizza = sailr_ptr_table_get_type(&table, (char*)"pizza");
	char st_size = sailr_ptr_table_get_type(&table, (char*)"size");
	char st_pieces = sailr_ptr_table_get_type(&table, (char*)"pieces"); 
	char st_friends = sailr_ptr_table_get_type(&table, (char*)"friends");
	char st_factorial = sailr_ptr_table_get_type(&table, (char*)"factorial");
	char st_exp = sailr_ptr_table_get_type(&table, (char*)"exp");
	char st_exp2 = sailr_ptr_table_get_type(&table, (char*)"exp2");
	char st_exp3 = sailr_ptr_table_get_type(&table, (char*)"exp3");

	CU_ASSERT_EQUAL( st_pizza , 'i');
	CU_ASSERT_EQUAL( st_size , 'd');
	CU_ASSERT_EQUAL( st_pieces , 'i');
	CU_ASSERT_EQUAL( st_friends , 'i');
	CU_ASSERT_EQUAL( st_factorial, 'i');
	CU_ASSERT_EQUAL( st_exp, 'i');
	CU_ASSERT_EQUAL( st_exp2, 'i');
	CU_ASSERT_EQUAL( st_exp3, 'd');

	int s_pizza = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"pizza"));
	double s_size = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"size"));
	int s_pieces = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"pieces")); 
	int s_friends = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"friends"));
	int s_factorial = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"factorial"));
	int s_exp = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"exp"));
	int s_exp2 = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"exp2"));
	double s_exp3 = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"exp3"));

	CU_ASSERT_EQUAL( s_pizza , 1);
	CU_ASSERT_DOUBLE_EQUAL( s_size , 0.25, 0.01);
	CU_ASSERT_EQUAL( s_pieces , 4);
	CU_ASSERT_EQUAL( s_friends , 3);
	CU_ASSERT_EQUAL( s_factorial, 24);
	CU_ASSERT_EQUAL( s_exp, 32);
	CU_ASSERT_EQUAL( s_exp2, 32);
	CU_ASSERT_DOUBLE_EQUAL( s_exp3, 10.648, 0.001);

	// Clean up
	sailr_tree_free(ps);
	sailr_ptr_table_del_all(&table);
	sailr_parser_state_free(ps);
}


