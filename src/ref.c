#include <string.h>
#include <stdlib.h>
#include "xtypes.h"
#include "var.h"
#include "ref.h"


/* Global storage buffer */
ref_list ref_pool[N_RLIST];

/**
 * Drop a stored reference
 * @param r Reference to drop
 */
void drop_ref(Ref r){
	if (r == NULL) return;
	
	switch (r->type) {
		case VAR: drop_var(r->inst);
			  break;
	/* TODO add missing drop functions */
		case FUN: //drop_fun(r->inst);
			  break;
		case CMD: //drop_cmd(r->inst);
			  break;
	}
	free(r->name);
	free(r);
}

/**
 * Destroy a ref_list and all the references that it constains.
 * @param l ref_list to drop
 * @param unnamed_only If true only the unnamed references will be dropped
 */
void drop_ref_list(ref_list l, bool unnamed_only){

	unsigned int i;
	for ( i = 0; i < l->length; i++){
		if (unnamed_only && l->list[i]->name != NULL) continue;
		drop_ref(l->list[i]);
	}
	
	free(l->list);
	free(l);
}


/**
 * Creates a new reference containing the given informations
 * @param name Name of the reference
 * @param inst Address to the instance of the reference
 * @param type Type of the reference 
 * @return A pointer to the newly created reference
 *	   or NULL in case of error
 */
Ref new_ref(char* name, void* inst, ref_t type){
	Ref r = malloc( sizeof (s_ref));
	if (r == NULL) return NULL;

	r->name = name;
	r->type = type;
	r->inst = inst;
	
	return r;
}
		
/**
 * Creates a new empty (zero-initialized) ref_list
 * @return The newly created ref_list
 */
ref_list new_ref_list(void){
	ref_list l = malloc( sizeof (s_ref_list));
	if (l == NULL) return NULL;
	memset(l, 0, sizeof (s_ref_list));
	return l;
}


/**
 * Search a reference inside the given ref_list
 * @param l The list where to search the reference
 * @param name The name of the reference to search
 * @param type The type of the reference to search
 * @return The index representing the position of the reference in relation
 *	   to l->ref_list or -1 if the reference has not been found
 */
int search_ref(const ref_list l, const char* name, ref_t type){
	unsigned int i;
	for (i = 0; i < l->length; i++){
		Ref r = l->list[i];
		if ( r->type == type && 
		     strcmp(r->name, name) == 0){
			return i;
		}
	}
	
	return -1;
}

/**
 * Insert the given reference at the end of the given ref_list
 * @param l The list where to insert r
 * @param r The reference to insert
 * @return The newly pushed reference in case of success or
 *	   NULL in case of error
 */
Ref push_ref(ref_list l, Ref r){
	#define ALLOC_UNIT 8
	if (l->list == NULL || l->length >= l->max){
		Ref* tmp = realloc(l->list,
			l->max + ALLOC_UNIT * sizeof (Ref));
		if (tmp == NULL) return NULL;
	
		l->list = tmp;
		l->max += ALLOC_UNIT;
	}
	
	l->list[l->length++] = r;
	return r;
}
	
	
/**
 * Replace the reference at the given position of the ref_list with a new one
 * @param r The reference to insert
 * @param i The index representing a valid position of a reference to replace
 * @param l The list where to insert r
 * @return The newly inserted reference or NULL in case of error
 * @note At the moment the function doesn't imply any possible return of NULL
 *	 howover this return value needs to be considered for compatibility
 *	 with future versions of this function
 */
Ref replace_ref_at(ref_list l, unsigned int i, Ref r){
	drop_ref(l->list[i]);
	l->list[i] = r;
	return r;
}

/**
 * Find and get a stored reference from the global pool
 * @param name The name of the reference to get
 * @param type The type of the reference to get
 * @return The reference or NULL if any reference having the
 *	   given name and type has been found
 */
Ref get_ref(const char* name, ref_t type){
	unsigned int h = hash(name);
	if (ref_pool[h] == NULL) return NULL;
	
	int i = search_ref(ref_pool[h], name, type);
	if (i == -1) return NULL;

	return ref_pool[h]->list[i];
}

/**
 * Create and store or update a reference in to the global pool
 * @param name Name of the reference to store/update
 * @param inst Pointer to the instance of the reference
 * @param type Tge type of the reference
 * @return The address of the newly stored/updated reference,
 *	   or NULL in case of error
 */
Ref set_ref(char* name, void* inst, ref_t type){

	unsigned int h = hash(name);
	if (ref_pool[h] == NULL){
		ref_pool[h] = new_ref_list();
		if (ref_pool[h] == NULL) return NULL;
	}	
		
	Ref r = new_ref(name, inst, type);
	if (r == NULL) return NULL;

	int i;	
	if ( (i = search_ref(ref_pool[h], name, type)) == -1){

		if (push_ref(ref_pool[h], r) == NULL) 
			goto e_r_lost;

	} else {
		
		if (replace_ref_at(ref_pool[h], i, r) == NULL) 
			goto e_r_lost;
	}
	
	return r;

e_r_lost:
	drop_ref(r);
	return NULL;
}
	
/* FNV-1a hash */
unsigned int hash(const char* key){
	unsigned int hash = 2166136261;
	while (*key != '\0') {
		hash ^= *key++;
		hash *= 16777619;
		hash %= N_RLIST;
	}
	return hash;
}
