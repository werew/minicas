#ifndef _FUN_H_
#define _FUN_H_

#include "xtypes.h"
#include "ref.h"

/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	int n_args;
	ref_t* types[];
	} s_cmd, *Cmd;




#endif
