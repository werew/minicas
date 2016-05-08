#ifndef _INTERNALS_H_
#define _INTERNALS_H_


typedef enum { EQUAL, NEQUAL, GETR, LESS } Condition;

void load_commands(void);
/* Functions */
Ref compose_call(ref_list args);
Ref eval_in_call(ref_list args);
Ref eval_if_call(ref_list args, Condition c);
Ref eval_ifeq_call(ref_list args);
Ref eval_ifneq_call(ref_list args);

/* Commands */
Ref quit_cmd(ref_list args);
Ref print_fc(ref_list args);

#endif

