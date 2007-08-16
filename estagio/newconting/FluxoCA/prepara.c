/*-----------------------------------prepara.c --------------------------------- 
/* Este arquivo possui rotinas que fazem a criacao de vetores necessarios ao  
/* armazenamento compacto da matriz de coeficientes de Ax = b                 
/* Funcoes ou rotinas presentes neste arquivo:                                
/* prepara()                                                                  
/* ncreat()                                                                   
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

/*----------------------- Prototipos de funcoes-------------------------------*/
void ncreat(int,int);

/*--------------------Inicio de prepara() ------------------------------------*/
void prepara(void){
  int i,k,l;

 /*--- Inicializacao dos vetores que representam a estrutura esparsa ---*/
  for(i=0; i< maxnzero; i++) {
    itag[i] = -1;      //Armazena a linha do proximo elem. nao nulo da matriz
    lnxt[i] = i+1;     //Armazena o indice da lista do prox. elemento nao nulo
  }
  lnxt[maxnzero-1] = -1;

  for(i=0; i<nb; i++)
  {
    lcol[i] = i;
    itag[i] = i;
    nseq[i] = i;   //Armazena a sequencia em que a matriz vai ser fatorada 
    noze[i] = 1;   //Armazena o num. de elem. nao nulos de cada coluna da matriz
    lnxt[i] = -1;
  }
  lf = nb;


  /*------------------------------------------------
    Inclusao do endereco dos elementos na estrutura
    esparsa, enquanto houver espaco no vetor   
  -------------------------------------------------*/

  i=0;
  while(i < nl && lf < maxnzero-1)
  {
    k = ni[i];
    l = nf[i];


    /*---------------------------------------------------
      Funcao para montar vetores apontadores e de indice 
    ---------------------------------------------------*/

    ncreat(k,l);
    ncreat(l,k);
    i++;
  }    /* fim do while*/


  /*--------------------------------------------------------------------
   Verificar se ha espaco suficiente no vetor de valores "ce" ou "cle" 
  ----------------------------------------------------------------------*/

  if(lf >= maxnzero-1)
  {
    printf("\n Area reservada insuficiente \n");
    exit(1);
  }
  return;
}
/*---------------------Fim de prepara() --------------------------------------*/


/*--------------------Inicio de ncreat() -------------------------------------*/

void ncreat(int k, int l)
{
  int achou;
  int lant, latual,laux;

  /*----------------------
    Inicializando valores 
   ---------------------*/

  latual = lcol[k];

  /*---------------------------------
    Verificando posicao para inserir 
   --------------------------------*/

  if(itag[latual] > l)
  {
/*-----------------------------------------------------------------------------
  Se a linha "l" do novo elemento for inferior ao ja presente, inserir no inicio
------------------------------------------------------------------------------*/

    lant = latual;
    lcol[k] = lf;
    latual = lf;
    itag[latual] = l;
    noze[k] = noze[k]+1;
    lf = lnxt[lf];
    lnxt[latual] = lant;
  }
  else
  {
    /*-------------------------------------------
       Aqui "l" eh maior ou igual a ja armazenada
     ------------------------------------------*/

    if(itag[latual] < l)
    {
      /*--------------------------------------------
       Neste ponto devo procurar lugar para inserir 
       -------------------------------------------*/

      achou = 0;    /* indica que nao achei lugar ainda   */
      lant = latual;
      latual = lnxt[latual];
      while(latual != -1 && achou==0)
      {
        /*--------------------------------------------------------
          Aqui estou procurando lugar ate achar ou acabar a lista 
	--------------------------------------------------------*/

        if(itag[latual] >= l)
        {
          /*------------------------------------------------
            Encontrei lugar: antes de "latual" ou "latual=l"
	  -------------------------------------------------*/

          achou = 1;  /*  indica que achei lugar na lista  */

        }   /*  fechando o then do if  */
        else
        {
          lant = latual;
          latual = lnxt[latual];

        }    /* fechando o else  */
      }  /* fechando o while(latual!=0....   */

      /*------------------------ 
        Verificar se latual = 0 
       -----------------------*/

      if(latual == -1)
        achou = 1;
      else
        if(itag[latual] == l)
          achou = 0;   /* nao eh feita a insercao, o valor ja existe   */

      /*---------------------------------------------------
       Verificar se realmente deve ser inserida a linha "l"
       ---------------------------------------------------*/

      if (achou == 1)
      {
        /*-------------------------
         Inserir antes de "latual" 
         ------------------------*/

        laux = lnxt[lf];
        itag[lf] = l;
        lnxt[lf] = latual;
        lnxt[lant] = lf;
        lf = laux;
        noze[k] = noze[k]+1;
      }   /* fim do if(achou==1)  */
    }   /*  fim do if(itag[latual]<l)   */
  }  /* fim do else de if(itag[latual]>l)  */
  
  return;
}
/*-----------------------------------Fim de ncreat() -------------------------*/
//Data de atualizacao:25/11/2006
