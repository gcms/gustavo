c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c ----------------------------------------------------------------------
c                 Faz Contingencia DC Incremental
c ----------------------------------------------------------------------
c               "FAST LINEAR CONTINGENCY ANALYSIS"
c ----------------------------------------------------------------------
c
c                [ Arquivos componentes: 1) pcx.f       ]
c                [                       2) dcincr.f    ]
c                [                       3) modif.f     ]
c                [                       4) ident.f     ]
c                [                       5) produto.f   ]
c                [                       6) inversa.f   ]
c                [                       7) svector.f   ]
c                [                       8) anaDC.f     ]
c                [                       9) mestre.f    ]
c                [                      10) escravo.f   ]
c                [                      11) pai.f       ]
c                [                      12) filho.f     ]
c                [                      13) fluxos.f    ]
c                [                      14) globvar.inc ]
c                [                      15) varcont.inc ]
c                [                      16) varsel.inc  ]
c                [                      17) tempos.inc  ]
c
c                [ Arquivos de dados:    1) opcoes.dat  ]
c                [                       2) dad.dat     ]
c
c
c ------------------------------
c [ PROGRAMA PCX  == SERVIDOR ]
c ------------------------------
c
      program pcx
c
      include '/opt/pvm3/include/fpvm3.h'
c
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
      integer*4 mytid,info,myinst,bufid
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)

      character*32 allhost(8)

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


c --------------------------
c Envolve o programa no PVM 
c --------------------------

      call PVMFMYTID( mytid )

c -------------------------------
c Fornece o ID da tarefa gerente
c -------------------------------

      call PVMFPARENT( mtid )

c ----------------------------------
c Juntando este processo a um grupo
c ----------------------------------

      call PVMFJOINGROUP( 'contingencia', myinst ) 
      if( myinst .lt. 0 ) write(*,*)' ERRO NO GRUPO contingencia '




c === Retorna o no. de membros que fazem parte do grupo:


      call  PVMFGSIZE( 'contingencia', nsize )                     
cc      write(*,*)' O grupo CONTINGENCIA tem ',nsize,' processos ...'




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





c --------------------------------------
c Recebe Dados Vindos da Tarefa GERENTE
c --------------------------------------

  99  call PVMFNRECV( mtid, msgtype1, bufid )    ! recebendo do processo GERENTE

      if( bufid .gt. 0 ) then
          call PVMFUNPACK( INTEGER4, kselmax, 1, 1, info )
          call PVMFUNPACK( INTEGER4, NPROC1, 1, 1, info )
          call PVMFUNPACK( INTEGER4, NPROC2, 1, 1, info )
          call PVMFUNPACK( INTEGER4, NPROC3, 1, 1, info )
          call PVMFUNPACK( INTEGER4, NPROC, 1, 1, info )
          call PVMFUNPACK( INTEGER4, tids, NPROC, 1, info )
          call PVMFUNPACK( INTEGER4, indice1, NPROC1, 1, info )
          call PVMFUNPACK( INTEGER4, indice2, NPROC2, 1, info )

          call PVMFUNPACK( INTEGER4, modelo, 1, 1, info )  ! de 'opcoes.dat'
          call PVMFUNPACK( INTEGER4, lim, 1, 1, info )
          call PVMFUNPACK( INTEGER4, limite, 1, 1, info )
          call PVMFUNPACK( INTEGER4, ncorte, 1, 1, info )
          call PVMFUNPACK( INTEGER4, kformat, 1, 1, info )

          call PVMFUNPACK( REAL8, tol, 1, 1, info )
          call PVMFUNPACK( REAL8, toler, 1, 1, info )

          call PVMFUNPACK( REAL8, fcont, 1, 1, info )  
          call PVMFUNPACK( REAL8, corte, 1, 1, info )

          call PVMFUNPACK( INTEGER4, nb, 1, 1, info )
          call PVMFUNPACK( INTEGER4, ncb, 1, 1, info )
          call PVMFUNPACK( INTEGER4, ng, 1, 1, info )

          call PVMFUNPACK( INTEGER4, itipo, nb, 1, info )
          call PVMFUNPACK( INTEGER4, nex, nb, 1, info )

          call PVMFUNPACK( REAL8, tet, nb, 1, info )
          call PVMFUNPACK( REAL8, ves, nb, 1, info )
          call PVMFUNPACK( REAL8, pes, nb, 1, info )
          call PVMFUNPACK( REAL8, qes, nb, 1, info )
          call PVMFUNPACK( REAL8, pca, nb, 1, info )
          call PVMFUNPACK( REAL8, qca, nb, 1, info )
          call PVMFUNPACK( REAL8, qmin, nb, 1, info )
          call PVMFUNPACK( REAL8, qmax, nb, 1, info )
          call PVMFUNPACK( REAL8, gdiag, nb, 1, info )
          call PVMFUNPACK( REAL8, bdiag, nb, 1, info )
          call PVMFUNPACK( REAL8, shu, nb, 1, info )

          call PVMFUNPACK( REAL8, carga, 1, 1, info )

          call PVMFUNPACK( INTEGER4, nl, 1, 1, info )
          call PVMFUNPACK( INTEGER4, ncl, 1, 1, info ) 

          call PVMFUNPACK( INTEGER4, ni, nl, 1, info )
          call PVMFUNPACK( INTEGER4, nf, nl, 1, info )

          call PVMFUNPACK( REAL8, xkl, nl, 1, info )
          call PVMFUNPACK( REAL8, gkl, nl, 1, info )
          call PVMFUNPACK( REAL8, bkl, nl, 1, info )
          call PVMFUNPACK( REAL8, b1kl, nl, 1, info )
          call PVMFUNPACK( REAL8, b2kl, nl, 1, info )
          call PVMFUNPACK( REAL8, fmi, nl, 1, info )
          call PVMFUNPACK( REAL8, fma, nl, 1, info )
          call PVMFUNPACK( REAL8, ylin, nl, 1, info )
          call PVMFUNPACK( REAL8, tap, nl, 1, info )
      
c234567
      else
          goto 99
      endif                ! if( bufid .gt. 0 ) 






c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

    8 continue                                            ! CONTINUAR







c --------------------------------------
c Recebe Dados Vindos da Tarefa GERENTE
c --------------------------------------

      call PVMFRECV( mtid, msg0, info )    ! recebendo do processo GERENTE

      call PVMFUNPACK( INTEGER4, NPROC, 1, 1, info )
      call PVMFUNPACK( INTEGER4, tids, NPROC, 1, info )

      call PVMFUNPACK( INTEGER4, kselect, 1, 1, info )

      call PVMFUNPACK( INTEGER4, nb, 1, 1, info )
      call PVMFUNPACK( INTEGER4, lfmax, 1, 1, info )

      call PVMFUNPACK( REAL8, vsend, nb, 1, info )
      call PVMFUNPACK( REAL8, tsend, nb, 1, info )
      call PVMFUNPACK( REAL8, p, nb, 1, info )
      call PVMFUNPACK( REAL8, q, nb, 1, info )
      
      call PVMFUNPACK( REAL8, ce, lfmax, 1, info )
      call PVMFUNPACK( REAL8, cle, lfmax, 1, info )

      call PVMFUNPACK( INTEGER4, lcol, nb, 1, info )
      call PVMFUNPACK( INTEGER4, noze, nb, 1, info )
      call PVMFUNPACK( INTEGER4, nseq, nb, 1, info )
      call PVMFUNPACK( INTEGER4, lrd, nb, 1, info )
      call PVMFUNPACK( INTEGER4, itag, lfmax, 1, info )
      call PVMFUNPACK( INTEGER4, lnxt, lfmax, 1, info )

      call PVMFUNPACK( INTEGER4, nrc, 1, 1, info )
      call PVMFUNPACK( INTEGER4, nsobr, 1, 1, info )
      call PVMFUNPACK( INTEGER4, nso, 1, 1, info )

      call PVMFUNPACK( INTEGER4, klasif, nrc+1, 1, info )

      do i = 0, mmax
         call PVMFUNPACK( INTEGER4, kcont(1,i), nrc, 1, info )
      enddo
            
      call PVMFUNPACK( INTEGER4, next, nb, 1, info )


     

c === Recupera estado do caso base

      do iii = 1,nb   
        v(iii) = vsend(iii)  
        t(iii) = tsend(iii)
        tet(iii) = tsend(iii)         ! provisorio
      enddo



c === O tamanho da LISTA BRUTA e nrc

     

c === Inicializa contadores a cada entrada de nova bateria de dados

      nilha = 0   ! inicializa contador de ilhamentos detectados pelo processador corrente
      ncrit = 0   ! inicializa contador da LISTA REDUZIDA


c === Limpa as posicoes dos INDICES DE SEVERIDADE

      do i = 1,nrc
         pia(i) = 0.
         piaux(i) = 0.
         idp(i) = 0
      enddo





c -----------------------------------------
c Calculo do DC Incremental e do Indice PI 
c -----------------------------------------

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ----------------------------------------------------------------------      
c         1.] CALCULA CONTINGENCIAS PELO METODO DC INCREMENTAL
c ---------------------------------------------------------------------- 
c                   PRE - PROCESSAMENTO "SCREENING"
c ----------------------------------------------------------------------



c ------------------------------------------------------------
c Obtem os Fluxos Ativos do Caso Base (Todos os PCX Executam)
c ------------------------------------------------------------


      call fluxativo ! calcula fluxos ativos p/ o caso base --> fpjk() p/ DC Incr.




c ---------------------------------
c Calculo do Indice PI e Ordenacao 
c ---------------------------------



      if ( mytid .eq. tids(1) ) then


c ===============
c Executa Mestre
c ===============

        if( modelo .eq. 0 ) call MestreS(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                                ! modo Sincrono


        if( modelo .eq. 1 ) call MestreA(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                                ! modo Assincrono por unidade


        if( modelo .eq. 2 ) call Pai(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                                ! modo Assincrono por conjunto


c ----------------------------------------
c Ordena a lista de indices de severidade
c ----------------------------------------

        call reordena     ! reordena a lista de indices de severidade
      
cc        write(*,*)'                         '
cc        write(*,*)' --> RANKING FEITO... !!!'
cc        write(*,*)'                         '



c ----------------------------------------------------------------
c Prepara Resultados do DC Incremental Para Envio ao Processo PCY
c ----------------------------------------------------------------
 
c == Indices: 

        isend1(0) = nrc - nilha
        klasif(0) = ncrit            ! tamanho da LISTA REDUZIDA
      




c -----------------------------------------------------------
c Envia Sinal ao Processo GERENTE Para Prosseguir Calculando
c -----------------------------------------------------------
        ksinal = 1000                                ! PROSSEGUIR
        if( kselect .eq. kselmax ) ksinal = 2000     ! FINALIZAR
        call PVMFINITSEND( PVMDEFAULT, info )
        call PVMFPACK( INTEGER4, ksinal,1,1,info )
        call PVMFSEND( mtid, msg3, info )
ccc      write(*,*)' --> PCX ENVIEI SINAL P/ GERENTE CONTINUAR/FINALIZAR !'

      else
        

c ================
c Executa Escravo
c ================

       if( modelo .eq. 0 ) call EscravoS(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                               ! modo Sincrono


       if( modelo .eq. 1 ) call EscravoA(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                               ! modo Assincrono por unidade


       if( modelo .eq. 2 ) call Filho(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)                               ! modo Assincrono por conjunto

      endif





c ------------------------------------------------------
c Recebe Mensagem do PCY Pai Informando Que Esta PRONTO
c ------------------------------------------------------

      if( kselect .eq. 1 ) goto 10

ccc      write(*,*)' --> SE NAO TEM MENSAGEM P/ FINALIZAR E P/ CONTINUAR !'
      kp = NPROC1 + 1
      call PVMFRECV( tids(kp), msg5, bufid )
      call PVMFUNPACK( INTEGER4, naviso,1,1,info )

   10 continue




c -----------------------------------------
c Transmite os Dados Para os Processos PCY
c -----------------------------------------
      if( mytid .eq. tids(1) ) then     

        do 650 ik = 1,NPROC2
          ip = NPROC1 + ik

          call PVMFINITSEND( PVMDEFAULT, info )
   
           
          call PVMFPACK( INTEGER4, NPROC, 1, 1, info )
          call PVMFPACK( INTEGER4, tids, NPROC, 1, info )

          call PVMFPACK( INTEGER4, kselect, 1, 1, info )

          call PVMFPACK( INTEGER4, nb, 1, 1, info )
          call PVMFPACK( INTEGER4, lfmax, 1, 1, info )

          call PVMFPACK( REAL8, vsend, nb, 1, info )
          call PVMFPACK( REAL8, tsend, nb, 1, info )
          call PVMFPACK( REAL8, p, nb, 1, info )
          call PVMFPACK( REAL8, q, nb, 1, info )

          call PVMFPACK( REAL8, ce, lfmax, 1, info )
          call PVMFPACK( REAL8, cle, lfmax, 1, info )

          call PVMFPACK( INTEGER4, lcol, nb, 1, info )
          call PVMFPACK( INTEGER4, noze, nb, 1, info )
          call PVMFPACK( INTEGER4, nseq, nb, 1, info )
          call PVMFPACK( INTEGER4, lrd, nb, 1, info )
          call PVMFPACK( INTEGER4, itag, lfmax, 1, info )
          call PVMFPACK( INTEGER4, lnxt, lfmax, 1, info )
      
          call PVMFPACK( INTEGER4, nrc, 1, 1, info )
          call PVMFPACK( INTEGER4, nsobr, 1, 1, info )
          call PVMFPACK( INTEGER4, nso, 1, 1, info )

          call PVMFPACK( INTEGER4, klasif, nrc+1, 1, info )

          do i = 0, mmax
            call PVMFPACK( INTEGER4, kcont(1,i), nrc, 1, info )
          enddo

          call PVMFPACK( INTEGER4, next, nb, 1, info )

c == Indices: 

          call PVMFPACK( REAL8,pia,nrc,1,info )
          call PVMFPACK( REAL8,piaux,nrc,1,info )

c == Ilhamentos: 
      
          call PVMFPACK( INTEGER4, nilha,1,1,info )
          call PVMFPACK( INTEGER4, kilha,nilha,1,info )

c == TIDS dos processos PVM e seus respectivos casos analisados (contabilidade):

          call PVMFPACK( INTEGER4, idp,nrc,1,info )

          call PVMFSEND( tids(ip), msg1, info )

  650 continue


      endif                  ! if( mytid .eq. tids(1) )






c ------------------------------------
c CONTINUAR Calculando ou FINALIZAR ?
c ------------------------------------
      if( kselect .lt. kselmax ) goto 8           ! CONTINUAR (receber novos dados e resultados)




c -------------------------------------------------------------------
c Verifica Se Tem Mensagem do GERENTE Para Finalizar o Processamento
c -------------------------------------------------------------------

    9 call PVMFNRECV( mtid,msg4,knn )             ! recebendo do processo GERENTE

      if( knn .gt. 0 ) then
          call PVMFUNPACK( INTEGER4,kaviso,1,1,info )
          goto 1050                               ! FINALIZAR
      else
c ----------------------------------------------------------------------------------
c Se Nao Tem Mensagem do GERENTE Para FINALIZAR Entao Continue Verificando o Buffer
c ----------------------------------------------------------------------------------
          if( kselect .eq. kselmax ) goto 9       ! Verificar o Buffer (receber mensagem para sair)  
      endif





 1050 continue                   ! FINALIZA

      write(*,*)' --> DC INCREMENTAL PRONTO !!!'

c -------------------------  
c Abandona o grupo e o PVM
c -------------------------
c Processamento concluido. Assegure que todos os membros tenham terminado antes de sair.

      call PVMFBARRIER( 'contingencia', -1, info )  
  
      call PVMFLVGROUP( 'contingencia', info ) 
       
      call PVMFEXIT( info )

      stop
      end                  ! Fim do programa servidor PCX




