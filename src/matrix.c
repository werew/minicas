#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Matrix newMatrix(unsigned int nb_rows, unsigned int nb_columns)
{
	if(nb_rows==0 || nb_columns==0)
	{
		return NULL; //erreur dimension
	}
	Matrix m=(Matrix)malloc(sizeof(struct s_matrix));
	if(m==NULL)
	{
		return NULL;	//erreur
	}
	m->mat=(float*)calloc(nb_rows*nb_columns,sizeof(float));
	if(m->mat==NULL)
	{
		return NULL;	//erreur
	}
	m->nrows=nb_rows;
	m->ncols=nb_columns;
	return m;
}

float getElt(Matrix m,unsigned int row,unsigned int column)
{
	return (m->mat[column+row*m->ncols]);
}

void setElt(Matrix m,unsigned int row,unsigned int column,float val)
{
	m->mat[row*m->ncols+column]=val;
}

float* getAddr(Matrix m,unsigned int i,unsigned int j)
{
	return (&(m->mat[i*m->ncols+j]));
}

Matrix copyMatrix(Matrix m)
{
	Matrix p=newMatrix(m->nrows,m->ncols);
	if(p==NULL)
	{
		return NULL;	//TODO
	}
	memcpy(getAddr(p,0,0),getAddr(m,0,0),m->ncols*m->nrows*sizeof(float));
	return p;
}


void displayMatrix(Matrix m)
{
	unsigned int i,j;
	for(i = 0 ; i < m->nrows; i++)
	{
		printf("\t\t[");
		for (j = 0; j < m->ncols; j++){
			printf("%- 6.4g",getElt(m,i,j));
		}
		printf("]\n");
	}
	printf("\n\n");
}

Matrix identite(unsigned int nb_cote)
{
	Matrix mx=newMatrix(nb_cote,nb_cote);
	if(mx==NULL)
	{
		return NULL;	//TODO
	}
	unsigned int i,j;
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

void dropMatrix(Matrix m)
{
	free(m->mat);
	free(m);
}
