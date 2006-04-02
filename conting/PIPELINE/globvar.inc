*****************************************************************
*      D I M E N S I O N A M E N T O S   P R O G R A M A    	*
*      A N A L I S E  DE   C O N T I N G E N C I A S  CA        *
*           DECLARACOES LIGADAS AO FLUXO DE CARGA CA            *
*                                                               *
*             AUTOR:  ANTONIO CESAR BALEEIRO ALVES              *
*********************** PROGRAMA PARALELO ***********************

*        [DIMENSIONADO PARA SIST. DE "nbmax" BARRAS]		*
*								*
***PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM**

c Variaveis globais associadas ao fluxo de carga

c Constantes/Parametros

      parameter(nbmax=1700,nlmax=3000)
      parameter(max=2*nlmax)
 
      implicit double precision(a-h,o-z)

c  Vetores

      common/bus1/v(nbmax),q(nbmax),p(nbmax)		
      common/bus2/ves(nbmax),tet(nbmax),t(nbmax)
      common/bus3/qes(nbmax),pes(nbmax)
      common/bus4/qmax(nbmax),qmin(nbmax)
      
      common/net1/gdiag(nlmax),bdiag(nlmax),xkl(nlmax)
      common/net2/gkl(nlmax),bkl(nlmax),tap(nlmax)
      common/net3/b1kl(nlmax),b2kl(nlmax)
      common/shunt/shu(nbmax),ylin(nlmax)
      common/carga/qca(nbmax),pca(nbmax)
      common/apont/noze(nbmax),lrd(nbmax),nex(nbmax) 
      common/apont1/itipo(nbmax)
      common/apont2/ni(nlmax),nf(nlmax)
      common/apont3/lcol(nbmax),nseq(nbmax),nprof(nbmax)
      common/apont4/itag(max),lnxt(max)
      common/flu/fmi(nlmax),fma(nlmax)
      common/linha/fpkm(nlmax),fpmk(nlmax),rqkm(nlmax),rqmk(nlmax)

c Variaveis nao-indexadas inteiras ou reais
     
      common/perd/carga,bmva
      common/itera/lim,nconv,ntot
      common/tolera/tol
      common/const1/lf,lfmax
      common/fator/fres,frea,fpca,fpger,ffnor
      common/const2/llavep,llaveq,nitea,niter
      common/const3/kformat,kord
      common/gerais/nb,ng,nl

c    Mexido em 20/07/95
