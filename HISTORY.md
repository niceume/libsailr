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

## Plan 

* Refactoring2
	+ Avoid (char*) casting
    + Functions in ptr_table.c. Pointer to pointer may be used wrongly; possibility for some local pointers are destroyed unintentionally.
* Matched regular expressions also need to be cleared for every row.


## Abondoned Ideas

* Ways to deal with general objects.
    + At ptr_table level?? Only at function call level?? => Implement only at function call level.
    + For example. let's think about how to deal with tm structure defined in time.h in C.
        + At ptr_table level, PTR_OBJ is used.
        + At vm stack level, PP_OBJ is used.
        + PP_OBJ pointts to a wrapper for some structure. 
        + That wrapper (wrapper_obj) holds the type of real object.


