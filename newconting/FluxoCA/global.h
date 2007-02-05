/*--------------------------------------global.h--------------------------------
Este arquivo contem a declaracao das variaveis simples e vetores (variaveis
indexadas) associados ao Calculo do Fluxo de Potencia
Constantes e parametros para redes eletricas tais como as redes do SIN e do IEEE
------------------------------------------------------------------------------*/

#define  nbmax    2500
#define  nlmax    3000
#define  maxnzero 6000
#define  SB       100.e0

/*------------------------------------------------------------------------------
   LEGENDA:
             nbmax     =   no. max. de barras
             nlmax     =   no. max. de ramos
             maxnzero  =   no. max. elementos da cadeia de Zollenkopf (elementos
                           nao nulos da matriz)
             SB        =   potencia base em MVA
------------------------------------------------------------------------------*/

/*------------------------------------------------
  Variaveis Simples Inteiras/Reais Nao Indexadas:
  -----------------------------------------------*/

extern int nitea,niter,nconv,nb,nl,ng,nz,lf,lim,ntot,limite,nitqm;

extern double tol,carga,tola,tolr,dtap,range,alfa;

/*--------
  Vetores:
  ------*/

extern int itipo[nbmax],itipob[nbmax],nslack[nbmax],nex[nbmax],ni[nlmax],
           nf[nlmax],ltc[nlmax],noze[nbmax],lrd[nbmax],lcol[nbmax],nseq[nbmax],
           itag[maxnzero],lnxt[maxnzero];

extern double  v[nbmax],ves[nbmax],qes[nbmax],q[nbmax],pes[nbmax],pca[nbmax],
               p[nbmax],gkl[nlmax],bkl[nlmax],tap[nlmax],tapi[nlmax],
               taps[nlmax],tapa[nlmax],tap0[nlmax],b1kl[nlmax],b2kl[nlmax],
               ylin[nlmax],gdiag[nbmax],bdiag[nbmax],shu[nbmax],shu0[nbmax],
               tet[nbmax],pkm[nlmax],pmk[nlmax],qkm[nlmax],qmk[nlmax],
               coef[maxnzero],qgmax[nbmax],qgmin[nbmax],qca[nbmax],pkl[nlmax];

/* ----FIM DA DEFINICAO DE VARIAVES RESTANTES RELACIONADAS AO FLUXO DE CARGA--*/
/* Modificacoes em 13/12/2006 */

