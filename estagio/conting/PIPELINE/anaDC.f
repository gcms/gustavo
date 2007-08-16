c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                 Faz Contingencia DC Incremental
c ----------------------------------------------------------------------
c               "FAST LINEAR CONTINGENCY ANALYSIS"
c ----------------------------------------------------------------------

      subroutine contingenciaDC(ce,impr,myinst,NPROC1)
c
      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'  
c
      dimension ce(max)
      integer*4 impr,kk,mcont

c
c Declaracao relativa ao PVM
c
      integer*4 myinst,NPROC1



c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ----------------------------------------------------------------------      
c         1.] CALCULA CONTINGENCIAS PELO METODO DC INCREMENTAL
c ---------------------------------------------------------------------- 
c                        PRE - PROCESSAMENTO
c ----------------------------------------------------------------------

cc      do 5 kk = 1, nrc                  ! nrc = quant. de ramos contingenciados <LOOP1>

      kk = ioutlocal                      ! caso a ser analisado

      mcont = kcont(kk,0)                 ! pega valor de mcont
      
      
      if( mcont .eq. 1) then              ! mcont = no. de alteracoes por caso  
 

c Calcula contingencia simples

      
      call outage_single(ce,kk,mcont,impr)      ! DC Incremental < simples >


      elseif( mcont .gt. 1 ) then   ! faz analise de contingencia multipla
     

c Calcula contingencia multipla


      call outage_multiple(ce,kk,mcont,impr)    ! DC Incremental < multipla >

  
      endif 

                   
c234567
cc    5 continue            ! pegar novo caso

      return
      end                   ! Fim da rotina contingenciaDC

c ----------------------------------------------------------------------










