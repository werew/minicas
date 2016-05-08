#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "operation.h"
#include "matrix.h"
#include "error.h"

#define EPSILON 0.00001

/**
 Créer une nouvelle liste chainée qui va servir à empiler les combinaisons
 linéaires lors de la triangularisation
*/
maillon newChaine()
{return NULL;}


/**
 Empile la combinaison linéaire dans la liste chainée (ajout en tête)
*/
maillon push(maillon m,unsigned int i,unsigned int j,float x,int op)
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


/**
 Renvoie une nouvelle matrice qui est la somme des deux matrices passé en argument
*/
Matrix addition(Matrix a, Matrix b)
{
	/* Si la dimension des deux matrices n'est pas la même*/
	if(a->ncols!=b->ncols||a->nrows!=b->nrows)
	{
		set_err(EMXDIM,"Dimension error");
		return NULL;	//Erreur dimension
	}
	else
	{
		Matrix c=newMatrix(a->nrows,a->ncols);
		if(c==NULL)
		{
			return NULL;	//TODO
		}
		unsigned int i,j;
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

/*
 Renvoie une nouvelle matrice qui est la différence des deux matrices passé en argument
*/
Matrix soustraction(Matrix a, Matrix b)
{
	/* Si la dimension des deux matrices n'est pas la même*/
	if(a->ncols!=b->ncols||a->nrows!=b->nrows)
	{
		set_err(EMXDIM,"Dimension error");
		return NULL;	//Erreur dimension
	}
	else
	{
		Matrix c=newMatrix(a->nrows,a->ncols);
		if(NULL==c)
		{
			return NULL;	//TODO
		}
		unsigned int i,j;
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


/*
 Renvoie une nouvelle matrice qui est le produit des deux matrices passé en argument (dans l'ordre A*B)
*/
Matrix multiplication(Matrix a,Matrix b)
{
	/* Si le nombre de colonne de la matrice A n'est pas égale au nombre de ligne de la matrice B
	 on ne peux pas faire la multiplication*/
	if(a->ncols!=b->nrows)
	{
		set_err(EMXDIM,"Dimension error");
		return NULL;	//Erreur dimension
	}
	Matrix c=newMatrix(a->nrows,b->ncols);
	if(NULL==c)
	{
		return NULL;	//TODO
	}
	unsigned int i,j,k;
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

/*
 Renvoie une nouvelle matrice qui est la transposé de la matrice passé en argument
*/
Matrix transpose(Matrix m)
{
	Matrix P=newMatrix(m->ncols,m->nrows);
	if(NULL==P)
	{
		return NULL;
	}
	unsigned int i,j;
	for(i=0;i<m->nrows;i++)
	{
		for(j=0;j<m->ncols;j++)
		{
			setElt(P,j,i,(getElt(m,i,j)));
		}
	}
	return P;
}


/*
 Renvoie une nouvelle matrice qui est la multiplication par un scalaire d'une matrice
*/
Matrix mult_Scal(float a,Matrix m)
{
	Matrix P=newMatrix(m->nrows,m->ncols);
	if(NULL==P)
	{
		return NULL;	//TODO
	}
	unsigned int i,j;
	for(i=0;i<P->nrows;i++)
	{
		for(j=0;j<P->ncols;j++)
		{
			setElt(P,i,j,(getElt(m,i,j)*a));
		}
	}
	return P;
}


/*
 Renvoie une nouvelle matrice qui est la fusion de A et B. La matrice B est placé à "droite" de la matrice A.
*/
Matrix fusionMat(Matrix A,Matrix B)
{
	/* Si le nombre de ligne de A et B n'est pas égale, on ne peux pas fusionner ces deux matrices*/
	if(A->nrows!=B->nrows)
	{
		set_err(EMXDIM,"Dimension error");
		return NULL;	//ereur dimension
	}
	Matrix C=newMatrix(A->nrows,A->ncols+B->ncols);
	if(NULL==C)
	{
		return NULL;	//TODO
	}
	unsigned int i;
	for(i=0;i<A->nrows;i++)
	{
		memcpy(getAddr(C,i,0),getAddr(A,i,0),A->ncols*sizeof(float));
		memcpy(getAddr(C,i,A->ncols),getAddr(B,i,0),B->ncols*sizeof(float));
	}
	return C;
}

/*
 Trouve le pivot le plus grand pour une colonne donnée.
 On renvoie le pivot par effet de bord grace au pointeur passé en argument (p)
 Code retour:
 0 : Indice de ligne supérieur au nombre de ligne de la matrice
 1 : Pivot trouvé
 2 : Si il n'a pas trouvé de pivot (tout la colonne est remplie de 0
*/
int PivotPartiel(Matrix m,unsigned int i,unsigned int *p)
{
	/* Si l'indice est plus grand que le nombre de ligne*/
	if(i>m->nrows)
	{
		return 0;
	}
	*p=i;
	float v=fabs(getElt(m,i,i));
	unsigned int j;
	float tmp;
	for(j=i+1;j<m->nrows;j++)
	{
		if((tmp=fabs(getElt(m,j,i)))>v)
		{
			*p=j;
			v=tmp;
		}
	}
	return v==0?2:1;
}


/*
 Trouve le premier pivot (donc non nul) pour une colonne donnée.
 On renvoie le pivot par effet de bord grace au pointeur passé en argument (p)
 Code retour:
 0 : Indice de ligne supérieur au nombre de ligne de la matrice
 1 : Pivot trouvé
 2 : Si il n'a pas trouvé de pivot (tout la colonne est remplie de 0
*/
int Pivot(Matrix m,unsigned int i,unsigned int* p)
{
	if(i>m->nrows)
	{
		return 0;
	}
	unsigned int j;
	for(j=i;j<m->nrows;j++)
	{
		if(getElt(m,j,i)!=0)
		{
			*p=j;
			return 1;
		}
	}
	return 2;
}


/*
 Fait une combinaison linéaire entre la ligne i et j avec un coefficient c. (i=i+c*j)
 La ligne est modifié directement dans la matrice, on ne fait pas de copie. La matrice est donc modifié par effet de bord.
 La fonction renvoie 0 si les indices passé en arguement (i et j) sont plus grands que les dimensions de la matrice.
*/
int addmultiple(Matrix A,unsigned int i,unsigned int j,float c)
{
	if(i>A->nrows || j>A->nrows)
	{
		return 0;	//Erreur accès ligne 
	}
	unsigned int k;
	float coef;
	for(k=0;k<A->ncols;k++)
	{
		coef=getElt(A,i,k)+c*getElt(A,j,k);
		if(coef<EPSILON && coef>(-EPSILON))
		{
			//Si le coef est très proche de zéro, ce qui est dut aux imprécisions, on met 0 dans la matrice
			setElt(A,i,k,0);
		}
		else
		{
			setElt(A,i,k,coef);
		}
	}
	return 1;
}


/*
 Echange les i et j dans la matrice. Modifie la matrice directement par effet de bord.
 La fonction renvoie 0 en cas d'erreur d'allocation ou si les indices i et j sont supérieurs au nombre de ligne de la matrice
*/
int echangeLigne(Matrix m,unsigned int i,unsigned int j)
{
	if(i>m->nrows || j>m->nrows)
	{
		return 0;	//Erreur ligne
	}
	Matrix tmp=newMatrix(1,m->ncols);
	if(tmp==NULL)
	{
		return 0;	//TODO
	}
	memcpy(getAddr(tmp,0,0),getAddr(m,i,0),m->ncols*sizeof(float));		//Place la ligne i dans une matrice temporaire
	memcpy(getAddr(m,i,0),getAddr(m,j,0),m->ncols*sizeof(float));		//Place la ligne j dans i
	memcpy(getAddr(m,j,0),getAddr(tmp,0,0),m->ncols*sizeof(float));		//Place la ligne temporaire dans la ligne j

	dropMatrix(tmp);

	return 1;
}


/*
 Renvoie une nouvelle matrice qui est la matrice passé en paramètre mais échelonné.
 La fonction renvoie la matrice si cela c'est bein déroulé et NULL si l'allocation la division de la ligne a echoué
*/
Matrix echelonnage(Matrix m)
{
	Matrix P=triangulaire(m,NULL,NULL,NULL,PivotPartiel);	//Triangularise d'abord la matrice
	if(P==NULL)
	{
		return NULL;
	}
	unsigned int i;

	for(i=0;i<P->nrows;i++)
	{
		if(getElt(P,i,i)!=0)
		{
			if(diviseLigne(P,i,getElt(P,i,i))==0)	//Divise la ligne pour avoir un 1 en premier élément non nul sur la ligne
			{
				dropMatrix(P);	//Suppression de la matrice en cas d'erreur
				return NULL; //erreur
			}
			if(getElt(P,i,i)!=0)	//Si la division ne donne pas exactement 1 à cause des imprécision, on met 1 "manuellement"
			{
				setElt(P,i,i,1);
			}
		}
	}
	return P;
}


/*
 Renvoie une nouvelle matrice qui est la matrice passé en argument mais triangularisé
 Param:
 m : matrice a echelonner
 c : pointeur pour stocker le nombre d'échange de ligne pour calculer le determinant
 ch : Liste chainée des combinaison linéaire effectué pour triangulariser la matrice
 La fonction renvoie la matrice en cas de réussite et NULL en cas d'échec
*/
Matrix triangulaire(Matrix m,float* c,maillon* ch,int* permut,int (*fct_pivot)(Matrix,unsigned int,unsigned int*))
{
	Matrix P=copyMatrix(m);
	if(P==NULL)
	{
		return NULL;	//TODO
	}
	unsigned int i,j;
	int piv_trouv;
	if(permut!=NULL)
	{
		*permut=0;
	}
	for(i=0;i<P->nrows-1;i++)
	{
		piv_trouv=fct_pivot(P,i,&j);
		
		if(piv_trouv==2)
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
				dropMatrix(P);
				return NULL;
			}
			if(ch!=NULL)
			{
				if((*ch=push(*ch,i,j,0,0))==NULL)
				{
					dropMatrix(P);
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
				dropMatrix(P);
				return NULL;
			}
			if(ch!=NULL)
			{
				if((*ch=push(*ch,j,i,coef,1))==NULL)
				{
					dropMatrix(P);
					return NULL;	//TODO
				}
			}
		}
	}
	return P;
}

int determinant (Matrix m,float* c)
{
	*c=1;
	Matrix P=triangulaire(m,c,NULL,NULL,PivotPartiel);
	if(P==NULL)
	{
		return 0;//erreur
	}

	unsigned int i;
	for(i=0;i<P->ncols;i++)
	{
		*c=*c*getElt(P,i,i);
	}
	dropMatrix(P);
	return 1;
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

int solve(Matrix A, Matrix B,Matrix* F)
{
	if(A->nrows!=B->nrows || B->ncols!=1)
	{
		set_err(EMXDIM,"Dimension error");
		return 0; //erreur dimension
	}
	float det;
	if(determinant(A,&det)==0)
	{
		return 0;
	}
	if(det==0)
	{
		*F=NULL;
		return 1;	//erreur matrix
	}
	Matrix C=fusionMat(A,B);
	if(C==NULL)
	{
		return 0;	//TODO erreur
	}
	Matrix D=echelonnage(C);
	if(D==NULL)
	{
		
		dropMatrix(C);
		return 0; //TODO Erreur
	}
	Matrix E=bienEchelonner(D);
	if(E==NULL)
	{
		dropMatrix(C);
		dropMatrix(D);
		return 0;	//TODO erreur
	}
	*F=sliceMatrix(E,0,A->nrows-1,A->ncols,A->ncols);
	if(F==0)
	{
		dropMatrix(C);
		dropMatrix(D);
		dropMatrix(E);
		return 0; //TODO erreur
	}

	dropMatrix(C);
	dropMatrix(D);
	dropMatrix(E);

	return 1;
}

Matrix expo(Matrix m,unsigned int p)
{
	Matrix A=copyMatrix(m);
	if(A==NULL)
	{
		return NULL;	//TODO
	}
	Matrix tmp=A;
	unsigned int i;
	for(i=1;i<p;i++)
	{
		tmp=A;
		if((A=multiplication(A,m))==NULL)
		{
			dropMatrix(tmp);
			return NULL;	//TODO erreur
		}
		dropMatrix(tmp);
	}
	return A;
}

int diviseLigne(Matrix A,unsigned int i,float c)
{
	if(c==0)
	{
		return 0;	//TODO erreur
	}
	unsigned int j;
	for(j=0;j<A->ncols;j++)
	{
		setElt(A,i,j,getElt(A,i,j)/c);
	}
	return 1;
}

Matrix bienEchelonner(Matrix A)
{
	Matrix B=copyMatrix(A);
	if(B==NULL)
	{
		return NULL;	//TODO
	}

	unsigned int i,j;
	for(i=A->nrows-1;i>0;i--)
	{
		for(j=0;j<i;j++)
		{
			if(addmultiple(B,j,i,-getElt(B,j,i))==0)
			{
				dropMatrix(B);
				return NULL; //erreur
			}
		}
	}
	return B;
}

Matrix sliceMatrix(Matrix A,unsigned int i1,unsigned int i2,unsigned int j1,unsigned int j2)
{
	if(i1>i2 || j1>j2)
	{
		return NULL;	//TODO erreur indice
	}
	if(i1>A->nrows || i2>A->nrows || j1>A->ncols || j2>A->ncols)
	{
		return NULL;	//TODO erreur indice plus grand que dimension matrice
	}
	Matrix B=newMatrix(i2-i1+1,j2-j1+1);
	if(NULL==B)
	{
		return NULL;	//TODO
	}

	unsigned int k;
	for(k=i1;k<=i2;k++)
	{
		memcpy(getAddr(B,k-i1,0),getAddr(A,k,j1),(j2-j1+1)*sizeof(float));
	}
	return B;
}

int invert(Matrix m,Matrix* D)
{
	float det;
	if(determinant(m,&det)==0)
	{
		return 0;
	}
	if(det==0)
	{
		*D=NULL;
		return 1;
	}
	else
	{
		Matrix Id=identite(m->ncols);
		if(Id==NULL)
		{
			return 0;	//TODO erreur
		}
		Matrix A=fusionMat(m,Id);
		if(A==NULL)
		{
			dropMatrix(Id);
			return 0;	//TODO
		}
		Matrix B=echelonnage(A);
		if(B==NULL)
		{
			dropMatrix(Id);
			dropMatrix(A);
			return 0;	//TODO
		}
		Matrix C=bienEchelonner(B);
		if(C==NULL)
		{
			dropMatrix(Id);
			dropMatrix(A);
			dropMatrix(B);
			return 0;	//TODO
		}
		*D=sliceMatrix(C,0,C->nrows-1,m->ncols,C->ncols-1);
		if(D==NULL)
		{
			dropMatrix(Id);
			dropMatrix(A);
			dropMatrix(B);
			dropMatrix(C);
			return 0;	//TODO
		}

		dropMatrix(Id);
		dropMatrix(A);
		dropMatrix(B);
		dropMatrix(C);

		return 1;
	}
}

int ligneZero(Matrix A,unsigned int l)
{
	unsigned int i;
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
	unsigned int lz=0;
	unsigned int i;
	for(i=0;i<B->nrows;i++)
	{
		lz+=ligneZero(B,i);
	}
	int ligne=B->nrows;
	dropMatrix(B);
	return ligne-lz;
}


int decomposition(Matrix A,Matrix* L, Matrix* U,Matrix* P)
{
	maillon m=newChaine();
	int permut;
	*U=triangulaire(A,NULL,&m,&permut,Pivot);
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
		dropMatrix(*U);
		return 0;	//TODO
	}
	Matrix E,tmp;
	while(m!=NULL)
	{
		E=identite(A->nrows);
		if(E==NULL)
		{
			dropMatrix(*U);
			dropMatrix(id);
			return 0;	//TODO
		}
		if(m->op==1)
		{
			setElt(E,m->i,m->j,m->coef);	//verifier valeur pour savoir si cest inversion de ligne ou comvinaison limeaire
			tmp=multiplication(id,E);
			if(tmp==NULL)
			{
				dropMatrix(*U);
				dropMatrix(id);
				dropMatrix(E);
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
				dropMatrix(*U);
				dropMatrix(id);
				dropMatrix(E);
				return 0;	//TODO
			}
			dropMatrix(E);
		}
		maillon suiv=m->suiv;
		free(m);
		m=suiv;
	}

	invert(id,L);
	if(*L==NULL)
	{
		return 0;	//TODO
	}
	dropMatrix(id);
	return 1;
}


int noyau(Matrix m,Matrix* base)
{
	Matrix A=echelonnage(m);
	if(A==NULL)
	{
		return 0;	//TODO
	}
	unsigned int i,j,lz=0;
	for(i=0;i<A->nrows;i++)
	{
		lz+=ligneZero(A,i);
	}
	if(lz==0)
	{
		*base=newMatrix(A->nrows,1);
		dropMatrix(A);
		if(*base==NULL)
		{
			return 0;
		}
		return 1;
	}
	*base=newMatrix(A->nrows,lz);
	if(base==NULL)
	{
		dropMatrix(A);
		return 0;	//TODO
	}
	unsigned int pos_un;
	unsigned int lnz=A->nrows-lz;
	Matrix res,mat_int,sol_int;
	for(i=0;i<lz;i++)
	{
		pos_un=(*base)->nrows-i-1;
		setElt(*base,pos_un,i,1);

		res=newMatrix(lnz,1);
		if(res==NULL)
		{
			dropMatrix(A);
			dropMatrix(*base);
			return 0;	//TODO
		}
		for(j=0;j<lz;j++)
		{
			setElt(res,j,0,-getElt(A,j,pos_un));
		}
		mat_int=sliceMatrix(A,0,lnz-1,0,lnz-1);
		if(mat_int==NULL)
		{
			dropMatrix(A);
			dropMatrix(*base);
			dropMatrix(res);
			return 0;	//TODO
		}
		solve(mat_int,res,&sol_int);
		if(sol_int==NULL)
		{
			dropMatrix(A);
			dropMatrix(*base);
			dropMatrix(res);
			dropMatrix(mat_int);
			return 0;	//TODO
		}
		for(j=0;j<lnz;j++)
		{
			setElt(*base,j,i,getElt(sol_int,j,0));
		}
		dropMatrix(res);
		dropMatrix(mat_int);
		dropMatrix(sol_int);
	}
	dropMatrix(A);
	return 1;
}
