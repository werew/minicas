#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "operation.h"
#include "matrix.h"

#define EPSILON 0.00001

maillon newChaine()
{return NULL;}

maillon push(maillon m,int i,int j,float x,int op)
{
	maillon n=(maillon)malloc(sizeof(struct s_maillon));
	if(n==NULL)
	{
		return NULL;	//TODO
	}
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
		return NULL;	//Erreur dimension
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
		return NULL;	//erreur dimension
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
	if(a->ncols!=b->nrows)
	{
		return NULL;	//erreur dimension
	}
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
	if(A->nrows!=B->nrows)
	{
		return NULL;	//ereur dimension
	}
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
	return v==0?-1:p;
}


int Pivot(Matrix m,int i)
{
	if(i>m->nrows)
	{
		return -1;
	}
	int j;
	for(j=i;j<m->nrows;j++)
	{
		if(getElt(m,j,i)!=0)
		{
			return j;
		}
	}
	return -1;
}

int addmultiple(Matrix A,int i,int j,float c)
{
	if(i>A->nrows || j>A->nrows)
	{
		return 0;	//Erreur accès ligne 
	}
	int k;
	float coef;
	for(k=0;k<A->ncols;k++)
	{
		coef=getElt(A,i,k)+c*getElt(A,j,k);
		if(coef<EPSILON && coef>(-EPSILON))
		{
			setElt(A,i,k,0);
		}
		else
		{
			setElt(A,i,k,coef);
		}
	}
	return 1;
}

int echangeLigne(Matrix m,int i,int j)
{
	if(i>m->nrows || j>m->nrows)
	{
		return 0;	//Erreur ligne
	}
	Matrix tmp=newMatrix(1,m->ncols);
	memcpy(getAddr(tmp,0,0),getAddr(m,i,0),m->ncols*sizeof(float));
	memcpy(getAddr(m,i,0),getAddr(m,j,0),m->ncols*sizeof(float));
	memcpy(getAddr(m,j,0),getAddr(tmp,0,0),m->ncols*sizeof(float));

	return 1;
}


Matrix echelonnage(Matrix m)
{
	Matrix P=triangulaire(m,NULL,NULL,NULL,1);
	if(P==NULL)
	{
		return NULL;
	}
	int i;

	for(i=0;i<P->nrows;i++)
	{
		if(getElt(P,i,i)!=0)
		{
			if(diviseLigne(P,i,getElt(P,i,i))==0)
			{
				return NULL; //erreur
			}
			if(getElt(P,i,i)!=0)
			{
				setElt(P,i,i,1);
			}
		}
	}
	return P;
}

Matrix triangulaire(Matrix m,float* c,maillon* ch,int* permut,int fct_pivot)
{
	Matrix P=copyMatrix(m);
	int i,j;
	if(permut!=NULL)
	{
		*permut=0;
	}
	for(i=0;i<P->ncols-1;i++)
	{
		if(fct_pivot==0)
		{
			j=Pivot(P,i);
		}
		else
		{
			j=PivotPartiel(P,i);
		}
		if(j==-1)
		{
			continue;
		}
		if(j!=i)
		{	if(permut!=NULL)
			{
				*permut=1;
			}
			if(echangeLigne(P,i,j)==0)
			{
				return NULL;
			}
			if(ch!=NULL)
			{
				if((*ch=push(*ch,i,j,0,0))==NULL)
				{
					return NULL;	//TODO
				}
			}
			if(c!=NULL)
			{
				*c=-(*c);
			}
		}
		for(j=i+1;j<P->nrows;j++)
		{
			float coef=(-getElt(P,j,i)/getElt(P,i,i));
			if(addmultiple(P,j,i,coef)==0)
			{
				return NULL;
			}
			if(ch!=NULL)
			{
				if((*ch=push(*ch,j,i,coef,1))==NULL)
				{
					return NULL;	//TODO
				}
			}
		}
	}
	return P;
}

float determinant (Matrix m)
{
	float c=1;
	Matrix P=triangulaire(m,&c,NULL,NULL,1);
	if(P==NULL)
	{
		//erreur
	}

	int i;
	for(i=0;i<P->ncols;i++)
	{
		c*=getElt(P,i,i);
	}
	dropMatrix(P);
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
	if(A->nrows!=B->nrows || B->ncols!=1)
	{
		return NULL; //erreur dimension
	}
	if(determinant(A)==0)
	{
		return NULL;	//erreur matrix
	}
	Matrix C=fusionMat(A,B);
	if(C==NULL)
	{
		return NULL;	//TODO erreur
	}
	Matrix D=echelonnage(C);
	if(D==NULL)
	{
		return NULL; //TODO Erreur
	}
	Matrix E=bienEchelonner(D);
	if(E==NULL)
	{
		return NULL;	//TODO erreur
	}
	Matrix F=sliceMatrix(E,0,A->nrows-1,A->ncols,A->ncols);
	if(F==0)
	{
		return NULL; //TODO erreur
	}

	dropMatrix(C);
	dropMatrix(D);
	dropMatrix(E);

	return F;
}

Matrix expo(Matrix m,int p)
{
	Matrix A=copyMatrix(m);
	int i;
	for(i=1;i<p;i++)
	{
		if((A=multiplication(A,m))==NULL)
		{
			return NULL;	//TODO erreur
		}
	}
	return A;
}

int diviseLigne(Matrix A,int i,float c)
{
	if(c==0)
	{
		return 0;	//TODO erreur
	}
	int j;
	for(j=0;j<A->ncols;j++)
	{
		setElt(A,i,j,getElt(A,i,j)/c);
	}
	return 1;
}

Matrix bienEchelonner(Matrix A)
{
	Matrix B=copyMatrix(A);

	int i,j;
	for(i=A->nrows-1;i>0;i--)
	{
		for(j=0;j<i;j++)
		{
			if(addmultiple(B,j,i,-getElt(B,j,i))==0)
			{
				return NULL; //erreur
			}
		}
	}
	return B;
}

Matrix sliceMatrix(Matrix A,int i1,int i2,int j1,int j2)
{
	if(i1<0 || i2<0 || j1<0 || j2<0)
	{
		return NULL;	//TODO erreur indice négatif
	}
	if(i1>i2 || j1>j2)
	{
		return NULL;	//TODO erreur indice
	}
	if(i1>A->nrows || i2>A->nrows || j1>A->ncols || j2>A->ncols)
	{
		return NULL;	//TODO erreur indice plus grand que dimension matrice
	}
	Matrix B=newMatrix(i2-i1+1,j2-j1+1);

	int k;
	for(k=i1;k<=i2;k++)
	{
		memcpy(getAddr(B,k-i1,0),getAddr(A,k,j1),(j2-j1+1)*sizeof(float));
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
		if(Id==NULL)
		{
			return NULL;	//TODO erreur
		}
		Matrix A=fusionMat(m,Id);
		if(A==NULL)
		{
			return NULL;	//TODO
		}
		Matrix B=echelonnage(A);
		if(B==NULL)
		{
			return NULL;	//TODO
		}
		Matrix C=bienEchelonner(B);
		if(C==NULL)
		{
			return NULL;	//TODO
		}
		Matrix D=sliceMatrix(C,0,C->nrows-1,m->ncols,C->ncols-1);
		if(D==NULL)
		{
			return NULL;	//TODO
		}
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
	Matrix B=echelonnage(A);
	if(B==NULL)
	{
		return -1;	//erreur
	}
	displayMatrix(B);
	int lz=0;
	int i;
	for(i=0;i<B->nrows;i++)
	{
		lz+=ligneZero(B,i);
	}
	return B->nrows-lz;
}


int decomposition(Matrix A,Matrix* L, Matrix* U,Matrix* P)
{
	*L=newMatrix(A->nrows,A->ncols);
	*U=newMatrix(A->nrows,A->ncols);
	maillon m=newChaine();
	int permut;
	*U=triangulaire(A,NULL,&m,&permut,0);
	if(*U==NULL)
	{
		return 0;
	}
	if(permut)
	{
		*P=identite(A->ncols);
	}
	else
	{
		*P=NULL;
	}
	Matrix id=identite(A->nrows);
	if(id==NULL)
	{
		return 0;	//TODO
	}
	Matrix E,tmp;
	while(m!=NULL)
	{
		E=identite(A->nrows);
		if(E==NULL)
		{
			return 0;	//TODO
		}
		if(m->op==1)
		{
			setElt(E,m->i,m->j,m->coef);	//verifier valeur pour savoir si cest inversion de ligne ou comvinaison limeaire
			tmp=multiplication(id,E);
			if(tmp==NULL)
			{
				return 0;	//TODO
			}
			dropMatrix(E);
			dropMatrix(id);
			id=tmp;

		}
		else
		{
			if(echangeLigne(*P,m->i,m->j)==0)
			{
				return 0;	//TODO
			}
		}
		maillon suiv=m->suiv;
		free(m);
		m=suiv;
	}

	*L=invert(id);
	if(*L==NULL)
	{
		return 0;	//TODO
	}
	dropMatrix(id);
	return 1;
}


Matrix noyau(Matrix m)
{
	Matrix A=echelonnage(m);
	if(A==NULL)
	{
		return NULL;	//TODO
	}
	int i,j,lz=0;
	for(i=0;i<A->nrows;i++)
	{
		lz+=ligneZero(A,i);
	}
	if(lz==0)
	{
		return NULL;
	}
	Matrix base=newMatrix(A->nrows,lz);
	int pos_un;
	int lnz=A->nrows-lz;
	Matrix res,mat_int,sol_int;
	for(i=0;i<lz;i++)
	{
		pos_un=base->nrows-i-1;
		setElt(base,pos_un,i,1);

		res=newMatrix(lnz,1);
		for(j=0;j<lz;j++)
		{
			setElt(res,j,0,-getElt(A,j,pos_un));
		}
		mat_int=sliceMatrix(A,0,lnz-1,0,lnz-1);
		if(mat_int==NULL)
		{
			return NULL;	//TODO
		}
		sol_int=solve(mat_int,res);
		if(sol_int==NULL)
		{
			return NULL;	//TODO
		}
		for(j=0;j<lnz;j++)
		{
			setElt(base,j,i,getElt(sol_int,j,0));
		}
		dropMatrix(res);
		dropMatrix(mat_int);
		dropMatrix(sol_int);
	}
	return base;
}
