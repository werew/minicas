#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
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

	load = set_fun("rank",rank_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function rank");

	load = set_fun("triangular",triangular_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function triangular");

	load = set_fun("decomposition",decomposition_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function decomposition");

	load = set_fun("kernel",kernel_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function kernel");

	load = set_fun("nullspace",kernel_call,NULL);
	if (load == NULL) inst_err(ELOAD, "function nullspace");

	/* Load commands */
	static ref_t args_cmd[4] = { FUN, FLOAT, FLOAT, FLOAT };
	load = set_cref("speedtest",speedtest_cmd,4,args_cmd);
	if (load == NULL) inst_err(ELOAD,"command speedtest");
	
}





/* Check if reference is a matrix */
bool arg_isMatrix(Ref r){
	if (cmptype_ref(MATRIX, r) == true) return true;
	set_err(ETYPE, "argument was expected to be a Matrix");
	return false;
}





/*  Creates a Matrix from a list of vectors of the same size */
Ref matrix_call(ref_list args){
	if (args->length < 1 ) {
		set_err(EMISSARG, "need at least one argument to craft a matrix");
		return NULL;
	}

	Var var = (Var) args->list[0]->inst;
	Matrix vect = (Matrix) var->val;

	Matrix m = newMatrix(args->length, vect->ncols);
	if (m == NULL) return NULL;

	unsigned int i,j;
	for (i = 0; i < m->nrows; i++){

		var = (Var) args->list[i]->inst;
		if ( var->type != MATRIX ) {
			set_err(ETYPE, "the arguments of \"matrix\""
				       " must be of type Matrix" );
			dropMatrix(m);
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
	if (r == NULL) dropMatrix(m);

	return r;
}

/* Transpose one matrix */
Ref transpose_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "need one and only one argument");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;

	Matrix m = CAST_REF2MATRIX(args->list[0]);	

	Matrix t = transpose(m);
	if (t == NULL) return NULL;

	Ref r = new_vref(NULL, t, MATRIX);
	if (r == NULL) dropMatrix(t);

	return r;
		
} 
			


/* Triangularize one matrix */
Ref triangular_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "need one and only one argument");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;

	Matrix m = CAST_REF2MATRIX(args->list[0]);	

	Matrix t = triangulaire(m,NULL,NULL,NULL,PivotPartiel);
	if (t == NULL) return NULL;

	Ref r = new_vref(NULL, t, MATRIX);
	if (r == NULL) dropMatrix(t);

	return r;
		
} 

/* Multiplies all the given matrices consecutively */	
Ref mult_call(ref_list args){
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
		
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	Matrix mult = copyMatrix(m);
	if (mult == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (arg_isMatrix(args->list[i]) == false) goto error;
		m = CAST_REF2MATRIX(args->list[i]);

		if (m->nrows != mult->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = multiplication(mult, m);
		if (tmp == NULL ) goto error;

		dropMatrix(mult);
		mult = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, mult, MATRIX);
	if (r == NULL) dropMatrix(mult);

	return r;
error:
	dropMatrix(mult);
	return NULL;
}



/* Addition of any number of matrices */
Ref addition_call(ref_list args){

	if (args->length < 1 ) {
		set_err(EMISSARG, "need at least one argument");
		return NULL;
	}
	if (arg_isMatrix(args->list[0]) == false) return NULL;
		
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	Matrix sum = copyMatrix(m);
	if (sum == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (arg_isMatrix(args->list[i]) == false) goto error;
		m = CAST_REF2MATRIX(args->list[0]);

		if (m->nrows != sum->nrows || m->ncols != sum->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = addition(sum, m);
		if (tmp == NULL ) goto error;

		dropMatrix(sum);
		sum = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, sum, MATRIX);
	if (r == NULL) dropMatrix(sum);

	return r;
error:
	dropMatrix(sum);
	return NULL;
}




/* Subtraction of any number of matrices */
Ref sub_call(ref_list args){
	
	if (args->length < 1 ) {
		set_err(EMISSARG, "need at least one argument");
		return NULL;
	}

	if (arg_isMatrix(args->list[0]) == false) return NULL;
		
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	Matrix sub = copyMatrix(m);
	if (sub == NULL) return NULL;

	unsigned int i;
	for (i = 1; i < args->length; i++){
		
		
		if (arg_isMatrix(args->list[i]) == false) goto error;
		m = CAST_REF2MATRIX(args->list[i]);

		if (m->nrows != sub->nrows || m->ncols != sub->ncols) {
			set_err(EMXDIM,"dimensions don't match");
			goto error;
		}

		Matrix tmp = soustraction(sub, m);
		if (tmp == NULL ) goto error;

		dropMatrix(sub);
		sub = tmp;
	}
    
	/* Return a reference */
	Ref r = new_vref(NULL, sub, MATRIX);
	if (r == NULL) dropMatrix(sub);

	return r;
error:
	dropMatrix(sub);
	return NULL;
}


/* Multiplicate any number of matrices and/or floats */
Ref mult_scal_call(ref_list args){
	
	if (args->length < 1 ) {
		set_err(EMISSARG, "need at least one argument");
		return NULL;
	}

	bool ret_matrix = false;

	float* ret_flt = malloc(sizeof (float));
	if (ret_flt == NULL) return NULL;

	*ret_flt = 1;

	Matrix ret_mat= NULL;

	unsigned int i;
	for (i = 0; i < args->length; i++){
		
		if (args->list[i]->type != VAR){
			set_err(ETYPE, "argument was expected to"
					" be a Matrix of a Float");
			goto error;
		}	

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

					dropMatrix(ret_mat);
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

					dropMatrix(ret_mat);
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
		if (r == NULL) dropMatrix(ret_mat);
	} else {
		r = new_vref(NULL, ret_flt, FLOAT);
		if (r == NULL) free(ret_flt);
	}

	return r;

error:
	if (ret_matrix)	dropMatrix(ret_mat);
	else free(ret_flt); 
	return NULL;
}





/*  Power of a matrix */
Ref expo_call(ref_list args){
	
	if (args->length != 2){
		set_err(ETYPE, "\"expo\" needs 2 arguments");
		return NULL;	
	}
	
	Matrix m; float p;

	if (cmptype_ref(FLOAT,  args->list[0]) && 
	    cmptype_ref(MATRIX, args->list[1]) ){

		p = *CAST_REF2FLOATP(args->list[0]);
		m =  CAST_REF2MATRIX(args->list[1]);

	} else if (cmptype_ref(MATRIX, args->list[0]) && 
		   cmptype_ref(FLOAT,  args->list[1]) ){

		m =  CAST_REF2MATRIX(args->list[0]);
		p = *CAST_REF2FLOATP(args->list[1]);

	} else {

		set_err(ETYPE,"couple Matrix-float expected");
		return NULL;
	}
	
	Matrix e = expo(m, p);
	if (e == NULL) return NULL;

	Ref r = new_vref(NULL, e, MATRIX);
	if (r == NULL) dropMatrix(e);

	return r;
		
} 



/* Determinant of one matrix */
Ref determinant_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "to many arguments");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
	
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	if (m->nrows != m->ncols) {
		set_err(ETYPE, "not a square Matrix");
		return NULL;
	}
	
	float* d = malloc(sizeof (float));	
	if (d == NULL) return NULL;

	if ( determinant(m, d) == false){
		free(d);
		return NULL;
	}

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
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
	
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	if (m->nrows != m->ncols) {
		set_err(ETYPE, "not a square Matrix");
		return NULL;
	}

	Matrix inv = NULL;
	if ( invert(m, &inv) == false) return NULL;


	Ref r = new_vref(NULL, inv, MATRIX);
	if (r == NULL) dropMatrix(inv);

	return r;
		
} 



/*  Solve a system */
Ref solve_call(ref_list args){
	
	if (args->length != 2){
		set_err(ETYPE, "\"solve\" needs 2 arguments");
		return NULL;	
	}

	if (arg_isMatrix(args->list[0]) == false ||
	    arg_isMatrix(args->list[1]) == false ) return NULL;

	Matrix m1 = CAST_REF2MATRIX(args->list[0]);
	Matrix m2 = CAST_REF2MATRIX(args->list[1]);
	
	if (m1->ncols != m2->nrows ||
	    m2->ncols > 1) {
		set_err(EMXDIM, "not a valid system");
		return NULL;
	}
	
	Matrix s = NULL;
	if (solve(m1, m2, &s) == false) return NULL;

	Ref r = new_vref(NULL, s, MATRIX);
	if (r == NULL) dropMatrix(s);

	return r;
} 


/* Determinant of one matrix */
Ref rank_call(ref_list args){
	
	if (args->length != 1){
		set_err(ETYPE, "too many arguments");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
	
	Matrix m = CAST_REF2MATRIX(args->list[0]);

	float* d = malloc(sizeof (float));	
	if (d == NULL) return NULL;
	*d = rank(m);


	Ref r = new_vref(NULL, d, FLOAT);
	if (r == NULL) free(d);

	return r;
		
} 



Ref decomposition_call(ref_list args){
	if (args->length != 1){
		set_err(ETYPE, "too many arguments");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
	
	Matrix M = CAST_REF2MATRIX(args->list[0]);

	Matrix LUP[3] = {NULL, NULL, NULL};
	decomposition(M,&LUP[0],&LUP[1],&LUP[2]);
	
	int i;		
	for ( i = 0; i < 3; i++){	
		if (LUP[i] != NULL){
			displayMatrix(LUP[i]);
			dropMatrix(LUP[i]);
		}
	}
		
	return NO_REF;

}


Ref kernel_call(ref_list args){
	if (args->length != 1){
		set_err(ETYPE, "too many arguments");
		return NULL;	
	}
	
	if (arg_isMatrix(args->list[0]) == false) return NULL;
	
	Matrix M = CAST_REF2MATRIX(args->list[0]);

	if (M->nrows != M->ncols) {
		set_err(ETYPE, "not a square Matrix");
		return NULL;
	}

	Matrix K;
	if ( noyau(M, &K) == 0 ) return NULL;
	
	/* Print kernel */
	unsigned int i,j;
	printf("\n");
	for(i = 0 ; i < K->nrows; i++)
	{
		printf("\t");
		for (j = 0; j < K->ncols; j++){
			printf("\t[%- 6.4g]",getElt(K,i,j));
		}
		printf("\n");
	}
	printf("\n\n");

	return NO_REF;
		
			
}


/******************** COMMANDS ************************/
void plot(char* title, int* x_size, clock_t* y_time, int n){
	
	printf("Generating chart for command: \"%s\"\n",title);

	char set_title[100];	
	snprintf(set_title,100,"set title \"Speedtest %s\"",title);

	char * commands[] = {
		set_title,
    		"set terminal gif medium size 448,336", 
    		"set ylabel \"c [clocks]\"",
    		"set xlabel \"Matrix dimension\"",
    		"set output \"chart.gif\"",
    		"plot '-' with lines"};
	

	FILE * gnuplotPipe = popen ("gnuplot", "w");
	if (gnuplotPipe == NULL) {
    		perror("Cannot open gnuplot");
    		return;
 	}

	int i;
	for (i=0; i < 6; i++) {
		fprintf(gnuplotPipe, "%s \n", commands[i]);
	}

	for (i = 0; i < n ; i++) {
		fprintf(gnuplotPipe, "%d %ld\n", x_size[i], y_time[i]);
	}

	fprintf(gnuplotPipe, "e");
	fclose(gnuplotPipe);
}

bool keepgoing = true;
void stop_speedtest(int sig){

	(void) sig; //Unused

	puts("Terminating test...");
	keepgoing = false;
}

Ref speedtest_cmd(ref_list args){

	puts("Starting test...");

	bool doplot = true; keepgoing = true; 

	Fun f = (Fun) args->list[0]->inst;
	int min = (int) (*(float*) ((Var)args->list[1]->inst)->val);
	int max = (int) (*(float*) ((Var)args->list[2]->inst)->val);
	int step = (int) (*(float*) ((Var)args->list[3]->inst)->val);
		
	if ( min > max || step == 0) {
		set_err(ENOVAL, "infinite tests are not allowed");
		return NULL;
	}

	/* Optional parameter */
	if (args->length > 4 && cmptype_ref(FLOAT, args->list[4])) {
		int countdown = (int) (*(float*) ((Var)args->list[4]->inst)->val);
		signal(SIGALRM, stop_speedtest);
		alarm(countdown);
	} 
	
	
	/* Points list */	
	clock_t* y_time = malloc (sizeof(clock_t) * (max-min)/step + 1);
	int* x_size = malloc (sizeof(int) * (max-min)/step + 1);
	if (y_time == NULL || x_size == NULL){
		free(x_size); free(y_time);
		return NULL;
	}

	
	int i,j;
	for ( i = min, j=0; i <= max && keepgoing ; i += step, j++){
		
		/* Generate matrices. 
		   Note: they don't need to be initialized 
		*/
		Matrix m1 = newMatrix(i,i), m2 = newMatrix(i,i), ret = NULL;
		float* f1 = malloc(sizeof(float));
		if (m1 == NULL || m2 == NULL || f1 == NULL){ 
			perror("");
			keepgoing = false; doplot = false;
		}

		/* Start test */
		clock_t start = clock();

		if (f->fun == mult_call) {
			ret = multiplication(m1,m2);

		} else if ( f->fun == sub_call) {
			ret = soustraction(m1,m2);

		} else if (f->fun == addition_call){
			ret = addition(m1,m2);

		} else if (f->fun == transpose_call){
			ret = transpose(m1);	

		} else if (f->fun == determinant_call){
			determinant(m1, f1);	

		} else if (f->fun == invert_call){
			invert(m1,&ret);

		} else if (f->fun == rank_call ){
			rank(m1);
		} else {
			printf("Sorry...cannot test this function\n");
			keepgoing = false; doplot = false;
		}
		
		
		/* Finish test */
		clock_t end = clock();
	
		/* Store result */	
		x_size[j] = i; y_time[j] = end-start;
		
		dropMatrix(m1);
		dropMatrix(m2);
		dropMatrix(ret);
		free(f1);
	}

	alarm(0); // Remove countdown

	if (doplot == true) plot(args->list[0]->name, x_size, y_time, j);

	free(x_size); free(y_time);

	return NO_REF;
}

