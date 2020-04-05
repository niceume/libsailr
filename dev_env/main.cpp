extern "C" {
	#include "sailr.h"
}

#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <cstring>
#include "stdlib.h"
#include "stdio.h"
#include "getopt.h"

int verboseflag = 0; /* 0: minimal output, 1: verbose output */
#define VERBOSE( X ) do{ if(verboseflag != 0){ X } } while(0);

int
main(int argc, char** argv)
{
	// Code to parse options
	// -v option enables verbose output.
	int matched;
	opterr = 0;
	while( (matched = getopt( argc, argv, "vhi:")) != -1  ){
	switch (matched){
	case 'v':
		verboseflag = 1;
		std::cout << "Option -v is specified" << std::endl;
		break;
	default:
		std::cerr << "Usage: " << argv[0] << " [-v]  \n" << std::endl;
		exit(EXIT_FAILURE);
		break;
	}
	}

	// Select the filename.
	VERBOSE( std::cout << "----- Obtaining file name -----" << std::endl; )
	const char* source_name;
	if (optind < argc ){
		source_name = argv[optind];
        VERBOSE( std::cout << "Non-parametered arg is filename of sailr code: " << source_name << std::endl; )
	} else {
		std::cerr << "ERRROR: File name is not specified." << std::endl;
	}


	// Open the code file
	VERBOSE( std::cout << "----- Opening file -----" << std::endl; )
	std::ifstream ifs(source_name);
	if(ifs.fail()){
		std::cerr << "Could not open file.\n" << std::endl;
		return 0;
	}


	// Sailr code is passed to this program.
	VERBOSE( std::cout << "----- Reading Sailr code -----" << std::endl; )
	std::string code((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	VERBOSE( std::cout <<  code << std::endl; )


	// Initializing pointer table. 
	VERBOSE( std::cout << "----- Initializing pointer table -----" << std::endl; )
	ptr_table_object* table = sailr_ptr_table_init() ;
    VERBOSE( printf("Constructed and initialized table's pointer is %p \n", table); )


	// Initializing parser_state, which stores TreeNode*.
	VERBOSE( std::cout << "----- Running parser -----"  << std::endl ; )
	parser_state_object* ps = sailr_new_parser_state (source_name, table);
	sailr_run_parser( code.c_str(), ps );  // Now ps holds tree and table!!

	VERBOSE( std::cout << "----- Show variables -----" << std::endl; )
	VERBOSE( std::cout << "All the Variables (LHS or RHS detected at parse.y)" << std::endl; )
	char** vars = sailr_varnames(ps);
	int var_num = sailr_varnames_num(ps);
	VERBOSE( int var_idx;
	for(var_idx=0; var_idx < var_num; var_idx++){
		std::cout << vars[var_idx] << " ";
	} 
	std::cout << std::endl; )

	VERBOSE( std::cout << "LHS variables." << std::endl; )
	char** lhs_vars = sailr_lhs_varnames(ps);
	int lhs_var_num = sailr_lhs_varnames_num(ps);
	VERBOSE( int lhs_var_idx;
	for(lhs_var_idx=0; lhs_var_idx < lhs_var_num; lhs_var_idx++){
		std::cout << lhs_vars[lhs_var_idx] << " ";
	} 
	std::cout << std::endl; )

	VERBOSE( std::cout << "RHS variables." << std::endl; )
	char** rhs_vars = sailr_rhs_varnames(ps);
	int rhs_var_num = sailr_rhs_varnames_num(ps);
	VERBOSE( int rhs_var_idx;
	for(rhs_var_idx=0; rhs_var_idx < rhs_var_num; rhs_var_idx++){
		std::cout << rhs_vars[rhs_var_idx] << " ";
	} 
	std::cout << std::endl; )

	// Free temporarily used cstring variable names
	// In the future, it is better to provide iteration for var name access, then no need to free.
	sailr_varnames_free(vars, var_num);
	sailr_varnames_free(lhs_vars, lhs_var_num);
	sailr_varnames_free(rhs_vars, rhs_var_num);


	// Assinging pointers onto table

	int* address_for_x = (int *)malloc(sizeof(int));
	double* address_for_d_x = (double *)malloc(sizeof(double));
	*address_for_x = 10;
	sailr_ptr_table_create_int_from_ptr(&table, "x" , &address_for_x, &address_for_d_x);

	int* address_for_y = (int *)malloc(sizeof(int));
	double* address_for_d_y = (double *)malloc(sizeof(double));
	*address_for_y = 10;
	sailr_ptr_table_create_int_from_ptr(&table, "y" , &address_for_y, &address_for_d_y);

	int* address_for_z = (int *)malloc(sizeof(int));
	double* address_for_d_z = (double *)malloc(sizeof(double));
	*address_for_z = 10;
	sailr_ptr_table_create_int_from_ptr(&table, "z" , &address_for_z, &address_for_d_z);

	int* address_for_z2 = (int *)malloc(sizeof(int));
	double* address_for_d_z2 = (double *)malloc(sizeof(double));
	*address_for_z2 = 10;
	sailr_ptr_table_create_int_from_ptr(&table, "z2" , &address_for_z2, &address_for_d_z2);

	double* address_for_bmi = (double *)malloc(sizeof(double));
	int* address_for_i_bmi = (int *)malloc(sizeof(int));
	*address_for_bmi = 0;
	sailr_ptr_table_create_double_from_ptr(&table, "bmi" , &address_for_bmi, &address_for_i_bmi);

	sailr_ptr_table_create_null(&table, "bmix" );

	double* address_for_height = (double *)malloc(sizeof(double));
	int* address_for_i_height = (int *)malloc(sizeof(int));
	*address_for_height = 1.67;
	sailr_ptr_table_create_double_from_ptr(&table, "height" , &address_for_height, &address_for_i_height);

	double* address_for_bodyweight = (double *)malloc(sizeof(double));
	int* address_for_i_bw = (int *)malloc(sizeof(int));
	*address_for_bodyweight = 70.5;
	sailr_ptr_table_create_double_from_ptr(&table, "weight" , &address_for_bodyweight, &address_for_i_bw);

	int* address_for_ow = (int *)malloc(sizeof(int));
	double* address_for_d_ow = (double *)malloc(sizeof(double));
	*address_for_ow = 10;
	sailr_ptr_table_create_int_from_ptr(&table, "overweight" , &address_for_ow, &address_for_d_ow);

	sailr_ptr_table_create_string_from_cstring( &table, "judgement" , "weight?" );

	sailr_ptr_table_create_string_from_cstring( &table, "greeting" , "Hello World");

	sailr_ptr_table_create_null(&table, "greeting2" );

	sailr_ptr_table_create_string_from_cstring( &table, "hello" , "Hello World");

	sailr_ptr_table_create_string_from_cstring( &table, "univ" , "U");

	sailr_ptr_table_create_string_from_cstring( &table, "dot" , "D");

	sailr_ptr_table_create_string_from_cstring( &table, "name" , "N");

	sailr_ptr_table_create_string_from_cstring( &table, "result" , "R");

	sailr_ptr_table_create_string_from_cstring( &table, "race" , "Asian-Pac-Islander");

	int* address_for_oa = (int *)malloc(sizeof(int));
	double* address_for_d_oa = (double *)malloc(sizeof(double));
	*address_for_oa = 0;
	sailr_ptr_table_create_int_from_ptr(&table, "old_asian" , &address_for_oa, &address_for_d_oa);

	int* address_for_race_id = (int *)malloc(sizeof(int));
	double* address_for_d_race_id = (double *)malloc(sizeof(double));
	*address_for_race_id  = 0;
	sailr_ptr_table_create_int_from_ptr(&table, "race_id" , &address_for_race_id, &address_for_d_race_id);

	int* address_for_age = (int *)malloc(sizeof(int));
	double* address_for_d_age = (double *)malloc(sizeof(double));
	*address_for_age  = 20;
	sailr_ptr_table_create_int_from_ptr(&table, "age" , &address_for_age, &address_for_d_age);

	sailr_ptr_table_create_null(&table, "age_null");
	sailr_ptr_table_create_null(&table, "age_null2");

	double* address_for_cre = (double *)malloc(sizeof(double));
	int* address_for_i_cre = (int *)malloc(sizeof(int));
	*address_for_cre  = 0;
	sailr_ptr_table_create_double_from_ptr(&table, "cre" , &address_for_cre, &address_for_i_cre);

	int* address_for_sex = (int *)malloc(sizeof(int));
	double* address_for_d_sex = (double *)malloc(sizeof(double));
	*address_for_sex  = 0;
	sailr_ptr_table_create_int_from_ptr(&table, "sex" , &address_for_sex, &address_for_d_sex);

	double* address_for_egfr = (double *)malloc(sizeof(double));
	int* address_for_i_egfr = (int *)malloc(sizeof(int));
	*address_for_egfr  = 0;
	sailr_ptr_table_create_double_from_ptr(&table, "egfr" , &address_for_egfr, &address_for_i_egfr);

	int* address_for_hrsweek = (int *)malloc(sizeof(int));
	double* address_for_d_hrsweek = (double*) malloc(sizeof(double));
	*address_for_hrsweek  = 20;
	sailr_ptr_table_create_int_from_ptr(&table, "hrsweek" , &address_for_hrsweek, &address_for_d_hrsweek);

	sailr_ptr_table_create_null(&table, "workaholic_asian" );
	sailr_ptr_table_create_null(&table, "workaholic_asian_name" );
	sailr_ptr_table_create_null(&table, "first" );
	sailr_ptr_table_create_null(&table, "second" );
	sailr_ptr_table_create_null(&table, "firstname" );
	sailr_ptr_table_create_null(&table, "lastname" );
	sailr_ptr_table_create_null(&table, "fullname" );
	sailr_ptr_table_create_null(&table, "great_dr" );

	sailr_ptr_table_create_null(&table, "pattern" );
	sailr_ptr_table_create_null(&table, "match" );
	sailr_ptr_table_create_null(&table, "date" );
	sailr_ptr_table_create_null(&table, "date_str" );

	sailr_ptr_table_create_null(&table, "greeting_rexp" );
	sailr_ptr_table_create_null(&table, "email_rexp" );

	sailr_ptr_table_create_null(&table, "email_tom" );
	sailr_ptr_table_create_null(&table, "email_full" );
	sailr_ptr_table_create_null(&table, "email_local" );
	sailr_ptr_table_create_null(&table, "email_domain" );

	sailr_ptr_table_create_null(&table, "carname" );
	sailr_ptr_table_create_null(&table, "hp" );
	sailr_ptr_table_create_null(&table, "cyl" );
	sailr_ptr_table_create_null(&table, "power" );
	sailr_ptr_table_create_null(&table, "country" );
	sailr_ptr_table_create_null(&table, "type" );

	sailr_ptr_table_create_null(&table, "a" );
	sailr_ptr_table_create_null(&table, "b" );
	sailr_ptr_table_create_null(&table, "c" );
	sailr_ptr_table_create_null(&table, "d" );
	sailr_ptr_table_create_null(&table, "e" );

	VERBOSE( std::cout << "----- Showing pointer table (BEFORE CALCULATION) -----" << std::endl; )
	VERBOSE( std::cout << "At this point, annonym strings should be already added."  << std::endl; )
    VERBOSE( printf("table's pointer is %p \n", table); )
	VERBOSE( sailr_ptr_table_show_all(&table); )

	VERBOSE( std::cout << "----- Showing parser tree -----"  << std::endl; )
	VERBOSE( sailr_tree_dump( ps ); )

	VERBOSE( std::cout << "----- Generating VM instructions -----" << std::endl; ) 
	VERBOSE( std::cout << "VM instructions are generated with parser state and pointer table " << std::endl; )
	vm_inst_object* inst_list = sailr_gen_code( ps, table); // VM Code is generated.
	VERBOSE( std::cout << "Generated. Showing..." << std::endl; )
	VERBOSE( sailr_vm_inst_list_show_all ( inst_list );)

	VERBOSE( std::cout << "----- Converting VM instructions -----" << std::endl; ) 
	VERBOSE( std::cout << "Converting VM instruction list(linked list format) into VM instruction code (array format)" << std::endl; )
	vm_inst_object* vmcode = sailr_vm_inst_list_to_code(inst_list);
	int vmcode_size = sailr_vm_inst_list_size( inst_list);
	
	/* Construct and run virtual machine */
	VERBOSE(std::cout << "----- Constructing VM stack ------" << std::endl; )
	vm_stack_object* vmstack = sailr_vm_stack_init();
	VERBOSE(std::cout << "----- Running VM code on VM stack ------" << std::endl; )
	sailr_vm_exec_code(vmcode, vmcode_size , table , vmstack);

	/* Pointer Table AFTER CALCULATION */
	VERBOSE( std::cout << "----- Showing pointer table AFTER CALCULATION -----" << std::endl; )
	VERBOSE( sailr_ptr_table_show_all(&table); )

	/* Test Code to delete pointer table records. */
//	std::cout << "Delete pointer records except ...  \n" << std::endl;
//	char* record_list[] = {"_HEAD_OF_UTHASH_", "greeting","bmi", "date", "date_str"};
//	sailr_ptr_table_del_records_except(&table, record_list, sizeof(record_list) / sizeof(record_list[0]));
//	sailr_ptr_table_show_all(&table);

	/* Free memory for instructions */
	sailr_vm_inst_list_free( inst_list );
	sailr_vm_inst_code_free( vmcode );

    /* Free memory */
	VERBOSE( std::cout << "----- Memory areas are going to be freed -----" << std::endl; )
	VERBOSE( std::cout << "Going to free parser tree \n" << std::endl; )
	sailr_tree_free(ps);
	VERBOSE( std::cout << "Going to free pointer table" << std::endl; )
	sailr_ptr_table_del_all(&table);
	VERBOSE( std::cout << "Going to free parser state object" << std::endl; )
	sailr_parser_state_free(ps);

	return 1;
}
