#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "xtypes.h"
#include "ref_all.h"
#include "parser.h"
#include "error.h"

/* Current symbol pointer */
static char* sym;
static char* line;


/***************************** PARSE-HELPERS **********************************/


/**
 * Allow '_' char in the name of a cmd/fun/var
 */
int isalnum_extended(int c){
	return isalnum(c) || c == '_';
}

/**
 * Allow '-' char in the name of a cmd/fun/var
 */
int isdigit_extended(int c){
	return isdigit(c) || c == '-';
}

/**
 * Get test function for a given c_class 
 * @param class Characters class
 * @return A pointer to a function testing the belonging
 *	   to the given class
 */
int (*get_classtest(c_class class)) (int) {
	int (*test_class)(int) = NULL;
	switch (class) {
		case ALPHA: test_class = isalpha;
			break;
		case DIGIT: test_class = isdigit_extended;
			break;
		case ALPHANUM: test_class = isalnum;
			break;
		case ALPHANUMEX: test_class = isalnum_extended;
			break;
		case SPACE: test_class = isspace;
			break;
	}
	return test_class;
}

/**
 * Find first char belonging to the given c_class 
 * @param ptr Starting point
 * @param class Characters class to jump
 * @return A pointer to the searched character 
 */
char* jump_cclass(char* ptr, c_class class){
	int (*test_class)(int);
	test_class = get_classtest(class);
	while ( test_class((int) *ptr) ){ ptr++; };
	return ptr;
}



/**
 * Uses global sym to pop an alphanumeric word.
 * sym is moved to the end of the word
 * @return A new allocated string containing a copy
 *	   of the word, or NULL in case of error
 */
char* pop_word(void){
	char* w_end = jump_cclass(sym, ALPHANUMEX);
	size_t length = w_end - sym;

	char* word = malloc(length + 1);
	if (word == NULL) return NULL;
	
	memcpy(word, sym, length); 
	word[length] = '\0';

	sym = w_end;
	return word;
}


/***************************** INTERNAL CALLS **********************************/


/**
 * Declare a new reference having a value equal to the expression
 * next to the global pointer sym.
 * @param name Key-name of the reference to declare
 * @return The address of the stored reference in case of success,
 * 	   otherwise NULL (in this case the parameter name will not
 *	   be stored and must be freed by the user)
 */
Ref declare_ref(char* name){

	/* Evaluate expression */
	Ref e_r = eval_expression();
	if (e_r == NULL) return NULL;
	
	/* Store a renamed copy of e_r */
	Ref r = set_ref(name, e_r->inst, e_r->type);
	if (r == NULL){
		/* Free all the data */
		drop_ref(e_r);
		return NULL;
	}

	/* Free just the unnamed references */
	if (e_r->name == NULL) free(e_r);

	return r;
}			

/**
 * Try to execute a function named 'name' if the combination 
 * of the given arg list and the predefined arguments doesn't 
 * contain any empty argument (NULL Ref ) in this case return
 * a new function with a new set of functions
 * 
 * @param name Name of the function
 * @param args List of arguments
 * @return A reference to the result of the function being 
 * 	   evaluated or a reference to a new function 
 * 	   with a new set of predefined arguments
 */
Ref exec_fun(char* name, ref_list args){

	Ref f_ref = get_fun(name);
	if (f_ref == NULL) {
		set_err(ENOTAFUN, name);
		return NULL;
	}

	Fun f = (Fun) f_ref->inst;
	
	unsigned int i_args = 0; // Index user args
	unsigned int i_preargs = 0; // Index predefined args
	bool eval = true;
	Ref call_arg , pre_arg;

	// COmposition args (user+predefined)
	ref_list comp_args = new_ref_list();

	if (f->args != NULL){


		for (; i_preargs < f->args->length; i_preargs++){

			pre_arg = f->args->list[i_preargs];

			if (pre_arg == NO_REF && i_args < args->length){

				call_arg = args->list[i_args++];
				eval = (call_arg == NO_REF)? false : eval;
				push_ref(comp_args,call_arg);

			} else {
				eval = (pre_arg == NO_REF)? false : eval;
				push_ref(comp_args, pre_arg );

			}
		}
	}
		
	for (; i_args < args->length; i_args++){

		call_arg = args->list[i_args];
		eval = (call_arg == NO_REF)? false : eval;
		push_ref(comp_args,call_arg);
	}


	Ref ret;	
	if (eval == true ){
		/* Evaluate function */	
		ret = f->fun(comp_args);
		drop_ref_list(comp_args, true);

	} else {
		/* Create new function */
		ret = new_fref(NULL, f->fun, comp_args);
		if (ret == NULL) drop_ref_list(comp_args, true);
	}
	
	return ret;
}

bool cmptype_arg(unsigned int type, const Ref arg){

	switch (type) {
		case ALL: return true;
		case FUN: 
		case VAR:
		case CMD: return arg->type == type;
		case FLOAT: 
		case MATRIX: return arg->type == VAR && 
				    ((Var) arg->inst)->type == type;
		default: return false;
	}
	
}
	

Ref exec_cmd(char* cmd, ref_list args){	

	Ref r = get_cmd(cmd);

	if (r == NULL || r->type != CMD) {
		set_err(ENOTACMD, cmd);
		return NULL;
	}

	Cmd c = (Cmd) r->inst;
	
	if ( c->n_args > args->length ){
		set_err(EMISSARG, cmd );
		return NULL;
	}	

	unsigned int i;
	for (i = 0; i < c->n_args; i++){
		if (cmptype_arg(c->types[i], args->list[i]) == false){
			return NULL;
		}
	}


	return c->fun(args);

}	
			
		
		

/***************************** EVALUATORS   **********************************/

/**
 * Evaluates one single instruction. The beginning of the instruction
 * must be pointed by the global pointer 'sym'. 
 * On success, at the end of the evaluation the pointer 'sym' 
 * will point to the next end-of-instruction character
 * (';' or '\0' in absence of syntax errors).
 *
 * @return 0 on success , -1 in case of error
 * @note Instruction are evaluated from left to right, for instance
 *	 the instruction: "mymatrix : matrix([1,2,3]));" 
 *	 will be valuated correctly and 'sym' will finally point to
 *	 the last character ')'. It is up to the caller to check that
 *	 'sym' points actually to a valid end-of-instruction character
 *	 and raise an exception if necessary.
 */
int exec_instrution(void){
	char* an_token = NULL;

	/* Jump all the blank characters */	
	sym = jump_cclass(sym, SPACE);

	/* Expect a letter */	
	if (isalpha(*sym) == 0){
		if ( memchr(";\n\0", *sym, 3) != NULL) return 0;
		set_err(ESYNTAX, sym); goto error;
	}

	/* Pop first word of the input */
	if ((an_token = pop_word()) == NULL) goto error;

	sym = jump_cclass(sym, SPACE);

	Ref ret = NULL;
	switch (*sym) {
		case ':':
			/* Line is a declaration */
			sym++;
			ret = declare_ref(an_token); //Note: do not free an_token
			if (ret == NULL) goto error;
			break;
		case '(':
			/* Line is a function call */
			ret = eval_fun(an_token);
			if (ret == NULL) goto error;
			free(an_token);
			break;
		default:;
			/* Line is an internal command */
			ret = eval_cmd(an_token);
			if (ret == NULL) goto error;
			free(an_token);
	}
	
	/* Print instruction output */
	if (ret != NULL && ret != NO_REF){
		print_ref(ret);
		if (ret->name == NULL) drop_ref(ret);
	}

	/* Move to the end of the instruction */
	sym = jump_cclass(sym, SPACE);

	return 0;

error:
	free(an_token);
	return -1;
}




/* sym = args */	
Ref eval_cmd(char* cmd){

	
	ref_list args = new_ref_list();
	if (args == NULL) return NULL;

	sym = jump_cclass(sym, SPACE);

	while ( *sym != ';' && *sym != '\0'){

		Ref r = eval_expression();
		if (r == NULL || push_ref(args, r) == NULL) goto error;

		sym = jump_cclass(sym, SPACE);
	}


	
	Ref ret = exec_cmd(cmd, args);	
	drop_ref_list(args, true);	
	
	return ret;

error:
	drop_ref_list(args, true);	
	return NULL;
}			




/**
 * Parse and evaluates a function call. When eval_fun is
 * called, the global pointer 'sym' must point to the
 * left parenthesis immediatly (exept spaces) following the
 * function name. Examples of valid syntax for a function 
 * call are:

 * 	fun1 ( 29, 7, 12);
 *	fun1( a, b, c ) 	# a, b and c must be declared
 * 	fun1 ( fun2(b, 12), c, fun3(fun4(a)) );
 * @param fun Name of the called function
 * @return The reference result of the function being evaluated, 
 *	   of NULL in case of error
 */
Ref eval_fun(char* fun){

	ref_list args = new_ref_list();
	if (args == NULL) return NULL;

	do {
		sym++;

		Ref r = eval_expression();
		if (r == NULL || push_ref(args, r) == NULL) goto error;

	} while ( *sym == ',');

	/* Check for a valid end-of-function */
	if (*sym != ')'){
		set_err(ESYNTAX,sym);
		goto error;
	} 

	/* By convention move to the char 
	   right next the expression */	
	sym++;

	Ref ret = exec_fun(fun, args);	
	drop_ref_list(args, true);	
	
	return ret;

error:
	drop_ref_list(args, true);	
	return NULL;
}			










/**
 * Parse and evaluates a vactor. When eval_vect is
 * called, the global pointer 'sym' must point to beginning
 * of the vector: '[' . 
 * Examples of valid syntax for a vector:
 * 	[ 12, 22, 66];
 *	[ a, b, c ] 	# a, b and c must be declared
 * 	[ fun(b, 12), c, fun2(fun3(a)) ];
 *
 * @return The reference result of the vector being evaluated, 
 *	   of NULL in case of error
 */
Ref eval_vector(void){

	ref_list elts = new_ref_list();
	if (elts == NULL) return NULL;
	
	do {
		sym++;

		Ref r = eval_expression();
		if (r == NULL || push_ref(elts, r) == NULL) goto error;

		if (r->type != VAR || ((Var)r->inst)->type == MATRIX) {
			set_err(ETYPE, "only Float allowed inside a Matrix");
			goto error;
		}

		sym = jump_cclass(sym, SPACE);

	} while ( *sym == ',');

	/* Check for a valid end-of-vector */
	if (*sym != ']'){
		set_err(ESYNTAX,sym);
		goto error;
	} 

	/* By convention move to the char 
	   right next the expression */	
	sym++;

	/* Craft a Matrix from the ref_list */
	Matrix m = ref_list2vect(elts);		
	if ( m == NULL ) goto error;

	drop_ref_list(elts, true);	

	/* Return a reference */
	Ref r = new_vref(NULL, m, MATRIX);
	if (r == NULL) free(m);
	
	return r;

error:
	drop_ref_list(elts, true);	
	return NULL;
}








/**
 * Parse and evaluates a float. When eval_float is
 * called, the global pointer 'sym' must point to beginning
 * of the float number . 
 * @return The reference result of the Float being evaluated, 
 *	   of NULL in case of error
 */
Ref eval_float(){
	float* value = malloc(sizeof (float));
	if (value == NULL) return NULL;

	char* tmp;
	*value = strtof(sym, &tmp);
	sym = tmp;

	Ref r = new_vref(NULL, value, FLOAT);
	if (r == NULL) free(value);

	return r;
}








/**
 * Evaluates one of the following kind of expression:
 *	- function
 *	- vector
 *	- variable
 *	- numeric value
 *
 * An expression could be a variable, a function, a numerical value or a vector
 */
Ref eval_expression(void){
	

	/* Jump all the blank characters */	
	sym = jump_cclass(sym, SPACE);

	/* Evaluate next token */
	Ref r_result = NULL;	

	if (isalpha(*sym)) {  	   
		/* Variable or function */		
		char* word = pop_word();
		if (word == NULL) return NULL;

		sym = jump_cclass(sym, SPACE);

		if (*sym == '('){ 
			r_result = eval_fun(word);

		} else {
			r_result = get_ref(word);
			if (r_result == NULL) set_err(ENOTAVAR, word);
		}

		free(word);
		
	} else if (isdigit_extended(*sym)) { 
		/* Num value */
		r_result = eval_float();

	} else if (*sym == '[') {   
		/* Vector */
		r_result = eval_vector();

	} else if (*sym == '_' ){
		sym++;
		return NO_REF; 

	} else {
		set_err(ESYNTAX,sym);
		return NULL;
	}

	return r_result;
}



/**
 * Evaluate and execute user input.
 *
 * @param user_input A pointer to a null terminated string containing the
 *	  instruction to evaluate. Each instruction must be separated using
 *	  the character ';', on the other hand an instruction can simply
 *	  end with a null character (no need of ';' )
 * @return 0 in case of success, -1 otherwise
 *
 * @note Note that this function DO NOT perform line separation, indeed
 *	 '\n' is considered as a normal character and not a valid separator
 *	 for the instructions. If you wish to perform a line-by-line evaluation
 *	 the lines-split must be performed before calling this function.
 */
int eval_input(char* user_input){

	/* Set global pointer to the beginning of the current input*/
	line = user_input;
	sym = line;

	do {

		if (exec_instrution() == -1) {
			print_error();

			/* Move to the instruction end*/
			char* tmp = strchr( sym, ';');
			sym = (tmp != NULL)? tmp : strchr( sym, '\0');

		} else if (*sym != ';' && *sym != '\0'){
			/* The instruction doesn't end in a proper way */		
			set_err(ESYNTAX,sym);
			print_error();
		
			/* Move to the instruction end*/
			char* tmp = strchr( sym, ';');
			sym = (tmp != NULL)? tmp : strchr( sym, '\0');
		}

	} while ( *sym++ != '\0');

	
	return 0;
}















/**
 * Creates a Matrix from a ref_list of float
 * @param l A ref_list containing references to Float values
 * @prec The ref_list must contain only Float references
 */
Matrix ref_list2vect(ref_list l){
	Matrix m = newMatrix(1, l->length);
	if (m == NULL) return NULL;

	unsigned int i;
	for (i = 0; i < l->length; i++){

		Var v = (Var) l->list[i]->inst;
		float* val = v->val;

		setElt(m, 0, i, *val);
	}
	
	return m;
}
