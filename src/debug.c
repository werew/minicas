/* XXX THIS FILE WILL NOT BE PART OF THE FINAL PROGRAM */

#include <stdio.h>
#include "debug.h"
#include "var.h"
#include "ref.h"
#include "xtypes.h"


void print_ref(Ref r){
	if (v == NULL) {
		puts("Invalid reference");
		return;
	}
	
	printf("-------------REF 0x%p-----------\n", v);

	char* type;
	switch (r->type) {
		case FUN: type = "FUN";
			break;
		case CMD: type = "CMD";
			break;
		case VAR: type = "VAR";
			print_var(r->inst);
			break;
	}

	printf("Ref: %s, type: %s\n", v->name, type);
}

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

	printf("Var value: %f, type: %s\n\n", v->name, (*(float*) v->val), type);
}
