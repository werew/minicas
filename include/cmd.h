#ifndef _CMD_H_
#define _CMD_H_

#include <stdarg.h>
#include "xtypes.h"
#include "ref.h"


/* Argument types */
extern ref_t A_FLOAT [];
extern ref_t A_MATRIX[];
extern ref_t A_FUN[];
extern ref_t A_CMD[];
extern ref_t A_ALL[];

/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	int n_args;
	ref_t** types;
	} s_cmd, *Cmd;


/* Create/destroy command */
Cmd new_cmd(Ref (*fun) (ref_list), int n_args, ref_t** types);
void drop_cmd(Cmd c);
Ref new_cref(char* name,Ref (*fun) (ref_list), int n_args, ref_t** types);
Ref set_cref(char* name,Ref (*fun) (ref_list), int n_args, ref_t** types);
Ref get_cmd(const char* name);

#endif
