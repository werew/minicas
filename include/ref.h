#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "xtypes.h"


/* Types of references handled by minicas */
typedef enum { VAR, FUN, CMD } ref_t;

/* Reference */
typedef struct {
	char* name; // Null terminated name
	void* inst; // Pointer to the instance of this reference
	ref_t type; // Type of reference
	} s_ref, *Ref;


/* Reference list */
typedef struct {
	Ref* list; 	     // List of the stored references
	unsigned int length; // Number of stored references
	unsigned int max;    // Storage capacity
	} s_ref_list, *ref_list;


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








