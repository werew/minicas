#include "init.h"
#include "ref_all.h"

/* Internals */
#include "internals.h"
/* Modules */
#include "mod_matrix.h"



void init_env(void){


	/* Load internal commands */
	load_commands();
	
	/* Load modules */
	load_mod_matrix();

}
