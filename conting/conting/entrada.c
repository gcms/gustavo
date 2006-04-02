/* ------- entrada.c --------------------*/
/*--------------------------------------------------------------------------------- 
	Este arquivo "leitura.c" contem as funcoes responsaveis pela leitura de dados no formato
	PECO, e sao definidas como se segue:
	1) ledado   -> Funcao que gerencia a leitura de dados e faz
			a preparacao de estruturas esparsas usadas;
	2) menu     -> Funcao que apresenta um menu de entrada na tela;
	3) le_barra -> Funcao que faz a leitura de dados de barra;
	4) le_rede  -> Funcao que faz a leitura de dados de rede
			(dados de linhas e transformadores);  
	5) le_lista -> Funcao que faz a leitura dos dados da lista de
			contingencias;
	6) nin      -> Funcao que transforma numeracao externa em interna das barras.
	OBS) Os nomes das barras nao devem conter espaco em branco entre os caracteres
	--------------------------------------------------------------------------
	Ler dados de ramo no formato PECO  
	
	LEGENDA:
		Valores que itipo() pode assumir:
						0 -> barra PQ
						1 -> barra PV
						2 -> barra SLACK 
	

------------------------------------------------------------------------------- */    
/*----------------------------arquivos incluidos-------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include "global.h"
#include "pvm3.h"
/*-----------------------------------------------------------------*/
	
	
/*------------------prototipos de funcoes--------------------------------- */
void menu(void);
int nin(int);
void le_barra(void);
void le_rede(void);
void prepara(void);
void le_lista(void);
/*-----------------------------------------------------------------*/
	
FILE *filePtr;   // Ponteiro para o arquivo dadc que sera lido
char nome[15];
	/*-------------------Inicio da Funcao ledado------------------ */

void ledado(void) {  
	/*--------------   chamando funcao menu de entrada ------------------*/
	menu();
	/*------------   Abre arquivo de leitura e testa se ha erro -------------*/
	if ((filePtr = fopen("dadc.txt", "r")) == NULL) {
		printf("\n      >>> Este arquivo nao pode ser aberto!! <<<\n");
		exit(1);
	} else {
		/*------------Faz leitura de dados de barra e de rede-----------*/
		le_barra();
		le_rede();
		le_lista();
	}
	/*------------	Fechar os  arquivos de leitura --------------------*/
	fclose(filePtr);
	/*-------------Chamando a funcao Prepara dados -----------------------------*/
	/*---------------------------------------------
	Funcao que prepara os vetores esparsos para o armazenamento compacto
	-------------------------------------------------------------------*/
	return;
}
	/*--------------------Fim da funcao ledado-----------------------*/
	
	
	
	
	/*--------------Inicio da funcao men------------------------ */
	
void menu(void) {
	printf("       \n");
	printf("       \n");
	printf("       ------------------------------------------------------------- \n");
	printf("       |    PROGRAMA DE ANALISE DE CONTINGENCIAS EM UM CLUSTER     | \n");
	printf("       |            VERSAO PARALELA PARA LINUX  - PVM              | \n");
	printf("       |                  [ SETEMBRO DE 2005 ]                     | \n");
	printf("       |     Alunos:     Liliane Balduino Hortencio                | \n");
	printf("       |                 Fabricio Alves de Melo                    | \n");
	printf("       |     Orientador: Antonio Cesar Baleeiro Alves              | \n");
	printf("       ------------------------------------------------------------- \n");
	printf("      \n");
	printf("      \n");
	return;
}
	/*-------------------------fim da funcao menu--------------------------- */
	
	/*------------------------------Inicio da funcao le_barra------------------ */
	/*---------------------Le os dados das barras----------------------*/
	// chamada na subrotina ledado()
	
void le_barra(void) {
	int  jex, iti, i, ii, inact, tamLeitura,
         npv = 0,
         nsl = 0;
	double pc, pg, vx, tt, qg, qgmi, qgma, qc, bsh;
		
	/*--------------Inicializacoes--------------*/
	carga = 0.e0;
	tot   = 0.e0;
	/*---------------ler o primeiro valor do primeiro registro -------------------*/
	tamLeitura = fscanf(filePtr, "%d", &jex);
	i = 0;

	while (i < 10000 && jex != 9999 && tamLeitura !=0) {
    	/*-------leitura das demais informacoes do registro --------*/
		fscanf(filePtr, "%d%d%s %lf%lf%lf %lf %lf %lf %lf %lf %lf \n",
                &inact, &iti, &nome, &vx, &tt, &pg, &qg, &qgmi, &qgma,
                &pc, &qc, &bsh);	
	    /*-------Armazenando em vetores valores lidos ---------------*/
		itipo[i] = iti;
		nex[i] = jex;
		ves[i] = vx;
		
        pg = pg * fpger; //teste
		pc = pc * fpca;  //teste	
	    
        qg = qg * fpger;  // teste CELG
	    qc = qc * fpca;  // teste CELG
    	/*------Inicializa com estado Teta nulo ----------------------------*/
		t[i] = 0.e0;
    	/*------Inicializa com estado V unit�io --------------------------*/
		v[i] = 1.e0;
		tet[i] = 0.e0;
    	/*------Injecoes de Pot�cias ---------------------*/
		
		
		pes[i] = (pg - pc) / 100.e0;    // Potencia ativa especificada
		qes[i] = (qg - qc) / 100.e0;    // Potencia reativa especificada
		pca[i] = pc / 100.e0;         // Potencia ativa de carga
		qca[i] = qc / 100.e0;         // Potencia reativa de carga
		
		carga = pc + carga;
		
		if (iti == 2) {               // Barra Slack
			nslack = i;
			nsl++;                 //contador de barra tipo slack
		} else {
			tot = tot + pes[i];
			if (iti == 0)           // Barra PQ - Carga
				carga = carga-pg;
			else
				npv++;              //contador de barras tipo PV
		}

		qmin[i] = (qgmi - qc) / 100.e0;
		qmax[i] = (qgma - qc) / 100.e0;
		//  Impondo Limites
		if (qgmi == 0.e0) 
			qmin[i] = -10.e0;
		if (qgma == 0.e0)
			qmax[i] = 10.e0;
		/*------------------Parametros da rede------------------*/
		bdiag[i] = bsh / 100.e0;
		shu[i] = bdiag[i];        // shunt
		/*-------------------Fazer nova leitura -----------------*/
		tamLeitura = fscanf(filePtr, "%d", &jex);
		i++;
	}                         //  fim do enquanto jex!=9999  
	/*------------------Definindo o numero de barras= nos----------------*/
	nb = i;
	ng = npv + nsl;              //definindo o numero de geradores

	return;
}
/*--------------------- fim da funcao le_barra---------------- */


/*--------------------Inicio da  Funcao le_rede--------------*/
/*----------------------Le os dados da rede (ramos)---------------------------*/
/*---------------------	chamada na subrotina ledado()----------------------------*/
void le_rede(void) {
	int lni, lnf, tamLeitura, m, k, i;
	double aux, xx, r, ysh, ta, ta1, ta2, fnorm, femer;

    for (k = 0; k < nb; k++)
		gdiag[k] = 0.e0;
	/*------------------------Ler o primeiro registro ------------------------*/
	tamLeitura = fscanf(filePtr, "%d", &lni);
	i = 0;
	while (i < 10000 && lni != 9999 && tamLeitura != 0) {
		tamLeitura = fscanf(filePtr, "%d %lf %lf %lf %lf %lf %lf %lf %lf \n",
                &lnf, &r, &xx, &ysh, &ta, &ta1, &ta2, &fnorm, &femer);
	
	/*----------Cria vetores de barras terminais de ramos:--------------*/
        ni[i] = nin(lni);    // transforma numeracao externa em interna n�inicial
		nf[i] = nin(lnf);    // transforma numeracao externa em interna n�final
	/*------------------------------Parametros de circuito da rede:	--------------*/
		r = r / 100.e0 ;       // resistencia
		xx = xx / 100.e0;      // reatancia serie
		aux = r * r + xx * xx;
		xkl[i] = xx;
	/*---Elementos da Matriz Admitancia Nodal -- Susceptancia Nodal (completando abaixo):--*/
		gkl[i] = r / aux;       // condutancia serie do ramo kl
		bkl[i] = -xx / aux;     // susceptancia serie do ramo kl 
		/*----------Tratamento de linhas extremamente curtas:-------*/
		k = ni[i];
		m = nf[i];
		if (fabs(xx) < 5.e-4 ) {
			if (itipo[k] > 0)
				v[m] = ves[k];
			else
				if (itipo[m] > 0)
					v[k] = ves[m];
		}
		/*-----OBS:
		Estamos interessados no MODELO AC - na formacao de
		Y (ou B) incluimos "shunts" e taps ------ */
		/*------------Elementos das Matrizes B' e B": Modelo BX ------------*/
		b1kl[i] = bkl[i];	
		b2kl[i] = -1.e0 / xx;
		fnorm = fnorm * ffnor;
		/* --------Mudado aqui para rodar o IEEE118 e o IEEE57 - ---------*/
		if (nb == 118 || nb == 57) {
			if (fnorm == 0.e0) {
				k = ni[i];
				m = nf[i];
				fnorm = 1.e-1 * (v[k] * v[m] / xx) * 100.e0; //ver esta notacao
			}
		}
		if (fnorm == 0.e0)
			fnorm = 9999.e0;// atribui um valor altissimo ao fluxo maximo se for lido zero
		fmi[i] = -fnorm / 100.e0;
		fma_[i] = fnorm / 100.e0;
		ylin[i] = ysh / 200.e0;      // modelo PI da linha
		if (ta == 1.e0 || ta == 0.e0) {
			ta = 0.e0;
			/*--------------------Elementos da matriz G e B-----------------------*/
			gdiag[k] = gdiag[k] + gkl[i];
			gdiag[m] = gdiag[m] + gkl[i];
			bdiag[k] = bdiag[k] + bkl[i] + ylin[i];
			bdiag[m] = bdiag[m] + bkl[i] + ylin[i];
			shu[k] = shu[k] + ylin[i];
			shu[m] = shu[m] + ylin[i];     //shunt de linha
		} else {
			/*--------Tap fixo--------*/
			aux = 1.e0/ta;
			bdiag[k] = bdiag[k] + bkl[i] * aux * aux;
			bdiag[m] = bdiag[m] + bkl[i];
			gdiag[k] = gdiag[k] + gkl[i] * aux * aux;
			gdiag[m] = gdiag[m] + gkl[i];
			shu[k] = shu[k] + bkl[i] * aux
                * (aux - 1.e0) / 2.e0; // "shunt" de transformador
			shu[m] = shu[m] + bkl[i] * (1.e0 - aux) / 2.e0;
			gkl[i] = gkl[i] * aux;
			bkl[i] = bkl[i] * aux;
		}
		tap[i] = ta;
		/*---------------------	Ler o proximo registro----------------------*/
		tamLeitura = fscanf(filePtr, "%d", &lni);
		i++;
	}
	/*-----------------------Definido o numero de linhas---------------------------*/

	nl = i;
	return;
}
/*------------------------------ fim da funcao le_rede---------------------- */


/*------------------------Inicio da funcao le_lista--------------------------------*/
/*--------------------Le os dados da lista de ramos contingenciados----------------*/
/*-----------------chamada na funcao ledado-------------------------
	OBS: O Vetor kk[] armazena os dados das contingencias onde:
	kk[0] armazena a quantidade de contingencias simultaneas e as demais posicoes
	do vetor armazenam os ramos que serao retirados, no caso, o maximo e 10 ramos
-----------------------------------------------------------------------------------*/
void le_lista(void) {
	int kk[mmax + 1], i, j, tamLeitura, aux, aux2, nc, info, bufid;
	/*------------	Ler o primeiro valor do primeiro registro------------------*/
	tamLeitura = fscanf(filePtr, "%d", &kk[0]);	
	nc = mmax + 1;
	i = 0;
	
	while (i < 10000 && kk[0] != 9999 && tamLeitura != 0) {
    	/*------------------Leitura das demais informacoes do registro--------------*/
    	tamLeitura = fscanf(filePtr, "%d %d %d %d %d %d %d %d %d %d \n",
                &kk[1], &kk[2], &kk[3], &kk[4], &kk[5],
                &kk[6], &kk[7], &kk[8], &kk[9], &kk[10]);

    	j = 0; 
    	while (kk[j] != 0) {
    		kcont[i][j] = kk[j];   // Lista de ramos - Ordem da contingencia
	    	klasif[i] = i;         // Lista com a ordem original de leitura 
		    j++;
    	}	
	
        /*---------------Lendo o segundo registro-----------------------*/
    	tamLeitura = fscanf(filePtr, "%d", &kk[0]);
	    i++;	
	} // fim do enquanto kk[0]!=9999  

	nrc = i;            // Numero total de ramos contingenciados
	nsobr = nrc;        
	nso = nsobr;

	return;
}
/*-------- fim da funcao le_lista-------------*/


/*--------------------------Inicio da funcao nin------------------------------- */
	// Subrotina que transforma a numeracao externa das barras em numeracao interna
	// E chamada na rotina le_rede()
	
int nin(int k) {
	int i = 0;
	
	while (i < nb && nex[i] != k)
		i++;
	if (i >= nb) {
		printf("\n    >>>Nao foi encontrada a barra: %d<<<\n",k);
		exit(1);
	}
        
	return i;
}
/*--------------------------- Fim da funcao nin----------------------------------- */
