#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "ptr_table.h"
#include "var_hash.h"

/* Struct for parser state */

typedef struct {
  char* fname;
  void *tree;
  int lineno;
  int tline;
  int yynerrs;
  ptr_table* ptrtable;
  var_hash* vars;
  var_hash* lhsvars;
  var_hash* rhsvars;
} parser_state;

parser_state* new_parser_state(char* fname, ptr_table* table);

#endif /* PARSER_STATE_H */

