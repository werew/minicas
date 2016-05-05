#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "operation.h"
#include "matrix.h"

maillon newChaine()
{return NULL;}

maillon push(maillon m,int i,int j,float x,int op)
{
	maillon n=(maillon)malloc(sizeof(struct s_maillon));
	n->i=i;
	n->j=j;
	n->coef=x;
	n->suiv=m;
	n->op=op;
	return n;
}



Matrix addition(Matrix a, Matrix b)
{
	if(a->ncols!=b->ncols||a->nrows!=b->nrows)
	{
		return NULL;
	}
	else
	{
		Matrix c=newMatrix(a->nrows,a->ncols);
		int i,j;
		for(i=0;i<a->nrows;i++)
		{
			for(j=0;j<a->ncols;j++)
			{
				setElt(c,i,j,(getElt(a,i,j)+getElt(b,i,j)));
			}
		}
		return c;
	}
}

Matrix soustraction(Matrix a, Matrix b)
{
	if(a->ncols!=b->ncols||a->nrows!=b->nrows)
	{
		return NULL;
	}
	else
	{
		Matrix c=newMatrix(a->nrows,a->ncols);
		int i,j;
		for(i=0;i<a->nrows;i++)
		{
			for(j=0;j<a->ncols;j++)
			{
				setElt(c,i,j,(getElt(a,i,j)-getElt(b,i,j)));
			}
		}
		return c;
	}
}

Matrix multiplication(Matrix a,Matrix b)
{
	Matrix c=newMatrix(a->nrows,b->ncols);
	int i,j,k;
	float total=0;

	for(i=0;i<a->nrows;i++)
	{
		for(j=0;j<a->ncols;j++)
		{
			total=0;
			for(k=0;k<b->nrows;k++)
			{
				total+=getElt(a,i,k)*getElt(b,k,j);
			}
			setElt(c,i,j,total);
		}
	}
	return c;
}

Matrix transpose(Matrix m)
{
	Matrix P=newMatrix(m->ncols,m->nrows);
	int i,j;
	for(i=0;i<m->nrows;i++)
	{
		for(j=0;j<m->ncols;j++)
		{
			setElt(P,j,i,(getElt(m,i,j)));
		}
	}
	return P;
}


Matrix mult_Scal(float a,Matrix m)
{
	Matrix P=newMatrix(m->nrows,m->ncols);
	int i,j;
	for(i=0;i<P->nrows;i++)
	{
		for(j=0;j<P->ncols;j++)
		{
			setElt(P,i,j,(getElt(m,i,j)*a));
		}
	}
	return P;
}



Matrix fusionMat(Matrix A,Matrix B)
{
	Matrix C=newMatrix(A->nrows,A->ncols+B->ncols);
	int i;
	for(i=0;i<A->nrows;i++)
	{
		memcpy(getAddr(C,i,0),getAddr(A,i,0),A->ncols*sizeof(float));
		memcpy(getAddr(C,i,A->ncols),getAddr(B,i,0),B->ncols*sizeof(float));
	}
	return C;
}


int PivotPartiel(Matrix m,int i)
{
	if(i>m->nrows)
	{
		return -1;
	}
	int p=i;
	float v=fabs(getElt(m,i,i));
	int j;
	float tmp;
	for(j=i+1;j<m->nrows;j++)
	{
		if((tmp=fabs(getElt(m,j,i)))>v)
		{
			p=j;
			v=tmp;
		}
	}
	return v==0?-1:p;	//TODO valeur par defaut
}

void addmultiple(Matrix A,int i,int j,float c)
{
	int k;
	for(k=0;k<A->ncols;k++)
	{
		setElt(A,i,k,getElt(A,i,k)+c*getElt(A,j,k));
	}
}

void echangeLigne(Matrix m,int i,int j)
{
	int k;
	float tmp;
	for (k=0;k<m->ncols;k++)
	{
		tmp=getElt(m,i,k);
		setElt(m,i,k,getElt(m,j,k));
		setElt(m,j,k,tmp);
	}
}

Matrix echelonnage(Matrix m)
{
	Matrix P=triangulaire(m,NULL,NULL,NULL);
	int i;

	for(i=0;i<P->nrows;i++)
	{
		if(getElt(P,i,i)!=0)
		{
			diviseLigne(P,i,getElt(P,i,i));	//TODO coordonée de getElt juste?
			setElt(P,i,i,1);
		}
	}
	return P;
}

Matrix triangulaire(Matrix m,float* c,maillon* ch,int* permut)
{
	Matrix P=copyMatrix(m);
	int i,j;
	if(permut!=NULL)
	{
		*permut=0;
	}
	for(i=0;i<P->ncols-1;i++)
	{
		j=PivotPartiel(P,i);
		if(j==-1)
		{
			continue;
		}
		if(j!=i)
		{	if(permut!=NULL)
			{
				*permut=1;
			}
			echangeLigne(P,i,j);
			if(ch!=NULL)
			{
				*ch=push(*ch,i,j,0,0);
			}
			if(c!=NULL)
			{
				*c=-(*c);
			}
		}
		for(j=i+1;j<P->nrows;j++)
		{
			float coef=(-getElt(P,j,i)/getElt(P,i,i));
			addmultiple(P,j,i,coef);
			if(ch!=NULL)
			{
				*ch=push(*ch,j,i,coef,1);
			}
		}
	}
	return P;
}

float determinant (Matrix m)
{
	float c=1;
	Matrix P=triangulaire(m,&c,NULL,NULL);

	int i;
	for(i=0;i<P->ncols;i++)
	{
		c*=getElt(P,i,i);
	}
	//TODO supprimer matrice intermediaire
	return c;
}

/*void remontee(Matrix A,Matrix X)
{
	int i,j;
	for(i=A->ncols-1;i>=0;i--)	//TODO -1 ou -2
	{
		setElt(X,i,0,getElt(A,i,A->ncols-1));
		for(j=i+1;j<A->ncols;j++)
		{
			float new_elt=getElt(X,i,0)-getElt(A,i,j)*getElt(X,j,0);
			setElt(X,i,0,new_elt);
		}
		setElt(X,i,0,(getElt(X,i,0)/getElt(A,i,i)));
	}
}*/

Matrix solve(Matrix A, Matrix B)
{
	if(determinant(A)==0)
	{
		return NULL;
	}
	Matrix C=fusionMat(A,B);
	Matrix D=echelonnage(C);
	//remontee(D,X);
	Matrix E=bienEchelonner(D);
	return sliceMatrix(E,A->ncols);
}

Matrix expo(Matrix m,int p)
{
	Matrix A=copyMatrix(m);
	int i;
	for(i=1;i<p;i++)
	{
		A=multiplication(A,m);
	}
	return A;
}

void diviseLigne(Matrix A,int i,float c)
{
	int j;
	for(j=0;j<A->ncols;j++)
	{
		setElt(A,i,j,getElt(A,i,j)/c);
	}
}

Matrix bienEchelonner(Matrix A)
{
	Matrix B=copyMatrix(A);

	int i,j;
	for(i=A->nrows-1;i>0;i--)
	{
		//diviseLigne(B,i,getElt(B,i,i));
		for(j=0;j<i;j++)
		{
			addmultiple(B,j,i,-getElt(B,j,i));
		}
	}
	return B;
}

Matrix sliceMatrix(Matrix A,int i)
{
	Matrix B=newMatrix(A->nrows,A->ncols-i);

	int j;
	for(j=0;j<B->nrows;j++)
	{
		memcpy(getAddr(B,j,0),getAddr(A,j,i),(A->ncols-i)*sizeof(float));
	}
	return B;
}

Matrix invert(Matrix m)
{
	if(determinant(m)==0)
	{
		return NULL;
	}
	else
	{
		Matrix Id=identite(m->ncols);
		Matrix A=fusionMat(m,Id);
		Matrix B=echelonnage(A);
		Matrix C=bienEchelonner(B);
		Matrix D=sliceMatrix(C,m->ncols);
		return D;
	}
}

int ligneZero(Matrix A,int l)
{
	int i;
	for(i=0;i<A->ncols;i++)
	{
		if(getElt(A,l,i)!=0)
		{
			return 0;
		}
	}
	return 1;
}

int rank(Matrix A)
{
	int lz=0;
	int i;
	for(i=0;i<A->nrows;i++)
	{
		lz+=ligneZero(A,i);
	}
	return A->nrows-lz;
}


void decomposition(Matrix A,Matrix* L, Matrix* U,Matrix* P)
{
	*L=newMatrix(A->nrows,A->ncols);
	*U=newMatrix(A->nrows,A->ncols);
	maillon m=newChaine();
	int permut;
	*U=triangulaire(A,NULL,&m,&permut);
	if(permut)
	{
		*P=identite(A->ncols);
	}
	else
	{
		*P=NULL;
	}
	Matrix id=identite(A->nrows);
	Matrix E,tmp;
	while(m!=NULL)
	{
		E=identite(A->nrows);
		if(m->op==1)
		{
			setElt(E,m->i,m->j,m->coef);	//verifier valeur pour savoir si cest inversion de ligne ou comvinaison limeaire
			tmp=multiplication(id,E);
			dropMatrix(E);
			dropMatrix(id);
			id=tmp;

		}
		else
		{
			echangeLigne(*P,m->i,m->j);
		}
		maillon suiv=m->suiv;
		free(m);
		m=suiv;
	}

	*L=invert(id);
	dropMatrix(id);
}
