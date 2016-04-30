#include "matrix.h"

#ifndef __OPERATION_H
#define __OPERATION_H

Matrix addition(Matrix a,Matrix b);
Matrix soustraction(Matrix a,Matrix b);
Matrix multiplication(Matrix a,Matrix b);
Matrix multScal(float a,const Matrix m);
Matrix expo(Matrix m,int p);
Matrix transpose(Matrix m);
Matrix fusionMat(Matrix A,Matrix B);
int PivotPartiel(Matrix m,int col);
void addmultiple(Matrix A,int i,int j,float c);
void echangeLigne(Matrix m,int i,int j);
void diviseLigne(Matrix A,int i,float c);
Matrix echelonnage(Matrix m);
float triangulaire(Matrix m,Matrix P);
float determinant(Matrix m);
void remontee(Matrix A,Matrix X);
Matrix solve(Matrix A,Matrix B);
Matrix bienEchelonner(Matrix A);
Matrix sliceMatrix(Matrix A,int i);
Matrix invert(Matrix m);
int rank(Matrix A);

#endif
