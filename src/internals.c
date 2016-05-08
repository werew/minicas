#include "ref_all.h"
#include "internals.h"
#include "error.h"
#include "parser.h"
#include <stdlib.h>


void load_commands(void){
	Ref load = NULL;
	
	load = set_fun("summ",summ_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function summ");

	load = set_fun("compose",compose_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function compose");

	load = set_fun("eval_in",eval_in_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function eval_in");

	load = set_fun("eval_ifeq",eval_ifeq_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function eval_ifeq");

	load = set_fun("eval_ifneq",eval_ifneq_call,NULL);
	if (load == NULL) inst_err(ELOAD,"function eval_ifneq");

	load = set_fun("show",print_fc,NULL);
	if (load == NULL) inst_err(ELOAD,"function show");

	load = set_cref("quit",quit_cmd,0,NULL);
	if (load == NULL) inst_err(ELOAD,"command quit");

	load = set_cref("print",print_fc,0,NULL);
	if (load == NULL) inst_err(ELOAD,"command print");
	

}



/* Sum of any number of floats */
Ref summ_call(ref_list args){	

	float* sum = malloc(sizeof(float));
	*sum = 0;

	unsigned int i;
	for (i = 0; i < args->length; i++){
		if (cmptype_ref(FLOAT, args->list[i]) == false){
			set_err(ETYPE, "a float was expected");
			free(sum);
			return NULL;
		}
		
		*sum += *CAST_REF2FLOATP(args->list[i]);
	}
	
	return new_vref(NULL, sum, FLOAT);
}
	

/* Executes arguments that are function references 
 * and feed them as argument the non-function references
 * next to them 
 */
Ref compose_call(ref_list args){	
	Ref r = NO_REF;
	unsigned int i,j;
	for (i = 0; i < args->length && r != NULL; i++){
		
		if (cmptype_ref(FUN, args->list[i]) == false) continue;
		Fun f = (Fun) args->list[i]->inst;

		ref_list fun_args = new_ref_list();
		if (fun_args == NULL) return NULL;

		for (j = i+1; j < args->length; j++){
			if (cmptype_ref(FUN, args->list[j]) == true) break;
			
			if (push_ref(fun_args, args->list[j]) == NULL) {
				free(fun_args->list); free(fun_args);
				return NULL;
			}
	
		}

		r = exec_fun(f, fun_args, false);
		free(fun_args->list); free(fun_args);
	}

	return r;
}


/* Functional assignement
 * @note: function reference are not assigned but 
 *	  directly evaluated
 */
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




/* Eval on condition */

Ref eval_ifeq_call(ref_list args){ return eval_if_call(args, EQUAL); };
Ref eval_ifneq_call(ref_list args){ return eval_if_call(args, NEQUAL); };

Ref eval_if_call(ref_list args, Condition c){	
	if (args->length < 3) {
		set_err(EMISSARG, "this function needs at least 3 arguments");
		return NULL;
	}
	
	Ref a = args->list[0];
	Ref b = args->list[1];
	if (cmp_ref(a,b) == true) {
		if (c == NEQUAL) return NO_REF;
	} else {
		if (c == EQUAL) return NO_REF;
	}
	
	Ref r = NO_REF;
	unsigned int i;
	for (i = 2; i < args->length && r != NULL ; i++){

		r = args->list[i];

		if (cmptype_ref(FUN, r )== false) continue;
		Fun f = (Fun) args->list[i]->inst;

		r = f->fun(f->args);
	}

	return r;
}

/* Quit the program */
#pragma GCC diagnostic ignored "-Wunused-parameter"
Ref quit_cmd(ref_list args){ // Respect general prototype
	drop_pool();
	exit(0);
}
#pragma GCC diagnostic pop


/* Print one or more references */
Ref print_fc(ref_list args){
	unsigned int i;
	for (i = 0; i < args->length; i++){
		print_ref(args->list[i]);
	}
	return NO_REF;
}
