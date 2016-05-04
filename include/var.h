#ifndef _VAR_H_
#define _VAR_H_

#include "xtypes.h"
#include "ref.h"


/* Types of variables handled by minicas */
typedef enum { FLOAT = 10,  // Range < 10 reserved 
	       MATRIX } var_t;

/* Variable */
typedef struct {
	int _ptrs;
	void* val;  // Pointer to the stored value
	var_t type; // Type of variable
	} s_var, *Var;

/* Create/destroy variables */
Var new_var(void* val, var_t type);
void drop_var(Var v);
Ref new_vref(char* name, void* val, var_t type);

/* Set to and get from global storage */
Ref set_var(char* name, void* val, var_t type);
Ref get_var(const char* name);

void print_var(Var v);
bool v_isa(Ref r, var_t type);

#endif








