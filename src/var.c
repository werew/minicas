#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "xtypes.h"
#include "ref_all.h"
#include "matrix.h"




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
	v->_ptrs = 0;
	
	return v;
}

/**
 * Creates a variable and a reference pointing to it
 * @param name Name of the reference
 * @param val Pointer to the content of the variable
 * @param type Type of the variable
 * @return A pointer to the new reference or NULL in case 
 *	   of error
 */
Ref new_vref(char* name, void* val, var_t type){
	Var v = new_var(val, type);
	if (v == NULL) return NULL;

	Ref r = new_ref(name, v, VAR);
	if (r == NULL){
		drop_var(v);
		return NULL;
	}

	return r;
}	
		
/**
 * Find and get a stored variable
 * @param name The name of the variable to get
 * @return The reference or NULL if any variable having the
 *	   given name has been found
 */
Ref get_var(const char* name){
	Ref r = get_reft(name, VAR);
	if (r == NULL) return NULL;

	return r;
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





/**
 * Print a Var
 */
void print_var(Var v){
	printf("Var ");
	switch (v->type){

		case MATRIX: 
			printf("Matrix :\n");
			displayMatrix(v->val);
			break;

		case FLOAT:
			printf("float : %f\n",(*(float*) v->val));
			break;
	}
}

/**
 * Test the type of a variable
 * @param r The reference to check
 * @param type The expected type
 * @return true if the given reference is a Var of the
 * 	   expected type
 */
bool v_isa(Ref r, var_t type){

	if (r == NULL || r->type != VAR) return false;

	Var var = (Var) r->inst;

	if ( var->type != type ) return false;

	return true;
}
