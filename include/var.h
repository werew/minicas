#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "xtypes.h"


/* Types of variables handled by minicas */
typedef enum { FLOAT, MATRIX } t_var;

/* Variable */
typedef struct {
	void* val;  // Pointer to the stored value
	t_var type; // Type of variable
	} s_var, *Var;

/* Create/destroy variables */
Var new_var(char* name, void* val, t_var type);
void drop_var(Var v);

/* Set to and get from global storage */
Var set_var(char* name, void* val, t_var type);
Var get_var(const char* name);

#endif








