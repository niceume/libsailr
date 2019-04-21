#include <stdlib.h>
#include "parser_state.h"
#include "var_hash.h"

parser_state*
new_parser_state(char* fname, ptr_table* table)
{
	parser_state* ps = (parser_state*)malloc(sizeof(parser_state));
	ps->fname = fname;
	ps->tree = NULL;
	ps->lineno = 0;
	ps->tline = 0;
	ps->yynerrs = 0;
	ps->ptrtable = table;
	ps->vars = var_hash_init();
	ps->lhsvars = var_hash_init();
	ps->rhsvars = var_hash_init();
	return ps;
}

int
parser_state_free(parser_state* ps)
{
	free(ps->fname); // Free fname
	// Tree and ptrtable are already freed.
	var_hash_free(&(ps->vars)); // Free vars
	var_hash_free(&(ps->lhsvars)); // Free lhsvars
	var_hash_free(&(ps->rhsvars)); // Free rhsvars
	free(ps);
	return 1;
}
