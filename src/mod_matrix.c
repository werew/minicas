#include <stdlib.h>
#include "matrix.h"
#include "mod_matrix.h"
#include "ref_all.h"


/** Module loader  **/
void load_mod_matrix(void){	

	/* Load env variables */

	/* Load module functions */
	set_fun("matrix",matrix_call,NULL);
	
	
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
		vect = (Matrix) var->val;

		for (j = 0; i < m->ncols; j++){
			float el = getElt(vect,0,j);
			setElt(m,i,j,el);
		}
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, m, MATRIX);
	if (r == NULL) free(m);

	return r;
}
			
	