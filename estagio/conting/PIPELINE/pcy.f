c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c ----------------------------------------------------------------------
c                         Faz Contingencia AC 
c ----------------------------------------------------------------------
c                           Varios Metodos
c ----------------------------------------------------------------------
c
c                [ Arquivos componentes: 1) pcy.f       ]
c                [                       2) outage.f    ]
c                [                       3) modif.f     ]
c                [                       4) ident.f     ]
c                [                       5) produto.f   ]
c                [                       6) inversa.f   ]
c                [                       7) svector.f   ]
c                [                       8) anaAC.f     ]
c                [                       9) state.f     ]
c                [                      10) fluxos.f    ]
c                [                      11) master.f    ]
c                [                      12) slave.f     ]
c                [                      13) father.f    ]
c                [                      14) son.f       ]
c                [                      15) potencia.f  ]
c                [                      16) bennett.f   ]
c                [                      17) zollen.f    ]
c                [                      18) globvar.inc ]
c                [                      19) varcont.inc ]
c                [                      20) varsel.inc  ]
c                [                      21) tempos.inc  ]
c
c                [ Arquivos de dados:    1) opcoes.dat  ]
c                [                       2) dad.dat     ]
c
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------
c
      program pcy
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
      integer*4 impr
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

      call  PVMFGSIZE( 'contingencia', nsize )                        ! (17)
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
      msg811 = 811    ! tag para emergencia na VM
      msg911 = 911    ! tag para emergencia na VM


      
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




    8 continue                            ! CONTINUAR






      kid = tids(1)                                      ! tids do PCX Pai
 
c --------------------------------------
c Recebe Dados Vindos da Tarefa PCX pai
c --------------------------------------

      call PVMFRECV( kid, msg1, info )    ! recebendo do processo PCX pai

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

c == Indices: 

      call PVMFUNPACK( REAL8,pia,nrc,1,info )
      call PVMFUNPACK( REAL8,piaux,nrc,1,info )

c == Ilhamentos: 
      
      call PVMFUNPACK( INTEGER4, nilha,1,1,info )
      call PVMFUNPACK( INTEGER4, kilha,nilha,1,info )

c == TIDS dos processos PVM e seus respectivos casos analisados (contabilidade):

      call PVMFUNPACK( INTEGER4, idp,nrc,1,info )


c === Recupera estado do caso base

      do iii = 1,nb   
        v(iii) = vsend(iii)  
        t(iii) = tsend(iii)
        tet(iii) = tsend(iii)         ! provisorio
      enddo



c === Recupera o tamanho da LISTA REDUZIDA

      ncrit = klasif(0)


c === Inicializa contadores a cada entrada de nova bateria de dados

      ndiv   = 0
      nnconv = 0
      nyconv = 0

      do i = 1,ncrit
         idt(i) = 0
      enddo






c ----------------------------
c Calculo de Contingencias AC 
c ----------------------------

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c === Processamento da ANALISE DE CONTINGENCIA: 


      if ( mytid .eq. tids(NPROC1+1) ) then


c ===============
c Executa Master
c ===============

        if( modelo .eq. 0 ) call MasterS(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)        ! modo Sincrono

        if( modelo .eq. 1 ) call MasterA(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)        ! modo Assincrono por unidade

        if( modelo .eq. 2 ) call Father(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)        ! modo Assincrono por conjunto


c --------------------------------------------------
c Prepara Resultados da Simulacao de Contingencias 
c Para Envio ao Processo MANAGER
c --------------------------------------------------



        do j = 1, ndiv
          isend2(j) = kdiverge(j) 
        enddo
        do j = 1, nnconv
          isend3(j) = knotconv(j) 
        enddo
        do j = 1, nyconv
          isend4(j) = kconv(j) 
          isend5(j) = kit1(j)
          isend6(j) = kit2(j)
        enddo
      

        isend2(0) = ndiv
        isend3(0) = nnconv
        isend4(0) = nyconv
        isend5(0) = nyconv
        isend6(0) = nyconv



c ---------------------------------------------------------
c Envia Sinal aos Processos PCX Para Prosseguir Calculando
c ---------------------------------------------------------
        naviso = 1000                   ! PROSSEGUIR
        do ip = 1, NPROC1
           call PVMFINITSEND( PVMDEFAULT, info )
           call PVMFPACK( INTEGER4, naviso,1,1,info )
           call PVMFSEND( tids(ip), msg5, info )
        enddo

      else
        

c ==============
c Executa Slave
c ==============

       if( modelo .eq. 0 ) call SlaveS(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)      ! modo Sincrono

       if( modelo .eq. 1 ) call SlaveA(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)      ! modo Assincrono por unidade

       if( modelo .eq. 2 ) call Son(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)      ! modo Assincrono por conjunto

      endif





c -----------------------------------------------------
c Recebe mensagem do MANAGER informando se esta PRONTO
c -----------------------------------------------------

      if( kselect .eq. 1 ) goto 10

c      write(*,*)' --> SE NAO TEM MENSAGEM P/ FINALIZAR EH P/ CONTINUAR !'
      call PVMFRECV( tids(NPROC), msg6, bufid )
      call PVMFUNPACK( INTEGER4, maviso,1,1,info )

   10 continue




c -------------------------------------------
c Transmite os Dados Para o Processo MANAGER
c -------------------------------------------

      ip = NPROC1 + 1
      if( mytid .eq. tids(ip) ) then

          call PVMFINITSEND( PVMDEFAULT, info )
   
          call PVMFPACK( INTEGER4, NPROC, 1, 1, info )
          call PVMFPACK( INTEGER4, tids, NPROC, 1, info )

          call PVMFPACK( INTEGER4, kselect, 1, 1, info )
          call PVMFPACK( INTEGER4, nb, 1, 1, info )

          call PVMFPACK( REAL8, vsend, nb, 1, info )
          call PVMFPACK( REAL8, tsend, nb, 1, info )
          call PVMFPACK( REAL8, p, nb, 1, info )
          call PVMFPACK( REAL8, q, nb, 1, info )

          call PVMFPACK( INTEGER4, nrc, 1, 1, info )
          call PVMFPACK( INTEGER4, nsobr, 1, 1, info )
          call PVMFPACK( INTEGER4, nso, 1, 1, info )

          call PVMFPACK( INTEGER4, klasif, nrc+1, 1, info )

          do i = 0, mmax
             call PVMFPACK( INTEGER4, kcont(1,i), nrc, 1, info )
          enddo

c == Indices: 

          call PVMFPACK( REAL8, pia, nrc, 1, info )
          call PVMFPACK( REAL8, piaux, nrc, 1, info )

c == Ilhamentos: 
      
          call PVMFPACK( INTEGER4, nilha, 1, 1, info )
          call PVMFPACK( INTEGER4, kilha, nilha, 1, info )

c == TIDS dos processos PVM e seus respectivos casos analisados (contabilidade):

          call PVMFPACK( INTEGER4, idp,nrc,1,info )
          call PVMFPACK( INTEGER4, idt,nrc,1,info )

c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes:

          nitens4 = ncorte + 10 ! com margem de seguranca para prevenir ncorte = 0 
 
          call PVMFPACK( INTEGER4, isend2,nitens4,1,info )
          call PVMFPACK( INTEGER4, isend3,nitens4,1,info )
          call PVMFPACK( INTEGER4, isend4,nitens4,1,info )
          call PVMFPACK( INTEGER4, isend5,nitens4,1,info )
          call PVMFPACK( INTEGER4, isend6,nitens4,1,info )

          call PVMFSEND( tids(NPROC), msg2, info )


      endif                  ! if( mytid .eq. tids(NPROC1+1) )




c ------------------------------------
c CONTINUAR Calculando ou FINALIZAR ?
c ------------------------------------
      if( kselect .lt. kselmax ) goto 8         ! CONTINUAR (receber novos dados e resultados)




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





 1050 continue                               ! FINALIZAR

      write(*,*)' --> SIMULACAO AC PRONTA !!!'

c -------------------------  
c Abandona o grupo e o PVM
c -------------------------
c Processamento concluido. Assegure que todos os membros tenham terminado antes de sair.

      call PVMFBARRIER( 'contingencia', -1, info ) 
      
      call PVMFLVGROUP( 'contingencia', info )

      call PVMFEXIT( info )

      stop
      end                  ! Fim do programa servidor PCY
