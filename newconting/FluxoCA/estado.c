/*----------------------estado.c------------------------------------------------
FAZ CHAMADAS RELATIVAS AS ROTINAS DE ZOLLENKOPF
FONTE DESTE ALGORITMO: Fluxograma da pag. 95 do Livro do Alcir Monticelli:
Fluxo de Carga em Redes de Energia Elétrica, Editora Edgard Blucher Ltda., 1983.
-------------------------------------------------------------*/
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

/*---------------------Prototipos de funcoes----------------------------------*/

void matrizes(double[],double[]);
void elim(double valores[],int nn);
void redu(double[]);
void potencia(void);
void solu(double xce[],double b[]);
void controle(double[],double[]);

/*-------------------------------------------------------------------------
  Declaracoes de lista passada via parametro (fatores L,D,U de B'')
  -----------------------------------------------------------------------*/
double cleb[maxnzero];

/*----------------------Inicio de estado() -----------------------------------*/

void estado(double ce[],double cle[]){
	double dx1[nbmax],dx2[nbmax],dpmax,dqmax,dpa,dqa,qg,aux;
	int k,kp,kq,inic,i,flagq;
	double vmi,vma,ten;
	int kmi,kma;
	
	/*-----------Inicializa contadores e chaves:------------*/
	nitea = 0;        // contador de iteracoes ativas
	niter = 0;        // contador de iteracoes reativas
	ntot  = 0;
	lim   = 1600;
	kp    = 1;        // chaves
	kq    = 1;
	
	/*--------- Montagem das sub-matrizes Jacobianas B' e B''-----------------*/
	
	matrizes(ce,cle);
	
	/*------------------------------ 
    Elimina a(s) barra(s) slack(s) 
   ------------------------------*/
    elim(ce,1);

    /*-------------------------------------------
    Armazena matriz B'' antes de eliminar barras
    --------------------------------------------*/
    for(i = 0; i < maxnzero; i++) cleb[i] = cle[i];
    
  /*------------------------------
    Elimina as barras slack e PV 
    -----------------------------*/
    elim(cle,0);
	
	/*----------Obtencao de fatores triangulares das matrizes B' e B''--------*/
	redu(ce);
	redu(cle);
	
	/*--------------------Inicio do processo iterativo------------------------*/
	inic = 1;
	
	while(niter <= lim && nitea <= lim && (kp!= 0 || kq != 0)){
		if(inic == 1){
                
	/*---------------CALCULA AS POTENCIAS ATIVAS------------------------------*/
	
	/*-------Calculo das injecoes de potencia nas barras:--- */
		potencia();
		inic = 0;
		
	/*-----	Calculo do MISMATCH ATIVO e do seu valor maximo:----*/
		dpmax = 0.e0;
		for(k = 0; k < nb; k++){
			dx1[k] = 0.e0;
			if(itipo[k] != 2){   // se nao for barra slack
               dx1[k] = pes[k] - p[k];
               dpa = fabs(dx1[k]);
               if(dpa > dpmax){
                  dpmax = dpa;
                  }
               dx1[k] = dx1[k]/v[k];
			   }
		    }
	/*-----Relaxando o criterio de convergencia:----------------------*/
        if(dpmax < 1.e-2 && nitea > 100 ) exit(1);
	        
	/*--------Teste de convergencia (fase ATIVA):---------------*/
		if( dpmax > 1.e+10 ){
            printf("\n Divergencia ...\n");
			exit(1);
            }
		    else
			   if(dpmax <= tola)
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
				      for(k = 0; k < nb; k++){
                          tet[k] = tet[k] + dx1[k];
                          }
				
		/*------------------------------------------
		Incrementa o Contador de Iteracoes Ativas 
		----------------------------------------- */
                      nitea++;
		/*-------------Retorna a chave REATIVA ao valor inicial-----------*/
		              kq = 1;
                      }  //  fim do else interno
                }     //  fim do else externo
	    else       // else de inic=1
		if(kq == 1){
              
		/*------------------------------------------------------
		OBS: dx1[]-> Armazena os incrementos dos angulos nodais
			dx2[]-> Armazena os incrementos das tensoes nodais
		-------------------------------------------------------- */
	   
       /*-------------- CALCULA AS POTENCIAS REATIVAS ------------------------*/
	
		/*-------Calculo das injecoes de potencia nas barras:--- */
			potencia();
			inic = 1;
		/* ---------------------
        ATUA OU NAO CONTROLES	
		-----------------------*/
		
		/*--1) Teste de Limites de Geracao Reativa----------------------------*/
		    flagq = 0;
			for(k = 0; k < nb; k++){
                 if(itipo[k] == 1 ){
                    qg = q[k] + qca[k];
                    if(qg >= qgmax[k] + 1.e-2) flagq = 999;
                    if(qg <= qgmin[k] - 1.e-2) flagq = 999;
                    }
                  }
            
		/*--2) Teste de Iteracoes para Decidir sobre Controle (etapa reativa)-*/
		    if(niter == nitqm || flagq == 999){
               if(niter >= nitqm){   
                  printf("\n ");
                  printf("\n  Controle PV&Taps - No. Iter Reat niter = %d",niter);
                  printf("\n ");
                  controle(cle,cleb);
                  }
               }
			
		/*---Calculo do MISMATCH REATIVO e do seu valor maximo:-----------*/
			dqmax = 0.e0;
			for(k = 0; k < nb; k++){
				 dx2[k] = 0.e0;
			     if(itipo[k] == 0){   // se nao for barra slack
                    dx2[k] = qes[k] - q[k];
                    dqa = fabs(dx2[k]);
                    if(dqa > dqmax){
                       dqmax = dqa;
                       }
                    dx2[k] = dx2[k]/v[k];
		            }
			     }     // fim do for...
			
            printf("\n Iteracoes Reativas --> %14.10lf  %d",dqmax,niter);
			
		/*-----Relaxando o criterio de convergencia:----------------------*/
	        if(dqmax < 1.e-2 && niter > 100 ) exit(1);

			/*---------Teste de convergencia (fase REATIVA):----------*/
			if(dqmax > 1.e+10 ){
				printf("\n Divergencia ...\n");
				exit(1);
			    }
			else{
				if(dqmax <= tolr)
				   kq = 0;
				   else{
		/*----------Solucao do sistema linear Q-V para MISMATCH REATIVO:
		----------------------------------------------------------------
		Chamada da rotina solu() - Solucao do Sist. Linear B".dv = dx2 
		----------------------------------------------------------------*/
					   solu(cle,dx2);
					
		/*-----------Atualiza o Vetor de Magnitudes Nodais V:-------------*/
					   for(k = 0; k < nb; k++){
					   	   if(itipo[k] == 0){
                              v[k] = v[k] + dx2[k];
                              }
                        }
							
	/*----------Incrementa o Contador de Iteracoes Reativas  ---------*/
					kq = 1;
					niter++;
					
	/*----------Retorna a chave ATIVA ao valor inicial----------*/
					kp = 1;
				}   //  else   (dqmax < tolr)
			}      //  else   (dqmax > 1e.10)
		}         //  else   (se kq=1)
	}            //  while  
	
	ntot = nitea + niter;
 	printf("\n Numero de Iteracoes Ativas + Reativas %4d = \n",ntot);

    /*------Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes--*/
    if ((ntot > lim)){
		nconv = 1;
		printf("\n O processo nao converge com %4d iteracoes ...\n", ntot);
		exit(1);
        }
	
	/*---------------- Fim do Processo Iterativo com Sucesso-------------*/
	
/* -------------------------------
   Para acompanhamento da solucao
   -------------------------*/

	  vmi = 1.e+10;
	  vma = 0.e0;
      for(k = 0;k < nb;k++){
          ten = v[k];
          if(ten > vma) kma = k;
          if(ten > vma) vma = ten;
	      if(ten < vmi) kmi = k;
	      if(ten < vmi) vmi = ten;
          }
	  printf("\n %4d %9.4lf %4d %9.4lf \n",nex[kma],v[kma],nex[kmi],v[kmi]);
 
	return;
}
/*--------------------------- Fim de estado() --------------------------------*/

