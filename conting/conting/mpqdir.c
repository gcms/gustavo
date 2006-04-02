/*----------------------------------mpqdir.c-------------------------------------*/

/* Funcao para simular contingencia multipla [ Metodo Mid Compensation ]

----------------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"global.h"


/*---------------------Inicio da funcao contingpq_multiple------------------------*/



void contingpq_multiple(double ce[],double cle[],int kk, int mcont,int impr){
	int i,j,k,ir;
	int k1,k2,k3,jj,l,linha,ncoluna,aux;
	double fc1[nbmax],f1[nbmax],deltaf1[nbmax];
	double fc2[nbmax],f2[nbmax],deltaf2[nbmax];
	double prod[mmax];
	double C1[mmax][mmax],WTH1[mmax][mmax],WB1[mmax];
	double Wtil1[nbmax][mmax],W1[nbmax][mmax];
	double C2[mmax][mmax],WTH2[mmax][mmax],WB2[mmax];
	double Wtil2[nbmax][mmax],W2[nbmax][mmax];
	double w[nbmax];
	int info;
	double dta[nbmax], dva[nbmax];

	double wjk1[nbmax], wjk2[nbmax];
	/*---------Inicializa vetores e matrizes-------------*/
	for(l=0; l<nb; l++)
		w[l] = 0.e0;
	
	for(k1=0 ;k1<nb; k1++){
		wjk1[k1] = 0.e0;
		wjk2[k1] = 0.e0;
		fc1[k1] = 0.e0;
		fc2[k1] = 0.e0;
	}
	
	for(k1=0; k1<mmax; k1++){
		WB1[k1] = 0.e0;
		WB2[k1] = 0.e0;
	}
	
	for(k1=0; k1<nb; k1++){
		for(k3=0; k3<mmax; k3++){
			W1[k1][k3] = 0.e0;
			W2[k1][k3] = 0.e0;
			Wtil1[k1][k3] = 0.e0;
			Wtil2[k1][k3] = 0.e0;
			W1t[k3][k1] = 0.e0;
			W2t[k3][k1] = 0.e0;
		}
	}
	for(k1=0; k1<mmax; k1++){
		for(k3=0; k3<mmax; k3++){
			WTH1[k1][k3] = 0.e0;
			C1[k1][k3] = 0.e0;
			WTH2[k1][k3] = 0.e0;
			C2[k1][k3] = 0.e0;
		}
	}
	/*-----------  Guarda informacoes do caso base----------------------*/
	/*-----------
	Menu Explicativo:
	info = 0 => guarda as informacoes
		do caso base 
	info = 1 => recupera as informacoes
		do caso base
	----------------------------------*/
	info = 0;
	ricupero(info);
	/*------------------------ITERACAO ATIVA/REATIVA-----------------*/
	/*-----Geracao de vetores e matrizes (ramo-a-ramo para cada caso)*/
	for(jj=1; jj<=mcont; jj++){	
	/*-------------Identifica ramo j--k do caso contingenciado-------*/
		ir = kcont[kk][jj];
		i = ir - 1;      
		j = ni[i];
		k = nf[i];
		icont = kk;
	/*----------------------------------------------------------
	Verifica se o ramo j--k e trafo ou linha ==> define ejk1[]
	--------------------------------------------------------*/
		identdc(j,k);  // definicao em funcao dos tipos de barras (B')
		wjk1[j] = ej1;
		wjk1[k] = ek1;
		identca(j,k,i);  // definicao em funcao dos tipos de barras (B'')
		wjk2[j] = ej2;
		wjk2[k] = ek2;
		/*Obtem caminhos parciais e faz uniao (do metodo dos vetores esparsos)*/
		caminho(j,k);
		/*---Solucao pelo metodo dos vetores esparsos-----*/
		soluffs(ce,wjk1);   // faz somente "FFS"	
		soluffs(cle,wjk2);  // faz somente "FFS"	
		/*-----Gera as matrizes Wtil1 e Wtil2 (ordem nb x m)----------*/
		aux = jj-1;	
		for(l=0; l<nb; l++){
			Wtil1[l][aux] = wjk1[l];
			Wtil2[l][aux] = wjk2[l];
			wjk1[l] = 0.e0;
			wjk2[l] = 0.e0;
		}
	/*----	Define o vetor independente fcl[]----------*/
		fc1[j] = fpjk[i];
		fc1[k] = -fpjk[i];
	/*-	Altera os elementos da matriz Yo=Go + j.Bo em fucao da contingencia-*/
	/*------------Modifica Yo: -----------*/
		modify(j,k,i);
	/*--------- Define matrizes diagonais (de alteracao da rede) ----*/
		WB1[aux] = 1.e0/(fcont*b1kl[i]); // retirada do ramo ==> fcont=1
		WB2[aux] = -xjk[i]/fcont;    // retirada do ramo ==> fcont=1
	}//fim for
	
	/* Obtem as matrizes W1 e W2 a partir de Wtil1 e Wtil2 (e as transpostas : W1t e W2t) -----*/
	for(k1=0; k1<mcont; k1++) {
		for(k2=0; k2<nb; k2++){
			W1[k2][k1] = (1.e0/ce[k2])*Wtil1[k2][k1];
			W2[k2][k1] = (1.e0/cle[k2])*Wtil2[k2][k1];
			W1t[k1][k2] = W1[k2][k1];  // W1 transposta
			W2t[k1][k2] = W2[k2][k1];  // W2 transposta
		}
	}
	/*---Realizando o produto (Wtil Transposta) x W para obter WTH--*/
	/*-- Geracao da matriz WTH1 de Thevenin       -------------*/
	multmm3(W1t,Wtil1,mcont,nb,WTH1);
	/*-----------Geracao da matriz WTH2 de Thevenin-------------*/
	multmm3(W2t,Wtil2,mcont,nb,WTH2);
	for(linha=0; linha<mcont; linha++){
		WTH1[linha][linha] = WTH1[linha][linha] + WB1[linha];
		WTH2[linha][linha] = WTH2[linha][linha] + WB2[linha];
	}
	/*--------------------------Gera as matrizes C' e C''---------*/
	for(ncoluna=0; ncoluna<mcont; ncoluna++){
		for(linha=0; linha<mcont; linha++){
			C1[linha][ncoluna] = WTH1[linha][ncoluna];
			C2[linha][ncoluna] = WTH2[linha][ncoluna];
		}
	}
	// Inversa d matriz C1 (ordem nxm) por pivotiamento (Metodo p/ matriz simetrica tipo rede)
	/*--------Singularidade da matriz C1 ==> ILHAMENTO ----*/
	inversa(C1,mcont);     // inversa retornada em C1
	/*--------- Teste de ILHAMENTO (Singularidade de WB1 + WH1)--------*/
	if(kdiag == -1){
		//printf("\n OCORREU ILHAMENTO !!! \n");
		kilha[nilha] = kk;  // vetor que guarda os ilhamentos ocorridos
		nilha++;
	}
	else{
		kdiag = 0;
		inversa(C2,mcont);
		/*---------Prepara para fazer compensacao ----------*/
		multmm2(Wtil1,C1,nb,mcont,RESULT1);// Retorna resultado em RESULT1
		multmm2(Wtil2,C2,nb,mcont,RESULT2);  // Retorna resultado em RESULT2
		/*---------------------------------------------------------
		Processo de solucao por mid-compensation (ATIVO)
		Solucao do sistema linear- P-TETA para MISMATCH ATIVO
		Solucao do sistema L'.D'.fcl=dp
		------------------------------------------------------*/
		solufs(ce,fc1);    // Faz FORWARD SUBSTITUION Convencional
		/*-----------------------------------------
		Calculo do vetor de compensacao deltaf1[]
		----------------------------------------*/
		multmv4(W1t,fc1,mcont,nb,prod);
		multmv2(RESULT1,prod,nb,mcont,deltaf1);
		/*---------------Compensando ...--------------*/
		for(l=0; l<nb; l++){
			if(itipo[k] != 2){
				f1[l] = fc1[l] - deltaf1[l];
				dta[l] = f1[l];
			}
		}
		/*-------------------
		Completando a solucao da rede ...
		Resolve o sistema U'.dta = f1
		----------------------------------*/
		solubs(ce,dta);
		/*-----------------------------------------
		Atualiza o Vetor de Angulos Nodais TETA
		----------------------------------------*/  
		for(k=0; k<nb; k++){
			if(itipo[k] != 2){
				tet[k] = tet[k]+dta[k];
				t[k] = tet[k];
			}
		}
		/*----FINALIZANDO A ITERACAO REATIVA -------*/
		/*--------
		Calcula o produto matricial M.dta 
		(M e derivada de uma sub-matriz Jacobiana)
		---------------------*/
		prodMdelta_teta (dta,w,kk,mcont);
		/*------------------------------------------------------
		Define o vetor independente dva[](dq - M.dta = B''.dv)
		----------------------------------------------------*/
		for(jj=1; jj<=mcont; jj++){
		/*----Identifica ramo j--k do caso contingenciado---*/
			ir = kcont[kk][jj];
			i = ir - 1;      
			j = ni[i];
			k = nf[i];
		/*----------Sub-problema Q-V:--------------*/
			fc2[j] = fqjk[i];
			fc2[k] = -fqjk[i];
			fc2[j] = fc2[j] - w[j];
			fc2[k] = fc2[k] - w[k];
		}
		/*------Processo de Solucao pro Mid-compensation (REATIVO)
		Solucao do sistema linear Q-V para MISMATCH REATIVO
		Solucao so sistema L''.D''.fc2 = dq
		--------------------------------------------------------*/
		solufs(cle,fc2);   // faz FORWARD SUBSTITUTION Convencional
		/*----------	Calculo do vetor de compensacao deltaf2[]-----*/
		multmv4(W2t,fc2,mcont,nb,prod);
		multmv2(RESULT2,prod,nb,mcont,deltaf2);
		/*---------------Compensando...--------------*/
		for(l=0; l<nb; l++){
			if(itipo[l] == 0){
				f2[l] = fc2[l]- deltaf2[l];
				dva[l] = f2[l];
			}
		}
		/*------------------Completando a solucao da rede...
			Resolve o sistema U''.dva=f2
		------------------------------*/
		solubs(cle,dva);
		/*----------Atualiza o Vetor de Magnitudes Nodais V ------*/
		for(k=0; k<nb; k++){
			if(itipo[k] == 0)
			v[k] = v[k] + dva[k];  // atualiza as tensoes
		}
		/*-----Calculo do indice de severidade (PIv ou PICv)------*/
		//sobretensao();    // indices de violacao de limites de tensoes
		 sobrecarga();     // indices de violacao de limites de fluxos
		// printf("rval  = %f \n",rval);
		pia[kk] = rval; // indice de severidade
		//  printf("Indice de severidade  pia[%i]= %f \n Número rval = %f  \n",kk,pia[kk], rval);
		/*----------------------------------------------
		Saida da analise de contingencia (diagnostico e/ou estado/fluxos)
		-------------------------------------------*/ 
		//	if(impr == icont)
		//	  salida(impr);
		
	}//fim else
	
	/*-------------------------------------
	Restabelece informacoes do caso base
	-----------------------------------*/
	info = 1;
	ricupero(info);
	return;
}
/*--------------Fim da funcao contingpq_multiple-------*/
/*-----------------Inicio da funcao refatorapq----------------*/
// Funcao para simular contingencia multipla [ Metodo Atualizacao de Fatores ]
void refatorapq(double ce[],double cle[],int kk,int mcont,int impr){
	int k,k1,i,j,jj,l,ir;
	double cebase[max],clebase[max],ekm[nbmax];
	double w[nbmax];
	double WB1[1],WB2[1];
	int info;
	double dta[nbmax], dva[nbmax];
	/*--------Guarda informacoes do caso base--------------------*/
	/*---------------------------
	Menu Explicativo:
	info = 0 => guarda as informacoes
		do caso base 
	info = 1 => recupera as informacoes
		do caso base
	-----------------------------------*/	    
	info = 0;
	ricupero(info);
	/*--------Inicializa variaveis e vetores-----------*/
	WB1[0] = 0.e0;
	WB2[0] = 0.e0;
	for(l=0; l<nb; l++)
		w[l] = 0.e0;

	for(k1=0; k1<nb; k1++)	{
		dta[k1] = 0.e0;
		dva[k1] = 0.e0;
	}
	/*-------------	Guarda matrizes B' e B'' do caso base	---------*/
	for(k1=0; k1<max; k1++){
		cebase[k1] = ce[k1];     // diagonal e off-diagonal
		clebase[k1] = cle[k1];   // diagonal e off-diagonal
	}
	/*---------- ITERACAO ATIVA/REATIVA ------------------*/
	/*---------Geracao de vetores e matrizes (ramo-a ramo para cada caso)--*/
	for(jj=1; jj<=mcont; jj++){
	/*-------------Identifica ramo j--k do caso contingenciado---*/
		ir = kcont[kk][jj];
		i = ir-1;    
		j = ni[i];
		k = nf[i];
		icont = kk;
	/*--Verifica se o ramo j--k e trafo ou linha ==> define ejk1[]---*/
		identdc(j,k);   // definicao em funcao dos tipos das barras (B')
		identca(j,k,i);   // definicao em funcao dos tipos das barras (B'')
	/*------Obtem caminhos parciais e faz uniao (do metodo dos vetores esparsos) ---*/ 
		caminho(j,k);   // une os caminhos dos "singletons"
	/*---Define os elementos de alteracao da rede (admitancia) ----------*/
		WB1[0] = fcont*b1kl[i];  // retirada de ramo ==> fcont=1
		WB2[0] = -fcont/xjk[i];  // retirada de ramo ==> fcont=1
	/*-----Atualiza os fatores da matriz B' ---------*/
		ekm[j] = ej1;
		ekm[k] = ek1;
		bennett(cebase,ekm,WB1);
	/*------Atualiza os fatores  da matriz B''---------*/
		ekm[j] = ej2;
		ekm[k] = ek2;
		bennett(clebase,ekm,WB2);
	/*---------Define o vetor independente dta[] -----------------*/
		dta[j] = fpjk[i];
		dta[k] = -fpjk[i];
	/*----Altera os elementos da matriz Yo= Go + j.Bo------------*/
	/*---------Modifica Yo:------------*/
		modify(j,k,i);
	}//fim for
	/*-Teste de ILHAMENTO (Singularidade dos fatores de B' ou de B'')-----*/
	
	if(kdiag == -1){
	// printf("\n OCORREU ILHAMENTO !!! \n");
		kilha[nilha] = kk;  // vetor que guarda os ilhamentos ocorridos
		nilha++;
	}
	else{
		kdiag = 0;
	/*-------------------------------------------------------
		Processo de solucao pro atualizacao de fatores
		Solucao do sistema linear P-TETA para MISMATCH ATIVO
		Solucao do sist. linear B'.dta=dp
	-----------------------------------------------------*/
		solu(cebase,dta);
	/*-------------atualiza o Vetor de Angulos Nodais TETA------------*/
		for(k=0; k<nb; k++){
			if(itipo[k] != 2){
			tet[k] = tet[k] + dta[k];
			t[k] = tet[k];
			}
		}
	/*------  FINALIZANDO A ITERACAO REATIVA    -----------*/
	/*----  Calcula o produto matricial M.dta (M e derivada de uma sub-matriz Jacobina)-----*/
		prodMdelta_teta(dta,w,kk,mcont);
	/*------Define o vetor independente dva[] (dp - M.dta = B''.dv)---*/
		for(jj=1; jj<=mcont; jj++){
	/*-----------Identifica ramo j--k do caso contingenciado-----------*/
			ir = kcont[kk][jj];
			i = ir - 1;      
			j = ni[i];
			k = nf[i];
	/*---------------Sub-problema Q-V:-----------------*/
			dva[j] = fqjk[i];
			dva[k] = -fqjk[i];
			dva[j] = dva[j] - w[j];
			dva[k] = dva[k] - w[k];
		}
	/*-------Processo de solucao por atualizacao de fatores
		Solucao do sistema linear Q-V para MISMATCH ATIVO
		Solucao do sist. linear B''.dva = dq
	--------------------------------------------------------------*/
		solu(clebase,dva);
	/*---------- Atualiza o vetor de Magnitudes Nodais V ---------*/
		for(k=0; k<nb; k++){
			if(itipo[k] == 0)
				v[k] = v[k] + dva[k];
		}
	/*----Calculo do indice de severidade (PIv ou PICv) --------------*/
		sobretensao();    // indices de violacao de limites de tensoes
	// sobrecarga();     // indices de violacao de limites de fluxos
	// printf("rval  = %f \n",rval);
		pia[kk] = rval; // indice de severidade
	//  printf("Indice de severidade  pia[%i]= %f \n Número rval = %f  \n",kk,pia[kk], rval);
	/*------------------------------------------------------------------
		Saida da analise de contingencia (diagnostico e/ou estado/fluxos)
	---------------------------------------------------------------*/
	// if(impr == icont)
	//salida(impr);
	}//fim else
	/*-------------Restabelece informacoes do caso base------------*/
	info = 1;
	ricupero(info);
	return;
}      // Fim de refatora 1P-1Q
/*--------------Fim da funcao refatorapq---------------------------*/
