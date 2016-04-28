#include <stdlib.h>
#include "ref.h"
#include "fun.h"
#include "error.h"

/**
 * Creates a new function structure
 * @param fun Pointer to the handler
 * @param args List of predefined arguments
 * @return The newly crafted function structure or NULL in case of error
 */
Fun new_fun(Ref (*fun) (ref_list), ref_list args){
	Fun f = malloc(sizeof (s_fun));
	if (f == NULL) return NULL;

	f->fun = fun;
	f->args = args;

	return f;
}

/**
 * Drop a stored Fun structure
 * @param f Fun to drop
 */
void drop_fun(Fun f){
	
	if (f == NULL) return;

	free(f->args);
	free(f);
}

/**
 * Creates a function and a reference pointing to it
 * @param name Name of the reference
 * @param fun Pointer to the handler
 * @param args List of predefined arguments
 * @return A pointer to the new reference or NULL in case 
 *	   of error
 */
Ref new_fref(char* name,Ref (*fun) (ref_list), ref_list args){
	
	Fun f = new_fun(fun, args);
	if (f == NULL) return NULL;

	Ref r = new_ref(name, f, FUN);
	if (r == NULL){
		drop_fun(f);
		return NULL;
	}
	
	return r;
}	
