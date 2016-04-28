#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "xtypes.h"


/* Types of variables handled by minicas */
typedef enum { FLOAT, MATRIX } var_t;

/* Variable */
typedef struct {
	void* val;  // Pointer to the stored value
	var_t type; // Type of variable
	} s_var, *Var;

/* Create/destroy variables */
Var new_var(void* val, var_t type);
void drop_var(Var v);

/* Set to and get from global storage */
Ref set_var(char* name, void* val, var_t type);
Ref get_var(const char* name);

#endif








