*      D I M E N S I O N A M E N T O S   P R O G R A M A    	*
*      A N A L I S E  DE   C O N T I N G E N C I A S  CA        *

*********************** PROGRAMA PARALELO ***********************

*        [DIMENSIONADO PARA SIST. DE "nbmax" BARRAS]		*
*								*
*        [ NO. MAXIMO DE RAMOS      = nlmax        ]            *
*        [ NO. MAXIMO DE ALTERACOES = mmax         ]            *

***PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM**

c Constantes/Parametros

        parameter( mmax = 6 )

        implicit double precision(a-h,o-z)

c Vetores/Matrizes 

        common/opcao/toler,limite
        common/klista/kcont(nlmax,0:mmax)
        common/klista1/kilha(nlmax),kdiverge(nlmax)
        common/klista2/knotconv(nlmax),kconv(nlmax)
        common/klista3/kit1(nlmax),kit2(nlmax)
        common/klista4/nrc,icont,jx,kx
        common/klista5/nilha,ndiv,nnconv,nyconv


        common/status/kdiag

        common/analise1/tetab(nbmax),vb(nbmax)
        common/analise2/c1p,c2p,ej1,ek1,ej2,ek2
        
        common/analisemid1/wjk1(nbmax)
        common/analisemid2/wjk2(nbmax)

        common/analisemid3/xjk(nlmax)

        common/analisemid4/W1t(mmax,nbmax),W2t(mmax,nbmax)
        common/analisemid5/RESULT1(nbmax,mmax),RESULT2(nbmax,mmax)
         
        common/altera1/gklbase(nlmax),bklbase(nlmax)
        common/altera2/gdiagbase(nbmax),bdiagbase(nbmax)

        common/fluxodc/fpjk(nlmax),fpkj(nlmax)
        
        common/spvector1/next(nbmax),nfastfor(nbmax)
        common/spvector2/ifbt

        common/factor/fcont

        common/chamasaida/ncaso

        common/paralelo0/ioutlocal,kdif,modelo
        common/paralelo1/kbusy(2)
        common/paralelo2/isend1(0:nlmax),isend2(0:nlmax),isend3(0:nlmax)
        common/paralelo3/isend4(0:nlmax),isend5(0:nlmax),isend6(0:nlmax)
        common/paralelo4/vsend(nbmax),tsend(nbmax)
        common/paralelo5/ppia(2),psend(nlmax)

        common/paralelo6/idp(nlmax),idt(nlmax)
        common/paralelo7/ksend(nlmax)
        common/paralelo8/kvel(0:100)
               
        common/dados/kselmax

c  Ultima atualizacao em 12/03/96
