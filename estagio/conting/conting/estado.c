
/*----------------------estado.c-------------------------
Este arquivo contem as seguintes funcoes:
     1) estado -> Funcao que processa as iteracoes do calculo do Fluxo de Carga AC
     2) potencia -> Funcao que calcula as injecoes ativas e reativas nas barras
OBS:Define a fun�o flow mas nao a usa neste arquivo 
FAZ CHAMADAS RELATIVAS AS ROTINAS DE ZOLLENKOPF
FONTE DESTE ALGORITMO: Fluxograma da pag. 95 do Livro do A. Monticelli
-------------------------------------------------------------*/
/*----------------- Arquivos e bibliotecas incluidos----------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "prototipos.h"
/*----------------------Inicio da funcao estado-------------------*/
void estado(double ce[],double cle[]){
	double dx1[nbmax],dx2[nbmax],dpmax,dqmax,aux;
	int k,ntot,lim,kp,kq,inic,i;
	/*Cadeia de Zolenkopf com elementos nao-nulos das matrizes B' e B''    */
	/*-----------Inicializando contadores e chaves----------*/
	nitea = 0;        // contador de iteracoes ativas
	niter = 0;        // contador de iteracoes reativas
	kp    = 1;        // chaves
	kq    = 1;
	lim   = 60;
	/*------------------- Inicio do processo iterativo-------------------- */
	inic = 1;
	while(niter <= lim && nitea <= lim && (kp!= 0 || kq != 0)){
		if(inic == 1){
	/*---------- CALCULO AS POTENCIAS ATIVAS-----------  */
	/*-------Calculo das injecoes de potencia nas barras:--- */
		potencia();
		inic = 0;
	/*-----	Calculo do MISMATCH ATIVO e do seu valor maximo:----*/
		dpmax = 0.e0;
		for(k=0; k<nb; k++){
			dx1[k] = pes[k] - p[k];
			if( itipo[k] != 2 ){   //se nao for barra slack
				if (fabs(dx1[k]) > dpmax)//maior valor absoluto do vetor de mismatch ativo
					dpmax = fabs(dx1[k]);
			}
			dx1[k] = dx1[k]/v[k];
		}
	/*--------Teste de convergencia (fase ATIVA):---------------*/
		if( dpmax > 1.e+20 ){
			exit(1);
		}
		else
			if( dpmax <= tol )
				kp = 0;
			else{
		/*---------------------------------------------------------------
		Solucao do sistema linear P-TETA para MISMATCH ATIVO:
		Chamada da rotina solu() - Solucao do Sist. Linear B'.dtet = dx1
		----------------------------------------------------------------*/
				solu(ce,dx1);
	    	/*----------------------------------------
		Atualiza o Vetor de Angulos Nodais TETA:
		----------------------------------------*/
				for( k=0; k<nb; k++){
					tet[k] = tet[k] + dx1[k];
					t[k] = tet[k];
				}
		/*------------------------------------------
		Incrementa o Contador de Iteracoes Ativas 
		----------------------------------------- */
		nitea++;
		/*-------------Retorna a chave REATIVA ao valor inicial-----------*/
		kq = 1;
		}  //  fim do else
	}
	else  // else de inic=1
		if(kq == 1){
		/*------------------------------------------------------
		OBS: dx1[]-> Armazena os incrementos dos angulos nodais
			dx2[]-> Armazena os incrementos das tensoes nodais
		--------------------------------------------------------
		--------------CALCULO DAS POTENCIAS REATIVAS------------
		--------------------------------------------
		Calculo das injecoes de potencia nas barras:
		---------------------------------------------*/
			potencia();
			inic = 1;
		/*---Calculo do MISMATCH REATIVO e do seu valor maximo:-----------*/
			dqmax = 0.e0;
			for( k=0; k<nb; k++){
				dx2[k] = qes[k] - q[k];
				if( itipo[k] <= 0 ){
					if (fabs(dx2[k]) > dqmax)
						dqmax = fabs(dx2[k]);   
				//maior valorabsoluto do vetor de mismatch reativo
					dx2[k] = dx2[k]/v[k];
				}
			}     // fim do for...
			/*---------Teste de convergencia (fase REATIVA):----------*/
			if( dqmax > 1.e+10 ){
				// printf("\n DIVERGENCIA ...\n");
				exit(1);
			}
			else{
				if( dqmax <= tol )
					kq = 0;
				else{
		/*----------Solucao do sistema linear Q-V para MISMATCH REATIVO:
		----------------------------------------------------------------
		Chamada da rotina solu() - Solucao do Sist. Linear B".dv = dx2 
		----------------------------------------------------------------*/
					solu(cle,dx2);
		/*-----------Atualiza o Vetor de Magnitudes Nodais V:-------------*/
					for( k=0; k<nb; k++)
						if( itipo[k] == 0 )
							v[k] = v[k] + dx2[k];
	/*----------Incrementa o Contador de Iteracoes Reativas  ---------*/
					kq = 1;
					niter++;
	/*----------Retorna a chave ATIVA ao valor inicial----------*/
					kp = 1;
				}    //  else   ( dqmax<tol  )
			}      //  else   ( dqmax> 1e10)
		}        //  else   ( se kq=1    )
	}            //  while
	if ((niter > lim) || (nitea >lim)){
/*------Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes-------------*/
		ntot = nitea + niter;
		nconv = 1;
		printf("\n O PROCESSO NAO CONVERGE COM %4d ITERACOES  !!\n", ntot);
		exit(1);
	}
	
	/*---------------- Fim do Processo Iterativo com Sucesso-------------*/
	
	
	/*----------Armazena as injecoes/geracoes do ponto inicial--------------*/
	for( k=0; k<nb; k++)
		if( itipo[k] == 2 )
			pg0[k]= p[k] + pca[k];
	else
		pg0[k]= pes[k] + pca[k];
	
	return;
}

/*-------------- Fim da funcao estado---------------*/

/*------------------Inicio da funcao---------------*/

/*  Funcao para calcular as injecoes ativa e reativa nas barras */

void potencia(void){
	int k,i,l;
	double co,si,tkl,vkl,vkk;
	for( k=0; k<nb; k++){            // loop 1 --> propria (kk)
		if( itipo[k] > 0 )
			v[k] = ves[k];
		vkk  = v[k]*v[k];
		p[k] = gdiag[k]*vkk;
		q[k] =-bdiag[k]*vkk;
	}
	for( i=0; i<nl; i++) {          // loop 2 --> transferencia (kl)
		k = ni[i];
		l = nf[i];
		vkl = v[k]*v[l];
		tkl = tet[k] - tet[l];
		si = sin(tkl);
		co = cos(tkl);
		p[k] = p[k] + vkl*(-gkl[i]*co - bkl[i]*si); 
		p[l] = p[l] + vkl*(-gkl[i]*co + bkl[i]*si);
		q[k] = q[k] + vkl*(-gkl[i]*si + bkl[i]*co);
		q[l] = q[l] + vkl*(gkl[i]*si + bkl[i]*co);
	}
	return;
}

/*----------------------- Fim da funcao potencia------------- */


//Data de atualizacao:02/06/03
