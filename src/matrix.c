#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 Fonction qui créé une nouvelle matrice et qui la renvoie
*/
Matrix newMatrix(unsigned int nb_rows, unsigned int nb_columns)
{
	if(nb_rows==0 || nb_columns==0)
	{
		return NULL; //erreur 
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

/*
 Fonction qui renvoie l'élément a la row ligne et column colonne de la matrice m
*/
float getElt(Matrix m,unsigned int row,unsigned int column)
{
	return (m->mat[column+row*m->ncols]);
}


/*
 Function qui met l'élément val à la ligne row et la colonne column de la matrice m
*/
void setElt(Matrix m,unsigned int row,unsigned int column,float val)
{
	if(row>m->nrows || column>m->ncols)
	{
		return;
	}
	m->mat[row*m->ncols+column]=val;
}


/*
 Fonction qui renvoie l'adresse de l'élément a la ligne i et colonne j de la matrice m
*/
float* getAddr(Matrix m,unsigned int i,unsigned int j)
{
	if(i>m->nrows || j>m->ncols)
	{
		return NULL;
	}
	return (&(m->mat[i*m->ncols+j]));
}


/*
 Fonction qui renvoie une nouvelle matrice qui est la copie de la matrice passé en argument
 En cas d'erreur d'allocation la fonction renvoie NULL
*/
Matrix copyMatrix(Matrix m)
{
	Matrix p=newMatrix(m->nrows,m->ncols);
	if(p==NULL)
	{
		return NULL;
	}
	memcpy(getAddr(p,0,0),getAddr(m,0,0),m->ncols*m->nrows*sizeof(float));
	return p;
}


/*
 Fonction qui affiche la matrice passé en argument
 Si la variable m contient NULL on affiche un message d'erreur
*/
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


/*
 Fonction qui créé et renvoie une nouvelle matrice identité de dimension passé en argument
 En cas d'erreur d'allocation la fonction renvoie NULL
*/
Matrix identite(unsigned int nb_cote)
{
	Matrix mx=newMatrix(nb_cote,nb_cote);
	if(mx==NULL)
	{
		return NULL;
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


/*
 Fonction qui supprime la matrice passé en argument pour éviter les fuites de mémoire
*/
void dropMatrix(Matrix m)
{	
	if (m == NULL) return;
	free(m->mat);
	free(m);
}
