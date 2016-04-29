#ifndef _MOD_MATRIX_H
#define _MOD_MATRIX_H


#include "matrix.h"
#include "ref_all.h"

void load_mod_matrix(void);

Ref matrix_call(ref_list args);
Ref transpose_call(ref_list args);


#endif 
