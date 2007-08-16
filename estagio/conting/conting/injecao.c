/*----------------------------injecao.c---------------------------------*/
/*
Funcao para calculo das injecoes reativas nas barras para compensar 
reativo no FDLF 

Preparada especialmente para o metodo 1P-1Q (Iteracao Reativa)

------------------------------------------------------------------------ */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "global.h"
/*----------------------Inicio da funcao injecao-----------------------*/
void prodMdelta_teta(double px[nbmax],double w[nbmax],int kk, int mcont){
	int j,l,j1,k1,k,jj,i,ir;
	double tjk;
	double xx[nlmax];
	/*------- Injecoes de Potencia (Produto de matriz por vetor)----------*/
	for(l=0; l<nb; l++)
	w[l] = 0.e0;
	
	for(l=0; l<nl; l++){
		j1 = ni[l];
		k1 = nf[l];
		j = lrd[j1];
		k = lrd[k1];
		xx[l] = gkl[l];
		for(jj=1; jj<=mcont; jj++){
	/*------Simulando a retirada do ramo "i" da matriz M------------*/
	// Se fcont=1. Implica na retirada de todo o ramo
			ir = kcont[kk][jj];
			i = ir-1;
			if(l == i){
				xx[l] = gkl[l] - fcont*gkl[l]; 
				break;
			}
		}//fim for
		tjk = px[j] - px[k];        // Geral
		w[j] = w[j] - xx[l]*tjk;
		w[k] = w[k] + xx[l]*tjk;
		if(itipo[j] == 2)
			w[j] = 0.e0;
		
		if(itipo[k] == 2)
			w[k] = 0.e0; 
	}
	return;
}




/*------------------------Fim da funcao injecao------------------------*/
