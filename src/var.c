#include <string.h>
#include <stdlib.h>
#include "xtypes.h"
#include "ref.h"
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
 * @return The reference or NULL if any variable having the
 *	   given name has been found
 */
Ref get_var(const char* name){
	Ref r = get_ref(name, VAR);
	if (r == NULL) return NULL;

	return r->inst;
}

/**
 * Create and store or update a variable 
 * @param name Name of the variable to store/update
 * @param val Pointer to the value of the variable
 * @param type Tge type of the data stored
 * @return The reference of the newly stored/updated variable,
 *	   or NULL in case of error
 */
Ref set_var(char* name, void* val, var_t type){

	Var v = new_var(val, type);
	if (v == NULL) return NULL;

	Ref r = set_ref(name, v, VAR);
	if (r == NULL) {
		drop_var(v);
		return NULL;
	}
	
	return r;

}
