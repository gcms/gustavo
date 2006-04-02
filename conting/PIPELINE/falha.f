c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c ----------------------------------------------------------------------
c           CONFIGURACAO DINAMICA  &  FLEXIBILIDADE DA APLICACAO
c ----------------------------------------------------------------------

      subroutine falha(NPROC,NPROC1,NPROC2,NPROC3,indice1,grupo1,
     &xhost,ahost,nhost,xhostdel,xhostadd,tids,ip,iddead,nfalha)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'      

c
c Declaracao relativa ao PVM
c
      integer*4 info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3,NPROC
      integer*4 indice1(20),tids(20)

      character*32 xhost(20),grupo1(20)
      character*32 ahost(20)

      character*32 xhostdel,xhostadd

      integer*4 opt,ip,nhost,nfalha





c ---------------------------------------------------------
c Esta rotina simula a falha de um HOST da Maquina Virtual
c ---------------------------------------------------------


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ----------------------------------------------------------------------      
c         1.] Escolhe o HOST que sofrera a falha do ESTAGIO SCREENING
c ---------------------------------------------------------------------- 
c         2.] DELETA um dado HOST da MAQUINA VIRTUAL     
c ----------------------------------------------------------------------

     
c -------------------
c Contador de falhas
c -------------------

      nfalha = nfalha + 1


c234567

c ---------------
c Simula a FALHA
c ---------------
    

c == Escolhe o HOST que sofrera a falha:

      if( NPROC1 .eq. 1 ) goto 1000      ! nao provocar FALHA se a configuracao for minima
      iddead = NPROC1
      if( nfalha .eq. 2 ) iddead = NPROC1-1
      ip = indice1(iddead)
      xhostdel = grupo1(ip)
      

c == Forma conjuntos de hosts ...

      opt = 1        ! forma conjuntos
      call disponivel(opt,ip,xhost,ahost,nhost,xhostdel,xhostadd,
     &nfalha)     



c == Mata o Processo PVM Afetado

cc      call PVMFKILL( tids(iddead), info )   


c == DELETA um dado HOST da MAQUINA VIRTUAL:

      call PVMFDELHOST( xhostdel, info )
      if( info .lt. 0 ) write(*,*)' Erro em PVMFDELHOST() !'



 1000 continue     ! nao provocar FALHA se a configuracao for minima

      return          
      end                  ! Fim da rotina falha()





