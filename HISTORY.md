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


## Plan

* Refactoring





