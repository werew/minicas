#ifndef __MATRIX_H
#define __MATRIX_H

typedef struct s_matrix
{
	float* mat;
	int nrows;
	int ncols;
}*Matrix;

Matrix newMatrix(int nb_rows, int nb_columns);
float getElt(Matrix m,int row,int column);
void setElt(Matrix m,int row,int column,float val);
float* getAddr(Matrix m,int i,int j);
Matrix copyMatrix(Matrix m);
void deleteMatrix(Matrix m);
void displayMatrix(Matrix m);
Matrix identite(int nb_cote);

#endif
