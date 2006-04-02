/*---------------------------------pqdir.c--------------------------------*/
/*
  Funcao para simular contingencia simples [Metodo 1P-1Q DIRETO ]

  EXPLICACAO: Este metodo supoe que a contingencia do ramo j-k afeta somente os estados das 
              barras j e k, incrementalmente.

  Faz Fluxo 1P-1Q Compensado:
                                      1) Usa Armazenamento de Zollenkopf
                                      2) Vetores Esparsos
                                      3) Uma Iter. Ativa e uma Reativa do FDLF
                                      4) Usa LIM para Simular Alt. em B' e B''
                                      5) Metodo Conhecido como 1P-1Q
                                      6) Contingencia Simples de Ramo
                                      7) Exclusivo para Contingencia Simples
                                      8) Usa "Post-Compensation"

-----------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "global.h"
#include "prototipos.h"
/*---------------Inicio da Funcao cont_pqdir----------------------*/
void cont_pqdir(double ce[],double cle[],int kk,int mcont,int impr){
	int info;
	int k,k1,k2,i,j,l,ir;
	double w[nbmax],dta[nbmax],dva[nbmax];
        double wjk1[nbmax], wjk2[nbmax];
	double wt1,wt2,wb1,wb2,expressao1,expressao2,delta2;
	/*--------------Inicializa vetores ------------------*/
	for(l=0; l<nb; l++)
		w[l] = 0.e0;
	for(k1=0;k1<nb;k1++){
		wjk1[k1] = 0.e0;
		wjk2[k1] = 0.e0;
		dta[k1] = 0.e0;
		dva[k1] = 0.e0;
	}	
	/*------------ Guarda as informacoes do caso base ---------
    Menu Explicativo:
   info = 0 => guarda as informacoes
               do caso base 
   info = 1 => recupera as informacoes
               do caso base 
    -----------------------------------*/
	info = 0;
	ricupero(info);
/*------------ Identifica ramo j--k do caso contingenciado ---------------*/
	ir = kcont[kk][1];
	i = ir - 1;
	j = ni[i];
	k = nf[i]; 
	icont = kk;
	/*- Altera os elementos da matriz Yo = Go. + j.Bo em funcao da contingencia
	Modifica Yo para calcular fluxos ou injecoes de potencia  */
	modify(j,k,i);
	/*-----------ETAPA RELATIVA A ITERACAO ATIVA-------------*/
	/*----------Verifica se o ramo j--k e trafo ou linha ==> define ejk1[] -----*/
	identdc(j,k);       // definicao em funcao dos tipos de barras
        wjk1[j] = ej1;
	wjk1[k] = ek1;

	/*-------- Obtem caminhos parciais e faz uniao----------- */
	caminho(j,k);         // une os caminhos dos singletons
	/*-------  Solucao do sistema de equacoes B'.wjk1 = ejk1 --*/
	soluve(ce,wjk1);     // faz "FFS & NBS"
	/*solufs(ce,wjk1);
	solubs(ce,wjk1);*/
	/*------- Define um vetor independente dta[] ---------------*/
	/*-------------------- Sub-problema P-TETA ------------------*/
	dta[j] = fpjk[i];
	dta[k] = -fpjk[i];
	/*-------    Solucao do sistema de equacoes B'.dta=dp---------*/
	soluve(ce,dta);     // faz "FFS & NBS"
  /*---------   Obtem o equivalente de Thevenin da rede vista de j-k---------------*/
	wt1 = ej1*wjk1[j] + ek1*wjk1[k]; // wt1 - escalar impiedancia equiv. relativa a B'
	/*----------------- Define a mudanca em B' c/ a retirada do ramo j-k------*/
	wb1 = 1.e0/(fcont*b1kl[i]);         // retirada de todo ramo ==> fcont=1
   /*------Teste de ILHAMENTO (Comparar wt1 com a impiedancia de ramo wb1  --> retirada)*/
	if(fabs(wt1 + wb1) < 1.e-6){
		// printf("\n OCORREU ILHAMENTO !!! \n");
		kdiag = -1;
		kilha[nilha] = kk;
		nilha++;
	}
	else {
		kdiag = 0;
		/*---    Matriz C' (ordem mcont x mcont) c1p - escalar ----*/
		c1p = 1.e0/(wb1 +wt1);
		expressao1 = -c1p*(ej1*dta[j] + ek1*dta[k]);
		/*---  Calcula o angulo TETA pos-contingencia -----*/
		for(k2=0; k2<nb; k2++){
			dta[k2] = dta[k2] + wjk1[k2]*expressao1;
			tet[k2] = tet[k2] + dta[k2];
			t[k2] = tet[k2];
		}
		/*--------ETAPA RELATIVA A ITERACAO REATIVA ---------*/
		/*--------Verifica se o ramo j--k e trafo ou linha ==> define ejk2[]--*/
		identca(j,k,i);
		/*--  Solucao do sistema de equacoes B'' .wjk2 = ejk2-*/
                wjk2[j] = ej2;
	        wjk2[k] = ek2;
		
		soluve(cle,wjk2);      // faz "FFS & NBS"
		
		/*-- Calcula o produto matricial M.dta (M e derivada de uma sub-matriz jacobiana)----*/
		prodMdelta_teta(dta,w,kk,mcont);     // w=M.dta
		/*-----Define o vetor independente dva[] (dq - Mdta = B''.dv)-----*/
		/*----- Sub-problema Q-V: ---------------*/
		dva[j] = fqjk[i];
		dva[k] = -fqjk[i];
		dva[j] = dva[j] - w[j];
		dva[k] = dva[k] - w[k];
		/*-----------Solucao do sistema de equacoes B''.dva=dq---------*/
		soluve(cle,dva);    // faz "FFS & NBS"
		/*---------------Obtem o equivalente de Theveni da rede vista de j-k---*/
		wt2 = ej2*wjk2[j] + ek2*wjk2[k];// wt2 = escalar Impied. equiv. relativa a B''
		/*----------Define a mudanca em B'' c/ a retirada do ramo j-k---------*/
		wb2 = 1.e0/(fcont*b2kl[i]);     // retirada de todo ramo ==> fcont=1
		/*----------Matriz C'' (ordem mcont x mcont) c2p = escalar -------------*/
		c2p = 1.e0/(wb2 +wt2);
		/*-------------------Avalia a expressao -----------------*/
		expressao2 = -c2p*(ej2*dva[j] + ek2*dva[k]);
		/*------------------Calcula a magnitude V pos-contingencia------------*/
		for(k2=0; k2<nb; k2++){
			if(itipo[k2] == 0){
				delta2 = dva[k2] + wjk2[k2]*expressao2;
				v[k2] = v[k2] + delta2;
			}
		}
		/*---------Calculo do indice de severidade (PIv ou PICv)--------------*/
		sobretensao();//indices de violacao de limites de tensoes
		// sobrecarga(); //indices de violacao de limites de fluxos
		pia[kk] = rval; // indice de severidade
		/*---Saida da analise de contingencia (diagnostico e/ou estado/fluxos)-*/
		// if(impr == icont)
		//salida(impr);
	}//fim else
	info = 1;
	ricupero(info);
	return;
}

/*----------------------------------Fim da Funcao cont_pqdir----------------------------------*/
/*----------------------------------Inicio da funcao sobrecarga --------------------------------*/

/* [ Funcao para calculo de fluxos ativos no modelo DC ]
   [ Calcula a sobrecarga TOTAL para cada contingencia ]
   [ OBJETIVO:   Calcular Indices de Severidade        ]
   [        Real Power Performance Index               ]  

--------------------------------------------------------------------------------------- */


void sobrecarga(void){
	int j,k,m;
	double tkm,pkm,vainf,vasup,dife,sobre,maior;
	double co,si,vkk,vmm,vkm,t1,bb;
	sobre = 0.e0;
	rval = 0.e0;
	for(j=0; j<nl; j++){
		k = ni[j];
		m = nf[j];
		tkm = t[k]-t[m];
		co = cos(tkm);
		si = sin(tkm);
		vkk = v[k]*v[k];
		vmm = v[m]*v[m];
		vkm = v[k]*v[m];
		if( tap[j] == 0.e0 )
			t1 = 1.e0;
		else
			t1 = 1.e0/tap[j];
	
		bb = bkl[j]+ylin[j];
		fpkm[j] = t1*gkl[j] *vkk - vkm*(gkl[j]*co+bkl[j]*si);
		fpmk[j] =(gkl[j]/t1)*vmm - vkm*(gkl[j]*co-bkl[j]*si);
		
		if( abs(fpkm[j]) >= abs(fpmk[j]))
			pkm = fpkm[j];
		else
			pkm = fpmk[j];
		// 90% dos limites de fluxos
		vainf = fmi[j]*9.e-1;
		vasup = fma[j]*9.e-1;
		if(pkm > vasup){
			dife = pkm/vasup;
			sobre = sobre + dife * dife;
		}
		else if(pkm < vainf){
			dife = pkm/vainf;
			sobre = sobre + dife * dife;
		}
	}
	/*---------------  [ Sobrecarga Total ]  ------------------*/
	rval = sobre;
  return;
}

/*----------------------------------Fim da funcao sobrecarga -----------------------------------*/
// Data de Atualizacao: 16/08/2005
/*----------------------------------Inicio da Funcao violacao---------------------------------*/
/* [  Funcao para calculo de indices de violacao de tensoes    ]
   [  Calcula o desvio da tensao pos-contingencia em relacao   ]
   [  ao ponto medio de uma faixa de tolerancia ou a limites   ]
   [  OBJETIVO: Calcular Indices de Severidade de Contingencias]
   [            do Ponto de Vista das Violacoes de Tensoes em  ]
   [            Barras PQ
   ----------------------------------------------------------------------------------------------*/
void sobretensao(void){
	int i;
	double sobre,vinf,vsup,vm,desvio1,desvio2,wv,dife,maior;
	//  printf("<< violacao >>\n");
	sobre = 0;
	rval = 0;
	wv = 1.e4;
	for(i=0; i<nb; i++){
		if(itipo[i] == 0){
	/*-----------------------------------------------------------------
	Estimativa de limites para magnitude de tensao (indices PIv/PICv)
	---------------------------------------------------------------*/
			vinf = vb[i] - 0.05; // vb[] magnitude de tensao do caso base
			vsup = vb[i] + 0.05;
	  /*---------------------------------------------
            Desvio da magnitude de tensao  p/ indice PIVc
	    -------------------------------------------*/
			desvio1 = vsup - v[i];
			desvio2 = vinf - v[i];
		/*----------------------------------------------
        	Peso (parametro de ponderacao p/ indice PICv)
		--------------------------------------------
			//wv = 1.e4;
			//wv = 10000;
			//printf("\n wv = %d", wv);
		--------------------------------------
			Avaliando o somatorio PIv (expoente 4)
		------------------------------------*/
			if(v[i] > vsup){
				dife = desvio1/vsup;
				sobre = sobre + dife*dife;//*dife*dife; // indice PIv
			} 
			else{
				if (v[i] < vinf){
					dife = desvio2/vinf;
					sobre = sobre + dife*dife*dife*dife;
				}
			}
		}// fim if
	}// fim do
	/*-------------- Violacao total  ------------*/
	rval = wv*sobre;
	
	return;
}

/*------------Fim da Funcao sobretensao -----------------*/

