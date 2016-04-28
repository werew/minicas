#ifdef _FUN_H_
#define _FUN_H_

#include "xtypes.h"
#include "ref."

/* Function */
typedef struct {
	Ref (*fun) (ref_list);
	ref_list args;
	} s_fun, *Fun;










#endif
