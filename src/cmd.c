#include <stdlib.h>
#include "ref.h"
#include "var.h"
#include "cmd.h"
#include "error.h"

/**
 * Creates a new command structure
 * @param fun Pointer to the handler
 * @param args List of predefined arguments
 * @return The newly crafted command structure or NULL in case of error
 */
Cmd new_cmd(Ref (*fun) (ref_list), int n_args, ref_t** types){
	Cmd c = malloc(sizeof (s_cmd));
	if (c == NULL) return NULL;

	c->fun = fun;
	c->n_args = n_args;
	c->types = types;
	

	return c;
}
