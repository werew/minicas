#ifndef _FUN_H_
#define _FUN_H_

#include <stdarg.h>
#include "xtypes.h"
#include "ref.h"

/* Argument types */
ref_t A_FLOAT [] = { VAR, FLOAT, ALL };
ref_t A_MATRIX[] = { VAR, MATRIX, ALL };
ref_t A_FUN[] = { FUN, ALL };
ref_t A_CMD[] = { CMD, ALL };
ref_t A_ALL[] = { ALL };

/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	int n_args;
	ref_t** types;
	} s_cmd, *Cmd;


/* Create/destroy command */
Cmd new_cmd(Ref (*fun) (ref_list), int n_args, ref_t** types);


#endif
