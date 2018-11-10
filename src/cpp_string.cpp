#include <string>
#include <iostream>
#include <sstream>
#include "stdlib.h"
#include "cpp_string.hpp"

const char*
read_cpp_string (cpp_string* void_str)
{
	return ( static_cast<std::string*>(void_str))->c_str();
}

cpp_string*
concat_cpp_string(cpp_string* void_str1 , cpp_string* void_str2 )
{
	std::string* str1 = static_cast<std::string*>(void_str1);
	std::string* str2 = static_cast<std::string*>(void_str2);
	std::stringstream ss;
	ss << *str1 << *str2;
	std::string* new_str = new std::string( ss.str() );
	return (void*) new_str;
}

cpp_string*
repeat_cpp_string(int rep, cpp_string* void_str)
{
	std::string* cpp_str = static_cast<std::string*>(void_str);
	std::stringstream ss;
	if (rep <= 0 ){
		std::cout << "ERROR: rep should be greater than 0. \n";
		exit(0);
	}
		
	for( ; rep > 0; rep-- ){
		ss << cpp_str ;
	}
	std::string* new_str = new std::string( ss.str() );
	return (void*) new_str;		
}

cpp_string*
new_cpp_string()
{
	std::string* new_str = new std::string();
	return (void*) new_str;
}

cpp_string*
update_cpp_string(cpp_string* void_str, char* new_chars)
{
	std::string* cpp_str = static_cast<std::string*>(void_str);
	*cpp_str = std::string(new_chars);
	return cpp_str;
}

int
free_cpp_string ( cpp_string* )
{

}



