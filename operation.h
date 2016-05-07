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
Matrix expo(Matrix m,int p);
Matrix transpose(Matrix m);
Matrix fusionMat(Matrix A,Matrix B);
int PivotPartiel(Matrix m,int col);
int Pivot(Matrix m,int i);
void addmultiple(Matrix A,int i,int j,float c);
void echangeLigne(Matrix m,int i,int j);
void diviseLigne(Matrix A,int i,float c);
Matrix echelonnage(Matrix m);
Matrix triangulaire(Matrix m,float* c,maillon* ch,int* permut,int fct_pivot);
float determinant(Matrix m);
void remontee(Matrix A,Matrix X);
Matrix solve(Matrix A,Matrix B);
Matrix bienEchelonner(Matrix A);
Matrix sliceMatrix(Matrix A,int i1,int i2,int j1,int j2);
Matrix invert(Matrix m);
int rank(Matrix A);
void decomposition(Matrix A,Matrix* L, Matrix* U,Matrix* P);
Matrix noyau(Matrix m);

#endif
