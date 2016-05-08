#include "ref_all.h"
#include "internals.h"
#include "error.h"
#include <stdlib.h>


void load_commands(void){
	Ref load = NULL;
	
	load = set_fun("compose",compose_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function compose");

	load = set_cref("quit",quit_cmd,0,NULL);
	if (load == NULL) inst_err(ELOAD,"command quit");

	load = set_cref("print",print__cmd,0,NULL);
	if (load == NULL) inst_err(ELOAD,"command print");
	

}

Ref compose_call(ref_list args){	
	Ref r = NO_REF;
	unsigned int i;
	for (i = 0; i < args->length && r != NULL; i++){

		if (cmptype_ref(FUN, args->list[i]) == false) continue;
		Fun f = (Fun) args->list[i]->inst;

		r = f->fun(f->args);
	}

	return r;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
Ref quit_cmd(ref_list args){ // Respect general prototype
	exit(0);
}
#pragma GCC diagnostic pop

Ref print__cmd(ref_list args){
	unsigned int i;
	for (i = 0; i < args->length; i++){
		print_ref(args->list[i]);
	}
	return NO_REF;
}
