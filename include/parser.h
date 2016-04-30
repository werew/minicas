#ifndef _PARSER_H_
#define _PARSER_H_

#include "xtypes.h"
#include "ref_all.h"


Matrix ref_list2vect(ref_list l); // Should this stay here?

/* Internal calls */
Ref declare_ref(char* name);
int exec_cmd(char* cmd);
Ref exec_fun(char* fun, ref_list args);

/* Parse-helpers */
int (*get_classtest(c_class class)) (int);
int isalnum_extended(int c);
char* pop_word(void);
char* jump_cclass(char* ptr, c_class class);

/* Expression evaluators */
int eval_input(char* user_input);
int exec_instrution(void);
Ref eval_expression(void);
Ref eval_float(void);
Ref eval_vector(void);
Ref eval_fun(char* fun);

#endif

