/*------------------------------------svector.c----------------------------------------

 Este arquivo contem rotinas relativas ao metodo dos vetores esparsos

 -------------------------------------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include "global.h"

/*----------------------Inicio da funcao grafo----------------------------- */

/*-------------------------------------------------------
 Construir Tabela de Caminhos de Fatoracao: lista next[i]
 ------------------------------------------------------*/

void grafo (void){
	int il,min,m,j,ki,k,i,mnxt;
	int ordem[nbmax];
  /*--------------------------------------------------------
    construir vetor com a ordem de fatoracao da coluna "ki" 
  ---------------------------------------------------------*/
	for(i=0; i<nb; i++){
		ki=nseq[i];
		ordem[ki]=i;
	}
  /*--------------------------------------------------------------
    Construir a lista next(i) ==> Tabela de caminhos de fatoracao 
    ------------------------------------------------------------*/
	for (k=0; k<nb; k++){
		il = lcol[k];
		min = nb;
		while (il >= 0){
			m = itag[il];
			if(m != k){
				j = ordem[m]; // ordem em que a coluna foi fatorada
				if(j < min){
					min = j;
					mnxt = m;
				}
			}
			il = lnxt[il];
		}      //fim do while
		if(min != nb)
			next[k] = mnxt;
		else
			next[k] = -1;     // existe apenas um elemento (a diagonal)
	}        //fim do k = 1,nb
	return;
}// fim da rotina
/*---------------------------Fim da funcao grafo----------------------------- */
/*--------------------------Inicio da funcao caminho-------------------------*/
/*-------------- Subrotina que faz a uniao dos caminhos----------------------*/
void caminho(int j1,int k1){
	int i,j,k,ip,ifb,aux;
	int ntex[nbmax],iaux[nbmax],nv[2];
	for (i=0; i<nbmax; i++){ 
		ntex[i] = 0;
		iaux[i] = 0;
	}
	nv[0] = j1;
	nv[1] = k1;
	ip = 0;
	for(i=0; i<2; i++) // 2 caminhos parciais
	{  
		k = nv[i];
		ip++;
		ntex[k] = ip;
		k = next[k];
		
		while (k != -1){
			ip++;
			//printf("\n Estou em svector rotina caminho ip = %d",ip);
			ntex[k] = ip;
			k = next[k];
		} // Fim while
	}      // fim for
	ifb = 0;
	for (i=0; i<nb; i++){
		
		if (ntex[i] != 0){
			//printf("\n Estou em svector rotina caminho ifb = %d",ifb);
			ifb++;
			iaux[ifb-1] = ntex[i];// alterado por causa do indice em 'c'
		} // Fim if
	}   // Fim for
	ifbt = ifb;            // Total de elementos de iaux[]
	bubble(iaux,ifb);
	//printf("\n Estou em svector rotina caminho j = %d",ifbt);
	for (i=0; i<ifbt; i++) {
		//printf("\n Estou em svector rotina caminho j = %d",j);
		for (j=0; j<nb; j++){
			if(iaux[i] == ntex[j])
				break;
		}
		nfastfor[i] = j;  // Caminho resultante da uniao
	}   // Fim for
	return;
}        // Fim da funcao
/*----------------------------Fim da funcao caminho---------------------------*/
/*--------------------------Inicio da funcao bubble---------------------------*/
/*------------  Subrotina para ordenar um vetor em ordem crescente-----------*/
int bubble(int ix[nbmax],int j){
	int ka1,ka2,ktemp;
	if(j > 1){
		for(ka1=0; ka1<j-1; ka1++){
			for(ka2=ka1+1; ka2<j; ka2++){
				if(ix[ka1] > ix[ka2]){
					ktemp = ix[ka1];
					ix[ka1] = ix[ka2];
					ix[ka2] = ktemp;
				} // Fim if
			}  // Fim for
		}    // Fim for
	}      // Fim if
	return;
}
/*----------------------------Fim da funcao bubble---------------------------*/
/*--------------------------Inicio da funcao soluve--------------------------*/
void soluve(double xce[], double u[]){
	int ki,k,l,i,nb1,j,jj;
	double cf,sum,x1,x2;
	/*------------  Fast Forward  -----------*/
	for(ki=0; ki<ifbt; ki++){
		k = nfastfor[ki];
		cf = u[k];
		u[k] = 0.e0;
		l = lcol[k];
		while(l > 0){
			i = itag[l];
			u[i] = u[i]+cf*xce[l];
			l = lnxt[l]; 
		} //fim while
	}    //fim for
	/*---------------------  BACKWARD CONVENCIONAL   -------------------*/
	nb1 = nb-1;
	for(jj=0; jj<nb1; jj++){
		j = nb-jj-2;  // mudanca feita para os indices da ling. c
		k = nseq[j];
		sum = u[k];
		l = lcol[k];
		i = itag[l];
		if((i-k) < 0 || (i-k) > 0){
			x1 = xce[l];
			x2 = u[i];
			sum = sum+x1*x2;
		}
		l = lnxt[l];
		while(l > 0){
			i = itag[l];
			if((i-k) < 0 || (i-k) > 0){
				x1 = xce[l];
				x2 = u[i];
				sum = sum+x1*x2;
			}
			l = lnxt[l];
		}
		u[k] = sum;
	}   // fim for
	/*
	for(j=0; j<nb; j++){
	k = nseq[j];
	cf = u[k];
	u[k] = 0.e0;
	l = lcol[k];
	i = itag[l];
	x1 = xce[l];
	u[i] = u[i]+cf*x1;
	l = lnxt[l];
	while(l > 0){
	i = itag[l];
	x1 = xce[l];
	u[i] = u[i]+cf*x1;
	l = lnxt[l];
	}    
	} */
	return;
}
/*--------------------------Fim da funcao soluve-----------------------------*/
/*--------------------------Inicio da funcao soluffs-------------------------*/
void soluffs(double xce[], double u[]){
	int ki,k,l,i;
	double cf;
	/*-------------  Fast Forward  -----------*/
	for(ki=0; ki<ifbt; ki++){
		k = nfastfor[ki];
		cf = u[k];
		u[k] = 0.e0;
		l = lcol[k];
		while(l > 0){
			i = itag[l];
			u[i] = u[i]+cf*xce[l];
			l = lnxt[l];
		}// Fim while
	}  // Fim for
	return;
}  // Fim da funcao
/*--------------------------Fim da funcao soluffs----------------------------*/
/*--------------------------Inicio da funcao solufs--------------------------*/
void solufs(double xce[], double u[]){
	int j,k,l,i;
	double x1,cf;
	/*---------------------   Forward Convencional   -------------------*/
	for(j=0; j<nb; j++){
		k = nseq[j];
		cf = u[k];
		u[k] = 0.e0;
		l = lcol[k];
		i = itag[l];
		x1 = xce[l];
		u[i] = u[i]+cf*x1;
		l = lnxt[l];
		while(l > 0){
			i = itag[l];
			x1 = xce[l];
			u[i] = u[i]+cf*x1;
			l = lnxt[l];
		}
	}
	return;
}
/*--------------------------Fim da funcao solufs-----------------------------*/
/*--------------------------Inicio da funcao solubs--------------------------*/
void solubs(double xce[], double u[]){
	int j,jj,k,l,i,nb1;
	double x1,x2,sum;
	/*----------------------   Backward Convencional   --------------------*/
	nb1 = nb-1;
	for(jj=0; jj<nb1; jj++){
		j = nb-jj-2;//alterei por causa do indice em 'c'
		k = nseq[j];
		sum = u[k];
		l = lcol[k];
		i=itag[l];
		if((i-k) < 0 || (i-k) > 0){
			x1 = xce[l];
			x2 = u[i];
			sum = sum+x1*x2;	
			l = lnxt[l];
		}
		else
			l = lnxt[l];
		while(l > 0){
			i = itag[l];
			if((i-k) < 0 || (i-k) > 0){
				x1 = xce[l];
				x2 = u[i];
				sum = sum+x1*x2;
				l = lnxt[l];
			} 
			else
				l = lnxt[l];
		}  // Fim while
		u[k] = sum;
	}    // Fim for
	return;
}

/*--------------------------Fim da funcao solubs-----------------------------*/

