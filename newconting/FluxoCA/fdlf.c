/*----------------------------fdlf.c--------------------------------------------
Este arquivo eh programa principal de Calculo de Fluxo de Potencia CA pelo 
Metodo Desacoplado Rapido (proposto por Brian Stott)
Implementados os Controles Basicos
Funcao: Obter estado e fluxos da rede eletrica do arquivo dad.txt
Arquivos fontes componentes:
                    1) fdlf.c
                    2) entrada.c
                    3) prepara.c
                    4) matrizes.c
                    5) zollen.c
                    6) estado.c
                    7) controle.c
                    8) potencia.c
                    9) fluxos.c
                   10) saida.c
Arquivos do tipo include:
                    1) global.h
                    2) vari.h
                    3) prototipos.h
Arquivos de dados:
                    1) dad.txt (banco de dados em formato PECO)
                    2) valores.txt (parametros para controles)
Arquivos de saida:
                    1) relat1.txt
                    2) relat2.txt          
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
  Arquivos do tipo include do aplicativo
----------------------------------------*/
#include "global.h"     //arquivo contem as declaracoes de variaveis globais
#include "vari.h"
#include "prototipos.h"

/*-------------------------------------------------------------------------
  Declaracoes de listas passadas via parametro (fatores L,D,U de B' e B'')
  -----------------------------------------------------------------------*/
double ce[maxnzero],cle[maxnzero];

/*----------------------Inicio do programa principal -------------------------*/
main(void){
/* -----------------------
/*  Potencia Base em MVA:
/*  SB = 100 MVA
-------------------------*/

     /*------- Leitura de Dados de Barra e de Linha (Formato PECO) -----------*/
     
	ledado();
	
	/*----- Etapa preparatoria para explorar a esparsidade de B' e B'' -------*/
	
	prepara();
	
	/*-- Ordenacao das colunas das matrizes segundo MD (Tinney II) Zollenkopf-*/
	
	orden();	

    /*------------ Obtencao das matrizes B' e B'' (estado())------------------*/
	/*------------ Fatoracao das matrizes B' e B'' (estado())-----------------*/
    /*-------------Processo iterativo para obter o estado da rede ------------*/
	
	estado(ce,cle);
		
	/*- Calcular fluxos nos ramos/linhas -------------------------------------*/
	
    fluxos();
	
    /*------ Gera saida em arquivos com os resultado -------------------*/
		
	relat1();  /*  Estado da rede eletrica */
	relat2();  /*  Fluxos de potencias ativas e reativas nas linhas*/
		
	/* ---- Mensagem de Fim de Processamento ---------------------------*/
	
	printf("     \n");
	printf(" FLUXO DE  P O T E N C I A  AC CALCULADO ...\n");
	
#ifdef WIN_32	
	getch();
#endif
}
/*---------- Fim da funcao Calculo do Fluxo de Potencia ------------ */
