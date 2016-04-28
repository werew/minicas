#ifndef _FUN_H_
#define _FUN_H_

#include "xtypes.h"
#include "ref.h"

/* Command */
typedef struct {
	Ref (*fun) (ref_list);
	ref_t type[];
	} s_cmd, *Cmd;




#endif
