/*----------------------------------saida.c-----------------------------------*/
/*Este arquivo contem as seguintes funcoes:
     1)relat1: Relatorios do ESTADO DO SISTEMA - MODELO AC   
     2)relat2: Relatorios dos FLUXOS DO SISTEMA - MODELO AC
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

/*--------- Inicio da funcao relat1 - Estado TETA/V da rede-------------------*/

void  relat1(void)
{
  double  vk,pl,atet,pg,qg,qgmi,qgma,pc,qc,pi = 4.e0*atan(1.e0);
  int k,mcont;
  char ti1,ti2;

  FILE *filoutPtr;   /*ponteiro para arquivo*/

/*------------------------------------Abre o arquivo para escrita-------------*/

  if ((filoutPtr = fopen("relat1.txt","w"))==NULL)
  {
     printf("\n     O arquivo nao pode ser aberto ...\n");
     exit(1) ;
  }
  fprintf(filoutPtr,"\n                          E S T A D O  AC  D O   S I S T E M A ");
  fprintf(filoutPtr,"\n                          C A S O  B A S E  D O  S I S T E M A ");
  fprintf(filoutPtr,"\n-----------------+------------------+------------------------------------+-------------------");
  fprintf(filoutPtr,"\n   BARRAMENTO    |      TENSAO      |              GERACAO               |       CARGA       ");
  fprintf(filoutPtr,"\n-----------------+------------------+------------------------------------+-------------------");
  fprintf(filoutPtr,"\n No. EXTE. TIPO  | MODULO   ANGULO  |  ATIVA   REATIVA    LI        LS   |  ATIVA   REATIVA  ");
  fprintf(filoutPtr,"\n                 |  [pu]   [graus]  |   [MW]   [MVAr]   [MVAr]    [MVAr] |   [MW]   [MVAr]   ");
  fprintf(filoutPtr,"\n-----------------+------------------+------------------------------------+-------------------");
  
  for(k=0; k<nb; k++){
         atet = tet[k]*(180.e0/pi);  
         if(itipo[k] == 2){
            ti1 = 'S';
            ti2 = 'L';
            }
/* ------Acessa a geracao/carga da barra k-----*/
         pc = pca[k]*SB;
         qc = qca[k]*SB;
	     pg = p[k]*SB + pc;
         qg = q[k]*SB + qc;        
         
         if(itipo[k] == 0){
            vk = v[k];
            ti1 = 'P';
            ti2 = 'Q';
	        qgmi = 0.e0;
	        qgma = 0.e0;
            }
         if(itipo[k] == 2){
            vk = ves[k];
	        qgmi = qgmin[k]*SB;
	        qgma = qgmax[k]*SB;
            } 
         if(itipo[k] == 1){
	        vk = v[k];
	        ti1 = 'P';
            ti2 = 'V';
	        qgmi = qgmin[k]*SB;
	        qgma = qgmax[k]*SB;
            }
         fprintf(filoutPtr,
         "\n %4d       %c%c    %7.4lf  %7.3lf  %8.2lf %8.2lf %8.2lf  %8.2lf %8.2lf %8.2lf",
         nex[k],ti1,ti2,vk,atet,pg,qg,qgmi,qgma,pc,qc);
  }

  fprintf(filoutPtr,"\n-----------------+------------------+-----------------------------------+--------------------");
  fprintf(filoutPtr,
  "\n CARGA ATIVA TOTAL DO SISTEMA [MW]:%12.3lf No.ITERACOES ATIVAS: %4d REATIVAS: %4d",
  carga,nitea,niter);
  fprintf(filoutPtr,"\n---------------------------------------------------------------------------------------------");
  fprintf(filoutPtr,"\n        No. BARRAS:%4d      No. GERADORES:%4d     No. RAMOS: %4d",nb,ng,nl);
  fprintf(filoutPtr,"\n---------------------------------------------------------------------------------------------");  
  
  /*------------- Fecha arquivo para escrita ---------------------------------*/
  fclose( filoutPtr );
  return;
}
/*-------------------------------- Fim de relat1() ---------------------------*/


/*------------------------------- Inico de relat2() --------------------------*/

void relat2(void)   
{         
  double somap,somaq,smk,pp,skm,pq,ta,ta1,ta2,ta3;
  int i,k,m,k1,m1;
  FILE *filoutPtr;   /*ponteiro para arquivo*/

  /*-------------Abre o arquivo para escrita----------------------------------*/

  if ((filoutPtr = fopen("relat2.txt","w"))==NULL)
  {
     printf("\n   O arquivo nao pode ser aberto ...\n");
     exit(1) ;
  }

  fprintf(filoutPtr,"\n                                   F L U X O   AC   D O  S I S T E M A ");
  fprintf(filoutPtr,"\n                                   C A S O  B A S E  D O S I S T E M A ");
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");
  fprintf(filoutPtr,"\n    BARRAS                FLUXOS             |   BARRAS             FLUXOS                |       PERDAS      ");
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");
  fprintf(filoutPtr,"\n  INI.  FIN.    P(MW)     Q(MVAr)    S(MVA)  | INI.  FIM    P(MW)     Q(MVAr)    S(MVA)   |   P(MW)    Q(MVAr)");
   
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");

  somap = 0.e0;
  somaq = 0.e0;
  for( i=0; i<nl; i++)
  {	  
    k = ni[i];             // numeracao interna
    m = nf[i];
    k1 = nex[k];           // numeracao externa
    m1 = nex[m];
    skm = sqrt(pkm[i]*pkm[i] + qkm[i]*qkm[i]);
    smk = sqrt(pmk[i]*pmk[i] + qmk[i]*qmk[i]);
    pp = (pkm[i] + pmk[i])*SB;
    pq = (qkm[i] + qmk[i])*SB;
    somap = somap + pp;  //perda total de potencia ativa
    somaq = somaq + pq;  //perda total de potencia reativa
    fprintf(filoutPtr,"\n %4d  %4d %9.3lf %9.3lf  %9.3lf   |%4d  %4d  %9.3lf %9.3lf  %9.3lf  |%8.3lf  %8.3lf"
    ,k1,m1,pkm[i]*SB,qkm[i]*SB,skm*100.e0,m1,k1,pmk[i]*SB,qmk[i]*SB,smk*100.e0,pp,pq);
  }
  fprintf(filoutPtr,"\n---------------------------------------------+--------------------------------------------+-------------------");
    {
     fprintf(filoutPtr,"\n   No. BARRAS:%4d      No. GERADORES:%4d      No. RAMOS:%4d             PERDAS TOTAIS: %9.3lf %9.3lf"
     ,nb,ng,nl,somap,somaq);
     fprintf(filoutPtr,"\n--------------------------------------------------------------------------------------------------------------\n");
    } 
      
  fprintf(filoutPtr,"\n    T A P S   D O S  T R A N S F O R M A D O R E S     "); 
  fprintf(filoutPtr,"\n--------------+----------------------------------------");
  fprintf(filoutPtr,"\n   BARRAS     |           VALORES DOS TAPS             ");              
  fprintf(filoutPtr,"\n--------------+----------------------------------------");
  fprintf(filoutPtr,"\n INI.   FIM   |VAL.INIC.  LIM.INF.  LIM.SUP.  VAL.FINAL");
  fprintf(filoutPtr,"\n--------------+----------------------------------------");
  for(i = 0;i<nl;i++){
      k = ni[i];          // numeracao interna
      m = nf[i];
      k1 = nex[k];        // numeracao externa
      m1 = nex[m];
	  ta = tap0[i];
	  ta1 = tapi[i];	  
      ta2 = taps[i];
      ta3 = tap[i];
      if(ta != 0.e0){
         fprintf(filoutPtr,"\n %4d  %4d       %5.3lf       %5.3lf     %5.3lf     %5.3lf",
         k1,m1,ta,ta1,ta2,ta3);
         }
      }
   fprintf(filoutPtr,"\n--------------+----------------------------------------");
   fclose(filoutPtr);
   return;
}     
/*---------------------- Fim de relat2()--------------------------------------*/

