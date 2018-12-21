#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include <stdlib.h>
#include "cpp_string.hpp"

typedef cpp_object string_object;
typedef char c_char;

string_object* string_new(const c_char* str );
string_object* string_new_with_len( const c_char* str, int len);
const c_char* string_read( string_object* str);
const c_char* string_read( string_object* str);
string_object* string_concat( string_object* str1, string_object* str2 );
string_object** string_ptr_concat( string_object* str1, string_object* str2 );
string_object* string_repeat( string_object* str, int rep );
string_object* string_subset( string_object* str, size_t from_idx, size_t to_idx);
int string_compare( string_object* str1, string_object* str2);
int string_move_ptr( string_object** str1, string_object** str2);
int string_free( string_object* str );


#endif /* COMMON_STRING_H */
