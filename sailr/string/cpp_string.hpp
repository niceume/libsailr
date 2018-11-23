#ifndef CPP_STRING_HPP
#define CPP_STRING_HPP

typedef void cpp_object;

#ifdef __cplusplus
extern "C" {
#endif
	cpp_object* cpp_string_new (const char*);
	cpp_object* cpp_string_new_with_len (const char* , int);
	const char* cpp_string_read (cpp_object*);
	cpp_object* cpp_string_concat (cpp_object*, cpp_object*);
	cpp_object* cpp_string_repeat (cpp_object*, int);
	cpp_object* cpp_string_subset (cpp_object*, size_t, size_t );
	// int cpp_string_copy_ptr ( cpp_object** , cpp_object** );
	int cpp_string_compare ( cpp_object* , cpp_object* );
	int cpp_string_move_ptr ( cpp_object** , cpp_object** );
	int cpp_string_free ( cpp_object* );
#ifdef __cplusplus
};
#endif


#endif /* CPP_STRING_HPP */

