#include <stdio.h>
#include <stdlib.h>
#include "tree_free.h"
#include "tree_dump.h"
#include "node.h"

void
tree_free( TreeNode* nd, int level)
{
  switch(nd->type){
  case NODE_PRGM:
    printf("Free NODE_PRGM\n");
    tree_free(nd->e1.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_PRGM" );
    break;
  case NODE_STMT:
    printf("Free NODE_STMT\n");
    if( nd->e1.nd != NULL )
      tree_free(nd->e1.nd, level );
    if( nd->e3.sibling != NULL)
      tree_free(nd->e3.sibling, level );
    printf( "%s%s\n", rep_spaces(level), "Free NODE_STMT" );
    free(nd);
    break;
  case NODE_INT:
    printf("Free NODE_INT\n");
    printf( "%s%s(%d)\n", rep_spaces(level), "Free NODE_INT", nd->e1.ival );
    free(nd);
    break;
  case NODE_DBL:
    printf("Free NODE_DBL\n");
    printf( "%s%s(%f)\n", rep_spaces(level), "Free NODE_FLOAT", nd->e1.dval );
    free(nd);
    break;
  case NODE_STR:
    printf("Free NODE_STR\n");
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_STR", nd->e1.str_key );
    free(nd);
    break;
  case NODE_REXP:
    printf("Free NODE_REXP\n");
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_REXP", nd->e1.rexp_key );
	free(nd);
    break;
  case NODE_IDENT:
    printf("Free NODE_IDENT\n");
    printf("%s%s(%s)\n", rep_spaces(level),"Free NODE_IDENT", nd->e1.id);
    free(nd);
    break;
  case NODE_FCALL:
    printf("Free NODE_FCALL\n");
    tree_free(nd->e1.nd, level);
    if( ! nd->e3.sibling )
      tree_free(nd->e3.sibling, level + 1);
    printf("%s%s(%s)\n", rep_spaces(level), "Free NODE_FCALL",nd->e1.nd->e1.id);
    free(nd);
    break;
  case NODE_FARG:
    printf("Free NODE_FARG\n");
    if( nd->e1.nd != NULL )
      tree_free(nd->e1.nd, level + 1 );
    if( nd->e3.sibling != NULL )
      tree_free(nd->e3.sibling, level );
    printf( "%s%s\n", rep_spaces(level), "Free NODE_FARG" );
    free(nd);
    break;
  case NODE_OP:
    printf("Free NODE_OP\n");
    if( nd->e2.nd != NULL )
      tree_free(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL )
      tree_free(nd->e3.nd, level + 1);
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_OP", nd->e1.op );
    free(nd);
    break;
  case NODE_UNIOP:
    printf("Free NODE_UNIOP\n");
    if( nd->e2.nd != NULL )
      tree_free(nd->e2.nd, level + 1);
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_UNIOP", nd->e1.op );
    free(nd);
    break;
  case NODE_LET:
    printf("Free NODE_LET\n");
    if( nd->e1.nd != NULL)
      tree_free(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_free(nd->e2.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_LET" );
    free(nd);
    break;
  case NODE_IF:
    printf("Free NODE_IF\n");
    if( nd->e1.nd != NULL)
      tree_free(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_free(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL)
      tree_free(nd->e3.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_IF" );
    free(nd);
    break;
  case NODE_NULL:
    printf( "%s%s\n", rep_spaces(level), "Free NODE_NULL" );
    free(nd);
    break;
  }
}

