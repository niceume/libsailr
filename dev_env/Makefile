CC="gcc"
CXX="g++"
AR="ar"
AR_RVS="ar rvs"
NM="nm"
RANLIB="ranlib"
RM="rm -f"

ifneq (,$(findstring clang, $(COMPILER)))
  $(info $(COMPILER))
  $(info $(subst clang,,$(COMPILER)))
  CLANG_VERSION=$(subst clang,,$(COMPILER))
  # Found
  CC="clang"$(CLANG_VERSION)
  CXX="clang++"$(CLANG_VERSION)
  AR="llvm-ar"$(CLANG_VERSION)
  AR_RVS="llvm-ar"$(CLANG_VERSION)" rvs"
  NM="llvm-nm"$(CLANG_VERSION)
  RANLIB="llvm-ranlib"$(CLANG_VERSION)
  RM="rm -f"
else
  # Not found
endif

.PHONY: build build_onigmo build_lib clean clean_onigmo clean_lib test_lib 

build: build_onigmo build_lib
	CXX=$(CXX) ./compile_cpp_parser.sh

build_onigmo:
	CC=$(CC) CXX=$(CXX) AR=$(AR) RANLIB=$(RANLIB) NM=$(NM) ./compile_onigmo_if_needed.sh

build_lib:
	make --directory ../ build  ONIG_INCLUDE_DIR=dev_env/onigmo_build/include CC=$(CC) CXX=$(CXX) AR=$(AR_RVS) CC_USER_DEFINES="-Wall -Wextra -DDEBUG -Wstrict-prototypes" ## CC_USER_DEFINES="-DDEBUG"


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
