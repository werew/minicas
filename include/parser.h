#ifndef _PARSER_H_
#define _PARSER_H_

#include "xtypes.h"
#include "matrix.h"
#include "var.h"
#include "ref.h"

int (*get_classtest(c_class class)) (int);
int eval_input(char* user_input);
char* pop_word(void);
char* jump_cclass(char* ptr, c_class class);
Ref exec_fun(char* fun, ref_list args);
Ref eval_fun(char* fun);
int exec_cmd(char* cmd);
Ref declare_ref(char* name);
Ref eval_expression(void);
Matrix ref_list2vect(ref_list l);
Ref eval_vector(void);
int exec_instrution(void);

#endif

