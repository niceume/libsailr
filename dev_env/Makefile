.PHONY: build build_onigmo build_lib clean clean_onigmo clean_lib test_lib 

build: build_onigmo build_lib
	./compile_cpp_parser.sh

build_onigmo:
	./compile_onigmo_if_needed.sh

build_lib:
	make --directory ../ build  ONIG_INCLUDE_DIR=dev_env/onigmo_build/include CC_USER_DEFINES="-Wreturn-type -Wparentheses -Wunused-value" ## CC_USER_DEFINES="-DDEBUG"


clean: clean_onigmo clean_dev clean_lib

clean_onigmo:  
	if [ -f "" ]; then \
		make --directory=onigmo_src/ distclean; \
	fi
	rm -fR ./onigmo_build/lib/*
	rm -fR ./onigmo_build/include/*
	rm -fR ./onigmo_src/enc/*.lo
	rm -fR ./onigmo_src/enc/*.o
	rm -fR ./onigmo_src/sample/*.lo
	rm -fR ./onigmo_src/sample/*.o
	rm -fR ./onigmo_src/*.la
	rm -fR ./onigmo_src/*.lo
	rm -fR ./onigmo_src/*.o

clean_dev:
	rm -fR ./*.o
	rm -fR ./a.out
	rm -fR ./core
	rm -fR ./myparsercpp


clean_lib:
	make --directory ../ clean


test_lib:
	make --directory ../ test
