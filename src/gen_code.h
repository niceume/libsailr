#ifndef GEN_CODE_H
#define GEN_CODE_H

#include "struct_string.h"
#include "ptr_table.h"
#include "vm_code.h"

vm_inst* gen_code(TreeNode*, ptr_table*);

vm_inst* gen_code_stmt(struct_string*, struct_string*);
vm_inst* gen_code_stmt_nosib(struct_string*);
vm_inst* gen_code_int(TreeNode*);  // Code gen for termainal node receives a pointer to node.
vm_inst* gen_code_double(TreeNode*);  // Code gen for termainal node
vm_inst* gen_code_str(TreeNode*);  // Code gen for termainal node
vm_inst* gen_code_ident(TreeNode*);  // Code gen for termainal node
vm_inst* gen_code_fcall(struct_string*, struct_string* );

vm_inst* gen_code_farg(struct_string*, struct_string*);
vm_inst* gen_code_farg_nosib(struct_string*);

vm_inst* gen_code_op(VM_CMD, vm_inst*, vm_inst*);
vm_inst* gen_code_unitary_op(VM_CMD, vm_inst*);

vm_inst* gen_code_let(vm_inst* , vm_inst* );

vm_inst* gen_code_if_condition(TreeNode*);
vm_inst* gen_code_then_block(TreeNode*);
vm_inst* gen_code_else_block(TreeNode*);

#endif /* GEN_CODE_H */






