/*----------------------potencia.c-------------------------*/
/* Calcula injecoes de potencias nas barras da rede        */
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

/*------------------Inicio da funcao---------------*/

void potencia(void){
	int k,i,l;
	double co,si,tkl,vkl,vkk,g,b;
	
    for(k = 0; k < nb; k++){      // loop 1 --> propria (kk)
		if(itipo[k] == 2){
           v[k] = ves[k];
           }
		vkk  = v[k]*v[k];
		p[k] = gdiag[k]*vkk;
		q[k] =-bdiag[k]*vkk;
        }
	for(i = 0; i < nl; i++){     // loop 2 --> transferencia (kl)
		k = ni[i];
		l = nf[i];
		vkl = v[k]*v[l];
		g = gkl[i];
		b = bkl[i];
		tkl = tet[k] - tet[l];
		si = sin(tkl);
		co = cos(tkl);
		p[k] = p[k] + vkl*(-g*co - b*si); 
		p[l] = p[l] + vkl*(-g*co + b*si);
		q[k] = q[k] + vkl*(-g*si + b*co);
		q[l] = q[l] + vkl*(g*si + b*co);
	    }
	return;
}

/*----------------------- Fim da funcao potencia------------- */

//Data de atualizacao: 09/12/2006
