/*---------------------------------------outage.c-------------------------------
 1- conting_single ->   Funcao para simular contingencia simples, usa metodo
                        iterativo ate convergir [ Metodo Pos Compensacao ]
 2- conting_multiple ->  Funcao para simular contingencia multipla, usa metodo
                        iterativo ate convergir[ Metodo Mid Compensation]
 3- refatora ->  Funcao para simular contingencia multipla, usa metodo
                 iterativo ate convergir [ Metodo Refatoracao Parcial ]
 OBS: SOMENTE PARA LISTA REDUZIDA DAS CONTINGENCIAS MAIS SEVERA
------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include<pvm3.h>
/*----------------Inicio da Funcao conting_single-----------------*/
void conting_single(double ce[],double cle[],int casoAnalizado,int mcont,int impr, int ordenSeveridade ){
	int info;
	int k,k1,i,j,l;//,jx,kx;
	double wjk1[nbmax], wjk2[nbmax];
	double wb1,wb2;
	/*-------------- Declaracoes relativas ao PVM -----------------*/
	//int msgtype0;
	/*-------------- Inicializa vetores------------------*/
	for(k1=0; k1<nb; k1++){
		wjk1[k1] = 0.e0;
		wjk2[k1] = 0.e0;
	}
	wb1 = 0.e0;
	wb2 = 0.e0;
	/*-----  Guarda as informacoes do caso base----
	Menu Explicativo:
	info = 0 => guarda as informacoes
		do caso base
	info = 1 => recupera as informacoes
		do caso base
	---------------------------------*/
	info = 0;
	ricupero(info);
	/*------- Identifica ramo j--k do caso contingenciado------------*/
	//ir = kcont[kk][1];
	i = kcont[casoAnalizado][1] - 1;
	j = ni[i];
	k = nf[i];
	//jx = nex[j];
	//kx = nex[k];              // numeracao externa 
	//icont = kk; // parte que usa esta variável está comentadada 18/08/2005
	/*---Verifica se o ramo j--k e trafo ou linha ==> define ejk1[]-------*/
	
	modify(j,k,i);
	
	identdc(j,k);  // definicao em funcao dos tipos de barras < Blinha >
	wjk1[j] = ej1;
	wjk1[k] = ek1;

	identca(j,k,i); // definicao em funcao dos tipos de barras < B2linha >
	wjk2[j] = ej2;
	wjk2[k] = ek2;
	/*-------------- Obtem caminhos parciais e faz uniao-------------*/
	caminho(j,k);   // une os caminhos dos singletons
	/*-------------------
	Solucao do sistema de equacoes B'.wjk1 = ejk1
	Obtem os vetores wjk1[] e wjk2[]
	--------------------------------------------*/
	soluve(ce,wjk1);// faz "FFS & NBS"
	
	soluve(cle,wjk2);// w" = wjk2 e w' = wjk1
	
	/*-----------------------------------------
	Altera os elementos da matriz Yo = Go. + j.Bo em funcao da contingencia
	Modifica Yo para calcular fluxos ou injecoes de potencia
	-------------------------------------------*/
	//modify(j,k,i);
	/*------ Obtem os escalares c1p e c2p <--> saida simples------------*/
	wb1 = 1.e0/(fcont*bklbase[i]);   // retirada de ramo ==> fcont = 1
	wb2 = -(xjk[i]/fcont);          // retirada de ramo ==> fcont = 1.
	c1p = (wb1 + ej1*wjk1[j] + ek1*wjk1[k]); // wb1 = elemento que saiu <impedancia>
	c2p = (wb2 + ej2*wjk2[j] + ek2*wjk2[k]); // wb2 = elemento que saiu
	/*--Teste de ILHAMENTO (Comparar wt1 com a impiedancia de ramo wb1  --> retirada)--*/
	if(fabs(c1p) < 1.e-6 || fabs(c2p) < 1.e-6){
		//  printf("\n OCORREU ILHAMENTO !!! \n");
		kdiag = -1;
	}
	else{
		kdiag = 0;         // Contingencia Normal
		c1p = 1.e0/c1p;
		c2p = 1.e0/c2p;
	/*----Inicio do processo iterativo < contingencia: fluxo de carga CA compensado >--*/
		state_pos(ce,cle,j,k,casoAnalizado,wjk1,wjk2);
	/*--Forma lista de casos que convergiram < contingencia CA >----------*/
		if( kdiag == 0 ){
			kconv[nyconv] = casoAnalizado;
			kordem[nyconv] = ordenSeveridade;
			kit1[nyconv] = nitea;            // no. de iteracoes ativas
			kit2[nyconv] = niter;           // no. de iteracoes reativas
			nyconv++;
		}
		/*---------------------------------------------------
		Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==>
		comentar p/ tomar tempos-----------------------------
     		//COMENTAR PORQUE ROTINA salida()ESTA COMENTADA
		if( impr == icont ){
		fluxos();        // fluxos ativos e reativos nos ramos
		 ----- PVMPVMPVMPVMPVMPVMPVMPVMPVMP---------------
		//Dados recebidos em saida.c
		  for(k1=0; k1<nb; k1++){
		vsend[k1] = v[k1];
		tsend[k1] = t[k1];
		}
		msgtype0 = 11;
		info = pvm_initsend(PvmDataRaw);
		info = pvm_pkint(&icont,1,1);
		info = pvm_pkint(&kdiag,1,1);
		info = pvm_pkint(&nitea,1,1);
		info = pvm_pkint(&niter,1,1);
		info = pvm_pkdouble(vsend,nb,1);
		info = pvm_pkdouble(tsend,nb,1);
		info = pvm_pkdouble(p,nb,1);
		info = pvm_pkdouble(q,nb,1);
		info = pvm_pkdouble(fpkm,nl,1);
		info = pvm_pkdouble(fpmk,nl,1);
		info = pvm_pkdouble(rqkm,nl,1);
		info = pvm_pkdouble(rqmk,nl,1);
		info = pvm_send(tids[0],msgtype0);
		}*/
	}
	/*------   Recupera as informacoes do caso base-----------------*/
	info = 1;
	ricupero(info);
	return;
}
/*--------------------Fim da Funcao conting_single----------------------*/
/*-------------------Inicio da Funcao conting_multiple------------------*/
void conting_multiple(double ce[],double cle[],int casoAnalizado, int mcont,int impr, int ordenSeveridade){
  int i,j,k,ir;//,jx,kx;
  int k1,k2,k3,jj,l,linha,ncoluna,aux;
  double C1[mmax][mmax],WTH1[mmax][mmax],WB1[mmax];
  double C2[mmax][mmax],WTH2[mmax][mmax],WB2[mmax];
  double Wtil1[nbmax][mmax],W1[nbmax][mmax];
  double Wtil2[nbmax][mmax],W2[nbmax][mmax];
  double wjk1[nbmax], wjk2[nbmax];	
  int info; 
  /*-----------   Declaracoes relativa ao PVM   ------------------------*/
//  int msgtype0; 
  /*-----------  Inicializa vetores e matrizes ---------------------------*/
  for(k1=0 ;k1<nb; k1++){
      wjk1[k1] = 0.e0;
      wjk2[k1] = 0.e0;
    }
  
  for(k1=0; k1<mcont; k1++)
    {
      WB1[k1] = 0.e0;
      WB2[k1] = 0.e0;
    }

  for(k1=0; k1<nb; k1++)
    {
      for(k3=0; k3<mcont; k3++)
	{
	  W1[k1][k3] = 0.e0;
	  W2[k1][k3] = 0.e0;
	  Wtil1[k1][k3] = 0.e0;
	  Wtil2[k1][k3] = 0.e0;
	  W1t[k3][k1] = 0.e0;
	  W2t[k3][k1] = 0.e0;
	}
    }
  
  for(k1=0; k1<mcont; k1++)
    {
      for(k3=0; k3<mcont; k3++)
	{
	  WTH1[k1][k3] = 0.e0;
	  C1[k1][k3] = 0.e0;
	  WTH2[k1][k3] = 0.e0;
	  C2[k1][k3] = 0.e0;
	}
    }

  /*--------------------------------
    Guarda informacoes do caso base
    ------------------------------*/

  /*------------------------------------
    Menu Explicativo:

   info = 0 => guarda as informacoes
               do caso base 

   info = 1 => recupera as informacoes
               do caso base

   -----------------------------------*/

  info = 0;
  ricupero(info);
  /*----------------------------------------------------------
    Geracao de vetores e matrizes (ramo-a-ramo para cada caso)
    ---------------------------------------------------------*/
  
  for(jj=1; jj<=mcont; jj++)
    {
      
      /*--------------------------------------------
        Identifica ramo j--k do caso contingenciado
	------------------------------------------*/
      ir = kcont[casoAnalizado][jj];
      i = ir - 1;      
      j = ni[i];
      k = nf[i];
     // jx = nex[j];
     // kx = nex[k];              // numeracao externa 
      icont = casoAnalizado;
      //  printf(" Ramo = %d   Saida do Ramo <numeracao externa> => %d --- %d  Caso =>%d \n",i+1,jx,kx,kk+1);
      //  printf(" Ramo = %d   Saida do Ramo <numeracao interna> => %d --- %d  Caso =>%d \n",i+1,j,k,kk+1);
        
      /*----------------------------------------------------------
	Verifica se o ramo j--k e trafo ou linha ==> define ejk1[]
	--------------------------------------------------------*/
      
      
      identdc(j,k);  // definicao em funcao dos tipos de barras (B')
      wjk1[j] = ej1;
      wjk1[k] = ek1;
      
      
      identca(j,k,i);  // definicao em funcao dos tipos de barras (B'')
      wjk2[j] = ej2;
      wjk2[k] = ek2;
      
      /*---------------------------------------------------------------------
	Obtem caminhos parciais e faz uniao (do metodo dos vetores esparsos)
	--------------------------------------------------------------------*/
      
      caminho(j,k);
      
      
    /*----------------------------------------
      Solucao pelo metodo dos vetores esparsos
      ---------------------------------------*/
      
        
      soluffs(ce,wjk1);   // faz somente "FFS"
      

      soluffs(cle,wjk2);  // faz somente "FFS"
      
      
   
      /*---------------------------------------------- 
	Gera as matrizes Wtil1 e Wtil2 (ordem nb x m)
	---------------------------------------------*/
      aux = jj-1;
      
      for(l=0; l<nb; l++)
	{
	  Wtil1[l][aux] = wjk1[l];
	  Wtil2[l][aux] = wjk2[l];
	  wjk1[l] = 0.e0;
	  wjk2[l] = 0.e0;
	}
      
      /*--------------------------------------------------------------------
	Altera os elementos da matriz Yo=Go + j.Bo em fucao da contingencia
	-------------------------------------------------------------------*/


      /*------------
	Modifica Yo:
	-----------*/
      
      modify(j,k,i);
      
      
     /*-------------------------------------------------
       Define matrizes diagonais (de alteracao da rede)
       ------------------------------------------------*/
      
      WB1[aux] = 1.e0/(fcont*bklbase[i]);   // retirada do ramo ==> fcont=1
      WB2[aux] = -xjk[i]/fcont;          // retirada do ramo ==> fcont=1
      
      
    }//fim for
  
  /*----------------------------------------------------------------------------------
    Obtem as matrizes W1 e W2 a partir de Wtil1 e Wtil2 (e as transpostas : W1t e W2t)
    ---------------------------------------------------------------------------------*/
  
  for(k1=0; k1<mcont; k1++)    // colunas
    {
      for(k2=0; k2<nb; k2++)
	{
	  W1[k2][k1] = (1.e0/ce[k2])*Wtil1[k2][k1];
          W2[k2][k1] = (1.e0/cle[k2])*Wtil2[k2][k1];
	  W1t[k1][k2] = W1[k2][k1];                  // W1 transposta
	  W2t[k1][k2] = W2[k2][k1];                  // W2 transposta
	}
    }
  

  /*-----------------------------------------------------------
    Realizando o produto (Wtil Transposta) x W para obter WTH
    --------------------------------------------------------*/


  /*-----------------------------------
    Geracao da matriz WTH1 de Thevenin
    ---------------------------------*/
  
  multmm3(W1t,Wtil1,mcont,nb,WTH1);         // W1_transposta . Wtil_1 = WTH1
  
  
  /*-----------------------------------
    Geracao da matriz WTH2 de Thevenin
    ---------------------------------*/
  
  multmm3(W2t,Wtil2,mcont,nb,WTH2);        // W2_transposta . Wtil_2 = WTH2  
  
  
  for(linha=0; linha<mcont; linha++)
    {
      WTH1[linha][linha] = WTH1[linha][linha] + WB1[linha];
      WTH2[linha][linha] = WTH2[linha][linha] + WB2[linha];
    }
  
  
  /*--------------------------
    Gera as matrizes C' e C''
    -------------------------*/
  
  for(ncoluna=0; ncoluna<mcont; ncoluna++)
    {
      for(linha=0; linha<mcont; linha++)
	{
	  C1[linha][ncoluna] = WTH1[linha][ncoluna];
	  C2[linha][ncoluna] = WTH2[linha][ncoluna];
	}
    }
  
 
  // Inversa d matriz C1 (ordem nxm) por pivotiamento (Metodo p/ matriz simetrica tipo rede)
  
  /*-----------------------------------------
    Singularidade da matriz C1 ==> ILHAMENTO
    ----------------------------------------*/
  
  
  inversa(C1,mcont);     // inversa retornada em C1
  
  
  /*------------------------------------------------
    Teste de ILHAMENTO (Singularidade de WB1 + WH1)
    -----------------------------------------------*/
  
  if(kdiag == -1)
     printf("\n OCORREU ILHAMENTO ... outage.c  !!! \n");
  else 
    {
      kdiag = 0;
      inversa(C2,mcont);
     
      
      
      
      /*-----------------------------------
	Prepara para fazer compensacao ...
	----------------------------------*/
      
      multmm2(Wtil1,C1,nb,mcont,RESULT1);  // Retorna resultado em RESULT1
      
      
      
      multmm2(Wtil2,C2,nb,mcont,RESULT2);  // Retorna resultado em RESULT2
      
      
      /*--------------------------------------------
	Inicio do Processo Iterativo <contingencia :
	fluxo de carga CA mid-compensation>
	
	------------------------------------------*/

      state_mid(ce,cle,casoAnalizado,wjk1,wjk2);
      
     
      /*------------------------------------------------------
	Forma lista de casos que convergiram <contingencia CA>
	-----------------------------------------------------*/
           
      if(kdiag == 0)
	{
	  
	   kconv[nyconv] = casoAnalizado;
           kordem[nyconv] = ordenSeveridade;
	   kit1[nyconv] = nitea;            // no. de iteracoes ativas
	   kit2[nyconv] = niter;            // no. de iteracoes reativas
	   nyconv++;
	}
      
     
      /*----------------------------------------------------------------------------------------------
	Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==> comentar p/ tomar tempos
	---------------------------------------------------------------------------------------------*/
      //COMENTAR PORQUE salida() esta comentada 
      
      /* if( impr ==  icont ) 
	{
	  fluxos();           //fluxos ativos e reativos nos ramos
	  
	  /* -----------------------------------------------
	     PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
	     ---------------------------------------------*/
	  
      /* for(k1=0; k1<nb; k1++)
	    {   
	      vsend[k1] = v[k1];  
	      tsend[k1] = t[k1];
	    }
	  
	  

	  msgtype0 = 11;
	  info = pvm_initsend(PvmDataRaw);
	  info = pvm_pkint(&icont,1,1);
	  info = pvm_pkint(&kdiag,1,1);
	  info = pvm_pkint(&nitea,1,1);
	  info = pvm_pkint(&niter,1,1);
	  
	  info = pvm_pkdouble(vsend,nb,1);
	  info = pvm_pkdouble(tsend,nb,1);
	  
	  info = pvm_pkdouble(p,nb,1);
	  info = pvm_pkdouble(q,nb,1);
	  info = pvm_pkdouble(fpkm,nl,1);
	  info = pvm_pkdouble(fpmk,nl,1);
	  info = pvm_pkdouble(rqkm,nl,1);
	  info = pvm_pkdouble(rqmk,nl,1);
	  info = pvm_send(tids[0],msgtype0);
	  
	  }*/
      
    }//fim else
  
  
  /*-------------------------------------
    Restabelece informacoes do caso base
    -----------------------------------*/
  
  info = 1;
  ricupero(info);

		
  return;
}
/*-----------------------Fim da funcao conting_multiple------------------------*/






/*--------------------------Inicio da funcao refatora--------------------------*/

 void refatora(double ce[],double cle[],int casoAnalizado,int mcont,int impr,int  ordenSeveridade)
  {
    int k,k1,k2,k3,i,j,jj,l,ir;
    double cebase[max],clebase[max],ekm[nbmax];
    double WB1[1],WB2[1];
    int info;
    
   
    
    /*----------------------------
      Declaracoes relativa ao PVM
      ------------------------*/
    
     int msgtype0; 
    
    /*--------------------------------
      Guarda informacoes do caso base
      -------------------------------*/
    
    /*------------------------------------
      Menu Explicativo:
      
      info = 0 => guarda as informacoes
	          do caso base 
	  
      info = 1 => recupera as informacoes
                  do caso base

     -----------------------------------*/	    


    info = 0;
    ricupero(info);
    
    /*-------------------------------
      Inicializa variaveis e vetores
      ------------------------------*/
    
    WB1[0] = 0.e0;
    WB2[0] = 0.e0;
    


    for(k1=0; k1<nbmax; k1++)
      ekm[k1] = 0.e0; 
    
    /*---------------------------------------
      Guarda matrizes B' e B'' do caso base
      -------------------------------------*/
    
    
    for(k1=0; k1<max; k1++)
      {
	cebase[k1] = ce[k1];     // diagonal e off-diagonal
	clebase[k1] = cle[k1];   // diagonal e off-diagonal
      }
    
    
    /*-----------------------------------------------------------
      Geracao de vetores e matrizes (ramo-a ramo para cada caso)
      ---------------------------------------------------------*/
    
	
    for(jj=1; jj<=mcont; jj++)
      {
	
	/*--------------------------------------------
	  Identifica ramo j--k do caso contingenciado
	  -------------------------------------------*/
	
	ir = kcont[casoAnalizado][jj];
	i = ir-1;    
	j = ni[i];
	k = nf[i];
	//jx = nex[j];
	//kx = nex[k];              // numeracao externa 
	icont = casoAnalizado;
	
	
	//	printf(" Ramo = %d   Saida do Ramo <numeracao externa> => %d --- %d  Caso =>%d \n",i+1,jx,kx,kk+1);
	//printf(" Ramo = %d   Saida do Ramo <numeracao interna> => %d --- %d  Caso =>%d \n",i+1,j,k,kk+1);
	
	/*-----------------------------------------------------------
	  Verifica se o ramo j--k e trafo ou linha ==> define ejk1[]
	  ----------------------------------------------------------*/
	
	
	identdc(j,k);      // definicao em funcao dos tipos das barras (B')
	identca(j,k,i);    // definicao em funcao dos tipos das barras (B'')
	
	/*---------------------------------------------------------------------
	  Obtem caminhos parciais e faz uniao (do metodo dos vetores esparsos)
	  -------------------------------------------------------------------*/ 
	
	
	caminho(j,k);    // une os caminhos dos "singletons"
	
	/*------------------------------------------------------
	  Define os elementos de alteracao da rede (admitancia)
	  -----------------------------------------------------*/
	
	
	WB1[0] = fcont*bklbase[i];  // retirada de ramo ==> fcont=1
	WB2[0] = -fcont/xjk[i];  // retirada de ramo ==> fcont=1
	
	
	/*---------------------------------
	  Atualiza os fatores da matriz B'
	  --------------------------------*/
	
	ekm[j] = ej1;
	ekm[k] = ek1;
	
	bennett(cebase,ekm,WB1);
	
	
	/*-----------------------------------
	  Atualiza os fatores  da matriz B''
	  ---------------------------------*/
	
	ekm[j] = ej2;
	ekm[k] = ek2;
	
	
	bennett(clebase,ekm,WB2);
		
	
	/*--------------------------------------------
	  Altera os elementos da matriz Yo= Go + j.Bo
	  -------------------------------------------*/
	/*-------------
	  Modifica Yo:
	  ------------*/
	
	modify(j,k,i);
	
      }//fim for
    
    /*--------------------------------------------------------------
      Inicio do Processo Iterativo <Contingencia: fluxo de carga CA>
      -------------------------------------------------------------*/
    
    state_normal(cebase,clebase,casoAnalizado);
    
         
    /*------------------------------------------------------
      Forma lista de casos que convergiram <contingencia CA>
      -----------------------------------------------------*/
     
   
    if(kdiag == 0)
      {
	kconv[nyconv] = casoAnalizado;
        kordem[nyconv] = ordenSeveridade;
	kit1[nyconv] = nitea;            // no. de iteracoes ativas
	kit2[nyconv] = niter;            // no. de iteracoes reativas
	nyconv++;
      }
	
	
      /*----------------------------------------------------------------------------------------------
      Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==> comentar p/ tomar tempos
      ---------------------------------------------------------------------------------------------*/
    
     //COMENTAR PORQUE salida() ESTA COMENTADA

    /*  if( impr == icont ) 
       {
       fluxos();           //fluxos ativos e reativos nos ramos
       
       /* -----------------------------------------------
       PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
       ---------------------------------------------*/
    
    /* for(k1=0; k1<nb; k1++)
        {   
            vsend[k1] = v[k1];  
	    tsend[k1] = t[k1];
	  }
	
        
	
	msgtype0 = 11;
	info = pvm_initsend(PvmDataRaw);
	info = pvm_pkint(&icont,1,1);
	info = pvm_pkint(&kdiag,1,1);
	info = pvm_pkint(&nitea,1,1);
	info = pvm_pkint(&niter,1,1);
	
	info = pvm_pkdouble(vsend,nb,1);
	info = pvm_pkdouble(tsend,nb,1);
	
	info = pvm_pkdouble(p,nb,1);
	info = pvm_pkdouble(q,nb,1);
	info = pvm_pkdouble(fpkm,nl,1);
	info = pvm_pkdouble(fpmk,nl,1);
	info = pvm_pkdouble(rqkm,nl,1);
	info = pvm_pkdouble(rqmk,nl,1);
	info = pvm_send(tids[0],msgtype0);
	
	}*/

    /*--------------------------------------
      Restabelece informacoes do caso base
      ------------------------------------*/
    
    info = 1;
    ricupero(info);
    
    
    return;
  }      // Fim de refatora

/*----------------------------Fim da funcao refatora---------------------------*/
