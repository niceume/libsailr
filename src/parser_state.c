#include <stdlib.h>
#include "parser_state.h"

parser_state*
new_parser_state(char* fname, ptr_table* table)
{
	parser_state* ps = (parser_state*)malloc(sizeof(parser_state));
	ps->fname = fname;
	ps->lval = NULL;
	ps->lineno = 0;
	ps->tline = 0;
	ps->yynerrs = 0;
	ps->ptrtable = table;
	return ps;
}
