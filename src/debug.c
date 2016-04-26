/* XXX THIS FILE WILL NOT BE PART OF THE FINAL PROGRAM */

#include <stdio.h>
#include "debug.h"
#include "storage.h"
#include "xtypes.h"


void print_var(Var v){
	if (v == NULL) {
		puts("Invalid variable");
		return;
	}
	
	printf("-------------VAR 0x%p-----------\n", v);

	char* type;
	switch (v->type) {
		case FLOAT: type = "FLOAT";
			break;
		case MATRIX: type = "MATRIX";
			break;
		default: type = "UNDEFINED";
	}

	printf("Var: %s, value: %f, type: %s\n\n", v->name, (*(float*) v->val), type);
}
