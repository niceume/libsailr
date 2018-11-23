#include <string>
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "cpp_string.hpp"

cpp_object*
cpp_string_new (const char* str)
{
	std::string* new_str = new std::string(str);
	return (void*) new_str;
}

cpp_object*
cpp_string_new_with_len(const char* str, int len)
{
	std::string* new_str = new std::string(str, len);
	return (void*) new_str;
}

const char*
cpp_string_read (cpp_object* obj )
{
	if (obj != NULL)
		return ( static_cast<std::string*>(obj))->c_str();
	else 
		std::cout << "String is NULL ??\n" << std::endl;
		return "";
}

cpp_object*
cpp_string_concat (cpp_object* obj1 , cpp_object* obj2 )
{
	std::string* str1 = static_cast<std::string*>(obj1);
	std::string* str2 = static_cast<std::string*>(obj2);
	std::stringstream ss;
	ss << *str1 << *str2;
	std::string* new_str = new std::string( ss.str() );
	return (void*) new_str;
}

cpp_object*
cpp_string_repeat(cpp_object* obj, int rep)
{
	std::string* cpp_str = static_cast<std::string*>(obj);

	std::stringstream ss;
	if (rep <= 0 ){
		std::cout << "ERROR: rep should be greater than 0. \n";
		exit(0);
	}
		
	for( ; rep > 0; rep-- ){
		ss << (*cpp_str) ;
	}
	std::string* new_str = new std::string( ss.str() );
	return (void*) new_str;	
}

cpp_object*
cpp_string_subset (cpp_object* obj, size_t from_idx , size_t to_idx )
{
	std::string* cpp_str = static_cast<std::string*>(obj);

	if(from_idx > to_idx ){
		int temp_idx = to_idx;
		to_idx = from_idx;
		from_idx = temp_idx;
	}
	if(to_idx >= cpp_str->size()){
		to_idx = cpp_str->size() - 1;
	}
	std::string* new_str = new std::string( cpp_str->substr(from_idx, (to_idx - from_idx + 1 )));
	return new_str;	
}

int
cpp_string_compare ( cpp_object* obj1, cpp_object* obj2)
{
	std::string* cpp_str1 = static_cast<std::string*>(obj1);
	std::string* cpp_str2 = static_cast<std::string*>(obj2);
	if( cpp_str1->compare(*cpp_str2) == 0 ) { // matched
		return 1;
	} else { // not matched
		return 0;
	}

}

int
cpp_string_copy_ptr(cpp_object** ptrptr , cpp_object** obj)  // This function is dangerous.
{
	*ptrptr = *obj;
	return 1;
}

int
cpp_string_move_ptr(cpp_object** ptrptr , cpp_object** obj)
{
	cpp_object* old_object = *ptrptr;
	cpp_string_copy_ptr(ptrptr, obj);
	delete (std::string* )old_object ;
	*obj = NULL;
	return 1;
}

int
cpp_string_free ( cpp_object* obj)
{
	std::string* cpp_str = static_cast<std::string*>(obj);
	delete cpp_str ;
	return 1;
}



