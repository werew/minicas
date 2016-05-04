#ifndef _REF_H_
#define _REF_H_

#include "xtypes.h"


/* Empty reference */
#define NO_REF ((Ref) 0x1)

/* Types of references handled by minicas */
typedef enum { VAR, FUN, CMD, ALL } ref_t;

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
#define N_RLIST 32
extern ref_list ref_pool[N_RLIST];

/* Create/destroy references */
Ref new_ref(char* name, void* inst, ref_t type);
void drop_ref(Ref r);

/* Set to and get references from global pool */
Ref set_ref(char* name, void* inst, ref_t type);
Ref get_ref(const char* name);
Ref get_reft(const char* name, ref_t type);


/* Vars list operations */
ref_list new_ref_list(void);
void drop_ref_list(ref_list l, bool unnamed_only);
Ref push_ref(ref_list l, Ref r);
Ref replace_ref_at(ref_list l, unsigned int i, Ref r);
int search_ref(const ref_list l, const char* name);

/* Various */
unsigned int hash(const char* key);
void print_ref(Ref r);

#endif








