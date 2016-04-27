#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "xtypes.h"


/* Types of value handled by minicas */
typedef enum { FLOAT, MATRIX } t_var;

/* Variable */
typedef struct {
	char* name; // Null terminated name
	void* val;  // Pointer to the stored value
	t_var type; // Type of variable
	} s_var, *Var;


/* Vars list */
typedef struct {
	Var* var_list; // List of the stored variables
	unsigned int length; // Number of stored variables
	unsigned int max; // Storage capacity
	} s_v_list, *v_list;


/* Global storage buffer */
#define N_VLIST 8
extern v_list storage_buff[N_VLIST];

/* Create/destroy variables */
Var new_var(char* name, void* val, t_var type);
void drop_var(Var v);

/* Set to and get from global storage */
Var set_var(char* name, void* val, t_var type);
Var get_var(const char* name);

/* Vars list operations */
v_list new_v_list(void);
void drop_v_list(v_list l, bool unnamed_only);
Var push_var(v_list l, const Var v);

/* Hash generation */
unsigned int hash(const char* key);

#endif








