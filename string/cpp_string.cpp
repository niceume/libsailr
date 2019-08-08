#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
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

cpp_object*
cpp_string_new_unescaped_string( cpp_object* obj )
{
	std::string* ori_str = static_cast<std::string*>(obj);
	std::string* new_str = new std::string();
	std::cout << (*ori_str) << "(" << ori_str->length() << ")" << std::endl;

	if (ori_str->empty()){
		std::cout << "LENGTH is zero" << std::endl;
		return (cpp_object*) new_str;
	}

	size_t ori_capacity = ori_str->capacity() ;
	if(new_str->capacity() < (ori_capacity + 1)){
		new_str->reserve(ori_capacity + 1);
	}
	
	char new_char;
	for( auto it = ori_str->begin(); it != ori_str->end(); ++it){
		if( *it == '\\'){
			++it;
			if( it == ori_str->end()){
				printf("ERROR: string litereal should not end with single backslash.");
				break;
;			}
			switch (*it){
			case 't' : new_char = '\t';
			break;
			case 'n' : new_char = '\n';
			break;
			case 'r' : new_char = '\r';
			break;
			case '\\' : new_char = '\\';
			break; 
			case '\'' : new_char = '\'';
			break;
			case '"' : new_char = '"';
			break;
			case '?' : new_char = '?';
			break;
			default : new_char = *it ;
			break;
			}
		}else{
			new_char = *it;
		}
//		std::cout << "---" << new_char << "---" << std::endl;
		new_str->push_back(new_char) ;
	}
	return (cpp_object*) new_str;
}


cpp_object*
cpp_string_clone (cpp_object* obj)
{
	std::string* ori_str = static_cast<std::string*>(obj);
	std::string* new_str = new std::string(*ori_str);
	return (cpp_object*) new_str;
}

cpp_object*
cpp_string_int2str(int num)
{
	std::string* p_str;
//	std::stringstream ss;
//	ss.clear();
//	ss << num ;
//	p_str = new std::string(ss.str());
	p_str = new std::string( std::to_string(num));
	return (cpp_object*) p_str;
}

cpp_object*
cpp_string_double2str(double num)
{
	std::string* p_str;
//	std::stringstream ss;
//	ss.clear();
//	ss << num ;
//	p_str = new std::string(ss.str());
	p_str = new std::string( std::to_string(num));
	return (cpp_object*) p_str;
}

cpp_object*
cpp_string_lstrip(cpp_object* obj)
{
	std::string* str = static_cast<std::string*>(obj);
	std::string* new_str = new std::string(*str);
	new_str->erase(new_str->begin(), std::find_if_not(new_str->begin(), new_str->end(), [](int c){return std::isspace(c);}));
    return (cpp_object*) new_str;
}

cpp_object*
cpp_string_rstrip(cpp_object* obj)
{
	std::string* str = static_cast<std::string*>(obj);
	std::string* new_str = new std::string(*str);
	new_str->erase(std::find_if_not(new_str->rbegin(), new_str->rend(), [](int c){return std::isspace(c);}).base(), new_str->end());
    return (cpp_object*) new_str;
}

cpp_object*
cpp_string_strip(cpp_object* obj)
{
	std::string* str = static_cast<std::string*>(obj);
	std::string* lstripped_str = static_cast<std::string*>(cpp_string_lstrip(str));
    std::string* sripped_str = static_cast<std::string*>(cpp_string_rstrip(lstripped_str));
	delete(lstripped_str);
    return (cpp_object*) sripped_str;
}


const char*
cpp_string_read (cpp_object* obj )
{
	std::string* str;
	if (obj != NULL){
		str = static_cast<std::string*>(obj);
//		std::cout << *str << std::endl;
//		printf("%s\n", str->c_str());
		return str->c_str();
	}else{ 
		std::cout << "WARNING: String is NULL?\n" << std::endl;
		return "";
	}
}

cpp_object*
cpp_string_concat (cpp_object* obj1 , cpp_object* obj2 )
{
//	std::cout << "start concat" << std::endl;
	std::string* str1 = static_cast<std::string*>(obj1);
	std::string* str2 = static_cast<std::string*>(obj2);
	std::stringstream ss;
	ss << *str1 << *str2;
	std::string* new_p_str = new std::string( ss.str() );
	return (void*) new_p_str;
}


cpp_object**
cpp_string_ptr_concat (cpp_object* obj1 , cpp_object* obj2 )
{
	std::string* str1 = static_cast<std::string*>(obj1);
	// std::cout << (*str1) << std::endl;
	std::string* str2 = static_cast<std::string*>(obj2);
	// std::cout << (*str2) << std::endl;
	std::stringstream ss;
	ss << *str1 << *str2;
	std::string* new_p_str = new std::string( ss.str() );
	std::string** new_pp_str = new std::string*();
	(*new_pp_str) = new_p_str;
	return (void**) new_pp_str;
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
cpp_string_subset (cpp_object* obj, size_t from_idx , size_t to_idx )  // index starts from zero.
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
cpp_string_has_char (cpp_object* obj, char c)
{
	std::string* cpp_str = static_cast<std::string*>(obj);
	std::size_t pos = cpp_str->find( c );
	if( pos != std::string::npos ){
		return 1;
	} else {
		return 0 ;
	} 
}


int
cpp_string_str2int(cpp_object* obj)
{
	int tmp_int;
	std::string* str = static_cast<std::string*>(obj);
	std::istringstream ( *str ) >> tmp_int;
	return tmp_int;
}


double
cpp_string_str2double(cpp_object* obj)
{
	double tmp_dbl;
	std::string* str = static_cast<std::string*>(obj);
	std::istringstream ( *str ) >> tmp_dbl;
	return tmp_dbl;
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



