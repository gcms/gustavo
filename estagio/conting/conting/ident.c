/*-----------------------------ident.c---------------------------------*/

/* Funcao para identificar o ramo e definir os parametros de alteracao 
      [ Versao p/ Contingencia Simp./Multip. Pos/Mid Compensation ] 

---------------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "global.h"

/*----------------------Inicio da funcao identdc--------------*/
void identdc(int j,int k){
	ej1 = 0.e0;
	ek1 = 0.e0;  
  /*-------------<< Seja linha ou transformador >>------------*/
	if(itipo[j] <= 1 && itipo[k] <= 1){ 
	//cobre toda combinacao sem "slack"
		ej1 =  1.e0;
		ek1 = -1.e0; 
	}
	else{
		if(itipo[j] == 2){
			if(itipo[k] == 0 || itipo[k] == 1)
				ek1 = -1.e0;
		}
		else{
			if(itipo[j] == 0 || itipo[j] == 1)
				ej1 = 1.e0;
		}
	}
	//wjk1[j] = ej1;
	//wjk1[k] = ek1;
	/*------------------ Para Matriz Blinha  -----------------*/
	 return;
}
/*------------------------Fim da funcao identdc-----------------------*/

/*----------------------Inicio da funcao identca----------------------*/

/* Funcao para identificar o ramo e definir os parametros de alteracao
   [ Versao para Contingencia Multipla CA <Fluxo> Mid Compensation ] 
   [ e Tambem para Metodos 1P-1Q Iterativos                        ]  */


void identca(int j,int k,int i){
	ej2 = 0.e0;
	ek2 = 0.e0; 
	xjk[i] = 1.e40; 
	if(itipo[j] == 0){
		ej2 = 1.e0;
		xjk[i] = xkl[i];
	}
	if(itipo[k] == 0){
		ek2 = -1.e0;
		xjk[i] = xkl[i]; 
	}
	if(tap[i] != 0){
		if(itipo[j] == 0)
			ej2 = 1.e0; 
	}
	//wjk2[j] = ej2;
	//wjk2[k] = ek2;
	/*-------------------- Para Matriz B2linha----------------*/
	return;
}
/*------------------------Fim da funcao identca-----------------------*/
//Data de Atualizacao: 11/06/2003
