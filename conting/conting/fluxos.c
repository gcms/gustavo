/*------------------------------fluxos.c---------------------------------------------*/

#include <stdlib.h>
#include <math.h>
#include "global.h"

/*-------------------Inicio da funcao fluxos-------------------------------*/
/* Funcao para calcular fluxos de potencia ativa e reativo nos ramos do sistema*/
/*-------------------------- MODELO AC DA REDE ELETRICA--------------------------*/

void fluxos(void){
	int k,m,j;
	double tkm,vkk,vkm,vmm,bb,co,si,t1;
	
	for (j=0; j<nl; j++){
		k = ni[j];
		m = nf[j];
		tkm = t[k]-t[m];
		co = cos(tkm);
		si = sin(tkm);
		vkk = v[k]*v[k];
		vmm = v[m]*v[m];
		vkm = v[k]*v[m];
		if( tap[j] == 0.e0 )
			t1 = 1.e0;
		else
			t1 = 1.e0/tap[j];
		
		bb = bkl[j]+ylin[j];
		fpkm[j] = t1*gkl[j] *vkk - vkm*(gkl[j]*co+bkl[j]*si);    // fluxo ativo
		rqkm[j] =  -t1* bb  *vkk - vkm*(gkl[j]*si-bkl[j]*co);    // fluxo reativo
		fpmk[j] =(gkl[j]/t1)*vmm - vkm*(gkl[j]*co-bkl[j]*si);
		rqmk[j] = -( bb /t1 )*vmm - vkm*(-gkl[j]*si-bkl[j]*co);
		fpjk[j] = fpkm[j];
	}
	return;
}  
/*----------------------Fim da funcao fluxos-----------------------------*/

/* Funcao para calcular fluxos de potencia ativa e reativa dos ramos do sistema   
        [ Somente prar Screening de Contingencia ]     */

void fluxpq(void){
	int k,m,jj;
	double tkm,vkk,vkm,vmm,bb,co,si,t1;
	for (jj=0; jj<nl; jj++){
		k = ni[jj];
		m = nf[jj];
		tkm = t[k]-t[m];
		co = cos(tkm);
		si = sin(tkm);
		vkk = v[k]*v[k];
		vmm = v[m]*v[m];
		vkm = v[k]*v[m];
		bb = bkl[jj]+ylin[jj];
		if( tap[jj] == 0.e0 )
			t1 = 1.e0;
		else
			t1 = 1.e0/tap[jj];
		fpjk[jj] = t1*gkl[jj]*vkk - vkm*(gkl[jj]*co + bkl[jj]*si); // fluxo ativo
		fqjk[jj] = -t1*bb*vkk - vkm *(gkl[jj]*si - bkl[jj]*co); // fluxo reativo
	}
	return;
}
/*------------Fim da funcao fluxpq------------------*/
