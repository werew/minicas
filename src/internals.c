#include "ref_all.h"
#include "internals.h"
#include "error.h"
#include <stdlib.h>


void load_commands(void){
	Ref load = NULL;
	
	load = set_fun("compose",compose_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function compose");

	load = set_fun("eval_in",eval_in_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function eval_in");

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


Ref eval_in_call(ref_list args){	
	Ref dst = NO_REF, src = NO_REF;
	unsigned int i;
	for (i = 0; i+1 < args->length; i += 2){

		dst = args->list[i];    // Destination
		src = args->list[i+1];	// Source

		if (cmptype_ref(FUN, src) == true){
		
			Fun f = (Fun) src->inst;
			src = f->fun(f->args);
			if (src == NULL) return NULL;
			if (src == NO_REF) continue;

			dst = update_ref(dst, src->inst, src->type);
			drop_ref(src);
			continue;
		}

		dst = update_ref(dst, src->inst, src->type);
	}	
		

	return dst;
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
