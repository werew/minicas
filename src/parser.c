#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "parser.h"
#include "error.h"
#include "storage.h"
#include "xtypes.h"
#include "debug.h" //DBG

/* Current symbol pointer */
static char* sym;
static char* line;

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
		case DIGIT: test_class = isdigit;
			break;
		case ALPHANUM: test_class = isalnum;
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
	char* w_end = jump_cclass(sym, ALPHANUM);
	size_t length = w_end - sym;

	char* word = malloc(length + 1);
	if (word == NULL) return NULL;
	
	memcpy(word, sym, length); 
	word[length] = '\0';

	sym = w_end;
	return word;
}




/* FIXME DBG content */
Var exec_fun(char* fun, v_list args){

	/* TODO */
	printf("* TODO HERE EXEC FUN: %s\nARGS:\n", fun);
	unsigned int i;
	for(i = 0; i < args->length; i++){
		print_var(args->var_list[i]);
	}
	fflush(stdout);

	/* TODO */
	float* f = malloc(sizeof (float));
	*f = 12345678;
	return new_var(NULL, f, FLOAT);
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
 * @return The Var as a result of the function being evaluated, 
 *	   of NULL in case of error
 */
Var eval_fun(char* fun){

	v_list args = new_v_list();
	if (args == NULL) return NULL;

	do {
		sym++;

		Var v = eval_expression();
		if (v == NULL || push_var(args, v) == NULL) goto error;

		sym = jump_cclass(sym, SPACE);

	} while ( *sym == ',');

	/* Check for a valid end-of-function */
	if (*sym != ')'){
		set_err(ESYNTAX,sym);
		goto error;
	} 

	/* By convention move to the char 
	   right next the expression */	
	sym++;

	Var ret = exec_fun(fun, args);	
	drop_v_list(args, true);	
	
	return ret;

error:
	drop_v_list(args, true);	
	return NULL;
}			



/* sym = '[' */	
Matrix eval_vector(char* vect){
	/* TODO */
	printf("* TODO EVAL VECT %s\n",vect);
	return NULL;
}




/* sym = args */	
int exec_cmd(char* cmd){
	/*TODO*/
	printf("* TODO HERE EXECUTE CMD:  %s *\n",cmd);
	return 0;
}			


/**
 * Declare a new variable having a value equal to the expression
 * next to the global pointer sym.
 * @param name Key-name of the variable to declare
 * @return The address of the stored variable in case of success,
 * 	   otherwise NULL (in this case the parameter name will not
 *	   be stored and must be freed by the user)
 */
Var declare_var(char* name){

	/* Evaluate expression */
	Var v_var = eval_expression();
	if (v_var == NULL) return NULL;

	/* Store a renamed copy of v_var */
	Var var = set_var(name, v_var->val, v_var->type);
	if (var == NULL){
		/* Free all the data */
		drop_var(v_var);
		return NULL;
	}

	/* Free just the unnamed vars */
	if (v_var->name == NULL) free(v_var);

	return var;
}			

/**
 * Evaluates one of the following kind of expression:
 *	- function
 *	- vector
 *	- variable
 *	- numeric value
 *
 * ntil one of the end_symbols, and stops there 
 * An expression could be a variable, a function, a numerical value or a vector
 *
 * @param end_symbols Cannot be alphanumerical, [()] or any kind of space
 */
//Var eval_expression(char* end_symbols, int n_sym){
Var eval_expression(void){
	

	/* Jump all the blank characters */	
	sym = jump_cclass(sym, SPACE);

	/* Evaluate next token */
	void* value = NULL;	
	Var var_result = NULL;	

	if (isalpha(*sym)) {  	   /* Variable or function */		

		char* word = pop_word();
		if (word == NULL) return NULL;

		sym = jump_cclass(sym, SPACE);

		if (*sym == '('){ 

			var_result = eval_fun(word);
		} else {

			var_result = get_var(word);
			if (var_result == NULL) set_err(ENOTAVAR, word);
		}

		free(word);
		
	} else if (isdigit(*sym)) { /* Num value */

		value = malloc(sizeof (float));
		if (value == NULL) return NULL;

		char* tmp;
		(*(float*)value) = strtof(sym, &tmp); //TODO what happens if we dont use tmp?
		sym = tmp;

		var_result = new_var(NULL, value, FLOAT);

	} else if (*sym == '[') {   /* Vector */

		value = eval_vector(sym);
		var_result = new_var(NULL, value, MATRIX);

	} else {
		set_err(ESYNTAX,sym);
		return NULL;
	}

	/* Error check */
	if (var_result == NULL) {
		free(value);
		return NULL;
	}

	return var_result;
}

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

	Var ret = NULL;
	switch (*sym) {
		case ':':
			/* Line is a var declaration */
			sym++;
			ret = declare_var(an_token); //Note: do not free an_token
			if (ret == NULL) goto error;
			puts("*TODO PRINT VAR DECLARATION*"); // TODO add result display
			break;
		case '(':
			/* Line is a function call */
			ret = eval_fun(an_token);
			if (ret == NULL) goto error;
			puts("TODO PRINT FUN EVALUATION"); // TODO add result display
			free(an_token); drop_var(ret);
			break;
		default:;
			/* Line is an internal command */
			int ret_cmd = exec_cmd(an_token);
			if (ret_cmd == -1) goto error;
			free(an_token);
	}

	/* Move to the end of the instruction */
	sym = jump_cclass(sym, SPACE);

	return 0;

error:
	free(an_token);
	return -1;
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