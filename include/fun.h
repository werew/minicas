#ifndef _FUN_H_
#define _FUN_H_

#include "xtypes.h"
#include "ref.h"

/* Function */
typedef struct {
	Ref (*fun) (ref_list);
	ref_list args;
	} s_fun, *Fun;





Fun new_fun(Ref (*fun) (ref_list), ref_list args);
void drop_fun(Fun f);
Ref new_fref(char* name,Ref (*fun) (ref_list), ref_list args);





#endif
