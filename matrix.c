#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Matrix newMatrix(int nb_rows, int nb_columns)
{
	Matrix m=(Matrix)malloc(sizeof(struct s_matrix));
	m->mat=(E*)malloc(nb_rows*nb_columns*sizeof(E));
	m->nrows=nb_rows;
	m->ncols=nb_columns;
	return m;
}

E getElt(Matrix m,int row,int column)
{
	return (m->mat[column+row*m->ncols]);
}

void setElt(Matrix m,int row,int column,E val)
{
	m->mat[row*m->ncols+column]=val;
}

float* getAddr(Matrix m,int i,int j)
{
	return (&(m->mat[i*m->ncols+j]));
}

Matrix copyMatrix(Matrix m)
{
	Matrix p=newMatrix(m->nrows,m->ncols);
	memcpy(getAddr(p,0,0),getAddr(m,0,0),m->ncols*m->nrows*sizeof(float));
	return p;
}

void deleteMatrix(Matrix m)
{
	free(m->mat);
	free(m);
}

void displayMatrix(Matrix m)
{
	if(m==NULL)
	{
		printf("Erreur Matrice");
	}
	int i;
	for(i=0;i<m->nrows*m->ncols;i++)
	{
		printf("%.3f	 ",m->mat[i]);
		if((i+1)%m->ncols==0)
		{
			printf("\n");
		}
	}
}

Matrix identite(int nb_cote)
{
	Matrix mx=newMatrix(nb_cote,nb_cote);
	int i,j;
	for(i=0;i<nb_cote;i++)
	{
		for(j=0;j<nb_cote;j++)
		{
			if(j==i)
				setElt(mx,i,j,1);
			else
				setElt(mx,i,j,0);
		}
	}
	return mx;
}
