/*-----------------------------------zollen.c-----------------------------------
 Este arquivo contem rotinas de Zollenkopf para fazer ordenacao, reducao da
 matriz para a forma LDU e resolver o sistema na forma fatorada, LDUx = b   
 Faz a ordenacao de como a matriz deve ser fatorada para minimizar o 
 aparecimento de "fill-ins"
 Contem as seguintes rotinas:
        1) orden()- Ordenacao das colunas de B' e B'' (ou barras)
        2) redu() - Obtencao dos fatores triangulares L, D, U de uma matriz
        3) solu() - Obtencao da solucao do sistema LDUx = b       
------------------------------------------------------------------------------*/
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
  
/*-----------------------------------Inicio da funcao orden-------------------*/

void orden(void){
  int j,k,min,m,kp,lk,li,la,l,ip,ln,i; 
  j=0;
  /* ordenar por colunas ate a de indice (nb-2), enquanto nao encheu o vetor*/
  while(j <= nb-2)
  {
    k=nseq[j];
    min=noze[k];
    m=j;
    /*  determinar coluna com menos nao nulos  */
    for (i=j+1; i<nb; i++)
    {
      k=nseq[i];
      if(noze[k] < min)
      {
        min=noze[k];
        m=i;
      }   /* fim do if(noze[k] < min)  */
    }   /*  fim do for i=j+1 ate nb-1  */
    /*  organizar o vetor de sequencias, se preciso  */
    if (m != j)
    {
      kp=nseq[m];
      nseq[m]=nseq[j];
      nseq[j]=kp;
    }
    else
      kp=nseq[m];

    /* lk-> indice, no vetor de enderecos "lcolptr", do 1. elem. nao nulo da
            coluna kp  */
    lk=lcol[kp];     

    /*  simular a eliminacao enqto. lk>0 e nao encher "lcolptr" */
    while (lk >= 0)
    {
      k=itag[lk];
      if(k != kp)
      {
        la=-1;
        li=lcol[kp];
        l=lcol[k];
        i=itag[l];
        while (li >= 0)
        {
          ip=itag[li];
          if(ip == i)
          {
            if(i == kp)
            {
              ln=lnxt[l];
              if (la < 0)
                lcol[k]=ln;
              else
                lnxt[la]=ln;
              lnxt[l]=lf;
              lf=l;
              noze[k]=noze[k]-1;
              l=ln;
            }  /* fim do if (i=kp)  */
            /*  ip=i e i<>kp => buscar proximo elemento da coluna k */
            else
            {
              la=l;
              l=lnxt[l];
            }   /*  fim do else  */

            /*  atualizar li  */
            li=lnxt[li];
            if (l >= 0)
              i=itag[l];
            else
              i=nb;
          }  /* fim do if(ip=i)  */
          else
            if (i < ip)
            {
              /*  se i<ip => buscar proximo da coluna k  */
              la=l;
              l=lnxt[l];
              if (l >= 0)
                i=itag[l];
              else
                i=nb;
            }  /*  fim de i < ip  */
            else
              /*  se i>ip => aparece um fill-in
                  verificar se existe lugar p/ inserir   */
              if (lf >= 0)
              {
                ln=lf;
                if (la >= 0)
                  lnxt[la]=ln;   /* insere apos um existente */
                else
                  lcol[k]=ln;   /*  insere no inicio da lista */
                lf=lnxt[ln];
                lnxt[ln]=l;
                itag[ln]=ip;
                noze[k]=noze[k]+1;
                la=ln;
                li=lnxt[li];
              }  /* fim do if(lf>0)  */
              else
              {
                /*  aqui o vetor nao tem mais espaco  */
                printf("\n Area reservada insuficiente ... /n");
                exit(1);
              }  /* fim do else vetor cheio  */

        }  /*  fim do while li >0   */
      }  /*  fim do if (k = kp)   */

      /*  aqui ou acabou a coluna pivot ou k=kp   */
      if (li < 0 || k == kp)
        lk=lnxt[lk];
    }  /*  fim do while lk>0  */
    j++;
  
  }  /*  fim do while (j<nb-1)*/


  return;
}
/*----------------------------------- Fim de orden() -------------------------*/


/*-----------------------------------Inicio de redu() ------------------------*/

void redu(double ce[])
{
  int j,kp,lk,lp,li,l,ip,k,i;
  double d,cf;
  char atualiza;

  j=0;
  while (j < nb)
  {
    kp=nseq[j];
    lk=lcol[kp];
    lp=lf;
    while (lp >= 0 && lk >= 0)
    {
      k=itag[lk];
      if(k == kp)
      {
        d=1.e0/ce[lk];
        ce[lk]=d;
      }
      else
        ce[lp]=ce[lk];
      lk=lnxt[lk];
      if(lk >= 0)
        lp=lnxt[lp];
    }
    if(lk < 0)
    {
      lk=lcol[kp];
      while(lk >=0)
      {
        k=itag[lk];
        if(k != kp)
        {
          cf=d*ce[lk];
          ce[lk]=-cf;
          lp=lf;
          li=lcol[kp];
	  l=lcol[k];
          atualiza='3';

          while(li >=0 && l>=0)
          {
       /* atualizacao de "i" ou de "ip", dependendo do valor em "atualiza" */
            switch (atualiza)
            {
              case '1':
                i=itag[l];
                break;
              case '2':
                 ip=itag[li];
                break;
              case '3':
                i=itag[l];
                ip=itag[li];
                break;
            }
            if(i < ip)
            {
              l=lnxt[l];
              atualiza='1';
            }
            else
            {
              if(i == ip)
              {
                ce[l]=ce[l]-cf*ce[lp];
                l=lnxt[l];
                atualiza='3';
              }   /*  fim de i==ip  */
              else
                atualiza='2';

              li=lnxt[li];
              lp=lnxt[lp];
            }  /*  fim de i>=ip  */
          }      /*  fim do while li>0 e l>0  */
        }    /*  fim do if k<>kp  */
        lk=lnxt[lk];
      }   /*  fim do while lk > 0 */
      j++;
    }   /* fim  do if lk <= 0   */
    else
    {
      printf("\n  Area reservada insuficiente ...    \n");
      exit(1);
    }
  }  /*  fim do while j<nb   */
  return;
}
/*-----------------------------------------Fim de redu() ---------------------*/

/*----------------------------Inicio de solu()--------------------------------*/

void solu(double xce[],double b[]){
	int j,k,l,i;
	double cf,sum;
	
	/* substituicao forward  */
	for (j=0; j<nb; j++){
		k=nseq[j];
		cf=b[k];
		b[k]=0.e0;
		l=lcol[k];
		while(l >= 0) {
			i=itag[l];
			b[i]=b[i]+xce[l]*cf;
			l=lnxt[l];
		}
	}
	 /*  substituicao backward  */
	for (j=nb-2; j>=0;j--){
		k=nseq[j];
		sum=b[k];
		l=lcol[k];
		while(l>=0){
			i=itag[l];
			if(i != k)
				sum=sum + xce[l]*b[i];
			l=lnxt[l];
		}
		b[k]=sum;
	}
	return;
}
/*-------------- Fim da funcao solu---------------*/

//Data de atualizacao: 25/11/2006


