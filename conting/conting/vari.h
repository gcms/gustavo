/*---------------------Arquivo vari.h----------------------------------------------
 ---------------------------------------------------------------------------------*/

#define  nbmax 2500
#define  nlmax 3000
#define  maxnzero 6000
#define  klasifmax 1500
#define  mmax 10
#define  max 3*nlmax

/*--------------------------------------------------------------------------------
   LEGENDA:
   nbmax     =   no. max. de barras
   nlmax     =   no. max. de ramos
   maxnzero  =   no. max. elementos da cadeia de Zollenkopf(el. nao nulos da matriz)
   mmax      = no. max. de alteracoes
---------------------------------------------------------------------------------*/

/*------- DEFINICAO DAS VARIAVEIS RELACIONADAS AO CALCULO DO FLUXO DE CARGA-------*/

//OBS: estavam definidas em "globvar.inc" do fortran



/*-----------------------------------------------
  Variaveis Simples Inteiras/Reais Nao-Indexadas:
  ---------------------------------------------*/


      int nslack,nitea,niter,nconv,nb,nl,ng,nz, 
          lf,lfmax,lfree,kfree,lfreemax;

      double tol,carga,tot;


/*--------
  Vetores:
  ------*/


      int     itipo[nbmax],nex[nbmax],ni[nlmax],nf[nlmax],
               noze[nbmax],lrd[nbmax],lcol[nbmax],nseq[nbmax],
               itag[maxnzero],lnxt[maxnzero];

      double   v[nbmax],ves[nbmax],qes[nbmax],q[nbmax],
               pes[nbmax],pca[nbmax],pg0[nbmax],p[nbmax],
               gkl[nlmax], bkl[nlmax],tap[nlmax],b1kl[nlmax],
               b2kl[nlmax],ylin[nlmax],gdiag[nbmax],bdiag[nbmax],
               shu[nbmax],tet[nbmax],t[nbmax],
               fpkm[nlmax],fpmk[nlmax],rqkm[nlmax],rqmk[nlmax],
               coef[maxnzero];
  

/*------FIM DA DEFINICAO DAS VARIAVEIS PARA CALCULO DO FLUXO DE CARGA-------------*/




/* -------DEFINICAO DE VARIAVES RESTANTES RELACIONADAS AO FLUXO DE CARGA---------*/

//OBS: estavam definidas em "globvar.inc" do fortran


/*----------------  Variaves simples  ---------------*/


     int lim,nconv,ntot,kord;


/*--------  Vetores  ------*/


     int      iger[nbmax],nprof[nbmax];

     double   qmax[nbmax],qmin[nbmax],qca[nbmax],pkl[nlmax],fmi[nlmax],fma[nlmax],xkl[nlmax];



/* ---FIM DA DEFINICAO DE VARIAVES RESTANTES RELACIONADAS AO FLUXO DE CARGA-----*/





/*----- DEFINICAO DAS VARIAVEIS RELACIONADAS A SELECAO DE CONTINGENCIAS--------*/

//OBS: estavam definidas em "varsel.inc" para o Fortran


/*------------------  Variaveis simples  -------------*/


    int      nso,nsobr,ival,ncorte,ncrit;

    double   rval,corte,pindex;
   

/*--------
  Vetores
  ------*/


    int      klasif[klasifmax+1];

    double   pia[nlmax],piaux[nlmax];


/*------FIM DA DEFINICAO DAS VARIAVEIS PARA SELECAO DE CONTINGENCIAS-------------*/






/*---------- DEFINICAO DAS VARIAVEIS PARA ANALISE DE CONTINGENCIAS---------------*/

//OBS: estavam definidas em "varcont.inc" para o Fortran


/*------------------
  Variaveis simples
 -----------------*/


    int     nrc,limite,icont,jx,kx,kdiag,nilha,ndiv,nnconv,nyconv,ifbt,ncaso; 

    double  toler,c1p,c2p,ej1,ek1,ej2,ek2,fcont;

/*------------------
  Vetores e matrizes
  ----------------*/


    int      kcont[nlmax][mmax+1],kilha[nlmax],kdiverge[nlmax],knotconv[nlmax],
             kordem[nlmax],kconv[nlmax],kit1[nlmax],kit2[nlmax],next[nbmax],nfastfor[nbmax]; 

    double   tetab[nbmax],vb[nbmax],wjk1[nbmax],wjk2[nbmax],
             xjk[nlmax],W1t[mmax][nbmax],W2t[mmax][nbmax],RESULT1[nbmax][mmax],
             RESULT2[nbmax][mmax],gklbase[nlmax],bklbase[nlmax],gdiagbase[nbmax],
             bdiagbase[nbmax],fpjk[nlmax],fpkj[nlmax],fqjk[nlmax],fqkj[nlmax];
    

// extra

    double  fpger,fpca,ffnor,frea,fres;
    int     viola; 
 
// extra PMV
 
    int ioutlocal,kdif,modelo;
    int kbusy[2];
    int isend1[nlmax],isend2[nlmax],isend3[nlmax];
    int isend4[nlmax],isend5[nlmax],isend6[nlmax],isend7[nlmax];
    double vsend[nbmax],tsend[nbmax];
    double psend[nlmax];
   
/*----------FIM DA DEFINICAO DAS VARIAVEIS PARA ANALISE DE CONTINGENCIAS---------*/
//teste


//  Data Atualizacao:05/05/2003


