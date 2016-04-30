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
	
	load = set_fun("mult",mult_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function mult");

	load = set_fun("addition",addition_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function addition");

	load = set_fun("sub",sub_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function sub");

	load = set_fun("mult_scal",mult_scal_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function mult_scal");

	load = set_fun("expo",expo_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function expo");

	load = set_fun("determinant",determinant_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function determinant");

	load = set_fun("invert",invert_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function invert");

	load = set_fun("solve",solve_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function solve");
}





/* Check if reference is a matrix */
bool expect_Matrix(Ref r){
	if (v_isa(r, MATRIX) == true) return true;
	
	set_err(ETYPE, "argument was expected to be a Matrix");
	return false;
}





/*  Creates a Matrix from a list of vectors of the same size */
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

/* Transpose one matrix */
Ref transpose_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "to many arguments");
		return NULL;	
	}
	
	if (expect_Matrix(args->list[0]) == false) return NULL;
	
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;
	Matrix t = transpose(m);

	if (t == NULL) return NULL;

	Ref r = new_vref(NULL, t, MATRIX);
	if (r == NULL) free(t); //TODO FIX

	return r;
		
} 
			



/* Multiplies all the given matrices consecutively */	
Ref mult_call(ref_list args){
	
	if (expect_Matrix(args->list[0]) == false) return NULL;
		
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;

	Matrix mult = copyMatrix(m);
	if (mult == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (expect_Matrix(args->list[i]) == false) goto error;

		var = (Var) args->list[i]->inst;
		m = (Matrix) var->val;
		if (m->nrows != mult->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = multiplication(mult, m);
		if (tmp == NULL ) goto error;

		free(mult); //FIXME
		mult = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, mult, MATRIX);
	if (r == NULL) free(mult); //TODO FIX

	return r;
error:
	free(mult); //TODO
	return NULL;
}



/* Addition of any number of matrices */
Ref addition_call(ref_list args){

	if (expect_Matrix(args->list[0]) == false) return NULL;
		
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;

	Matrix sum = copyMatrix(m);
	if (sum == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (expect_Matrix(args->list[i]) == false) goto error;

		var = (Var) args->list[i]->inst;
		m = (Matrix) var->val;
		if (m->nrows != sum->nrows || m->ncols != sum->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = addition(sum, m);
		if (tmp == NULL ) goto error;

		free(sum); //FIXME
		sum = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, sum, MATRIX);
	if (r == NULL) free(sum); //TODO FIX

	return r;
error:
	free(sum); //TODO
	return NULL;
}




/* Subtraction of any number of matrices */
Ref sub_call(ref_list args){
	
	if (expect_Matrix(args->list[0]) == false) return NULL;
		
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;

	Matrix sub = copyMatrix(m);
	if (sub == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (expect_Matrix(args->list[i]) == false) goto error;

		var = (Var) args->list[i]->inst;
		m = (Matrix) var->val;
		if (m->nrows != sub->nrows || m->ncols != sub->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = soustraction(sub, m);
		if (tmp == NULL ) goto error;

		free(sub); //FIXME
		sub = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, sub, MATRIX);
	if (r == NULL) free(sub); //TODO FIX

	return r;
error:
	free(sub); //TODO
	return NULL;
}


/* Multiplicate any number of matrices and/or floats */
Ref mult_scal_call(ref_list args){
	
	bool ret_matrix = false;

	float* ret_flt = malloc(sizeof (float));
	if (ret_flt == NULL) return NULL;
	*ret_flt = 1;

	Matrix ret_mat= NULL;

	unsigned int i;
	for (i = 0; i < args->length; i++){
	
		Var var = (Var) args->list[i]->inst;
		
		Matrix tmp;
		float* arg_f = (float*) var->val;
		Matrix arg_m = (Matrix) var->val;

		switch (var->type) {

			case FLOAT:
				if (ret_matrix){
					// Mat x Float
					tmp = multScal(*arg_f, ret_mat);
					if (tmp == NULL) goto error;

					free(ret_mat); //FIXME
					ret_mat = tmp;	

				} else {
					// Float x Float
					*ret_flt *= *arg_f;
				}
				break;

			case MATRIX:
				if (ret_matrix){
					// Mat x Mat
					if (arg_m->nrows != ret_mat->ncols) {
						set_err(EMXDIM,
						"dimensions don't match");
						goto error;
					}

					tmp = multiplication(ret_mat, arg_m);
					if (tmp == NULL ) goto error;

					free(ret_mat); //FIXME
					ret_mat = tmp;	

				} else {
					// Float x Mat
					tmp = multScal(*ret_flt, arg_m);
					if (tmp == NULL) goto error;

					ret_mat = tmp;
					ret_matrix = true;
					free(ret_flt);
					
				}	
				break;
			default:
				set_err(ETYPE, "argument was expected to"
						" be a Matrix of a Float");
				goto error;
		}
	}

	/* Return a reference */

	Ref r = NULL;

	if (ret_matrix) {
		r = new_vref(NULL, ret_mat, MATRIX);
		if (r == NULL) free(ret_mat); //TODO FIX
	} else {
		r = new_vref(NULL, ret_flt, FLOAT);
		if (r == NULL) free(ret_flt);
	}

	return r;

error:
	if (ret_matrix)	free(ret_mat); //TODO
	else free(ret_flt); 
	return NULL;
}





/*  Power of a matrix */
Ref expo_call(ref_list args){
	
	if (args->length != 2){
		set_err(ETYPE, "\"expo\" needs 2 arguments");
		return NULL;	
	}
	
	Var arg1 = (Var) args->list[0]->inst;
	Var arg2 = (Var) args->list[1]->inst;

	Matrix m; float p;

	if (v_isa(args->list[0], FLOAT) && 
	    v_isa(args->list[1], MATRIX)) {

		p = (*(float*) arg1->val);	
		m = (Matrix) arg2->val;

	} else if (v_isa(args->list[0], MATRIX) && 
	     v_isa(args->list[1], FLOAT)) {

		p = (*(float*) arg2->val);	
		m = (Matrix) arg1->val;

	} else {

		set_err(ETYPE,"couple Matrix-float expected");
		return NULL;
	}
	
	Matrix e = expo(m, p);
	if (e == NULL) return NULL;

	Ref r = new_vref(NULL, e, MATRIX);
	if (r == NULL) free(e); //TODO FIX

	return r;
		
} 



/* Determinant of one matrix */
Ref determinant_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "to many arguments");
		return NULL;	
	}
	
	if (expect_Matrix(args->list[0]) == false) return NULL;
	
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;

	if (m->nrows != m->ncols) {
		set_err(ETYPE, "not a square Matrix");
		return NULL;
	}
	
	float* d = malloc(sizeof (float));	
	if (d == NULL) return NULL;
	*d = determinant(m);


	Ref r = new_vref(NULL, d, FLOAT);
	if (r == NULL) free(d);

	return r;
		
} 






/* Invert one matrix */
Ref invert_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "to many arguments");
		return NULL;	
	}
	
	if (expect_Matrix(args->list[0]) == false) return NULL;
	
	Var var = (Var) args->list[0]->inst;
	Matrix m = (Matrix) var->val;

	if (m->nrows != m->ncols) {
		set_err(ETYPE, "not a square Matrix");
		return NULL;
	}

	Matrix inv = invert(m);
	if (inv == NULL) return NULL;


	Ref r = new_vref(NULL, inv, MATRIX);
	if (r == NULL) free(inv); //TODO

	return r;
		
} 



/*  Solve a system */
Ref solve_call(ref_list args){
	
	if (args->length != 2){
		set_err(ETYPE, "\"solve\" needs 2 arguments");
		return NULL;	
	}

	if (expect_Matrix(args->list[0]) == false ||
	    expect_Matrix(args->list[1]) == false ) return NULL;

	Var arg1 = (Var) args->list[0]->inst;
	Var arg2 = (Var) args->list[1]->inst;

	Matrix m1 = (Matrix) arg1->val;
	Matrix m2 = (Matrix) arg2->val;
	
	if (m1->nrows != m2->nrows ||
	    m2->ncols > 1) {
		set_err(EMXDIM, "not a valid system");
		return NULL;
	}
	
	Matrix s = solve(m1, m2);
	if (s == NULL) return NULL;

	Ref r = new_vref(NULL, s, MATRIX);
	if (r == NULL) free(s); //TODO FIX

	return r;
} 
