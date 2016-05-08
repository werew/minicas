#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "parser.h"
#include "init.h"


void interpreter_loop(FILE* f_in){

	struct stat buf; 
	char *line = NULL;
	size_t n=0;

	if (!fstat(0, &buf) && S_ISREG(buf.st_mode)) {
		// Output is a regular file
		while (getline(&line, &n, f_in) != -1) {
			eval_input(line);
			free(line); line=NULL;
        	}

    	} else {
		// Output is probably a terminal
		printf("minic@s> ");
		while (getline(&line, &n, f_in) != -1) {
			eval_input(line);
 			printf("minic@s> ");
 			free(line); line=NULL;
		}
	}
	free(line);
}

int main(int argc, char **argv) {

	// Open input 	
	FILE *f_in;
	f_in = (argc > 1)? fopen(argv[1],"r") : fdopen(0, "r");
	if (f_in == NULL) {
		perror("cannot open file");
		exit(1);
	}
	
	
	/* Init environment */
	init_env();

	/* Launch interpreter */
	interpreter_loop(f_in);
	
	drop_pool();

      	 
	return 0;   
}
