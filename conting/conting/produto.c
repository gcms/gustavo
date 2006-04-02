/*----------------------------------produto.c-------------------------------*/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"global.h"


/*------------------------Inicio da funcao multmv1---------------------------*/

/* Funcao para multiplicar matriz por vetor  <xmatriz.vetor = prod> 
                   [ Para Matriz Quadrada m x m ]      

-----------------------------------------------------------------------------*/



void multmv1(double xmatriz[mmax][mmax],double vetor[mmax],int nordem,double prod[mmax])
{
  int i,l;
  float soma;
   
  for(i=0; i<nordem; i++)                 // no. de linhas da matriz
    {
      soma = 0.0;
      for(l=0; l<nordem; l++)             // no. de colunas da matriz
	soma = soma + xmatriz[i][l] * vetor[l];
     
      prod[i] = soma;
    }
 
  return;

}

/*--------------------------Fim da funcao multmv1----------------------------*/ 



/*------------------------Inicio da funcao multmv2---------------------------*/

/* Funcao para multiplicar matriz por vetor  <xmatriz.vetor = prod> 
                   [ Para Matriz Retangular  nb x m ]     
------------------------------------------------------------------------------*/



void multmv2(double xmatriz[nbmax][mmax],double vetor[mmax],int nlinx,int ncolx, double deltaf1[nbmax])
{
  int i,j;
  double soma;
 
  for(i=0; i<nlinx; i++)                 // no. de linhas da matriz
    {
      soma = 0.e0;
      for(j=0; j<ncolx; j++)         // no. de colunas da matriz
        soma = soma +( xmatriz [i][j] * vetor[j]);
                
      deltaf1[i] = soma;
     
    }
  
  return;

}

/*--------------------------Fim da funcao multmv2----------------------------*/ 



/*------------------------Inicio da funcao multmv3--------------------------*/

/* Funcao para multiplicar matriz por vetor  <xmatriz.vetor = prod> 
    [ Para Matriz Retangular Armazenada Compacta no Vetor elem[] ]   

----------------------------------------------------------------------------*/



void multmv3(double elem[2*mmax],int iplin[2*mmax+1],int kcoluna[2*mmax+1],double vetor[nbmax],int nlinx, double prod[mmax])
{
  int i,l,inicio,ifim,m;
  float soma;
 
  for(i=0; i<nlinx; i++)               // no. de linhas da matriz
    {
      soma = 0.0;
      inicio = iplin[i];
      ifim = iplin[i+1]-1;
      for(l=inicio; l<ifim; l++)         // no. de colunas da matriz
	{
         m = kcoluna[l];
         soma = soma + elem [l] * vetor[m];
        }

      prod[i] = soma;
    }
  return;

}

/*--------------------------Fim da funcao multmv3----------------------------*/ 





/*------------------------Inicio da funcao multmv4---------------------------*/

/* Funcao para multiplicar matriz por vetor  <xmatriz.vetor = prod> 
                   [ Para Matriz Retangular m x nb ]

-----------------------------------------------------------------------------*/



void multmv4(double xmatriz[mmax][nbmax],double vetor[nbmax],int nlinx,int ncolx, double prod[mmax])
{
 
  int i,l;
  float soma;
  for(i=0; i<nlinx; i++)          // no. de linhas da matriz
    {
      soma = 0.0;
      for(l=0; l<ncolx; l++)     // no. de colunas da matriz
	soma = soma + xmatriz[i][l] * vetor[l];
     
      prod[i] = soma;
     
    }
  return;
}

/*--------------------------Fim da funcao multmv4----------------------------*/ 



/*------------------------Inicio da funcao multmm1---------------------------*/

/* Funcao para multiplicar matriz por matriz  <xmatriz.ymatriz = zmatriz> 
                   [ Para Matriz Quadrada  m x m ]                        

-----------------------------------------------------------------------------*/



void multmm1(double xmatriz[mmax][mmax],double ymatriz[mmax][mmax],int nordem, double zmatriz[mmax][mmax])
{
  int i,j,k;
  float soma;
 
 
  for(j=0; j<nordem; j++)         // colunas da 2a.matriz
    {
      for(i=0; i<nordem; i++)     // linhas 1a. matriz
	{
         soma = 0.0;
         for(k=0; k<nordem; k++)  // colunas da 1a.matriz 
	   soma = soma + xmatriz[i][k] * ymatriz[k][j];
     
         zmatriz[i][j] = soma;
         
        }
    }
    return;

}

/*--------------------------Fim da funcao multmm1----------------------------*/ 



/*------------------------Inicio da funcao multmm2---------------------------*/

/* Funcao para multiplicar matriz por matriz  <xmatriz.ymatriz = zmatriz> 
                   [ Para Matriz do tipo (nb x m) .(m x m) ]                 
       
-----------------------------------------------------------------------------*/



void multmm2(double xmatriz[nbmax][mmax],double ymatriz[mmax][mmax],int nlinx,int ncolx, double zmatriz[nbmax][mmax])
{
  int i,j,k;
  float soma;
 
  for(j=0; j<ncolx; j++)        // colunas da 2a.matriz
    {
      for(i=0; i<nlinx; i++)    // linhas da 1a. matriz
	{
         soma = 0.0;
         for(k=0; k<ncolx; k++) // colunas da 1a.matriz
	   soma = soma + xmatriz[i][k] * ymatriz[k][j];
     
         zmatriz[i][j] = soma;
	}
    }
    return;

}

/*--------------------------Fim da funcao multmm2----------------------------*/ 



/*------------------------Inicio da funcao multmm3---------------------------*/

/* Funcao para multiplicar matriz por matriz  <xmatriz.ymatriz = zmatriz> 
                   [ Para Matriz do tipo (m x nb) .(nb x m) ]                        
-----------------------------------------------------------------------------*/



void multmm3(double xmatriz[mmax][nbmax],double ymatriz[nbmax][mmax],int nlinx,int ncolx, double zmatriz[mmax][mmax])
{
  int i,j,k;
  float soma;
 
  for(j=0; j<nlinx; j++)        // colunas da 2a.matriz
    {
      for(i=0; i<nlinx; i++)    // linhas da 1a. matriz
	{
         soma = 0.0;
         for(k=0; k<ncolx; k++) // colunas da 1a.matriz
	   soma = soma + xmatriz[i][k] * ymatriz[k][j];
     
         zmatriz[i][j] = soma;
         
        }
    }
    return;

}

/*--------------------------Fim da funcao multmm3----------------------------*/ 





/*------------------------Inicio da funcao multmm4---------------------------*/

/* Funcao para multiplicar matriz por matriz  <xmatriz.ymatriz = zmatriz> 
                   [ Para Matriz do tipo (m x nb) .(nb x m) ]  
        [ Primeira Matriz Armazenada Compactamente no Vetor elem[] ]        

-----------------------------------------------------------------------------*/


void multmm4(double elem[2*mmax],int iplin[2*mmax+1],int kcoluna[2*mmax+1],double ymatriz[nbmax][mmax],int nlinx, double zmatriz[mmax][mmax])
{
  int i,j,k,inicio,ifim,m;
  float soma;
 
  for(j=0; j<nlinx; j++)        // colunas da 2a.matriz
    {
      for(i=0; i<nlinx; i++)    // linhas da 1a. matriz
	{
         soma = 0.0;
	 inicio = iplin[i];
	 ifim = iplin[i+1]-1;
	 for(k=inicio; k<ifim; k++) // colunas da 1a.matriz
	   {
	     m = kcoluna[k];
             soma = soma + elem[k] * ymatriz[m][j];
           }
	  
     
         zmatriz[i][j] = soma;
         
        }
    }
    return;

}

/*--------------------------Fim da funcao multmm4----------------------------*/ 
