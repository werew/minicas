#include <stdlib.h>
#include <stdio.h>
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
	f->_ptrs = 0;

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


/**
 * Create and store or update a Funciton
 * @param name Name of the function to store/update
 * @param fun Pointer to the handler
 * @param args List of predefined arguments
 * @return The reference of the newly stored/updated function,
 *	   or NULL in case of error
 */
Ref set_fun(char* name, Ref (*fun) (ref_list), ref_list args){

	Fun f = new_fun(fun, args);
	if (f == NULL) return NULL;

	Ref r = set_ref(name, f, FUN);
	if (r == NULL) {
		drop_fun(f);
		return NULL;
	}
	
	return r;

}

/**
 * Find and get a stored function
 * @param name The name of the function to get
 * @return The reference or NULL if any function having the
 *	   given name has been found
 */
Ref get_fun(const char* name){
	Ref r = get_reft(name, FUN);
	if (r == NULL) return NULL;

	return r;
}

/**
 * Print a function and his predefined arguments 
 */
void print_fun(Fun f){
	printf("function\n");
	if (f->args == NULL) return;

	unsigned int i;
	for (i = 0; i < f->args->length; i++){
		printf("\t\t-- Arg %d --\n\t",i);

		Ref arg = f->args->list[i];
		if (arg != NO_REF && cmptype_ref(FUN, arg) == true ){
			printf("\t@%s -> function\n", arg->name);
		} else {
			print_ref(arg);
		}
		printf("\n");
	}
}
