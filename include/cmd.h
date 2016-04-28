#ifndef _FUN_H_
#define _FUN_H_

#include "xtypes.h"
#include "ref.h"

typedef struct argt {
	ref_t type;
	struct argt* sub;
	} arg_t;

/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	int n_args;
	arg_t type[];
	} s_cmd, *Cmd;







#endif
