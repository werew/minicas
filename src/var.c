#include <string.h>
#include <stdlib.h>
#include "xtypes.h"
#include "var.h"


/**
 * Drop a stored variable structure
 * @param v Variable to drop
 */
void drop_var(Var v){
	if (v == NULL) return;

	free(v->val);
	free(v);
}


/**
 * Creates a new Var containing the given informations
 * @param val Address to the value of the variable
 * @param type Type of the variable (FLOAT or MATRIX)
 * @return A pointer to the newly created variable 
 *	   or NULL in case of error
 */
Var new_var(void* val, var_t type){
	Var v = malloc( sizeof (s_var));
	if (v == NULL) return NULL;

	v->type = type;
	v->val = val;
	
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
