c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                        Faz Contingencia AC
c ----------------------------------------------------------------------
c                         Varios Metodos AC
c ----------------------------------------------------------------------

      subroutine contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)
c
      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'  
c
      dimension ce(max),cle(max)
      integer*4 impr,kk,mcont
c
c Declaracao relativa ao PVM
c
      integer*4 myinst,NPROC2
      integer*4 tids(20)


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ----------------------------------------------------------------------      
c                    2.] CALCULA CONTINGENCIAS AC 
c ----------------------------------------------------------------------


cc      do 6 ll = 1, ncrit   ! ncrit = quant. reduzida de ramos contingenciados <LOOP2>
       
cc      kk = klasif(ll)      ! toma os elementos componentes da LISTA REDUZIDA

      kk = ioutlocal         ! caso p/ simular

      mcont = kcont(kk,0)    ! pega valor de mcont

      if ( mcont .eq. 1 ) then

c ----------------------------------------------------------------------      
c         CALCULA CONTINGENCIAS CA SIMPLES POR POS - COMPENSACAO
c ----------------------------------------------------------------------
   

      call conting_single(ce,cle,kk,mcont,impr,tids)      ! Pos - compensacao


      else   ! faz analise de contingencia multipla

        if ( mcont .le. 4 ) then   ! ou seja: mcont = 2,3 ou 4

c ----------------------------------------------------------------------      
c        CALCULA CONTINGENCIAS CA MULTIPLA POR MID - COMPENSACAO
c ----------------------------------------------------------------------
        
        call conting_multiple(ce,cle,kk,mcont,impr,tids)  ! Mid - compensacao


        else                       ! ou seja: mcont = 5,6,7, ... ate 10

c ----------------------------------------------------------------------      
c        CALCULA CONTINGENCIAS CA MULTIPLA POR ATUALIZACAO DE FATORES
c ----------------------------------------------------------------------
        
        call refatora(ce,cle,kk,mcont,impr,tids)          ! Refatorizacao parcial

        endif

      endif
                      


c234567
cc    6 continue              ! pegar novo caso


      return
      end                   ! Fim da rotina contingenciaAC
 
c ----------------------------------------------------------------------










