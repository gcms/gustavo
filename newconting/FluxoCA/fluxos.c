/*------------------------------fluxos.c--------------------------------------*/
/* Calcular fluxos de potencia ativa e reativo nos ramos do sistema           */
/*----------------------- MODELO AC DA REDE ELETRICA--------------------------*/
/*------------------------------------------------------
  Bibliotecas e arquivos do tipo include do compilador
------------------------------------------------------*/
#ifdef WIN_32
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

/*---------------------------------------
  Arquivo do tipo include do aplicativo
----------------------------------------*/
#include "global.h"   

/*-------------------Inicio de fluxos()---------------------------------------*/

void fluxos(void){
	int k,m,i;
	double tkm,vk,vm,vkk,vmm,ysh,bb,co,si,t1,ta,ta3,g,b;
	
	for (i = 0; i < nl; i++){
        ta3 = tap[i];
        ta = tap0[i];
        if( ta == 0.e0 )
			t1 = 1.e0;
		else
			t1 = 1.e0/ta3;
		k = ni[i];
		m = nf[i];
		tkm = tet[k]-tet[m];
		g = gkl[i]/t1;
		b = bkl[i]/t1;
		co = cos(tkm);
		si = sin(tkm);
		vk = v[k];
		vkk = vk*vk;
		vm = v[m];
		vmm = vm*vm;
		ysh = ylin[i];
		bb = b + ysh;
		pkm[i] = t1*t1*g*vkk - t1*vk*vm*(g*co + b*si);       // fluxo ativo
		qkm[i] =  -t1*t1*bb*vkk - t1*vk*vm*(g*si - b*co);    // fluxo reativo
		pmk[i] = g*vmm - t1*vk*vm*(g*co - b*si);             // fluxo ativo
		qmk[i] =  -bb*vmm + t1*vk*vm*(g*si + b*co);          // fluxo reativo
	}
	return;
}  
/*----------------------Fim de fluxos()---------------------------------------*/

