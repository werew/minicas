#ifndef __MATRIX_H
#define __MATRIX_H

typedef float E;
typedef struct s_matrix
{
	E* mat;
	int nrows;
	int ncols;
}*Matrix;

Matrix newMatrix(int nb_rows, int nb_columns);
E getElt(Matrix m,int row,int column);
void setElt(Matrix m,int row,int column,E val);
float* getAddr(Matrix m,int i,int j);
Matrix copyMatrix(Matrix m);
void deleteMatrix(Matrix m);
void displayMatrix(Matrix m);
Matrix identite(int nb_cote);

#endif
