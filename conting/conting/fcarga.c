/*-------------------------------------fcarga.c------------------------------------*/

/*Este arquivo e  programa de calculo do Fluxo de Carga
 AC pelo metodo do Desacoplado Rapido (FDLF) - Versao BX.


 Funcoes componentes:
                    1)prepara-dados.c
                    2)monta_matriz.c
                    3)estaddo.c
                    4)zollen.c
                    5)relatorios.c

------------------------------------------------------------------------------------- */
/*Bibliotecas e  arquivos que devem ser incluidos*/

#include <math.h>
#include <stdio.h>
#include "global.h"
#include "pvm3.h"

void fcarga(double ce[],double cle[]){
	int i,ii;
	int kc,il,m;
	
	/*----------------------------
	Declaracoes relativas ao PVM
	--------------------------*/
	
	int msgtype0;
	int info,bufid;
	int aux;
	if(myinst == 0){
	/*-----------Leitura dos Dados no formato Peco-------------------*/
	ledado();
	/*-----------------------------------------------
		Prepara o buffer e enpacota os dados para fazer
		um Broadcast dos dados brutos mais os preparados
		------------------------------------------------*/
	msgtype0 = 11;
	bufid = pvm_initsend(PvmDataRaw);
	for(i=1; i<NPROC; i++){
		info = pvm_pkint(&NPROC,1,1);
		info = pvm_pkint(tids,NPROC,1);
		info = pvm_pkint(&ncorte,1,1);
		info = pvm_pkdouble(&tol,1,1);
		info = pvm_pkdouble(&toler,1,1); 
		info = pvm_pkdouble(&fcont,1,1);
		info = pvm_pkdouble(&corte,1,1);
		info = pvm_pkint(&nb,1,1);
		info = pvm_pkint(&ng,1,1);
		info = pvm_pkint(itipo,nb,1);
		info = pvm_pkint(nex,nb,1);
		info = pvm_pkdouble(tet,nb,1);
		info = pvm_pkdouble(v,nb,1);
		info = pvm_pkdouble(ves,nb,1);
		info = pvm_pkdouble(pes,nb,1);
		info = pvm_pkdouble(qes,nb,1);
		info = pvm_pkdouble(pca,nb,1);
		info = pvm_pkdouble(qca,nb,1);
		info = pvm_pkdouble(qmin,nb,1);
		info = pvm_pkdouble(qmax,nb,1);
		info = pvm_pkdouble(gdiag,nb,1);
		info = pvm_pkdouble(bdiag,nb,1);
		info = pvm_pkdouble(shu,nb,1);
		info = pvm_pkdouble(&carga,1,1);
		info = pvm_pkint(&nl,1,1);
		info = pvm_pkint(ni,nl,1);
		info = pvm_pkint(nf,nl,1);
		info = pvm_pkdouble(xkl,nl,1);
		info = pvm_pkdouble(gkl,nl,1);
		info = pvm_pkdouble(bkl,nl,1);
		info = pvm_pkdouble(fmi,nl,1); 
		info = pvm_pkdouble(fma,nl,1);
		info = pvm_pkdouble(ylin,nl,1);
		info = pvm_pkdouble(tap,nl,1);
		info = pvm_pkdouble(b1kl,nl,1);
		info = pvm_pkdouble(b2kl,nl,1);
		info = pvm_pkint(&nrc,1,1);
		info = pvm_pkint(&nsobr,1,1);
		info = pvm_pkint(&nso,1,1);
		info = pvm_pkint(klasif,nrc,1);
		/* E necessario mandar o tamanho e
		alguns dados de kcont se nao, nao
		funciona.Nao sei porque,mas e assim*/
		aux =(mmax +1)*nrc;
		info = pvm_pkint(&aux,1,1);
		for(ii=0; ii<1;ii++){
			for(i=0; i<=mmax; i++)
				info = pvm_pkint(&kcont[ii][i],aux,1);
			}
		}
		info = pvm_mcast(tids,NPROC, msgtype0);
	}
	else // [SOU FILHO VOU RECEBER OS DADOS ENVIADOS PELO PAI]
	{
	/*----------Recebe e desempacota os dados enviados pelo pai-----------*/
		msgtype0 = 11;
		info = pvm_recv(tids[0],msgtype0);
		info = pvm_upkint(&NPROC,1,1);
		info = pvm_upkint(tids,NPROC,1);
		info = pvm_upkint(&ncorte,1,1);
		info = pvm_upkdouble(&tol,1,1);
		info = pvm_upkdouble(&toler,1,1);
		info = pvm_upkdouble(&fcont,1,1);
		info = pvm_upkdouble(&corte,1,1);
		info = pvm_upkint(&nb,1,1);
		info = pvm_upkint(&ng,1,1);
		info = pvm_upkint(itipo,nb,1);
		info = pvm_upkint(nex,nb,1);
		info = pvm_upkdouble(tet,nb,1);
		info = pvm_upkdouble(v,nb,1);
		info = pvm_upkdouble(ves,nb,1);
		info = pvm_upkdouble(pes,nb,1);
		info = pvm_upkdouble(qes,nb,1);
		info = pvm_upkdouble(pca,nb,1);
		info = pvm_upkdouble(qca,nb,1);
		info = pvm_upkdouble(qmin,nb,1);
		info = pvm_upkdouble(qmax,nb,1);
		info = pvm_upkdouble(gdiag,nb,1);
		info = pvm_upkdouble(bdiag,nb,1);
		info = pvm_upkdouble(shu,nb,1);
		info = pvm_upkdouble(&carga,1,1);
		info = pvm_upkint(&nl,1,1);
		info = pvm_upkint(ni,nl,1);
		info = pvm_upkint(nf,nl,1);
		info = pvm_upkdouble(xkl,nl,1);
		info = pvm_upkdouble(gkl,nl,1);
		info = pvm_upkdouble(bkl,nl,1);
		info = pvm_upkdouble(fmi,nl,1); 
		info = pvm_upkdouble(fma,nl,1);
		info = pvm_upkdouble(ylin,nl,1);
		info = pvm_upkdouble(tap,nl,1);
		info = pvm_upkdouble(b1kl,nl,1);
		info = pvm_upkdouble(b2kl,nl,1);
		info = pvm_upkint(&nrc,1,1);
		info = pvm_upkint(&nsobr,1,1);
		info = pvm_upkint(&nso,1,1);
		info = pvm_upkint(klasif,nrc,1);
		info = pvm_upkint(&aux,1,1);
		for(ii=0; ii<1;ii++){
			for(i=0; i<=mmax; i++){
				info = pvm_upkint(&kcont[ii][i],aux,1);
			}
		}
	}
	/*------------	TODOS FAZEM-------*/
	/*-Etapa preparatoria para explorar a esparsidade das sub-matrizes Jacobianas*/
	prepara();
	/*-------------Ordenacao da matriz segundo Zollenkopf ----------*/
	orden();   //  Ordenar "ce"
	/*--------Montar matrizes Jacobianas B' e B''---------------------*/
	monta_mat(ce,cle);
	/*----Obter fatores triangulares da matrizes Jacobianas B' e B''---------------*/
	redu(ce);
	redu(cle);
	/*----------Processo iterativo para obter a solucao --------------*/
	estado(ce,cle);
	/*-----------Calcular fluxos nos ramos/linhas ------------------------*/
	fluxos();	
	/*-------------------PVM PVM PVM PVM PVM PVM----------------------*/

        printf("CASO BASE PROCESSADO !!!\n");
			
	if(myinst == 0){
		if(nconv == 1){
			printf("O CASO BASE NAO CONVERGE !!!\n");
			exit(0);
		}
/*--------------------	Saida de resultados	-------------------*/
		printf("\n ITERACOES ATIVAS = %d     ITERACOES REATIVAS = %d  \n",nitea,niter);
		printf("\n TOLERANCIA USADA = %lf \n\n",tol);
		relat1();  /*  Estado da rede eletrica  */
		relat2();  /*  Fluxos de potencias ativas e reativas nas linhas   */
	}
	
	return;
	
  }
