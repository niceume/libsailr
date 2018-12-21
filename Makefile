YACC = bison -y -d
LEX = flex
CC = gcc
CXX = g++
AR = ar rvs
CFLAGS += -fPIC -O3 -Ivm -Istring
CXXFLAGS += -fPIC -O3 -Ivm -Istring
RM = rm -f

TARGET = libsailr.a
ifeq (Windows_NT,$(OS))
TARGET:=$(subst .a , .lib , $(TARGET))
endif

SRCS=$(filter-out y.tab.c lex.yy.c, $(wildcard *.c)) $(wildcard vm/*.c) $(wildcard string/*.c)
OBJS:=$(SRCS:.c=.o) parse.o lex.o
OBJS_STR:= string/common_string.o string/cpp_string.o
DEPS:=$(OBJS:.o=.d)

# List "objfile depends on source and header"
-include $(DEPS)

.PHONY: build

build : parse.o lex.o  $(TARGET) 

$(TARGET) : $(OBJS)  $(OBJS_STR)
	$(AR) $(TARGET) $(OBJS) $(OBJS_STR)

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
	$(CXX) -c -o $@ $^ $(CXXFLAGS) -MMD -MP

string/%.o : string/%.c 
	$(CC) -c -o $@ $^  $(CFLAGS) -I. -MMD -MP



clean :
	$(RM) *.o
	$(RM) vm/*.o
	$(RM) string/*.o
	$(RM) *.d
	$(RM) vm/*.d
	$(RM) string/*.d
	$(RM) *.so
	$(RM) *.a
	$(RM) y.tab.c y.tab.h y.output
	$(RM) lex.yy.c
	$(RM) a.out
	$(RM) $(TARGET)
