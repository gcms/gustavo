/*----------------------------------------saida.c-----------------------------------------------------*/

/*Este arquivo contem as seguintes funcoes:
                                          1)relat1: Emitir relatorios sobre o ESTADO DO SISTEMA - MODELO AC   
                                          2)relat2: Emitir relatorios sobre os FLUXOS DO SISTEMA - MODELO AC
					  3)relat3: Emitir relatorios sobre os CASOS MAIS CRITICOS DE SEVERIDADE - LISTA REDUZIDA


OBS: estas funcoes sao chamadas pelo programa principal


/*------------------------ Inicio da funcao relat1 - Estado TETA/V da rede--------------------------------*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "global.h"

#include "pvm3.h"


void  relat1(void)

{   
  double  vk,pl,atet,pg,pc,pi= 4.e0*atan(1.e0);
  int k,mcont;
  FILE *filoutPtr;   /*ponteiro para arquivo*/

/*------------------------------------Abre o arquivo para escrita-------------------------------------------*/

  if ((filoutPtr = fopen("relat1.sai","w"))==NULL)
  {
     printf("\n     >>> O arquivo nao pode ser aberto!! <<<\n");
     exit(1) ;
  }
  fprintf(filoutPtr,"\n               E S T A D O  AC  D O   S I S T E M A \n");
  fprintf(filoutPtr,"\n               C A S O  B A S E  D O  S I S T E M A \n");
  fprintf(filoutPtr,"\n----------------+-------------------+-------------+-----------+-------------|");
  fprintf(filoutPtr,"\n    BARRAMENTO  |       TENSAO      |    GERACAO  |   CARGA   |  INJ. LIQ.  |");
  fprintf(filoutPtr,"\n                |                   |             |           |             |");
  fprintf(filoutPtr,"\n                |                   |             |           |             |");
  fprintf(filoutPtr,"\n  No. EXT. TIPO |  MODULO    ANGULO |     [MW]    |   [MW]    |   [MW]      |");
  fprintf(filoutPtr,"\n----------------+-------------------+-------------+-----------+-------------|");


  for(k=0; k<nb; k++)
  {
    atet = t[k]*(180.e0/pi);
    pl = p[k]*100.e0;

    /*----------------------------
      Acessa a geracao da barra k:
    -----------------------------*/

    pg = pg0[k]*100.e0;
    pc = pg - pl;
    if( itipo[k] == 0 )
      vk = v[k];
    else
    vk = ves[k];
    fprintf(filoutPtr,"\n %5d    %3d   |%8.3lf %9.3lf |%12.3lf |%10.3lf |%12.3lf |",
    nex[k],itipo[k],vk,atet,pg,pc,pl);
  }

  fprintf(filoutPtr,"\n----------------------------------------------------------------------------| ");
  fprintf(filoutPtr,"\n CARGA TOTAL DO SISTEMA[MW]:%10.2lf No.ITERACOES ATIVAS:%3d REATIVAS: %3d|", carga,nitea,niter);
  fprintf(filoutPtr,"\n----------------------------------------------------------------------------|");

  if(ncaso != 0)  // Se tiver ocorrido alguma contingencia
    {
     mcont = kcont[ncaso][0];  // mcont => no. de contingencias p/caso
     fprintf(filoutPtr,"\n       No. BARRAS:%4d      No. GERADORES:%4d      No. RAMOS:%4d          |",nb,ng,nl-mcont);
     fprintf(filoutPtr,"\n----------------------------------------------------------------------------|");

     if(mcont == 1)
       fprintf(filoutPtr,"\n       CONTINGENCIA SIMPLES: CASO %4d      FATOR DE CONTINGENCIA:%12.3lf|" ,ncaso,fcont);
    else    
       fprintf(filoutPtr,"\n       CONTINGENCIA MULTIPLA: CASO %4d      FATOR DE CONTINGENCIAS:%12.3lf|" ,ncaso,fcont);

       fprintf(filoutPtr,"\n----------------------------------------------------------------------------|  ");         
    }
  else
       fprintf(filoutPtr,"\n       No. BARRAS:%4d      No. GERADORES:%4d      No. RAMOS:%4d          |",nb,ng,nl);

       fprintf(filoutPtr,"\n----------------------------------------------------------------------------|  ");  
 
      

  /*---------------------------------------------- Fecha Arquivo para escrita ----------------------------------------*/
  fclose( filoutPtr );
  return;
}
 /*------------------------------------------------- Fim da funcao relat1 --------------------------------------------*/




/*-------------------------------------------------- Inico da  funcao relat2 -----------------------------------------*/

void relat2(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  double somap,somaq,smk,pp,skm,pq;
  int i, k, m,k1,m1,mcont;

  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat2.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n                                   F L U X O   AC   D O   S I S T E M A \n");
  fprintf(filoutPtr,"\n                                   C A S O  B A S E  D O  S I S T E M A \n");
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");
  fprintf(filoutPtr,"\n    BARRAS                FLUXOS             |   BARRAS             FLUXOS                |       PERDAS      ");
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");
  fprintf(filoutPtr,"\n  INI.  FIN.    P(MW)     Q(MVAr)    S(MVA)  | INI.  FIM    P(MW)     Q(MVAr)    S(MVA)   |   P(MW)    Q(MVAr)");
   
  fprintf(filoutPtr,"\n  -------------------------------------------+--------------------------------------------+-------------------");

  somap = 0.e0;
  somaq = 0.e0;
  for( i=0; i<nl; i++)
  {	  
    k = ni[i];               // numeracao interna
    m = nf[i];
    k1 = nex[k];           // numeracao externa
    m1 = nex[m];
    skm = sqrt(fpkm[i]* fpkm[i] + rqkm[i]* rqkm[i]);
    smk = sqrt(fpmk[i]* fpmk[i] + rqmk[i]* rqmk[i]);
    pp = (fpkm[i] + fpmk[i])*100.e0;
    pq = (rqkm[i] + rqmk[i])*100.e0;
    somap = somap + pp;  //perda total de potencia ativa
    somaq = somaq + pq;  //perda total de potencia reativa

    fprintf(filoutPtr,"\n %4d  %4d %9.3lf %9.3lf  %9.3lf   |%4d  %4d  %9.3lf %9.3lf  %9.3lf  |%8.3lf  %8.3lf",k1,m1,fpkm[i]*100.e0,rqkm[i]*100.e0,skm*100.e0,m1,k1,fpmk[i]*100.e0,rqmk[i]*100.e0,smk*100.e0,pp,pq);
  
  }
  fprintf(filoutPtr,"\n---------------------------------------------+--------------------------------------------+-------------------");

  if(ncaso != 0)   // se tiver ocorrido alguma contingencia
    {
      mcont = kcont[ncaso][0];
      if(icont != 0)
	{
         fprintf(filoutPtr,"\n       No. BARRAS:%4d      No. GERADORES:%4d      No. RAMOS:%4d          |",nb,ng,nl-mcont);
         fprintf(filoutPtr,"\n----------------------------------------------------------------------------|");
	}
      if(mcont == 1)
       fprintf(filoutPtr,"\n       CONTINGENCIA SIMPLES: CASO %4d|" ,ncaso);
      else    
       fprintf(filoutPtr,"\n       CONTINGENCIA MULTIPLA: CASO %4d|" ,ncaso);

       fprintf(filoutPtr,"\n----------------------------------------------------------------------------|  ");         
    }
  else
    {
     fprintf(filoutPtr,"\n   No. BARRAS:%4d      No. GERADORES:%4d      No. RAMOS:%4d             PERDAS TOTAIS: %9.3lf %9.3lf",nb,ng,nl,somap,somaq);
     fprintf(filoutPtr,"\n--------------------------------------------------------------------------------------------------------------\n");

    } 

   fclose(filoutPtr);

  return;
}     


/*--------------------------------- Fim da funcao relat2---------------------------------------------------*/




/*-------------------------------- Inico da  funcao relat3 ------------------------------------------------*/

void relat3(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  int ii,jj,ll,ll1,mcont;


  /*---------------------------------------------------
   Seleciona os resultados da analise de contingencias:
   --------------------------------------------------*/


  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat3.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n                LISTA DE SEVERIDADES DE CONTINGENCIAS           \n");
  fprintf(filoutPtr,"\n                     RESULTADO DO SCREENING 1P-1Q               \n");
  fprintf(filoutPtr,"\n  --------------+--------------+-------------+-------------------");
  fprintf(filoutPtr,"\n    Ordem de    |  Numero do   |  Numero do  |     Indice de     ");
  fprintf(filoutPtr,"\n   Severidade   |    Caso      |    Ramo     |    Severidade     ");
  fprintf(filoutPtr,"\n  --------------+--------------+-------------+-------------------");
 
  for(ii=0; ii<nsobr; ii++)
  {	  
    ll = klasif[ii];
    mcont = kcont[ll][0];   
    for(jj=1; jj<=mcont; jj++)//mudei
      {
	ll1 = kcont[ll][jj];//mudei
	if(jj != mcont)
	  fprintf(filoutPtr,"\n                                  %5d ",ll1);
	else 	 
	  fprintf(filoutPtr,"\n     %5d         %5d          %5d         %.6f ",ii+1,ll+1,ll1,pia[ii]);


      }
    fprintf(filoutPtr,"\n  ---------------------------------------------------------------");
  }
  fprintf(filoutPtr,"\n  ---------------------------------------------------------------");
  fprintf(filoutPtr,"\n  ---> LISTA INICIAL = %4d          --->LISTA REDUZIDA = %4d      ",nsobr,ncrit);
  fprintf(filoutPtr,"\n  ---------------------------------------------------------------");
  fclose(filoutPtr);

  return;
}     


/*--------------------------------------------------Fim da  funcao relat3---------------------------------------------*/




/*--------------------------------------------------Inicio da  funcao relat4------------------------------------------*/
void relat4(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  int ii,jj,ll,ll1,mcont;


 /*---------------------------------------------------
   Seleciona os resultados da analise de contingencias:
   --------------------------------------------------*/

  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat4.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n        ILHAMENTOS OCORRIDOS               \n");
  fprintf(filoutPtr,"\n    RESULTADO DO SCREENING 1P-1Q           \n");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n       Numero do       |       Numero do     ");
  fprintf(filoutPtr,"\n         Caso          |         Ramo        ");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  

  for(ii=0; ii<nilha; ii++)
  {	  
    ll = kilha[ii];
    mcont = kcont[ll][0];   
    for(jj=1; jj<=mcont; jj++)//mudei
      {
	ll1 = kcont[ll][jj];
	if(jj != mcont)
	  fprintf(filoutPtr,"\n                %5d ",ll1);
	else 
          fprintf(filoutPtr,"\n       %5d           |       %5d  ",ll+1,ll1);
      }
    fprintf(filoutPtr,"\n  -------------------------------------------");
  }

  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n  QUANT. DE ILHAMENTO(S) OCORRIDO(S) =%5d",nilha);

  fclose(filoutPtr);

  return;
}     

/*--------------------------------------------------Fim da  funcao relat4-------------------------------------------*/



/*--------------------------------------------------Inicio da  funcao relat5------------------------------------------*/
void relat5(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  int ii,jj,ll,ll1,mcont;


  /*-------------------------------
    Lista as Divergencias Ocorridas
    ------------------------------*/

  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat5.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n                DIVERGENCIAS OCORRIDAS           \n");
  fprintf(filoutPtr,"\n         RESULTADO DA ANALISE DE CONTINGENCIAS   \n");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n       Numero do       |       Numero do     ");
  fprintf(filoutPtr,"\n         Caso          |         Ramo        ");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  

  for(ii=0; ii<ndiv; ii++)
  {
    ll = kdiverge[ii];
    mcont = kcont[ll][0];   
    for(jj=1; jj<=mcont; jj++)//mudei
      {
	ll1 = kcont[ll][jj];
	if(jj != mcont)
	  fprintf(filoutPtr,"\n                %5d ",ll1);
	else 
          fprintf(filoutPtr,"\n      %5d            |      %5d  ",ll+1,ll1);
      }
    fprintf(filoutPtr,"\n  -------------------------------------------");
  }

  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n  QUANT. DE DIVERGENCIA(S) OCORRIDO(S) =%5d",ndiv);

  fclose(filoutPtr);

  return;
}     

/*--------------------------------------------------Fim da  funcao relat5-------------------------------------------*/


/*--------------------------------------------------Inicio da  funcao relat6-----------------------------------------*/
void relat6(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  int ii,jj,ll,ll1,mcont;


  /*---------------------------
    Lista as Nao Convergencias
    --------------------------*/

  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat6.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n                 NAO CONVERGENCIAS               \n");
  fprintf(filoutPtr,"\n         RESULTADO DA ANALISE DE CONTINGENCIAS   \n");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n       Numero do       |       Numero do     ");
  fprintf(filoutPtr,"\n         Caso          |         Ramo        ");
  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  

  for(ii=0; ii<nnconv; ii++)
  {	  
    ll = knotconv[ii];
    mcont = kcont[ll][0]; 
      
    for(jj=1; jj<=mcont; jj++)
      {
	ll1 = kcont[ll][jj];
	if(jj != mcont)
	  fprintf(filoutPtr,"\n                %5d ",ll1);
	else 
          fprintf(filoutPtr,"\n  %5d    |   %5d  ",ll+1,ll1);
      }
  }

  fprintf(filoutPtr,"\n  ---------------------+---------------------");
  fprintf(filoutPtr,"\n  QUANT. DE NAO CONVERGENCIA(S) =%5d\n",nnconv);
  fprintf(filoutPtr,"\n  NO. ESPECIFICADO DE ITERACOES =%2d\n",2*limite +1);
  fprintf(filoutPtr,"\n  TOLERANCIA ESPECIFICADA =%12.8lf\n",toler);
  fclose(filoutPtr);

  return;
}     

/*--------------------------------------------------Fim da  funcao relat6-------------------------------------------*/


/*--------------------------------------------------Inicio da  funcao relat7----------------------------------------*/
void relat7(void)   
{      
      
  FILE *filoutPtr;   /*ponteiro para arquivo*/
  int ii,jj,ll,ll1,mcont,it1,it2;


  /*---------------------------
    Lista as Convergencias
    --------------------------*/

  /*-------------------------------------------Abre o arquivo para escrita---------------------------------------------*/

  if ((filoutPtr = fopen("relat7.sai","w"))==NULL)
  {
     printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
     exit(1) ;
  }


  fprintf(filoutPtr,"\n                             CONVERGENCIAS OCORRIDAS          \n");
  fprintf(filoutPtr,"\n                      RESULTADO DA ANALISE DE CONTINGENCIAS   \n");
  fprintf(filoutPtr,"\n  ---------------------+---------------------+--------------------+------------------------");
  fprintf(filoutPtr,"\n       Numero do       |       Numero do     |     Indice de      |    No. de Iteracoes    ");
  fprintf(filoutPtr,"\n         Caso          |         Ramo        |    Severidade      |  Ativas     Reativas   ");
  fprintf(filoutPtr,"\n  ---------------------+---------------------+--------------------+------------------------");
  

  for(ii=0; ii<nyconv; ii++)
  {	  
    ll = kconv[ii];
    mcont = kcont[ll][0]; 
    it1 = kit1[ii];
    it2 = kit2[ii];
  
    for(jj=1; jj<=mcont; jj++)
      {
	ll1 = kcont[ll][jj];
	fprintf(filoutPtr,"\n        %5d          |        %5d        |      %.6f     |     %2d       %2d   ",ll+1,ll1,pia[kordem[ii]],it1,it2);
      }
    fprintf(filoutPtr,"   \n  -----------------------------------------------------------------------------------------\n");
  }

 
  fprintf(filoutPtr,"\n  QUANT. DE CONVERGENCIA(S) OCORRIDA(S)=%5d\n",nyconv);
  fclose(filoutPtr);

  return;
}     

/*--------------------------------------------------Fim da  funcao relat7------------------------------------------*/

/*----------------------------------------------Inicio da  funcao escolha-------------------------------------------*/
void escolha(int impr)   
{      
      
  char choose;

 printf("\n     Voce Deseja a Saida do Estado de Alguma Contingencia ? [s/n] :\n");
 scanf("\n %c" ,&choose);

 while(choose == 'S' || choose == 's')
   {
    printf("\n     Entre com No.do CASO CONTINGENCIADO(Estado/fluxos):\n");
    scanf("\n %c" ,&impr);
    return;
   }
 if(choose == 'N' || choose == 'n')
   return;
}     

/*--------------------------------------------------Fim da  funcao escolha-------------------------------------------*/


/*----------------------------------------------Inicio da  funcao salida---------------------------------------------*/

void salida(int impr)   
{

  int i,info;
  int msgtype0;

  FILE *filoutPtr1;   /*ponteiro para arquivo*/
  FILE *filoutPtr2; 

  printf("impr=%d\n",impr);
  printf("ESTOU EM SALIDA!!!\n");


  /*-------------------------------------------------------
    Saida da Contingencia   < estado/fluxos  -- opcional >
    -----------------------------------------------------*/


  /* -----------------------------------------------
     PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
     ---------------------------------------------*/

      
  /*  msgtype0 = 11;
  info = pvm_recv(-1,msgtype0);
  info = pvm_upkint(&icont,1,1);
  info = pvm_upkint(&kdiag,1,1);
  info = pvm_upkint(&nitea,1,1);
  info = pvm_upkint(&niter,1,1);
      
  info = pvm_upkdouble(vsend,nb,1);
  info = pvm_upkdouble(tsend,nb,1);
     

  info = pvm_upkdouble(p,nb,1);
  info = pvm_upkdouble(q,nb,1);
  info = pvm_upkdouble(fpkm,nl,1);
  info = pvm_upkdouble(fpmk,nl,1);
  info = pvm_upkdouble(rqkm,nl,1);
  info = pvm_upkdouble(rqmk,nl,1);
  
  for(i=0; i<nb; i++) 
    {  
      v[i] = vsend[i];  
      t[i] = tsend[i];
    }
  */   



  if(impr == icont && kdiag != -1)
   {

    /*-----------------------------------------------------------
      Saida da analise de contingencia < stado/fluxos -- opcional
      ---------------------------------------------------------*/

    if ((filoutPtr1 = fopen("relat1.sai","w")) == NULL)
     {
       printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
       exit(1) ;
     }

    if ((filoutPtr2 = fopen("relat2.sai","w")) == NULL)
     {
       printf("\n     >>>O arquivo nao pode ser aberto!!<<<\n");
       exit(1) ;
     }
 
     ncaso = impr;  // no. do caso na ordem da leitura da lista
     fprintf(filoutPtr1,"\n Arquivo relat1.sai \n");
     relat1();   // estado
     relat2();

    fclose(filoutPtr1);
    fclose(filoutPtr2);
   }

  return;
}
/*----------------------------------------------Fim da  funcao salida---------------------------------------------*/
/*----------------------------------------------Inicio da  funcao output------------------------------------------*/
/* Subrotina para chamadas de relatorios de saida */
void output(void)
{
	/*----------------------------------
	Saida de resultados Fluxo de Carga
	---------------------------------*/
	// N� deve ser chamado neste local. Deve ser chamado em fcarga!
	// relat1();  /*  Estado da rede eletrica  */
	// relat2();  /*  Fluxos de potencias ativas e reativas nas linhas   */
	/*-----------------------------------------------------------------------
	Gera arquivo com lista reduzida em funcao da severidade da contingencia
	---------------------------------------------------------------------*/
	
	if(nilha != nrc)
		relat3();   // lista ordenada reduzida
	/*--------------------------------------------
	Gera arquivo com lista ilhamentos detectados
	-------------------------------------------*/
	if(nilha != 0)
		relat4();
	/*---------------------------------------------
	Gera arquivo com lista divergencias ocorridas
	--------------------------------------------*/
	if(ndiv != 0)
		relat5();
	/*-----------------------------------------------------
	Gera arquivo com lista de nao convergencias ocorridas
	----------------------------------------------------*/
	if(nnconv != 0)
		relat6();
	/*-------------------------------------------------
	Gera arquivo com lista de convergencias ocorridas
	------------------------------------------------*/
	
	if(nyconv != 0)
		relat7();
	return;
	}


/*----------------------------------------------Fim da  funcao output---------------------------------------------*/
