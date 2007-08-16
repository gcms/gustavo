/*-------- mestre.c ----------------------*/
/* -----------------Inicio da funcao MestreA--------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "prototipos.h"
#include "pvm3.h"

void  MestreA(double ce[max],double cle[max],int impr){

	int  nsize1[63],nsize2[63],nsize3[63],nsize4[63]; //local
	//int  jklasif[nlmax];          // local
	int  k,i,j;
/*----------------  Declaracao relativa ao PVM   -------------------*/
	int knt,info, bufid;
	int icont0,icontr,kount,ip,nrcaso,nofinal;
	int nbase,nilhasoma,ndivsoma,nnconvsoma,nyconvsoma;
	int nitens3,nitens4,nitens5,nitens6;
	int msgtype2,msgtype3,msgtype4,msgtype5,msgtype6;
	int nndim, casoAnalizado;
/* ---------------------------------------
Rotina MestreA(): Modo Paralelo Assincrono
-----------------------------------------
-----------------------------------------------
 PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
---------------------------------------------
--------------------------------
 Rotulos das mensagens [ types ]
------------------------------*/
	msgtype2 = 31;
	msgtype3 = 41;
	msgtype4 = 51;
	msgtype5 = 61;
	msgtype6 = 71;
	nbase = NPROC;
	nofinal = NPROC-1;
	nilhasoma = 0;
	ndivsoma = 0;
	nnconvsoma = 0;
	nyconvsoma = 0;
	/*------- Inicio do Processamento da Contingencia AC Lista Reduzida--------
		PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
	PROCESSAMENTO PARALELO ASSINCRONO
	-----------------------------------------------
	------------------------------------------------
	O LOOP AC e Executado em Paralelo Assincrono: O
	Processador Assim que Termina seu Caso Pega  um
	Novo Caso para Executar
	------------------------------------------------
	O Controle e Feito Somente pelo No "0" que Tam-
	bem Executa Calculos AC Enquanto Espera
	----------------------------------------------*/
	if ( nbase == 1 ){
		for(k=0; k < ncrit; k++){
			casoAnalizado = klasif[k]; // caso a ser analisado
			//printf("Caso a ser analizado => %d",casoAnalizado);
			//printf("\n Ioutlocal em mestrea =  %d", ioutlocal);
			contingenciaAC(ce,cle,impr,casoAnalizado,k);// esta rotina faz chamadas da contingencia AC
		}
	}
	if ( nbase > 1 ){
	/*---- Preparando a Carga dos Processadores  ------*/
		if(nrc < nbase){
			nndim = nrc;
			printf("Toda a lista e menor do que o no. de hosts !!\n");
			printf("REDUZA O TAMANHO DA VM PARA %d\n",nndim);
			return;
		}
		kdif = 0;
		if(ncrit < nbase){
			printf("Voce tem menos casos do que o no. de hosts !!\n");
			kdif = NPROC - ncrit; // no. de casos artificiais para completar a carga
		}
		/*for(i=0; i<nrc; i++)
			jklasif[i] = klasif[i];*/

		/*for(i=kdif+ncrit; i<nrc; i++)
			jklasif[i] = -1;*/

		kount = 0;    // inicializa contador geral de casos
		icont0 = 0;   // inicializa contador de casos processados em "0"
		icontr = 0;   // inicializa contador de casos recebidos de outros hosts != "0"
/*---------------------- Manda os primeiros casos---------------------*/
		for(i=1; i<=nofinal; i++){
			//if(kount < ncrit){
				//kbusy[0] = jklasif[kount];
				kbusy[0] = klasif[kount];
				kbusy[1] = kount;
				kount++;
				icontr++;
				info = pvm_initsend(PvmDataDefault);
				info = pvm_pkint(kbusy,2,1);
				info = pvm_pkint(&ncrit,1,1);
				//info = pvm_pkint(klasif,ncrit,1);
				info = pvm_send(tids[i],msgtype2); // hostdest = i (i= 1,2,3,...,nofinal)
			//}
		}
		/*------No "0" pega 1 caso para analisar--------------*/
		if(kount < ncrit){
			casoAnalizado = klasif[kount];  // caso a ser analisado
			contingenciaAC(ce,cle,impr,casoAnalizado,kount);  // esta rotina faz chamadas da contingencia AC
			kount++;
			icont0++;
		}

		//while(casoAnalizado != -1){
		while(kount < ncrit + NPROC - 1  ){
			//----No "0" verifica o buffer---------
			knt = pvm_nrecv(-1,msgtype3);
			if(knt > 0){
				//printf("\n Verificando o Buffer");
				pvm_upkint(kbusy,2,1);
				ip = kbusy[0];    // tid do hostdest = ip
				nrcaso = kbusy[1];
				icontr++;
				//--Envia novo caso para "ip" analisar-------
				kbusy[0] = klasif[kount];
				kbusy[1] = kount;
				pvm_initsend( PvmDataDefault );
				pvm_pkint(kbusy, 2, 1);
				pvm_send(ip,msgtype4); // envia um caso para o escravo que terminou
				kount++;
			}
			else {
				if ( kount < ncrit){
		  //----- No "0" pega 1 caso para analisar--------
					casoAnalizado = klasif[kount];   // caso a ser analisado
					contingenciaAC(ce,cle,impr,casoAnalizado,kount);  // esta rotina faz chamadas da contingencia AC
					kount++;
					icont0++;
				}
			}
		}
	  /*--------- Fim do Processamento da Contingencia AC Lista Reduzida-
            ------------------------------------------------
	    Pega Resultados da Simulacao de Contingencias do
	    Processador "0"
	    -----------------------------------------------*/
		nilhasoma = nilha;
		ndivsoma = ndiv;
		nnconvsoma = nnconv;
		nyconvsoma = nyconv;
	/*------------------------------------------------
	 Recebe Resultados da Simulacao de Contingencias 
	 dos Demais Processadores
	 -----------------------------------------------
	// == Ilhamentos/Divergencias/Nao-Convergencias/Convergencias/Iteracoes: */
		printf("\n NO  | ILHAMENTO | DIVERG. | CONVERG.  | NAO-CONVERG. |\n");
		printf("\n %2d  | %5d     |%5d    |  %5d    |    %5d     |\n",myinst,nilha,ndiv,nyconv,nnconv);
	
		for(i=1; i<=nofinal; i++){
		//printf("recebendo os vetores");
		//info = -5;
		//while(info<=0)
		bufid = pvm_nrecv( tids[i], msgtype5 );
		while(bufid<=0)
			bufid = pvm_nrecv( tids[i], msgtype5 );

		info = pvm_upkint(&isend1[0],1,1 );
		info = pvm_upkint(&isend2[0],1,1 );
		info = pvm_upkint(&isend3[0],1,1 );
		info = pvm_upkint(&isend4[0],1,1 );

		nsize1[i] = isend1[0]+1;
		nsize2[i] = isend2[0]+1;
		nsize3[i] = isend3[0]+1;
		nsize4[i] = isend4[0]+1;
		}
	for(i=1; i<=nofinal; i++){
		nitens3 = nsize1[i];
		nitens4 = nsize2[i]; 
		nitens5 = nsize3[i];
		nitens6 = nsize4[i]; 
		info = pvm_recv( tids[i], msgtype6);
		info = pvm_upkint(isend1,nitens3,1);
		info = pvm_upkint(isend2,nitens4,1);
		info = pvm_upkint(isend3,nitens5,1);
		info = pvm_upkint(isend4,nitens6,1);
		info = pvm_upkint(isend5,nitens6,1);
		info = pvm_upkint(isend6,nitens6,1);
		info = pvm_upkint(isend7,nitens6,1);
		nilha = isend1[0];
		ndiv = isend2[0];
		nnconv = isend3[0];
		nyconv = isend4[0];
		//nyconv = isend5[0];
		//nyconv = isend6[0];
		// Recuperando kilha()
		if ( nilha != 0 ){
			for(j=0; j<nilha; j++)
				kilha[j+nilhasoma] = isend1[j+1];
			nilhasoma = nilhasoma + nilha;
		}
		// Recuperando kdiverge()
		if ( ndiv != 0 ){
			for(j=0; j<ndiv; j++)
				kdiverge[j+ndivsoma] = isend2[j+1];
			
			ndivsoma = ndivsoma + ndiv;
		}
		// Recuperando knotconv()
		if ( nnconv != 0 ){
			for(j=0; j<nnconv; j++)
				knotconv[j+nnconvsoma] = isend3[j+1];
			nnconvsoma = nnconvsoma + nnconv;
		}
		      // Recuperando kconv(), kit1() e kit2()
		if ( nyconv != 0 ){
			for(j=0; j<nyconv; j++){
				kconv[j+nyconvsoma] = isend4[j+1];
				kit1[j+nyconvsoma] = isend5[j+1];
				kit2[j+nyconvsoma] = isend6[j+1];
				kordem[j+nyconvsoma] = isend7[j+1];
			}
			nyconvsoma = nyconvsoma + nyconv;
		}
	}//fim for
	nilha = nilhasoma;
	ndiv = ndivsoma;
	nnconv = nnconvsoma;
	nyconv = nyconvsoma;
	} // if ( nbase )

	return;

}

void  MestreScreening(double ce[max],double cle[max],int impr){
	int ll,kk,k,l;
	/*---------------------------
	 Declaracao relativa ao PVM
	------------------------*/
	int info,bufid,aux;
	int icont0,icontr,nofinal;
	int nbase;
	int nitens1;
	int msgtype1;
	/* ---------------------------------------
	  Rotina MestreA(): Modo Paralelo Assincrono
	 ------------------------------------------
	 PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
	--------------------------------------------
	--------------------------------
	Rotulos das mensagens [ types ]
	------------------------------*/
	msgtype1 = 21;
	nbase = NPROC;
	nofinal = NPROC-1;
	nitens1 = nrc + 1;
	/*---------------------------------
	Calculo do indice PI e ordenacao
	-------------------------------*/
	if( nbase == 1 ){
		/*--------------------------------------------
		Apenas o Processador "0" Executa [ serial ]
		------------------------------------------*/
		for(k=0; k<nrc; k++){
			//ioutlocal = ll; // caso a ser analisado
			scrdireto_reativo(ce,cle,impr,k);// esta rotina faz chamadas do metodo 1P-1Q
		}
	}
	else{
		/*-------------------------------------------
		Todos os Processadores Executam [ paralelo ]
		-------------------------------------------*/
		icont0 = 0;// inicializa contador de casos calculados pelo processador "0"
		for(k=myinst; k<nrc; k=k+nbase){
			//ioutlocal = kk;  // caso a ser analisado
			scrdireto_reativo(ce,cle,impr,k);// esta rotina faz chamadas do metodo 1P-1Q
			icont0++;
		}
		/*----------------------------------------------------------------
		1. Pega Resultados do  screening do Processador "0"
		2. Recebe Resultados dos Demais Processadores
		--------------------------------------------------------------*/
		for(k=1; k<=nofinal; k++){
			bufid = pvm_recv(tids[k],msgtype1);
			info = pvm_upkint(isend1,nitens1,1);
			info = pvm_upkint(&aux,1,1);
			info = pvm_upkdouble(psend,aux,1);
			icontr = aux-1;
			if ( icontr != 0 ){
				for(l=1; l<=icontr; l++){
					kk = isend1[l];  // no. do caso
					pia[kk] = psend[l];  // indice de severidade
				}
			}
		}
	}  // if ( nbase )
	
	reordena();// reordena a lista de indices de severidade
}
/*------------------ Fim da funcao MestreA-------------------*/
