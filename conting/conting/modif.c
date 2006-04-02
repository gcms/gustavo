/*------------------------------modif.c-----------------------------*/

/* Funcao para guardar/reestabelecer Yo do caso base  */

#include <stdio.h>
#include <stdlib.h>
#include "global.h"
/*-----------------------Inicio da funcao ricupero--------------------*/
void ricupero(int info){
	int i; 
	/*---------------------------------------------------------------------
	Guarda a matriz Yo=Go + j.Bo do caso base (para posterior utilizacao)
	LEGENDA:  info = 0   ==> Guarda  a matriz Yo = Go + j.Bo  do caso base p/ posterior utiliz.
	info = 1   ==> Reestabelece a matriz Yo = Go + j.Bo a sua forma original
	-----------------------------------------------------------------------*/
	if(info == 0){	
	/*-------------------------------------------------------
	Guarda o estado do caso base para posterior utilizacao
	------------------------------------------------------*/
		for(i=0; i<nb; i++){  // Diagonal
			gdiagbase[i] = gdiag[i];
			bdiagbase[i] = bdiag[i];     
		}
		for(i=0; i<nl; i++){       // Elementos do ramo
			gklbase[i] = gkl[i];
			bklbase[i] = bkl[i];
		//        printf("\n GUARDANDO...bklbase[%d]=%lf \n",li,bklbase[li]);
		//        printf("\n GUARDANDO...bkl[%d]=%lf \n",li,bkl[li]);
		}
		for(i=0; i<nb ;i++){
			vb[i] = v[i];
			tetab[i] = tet[i];
		}
	}  // fim if
	if(info == 1){
	/*-------------------------------------------------
	Guarda a matriz Yo=Go + j.Bo a sua forma original
	------------------------------------------------*/
		for(i=0; i<nb; i++){   // Diagonal
			gdiag[i] = gdiagbase[i];
			bdiag[i] = bdiagbase[i];
		}
		for(i=0; i<nl; i++){          // Elementos do ramo
			gkl[i] = gklbase[i];
			bkl[i] = bklbase[i];
	//printf("\n RECUPERANDO...bklbase[%d]=%lf \n",li,bklbase[li]);
	//printf("\n RECUPERANDO...bkl[%d]=%lf \n",li,bkl[li]);  
		}
	/*-Restabelece o estado do caso base para utilizacao no proximo caso-----*/
		for(i=0; i<nb ;i++){
			v[i] = vb[i];
			tet[i] = tetab[i];
		}
	}    // fim if
	return;
}
/*--------------------------Fim da funcao ricupero---------------------*/
/*-------------------------Inicio da funcao modify---------------------*/
/* Funcao para modificar a matriz Y em funcao da contingencia
OBS: fcont e o fator de contingencias de ramo [-1.<= fcont =<+1.]
    fcont e introduzido para permitir a retirada de uma fracao da
    admitancia do ramo. Se fcont-1.0 tira-se o ramo integralmente. 

OBS: ramos paralelos (transformador ou linha) sao armazenados
     individualmente nos vetores gdiag[],bdiag[] e bkl[]

OBS: ter bastante cuidado com a maneira como estao armazenados os 
     parametros do circuito PI de transformadores em fase
*/

void modify(int j,int k,int i){ 
	double t;
	//printf("fcont  = %f", fcont);
	if(tap[i] == 0.e0) {          // Linha de transmissao
		gdiag[j] = gdiag[j] - fcont * gkl[i];
		gdiag[k] = gdiag[k] - fcont * gkl[i];
		bdiag[j] = bdiag[j] - fcont * bkl[i] - fcont * ylin[i];
		bdiag[k] = bdiag[k] - fcont * bkl[i] - fcont * ylin[i];
	}
	else{
		t = 1.e0/tap[i];
		gdiag[j] = gdiag[j] - fcont * gkl[i] * t;
		gdiag[k] = gdiag[k] - fcont * gkl[i] / t;
		bdiag[j] = bdiag[j] - fcont * bkl[i] * t;
		bdiag[k] = bdiag[k] - fcont * bkl[i] / t;
	}
	gkl[i] = gkl[i] - fcont * gkl[i];
	bkl[i] = bkl[i] - fcont * bkl[i];
	return;
}

/*-----------------------------Fim da funcao modify------------------------*/
