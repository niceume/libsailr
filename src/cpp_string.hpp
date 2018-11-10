#define cpp_string void

#ifdef __cplusplus
extern "C" {
#endif
	const char* read_cpp_string (cpp_string*);
	cpp_string* concat_cpp_string(cpp_string*, cpp_string*);
	cpp_string* repeat_cpp_string(int, cpp_string*);
	cpp_string* new_cpp_string();
	cpp_string* update_cpp_string(cpp_string*, char* );
	int free_cpp_string ( cpp_string* );
#ifdef __cplusplus
};
#endif
