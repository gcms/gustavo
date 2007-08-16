/*--------------------------------------globvar.h-----------------------------------

Este arquivo contem a declaracao das variaveis simples e vetores
associados ao Calculo do Fluxo de Carga AC e Analise de contingencias 
Constantes e parametros para redes como a de 1663 barras - Sul e sudeste brasileiro


 ---------------------------------------------------------------------------------*/


 ---------------------------------------------------------------------------------*/

#define  nbmax 2500
#define  nlmax 3000
#define  maxnzero 6000
#define  klasifmax 2600
#define  mmax 10
#define  max= 3*nlmax

/*--------------------------------------------------------------------------------
   LEGENDA:
             nbmax     =   no. max. de barras
             nlmax     =   no. max. de ramos
             maxnzero  =   no. max. elementos da cadeia de Zollenkopf (elementos
                           nao nulos da matriz)
             mmax  = no. max. de alteracoes
        
---------------------------------------------------------------------------------*/

/*------- DEFINICAO DAS VARIAVEIS RELACIONADAS AO CALCULO DO FLUXO DE CARGA-------*/

//OBS: estavam definidas em "globvar.inc" do fortran



// Variaveis Simples Inteiras/Reais Nao-Indexadas:

   extern      int nslack,nitea,niter,nconv,nb,nl,ng,nz, 
                   lf,lfmax,lfree,kfree,lfreemax;

   extern      double tol,carga,tot;

//  Vetores:

   extern      int     itipo[nbmax],nex[nbmax],ni[nlmax],nf[nlmax],
                       noze[nbmax],lrd[nbmax],lcol[nbmax],nseq[nbmax],
                       itag[maxnzero],lnxt[maxnzero];
   extern      double  v[nbmax],ves[nbmax],qes[nbmax],q[nbmax],pes[nbmax],
                       pca[nbmax],pg0[nbmax],p[nbmax],gkl[nlmax], bkl[nlmax],
                       tap[nlmax],b1kl[nlmax],b2kl[nlmax],ylin[nlmax],
                       gdiag[nbmax],bdiag[nbmax],shu[nbmax],tet[nbmax],tet0[nbmax],
                       fpkm[nlmax],fpmk[nlmax],rqkm[nlmax],rqmk[nlmax],coef[maxnzero];
  

/*------FIM DA DEFINICAO DAS VARIAVEIS PARA CALCULO DO FLUXO DE CARGA-------------*/


/* -------DEFINICAO DE VARIAVES RESTANTES RELACIONADAS AO FLUXO DE CARGA---------*/

//OBS: estavam definidas em "globvar.inc" do fortran

//variaves simples

   extern     int lim,nconv,ntot,kord;

//vetores

   extern     int      iger[nbmax],nprof[nbmax];

   extern     double   qmax[nbmax],qmin[nbmax],qca[nbmax],pkl[nlmax],fmi[nlmax],fma_[nlmax],t[nbmax];



/* ----FIM DA DEFINICAO DE VARIAVES RESTANTES RELACIONADAS AO FLUXO DE CARGA------*/
