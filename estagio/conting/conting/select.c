/*----------------------------select.c------------------------------*/

/*        Funcoes associadas a selecao de contingencias
--------------------------------------------------  ---------------- */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"global.h"
#include"prototipos.h"
/*------------Inicio da Funcao rank2 --------------------------*/
void rank2(int linre[nlmax]){
	int i,modo,li;
	int lista[nlmax];
	double aax;
	double aa[nlmax];
	modo = 1;         // ordem decrescente
	/*---- Normalizando os indices de severidade pelo maior indice ----*/
	//nsobr =10;
	if(pindex == 0)
		pindex = 1;
	for(i=0; i<nsobr; i++){
		if (pia[i]>pindex)
			pindex = pia[i];
	}
	
	for(i=0; i<nsobr; i++){
		lista[i] = i;
		piaux[i] = pia[i]/pindex;
		aa[i] = pia[i]/pindex;
		//printf("\n Apos a normalizacao do piaux[%d]  = %f --- lista[%d] = %d" , i, piaux[i], i, lista[i]);
		// printf("\n Indice ja normalizado aa[%d] em select.c = %f", i, aa[i]);                   // pia[] ja normalizado
	}

	ordena2(nsobr,aa,lista,modo);

	for(i=0; i<nsobr; i++){
		li = lista[i];
		klasif[i] = linre[li];
		pia[i] = piaux[li];
		//printf("\n Indice ja normal e ordenado pia[%d] = %f --- lista[%d] = %d", i,pia[i], i, lista[i]);
	}
	return;
}

/*--------Fim da Funcao rank2---------------*/

/*---------------Inicio da Funcao reordena------------------*/

void reordena(void){
	int i,ii;
	int linre[nlmax];
	
	for(i=0; i<nsobr; i++)
		linre[i] = klasif[i];
	
	rank2(linre);
  /*------- A.) Corte da lista pelo indice de severidade------------*/
	for(i=0; i<nsobr; i++){
		if(pia[i] >= corte)
			ncrit++;                         // corte da lista
	}
  /*---------B.) Corte da lista pelo tamanho--------------*/
	if(ncrit >= ncorte)
		ncrit = ncorte;     // corte pelo tamanho da lista
	printf("\n Lista Original: nsobr= %d  \n ", nsobr);
	printf("\n Lista Reduzida: ncrit= %d  \n ", ncrit);
	//printf("estou em  reodena em Mestre screening");
	return;
	
}
/*-------------Fim da Funcao reordena--------------*/

/*------------Inicio da Funcao ordena--------------*/

/*Algoritmo Heapsort Knuth vol.3
Rotina que obtem os indices correspondentes a ordenacao parcial do vetor lista.
n     - no.de elementos que se deseja ordenar.
k     - no.de elementos dos vetores aa e lista
aa    - vetor real a ordenar (nao e modificado)
lista - vetor inteiro que retorna os indices ordenados do vetor aa
modo  - chave que determina ordem:
      = 0 (crescente)
      = 1 (decrescente)
      (Obs: inicializar o vetor lista com 0 ate n-1.)*/

void ordena(int n,double aa[nlmax],int lista[nlmax],int modo){
	int i,ir,m,nkl,j,l,aux,k;
	double xk;
	k = n;
	if(n != 0){
		l = k/2 + 1; 
		aux = l - 1;
		m = k ;
		nkl = k - n;
		while(l != 0){
			if(l == 1){
				ir=lista[m-1];
				xk=aa[ir];
				lista[m-1]=lista[0];
				m=m-1;
				if(m == nkl || m == 1){
					lista[0]=ir;
					break;
				}
			}
			else{
				l=l-1;
				ir = lista[l - 1];
				xk = aa[ir];
			}
			j=l;
			i=j;
			j=2*j;
			while(j-m <= 0){
				if(j-m < 0){
					if(modo == 0){  // Ordem crescente
						if(aa[lista[j-1]] > aa[lista[j]])
							j++;
						if(xk <= aa[lista[j-1]])
							break;
					}
					else{  // Ordem decrescente
						if(aa[lista[j-1]] < aa[lista[j]])
							j++;
						if(xk >= aa[lista[j-1]])
							break;
					}
				}// fim if(j-m < 0)
				else{
					if(modo == 0){// Ordem crescente
						if(xk <= aa[lista[j-1]])
							break;
					}
					else{  // Ordem decrescente  
						if(xk >= aa[lista[j-1]])
							break;
					}
				}
				lista[i-1] = lista[j-1];
				i = j ;
				j = 2 * j;
			}// fim while(j-m <= 0)
		lista[i-1]=ir;
		} // fim  while(l != 0)
		for(i=k-1;i>=aux;i--){
			ir=lista[k-i-1];
			lista[k-i-1]=lista[i];
			lista[i]=ir;
		}
	}//fim if(n != 0)
	return;
}

/*-----Fim da Funcao heapsort-------------*/

void ordena2(int n,double aa[],int lista[],int modo){
	int i,ir,j,k,xx;
	double xk;
	xk = 0;
	k = 0;
	xx = n;
	for (j = 0; j < n; j++){
		for(i = k; i < xx ;i++){
			if ( aa[i] >= xk){
				xk = aa[i];
				ir = i;
			}
		}
		lista[j] = ir;
		if(ir == k)
			k++;
		if(ir == xx)
			xx = xx - 1;

		aa[ir] = -1;
		xk = 0;
	}
	return;
}



