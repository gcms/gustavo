/*----------------------controle.c----------------------------------------------
Subrotina para processar controles em barras PVs e taps de LTCs
          Controle V e Q em barras PVs: pagina 114
          Controle de V por variacao de tap de LTC
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

void elim(double valores[],int nn);
void redu(double[]);
void potencia(void);
void solu(double xce[],double b[]);
	
/*----------------------Inicio de controle() ---------------------------------*/

void controle(double cle[],double cleb[]){
	double dx2[nbmax];
	double vk,vm,ve,dvm,qg,dqmax,dq,t1,t4,ta,ta1,ta2,ta3,ta4,ta5,ysh;
    int ka[nbmax],kb[nbmax],atua[nbmax];
	int k,m,i,lab,nitq,pare,flagt,flagq;
	
	/*-----------Inicializa contadores e chaves:------------*/
	nitq = 0;     // contador de iteracoes reativas durante o controle
	pare = 0; 
	for(k = 0; k < nb; k++){
	    atua[k] = 0;
	    ka[k] = 0;
	    kb[k] = 0;
        }
	
	/*-----------Inicia o processo iterativo---------------------------------*/
	        
	while(pare != 999){
		
    /* ---------------------------------
    Teste de Limites de Geracao Reativa
    --------------------------------*/
	     flagq = 0;
         for(k = 0; k < nb; k++){
           lab = 0;
	       if(itipo[k] == 1){
	          qg = q[k] + qca[k];
	          if(qgmax[k]*SB == 999.e0){
	              v[k] = ves[k];	  // 999 significa sem limite superior
		          lab = 999;
                  }
	          if(qgmin[k]*SB == -999.e0){
	              v[k] = ves[k];	  // -999 significa sem limite inferior
		          lab = 999;
                  }
              if(lab == 0){
	  	          if(qg >= qgmax[k] + 1.e-2){
	                 itipo[k] = 0;	      // converte barra PV em barra PQ
	                 qes[k] = qgmax[k] - qca[k];
	                 ka[k] = 1;		
                     kb[k] = 0;
	                 flagq = 999;
                     }
	              if(qg <= qgmin[k] - 1.e-2){
                     itipo[k] = 0;		  // converte barra PV em barra PQ	
	                 qes[k] = qgmin[k] - qca[k];
	                 ka[k] = 0;
                     kb[k] = 1;
	                 flagq = 999;
                     }
                  }     // fim do if(lab == 0){
           }        // fim do if(itipo[k] == 1){
         }         // fim do for(k = 0; k < nb; k++){
		
	/* ---------------------------------
    Teste de Tensoes Especificadas em 
    Barras de Ramos com LTC
    --------------------------------*/
    
    /*------------Objetivo: controlar a tensao Vm-----------------------------*/
		 flagt = 0;
	     for(i = 0; i < nl; i++){
	         k = ni[i];
	         m = nf[i];
	         if(ltc[i] == 1){
		        if(nb != 30){
			       atua[m] = 0;
		           flagt = 0;
                   }
	            if(v[m] > ves[m] + range){
		           atua[m] = 999;   // significa ter que reduzir o tap
	               flagt = 999;
                   }
		        if(v[m] < ves[m] - range){
		           atua[m] = -999;	 // significa ter que aumentar o tap
	               flagt = 999;
                   }
                }
              }
              
	/*------------Ajuste dos Taps---------------------------------------------*/
	     if(flagt == 999){
            for(i = 0; i < nl; i++){
	            k = ni[i];
	            m = nf[i];
	            if(ltc[i] == 1){
	               ta3 = tap[i]; 
	               ta2 = taps[i];
	               ta1 = tapi[i];
	               tapa[i] = ta3;   // guarda o tap da iteracao anterior
	               vk = v[k];
	               vm = v[m];
	               ve = ves[m];
	               dvm = ve - vm;
	               if(atua[m] == 999){
	                  tap[i] = ta3 + dtap;
	                  if(tap[i] < tapi[i]) tap[i] = tapi[i];
			          if(tap[i] > taps[i]) tap[i] = taps[i]; 
	                  ta4 = 1.e0/ta3;
	                  ta5 = 1.e0/tap[i];
			          v[m] = v[m] + alfa*v[k]*(ta5 - ta4);
	                  if(v[m] < ve - range ) v[m] = ve - range;
	                  
 // Teste
 printf("\n aumenta tap e diminui tensao");
 printf("\n %4d %4d %9.4lf %9.4lf %9.4lf %9.4lf",nex[k],nex[m],tapa[i],tap[i],vm,v[m]); 
	                  
                      }
			       if(atua[m] == -999){
                      tap[i] = ta3 - dtap;
	                  if(tap[i] < tapi[i]) tap[i] = tapi[i];
			          if(tap[i] > taps[i]) tap[i] = taps[i];
	                  ta4 = 1.e0/ta3;
	                  ta5 = 1.e0/tap[i];
			          v[m] = v[m] + alfa*v[k]*(ta5 - ta4);
	                  if(v[m] > ve + range) v[m] = ve + range;
	                  
 // Teste
 printf("\n diminui tap e aumenta tensao");
 printf("\n %4d %4d %9.4lf %9.4lf %9.4lf %9.4lf",nex[k],nex[m],tapa[i],tap[i],vm,v[m]); 
 
                      }
                }
            }

	/*------------Atualizacao da Matriz Y com o Ajuste dos Taps---------------*/
	        for(k = 0; k < nb; k++){
                gdiag[k] = 0.e0;
	            bdiag[k] = shu0[k];  // vem dos dados de barras
	            shu[k]   = shu0[k];
                }
                for(i = 0; i < nl; i++){
	                k = ni[i];
	                m = nf[i];
	                ta = tap0[i];
	                ta1 = tapi[i];
	                ta2 = taps[i];
	                ta3 = tap[i];
	                ta4 = tapa[i];
	                ysh = 2.e0*ylin[i];
	                if(ta == 0.e0){                 
                       gdiag[k] = gdiag[k] + gkl[i];
                       gdiag[m] = gdiag[m] + gkl[i];
                       bdiag[k] = bdiag[k] + bkl[i] + ysh/2.e0;
                       bdiag[m] = bdiag[m] + bkl[i] + ysh/2.e0;
                       shu[k] = shu[k] + ysh/2.e0;           
                       shu[m] = shu[m] + ysh/2.e0;  
                       }
	                if(ta != 0.e0){
	                   t4 = 1.e0/ta4;
	                   t1 = 1.e0/ta3;
	                   gkl[i] = gkl[i]/t4;
                       bkl[i] = bkl[i]/t4;
                       bdiag[k] = bdiag[k] + bkl[i]*t1*t1; 
                       bdiag[m] = bdiag[m] + bkl[i];
                       gdiag[k] = gdiag[k] + gkl[i]*t1*t1;
                       gdiag[m] = gdiag[m] + gkl[i];
                       shu[k] = shu[k] + bkl[i]*t1*(t1 - 1.e0)/2.e0;
                       shu[m] = shu[m] + bkl[i]*(1.e0 - t1)/2.e0;
                       gkl[i] = gkl[i]*t1;
                       bkl[i] = bkl[i]*t1;
                       } 
                }   // fim do for(i = 0; i < nl; i++){
             }      // fim do if(flagt == 999){

	
	/*------------Calculo das Injecoes de Potencia nas Barras-----------------*/
             potencia();
	
	/* -----------------------------------------------------
    Se flagq = 999 significa que o limite de Qg foi imposto
    --------------------------------------------------------*/
	         if(flagq == 999){

	/*------------Recupera Estrutura Original de B''--------------------------*/
                  for(i = 0; i < maxnzero; i++) cle[i] = cleb[i];
    
	/*------------Elimina as barras slack e PV--------------------------------*/
                  elim(cle,0);
                  
    /*------------Fatora B'' com a rotina redu()------------------------------*/
		          redu(cle);
                  }  // fim do if(flagq == 999){
                        
	/*------------Calculo do Mismatch Reativo e do seu Valor Maximo-----------*/
	              dqmax = 0.e0;
                  for(k = 0; k < nb; k++){
	                  dx2[k] = 0.e0;
	                  if(itipo[k] == 0){
	                     dx2[k] = qes[k] - q[k];      
                         dq = fabs(dx2[k]);
                         if(dq > dqmax){
                            dqmax = dq;
                            }
                         dx2[k] = dx2[k]/v[k];
                         }
                      }
                   printf("\n \n");
                   printf("\n Iteracoes Auxiliares %12.8lf %4d...",dqmax,nitq);
	         
	/* ----------------------------------
    Teste de Convergencia (fase Reativa) 
    -----------------------------------*/
                   if(dqmax > 1.e10 ){
                      printf("\n Divergencia Controle ...\n");
				      exit(1);
                      }
    /* -----------------------------------
    Relaxando o criterio de convergencia
    -------------------------------------*/
	               if(dqmax < 1.e-2 && nitq > 100) exit(1);


    /*------------Solucao do Sistema Linear Q-V para Mismatch Reativo---------*/
                   solu(cle,dx2);
    
    /*--------------Atualiza o Vetor de Magnitudes Nodais V-------------------*/
                   for(k = 0; k < nb; k++){
				       if(itipo[k] == 0) v[k] = v[k] + dx2[k];
                       }

/*---------------Verifica se Barras Podem Voltar a Ser PVs--------------------*/		
                   if(flagq == 999){
                      for(k = 0; k < nb; k++){
	                      if(ka[k] == 1){
	                         if(v[k] > ves[k] + range ){
	                            itipo[k] = 1;  // converte barra PQ em barra PV
                                }
                             }							
                          if(kb[k] == 1){
                             if(v[k] < ves[k] - range){
                                itipo[k] = 1; // converte barra PQ em barra PV
                                }
                             }
                       }    // fim do for(k = 0; k < nb; k++)
                   }     // fim do if(flagq == 999){

/* -------------------------------
    Teste de Iteracoes (fase Reativa) 
    --------------------------------*/
                   nitq++;
                   printf("\n \n");
                   printf("\n No. Iteracoes Reativas Auxiliares %4d...\n",nitq);
                   
                   if(nitq > limite) pare = 999;
                   if(flagq == 0 && flagt == 0) pare = 999;
	}            // fim do while
	
   /*------Voltar o vetor de tipos de barras ao seu conteudo original --------*/
	for(k = 0;k < nb;k++){
        itipo[k] = itipob[k];
        }
	
	return;
}
/*--------------------------- Fim de controle() ------------------------------*/

