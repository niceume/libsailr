extern "C" {
	#include "sailr.h"
}

#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include "stdlib.h"
#include "stdio.h"



int
main(int argc, char** argv)
{
	char* source_name = argv[1];
	std::ifstream ifs(source_name);
	if(ifs.fail()){
		std::cerr << "Could not open file.\n" << std::endl;
		return 0;
	}
	std::string code((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	std::cout <<  code << std::endl;

	// Initializing pointer table. 
	ptr_table_object* table = sailr_ptr_table_init() ;
    printf("table's pointer is %p \n", table);

	//	sailr_ptr_table_create_anonym_string(&table, "Hello, Japan!");

	// Initializing parser_state, which stores TreeNode*.
	std::cout << "Constructing parser! \n"  << std::endl ;
	parser_state_object* ps = sailr_new_parser_state (source_name, table);
	sailr_construct_parser( code.c_str(), ps );  // Now ps holds tree and table!!

	std::cout << "Show variables." << std::endl;
	char** vars = sailr_varnames(ps);
	int var_num = sailr_varnames_num(ps);
	int var_idx;
	for(var_idx=0; var_idx < var_num; var_idx++){
		std::cout << vars[var_idx] << std::endl;
	}

	std::cout << "Show LHS variables." << std::endl;
	char** lhs_vars = sailr_lhs_varnames(ps);
	int lhs_var_num = sailr_lhs_varnames_num(ps);
	int lhs_var_idx;
	for(lhs_var_idx=0; lhs_var_idx < lhs_var_num; lhs_var_idx++){
		std::cout << lhs_vars[lhs_var_idx] << std::endl;
	}

	std::cout << "Show RHS variables." << std::endl;
	char** rhs_vars = sailr_rhs_varnames(ps);
	int rhs_var_num = sailr_rhs_varnames_num(ps);
	int rhs_var_idx;
	for(rhs_var_idx=0; rhs_var_idx < rhs_var_num; rhs_var_idx++){
		std::cout << rhs_vars[rhs_var_idx] << std::endl;
	}


	// Assinging pointers onto table

	int* address_for_x = (int *)malloc(sizeof(int));
	double* address_for_d_x = (double *)malloc(sizeof(double));
	*address_for_x = 10;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"x" , &address_for_x, &address_for_d_x);

	int* address_for_y = (int *)malloc(sizeof(int));
	double* address_for_d_y = (double *)malloc(sizeof(double));
	*address_for_y = 10;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"y" , &address_for_y, &address_for_d_y);

	int* address_for_z = (int *)malloc(sizeof(int));
	double* address_for_d_z = (double *)malloc(sizeof(double));
	*address_for_z = 10;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"z" , &address_for_z, &address_for_d_z);

	int* address_for_z2 = (int *)malloc(sizeof(int));
	double* address_for_d_z2 = (double *)malloc(sizeof(double));
	*address_for_z2 = 10;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"z2" , &address_for_z2, &address_for_d_z2);

	double* address_for_bmi = (double *)malloc(sizeof(double));
	int* address_for_i_bmi = (int *)malloc(sizeof(int));
	*address_for_bmi = 0;
	sailr_ptr_table_create_double_from_ptr(&table, (char*)"bmi" , &address_for_bmi, &address_for_i_bmi);

	sailr_ptr_table_create_null(&table, (char*)"bmix" );

	double* address_for_height = (double *)malloc(sizeof(double));
	int* address_for_i_height = (int *)malloc(sizeof(int));
	*address_for_height = 1.72;
	sailr_ptr_table_create_double_from_ptr(&table, (char*)"height" , &address_for_height, &address_for_i_height);

	double* address_for_bodyweight = (double *)malloc(sizeof(double));
	int* address_for_i_bw = (int *)malloc(sizeof(int));
	*address_for_bodyweight = 57.5;
	sailr_ptr_table_create_double_from_ptr(&table, (char*)"weight" , &address_for_bodyweight, &address_for_i_bw);

	int* address_for_ow = (int *)malloc(sizeof(int));
	double* address_for_d_ow = (double *)malloc(sizeof(double));
	*address_for_ow = 10;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"overweight" , &address_for_ow, &address_for_d_ow);

	string_type_object* judgement = sailr_new_string("weight?");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "judgement" , &judgement);

	string_type_object* greeting = sailr_new_string("Hello World");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "greeting" , &greeting);

	string_type_object* univ = sailr_new_string("U");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "univ" , &univ);

	string_type_object* dot = sailr_new_string("D");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "dot" , &dot);

	string_type_object* name = sailr_new_string("N");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "name" , &name);

	string_type_object* result = sailr_new_string("R");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "result" , &result);

	string_type_object* race = sailr_new_string("Asian-Pac-Islander");
	sailr_ptr_table_create_string_from_ptr( &table, (char*) "race" , &race);

	int* address_for_oa = (int *)malloc(sizeof(int));
	double* address_for_d_oa = (double *)malloc(sizeof(double));
	*address_for_oa = 0;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"old_asian" , &address_for_oa, &address_for_d_oa);

	int* address_for_race_id = (int *)malloc(sizeof(int));
	double* address_for_d_race_id = (double *)malloc(sizeof(double));
	*address_for_race_id  = 0;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"race_id" , &address_for_race_id, &address_for_d_race_id);

	int* address_for_age = (int *)malloc(sizeof(int));
	double* address_for_d_age = (double *)malloc(sizeof(double));
	*address_for_age  = 0;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"age" , &address_for_age, &address_for_d_age);

	double* address_for_cre = (double *)malloc(sizeof(double));
	int* address_for_i_cre = (int *)malloc(sizeof(int));
	*address_for_cre  = 0;
	sailr_ptr_table_create_double_from_ptr(&table, (char*)"cre" , &address_for_cre, &address_for_i_cre);

	int* address_for_sex = (int *)malloc(sizeof(int));
	double* address_for_d_sex = (double *)malloc(sizeof(double));
	*address_for_sex  = 0;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"sex" , &address_for_sex, &address_for_d_sex);

	double* address_for_egfr = (double *)malloc(sizeof(double));
	int* address_for_i_egfr = (int *)malloc(sizeof(int));
	*address_for_egfr  = 0;
	sailr_ptr_table_create_double_from_ptr(&table, (char*)"egfr" , &address_for_egfr, &address_for_i_egfr);

	int* address_for_hrsweek = (int *)malloc(sizeof(int));
	double* address_for_d_hrsweek = (double*) malloc(sizeof(double));
	*address_for_hrsweek  = 20;
	sailr_ptr_table_create_int_from_ptr(&table, (char*)"hrsweek" , &address_for_hrsweek, &address_for_d_hrsweek);

	sailr_ptr_table_create_null(&table, (char*)"workaholic_asian" );
	sailr_ptr_table_create_null(&table, (char*)"workaholic_asian_name" );
	sailr_ptr_table_create_null(&table, (char*)"first" );
	sailr_ptr_table_create_null(&table, (char*)"second" );
	sailr_ptr_table_create_null(&table, (char*)"fullname" );
	sailr_ptr_table_create_null(&table, (char*)"great_dr" );

	sailr_ptr_table_create_null(&table, (char*)"pattern" );

	std::cout << "Show pointer table! At this point, annonym STRING should be already added.\n"  << std::endl ;
    printf("table's pointer is %p \n", table);
	sailr_ptr_table_show_all(&table);

	std::cout << "Show parser tree! \n"  << std::endl ;

	sailr_tree_dump( ps );


	std::cout << "Show pointer table again! \n"  << std::endl ;
	sailr_ptr_table_show_all(&table);


	vm_inst_object* inst_list = sailr_gen_code( ps, table); // VM Code is generated.

	sailr_vm_inst_list_show ( inst_list ); // Show VM code list.

	vm_inst_object* vmcode = sailr_vm_inst_list_to_code(inst_list);
	int vmcode_size = sailr_vm_inst_list_size( inst_list);
	
	/* Run virtual machine!! */
	vm_stack_object* vmstack = sailr_vm_stack_init();
	sailr_vm_exec_code(vmcode, vmcode_size , table , vmstack);

	/* Show Ptr Table */
	std::cout << "\nShow pointer table! \n" << std::endl;
	sailr_ptr_table_show_all(&table);

    /* Free memory */
	std::cout << "\nFree parser tree! \n" << std::endl;
	sailr_tree_free(ps);

	return 1;
}
