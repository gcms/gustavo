/* --------------------- entrada.c ---------------------------------------------
Este arquivo contem as funcoes responsaveis pela leitura de dados no formato
PECO, e sao definidas como se segue:
     1) ledado()  - Funcao que gerencia a leitura de dados e faz
                    a preparacao de estruturas esparsas iniciais;
     2) menu()    - Funcao que apresenta um menu de entrada na tela;
     3) lebarra() - Funcao que faz a leitura de dados de barra;
     4) lerede()  - Funcao que faz a leitura de dados de rede
			       (dados de linhas e transformadores);  
     5) nin()     - Funcao que transforma numeracao externa em interna (barras).
Observacoes: a) Os nomes das barras nao devem conter espaco em branco entre 
                os caracteres. 
             b) Os dados em cada linha do arquivo de dados sao lidos como dados 
                de grandezas distintas se houver um espaco em branco, por isso, 
                cada coluna de dado deve estar separada por espaco em branco.
-------------------------------------------------------------------------------- 
LEGENDA: Valores que itipo() pode assumir:
             0 - barra tipo PQ
             1 - barra tipo PV
             2 - barra tipo SLACK 
--------------------------------------------------------------------------------                
ARQUIVOS DO TIPO .txt LIDOS:  valores.txt
                              dad.txt
--------------------------------------------------------------------------------
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
#include "global.h"     //arquivo contem as declaracoes de variaveis globais
                        //arquivo vari.h contem as mesmas declaracoes - espelho
	
/*------------------Prototipos de funcoes-------------------------------------*/
	void menu(void);
	int nin(int);
	void lebarra(void);
	void lerede(void);
	
/*----------------------------------------------------------------------------*/
	FILE *filePtr;   // Ponteiro para o arquivo dad.txt que sera lido
	FILE *filePnt;   // Ponteiro para o arquivo valores.txt que sera lido
	char nome[15];
	char name[15];
	
/*-------------------Inicio da funcao ledado----------------------------------*/
void ledado(void){ 
    int i,dd=0,k,tamLeitura,cc[2];
	double da[4]; 

/*----------------Chamando funcao menu de entrada ----------------------------*/
	menu();
	
/*---------------Abre arquivo de parametros e testa se ha erro ---------------*/
	if ((filePnt = fopen("valores.txt","r")) == NULL){
		printf("\n  Este arquivo nao pode ser aberto ... \n");
		exit(1);
	}
	else{
	/*-------------Faz leitura de parametros para o programa (controles)------*/
	/*---------------Le o primeiro valor do primeiro registro ----------------*/
	tamLeitura = fscanf(filePnt," %d ",&dd );
	i = 0;
	while(i <= 4 && dd != 9999 && tamLeitura != 0){
        
	/*----------------Leitura das demais informacoes do registro -------------*/
           fscanf(filePnt," %d %lf \n",&dd,&da[i]);
    /*------------------------Faz nova leitura -------------------------------*/
           tamLeitura = fscanf(filePnt," %d ",&dd );
		   i++;
	}
	tola  = da[0];
	tolr  = da[1];
	dtap  = da[2];
	range = da[3];
	alfa  = da[4];
    
	/*---------------Le os demais valores do registro ------------------------*/
	tamLeitura = fscanf(filePnt," %d ",&dd );
	i = 0;
	while(i < 10000 && dd != 9999 && tamLeitura != 0 ){
          
	/*----------------Leitura das demais informacoes do registro -------------*/
	
           fscanf(filePnt," %d \n",&cc[i]);
           
           /*-------------------Fazer nova leitura ---------------------------*/
           tamLeitura = fscanf(filePnt," %d ",&dd );
           i++;
	}
	nitqm  = cc[0];
	limite = cc[1];
	
	/*---------------Fecha o arquivo -----------------------------------------*/
	fclose(filePnt);
    }
	
/*---------------Abre arquivo de leitura e testa se ha erro ------------------*/
	if ((filePtr = fopen("dad.txt","r")) == NULL){
		printf("\n  Este arquivo nao pode ser aberto!! \n");
		exit(1);
	}
	else{
	/*-------------Faz leitura de dados de barra e de rede -------------------*/
		lebarra();
		lerede();
	}
	/*---------------Fecha o arquivo de leitura ------------------------------*/
	fclose(filePtr);
	return;
}
/*--------------------Fim de ledado()-----------------------------------------*/
	
	
/*-------------------------Inicio de menu()-----------------------------------*/
	
void menu(void){
	printf("       \n");
	printf("       \n");
	printf(" ------------------------------------------------------------- \n");
	printf(" |         PROGRAMA DE CALCULO DE FLUXO DE POTENCIA          | \n");
	printf(" |               Algoritmo Desacoplado Rapido                | \n");
	printf(" |                 Controles Implementados                   | \n");
	printf(" |                   [ NOVEMBRO DE 2006 ]                    | \n");
	printf(" |              Antonio Cesar Baleeiro Alves                 | \n");
	printf(" ------------------------------------------------------------- \n");
	printf("      \n");
	printf("      \n");
	return;
	}
/*----------------------------Fim de menu()-----------------------------------*/
	
/*----------------------Inicio de lebarra()-----------------------------------*/
/*--------------------Le dados das barras-------------------------------------*/
	void lebarra(void){
	int  jex,k,iti,i,ii,inact,tamLeitura,npq,npv,nsl;
	double pc,pg,vx,tt,qg,qgmi,qgma,qc,bsh;
	
    /* --------------------------------------------------
    /*  Potencia Base em MVA:
    /*  SB = 100 MVA (definida em global.h e vari.h)
    ---------------------------------------------------*/
	/*--------------Inicializacoes--------------------------------------------*/
	k = 0;
	npq = 0;
	npv = 0;
	nsl = 0;
	carga = 0.e0;
	
	
	/*---------------Le o primeiro valor do primeiro registro-----------------*/
	tamLeitura = fscanf(filePtr,"%d",&jex );
	i = 0;
	while (i < 10000 && jex != 9999 && tamLeitura != 0){
	
	/*----------------Leitura das demais informacoes do registro -------------*/
	
		fscanf(filePtr,"%d %d %s %lf %lf %lf %lf %lf %lf %lf %lf %lf \n",
		&inact,&iti,&nome,&vx,&tt,&pg,&qg,&qgmi,&qgma,&pc,&qc,&bsh );	
	/*----------------------Armazenando em vetores valores lidos--------------*/
		itipo[i] = iti;
		itipob[i] = iti;   // importante na implementacao dos controles
		nex[i] = jex;
		
	/*-----------------Inicializa com estado Teta nulo------------------------*/
	    tet[i] = 0.e0;
	    
	/*-----------------Inicializa com estado V unitario-----------------------*/
		v[i] = 1.e0;
		ves[i] = vx;
		if (iti == 1){
           v[i] = vx;
        }
		
	/*-----------------Injecoes de Potencias ---------------------------------*/
		
		pes[i] = (pg - pc)/SB;    // Potencia ativa especificada
		qes[i] = (qg - qc)/SB;    // Potencia reativa especificada
		pca[i] = pc/SB;           // Potencia ativa de carga
		qca[i] = qc/SB;           // Potencia reativa de carga

        if(iti == 2){
           nslack[k] = i;
           k++; 
         }
         carga = carga + pc;
		
		/*------------------Armazena parametros da rede-----------------------*/
		bdiag[i] = bsh/100.e0;
		shu[i]   = bsh/100.e0;        // shunt (% no banco de dados)
		shu0[i]  = bsh/100.e0;        // shunt (importante para controle)
	
	    /*------------------Limites de Geracao Reativa------------------------*/
		qgmax[i] = qgma/SB;
		qgmin[i] = qgmi/SB;
		
		/*------------------Contando as barras por tipo-----------------------*/
		if(iti == 1) npv++;                 //contador de barra tipo PV
		
        if(iti == 0) npq++;                //contador de barra tipo PQ
        
		if(iti == 2) nsl++;                //contador de barra tipo Slack	
		
        /*-------------------Fazer nova leitura ------------------------------*/
		tamLeitura = fscanf(filePtr,"%d",&jex );
		i++;
	}                         //  fim do enquanto jex!=9999  
	/*----Armazenando o numero de barras e o numero de geradores--------------*/
	nb = i;
	ng = npv + nsl;
	return;
}
/*--------------------- Fim de lebarra()--------------------------------------*/

/*-----------------------Inicio de lerede()-----------------------------------*/
/*----------------------Le dados de ramos da rede-----------------------------*/
void lerede(void){
	int lni,lnf,tamLeitura,m,k,i;
	double aa,xx,r,ysh,t1,ta,ta1,ta2,fnorm,femer;
	for(k = 0; k < nb; k++)
		gdiag[k] = 0.e0;
		
	for(k = 0; k < nl; k++){
		tapi[k] = 0.e0;
		taps[k] = 0.e0;
		tap[k] = 0.e0;
		tapa[k] = 0.e0;
        }
		
	/*------------------------Ler o primeiro registro ------------------------*/
	tamLeitura = fscanf(filePtr,"%d",&lni);
	i = 0;
	while (i<10000 && lni!=9999 && tamLeitura !=0){
		tamLeitura = fscanf(filePtr,"%d %lf %lf %lf %lf %lf %lf %lf %lf \n",
			&lnf,&r,&xx,&ysh,&ta,&ta1,&ta2,&fnorm,&femer);
	
	/*----------Cria vetores de barras terminais de ramos:--------------------*/
		ni[i] = nin(lni); // transforma numeracao externa em interna no inicial
		nf[i] = nin(lnf); // transforma numeracao externa em interna no final
		
	/*------------------------------Parametros de circuito da rede:	----------*/

		r = r/100.e0 ;       // resistencia (no BD esta em porcentagem)
		xx = xx/100.e0;      // reatancia serie (no BD esta porcentagem)
		aa = r*r + xx*xx;
		
/*---Elementos da Matriz Admitancia Nodal---Susceptancia Nodal ---------------*/
		gkl[i] = r/aa;       // condutancia serie do ramo kl
		bkl[i] = -xx/aa;     // susceptancia serie do ramo kl 

/*-----Estamos interessados no MODELO AC -------------------------------------*/
/*-----Na formacao de Y (ou B) incluimos "shunts" e taps -------------------- */

/*---Elementos das Matrizes B' e B"-------------------------------------------*/
		b1kl[i] = bkl[i];	    // versao BX
		b2kl[i] = -1.e0/xx;
		
//		b1kl[i] = -1.e0/xx;    // versao XB
//		b2kl[i] = bkl[i];
		
/* Para o sistema CELG, a versao BX em geral resulta 
/* em menor numero de iteracoes  */

        ysh = ysh/100.e0;     // (no BD esta em %)
		ylin[i] = ysh/2.e0;   // elemento do modelo PI da LT 
		
/*-----Armazena taps em vetores reais-----------------------------------------*/
        tapi[i] = ta1;  // limite inferior
        taps[i] = ta2;  // limite superior	
        tap0[i] = ta;   // valor inicial do tap
        tap[i]  = ta;   // tap que sera alterado nos controles
        tapa[i] = ta;   // vetor que armazena o tap da iteracao anterior	
		
/*-----Numeracao interna das barras dos extremos do ramo i--------------------*/
		k = ni[i];
		m = nf[i];
		
/*------ltc() = 1 significa que o transformador tem tap variavel--------------*/
		
		ltc[i] = 0;
		if(ta1 != 0.e0 && ta2 != 0.e0 ) ltc[i] = 1;
			
        if(ta == 0.e0){
		   gdiag[k] = gdiag[k] + gkl[i];
		   gdiag[m] = gdiag[m] + gkl[i];
		   bdiag[k] = bdiag[k] + bkl[i] + ylin[i];
		   bdiag[m] = bdiag[m] + bkl[i] + ylin[i];
		   shu[k] = shu[k] + ysh/2.e0;
		   shu[m] = shu[m] + ysh/2.e0;     //shunt de linha
		   }
		if(ta != 0.e0){
           t1 = 1.e0/ta;
		   bdiag[k] = bdiag[k] + bkl[i]*t1*t1;
		   bdiag[m] = bdiag[m] + bkl[i];
		   gdiag[k] = gdiag[k] + gkl[i]*t1*t1;
		   gdiag[m] = gdiag[m] + gkl[i];
		   shu[k] = shu[k] + bkl[i]*t1*(t1 - 1.e0)/2.e0;
	       shu[m] = shu[m] + bkl[i]*(1.e0 - t1)/2.e0;
	       gkl[i] = gkl[i]*t1;
           bkl[i] = bkl[i]*t1;
           }	
/*---------------------	Le o proximo registro---------------------------------*/
		tamLeitura = fscanf(filePtr,"%d", &lni);
		i++;
	}
	/*------------------------Armazenando o numero de linhas------------------*/
	nl = i ;
	return;
}
/*------------------------------ Fim de lerede()------------------------------*/
	
	
/*------------------------------Inicio da funcao nin()------------------------*/
int nin(int k){
	
	int i = 0;
	
	while (i < nb && nex[i] != k)
		i++;
	    if(i >= nb){
	   	   printf("\n  Nao foi encontrada a barra ... %d  \n",k);
           exit(1);
	       }
	return(i);	
}
/*------------------------------Fim da funcao nin()---------------------------*/
