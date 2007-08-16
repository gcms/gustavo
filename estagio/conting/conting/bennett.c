/*----------------------------bennett.c------------------------------------*/

/* Funcao para atualizar os fatores L,D U de uma matriz dada        */

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "global.h"

/*----------------------Inicio da funcao bennett--------------------------*/

void bennett(double xce[],double ekm[],double b[1]){
	int k,i,il,l;
	double dd,xll,c;
	for(k=0; k<ifbt; k++){
	/*----Loop sobre o Caminho de Fatoracao-----*/ 
		i = nfastfor[k];  // Caminho de fatoracao --> Define a coluna
		il = lcol[i];	
	/*-----------Atualiza elementos da diagonal-------------*/
		dd = 1.e0/xce[i];
		dd = dd + b[0]*ekm[i]*ekm[i];
	/*--------Para Detectar ILHAMENTO---------------*/
		if(fabs(dd) < 1.e-6)
		kdiag = -1;
	
		c = b[0]*ekm[i];	
		xce[i] = 1.e0/dd;   // Elemento de D inv.	
		while(il > 0){
			l = itag[il];
			if(l != i){
			// Loop sobre os elementos off-diagonal
				xll = -xce[il];
				ekm[l] = ekm[l] - ekm[i]*xll;
				xll = xll + c*ekm[l]/dd;
				xce[il] = -xll;
			}
			il = lnxt[il];
		}  //fim while
	b[0] = b[0] - c*c/dd;
	}    //fim for	
	/*------------	Limpa as posicoes do vetor ekm[] ------------*/
	for(k=0; k<nb; k++)
		ekm[k] = 0.e0;
	
	return;
}
/*-------------------------Fim da funcao bennett--------------------------*/
