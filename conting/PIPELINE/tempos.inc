*                  ANALISE DE CONTINGENCIAS                     *
*        [    DECLARACAO DE VARIAVEIS DE TEMPO     ]            *
*        [DIMENSIONADO PARA SIST. DE "nbmax" BARRAS]		*

*								*
*        [ NO. MAXIMO DE RAMOS      = nlmax        ]            *
*        [ NO. MAXIMO DE ALTERACOES = mmax         ]            *
***PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM**

c Constante/Parametro 

      parameter( ktmax = 10 )

      implicit double precision(a-h,o-z)

      common/tempo1/time(0:ktmax)
	
