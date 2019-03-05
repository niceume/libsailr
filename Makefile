YACC = bison -y -d
LEX = flex
CC = gcc
CPPC := g++
AR = ar rvs
CFLAGS += -fPIC -O3 -Ivm -Istring -Isimple_re -I$(ONIG_INCLUDE_DIR)
CPPCFLAGS += -fPIC -O3 -Ivm -Istring
RM = rm -f

TARGET = libsailr.a
ifeq (Windows_NT,$(OS))
TARGET:=$(subst .a , .lib , $(TARGET))
endif

SRCS=$(filter-out y.tab.c lex.yy.c, $(wildcard *.c)) $(wildcard vm/*.c) # $(wildcard string/*.c)
OBJS:=$(SRCS:.c=.o) parse.o lex.o
OBJS_STR:= string/common_string.o string/cpp_string.o
OBJS_RE:= simple_re/simple_re.o
SRCS_CFUNC:= $(wildcard vm/func/c_functions/*.c)
OBJS_CFUNC:=$(SRCS_CFUNC:.c=.o)
DEPS:=$(OBJS:.o=.d) $(OBJS_STR:.o=.d) $(OBJS_RE:.o=.d) $(OBJS_CFUNC:.o=.d)

# List "objfile depends on source and header"
-include $(DEPS)

.PHONY: build

build : parse.o lex.o  $(TARGET) 

$(TARGET) : $(OBJS)  $(OBJS_STR)  $(OBJS_RE)  $(OBJS_CFUNC)
	$(AR) $(TARGET) $(OBJS) $(OBJS_STR) $(OBJS_RE) $(OBJS_CFUNC)

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


clean :
	$(RM) *.o
	$(RM) vm/*.o
	$(RM) string/*.o
	$(RM) simple_re/*.o
	$(RM) vm/func/c_functions/*.o
	$(RM) *.d
	$(RM) vm/*.d
	$(RM) string/*.d
	$(RM) simple_re/*.d
	$(RM) vm/func/c_functions/*.d
	$(RM) *.so
	$(RM) vm/*.so
	$(RM) string/*.so
	$(RM) simple_re/*.so
	$(RM) vm/func/c_functions/*.so
	$(RM) *.a
	$(RM) vm/*.a
	$(RM) string/*.a
	$(RM) simple_re/*.a
	$(RM) vm/func/c_functions/*.a
	$(RM) a.out
	$(RM) vm/a.out
	$(RM) string/a.out
	$(RM) simple_re/a.out
	$(RM) vm/func/c_functions/a.out
	$(RM) core
	$(RM) vm/core
	$(RM) string/core
	$(RM) simple_re/core
	$(RM) vm/func/c_functions/core
	$(RM) y.tab.c y.tab.h y.output
	$(RM) lex.yy.c
	$(RM) $(TARGET)


