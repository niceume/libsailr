# HISTORY

## Project Started [Aug 2018]

* I started this project to make it easier to manipulate dataset.
* First, I tried to make a simple calculator


## Milestone Ver 0.1 [Nov 2018]

* The program starts woring.
	+ Virtual stack machine is implemented.
	+ Arithmetic calculations are implemented. 
	+ How to compile & run

```
cd vm; ./compile_vm.sh; cd .. 
./compile.sh
./myparser sample_code/code1.slr
```

## Ver 0.2 [Nov. 23 2018]

* The library now deals with C++ strings.
    + cpp_string.cpp implements that part.
    + struct_string.cpp is now not updated. (But remains there)
    + If this library might be used from C program, mechanism to switch between struct_string and cpp_string shoul be implementd
* The library is now used from C++.
    + C++ main program can calculate using this library. See main.cpp
* Makefile (GNU Make) can compile 
    + Meaning that this can be easily integrated with Rcpp project.

```
./compile_cpp.sh
```

* The project name is now changed from RCppCalc to LibsailrDev.
* To use libsailr part, use only under LibsailrDev/sailr/ directory.

```
cd <LibsailrDev_Project_Directory>/sailr/
make build
```


## Ver 0.21 [Nov. 28 2018]

* Bug fixes
    + string manipulations are improved. 
* From Rcpp this library is successfully loaded and utilized.

## Ver 0.22 [Dec. 17 2018]

* Bug fixes
    + Nested if else did not work well. Now it's fixed.
    + working fine.

## Ver 0.23 [Dec. 18 2018]

* Newly created variables are supported.
    1. Variable should be prepared as PTR_NULL on ptr_table.
    2. The variable type should be changed dynamically.


## Ver 0.24 [Dec.21 2018]

* Bug fix: double was not dealt properly, and it's fixed.

## Ver 0.25 [Dec.21 2018]

* Change directory structure.
    + Easier to be used from other projects.

## Ver 0.26 [Dec. 21 2018]

* For undefined varialbe, its type is assigned to the type of first value.
* Number (int and double) are dealt in the same way at VM instruction level.

## Ver 0.30 [Dec. 25 2018]

* Numbers are converted between double and int.
	* Add extra address field to ptr_table/ptr_record.
	* This enables number-type record to keep memory for int and double.
* This happens when assingning and LHS type is different from RHS type.

* Add unitary minus operator support.

## Ver 0.31 [Dec. 28 2018]

* Minor fixes
	* Function ptr_table_get_pptr() 
	* Some bug fixes.

## Ver 0.32

* Minor fixes for R package.
    * Code21, in which strings are manipulated, passed. 

## Ver 0.33 [Jan. 16 2019]

* Minor fixes
    + Add missing values support. (Period is dealt as missing number.)

## Ver 0.35 [Feb. 18 2019]

* Regular expression support 
* Function support

## Ver 0.40 [Mar. 5 2019]

* Functions are supported. (Just a few functions)
    + Built-in functions (written in C) are avaible.
    + Only print() function is implemented. 


## Ver 0.50 [Apr. 6 2019]

* More functions are added
    + print( str1, str2 ... )
    + num_to_str( num ) 
    + str_strip( str )
    + str_lstrip( str ) 
    + str_rstrip( str )
    + str_concat( str1, str2 ... ) 
    + str_repeat( str )
    + str_subset( str, num, num )  // index starts from zero.
    + str_to_num( str )
    + rexp_matched( num )  // back reference
    + date_ymd( year, month, day )
    + date_ym_weekday_nth( year, month, weekday, nth )  // weekday should be "Sun", "Mon" ... 
    + date_add_n_years( unix_date, years )
    + date_add_n_months( unix_date, months )
    + date_add_n_days( unix_date, days )
    + date_format( unix_date, str_format )  // This format should follow C++'s std::chrono::format. "%m/%d/%y" or "%Y-%m-%d" is popular.
* Back reference mechanism for regular expresion
    + Use function to do this. => rexp_matched(1)

## Ver 0.51 (Apr. 21 2019)

* Refactoring1
    + Stop Memory Leak
    + Free pointer table at last

## Ver 0.60 (May. 15 2019)

* Major improvements.
* Refactoring2
    + Introduce DEBUG_PRINT macro (in helper.h) for C part.
* Improve vm_calc.c (1)
    + Take care of INT_MIN and INT_MAX for integers. Performance is not efficient, but before int calculation, double calculation and integer range check is conducted.
    + deal with missing values.
        + At parsing phase, missing values are already treated as nan in double.
        + Be careful not to convert it into integer unexpectedly.
* Improve vm_calc.c (2) , vm/func/c_func , vm_assign.c , vm_rexp.c and vm_stack.c
    + Prevent from dealing with stack pointer directly. 
        + Use stack vm_stack_push_*** and vm_stack_clean_and_pop()
    + For vm_assign.c, it is refactored.
* Rethink about libsailr API. (for users to prevent memory leak easily.)
    + "Users should use pointers like PTR_INT, PTR_DBL and PTR_STR on ptr_table."
        + User should not use IVAL, DVAL. 
    + *Users should usually prepare memory for those known variables* , and they should be freed manually.
* Made vm_assign.c tidy
* Refactor pp2val.c (using vm_stack_item_is_temp() function)
* Refactor ptr_table: ptr_record_free_memory_if_gc_required() function. This should use string_free(), simple_re_free(). 

## Ver 0.61 (Jun. 1 2019)

* Unit tests are introduced. (CUnit)
* Along with the tests, bug fixes wree done.
	+ Examples:
	+ fcall was reduced to expr, but now is reduced to arg (in parse.y)
	+ Operations for missing values are corrected.
		+ For eq(==), when both sides have nan, return true. If either side has nan, return false. For neq(!=), the behavior is opposite.
		+ For other operators, like + - * =, follow C math.h implementation, meaning returning false
			+ https://stackoverflow.com/questions/38798791/nan-comparison-rule-in-c-c
	+ Dealing with comments is difficult.
		+ (Start) states are explicitly defined for most of the rules.
		+ One line comments and multiple line comments are implemented.
		+ One line comments should work like a terminator.
		+ Multiple line comments should start in a new line at current implementation. DON'T insert it wihtin a statement.
			+ (ref.) http://www.cs.man.ac.uk/~pjj/cs2121/ex2_str_comm.html 

## Ver 0.62 (Jun. 16 2019)

* Start to support 32bit/64bit windows platform using mingw-w64.
	+ See build scripts/envs under mingw_env directory.
* Period(.) is allowed to be included for variable name or column name. (iris data includes period.)

## Ver 0.63 (Aug. 8 2019)

* Move 32bit/64bit windows build scripts outside of this repository.
* Added compiler flags
    + Explicitly use "-std=c99", "std=c++11" for compilers
    + Use "-g" for debugging purpose (This does not slow donw execution)
    + "-fstack-protector-strong" for C/C++ compilers
        + This flag is disabled for mingw compiler.
        + See the conditional in makefile.
    + These are learnt from testthat library compilations.
* In Makefile
    + $^ is replaced with $<
        + $^ : All the prerequisites
        + $< : The 1st prerequisite
        + The Makefile uses -include $(DEPS) & -MD -MMP mechanism, meaning that header files are set to be prerequisites for target file.
            + Only the 1st prerequisite should be compiled. Prerequisite at 2nd and after are header files, so they should not be listed in gcc/g++ arguments.
* Under dev_env directory
    + Makefile is used for test parser (myparsercpp) compilation.
    + Onigmo source for this test parser is put under dev_env/onigmo_src
        + This onigmo_src is ignored from git. 
        + Download onigmo source and extract the files there. 


## Ver 0.64 (Sep. 10 2019)

* Parser (bison/flex) is now reentrant.

## Ver 0.65 (Sep.11 2019)

* Global variables in ptr_table.c are removed. 
    + Those counter information for anonymous string and regexp are stored in the 1st element of ptr_table object.
    + The type is defined in ptr_table_info that is stored in the seed element called "_HEAD_OF_UTHASH_"

## Ver 0.66 (Sep.23 2019)

* ptr_table now holds information whether null variable (Known but not defined) obtains type definition.
    + ptr_table_info->null_update holds this information. This does not holds which variable became defined.


## Ver 0.67 (Sep.24 2019)

* vm/func/c_func.c is updated.
    + Previous codes created anonymous string on ptr_table every time the function returns new string even within RHS of assignment operator.
    + The current code jsut creates temporary string on vm stack as return from function.
    + Minor related fixes.

## Ver 0.68 (Sep.25 2019)

* vm_stack is now able to hold string encoding information. 
    + Based on this information, vm stack is going to call appropriate string functions.
    + Add test case for this functionality.

## Ver 0.69 (Sep. 30 2019)

* UTF8 support. The following functions can deal with utf8 strings
    + cpp_string_subset() calls appropriate functions based on encodings. (Now this fuction requires encoding.)
        + cpp_string_subset_utf8()
        + cpp_string_subset_latin1()
    + cpp_string_new_unescaped_string() calls appropriate functions based on encodings. (Now this fuction requires encoding.)
        + cpp_string_new_unescaped_string_utf8()
        + cpp_string_new_unescaped_string_latin1()
* vm_stack holds information how to deal with strings.
    + calls appropriate functions.
* parser_state object now holds source file encoding, which can be used for creating new regular expression object.
    + regular expression object continues to hold this encoding information.
    + string objects do not need this information. They just hold byte sequences that come from input data or source file string literals.

## Ver 0.70 (Oct. 6 2019)

* Avoid (char*) casting
    + ptr_table functions take const char* as key.
        + ,though the key string of ptr_record continues to be char[].
    + simple_date_format()


## Plan 

* Report run time error.
    + Append line number and column number of corresponding codes to AST node.
    + Paass the information to VM instruction.
    + Report error with this column and line number.
* Avoid directly manipulate ptr_table's properties. Provide functions and use them.
* Matched regular expressions also need to be cleared for every row. Provide such functionalities.
* Consider some script language extension. BSD licensed language is best (e.g. Lua, mruby or Gauche??)
* Macro to add variables for users to ptr_table.
    + When adding value to ptr_table, missing values should be taken care of.
        + Missing value should be added as nan in double.
* Refactoring2
    + Functions in ptr_table.c. Pointer to pointer may be used wrongly; possibility for some local pointers are destroyed unintentionally.


## Abondoned Ideas

* Ways to deal with general objects.
    + At ptr_table level?? Only at function call level?? => Implement only at function call level.
    + For example. let's think about how to deal with tm structure defined in time.h in C.
        + At ptr_table level, PTR_OBJ is used.
        + At vm stack level, PP_OBJ is used.
        + PP_OBJ pointts to a wrapper for some structure. 
        + That wrapper (wrapper_obj) holds the type of real object.


