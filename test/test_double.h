#include "sailr.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_double_test1( void );

void
test_double_add_tests(CU_pSuite testSuite)
{
	CU_add_test(testSuite, "test doubles", test_double_test1 );
}

void
test_double_test1( void )
{
	// Code 
	const char* code = " "
"pi2 = 3.14 \n"
"pi10 = 3.1415926535 \n"
"e = 2.718281828 \n"
"radius = 2.5 \n"
"area = pi10 * (radius ^ 2) \n"
"volume = 4 / 3 * pi10 * (radius ^ 3)  \n"
"if( volume > 30){ \n"
"    big = 1 \n"
"} else { \n"
"    big = 0 \n"
"} \n" ;

	// Parser Initialization
	ptr_table_object* table = sailr_ptr_table_init() ;
	parser_state_object* ps = sailr_new_parser_state ("souce from string literal", table);
	sailr_construct_parser( code, ps ); 


	// Add variables
	sailr_ptr_table_create_null(&table, (char*)"pi2" );
	sailr_ptr_table_create_null(&table, (char*)"pi10" );
	sailr_ptr_table_create_null(&table, (char*)"e" );
	sailr_ptr_table_create_null(&table, (char*)"radius" );
	sailr_ptr_table_create_null(&table, (char*)"area" );
	sailr_ptr_table_create_null(&table, (char*)"volume" );
	sailr_ptr_table_create_null(&table, (char*)"big" );

	// Creating virtual machine codes
	vm_inst_object* inst_list = sailr_gen_code( ps, table); // VM Code is generated.
	vm_inst_object* vmcode = sailr_vm_inst_list_to_code(inst_list);
	int vmcode_size = sailr_vm_inst_list_size( inst_list);
	vm_stack_object* vmstack = sailr_vm_stack_init();

	// Run
	sailr_vm_exec_code(vmcode, vmcode_size , table , vmstack);

	// Assert
	// sailr_ptr_table_show_all(&table);

	char st_pi2 = sailr_ptr_table_get_type(&table, (char*)"pi2");
	char st_pi10 = sailr_ptr_table_get_type(&table, (char*)"pi10");
	char st_e = sailr_ptr_table_get_type(&table, (char*)"e"); 
	char st_radius = sailr_ptr_table_get_type(&table, (char*)"radius");
	char st_area = sailr_ptr_table_get_type(&table, (char*)"area");
	char st_volume = sailr_ptr_table_get_type(&table, (char*)"volume");
	char st_big = sailr_ptr_table_get_type(&table, (char*)"big");

	CU_ASSERT_EQUAL( st_pi2 , 'd');
	CU_ASSERT_EQUAL( st_pi10 , 'd');
	CU_ASSERT_EQUAL( st_e , 'd');
	CU_ASSERT_EQUAL( st_radius , 'd');
	CU_ASSERT_EQUAL( st_area, 'd');
	CU_ASSERT_EQUAL( st_volume, 'd');
	CU_ASSERT_EQUAL( st_big, 'i');

	double s_pi2 = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"pi2"));
	double s_pi10 = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"pi10"));
	double s_e = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"e")); 
	double s_radius = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"radius"));
	double s_area = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"area"));
	double s_volume = *((double*) *sailr_ptr_table_get_pptr(&table, (char*)"volume"));
	int s_big = *((int*) *sailr_ptr_table_get_pptr(&table, (char*)"big"));

	CU_ASSERT_EQUAL( s_pi2 , 3.14);
	CU_ASSERT_EQUAL( s_pi10 , 3.1415926535);
	CU_ASSERT_EQUAL( s_e , 2.718281828);
	CU_ASSERT_EQUAL( s_radius , 2.5);
	CU_ASSERT_DOUBLE_EQUAL( s_area, 19.63495, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL( s_volume, 65.44985, 0.00001);
	CU_ASSERT_EQUAL( s_big, 1);

	// Clean up
	sailr_tree_free(ps);
	sailr_ptr_table_del_all(&table);
	sailr_parser_state_free(ps);
}


