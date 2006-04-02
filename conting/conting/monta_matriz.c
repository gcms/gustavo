/*----------------------------monta_matriz.c---------------------------------*/

/* Este arquivo contem a funcao monta_mat que e responsavel por construir a matriz B' e B''
 Ela utiliza as seguintes subfuncoes:
                                   1) preench -> reenche as matrizes jacobianas B' e B''
                                   2) elim    -> Elimina as colunas e linhas referentes a barra slack para ce(B')
                                                 Elimina as colunas e linhas referentes as barras slacks e PV
                                                 para cle(B")
                                     
 ----------------------------------------------------------------------------------*/
/*-----------------   arquivos incluidos  ------------------*/
#include<stdlib.h>
#include "global.h"
/*--------------prototipo de funcoes usadas  ------------*/
void preench(int,int,double,double[]);
void elim(double valores[],int nn);
/*----------------------Inicio da Funcao monta_mat------------ */
void monta_mat(double ce[],double cle[]){
  int i,ki,k,l;
  double xbl;
 
  /*---------------------------------
    Inicializar vetores "ce" e "cle"
    -------------------------------*/

  for(i=0; i<maxnzero; i++)
  {
    ce[i] = 0.e0;   // B'
    cle[i] = 0.e0;  // B''
  }

 
 /*--------------------------------------------------------
    Construir vetor com a ordem de fatoracao da coluna "ki" 
   -------------------------------------------------------*/

  for(i=0; i<nb; i++)
  {
    ki = nseq[i];
    lrd[ki] = i;
  }

 
 /*-------------------------------------------------------------------
    Constroi as matrizes B' e B'', fazendo a eliminacao da barra slack 
   --------------------------------------------------------------------*/
 
 for(i=0; i<nl; i++)
  {
    k = ni[i];
    l = nf[i];

    /*----------------------------------------------------
      construir a matirz B' ( substitui a funcao  matb1) 
      --------------------------------------------------*/

    ce[k] = ce[k]-b1kl[i];
    ce[l] = ce[l]-b1kl[i];
    preench(k,l,b1kl[i],ce);

    /*-------------------------------------------------- 
     construir a matriz B''  (substitui a funcao matb2) 
     -------------------------------------------------*/

    cle[k] = cle[k]-b2kl[i];
    cle[l] = cle[l]-b2kl[i];
    preench(k,l,b2kl[i],cle);
  }

  for(i=0; i<nb; i++)
  {
    xbl = -2.e0*shu[i];    // shu[i]  e o shunt total
    preench(i,i,xbl,cle);
  }

  /*----------------------- 
    Eliminar a linha slack 
   ----------------------*/

  elim(ce,1);

  /*------------------------------
    Eliminar as linhas slack e PV 
    -----------------------------*/

  elim(cle,0);

  return;
}

/*--------------------------- Fim da funcao monta_mat------------------------*/




/*-----------------------Inicio da funcao  preench---------------------------*/

void preench(int k,int l,double custo,double valores[])
{
  int p1,p2,lc,le;
 
  p1 = lrd[k];   /* indice da coluna de ordem "k" de execucao */
  p2 = lrd[l];   /* indice da coluna de ordem "l" de execucao */
  if(p1 == p2)
  {
    le = k;
    lc = lcol[k];
  }
  else
    if(p1 < p2)
    {
      le = l;
      lc = lcol[k];
    }
    else
    {
      lc = lcol[l];
      le = k;
    }
  while(lc>=0 && le>itag[lc])
    lc = lnxt[lc];

  if(le == itag[lc])
    valores[lc] = custo+valores[lc];
   return;
}
/*-----------------------------Fim da funcao preench---------------------------------*/


/*-------------------------Inicio da funcao elim-------------------------------------*/

/* Elimina a linha e coluna referente a barra Slack de ce e da cle elimina linha e
coluna referentes as barras Slack e PV   */

void elim(double valores[],int nn)
{
 
  int i = 0;
  while( i < nb )
  {
    if((itipo[i] != 0 && nn == 0) || (itipo[i] == 2 && nn == 1))
      valores[i] = 1.e20;         //Atribui um valor  grande para eliminar linha e coluna
    i++;
  }
  return;
}

/*---------------------------- Fim da funcao elim-------------------------------------*/

//Data de atualizacao:02/06/03





