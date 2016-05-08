#ifndef _INTERNALS_H_
#define _INTERNALS_H_



void load_commands(void);
/* Functions */
Ref compose_call(ref_list args);

/* Commands */
Ref quit_cmd(ref_list args);
Ref print__cmd(ref_list args);

#endif

