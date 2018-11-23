#include <stdio.h>
#include "cpp_string.hpp"


int main(int argc, char** argv)
{
	cpp_object* obj = cpp_string_new("Hello ");
	cpp_object* obj2 = cpp_string_new("World!");
	cpp_object* obj3 = cpp_string_concat(obj, obj2);
	printf( "%s\n" , cpp_string_read(obj3));
	printf( "%p\n" , obj);
	printf( "%p\n" , obj2);
	printf( "%p\n" , obj3);
	cpp_string_free(obj);
	cpp_string_free(obj2);

	cpp_object* obj4 = cpp_string_new("KONNICHIWA");
	cpp_string_move_ptr ( &obj3, &obj4);
	printf( "%s\n" , cpp_string_read (obj3));
	printf( "%s\n" , cpp_string_read (obj4)); // It's deleted

	cpp_object* obj6 = cpp_string_subset(obj3, 3, 5);
	printf( "%s\n" , cpp_string_read (obj6));
	printf( "%p\n" , obj6);

	cpp_object* obj7 = cpp_string_repeat(obj6, 3);
	printf( "%s\n" , cpp_string_read (obj7));
	printf( "%p\n" , obj7);


	cpp_string_free(obj3); // obj5 is also deleted.
	cpp_string_free(obj6);
	cpp_string_free(obj7);
}
