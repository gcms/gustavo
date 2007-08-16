/*--------------------------state.c----------------------------

Este arquivo contem as seguintes funcoes:
  1) state_pos -> Funcao para processar as iteracoes do  Fluxo de Carga Pos-Compensado
  2) state_mid -> Funcao para processar as iteracoes do Fluxo de Carga  Mid-Compensado
  3) state_normal -> Funcao para processar as iteracoes do Fluxo de Carga Parcial Refatorado
--------------------------------------------------------------------*/
/*------------------ Arquivos e bibliotecas incluidos--------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "global.h"

void state_pos(double ce[],double cle[],int j,int k,int casoAnalizado, double wjk1[], double wjk2[]){
	double dtet[nbmax],dv[nbmax];
	double dta[nbmax],dva[nbmax];
	//double dva1[nbmax];
	double deltp,deltq,dtp,dtq;
	double dtc,dvc;
	int llavep,llaveq;
	int i; 
	 
	kdiag = 0;   //fornece o diagnostico     
	llavep = 0;
	llaveq = 0;
	nitea = 0;   // numero de iterações ativas
	niter = 0;   // numero de iterações reativas
	limite = 60; // limite de iterações do fluxo de potência
	
	//--- Iteracao ativa << ativa >> << ativa >> << ativa >> << ativa >>----
	
	while(niter <= limite && nitea <= limite && (llavep!= 1 || llaveq != 1)){
		/*---Calculo das injecoes de potencia nas barras: ---------- */
		potencia();
		for(i=0; i<nb; i++){
			dta[i] = 0.e0;
			dva[i] = 0.e0;
		}
		/*---- Teste de convergencia  << parte ativa >-------*/
		deltp = 0.e0;
		for(i=0; i<nb; i++){
			dta[i] = pes[i]- p[i];     // mismatch
			dtp = fabs(dta[i]);
			dta[i] = dta[i]/v[i];
			if(itipo[i] != 2){
				if(dtp > deltp)
					deltp = dtp;// determina o maior mismatch
			}
		}
		if( deltp > 1.e+20 ){
			 kdiverge[ndiv] = casoAnalizado; // divergencias ocorridas
			 ndiv++;
			 kdiag = 2;            // significa divergencia
			 return;
		}
		if(deltp <= toler){
			llavep = 1;
			if(llaveq == 1)
				return;
		}
		else
			llavep = 0;
		/*-------------------------------------------------
		Solucao do sistema linear P-TETA para MISMATCH ATIVO:
		Chamada da rotina solu() - Solucao do Sist. Linear B'.dta = dp/v
		--------------------------------------------------*/
		/*for( i=0; i<10; i++)
			printf("\n antes dta[%d] = %f ", i, dta[i]);*/
		
		solu(ce,dta);
		
		/*---------- Compensando os incrementos de angulos  ---------------*/
		
		dtc = -(ej1*dta[j] + ek1*dta[k])*c1p;

		for( i=0; i<nb; i++)
			dtet[i] = dta[i] + dtc*wjk1[i];
		/*----Atualiza o Vetor de Angulos Nodais TETA:-----*/
		for( i=0; i < nb; i++){
			tet[i] = tet[i] + dtet[i];
			t[i] = tet[i];
		}
		/*---------retorna chave ativa para valor incial------
		// llavep=0   // importante isto ficar comentado
		---------Incrementa o Contador de Iteracoes Ativas -- */
		nitea++;
		//-- Iteracao reativa << reativa >> << reativa >> << reativa >>-
		/*---------Calculo das injecoes de potencia nas barras:--- */
		potencia();
		for(i=0; i<nb; i++){
			dta[i] = 0.e0;
			dva[i] = 0.e0;
		}
		/*-------Teste de convergencia  << parte ativa >--------*/
		deltq = 0.e0;
		for(i=0; i<nb; i++) {
			dva[i] = qes[i] - q[i];     // mismatch
			dtq = fabs(dva[i]);
			if(itipo[i] <= 0){
				if(dtq > deltq){
					deltq=dtq;  // determina o maior mismatch
					//printf("\n dva[%d]  = %f", i, fabs(dva[i]));
				}
				dva[i] = dva[i]/v[i];
			}
		}
		if( deltq > 1.e+10 )  {
			kdiverge[ndiv] = casoAnalizado;   // divergencias ocorridas
			kdiag = 2;                       // significa divergencia
			ndiv++;
			return;
		}
		if(deltq <= toler){ 
			llaveq = 1;
			if(llavep == 1)
				return; 
		}
		else
			llaveq = 0;
		/*---------------------------------------------------
		Solucao do sistema linear P-TETA para MISMATCH ATIVO:
		Chamada da rotina solu() - Solucao do Sist. Linear B".dva = dp/v
		----------------------------------------------------*/
		solu(cle,dva);

		/*------Compensando os incrementos de angulos  ----*/
		
		dvc = -(ej2*dva[j] + ek2*dva[k])*c2p;
		
		for( i=0; i<nb; i++)
			dv[i] = dva[i] + dvc*wjk2[i];
		/*----- Atualiza as Tensoes       -----------------*/
		for( i=0; i<nb; i++){
			v[i] = v[i] + dv[i];
		}
		/*----------------  retorna chave ativa para valor incial---*/
		llaveq=0;
		/*------  Incrementa o Contador de Iteracoes Reativas ------*/
		niter++;
		
	}//fim while
	if ((niter > limite) || (nitea >limite)){
		/*----------------------------------------------------------------
		Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
		---------------------------------------------------------------*/
		ntot = nitea + niter;
		knotconv[nnconv] = casoAnalizado;
		kdiag = 1;        // Significa nao convergencia
		nnconv++;
		// printf("\n O PROCESSO NAO CONVERGE COM %4d ITERACOES !!\n", ntot);
		return;
	}
	return;
}


	 
/*--------------------------Fim da funcao state_pos-----------------------*/

/*------------------- Inicio da funcao state_mid--------------------------*/


void state_mid(double ce[],double cle[],int casoAnalizado, double wjk1[], double wjk2[])   // chamada no programa principal
{
  double dtet[nbmax],dv[nbmax],dta[nbmax],dva[nbmax];
  double fc1[nbmax],f1[nbmax],deltaf1[nbmax];
  double fc2[nbmax],f2[nbmax],deltaf2[nbmax];
  double prod[nbmax];
  double dtp,dtq,deltp,deltq;
  int llavep,llaveq,mcont;
  int i,ii;
 

  kdiag = 0;      // variavel que fornece o diagnostico     
  llavep = 0;
  llaveq = 0;
  nitea = 0;
  niter = 0;
  limite = 60;
 
 
  //------ Iteracao ativa << ativa >> << ativa >> << ativa >> << ativa >>--------
 

  while(niter <= limite && nitea <= limite && (llavep!= 1 || llaveq != 1))
    {

	/*------------------------------------------- 
	  Calculo das injecoes de potencia nas barras:
	  ------------------------------------------- */

	potencia();

 	for(i=0; i<nb; i++)
	  {
	    deltaf1[i] = 0.e0;
	    dtet[i] = 0.e0;
	    deltaf2[i] = 0.e0;
	    dv[i] = 0.e0;
	   
	  }
  
    
	/*---------------------------------------
	  Teste de convergencia  << parte ativa >
	  --------------------------------------*/

	
	deltp = 0.e0;
	for(i=0; i<nb; i++)
	  {
            fc1[i] = pes[i]-p[i];     // mismatch
	    dtp = fabs(fc1[i]);
	    fc1[i] = fc1[i]/v[i];
	   
	    if(itipo[i] != 2)
	      {
	        if(dtp > deltp)
		  deltp=dtp;       // determina o maior mismatch
	      }
	  }
	

	if( deltp > 1.e+10 )
	  {
	    // printf("\n  DIVERGENCIA ...:Contingencia --> %d \n",icont + 1);
	    
	    kdiverge[ndiv] = casoAnalizado;             // vetor que guarda as divergencias ocorridas
	    kdiag = 2;                       // significa divergencia
	    ndiv++;
	    return;
	  }

	  if(deltp <= toler)
	    {  
	      llavep = 1;
	      if(llaveq == 1)
		return; 
	    }
	  else
	    llavep = 0;
		

	  /*------------------------------------
	    Solucao do sistema D'.L'.fc1 = dp/v
	    -----------------------------------*/

	 

	  solufs(ce,fc1);
	 
	  /*---------------------------------------
	    Calculo do vetor de compensacao deltaf1
	    -------------------------------------*/
	 

	  multmv4(W1t,fc1,mcont,nb,prod);
	 
	 

	  multmv2(RESULT1,prod,nb,mcont,deltaf1);
	  
	  
	   
	  /*---------------
	    Compensando ...
	    --------------*/

	           
	  for( i=0; i<nb; i++)
	    {
	      f1[i] = fc1[i] - deltaf1[i];
	      dtet[i] = f1[i];
	    }
	  
	 


	  /*------------------------------
	    Solucao completa da rede

	    Resolve o sistema U'.dtet = f1
	    ------------------------------*/
	  
	  solubs(ce,dtet);

	 
	 
	  /*----------------------------------------
	    Atualiza o Vetor de Angulos Nodais TETA:
	    ----------------------------------------*/

	  for(i=0; i<nb; i++)
	    {
	      tet[i] = tet[i] + dtet[i];
	      t[i] = tet[i];
	    }
	  
	  /*------------------------------------------
	    Incrementa o Contador de Iteracoes Ativas 
	    ----------------------------------------- */
	  
	  nitea++;
	  
	  /*----------------------------------------
	    Retorna a chave REATIVA ao valor inicial
	    -----------------------------------------*/
	  
	  //llavep = 0;
	  
      
	

  
        /*-----ITERACAO REATIVA << reativa >> << reativa >>  << reativa >>------*/
  
      
	/*--------------------------------------------
          Calculo das injecoes de potencia nas barras:
	  ---------------------------------------------*/
	  
	  potencia();
	
	  for(i=0; i<nb; i++)
	    {
	      deltaf1[i] = 0.e0;
	      deltaf2[i] = 0.e0;
	      dtet[i] = 0.e0;
	      dv[i] = 0.e0;
	    }

	 	  
	  /*-------------------------------------------------
	    Calculo do MISMATCH REATIVO e do seu valor maximo:
	    --------------------------------------------------*/
	  
	  deltq = 0.e0;
	  for(i=0; i<nb; i++)
	    {
	      fc2[i] = qes[i] - q[i];
	      dtq = fabs(fc2[i]);
	      if( itipo[i] <= 0 )
		{
		  if (dtq > deltq)    
		    deltq = dtq;        //maior valor absoluto do vetor de mismatch reativo
		  
		  fc2[i] = fc2[i]/v[i];
		}
	    }     // fim do for...
	  
	  

	  
	  /*-------------------------------------
	    Teste de convergencia (fase REATIVA):
	    -------------------------------------*/
	  
	  if( deltq > 1.e+10 )
	    {
	      // printf("\n  DIVERGENCIA ...:Contingencia --> %d \n",icont + 1);
	      kdiverge[ndiv] = casoAnalizado;
	      kdiag = 2;
	      ndiv++;
	      return;
	     
	    }
	    if( deltq <= toler )
	      {
		llaveq = 1;
		if(llavep == 1)
		  return;
	      }
	    else
	      {
		llaveq = 0;

		/*---------------------------------------------------
		  Solucao do sistema linear Q-V para MISMATCH REATIVO:
		  ---------------------------------------------------*/
	    
		  
		/*---------------------------------
		  Solucao do Sist. D".L".fc2 = dq/v 
		  --------------------------------*/
		
		solufs(cle,fc2);

	
		/*---------------------------------------
		  Calculo do vetor de compensacao deltaf2
		  -------------------------------------*/
	    
		multmv4(W2t,fc2,mcont,nb,prod);

	
		
		multmv2(RESULT2,prod,nb,mcont,deltaf2);
	    
	
		/*---------------
		  Compensando ...
		  --------------*/
	    
		  
		for( i=0; i<nb; i++)
		  {
		    f2[i] = fc2[i] - deltaf2[i];
		    dv[i] = f2[i];
		  }
		
		
		/*------------------------------
		  Resolve o sistema U'.dtet = f1
		  ------------------------------*/
	      
		solubs(cle,dv);
		
	     
		/*----------------------------------------
		  Atualiza o Vetor de Magnitudes Nodais V:
		  -----------------------------------------*/
	      
		for( i=0; i<nb; i++)
		  v[i] = v[i] + dv[i];
	    
	
		
	    
		/*-------------------------------------- 
		  Retorna a chave REATIVA ao valor inicial
		  -------------------------------------*/
		llaveq = 0;


		/*-------------------------------------------
		  Incrementa o Contador de Iteracoes Reativas
		  --------------------------------------------*/

			niter++;
	

			
	      }//fim else
	    
    }//  while
  
  
  if ((niter > limite) || (nitea >limite))
    {
      /*----------------------------------------------------------------
	Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
	----------------------------------------------------------------*/
      
      ntot = nitea + niter;
      knotconv[nnconv] = casoAnalizado;
      kdiag = 1;        // Significa nao convergencia
      nnconv++;
      //printf("\n O PROCESSO NAO CONVERGE COM %4d ITERACOES  !!\n", ntot);
      return;
    }
  
  
  return;
}

/* ---------------------------------Fim da funcao state_mid-------------------------------------*/




/* ---------------------------------Inicio da funcao state_normal-------------------------------------*/


void state_normal(double cebase[],double clebase[],int casoAnalizado)   // chamada no programa principal
{
  double dtet[nbmax],dv[nbmax],dta[nbmax],dva[nbmax];
  double deltp,deltq,dtp,dtq;
  int llavep,llaveq;
  int i;
 
  kdiag = 0;      // variavel que fornece o diagnostico     
  llavep=0;
  llaveq=0;
  nitea=0;
  niter=0;
  limite = 60;




  //------ Iteracao ativa << ativa >> << ativa >> << ativa >> << ativa >>--------
 

  while(niter <= limite && nitea <= limite && (llavep!= 1 || llaveq != 1))
    {

	/*------------------------------------------- 
	  Calculo das injecoes de potencia nas barras:
	  ------------------------------------------- */

      
	potencia();

 	for(i=0; i<nb; i++)
	  {
	    dta[i] = 0.e0;
	    dva[i] = 0.e0;
	  }
  
    
	/*---------------------------------------
	  Teste de convergencia  << parte ativa >
	  --------------------------------------*/


	deltp = 0.e0;
	for(i=0; i<nb; i++)
	  {
            dtet[i]=pes[i]-p[i];     // mismatch
	    dtp = fabs(dtet[i]);
	    dtet[i] = dtet[i]/v[i];
	    if(itipo[i] != 2)
	      {
	        if(dtp > deltp)
		  deltp=dtp;       // determina o maior mismatch
	      }
	  }
  

	if( deltp > 1.e+10 )
	  {
	    // printf("\n  DIVERGENCIA ...:Contingencia --> %d \n",icont + 1);
	    kdiverge[ndiv] = casoAnalizado;             // vetor que guarda as divergencias ocorridas
	    kdiag = 2;                       // significa divergencia
	    ndiv++;
	    return;
	  }

	  if(deltp <= toler)
	    {  
	      llavep = 1;
	      if(llaveq == 1)
		return; 
	    }
	  else
	    llavep = 0;
			 
	    
	  /*---------------------------------------------------------------
	    Solucao do sistema linear P-TETA para MISMATCH ATIVO:
	    Chamada da rotina solu() - Solucao do Sist. Linear B'.dta = dp/v
	    ----------------------------------------------------------------*/
	  
	  solu(cebase,dtet);
	  
	    


	  /*----------------------------------------
	    Atualiza o Vetor de Angulos Nodais TETA:
	    ----------------------------------------*/
	  
	  for( i=0; i<nb; i++)
	    {
	      tet[i] = tet[i] + dtet[i];
	      t[i] = tet[i];
	    }
	  
	  /*-------------------------------------
	    retorna chave ativa para valor incial
	    ------------------------------------*/

	  // llavep=0;    // importante isto ficar comentado


	  /*------------------------------------------
	    Incrementa o Contador de Iteracoes Ativas 
	    ----------------------------------------- */

	  nitea++;
	  

	 
	  //------ Iteracao reativa << reativa >> << reativa >> << reativa >>------ 


	  /*------------------------------------------- 
	    Calculo das injecoes de potencia nas barras:
	    ------------------------------------------- */

	  potencia();

	  for(i=0; i<nb; i++)
	    {
	      dta[i] = 0.e0;
	      dva[i] = 0.e0;
	    }
  
    
	  /*---------------------------------------
	    Teste de convergencia  << parte ativa >
	    --------------------------------------*/


	  deltq = 0.e0;
	  for(i=0; i<nb; i++)
	    {
	      dv[i]=qes[i]-q[i];     // mismatch
	      dtq = fabs(dv[i]);
	      
	      if(itipo[i] <= 0)
		{
		  if(dtq > deltq)
		    deltq=dtq;       // determina o maior mismatch

		  dv[i] = dv[i]/v[i];
		}
	    }
	  

	  if( deltq > 1.e+10 )
	    {
	      // printf("\n  DIVERGENCIA ...:Contingencia --> %d \n",icont + 1);
	      kdiverge[ndiv] = casoAnalizado;             // vetor que guarda as divergencias ocorridas
	      kdiag = 2;                       // significa divergencia
	      ndiv++;
	      return;
	    }

	  if(deltq <= toler)
	    {  
	      llaveq = 1;
	      if(llavep == 1)
		return; 
	    }
	  else
	    llaveq = 0;
	  
	    
	  /*---------------------------------------------------------------
	    Solucao do sistema linear P-TETA para MISMATCH ATIVO:
	    Chamada da rotina solu() - Solucao do Sist. Linear B'.dta = dp/v
	    ----------------------------------------------------------------*/
	  
	  solu(clebase,dv);
	  
	 	  
	  
	  /*--------------------
	    Atualiza as Tensoes
	    -------------------*/
	  
	  for( i=0; i<nb; i++)
	    v[i] = v[i] + dv[i];
	    
	  
	  /*-------------------------------------
	    retorna chave ativa para valor incial
	    ------------------------------------*/

	   llaveq=0;    


	  /*-------------------------------------------
	    Incrementa o Contador de Iteracoes Reativas 
	    ------------------------------------------*/

	  niter++;
	  
    }//fim while

  if ((niter > limite) || (nitea >limite))
    {
      /*----------------------------------------------------------------
	Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
	----------------------------------------------------------------*/
      
      ntot = nitea + niter;
      knotconv[nnconv] = casoAnalizado;
      kdiag = 1;        // Significa nao convergencia
      nnconv++;
      // printf("\n O PROCESSO NAO CONVERGE COM %4d ITERACOES !!\n", ntot);
      return;
    }
  
 
  return;

}
/* ---------------------------------Fim da funcao state_normal-------------------------------------*/

//Data de atualizacao: 03/10/2003
