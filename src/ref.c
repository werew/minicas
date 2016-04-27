#include <string.h>
#include <stdlib.h>
#include "xtypes.h"
#include "storage.h"


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
	/* TODO add missing drop functions 
		case FUN: drop_fun(r->inst);
			  break;
		case CMD: drop_cmd(r->inst);
			  break;
	*/
	}
	free(v->name);
	free(v);
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
Ref new_ref(char* name, void* inst, ref_t type);
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
		Var* tmp = realloc(l->list,
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
 * Find and get a stored variable
 * @param name The name of the variable to get
 * @return The variable or NULL if any variable having the
 *	   given name has been found
 */
Var get_var(const char* name){
	unsigned int h = hash(name);
	if (storage_buff[h] == NULL) return NULL;
	
	int i = search_var(name, storage_buff[h]);
	if (i == -1) return NULL;

	return storage_buff[h]->var_list[i];
}

/**
 * Create and store or update a variable 
 * @param name Name of the variable to store/update
 * @param val Pointer to the value of the variable
 * @param type Tge type of the data stored
 * @return The address of the newly stored/updated variable,
 *	   or NULL in case of error
 */
Var set_var(char* name, void* val, t_var type){

	unsigned int h = hash(name);
	if (storage_buff[h] == NULL){
		storage_buff[h] = new_v_list();
		if (storage_buff[h] == NULL) return NULL;
	}	
		
	Var v = new_var(name, val, type);
	if (v == NULL) return NULL;

	int i;	
	if ( (i = search_var(name, storage_buff[h])) == -1){

		if (push_var(storage_buff[h], v) == NULL) 
			goto e_v_lost;

	} else {
		
		if (replace_var_at(storage_buff[h], i, v) == NULL) 
			goto e_v_lost;
	}
	
	return v;

e_v_lost:
	drop_var(v);
	return NULL;
}
	
/* FNV-1a hash */
unsigned int hash(const char* key){
	unsigned int hash = 2166136261;
	while (*key != '\0') {
		hash ^= *key++;
		hash *= 16777619;
		hash %= N_VLIST;
	}
	return hash;
}
