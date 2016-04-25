#include <string.h>
#include <stdlib.h>
#include "xtypes.h"
#include "storage.h"


/* Global storage buffer */
v_list storage_buff[N_VLIST];

/**
 * Drop a stored variable structure
 * @param v Variable to drop
 */
void drop_var(Var v){
	if (v == NULL) return;

	free(v->name);
	free(v->val);
	free(v);
}

/**
 * Destroy a v_list all the variables that it constains.
 * @param l v_list to drop
 * @param unnamed_only If true only the unnamed variable will be dropped
 */
void drop_v_list(v_list l, bool unnamed_only){

	unsigned int i;
	for ( i = 0; i < l->length; i++){
		if (unnamed_only && l->var_list[i]->name != NULL) continue;
		drop_var(l->var_list[i]);
	}
	
	free(l);
}


/**
 * Creates a new Var containing the given informations
 * @param name Name of the variable
 * @param val Address to the value of the variable
 * @param type Type of the variable (FLOAT or MATRIX)
 * @return A pointer to the newly created variable 
 *	   or NULL in case of error
 */
Var new_var(char* name, void* val, t_var type){
	Var v = malloc( sizeof (s_var));
	if (v == NULL) return NULL;

	v->name = name;
	v->type = type;
	v->val = val;
	
	return v;
}
		
/**
 * Creates a new empty (zero-initialized) v_list
 * @return The newly created v_list
 */
v_list new_v_list(void){
	v_list l = malloc( sizeof (s_v_list));
	if (l == NULL) return NULL;
	memset(l, 0, sizeof (s_v_list));
	return l;
}


/**
 * Search a variable inside the given v_list
 * @param name The name of the variable to search
 * @param l The list where to search the variable
 * @return The index representing the position of the variable in relation
 *	   to l->var_list or -1 if the variable has not been found
 */
int search_var(const char* name, const v_list l){
	unsigned int i;
	for (i = 0; i < l->length; i++){
		if ( strcmp( l->var_list[i]->name, name) == 0){
			return i;
		}
	}
	
	return -1;
}

/**
 * Insert the given variable at the end of the given v_list
 * @param v The variable to insert
 * @param l The list where to insert v
 * @return The newly pushed variable in case of success or
 *	   NULL in case of error
 */
Var push_var(v_list l, const Var v){
	#define ALLOC_UNIT 8
	if (l->var_list == NULL || l->length >= l->max){
		Var* tmp = realloc(l->var_list, 
			l->max + ALLOC_UNIT * sizeof (Var));
		if (tmp == NULL) return NULL;
	
		l->var_list = tmp;
		l->max += ALLOC_UNIT;
	}
	
	l->var_list[l->length++] = v;	
	return v;
}
	
	
/**
 * Replace the variable at the given position of the v_list with a new one
 * @param v The variable to insert
 * @param i The index representing a valid position of a variable to replace
 * @param l The list where to insert v
 * @return The newly inserted variable or NULL in case of error
 * @note At the moment the function doesn't imply any possible return of NULL
 *	 howover this return value needs to be considered for compatibility
 *	 with future versions of this function
 */
Var replace_var_at(v_list l, int i, const Var v){
	drop_var(l->var_list[i]);
	l->var_list[i] = v;
	return v;
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