#include <stdio.h>
#include <stdlib.h>
#include "tree_dump.h"
#include "node.h"
#include "helper.h"

char*
rep_spaces( int rep )
{
  int len = 2 * rep;
  char* spaces = (char*)malloc(sizeof(char)*(len+1));
  int i ;
  for(i = 0; i < len; i++){
    spaces[i] = ' ';
  }
  spaces[len] = '\0';
  return spaces;
}

void
tree_dump( TreeNode* nd, int level )
{
  char* prefix_space ;

  switch(nd->type){
  case NODE_PRGM:
    printf( "%s%s\n", rep_spaces(level), "NODE_PRGM" );
    tree_dump(nd->e1.nd, level + 1);
    break;
  case NODE_STMT:
    printf( "%s%s\n", rep_spaces(level), "NODE_STMT" );
    if( nd->e1.nd != NULL )
      tree_dump(nd->e1.nd, level + 1 );
    if( nd->e3.sibling != NULL)
      tree_dump(nd->e3.sibling, level + 1);
    break;
  case NODE_INT:
    printf( "%s%s(%d)\n", rep_spaces(level), "NODE_INT", nd->e1.ival );
    break;
  case NODE_DBL:
    printf( "%s%s(%f)\n", rep_spaces(level), "NODE_FLOAT", nd->e1.dval );
    break;
  case NODE_STR:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_STR", nd->e1.str_key );
    break;
  case NODE_REXP:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_REXP", nd->e1.rexp_key );
    break;
  case NODE_IDENT:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_IDENT", nd->e1.id );
    break;
  case NODE_FCALL:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_FCALL" , nd->e1.nd->e1.id );
    if( nd->e3.nd != NULL )
      tree_dump(nd->e3.nd, level + 1);
    break;
  case NODE_FARG:
    printf( "%s%s\n", rep_spaces(level), "NODE_FARG" );
    if( nd->e1.nd != NULL )
      tree_dump(nd->e1.nd, level + 1 );
    if( nd->e3.sibling != NULL )
      tree_dump(nd->e3.sibling, level );
    break;
  case NODE_OP:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_OP", nd->e1.op );
    if( nd->e2.nd != NULL )
      tree_dump(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL )
      tree_dump(nd->e3.nd, level + 1);
    break;
  case NODE_UNIOP:
    printf( "%s%s(%s)\n", rep_spaces(level), "NODE_UNIOP", nd->e1.op );
    if( nd->e2.nd != NULL )
      tree_dump(nd->e2.nd, level + 1);
    break;
  case NODE_LET:
    printf( "%s%s\n", rep_spaces(level), "NODE_LET" );
    if( nd->e1.nd != NULL)
      tree_dump(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_dump(nd->e2.nd, level + 1);
    break;
  case NODE_IF:
    printf( "%s%s\n", rep_spaces(level), "NODE_IF" );
    if( nd->e1.nd != NULL)
      tree_dump(nd->e1.nd, level + 1);
    if( nd->e2.nd != NULL)
      tree_dump(nd->e2.nd, level + 1);
    if( nd->e3.nd != NULL)
      tree_dump(nd->e3.nd, level + 1);
    break;
  case NODE_NULL:
    printf( "%s%s\n", rep_spaces(level), "NODE_NULL" );
    break;
  }
}


