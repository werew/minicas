#include <stdlib.h>
#include <stdio.h>
#include "ref_all.h"
#include "error.h"


/**
 * Creates a new command structure
 * @param fun Pointer to the handler
 * @param n_args Minimum number of arguments of the given program
 * @param types Array representing the types of the arguments
 * @return The newly crafted command structure or NULL in case of error
 */
Cmd new_cmd(Ref (*fun) (ref_list), unsigned int n_args, ref_t* types){
	Cmd c = malloc(sizeof (s_cmd));
	if (c == NULL) return NULL; 
	c->fun = fun;
	c->n_args = n_args;
	c->types = types;
	

	return c;
}

/**
 * Drop a stored Cmd structure
 * @param c Cmd to drop
 */
void drop_cmd(Cmd c){
	free(c);
}

/**
 * Creates a command and a reference pointing to it
 * @param name Name of the reference
 * @param fun Pointer to the handler
 * @param n_args Minimum number of arguments of the given program
 * @param types Array representing the types of the arguments
 * @return A pointer to the new reference or NULL in case 
 *	   of error
 */
Ref new_cref(char* name,Ref (*fun) (ref_list), unsigned int n_args, ref_t* types){
	
	Cmd c = new_cmd(fun, n_args, types);
	if (c == NULL) return NULL;

	Ref r = new_ref(name, c, CMD);
	if (r == NULL){
		drop_cmd(c);
		return NULL;
	}
	
	return r;
}	


/**
 * Create and store or update a command
 * @param name Name of the command to store/update
 * @param fun Pointer to the handler
 * @param n_args Minimum number of arguments of the given program
 * @param types Array representing the types of the arguments
 * @return The reference of the newly stored/updated command,
 *	   or NULL in case of error
 */
Ref set_cref
(char* name,Ref (*fun) (ref_list), unsigned int n_args, ref_t* types){

	Cmd c = new_cmd(fun, n_args, types);
	if (c == NULL) return NULL;

	Ref r = set_ref(name, c, CMD);
	if (r == NULL) {
		drop_cmd(c);
		return NULL;
	}
	
	return r;

}

/**
 * Find and get a stored command
 * @param name The name of the command to get
 * @return The reference or NULL if any command having the
 *	   given name has been found
 */
Ref get_cmd(const char* name){
	Ref r = get_reft(name, CMD);
	if (r == NULL) return NULL;

	return r;
}


/**
 * Print a command
 */
void print_cmd(Cmd c){
	// TODO
	c++; // Avoid warning
}
