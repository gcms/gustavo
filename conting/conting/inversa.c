/*------------------------------------inversa.c---------------------------------*/
/*              
                     [Arquivo com uma rotina para inverter matriz]              

--------------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include "global.h"


/*-----------------------Inicio da Funcao inversa------------------------------*/

void inversa(double aa[mmax][mmax],int nordem){
	float cte;
	int i,j,k;
	/*-------------Inicializando diagnostico----------------------*/
	kdiag = 0;                     // Contingencia normal
	for(i=0; i<nordem; i++){
		cte = aa[i][i];
		aa[i][i] = 1.e0;
		if(fabs(cte) < 1.e-6){   // ILHAMENTO
			kdiag = -1; // Matriz singular significa ILHAMENTO
			// printf(" \n Matriz Singular ==> ILHAMENTO !!! \n ");
			break;
		}
		for(j=0; j<nordem; j++)
			aa[i][j] = aa[i][j]/cte;
		for(k=0; k<nordem; k++){
			if(k != i){
				cte = aa[k][i];
				aa[k][i] = 0.e0;
				for(j=0; j<nordem; j++){
					if( fabs(aa[i][j]) != 0.e0 )
						aa[k][j] = aa[k][j] - cte*aa[i][j];
				}//fim for
			}//fim if
		}//fim for	
	}//fim for
	return;
}

/*-------------------------Fim da Funcao inversa------------------------------*/
