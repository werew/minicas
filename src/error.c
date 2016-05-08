#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "error.h"

/* Shared global variable definition */
char err_arg[MAX_EMSG];
Error local_err = SUCCESS;

void inst_err(Error e, char* arg){
	set_err(e, arg);
	print_error();
}

/**
 * Set custom error 
 * @param e Error
 * @param arg Error argument according to print_error
 */
void set_err(Error e, char* arg){
	local_err = e;
	if (arg != NULL) {
		strncpy(err_arg, arg ,MAX_EMSG);
	}
}


/* If set print both libc errno string and custom local error */
void print_error(void){
	
	/* Print libc error */
	if (errno != 0) {
		perror("");
		errno = 0;
	} 

	/* Print custom error */
	switch (local_err) {
		case ESYNTAX:
			fprintf(stderr,"Syntax error --> %s\n", err_arg);
			break;
		case ENOTAFUN:
			fprintf(stderr,"Not a valid function: %s\n", err_arg);
			break;
		case ENOTACMD:
			fprintf(stderr,"Not a valid command: %s\n", err_arg);
			break;
		case ENOTAVAR:
			fprintf(stderr,"Not a valid variable: %s\n", err_arg);
			break;
		case ETYPE:
			fprintf(stderr,"Type error: %s\n", err_arg);
			break;
		case ELOAD:
			fprintf(stderr,"Cannot load: %s\n", err_arg);
			break;
		case EMXDIM:
			fprintf(stderr,"Bad dimensions: %s\n", err_arg);
			break;
		case ENOVAL:
			fprintf(stderr,"No such value: %s\n", err_arg);
			break;
		case EMISSARG:
			fprintf(stderr,"Argument missing: %s\n", err_arg);
			break;
		default: /* Not an error */
			return;
	}
}


