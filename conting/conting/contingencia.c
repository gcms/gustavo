/*----------------------------contingencia.c-------------------------------------
****************************** PROGRAMA PARALELO ******************************** 
 PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM 
Este arquivo e o corpo principal do programa de Analise de Contingencias
Arquivos componentes:
                    1)contingencia.c
                    2)inipvm.c
                    2)entrada.c
                    3)prepara-dados.c
                    4)monta_matriz.c
                    5)state.c
                    6)zollen.c
                    7)fcarga.c
                    8)saida.c
                    9)fluxos.c
                   10)ident.c
                   11)injecao.c
                   12)inversa.c
                   13)modif.c
                   14)mpqdir.c
                   16)potencia.c
                   17)pqdir.c
                   18)produto.c
                   20)saida.c
                   21)analise.c
                   22)select.c
                   23)svector.c
                   24)outage.c
                   25)state.c
Arquivos incluidos:
                    1)global.h
                    2)prototipos.h
                    3)vari.h  
Arquivos de dados:
                    1)dadc.txt
Obs: Arquivos que trazem instrucoes de prog. paralela
[contingencia.c/inipvm.c/fcarga.c/outage.c/analise.c/saida.c mestre.c escravo.c]
------------------------------------------------------------------------------------- */

/*-----------------------------------------------
  Bibliotecas e  arquivos que devem ser incluidos
------------------------------------------------*/
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"global.h"
#include"vari.h"          //arquivo contem as declaracoes de variaveis globais
//#include"variaveispvm.h"  // variaveis do pvm
#include"prototipos.h"    //arquivo de definicao dos prototipos das funcoes usadas
#include <pvm3.h>
/*------------------------------------------------
  Declaracoes de variaveis  passadas via parametro
  ----------------------------------------------*/

double ce[max],cle[max];
int myinst, NPROC;
int tids[5];
/*----------------------Inicio da funcao principal----------------------------- */
main(void){
	int i,l,k,impr,info;
	/*---------------------------------------------------------------
	Entra com os Parametros do Programa para Analise de Contingencia	
	----------------------------------------------------------------*/
	pindex = 0;
	// ncorte = 50;
        ncorte = 10;
	corte = 0.0;
	tol = 30.e-3;
	toler = 3.e-3;
	// tol = 1.e-5;
	// toler = 1.e-5;

	fcont = 1.0;
	// fpger = 1.0;
	// fpca = 1.0;

	fpger = 0.93;  // teste CELG
	fpca = 0.93;   // teste CELG

        fpger = 1.0;  // teste CELG
	fpca = 1.0;   // teste CELG

	ffnor = 1.0;
	fres = 1.0;
	frea = 1.0;
	nilha = 0;
	ncrit = 0;
	nyconv = 0;
	nnconv = 0;
	ndiv = 0;
	/*----------------------------------------------------------------
	PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM
	----------------------------------------------------------------
	----------------------------------------------
	Inicia processos pvm e cria variaveis associadas
	---------------------------------------------*/
	inicio();  //=> Funcao para disparar os hosts
	/*-----------------------------------------------------------
	A rotina fcarga() faz o I/O e o broadcast dos dados brutos
	-------------------------------------------------------------
	Entra com os Dados e Obtem o Estado Base
	-------------------------------------------------------------
	Calcula o Fluxo de Potencia do Caso Base
	Usa: 	1) Armazenamento Compacto por Zollenkopf
		2) Metodo de Newton Desacoplado Rapido (Versao BX)
	OBS: TODOS EXECUTAM PAI E FILHOS
------------------------------------------------------*/
	fcarga(ce,cle);
	if(myinst == 0){
		printf("\n No. de Barras nb->%d \n",nb);
		printf("\n No. de Ramos  nl->%d \n",nl);
		printf("\n --> FIM DO PROCESSAMENTO DO  CASO BASE! \n");
	}
	/*--------------------------------------
	Seleciona Opcao de Saida de Resultados
	-------------------------------------
	//escolha(impr);        // escolha de saida p/ contingencia
	------------------------------------------------
	Executa a selecao de contingencias [SCREENING]
	-------------------------------------------------
	----------------------------------------------
	Obtem os fluxos ativos e reativos do Caso Base
	--------------------------------------------*/
	fluxpq();
	/*-----------------------------------------------------
	Obtem Caminhos de Fatoracao  (esparsidade de vetores)
	----------------------------------------------------*/
	grafo();
	/*----------------------------------------------------------------
	PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM	
	-------------------------------------------------------------------
	PROCESSAMENTO PARALELO SINCRONO/ASSINCRONO
	-------------------------------------------
	LOOP SCREENING Executado em Paralelo Sincrono:
	Supoe divisao equitativa de casos entre hosts
	----------------------------------------------
	LOOP AC Executado em Paralelo Assincrono:
	Supoe divisao equitativa balanceada
	(em tempo) de casos entre os hosts
	--------------------------------------------*/
	/*------------------------------------------------------------------------
	** INICIO DO PROCESSAMENTO DE ANALISE DE CONTINGENCIAS POR "SCREENING" ***
	--------------------------------------------------------------------------*/
	if(myinst == 0){   // PROCESSO PAI
		/*---------------EXECUTA MESTRE---------------*/	
		MestreScreening(ce,cle,impr);
	}
	else{  // PROCESSO FILHO
		/*EXECUTA FILHO */
		EscravoScreening(ce,cle,impr);
	}
	if(myinst == 0){   // PROCESSO PAI
		/*---------------EXECUTA MESTRE---------------*/	
	//	MestreScreening(ce,cle,impr,myinst,NPROC,tids);
		//printf("Antes do Mestre avaliação");
		MestreA(ce,cle,impr);
		/*----------Mostra a saida dos resultados-----------*/
		output();       // chama rotinas que criam relatorios de saida
		//salida(impr);   // relatorios de estado/fluxos pos-contingencia (opcional)
	//	printf("\n --> FIM DO PROCESSAMENTO DA ANALISE DE CONTINGENCIA !!! \n");
	}
	else{  // PROCESSO FILHO
		/*EXECUTA FILHO */
		//printf("antes do processo de screening");
	//	EscravoScreening(ce,cle,impr,myinst,NPROC,tids);
		EscravoA(ce,cle,impr);
	}
	info = pvm_exit();
	return;
}

/*---------- Fim da funcao contingencia------------ */
