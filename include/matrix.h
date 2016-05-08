#ifndef __MATRIX_H
#define __MATRIX_H

typedef struct s_matrix
{
	float* mat;	//Tableau à une dimension pour stocker les matrices
	unsigned int nrows;	//Nombre de ligne de la matrice
	unsigned int ncols;	//Nombre de colonne de la matrice
}*Matrix;


Matrix newMatrix(unsigned int nb_rows, unsigned int nb_columns);
float getElt(Matrix m,unsigned int row,unsigned int column);
void setElt(Matrix m,unsigned int row,unsigned int column,float val);
float* getAddr(Matrix m,unsigned int i,unsigned int j);
Matrix copyMatrix(Matrix m);
void displayMatrix(Matrix m);
Matrix identite(unsigned int nb_cote);
void dropMatrix(Matrix m);

#endif
