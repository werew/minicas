#include <stdlib.h>
#include "matrix.h"
#include "operation.h"
#include "mod_matrix.h"
#include "ref_all.h"
#include "error.h"


/** Module loader  **/
void load_mod_matrix(void){	
	Ref load = NULL;
	/* Load env variables */

	/* Load module functions */
	load = set_fun("matrix",matrix_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function matrix");
	
	load = set_fun("transpose",transpose_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function transpose");
	
}

/**
 * Creates a Matrix from a list of vectors
 * of the same size
 */
Ref matrix_call(ref_list args){
	Var var = (Var) args->list[0]->inst;
	Matrix vect = (Matrix) var->val;

	Matrix m = newMatrix(args->length, vect->ncols);
	if (m == NULL) return NULL;

	int i,j;
	for (i = 0; i < m->nrows; i++){

		var = (Var) args->list[i]->inst;
		if ( var->type != MATRIX ) {
			set_err(ETYPE, "the arguments of \"matrix\""
				       " must be of type Matrix" );
			free(m); //FIX
			return NULL;
		}
	
		vect = (Matrix) var->val;

		for (j = 0; j < m->ncols; j++){
			float el = getElt(vect,0,j);
			setElt(m,i,j,el);
		}
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, m, MATRIX);
	if (r == NULL) free(m); //TODO FIX

	return r;
}

Ref transpose_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "to many arguments");
		return NULL;	
	}

	if ( args->list[0]->type != VAR) {
		set_err(ETYPE, "only Var accepted in \"transpose\"");
		return NULL;
	}

	Var vm = (Var) args->list[0]->inst;
	if (vm->type != MATRIX) {
		set_err(ETYPE, "only Matrix accepted in \"transpose\"");
		return NULL;
	} 
	
	Matrix m = (Matrix) vm->val;
	Matrix t = transpose(m);
	if (t == NULL) return NULL;

	Ref r = new_vref(NULL, t, MATRIX);
	if (r == NULL) free(t); //TODO FIX

	return r;
		
} 
			
	
