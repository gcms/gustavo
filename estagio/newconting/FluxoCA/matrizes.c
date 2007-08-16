/*-----------------------------------matrizes.c -------------------------------- 
/* A rotina matrizes eh responsavel por construir as matrizes B' e B''        
/* e executa as seguintes funcoes:                                            
/* a) Preenche as matrizes jacobianas B' e B''                                
/* b) Elimina as colunas e linhas referentes a barra slack para ce(B')        
/* c) Elimina as colunas e linhas referentes as barras slacks e PV p/ cle(B'')                        
/* -----------------------------------------------------------------------------
/* Funcoes ou rotinas presentes neste arquivo:                                                                                                */
/* preench()                                                                  
/* elim()                                                                     
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
#include "global.h"   

/*----------------------- Prototipos de funcoes usadas -----------------------*/
void preench(int,int,double,double[]);

/*----------------------Inicio de matrizes() -------------------------------- */
void matrizes(double ce[],double cle[]){
  int i,ki,k,l;
  double xbl;
 
  /*---------------------------------
    Inicializa vetores "ce" e "cle"
    -------------------------------*/

  for(i = 0; i < maxnzero; i++)
  {
    ce[i] = 0.e0;   // B'
    cle[i] = 0.e0;  // B''
  }

 /*--------------------------------------------------------
    Constroi vetor com a ordem de fatoracao da coluna "ki" 
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

    /*------------------------------------------------------------------------
      constroi a matirz B' ( substitui a funcao  matb1 do programa em Fortran) 
      -----------------------------------------------------------------------*/

    ce[k] = ce[k]-b1kl[i];
    ce[l] = ce[l]-b1kl[i];
    preench(k,l,b1kl[i],ce);

    /*------------------------------------------------------------------------ 
     constroi a matriz B''  (substitui a funcao matb2 do programa em Fortran) 
     ------------------------------------------------------------------------*/

    cle[k] = cle[k]-b2kl[i];
    cle[l] = cle[l]-b2kl[i];
    preench(k,l,b2kl[i],cle);
  }

  for(i=0; i<nb; i++)
  {
    xbl = -2.e0*shu[i];    // shu[i] eh o shunt total
    preench(i,i,xbl,cle);
  }
  return;
}

/*--------------------------- Fim de matrizes() ------------------------------*/

/*-----------------------Inicio de preench() ---------------------------------*/

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
  while(lc >= 0 && le > itag[lc])
    lc = lnxt[lc];

  if(le == itag[lc])
    valores[lc] = custo + valores[lc];
   return;
}
/*-------------------------------Fim de preench() ----------------------------*/

/*-------------------------Inicio de elim() ----------------------------------*/

void elim(double valores[],int nn)
{
 
  int i = 0;
  while( i < nb )
  {
    if((itipo[i] != 0 && nn == 0) || (itipo[i] == 2 && nn == 1))
      valores[i] = 1.e20; //Atribui um valor grande para eliminar linha e coluna
    i++;
  }
  return;
}
/*----------------------------- Fim de elim() --------------------------------*/
//Data de atualizacao:25/11/2006
