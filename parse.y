%{
#include <stdio.h>
#include "node.h"
#include "parser_state.h"
#include "common_string.h"

int yydebug = 0; /* 0 : no debug, 1: debug */

%}

/* Define how each pseudo-variable returns values */
/* This part defines YYSTYPE */
%union {
  TreeNode* nd;
  string_object* str;
  char* id;
}

/* Options about parser function  and how to interact with yylex() */
/* pure-parser makes variables local. Note that currenlty pusre-parser is deprecated, and %define api.pure full is strongly recommended. */
/* (ref) https://homeunix.nl/manual/bison/NEWS */
/* To share values with another binary,*/
/* you need to pass it via function arguments. */
/* parse-param: argument for yyparse() and yyerror() definition. */
/* Then you can use p in actions from yyparse() .*/
/* Also you can write "int yyerror(parser_state* p , char* str){} "*/
/* lex-param: additional argument to pass  when calling yylex(). */
%define api.pure full
/*(ref) https://stackoverflow.com/questions/12468282/making-bison-flex-parser-reentrant-with-integral-yystype*/
%parse-param {parser_state *p } {void* scanner } 
/* This results in adding "parser_state* " argument to yyparse() */
/* yyerror() also takes the same parameters from parse-param*/
%lex-param {parser_state *p } {void* scanner} 
/* This results in adding "parser_state* " argument to yylex() */
/* About %parse-param and %lex-param, */
/* See this ref. https://stackoverflow.com/questions/34418381/how-to-reference-lex-or-parse-parameters-in-flex-rules */

%{
// Prevent undefined reference warnings
// (ref) https://stackoverflow.com/questions/23717039/generating-a-compiler-from-lex-and-yacc-grammar
// (ref) https://stackoverflow.com/questions/28643114/how-to-use-flex-with-my-own-parser
int yylex (YYSTYPE* yylval, parser_state* p, void* scanner);
void yyerror (parser_state* p, void* scanner, char const *);
%}


/* ***************************** */
/*  Non-terminals                */
/*  Types of pseudo-variables    */
/* ***************************** */

%type<nd> prgm stmts stmt expr arg primary fcall args
%type<nd> fname
%type<nd> if_stmt condition then_stmts opt_else
%type<nd> assign_stmt lvar

/* ***************************** */
/*  Terminals & Operators        */
/*  Types of pseudo-variables    */
/* ***************************** */

/* All the tokens that are returned by yylex() should be listed. */
%token<nd> LIT_NUM
%token<nd> NA_NUM
%token<str> LIT_STR
%token<str> LIT_REXP
%token<id> IDENT
%token KEY_IF KEY_ELSE
%token ASSIGN
%token TERMIN
%token PLCUR PRCUR COMMA

/* Operators */
/* Defnition of associativity */
/* Latter rules have higher priority. */

/* Logical Operators*/
%left<nd> OR
%left<nd> AND
%nonassoc<nd> OP_EQ OP_NEQ REXP_MATCH
%left<nd> OP_LT OP_LE OP_GT OP_GE

/* Numeric Operators*/
/* UMINUS is special. No such token exits. */
%left<nd> OP_PLUS OP_SUB 
%left<nd> OP_MULT OP_DIV OP_MOD
%right<nd> OP_POWER
%left<nd> FACTOR
%right<nd> UMINUS  /* https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.bpxa600/bpxa698.htm */

%%

program	: prgm					{ p->tree = new_node_prgm( $1 );}

prgm		: opt_termins stmts opt_termins	{ $$ = $2;  }
			| opt_termins			{ $$ = NULL; }

stmts		: stmt					{ $$ = new_node_stmt($1); }
			| stmts termins stmt
					{
					$$ = pushback_node_stmt($1, new_node_stmt($3));
					}

stmt		: assign_stmt			{ /*printf("ASSIGN STMT!!!"); */ $$ = $1; }
			| if_stmt				{ /*printf("IF STMT!!!"); */ $$ = $1; }
			| expr					{ /*printf("JUST STMT!!!"); */ $$ = $1; }

expr		: expr AND expr		{ $$ = new_node_op("AND", $1, $3); }
			| expr OR expr			{ $$ = new_node_op("OR", $1, $3); }
			| arg					{ $$ = $1; }

arg		: fcall				{ $$ = $1; }
			| arg OP_PLUS arg		{ $$ = new_node_op("PLUS", $1, $3); }
			| arg OP_SUB arg		{ $$ = new_node_op("SUB", $1, $3); }
			| arg OP_MULT arg		{ $$ = new_node_op("MULT", $1, $3); }
			| arg OP_DIV arg		{ $$ = new_node_op("DIV", $1, $3); }
			| arg OP_MOD arg		{ $$ = new_node_op("MOD", $1, $3); }
			| arg FACTOR			{ $$ = new_node_uniop("FACTOR", $1); }
			| arg OP_POWER arg		{ $$ = new_node_op("POWER", $1, $3); }
			| arg OP_EQ arg		{ $$ = new_node_op("EQ", $1, $3); }
			| arg OP_NEQ arg		{ $$ = new_node_op("NEQ", $1, $3); }
			| arg OP_GT arg		{ $$ = new_node_op("GT", $1, $3); }
			| arg OP_LT arg		{ $$ = new_node_op("LT", $1, $3); }
			| arg OP_GE arg		{ $$ = new_node_op("GE", $1, $3); }
			| arg OP_LE arg		{ $$ = new_node_op("LE", $1, $3); }
			| OP_SUB arg %prec UMINUS	{ $$ = new_node_uniop("UMINUS", $2); }
			| primary REXP_MATCH primary		{ $$ = new_node_op("REXP_MATCH", $1, $3); }
			| primary				{ $$ = $1; }

primary	: IDENT	
			{
			$$ = new_node_ident( $1 ); 
			var_hash_add_name( &(p->vars) , $1 );
			var_hash_add_name( &(p->rhsvars) , $1 );
			}
			| LIT_NUM				{ $$ = $1; }
			| LIT_STR				{ $$ = new_node_str( $1 , p->ptrtable ); }
			| LIT_REXP				{ $$ = new_node_rexp( $1 , p->ptrtable , p->rexp_encoding ); }
			| '(' expr ')'			{ $$ = $2; }
			| NA_NUM				{ $$ = $1; }

fcall		: fname '(' args ')'	{ $$ = new_node_fcall($1, $3); }

fname		: IDENT				{ $$ = new_node_ident($1); }

args		: /* empty */  		{ $$ = new_node_null(); }
			| expr				{ 
								$$ = new_node_farg($1); 
								}
			| args COMMA expr
					{
					$$ = pushback_node_farg($1, new_node_farg($3));
					}

//if_stmt	: KEY_IF condition then_stmts opt_else
//					{
//					$$ = new_node_if( $2, $3, $4 );
//					}
if_stmt	: KEY_IF condition then_stmts opt_else
					{
					$$ = new_node_if( $2, $3, $4 );
					}

condition	: '(' expr ')' opt_termin		{ $$ = $2 ; }

then_stmts	: stmt TERMIN					{ $$ = $1; }
			| '{' prgm '}'		{ $$ = $2; }
//			| '{' prgm '}' opt_termin		{ $$ = $2; }

opt_else	: { $$ = NULL; }
			| KEY_ELSE stmt			{ $$ = $2; }
			| KEY_ELSE '{' prgm '}'	{ $$ = $3; }
//			| TERMIN KEY_ELSE '{' prgm '}' { $$ = $4; }

assign_stmt	: lvar ASSIGN expr	{ $$ = new_node_let($1, $3); }

lvar			: IDENT
					{
					$$ = new_node_ident( $1 );
					var_hash_add_name( &(p->vars) , $1 );
					var_hash_add_name( &(p->lhsvars) , $1 );
					}

opt_termin		: /* empty */
				| TERMIN

opt_termins	: /* empty */
				| termins

termins		: TERMIN termins
				| TERMIN


%%

void yyerror(parser_state* p, void* scanner, char const* s)
{
  p->yynerrs++;
  fprintf(stderr, "%d: %s\n", p->lineno, s);
  /* yylineno does not seem to work in reentrant parser, b/c it's global.*/
  /* Instead, the line number is set in parser_state by tokenizer or scanner. */
}

