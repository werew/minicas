#include "matrix.h"

#ifndef __OPERATION_H
#define __OPERATION_H

typedef struct s_maillon
{
	int i,j;
	float coef;
	int op;
	struct s_maillon* suiv;
}*maillon;

Matrix addition(Matrix a,Matrix b);
Matrix soustraction(Matrix a,Matrix b);
Matrix multiplication(Matrix a,Matrix b);
Matrix multScal(float a,const Matrix m);
Matrix expo(Matrix m,unsigned int p);
Matrix transpose(Matrix m);
Matrix fusionMat(Matrix A,Matrix B);
int PivotPartiel(Matrix m,unsigned int i,unsigned int *p);
int Pivot(Matrix m,unsigned int i,unsigned int* p);
int addmultiple(Matrix A,unsigned int i,unsigned int j,float c);
int echangeLigne(Matrix m,unsigned int i,unsigned int j);
int diviseLigne(Matrix A,unsigned int i,float c);
Matrix echelonnage(Matrix m);
Matrix triangulaire(Matrix m,float* c,maillon* ch,int* permut,int fct_pivot);
int determinant(Matrix m,float* c);
void remontee(Matrix A,Matrix X);
int solve(Matrix A, Matrix B,Matrix* F);
Matrix bienEchelonner(Matrix A);
Matrix sliceMatrix(Matrix A,unsigned int i1,unsigned int i2,unsigned int j1,unsigned int j2);
int invert(Matrix m,Matrix* D);
int rank(Matrix A);
int decomposition(Matrix A,Matrix* L, Matrix* U,Matrix* P);
int noyau(Matrix m,Matrix* base);

#endif
