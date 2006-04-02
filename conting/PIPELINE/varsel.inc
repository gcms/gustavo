*      D I M E N S I O N A M E N T O S   P R O G R A M A    	*
*      A N A L I S E  DE   C O N T I N G E N C I A S  CA        *

*********************** PROGRAMA PARALELO ***********************

*        [DIMENSIONADO PARA SIST. DE "nbmax" BARRAS]		*
*								*
*        [ NO. MAXIMO DE RAMOS      = nlmax        ]            *
*        [ NO. MAXIMO DE ALTERACOES = mmax         ]            *

***PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM**

c Constantes/Parametros

        parameter( klasifmax = 2600 )
   
	implicit double precision(a-h,o-z)

	common/selecao1/klasif(0:klasifmax)
	common/selecao2/rval,corte
        common/selecao3/ival,ncorte,ncrit,nso,nsobr        

        common/conting1/pia(nlmax),piaux(nlmax)

        

        

