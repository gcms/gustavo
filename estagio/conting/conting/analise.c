/*-----------------------------analise.c--------------------------------- */
#include<stdio.h>
#include<stdlib.h>
#include "global.h"
#include<pvm3.h>
/*------------------------------------------------------------------
 Faz Screening de Contingencias Atraves de Metodos Diretos
 Metodo disponivel para Contingencia Simples
      1) 1P-1Q DIRETO POST-COMP.       (zollenkopf/LIM)
  Metodos disponiveis para Contingenicas Multiplas (ativo/reativo):
      1) 1P-1Q DIRETO MID-COMP.        (zollenkopf/LIM)
      2) 1P-1Q DIRETO FACT. UPDATING   (zollenkopf)     
----------------------------------------------------------------------*/

/*--------------Inicio da funcao scrdireto_reativo--------*/

void scrdireto_reativo(double ce[],double cle[],int impr, int casoAnalizado){
    int mcont;
  /*
  ----------------------------------------------
    Obtem os fluxos ativos e reativos do Caso Base
    --------------------------------------------
  //fluxpq();  
  --------------------------------------------
    PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM
    -----------------------------------------
  ---------------------------------------------------------------
    Metodos de SCREENING Associados no Modelo Incremental da Rede:
    -------------------------------------------------------------
  // printf("\n USANDO O METODO 1P-1Q  \n"); 
  
  ---------------------------------------------------------
    Faz o PRE-PROCESSAMENTO OU SCREENING (Modelo Incremental)
    ----------------------------------------------------------
  */
  
    //printf("Ioutlocal em scrdireto_reativo =  %d", ioutlocal);
    mcont = kcont[casoAnalizado][0];      //mcont = no. de alteracoes por caso
    if(mcont == 1)             //contingencia simples
	    /*--------------------------------------------------
	    Calcula Contingencia Simples
	    Uma Iteracao Completa do FDLF (Post-Compensation)
	    --------------------------------------------------*/
	    cont_pqdir(ce,cle,casoAnalizado,mcont,impr);
  
  else 
    {
      if(mcont > 1 && mcont <= 4)   //contingencia multipla mcont=2,3 ou 4
	/*--------------------------------------------------
	  Calcula Contingencia Multipla
	  Uma Iteracao Completa do FDLF (Post-Compensation)
	  ---------------------------------------------------*/
	contingpq_multiple(ce,cle,casoAnalizado,mcont,impr);
        if (mcont > 4)                //Atualizacao de Fatores mcont= 5,6,7,...ate 10
	
	refatorapq(ce,cle,casoAnalizado,mcont,impr);
      
    }

  return;
  
} // Fim da funcao
/*----------------------Fim da funcao scrdireto_reativo-------------------------*/
/*----------------------Inicio da funcao contingenciaAC----------------------------- */

void contingenciaAC(double ce[],double cle[],int impr, int casoAnalizado,int ordenSeveridade){
	int i,mcont;
	/*-----------------------------------------------------
	Obtem Caminhos de Fatoracao  (esparsidade de vetores)
	---------------------------------------------------
	-------------CALCULA CONTINGENCIA AC------------------------*/
	
	mcont = kcont[casoAnalizado][0];  //mcont = no. de alteracoes por caso
	if(mcont == 1){             //contingencia simples
		  /*-------------------------------------------------------
		  CALCULA CONTINGENCIAS CA SIMPLES POR POS - COMPENSACAO
		  -----------------------------------------------------*/
		  conting_single(ce,cle,casoAnalizado,mcont,impr,ordenSeveridade);
	}
	else {
		if(mcont > 1 && mcont <= 4)   //contingencia multipla
/*-----------CALCULA CONTINGENCIA CA MULTIPLA POR MID - COMPENSACAO-----------*/
			conting_multiple(ce,cle,casoAnalizado,mcont,impr,ordenSeveridade);
		if (mcont > 4) //Atualizacao dos Fatores: mcont= 5,6,7,...ate 10
/*----------- CALCULA CONTINGENCIA CA MULTIPLA POR ATUALIZACAO DE FATORES---*/
			refatora(ce,cle,casoAnalizado,mcont,impr,ordenSeveridade);
	}
	return;
}
/*----------------- Fim da funcao contingenciaAC  --------------*/ 

