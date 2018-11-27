#include "common_string.h"
#include <stdio.h>
#include <stdint.h>

string_object*
string_new(const c_char* str )
{
	return cpp_string_new( str );
}

string_object*
string_new_with_len(const c_char* str, int len)
{
	return cpp_string_new_with_len( str, len);
}

const c_char*
string_read( string_object* str)
{
  const char* c_str = cpp_string_read( str );
  printf("C STRING: %s", c_str);
	return c_str;
}

string_object*
string_concat( string_object* str1, string_object* str2 )
{
	return  cpp_string_concat( str1, str2 );
}

string_object**
string_ptr_concat( string_object* str1, string_object* str2 )
{
	string_object** str_obj = (string_object**)(intptr_t) cpp_string_ptr_concat( str1, str2 );
	return str_obj;
}

string_object*
string_repeat( string_object* str, int rep )
{
	return cpp_string_repeat( str, rep );
}

string_object*
string_subset( string_object* str, size_t from_idx, size_t to_idx)
{
	return cpp_string_subset( str, from_idx, to_idx );
}

int
string_compare( string_object* str1, string_object* str2)
{
	return cpp_string_compare( str1, str2);
}

int
string_move_ptr( string_object** str1, string_object** str2)
{
	return cpp_string_move_ptr(str1, str2);
}

int
string_free( string_object* str )
{
	return cpp_string_free( str);
}


