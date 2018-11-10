#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "ptr_table.h"

/* Struct for parser state */

typedef struct {
  char* fname;
  void *lval;
  int lineno;
  int tline;
  int yynerrs;
  ptr_table* ptrtable;
} parser_state;

parser_state* new_parser_state();

#endif /* PARSER_STATE_H */

