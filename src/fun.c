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
