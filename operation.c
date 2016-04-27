#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "operation.h"
#include "matrix.h"

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
	int p=i;
	int v=fabs(getElt(m,i,i));
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
	return p;	//TODO valeur par defaut
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

float echelonnage(Matrix m,Matrix P)
{
	int i,j;
	float c=1;
	for(i=0;i<P->ncols-1;i++)
	{
		j=PivotPartiel(P,i);
		if(j!=i)
		{
			echangeLigne(P,i,j);
			c=-c;
		}
		for(j=i+1;j<P->ncols;j++)
		{
			addmultiple(P,j,i,(-getElt(P,j,i)/getElt(P,i,i)));
		}
	}
	return c;
}

Matrix triangulaire(Matrix m)
{
	Matrix P=copyMatrix(m);
	echelonnage(m,P);
	return P;
}

float determinant (Matrix m)
{
	Matrix P=copyMatrix(m);
	float c=echelonnage(m,P);

	int i;
	for(i=0;i<P->ncols;i++)
	{
		c*=getElt(P,i,i);
	}
	return c;
}

void remontee(Matrix A,Matrix X)
{
	int i,j;
	for(i=A->ncols-1;i>=0;i--)	//TODO -1 ou -2
	{
		setElt(X,i,0,getElt(A,i,A->ncols-1));
		for(j=i+1;A->ncols;i++)
		{
			float new_elt=getElt(X,i,0)-getElt(A,i,j)*getElt(X,j,0);
			setElt(X,i,0,new_elt);
		}
		setElt(X,i,0,(getElt(X,i,0)/getElt(A,i,i)));
	}
}

void solve(Matrix A, Matrix B, Matrix X)
{
	Matrix C=fusionMat(A,B);
	triangulaire(C);
	remontee(C,X);
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

void diviseLigne(Matrix A,int i,int c)
{
	int j;
	for(j=0;j<A->ncols;j++)
	{
		setElt(A,i,j,getElt(A,i,j)/c);
	}
}

Matrix bienEchelonner(Matrix A,int r)
{
	Matrix B=copyMatrix(A);

	int i,j;
	for(i=r-1;r>=0;i--)
	{
		diviseLigne(B,i,getElt(B,i,i));
		for(j=0;j<i;j++)
		{
			addmultiple(B,j,i,getElt(B,j,i));
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
		Matrix B=triangulaire(A);
		Matrix C=bienEchelonner(B,m->nrows);
		Matrix D=sliceMatrix(C,m->ncols);
		return D;
	}
}
