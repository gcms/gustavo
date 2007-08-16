/*-------- escravo.c-------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "prototipos.h"

#include "pvm3.h"
/*------------Inicio da funcao EscravoA--------------------
	--------------------------------------------
	Rotina EscravoA(): Modo Paralelo Assincrono
	-------------------------------------------*/

void EscravoA(double ce[],double cle[],int impr){
	//printf("inicio do processo contingencia");
	int  j;
/*--------------------- Declaracao relativa ao PVM--------------*/
	int  mytid,info,bufid,icontr,nrcaso,lnt;
	int  nitens1,nitens2,nitens3,nitens4,nitens5,nitens6;
	int  msgtype2,msgtype3,msgtype4,msgtype5,msgtype6,nofinal;
	int  nbase, casoAnalizado;
/*------------ Rotulos das mensagens [ types ]------------*/
	msgtype2 = 31;
	msgtype3 = 41;
	msgtype4 = 51;
	msgtype5 = 61;
	msgtype6 = 71;
	nbase = NPROC;
	nofinal = NPROC-1;
	nitens1 = nrc + 1;
	nitens2 = nrc + 1;   
/*------------------------------------------------------------------
  Abro arquivo para escrita ver o que os filhos estao calculando ...
-------------------------------------------------------------------
---------------------------------
 Calculo do indice PI e ordenacao 
-------------------------------*/
	icontr = 0;    // inicializa contador de casos
/*
-----------------------------------------------
       PROCESSAMENTO PARALELO ASSINCRONO
----------------------------------------------
    ------------------------------------------------
    O LOOP AC e Executado em Paralelo Assincrono: O 
    Processador Assim que Termina seu Caso Pega  um
    Novo Caso para Executar 
    ------------------------------------------------
    O Controle e Feito Somente pelo No "0" que Tam-  
    bem Executa Calculos AC Enquanto Espera
    ------------------------------------------------
    --------- Inicio do Processamento da Contingencia Lista Reduzida --------
    --------------------------
    Recebe os primeiros casos 
    -------------------------*/

	bufid = pvm_recv(tids[0], msgtype2);
	info = pvm_upkint(kbusy,2,1);
	info = pvm_upkint(&ncrit,1,1);
	casoAnalizado = kbusy[0];          // caso a ser analisado
	nrcaso = kbusy[1];                 //  no. do caso recebido
	
	//while(casoAnalizado != -1){
	while(nrcaso < ncrit){
		contingenciaAC(ce,cle,impr,casoAnalizado,nrcaso);// esta rotina faz chamadas da contingencia AC
		mytid = pvm_mytid();
		kbusy[0] = mytid;  // tids() do processo corrente
		kbusy[1] = nrcaso; // no. do caso ja calculado
		info = pvm_initsend(PvmDataDefault);
		info = pvm_pkint(kbusy,2,1);  
		info = pvm_send(tids[0],msgtype3);
		//----------Host corrente verifica o "buffer" -------------
		lnt =  pvm_recv(tids[0], msgtype4 );
		//while ( lnt < 0 ){
	 	//------- Host corrente verifica o "buffer" ------
		//	lnt =  pvm_recv( -1, msgtype4 );
		//}
		//printf("\n LNT = %d", lnt);
	//	if(lnt >= 0){
			//----------Escravo recebe novo caso para analisar ---------
		info = pvm_upkint(kbusy,2,1);
		casoAnalizado = kbusy[0];     // caso a ser analisado
		nrcaso = kbusy[1];       // no. do caso recebido
			//printf("\n kbusy[0] = %d",kbusy[0]);
		//}
	}// Fim while iout
	/* --- Fim do Processamento da Contingencia para Lista Reduzida---------
	  ------------------------------------------------
	 Envia Resultados da Simulacao de Contingencias 
	 ao Processador "0"
 ----------------------------------------------
  // == Ilhamentos/Divergencias/Nao-Convergencias/Convergencias/Iteracoes:*/
	printf("\n NO  | ILHAMENTO | DIVERG. | CONVERG.  | NAO-CONVERG. |\n");
	printf("\n %2d  | %5d     |%5d    |  %5d    |    %5d     |\n",myinst,nilha,ndiv,nyconv,nnconv);
	
	for(j=1; j<=nilha; j++)
		isend1[j] = kilha[j-1]; 

	for(j=1; j<=ndiv; j++) 
		isend2[j] = kdiverge[j-1] ;

	for(j=1; j<=nnconv; j++) 
		isend3[j] = knotconv[j-1]; 

	for(j=1; j<=nyconv; j++){
		isend4[j] = kconv[j-1]; 
		isend5[j] = kit1[j-1];
		isend6[j] = kit2[j-1];
		isend7[j] = kordem[j-1];
	}
	isend1[0] = nilha;
	isend2[0] = ndiv;
	isend3[0] = nnconv;
	isend4[0] = nyconv;
	isend5[0] = nyconv;
	isend6[0] = nyconv;
	nitens3 = isend1[0]+1;
	nitens4 = isend2[0]+1;
	nitens5 = isend3[0]+1;
	nitens6 = isend4[0]+1;
	// == Envia os tamanhos dos vetores:
	info = pvm_initsend( PvmDataDefault );
	info = pvm_pkint(&isend1[0],1,1 );
	info = pvm_pkint(&isend2[0],1,1 );
	info = pvm_pkint(&isend3[0],1,1 );
	info = pvm_pkint(&isend4[0],1,1 );
	info = pvm_send( tids[0], msgtype5 );
	// == Envia os vetores:
	info = pvm_initsend( PvmDataRaw );
	info = pvm_pkint( isend1,nitens3,1 );
	info = pvm_pkint( isend2,nitens4,1 );
	info = pvm_pkint( isend3,nitens5,1 );
	info = pvm_pkint( isend4,nitens6,1 );
	info = pvm_pkint( isend5,nitens6,1 );
	info = pvm_pkint( isend6,nitens6,1 );
	info = pvm_pkint( isend7,nitens6,1 );
	info = pvm_send( tids[0], msgtype6 );
	return;
}


void EscravoScreening(double ce[max],double cle[max],int impr){
	int  kk;
	/*---------------------------
	Declaracao relativa ao PVM
	-------------------------*/
	int  mytid,info,aux,nofinal,icontr;
	int  nitens1,nitens2,nitens6;
	int  msgtype1;
	int  nbase;
	msgtype1 = 21;  // rotulo da mensagem enviada ao pai
	nbase = NPROC;
	nofinal = NPROC-1;
	nitens1 = nrc + 1;
	nitens2 = nrc + 1;   
	/*------------------------------------------------------------------
	Abro arquivo para escrita ver o que os filhos estao calculando ...
	-------------------------------------------------------------------*/
	/*---------------------------------
	Calculo do indice PI e ordenacao 
	-------------------------------*/
	icontr = 0;    // inicializa contador de casos calculados pelo processador corrente
	for(kk=myinst; kk<nrc; kk+=nbase){
		// kk ==>> caso a ser analisado
		// esta rotina faz chamadas do metodo 1P-1Q
		scrdireto_reativo(ce,cle,impr,kk);
		icontr++;
		isend1[icontr] = kk;        // no. do caso
		psend[icontr] = pia[kk];    // indice de severidade
	}
	isend1[0] = icontr;
	psend[0] = 0.e0; // so para 1a. posicao nao ficar lixo
	aux = icontr + 1;
	/*----------------------------------------------------------
	Envia Resultados do screening para o Processador "0"
	--------------------------------------------------------*/
	info = pvm_initsend( PvmDataDefault );
	//info = pvm_pkint(&pindex,1,1);//acrescentado em 17/08/2005
	info = pvm_pkint(isend1,nitens1,1 );
	info = pvm_pkint(&aux,1,1);
	info = pvm_pkdouble(psend,aux,1 );
	info = pvm_send(tids[0],msgtype1);
	return;
}

