YACC = bison -y -d
LEX = flex
CC = gcc
CPPC := g++
AR = ar rvs
CFLAGS += -fPIC -O3 -I. -Ivm -Istring -Isimple_re -Isimple_date -I$(ONIG_INCLUDE_DIR) $(CC_USER_DEFINES)
CPPCFLAGS += -std=c++11 -fPIC -O3 -Ivm -Istring $(CPPC_USER_DEFINES)
RM = rm -f

TARGET = libsailr.a
ifeq (Windows_NT,$(OS))
TARGET:=$(subst .a , .lib , $(TARGET))
endif

SRCS=$(filter-out y.tab.c lex.yy.c, $(wildcard *.c)) $(wildcard vm/*.c) 
OBJS:=$(SRCS:.c=.o) parse.o lex.o
OBJS_STR:= string/common_string.o string/cpp_string.o
OBJS_RE:= simple_re/simple_re.o
OBJS_DATE:= simple_date/simple_date.o simple_date/cpp_date.o
SRCS_CFUNC:= $(wildcard vm/func/c_func/*.c)
OBJS_CFUNC:=$(SRCS_CFUNC:.c=.o)
DEPS:=$(OBJS:.o=.d) $(OBJS_STR:.o=.d) $(OBJS_RE:.o=.d) $(OBJS_DATE:.o=.d) $(OBJS_CFUNC:.o=.d)

# List "objfile depends on source and header"
-include $(DEPS)

.PHONY: build test

build : parse.o lex.o  $(TARGET) 

$(TARGET) : $(OBJS)  $(OBJS_STR)  $(OBJS_RE) $(OBJS_DATE) $(OBJS_CFUNC)
	$(AR) $(TARGET) $(OBJS) $(OBJS_STR) $(OBJS_RE) $(OBJS_DATE) $(OBJS_CFUNC)

# yacc creates y.tab.c. -d => y.tab.h. -v => y.output.

parse.o : y.tab.c
	$(CC) -o parse.o -c y.tab.c $(CFLAGS) -MMD -MP 

y.tab.c : parse.y
	yacc -d -v --verbose --debug parse.y

# yacc creates y.tab.c. -d option Creates y.tab.h. -v option creates y.output.
# --debug 

lex.o : lex.l  # lex.yy.c is includgin y.tab.h
	$(LEX) -olex.yy.c lex.l
	$(CC) -o lex.o -c lex.yy.c $(CFLAGS)  # This file requires y.tab.h

%.o : %.c 
	$(CC) -c -o $@ $^  $(CFLAGS) -MMD -MP

vm/%.o : vm/%.c 
	$(CC) -c -o $@ $^  $(CFLAGS) -I. -MMD -MP

string/cpp_string.o : string/cpp_string.cpp
	$(CPPC) -c -o $@ $^ $(CPPCFLAGS) -MMD -MP 

string/%.o : string/%.c 
	$(CC) -c -o $@ $^  $(CFLAGS) -I. -MMD -MP

simple_re/%.o : simple_re/%.c
	$(CC) -c -o $@ $^  $(CFLAGS) -I. -MMD -MP

simple_date/cpp_date.o : simple_date/cpp_date.cpp
	$(CPPC) -c -o $@ $^ $(CPPCFLAGS) -MMD -MP 

simple_date/%.o : simple_date/%.c
	$(CC) -c -o $@ $^  $(CFLAGS) -I. -MMD -MP

test : 
	@echo "\033[1;34m Make sure you have rebuilt $(TARGET) before running tests \033[0m"
	@echo "\033[1;34m CUnit tests for $(TARGET) \033[0m"
	$(CC) test/test_main.c $(TARGET) -o test/run_test -Wall -I. -lstdc++ -lm -L/usr/local/lib -lcunit -Ldev_env/onigmo/lib -lonigmo
	test/run_test

clean :
	$(RM) *.o
	$(RM) vm/*.o
	$(RM) string/*.o
	$(RM) simple_re/*.o
	$(RM) simple_date/*.o
	$(RM) vm/func/c_func/*.o
	$(RM) test/*.o
	$(RM) *.d
	$(RM) vm/*.d
	$(RM) string/*.d
	$(RM) simple_re/*.d
	$(RM) simple_date/*.d
	$(RM) vm/func/c_func/*.d
	$(RM) test/*.d
	$(RM) *.so
	$(RM) vm/*.so
	$(RM) string/*.so
	$(RM) simple_re/*.so
	$(RM) simple_date/*.so
	$(RM) vm/func/c_func/*.so
	$(RM) test/*.so
	$(RM) *.a
	$(RM) vm/*.a
	$(RM) string/*.a
	$(RM) simple_re/*.a
	$(RM) simple_date/*.a
	$(RM) vm/func/c_func/*.a
	$(RM) test/*.a
	$(RM) a.out
	$(RM) vm/a.out
	$(RM) string/a.out
	$(RM) simple_re/a.out
	$(RM) simple_date/a.out
	$(RM) vm/func/c_func/a.out
	$(RM) test/a.out
	$(RM) core
	$(RM) vm/core
	$(RM) string/core
	$(RM) simple_re/core
	$(RM) simple_date/core
	$(RM) vm/func/c_func/core
	$(RM) test/core
	$(RM) y.tab.c y.tab.h y.output
	$(RM) lex.yy.c
	$(RM) test/*.xml
	$(RM) $(TARGET)


