c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                      [ Explora a Esparsidade de M ]
c                      [  Janeiro / Fevereiro 1995  ] 
c                      [ Modif./Expansoes: Fev/1996 ] 
c******************** PROGRAMA PARALELO/DISTRIBUIDO ********************
c*********************** MODELO PIPELINE DINAMICO **********************
c
cccPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c       
c      [ Versao 1: Contingencia simples ou multipla de ramo       ]
c      [           Explora esparsidade dos vetores                ]
c      [           Contingencias: A) --> Metodo DC Incremental    ]
c      [                          B) --> Metodo Pos-Compensation  ]
c      [                          C) --> Metodo Mid-Compensation  ]
c      [                          D) --> Refatorizacao Parcial    ]
c
c                [ Arquivos componentes: 1) gerente.f   ]
c                [                       2) input.f     ]
c                [                       3) entrada.f   ]
c                [                       4) fcarga.f    ]
c                [                       5) broadcast.f ]
c                [                       6) comunica.f  ]
c                [                       7) falha.f     ]
c                [                       8) dynamic.f   ]
c                [                       9) esparsa.f   ]
c                [                      10) estado.f    ]
c                [                      11) potencia.f  ]
c                [                      12) formb.f     ]
c                [                      13) zollen.f    ]
c                [                      14) svector.f   ]
c                [                      15) clock.f     ]
c                [                      16) globvar.inc ]
c                [                      17) varcont.inc ]
c                [                      18) varsel.inc  ]
c                [                      19) tempos.inc  ]
c
c                [ Arquivos de dados:    1) opcoes.dat  ]
c                [                       2) dad.dat     ]
c
c ---------------------
c [ PROGRAMA GERENTE ]
c ---------------------
c
      program gerente
c
      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'  
     
c
c Cadeias de Zollenkopf
c
      dimension ce(max),cle(max)

c
c Declaracao Relativa ao PVM
c
      integer*4 mytid,info,myinst,nsize
      integer*4 inumt,ihost
      integer*4 NPROC1,NPROC2,NPROC3,NPROC
      integer*4 nhost,dtid
      integer*4 indice1(20),indice2(20),tids(20)

      character*32 xhost(20),ahost(20),allhost(8),grupo1(20),grupo2(20)
      character*32 host,arch,hostin,nomehost,hostout
      character*32 xhostdel,xhostadd

      integer*4 ip,nhost,nfalha


c
c Declaracao relativa a medicao de tempos
c
      double precision dummy
      double precision t_getutime, t_getrtime 

      external t_start !$pragma C(t_start) 
      external t_stop !$pragma C(t_stop)
      external t_getutime !$pragma C(t_getutime)
      external t_getrtime !$pragma C(t_getrtime)


c
c Para gerar numero aleatorio
c

      integer seed
      double precision random
        


c
c Host names (LSEE-23)
c

cc      data allhost /'tuvira','dourado','lambari','pintado','cascudo',
cc     &              'piava','tubarao'/

      data allhost /'carpa','tuvira','dourado','lambari','pintado',
     &              'cascudo','piava','tubarao'/





c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------
c -----------------------------------------------
c Envolve o programa no PVM e junta-o a um grupo
c -----------------------------------------------

      call PVMFMYTID( mytid )




c -----------------------------------------------------
      dummy = t_start()   ! ativa o relogio -- time(0)
c -----------------------------------------------------

      call PVMFJOINGROUP( 'contingencia', myinst ) 
      if( myinst .lt. 0 )  write(*,*)' ERRO NO GRUPO contingencia  '




c === Informacoes sobre a Maquina Virtual:


      call PVMFCONFIG( nhost, narch, dtid, host, arch, speed, info )
      if( info .lt. 0 ) write(*,*)' ERRO NA CONFIGURACAO '
      hostin = host                  ! Nome do HOST-LOGIN
      hostout  = 'piava'            ! Nome do HOST do processo manager
      
      if( hostin .ne. hostout) NPROC = nhost - 1   ! No. total de processos desovados = no. de maquinas - 1
      if( hostin .eq. hostout) NPROC = nhost       ! No. total de processos desovados = no. de maquinas

c === Repartindo os processos entre os hosts ( o NPROC minimo e 3 ):
    
      if( NPROC .eq. 3 ) then
          NPROC1 = 1               
          NPROC2 = 1               
          NPROC3 = 1                
      else
          NP = NPROC - 1        
          NPROC1 = NP/2            
          NPROC2 = NP - NPROC1     
          NPROC3 = 1                
      endif
   
      

      print*,' NPROC1 = ',NPROC1
      print*,' NPROC2 = ',NPROC2
      print*,' NPROC3 = ',NPROC3
      print*,' -------  ---------'
      print*,' NPROC  = ',NPROC


      
      write(*,*)'                                 '
      write(*,*)' --> PROCESSO GERENTE NA MAQUINA ',hostin
      write(*,*)'                                 '
      write(*,*)' --> PROCESSO MANAGER NA MAQUINA ',hostout
      write(*,*)'                                 '

      
      ihost = 0
    

c === Define maquinas para os processos dos GRUPOS 1 E 2:
      ncnt = 1
      kcnt = 1
      do 20 i = 1,nhost
        call PVMFCONFIG( nhost,narch,dtid,xhost(i),arch,speed,info )
        if( xhost(i) .ne. hostin .and. xhost(i) .ne. hostout ) then
            if( ncnt .le. NPROC1 ) then
                grupo1(i) = xhost(i)
                write(*,*)' GRUPO 1: MAQ. --> ',grupo1(i)
                indice1(ncnt) = i
                ncnt = ncnt + 1
            else
                grupo2(i) = xhost(i)
                write(*,*)' GRUPO 2: MAQ. --> ',grupo2(i)
                indice2(kcnt) = i 
                kcnt = kcnt + 1 
            endif
        endif
  20  continue


c === Desova processos servidores do GRUPO 1:

c ----------------------------------------------------------------------
      write(*,*)'                                 '
      do i = 1, NPROC1
         ip = indice1(i)
         nomehost = grupo1(ip)
         write(*,*)' --> DESOVANDO PCX NA MAQUINA ',nomehost
         call PVMFSPAWN( 'pcx',PVMHOST,nomehost,1,tids(i),inumt )
         ihost = ihost + inumt
      enddo
      if ( ihost .lt. NPROC1 ) then
         write(*,*)' Problema na DESOVA. Verifique o tids !'
         call PVMFLVGROUP( 'contingencia', info )                                 
         call PVMFEXIT( info )
      endif


      ihost = 0
     



c === Desova processos servidores do GRUPO 2:

c ----------------------------------------------------------------------
      write(*,*)'                                 '
      do i = 1, NPROC2
         ip = indice2(i)
         nomehost = grupo2(ip)
         kp = NPROC1 + i
         write(*,*)' --> DESOVANDO PCY NA MAQUINA ',nomehost
         call PVMFSPAWN( 'pcy',PVMHOST,nomehost,1,tids(kp),inumt )
         ihost = ihost + inumt
      enddo
      if ( ihost .lt. NPROC2 ) then
         write(*,*)' Problema na DESOVA. Verifique o tids !'
         call PVMFLVGROUP( 'contingencia', info )           
         call PVMFEXIT( info )
      endif



     

c === Desova processos servidores do GRUPO 3:

c ----------------------------------------------------------------------
      write(*,*)'                                  '
      write(*,*)' --> DESOVANDO MANAGER NA MAQUINA ',hostout
      call PVMFSPAWN('manager',PVMHOST,hostout,1,tids(NPROC),inumt)
      if( inumt .lt. NPROC3 ) then
         write(*,*)' Problema na DESOVA. Verifique o tids !'
         call PVMFLVGROUP( 'contingencia', info )                
         call PVMFEXIT( info )
      endif
      



c === Retorna o no. de membros que fazem parte do grupo:
      

      call  PVMFGSIZE( 'contingencia', nsize )   
      if( nsize .lt. 0 ) write(*,*)' ERRO NO TAMANHO DO GRUPO'    
cc      write(*,*)' O grupo CONTINGENCIA tem ',nsize,' processos ...'







c -----------------------------------------------------
      dummy = t_stop() ! desativa o relogio -- time(0)

      time(0) = t_getrtime()
c -----------------------------------------------------
c -----------------------------------------------------
      dummy = t_start()   ! ativa o relogio -- time(1)
c -----------------------------------------------------


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c -----------------------------------
c Seletor de Fluxo Continuo de Dados
c -----------------------------------
       kselmax = 10

       write(*,*)'                                '
       write(*,*)' --> FLUXO CONTINUO DE DADOS... ',kselmax
       write(*,*)'                                '





c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      msgtype1 = 1
      msg0 = 11
      msg1 = 21
      msg2 = 31
      msg3 = 41
      msg4 = 51
      msg5 = 61
      msg6 = 71
      msg7 = 81
      msg8 = 91
      msg9 = 101
      msg10 = 111
      msg11 = 121
      msg811 = 811          ! tag para emergencia na VM
      msg911 = 911          ! tag para emergencia na VM


c ----------------------------------------------------------------------------
c Requisita notificacao dos processos PVM sobre a ocorrencia de FALHA DE HOST
c ----------------------------------------------------------------------------
c ===== Faz Parte da Configuracao Dinamica com Tolerancia a Falha de Host ====
c ----------------------------------------------------------------------------

      call PVMFNOTIFY( PVMHOSTDELETE, msg911, NPROC, tids, info )
      if( info .lt. 0 ) write(*,*)' Erro em PVMFNOTIFY() !' 
  



c ------------------------------------
c Instante da Ocorrencia do Disturbio
c ------------------------------------

cc       seed = kselmax*nhost               ! para 5 ou 6 hosts (falha em 1)                      
cc       mynumb = 1 + int( DFLOAT(kselmax)*random(seed) )
cc       if( mynumb .gt. kselmax ) mynumb = kselmax
cc       write(*,*)' Numero Pseudo-Aleatorio mynumb= ',mynumb
c                                                                   ^
c ----------------------------------------------------------------- |
c OBS: Comentar este trecho para DESABILITAR CONFIGURACAO DINAMICA _|
c -----------------------------------------------------------------


c -------------------------------------------------
c Leitura de dados iniciais (arquivo 'opcoes.dat')
c -------------------------------------------------

       open(unit=065,file='opcoes.dat')
       call menu                      ! lendo dados iniciais (veja entrada.f)
       close(065)

c -------------------------------------------------------
c Leitura de dados da rede eletrica (arquivo 'dad1.dat')
c -------------------------------------------------------
       
       kselect = 1          ! ( 1a. vez )

       call input(kselect)

c ------------------------------------------------------------
c OBS: As chamadas subsequentes de input() serao feitas logo
c      antes da chamada de fcarga().
c ------------------------------------------------------------

 
c -----------------------------------------------------------
c Faz o Broadcast de Dados Basicos Para Todos os Processos
c -----------------------------------------------------------
c -----------------------------------------------------------
c Sao Transmitidos Aqui os Dados Iniciais (NPROC, tids, ...)
c os Dados que Definem a Topologia da Rede e Seus Parametros
c -----------------------------------------------------------

      call broadcast(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &tids)


    

c -----------------------------------------------------
      dummy = t_stop() ! desativa o relogio -- time(1)

      time(1) = t_getrtime()
c -----------------------------------------------------
c -----------------------------------------------------
      dummy = t_start()   ! ativa o relogio -- time(2)
c -----------------------------------------------------







c -----------------------------------------------------------
c Entra com os Dados e Obtem o Estado do Caso Base (1a. vez)
c ----------------------------------------------------------- 

c -------------------------------------------------------------
c OBS: A 1a. chamada de input() foi feita acima. Em seguida, 
c      foi feito o BROADCAST dos dados iniciais e topologicos.
c -------------------------------------------------------------

      kselect = 1               ! 1a vez que processa o caso base

         
      call fcarga(ce,cle)

      write(*,*)'                         '
      write(*,*)' --> CASO BASE PRONTO !!!'
      write(*,*)'                         '


      if( kselect .eq. 1 ) then
        write(*,*)'                          '
        write(*,*)' No. de Barras                  --> nb = ',nb
        write(*,*)' No. de Ramos                   --> nl = ',nl
        write(*,*)' No. que indica o fim da cadeia --> lfmax = ',lfmax
        write(*,*)'                          '
      endif
 


c -------------------- CONFIGURACAO DINAMICA ---------------------------

c -----------------------
c Simula a Falha de HOST
c -----------------------

          if( kselect .eq. mynumb ) then       ! para 5 ou 6 hosts (falha em 1)
    
              call falha(NPROC,NPROC1,NPROC2,NPROC3,indice1,grupo1,
     &xhost,ahost,nhost,xhostdel,xhostadd,tids,ip,iddead,nfalha)

              write(*,*)'                                         ' 
              write(*,*)' --> SIMULADA A FALHA DE HOST !!!        '
              write(*,*)'                                         '
              ksh = 1
          endif

c -------------------------------------------------------
c Espera Notificacao sobre a Ocorrencia de Falha de Host
c -------------------------------------------------------
 
c -- Host failure

 99       call PVMFNRECV( -1, msg911, knm )

          if( knm .gt. 0 ) then
              call PVMFUNPACK( INTEGER4, kddead, 1, 1, info )
              write(*,*)' FALHA DE HOST CONFIRMADA ....!'

c ------------------------
c Corrige a Falha de HOST 
c ------------------------

              call dynamic(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &grupo1,grupo2,xhost,ahost,nhost,xhostdel,xhostadd,kselect,tids,
     &nsize,ip,iddead,nfalha)
              ksh = 0                          ! falha sanada

          else
              if( ksh .eq. 1 ) goto 99         ! ocorreu falha
          endif

c ----------------------------------------------------------------------




c -----------------------------------------------------------
c Transmite aos Processos PCX Dados e Resultados (1a. vez)
c ----------------------------------------------------------- 
  
      call comunica(ce,cle,NPROC,NPROC1,NPROC2,NPROC3,kselect,
     &tids)


      write(*,*)' --> TRANSMITIDA A BASE DE DADOS !!!'
      write(*,*)'                                    '





c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c ---------------------------------------------------------------
c Espera Sinal do Processo PCX Para Prosseguir Calculando ou Nao
c ---------------------------------------------------------------

c234567
      nchave = 0
   8  call PVMFNRECV( tids(1),msg3,knl )

      if( knl .gt. 0 ) then
          call PVMFUNPACK( INTEGER4,ksinal,1,1,info )    ! PROSSEGUIR

          if( ksinal .eq. 1000) write(*,*)' --> RECEBI O SINAL 
     &DO PCX PAI --> CONTINUAR !!'

          if( ksinal .eq. 2000) write(*,*)' --> RECEBI O SINAL 
     &DO PCX PAI --> FINALIZAR !!'
 


c -------------------- CONFIGURACAO DINAMICA ---------------------------

c -----------------------
c Simula a Falha de HOST
c -----------------------

          if( ksinal .eq. 1000 .and. kselect .eq. mynumb ) then
    
              call falha(NPROC,NPROC1,NPROC2,NPROC3,indice1,grupo1,
     &xhost,ahost,nhost,xhostdel,xhostadd,tids,ip,iddead,nfalha)

              write(*,*)'                                         ' 
              write(*,*)' --> SIMULADA A FALHA DE HOST !!!        '
              write(*,*)'                                         '
              ksh = 1              
          endif

c -------------------------------------------------------
c Espera Notificacao sobre a Ocorrencia de Falha de Host
c -------------------------------------------------------
 
c -- Host failure

  990     call PVMFNRECV( -1, msg911, kmn )

          if( kmn .gt. 0 .and. ksh .eq. 1 ) then         ! provisorio
              call PVMFUNPACK( INTEGER4, kddead, 1, 1, info )
              write(*,*)' FALHA DE HOST CONFIRMADA ....!'

c ------------------------
c Corrige a Falha de HOST 
c ------------------------

              call dynamic(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &grupo1,grupo2,xhost,ahost,nhost,xhostdel,xhostadd,kselect,tids,
     &nsize,ip,iddead,nfalha)
              ksh = 0                      ! falha sanada
    
          else
              if( ksh .eq. 1 ) goto 990    ! ocorreu falha
          endif

c ----------------------------------------------------------------------



          if( ksinal .eq. 1000 .and. nchave .eq. 1 ) then

c -----------------------------------------------
c Transmite aos Processos PCX Dados e Resultados 
c -----------------------------------------------
         
              call comunica(ce,cle,NPROC,NPROC1,NPROC2,NPROC3,kselect,
     &tids)

              write(*,*)' --> TRANSMITIDA A BASE DE DADOS !!!'

              nchave = 0                         ! acabou de comunicar

          endif                                  ! if( ksinal .eq. 1000 .and. nchave .eq. 1 ) then
          if( ksinal .eq. 2000 ) goto 2050       ! ESPERAR ULTIMO SINAL DO MANAGER

          goto 8                                 ! verifica o buffer
      else
          if( kselect .eq. kselmax .and. ksinal .eq. 2000 ) goto 2050   ! ESPERAR ULTIMO SINAL DO MANAGER
          if( kselect .eq. kselmax .and. ksinal .ne. 2000 ) goto 8      ! Verificar o "Buffer"
          if( kselect .lt. kselmax .and. nchave .eq. 0 ) then

c -------------------------------------------------
c Entra com os Dados e Obtem o Estado do Caso Base
c ------------------------------------------------- 

              kselect = kselect + 1 

              call input(kselect)

              call fcarga(ce,cle)

              write(*,*)'                         '
              write(*,*)' --> CASO BASE PRONTO !!!'
              write(*,*)'                         '

           
              nchave = 1                             ! acabou de calcular FCAC
          endif                                      ! if( kselect .lt. kselmax .and. nchave .eq. 0 ) then

          goto 8                                     ! Verificar o "Buffer"
      endif                                          ! if( knl .gt. 0 )





c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

 2050 continue                                   ! ESPERAR ULTIMO SINAL DO MANAGER



c -----------------------------------------------------
      dummy = t_stop() ! desativa o relogio -- time(2)

      time(2) = t_getrtime()
c -----------------------------------------------------
c -----------------------------------------------------
      dummy = t_start()   ! ativa o relogio -- time(3)
c -----------------------------------------------------


c -------------------------------------
c Esperar Pelo Ultimo Sinal do MANAGER
c -------------------------------------
    9 call PVMFNRECV( tids(NPROC),msg11,lnt )

      if( lnt .gt. 0 ) then
          call PVMFUNPACK( INTEGER4,ksinal,1,1,info ) 
          write(*,*)' --> RECEBI O SINAL DO MANAGER --> FIM !'
          goto 1050                              ! FINALIZAR   
      else
          goto 9                                 ! verifica o buffer
      endif                                      ! if( lnt .gt. 0 )




 1050 continue                          ! ENVIAR MENSAGEM PARA FINALIZAR


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------



c -------------------------------------------------------------------
c Envia Mensagem a Todos os Programas Para Finalizar o Processamento 
c -------------------------------------------------------------------

      do ii = 1, NPROC
         kaviso = 2000
         call PVMFINITSEND( PVMDEFAULT, info )
         call PVMFPACK( INTEGER4,kaviso,1,1,info )

         call PVMFSEND( tids(ii),msg4,info )
         if( info .lt. 0 ) write(*,*)' ERRO EM SEND'
      enddo           




        
c -----------------------------------------------------
      dummy = t_stop() ! desativa o relogio -- time(3)

      time(3) = t_getrtime()
c -----------------------------------------------------




c --------------------------------------------------
c Escreve em DISCO Resultados de Tempos do Programa 
c --------------------------------------------------
  
      call clock(nhost,NPROC)


        
      write(*,*)' FIM DO PROCESSAMENTO DA ANALISE DE CONTINGENCIA !!!'


c -------------------------  
c Abandona o grupo e o PVM
c -------------------------
c Processamento concluido. Assegure que todos os membros tenham terminado antes de sair.

      call PVMFBARRIER( 'contingencia', -1, info )

      call PVMFLVGROUP( 'contingencia', info ) 



c --------------------------------------------------- 
c Mata Todos os Processos PVM Desovados Pelo GERENTE
c ---------------------------------------------------
cc      do ip = 1, NPROC
cc        call PVMFKILL( tids(ip), info )   
cc      enddo  
     

      call PVMFEXIT( info )

      stop  
      end           ! Fim do programa gerente







c-----------------------------------------------------------------------------
      double precision function random(seed)

c-----------------------------------------------------
c  Routine returns a pseudo-random number between 0-1. 
c-----------------------------------------------------

c ------------------------------------------------------------------
c Mudar o valor de seed se quiser alterar o INSTANTE da ocorrencia
c ------------------------------------------------------------------
      integer m, i, md, seed
      double precision fmd

cc      data m/25173/,i/13849/,md/65536/,fmd/65536.d0/,seed/17/
      data m/25173/,i/13849/,md/65536/,fmd/65536.d0/

cc      save seed

      seed   = mod(m*seed+i,md)
      random = seed/fmd
      return
      end
