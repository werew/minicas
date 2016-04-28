#ifndef _PARSER_H_
#define _PARSER_H_

#include "xtypes.h"
#include "var.h"
#include "ref.h"

/* 
 * FIXME temporary definitions 
 * waiting for the merge with branch matrix
 */
typedef float* Matrix; 

int (*get_classtest(c_class class)) (int);
int eval_input(char* user_input);
char* pop_word(void);
char* jump_cclass(char* ptr, c_class class);
Ref exec_fun(char* fun, ref_list args);
Ref eval_fun(char* fun);
int exec_cmd(char* cmd);
Ref declare_ref(char* name);
Ref eval_expression(void);
Matrix eval_vector(char* vect);
int exec_instrution(void);

#endif

