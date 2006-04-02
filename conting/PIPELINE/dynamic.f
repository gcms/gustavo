c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c ----------------------------------------------------------------------
c           CONFIGURACAO DINAMICA  &  FLEXIBILIDADE DA APLICACAO
c ----------------------------------------------------------------------

      subroutine dynamic(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &grupo1,grupo2,xhost,ahost,nhost,xhostdel,xhostadd,kselect,tids,
     &nsize,ip,iddead,nfalha)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'

c
c Declaracao relativa ao PVM
c
      integer*4 info,nsize
      integer*4 NPROC,NPROC1,NPROC2,NPROC3,NPROC
      integer*4 indice1(20),indice2(20),tids(20)

      character*32 xhost(20),grupo1(20),grupo2(20)
      character*32 ahost(20)

      character*32 xhostdel,xhostadd

      integer*4 opt,ip,nhost,nfalha





c -----------------------------------------------------------
c Esta rotina CORRIGE a falha de um HOST da Maquina Virtual
c ADICIONANDO um novo HOST a VM e DESOVA neste HOST um
c processo substituto ...
c
c O HOST a ser adicionado sera escolhido de  uma  lista  de 
c hosts DISPONIVEIS (nao configurados)
c -----------------------------------------------------------


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ----------------------------------------------------------------------      
c         1.] Escolhe o HOST a ser adicionado    
c ----------------------------------------------------------------------
c         2.] ADICIONA um outro HOST a MAQUINA VIRTUAL    
c ----------------------------------------------------------------------
c         3.] DESOVA neste HOST um processo substituto (PCX)  
c ----------------------------------------------------------------------
c         4.] Atualiza as informacoes ao processos afetados 
c ----------------------------------------------------------------------
      


c ----------------
c Corrige a FALHA
c ----------------


c == Escolhe o HOST a ser adicionado:

      opt = 2       ! seleciona host e atualiza conjuntos
      call disponivel(opt,ip,xhost,ahost,nhost,xhostdel,xhostadd,
     &nfalha)     



     
c == ADICIONA um outro HOST a MAQUINA VIRTUAL:

      call PVMFADDHOST( xhostadd, linfo )
      if( linfo .lt. 0 ) write(*,*)' Erro em PVMFADDHOST() !'
cc      xhost(ip) = xhostadd
      grupo1(ip) = xhostadd




c == DESOVA processo substituto no HOST recem ADICIONADO:

      call PVMFSPAWN('pcx',PVMHOST,xhostadd,1,tids(iddead),inumt)
      if( inumt .eq. 1 ) then
          write(*,*)' Processo Substituto Desovado !'
      else
          write(*,*)' Erro na Desova do Processo Substituto !'
          call PVMFLVGROUP( 'contingencia', info )                                 
          call PVMFEXIT( info )
      endif
 



c === Retorna o no. de membros que fazem parte do grupo contingencia:
      

      call PVMFGSIZE( 'contingencia', nsize )   
      if( nsize .lt. 0 ) write(*,*)' ERRO NO TAMANHO DO GRUPO'    
cc      write(*,*)' O grupo CONTINGENCIA tem ',nsize,' processos ...'





c == Atualiza as informacoes dos processos AFETADOS e atualiza os TIDS das tarefas

c -----------------------------------------------------------
c Faz o Broadcast de Dados Basicos Para Todos os Processos
c -----------------------------------------------------------
c -----------------------------------------------------------
c Sao Transmitidos Aqui os Dados Iniciais (NPROC, tids, ...)
c os Dados que Definem a Topologia da Rede e Seus Parametros
c -----------------------------------------------------------

      call broadcast(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &tids)


      

 1000 continue     ! nao provocar FALHA se a configuracao for minima

      return          
      end                  ! Fim da rotina dynamic()





c ----------------------------------------------------------------------
c                CONTABILIZA HOSTS DISPONIVEIS E EM USO
c ----------------------------------------------------------------------

      subroutine disponivel(opt,ip,xhost,ahost,nhost,xhostdel,xhostadd,
     &nfalha)

      include '/opt/pvm3/include/fpvm3.h'


c
c Declaracao relativa ao PVM
c

      character*32 xhost(20)
      character*32 ahost(20),allhost(8)

      character*32 xhostdel,xhostadd,anyhost

      integer*4 opt,ip,nhost,nfalha

c
c Host names (LSEE-23)
c

cc      data allhost /'tuvira','dourado','lambari','pintado','cascudo',
cc     &              'piava','tubarao'/

      data allhost /'carpa','tuvira','dourado','lambari','pintado',
     &                'cascudo','piava','tubarao'/




c MENU DE OPCOES:
c
c  opt = 1    -- forma conjunto de hosts disponiveis
c 
c  opt = 2    -- atualiza conjuntos de hosts e seleciona o host a ser adicionado
c


c == Forma conjunto de HOST disponiveis (livres)

c234567
       
      if( opt .eq. 1 ) then
          ndisp = 0
          do 10 i = 1,11 
             anyhost = allhost(i)
             do 20 j = 1, nhost
                if( xhost(j) .eq. anyhost ) goto 10
   20        continue
             ndisp = ndisp + 1
             ahost(ndisp) = anyhost   
   10     continue
      endif




c == Escolhe o HOST a ser adicionado e atualiza conjuntos de HOST disponiveis e de HOST em uso 

c234567
      if( opt .eq. 2 ) then
          xhostadd = ahost(1)       ! pega o 1o. da lista de disponiveis

          xhost(ip) = xhostadd
          ahost(ndisp+1) = xhostdel
      
          do i = 1+nfalha, ndisp+1
             anyhost = ahost(i)
             ahost(i-1) = anyhost
          enddo
       endif


c == TESTES TESTES TESTES TESTES TESTES TESTES TESTES TESTES TESTES 
cc           write(*,*)'   DISPONIVEIS ======'
cc           do i = 1,ndisp
cc              write(*,*)' ahost(',i,') = ',ahost(i)
cc           enddo

cc           write(*,*)'   EM USO ======'
cc           do i = 1,nhost
cc              write(*,*)' xhost(',i,') = ',xhost(i)
cc           enddo

c234567
      return
      end            ! Fim da rotina disponivel()
