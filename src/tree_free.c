#include <stdio.h>
#include <stdlib.h>
#include "tree_free.h"
#include "node.h"

void
tree_free( TreeNode* nd, int level)
{
  switch(nd->type){
  case NODE_PRGM:
    tree_free(nd->e1.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_PRGM" );
    break;
  case NODE_STMT:
    if( nd->e1.nd != NULL )
      tree_free(nd->e1.nd, level );
    if( nd->e3.sibling != NULL)
      tree_free(nd->e3.sibling, level );
    printf( "%s%s\n", rep_spaces(level), "Free NODE_STMT" );
    free(nd);
    break;
  case NODE_INT:
    printf( "%s%s(%d)\n", rep_spaces(level), "Free NODE_INT", nd->e1.ival );
    free(nd);
    break;
  case NODE_DBL:
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_FLOAT", nd->e1.dval );
    free(nd);
    break;
  case NODE_STR:
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_STR", nd->e1.str_key );
    free(nd);
    break;
  case NODE_IDENT:
    printf("%s%s(%s)\n", rep_spaces(level),"Free NODE_IDENT", nd->e1.id);
    free(nd);
    break;
  case NODE_FCALL:
    tree_free(nd->e1.nd, level);
    if( ! nd->e3.sibling )
      tree_free(nd->e3.sibling, level + 1);
    printf("%s%s(%s)\n", rep_spaces(level), "Free NODE_FCALL",nd->e1.op);
    free(nd);
    break;
  case NODE_FARG:
    if( nd->e1.nd != NULL )
      tree_free(nd->e1.nd, level + 1 );
    if( nd->e3.sibling != NULL )
      tree_free(nd->e3.sibling, level );
    printf( "%s%s\n", rep_spaces(level), "Free NODE_FARG" );
    free(nd);
    break;
  case NODE_OP:
    if( nd->e2.nd != NULL )
      tree_free(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL )
      tree_free(nd->e3.nd, level + 1);
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_OP", nd->e1.op );
    free(nd);
    break;
  case NODE_UNIOP:
    if( nd->e2.nd != NULL )
      tree_free(nd->e2.nd, level + 1);
    printf( "%s%s(%s)\n", rep_spaces(level), "Free NODE_UNIOP", nd->e1.op );
    free(nd);
    break;
  case NODE_LET:
    if( nd->e1.nd != NULL)
      tree_free(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_free(nd->e2.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_LET" );
    free(nd);
    break;
  case NODE_IF:
    if( nd->e1.nd != NULL)
      tree_free(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_free(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL)
      tree_free(nd->e3.nd, level + 1);
    printf( "%s%s\n", rep_spaces(level), "Free NODE_IF" );
    free(nd);
    break;
  }
}

