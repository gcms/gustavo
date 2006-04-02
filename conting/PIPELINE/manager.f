c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                      [ Explora a Esparsidade de M ]
c                      [  Janeiro / Fevereiro 1995  ]  
c*************************** PROGRAMA PARALELO *************************
c**************************** MODELO PIPELINE **************************
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
c                [ Arquivos componentes: 1) manager.f   ]  
c                [                       2) saida.f     ]
c                [                       3) globvar.inc ]
c                [                       4) varcont.inc ]
c                [                       5) varsel.inc  ]
c                [                       6) tempos.inc  ]
c
c                [ Arquivos de dados:    1) opcoes.dat  ]
c                [                       2) dad.dat     ]
c
c ---------------------
c [ PROGRAMA MANAGER ]
c ---------------------
c
      program manager
c
      include '/opt/pvm3/include/fpvm3.h'
c
      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'

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
      if( myinst .lt. 0 )  write(*,*)' ERRO NO GRUPO contingencia  '




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
      msg811 = 811     ! tag para emergencia na VM
      msg911 = 911     ! tag para emergencia na VM





      
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







      ip = NPROC1+1
      kid = tids(ip)                                      ! tids do PCY Pai

c --------------------------------------
c Recebe Dados Vindos da Tarefa PCY pai
c --------------------------------------

      call PVMFRECV( kid, msg2, info )    ! recebendo do processo PCY pai
       
      call PVMFUNPACK( INTEGER4, NPROC, 1, 1, info )
      call PVMFUNPACK( INTEGER4, tids, NPROC, 1, info )

      call PVMFUNPACK( INTEGER4, kselect, 1, 1, info )
      call PVMFUNPACK( INTEGER4, nb, 1, 1, info )

      call PVMFUNPACK( REAL8, vsend, nb, 1, info )
      call PVMFUNPACK( REAL8, tsend, nb, 1, info )
      call PVMFUNPACK( REAL8, p, nb, 1, info )
      call PVMFUNPACK( REAL8, q, nb, 1, info )

      call PVMFUNPACK( INTEGER4, nrc, 1, 1, info )
      call PVMFUNPACK( INTEGER4, nsobr, 1, 1, info )
      call PVMFUNPACK( INTEGER4, nso, 1, 1, info )

      call PVMFUNPACK( INTEGER4, klasif, nrc+1, 1, info )

      do i = 0, mmax
         call PVMFUNPACK( INTEGER4, kcont(1,i), nrc, 1, info )
      enddo


c == Indices: 

       call PVMFUNPACK( REAL8, pia, nrc, 1, info )
       call PVMFUNPACK( REAL8, piaux, nrc, 1, info )

c == Ilhamentos: 
      
       call PVMFUNPACK( INTEGER4, nilha, 1, 1, info )
       call PVMFUNPACK( INTEGER4, kilha, nilha, 1, info )

c == TIDS dos processos PVM e seus respectivos casos analisados (contabilidade):

       call PVMFUNPACK( INTEGER4, idp, nrc, 1, info )
       call PVMFUNPACK( INTEGER4, idt, nrc, 1, info )

c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes:

       nitens4 = ncorte + 10  ! com margem de seguranca para prevenir ncorte = 0

       call PVMFUNPACK( INTEGER4, isend2,nitens4,1,info )
       call PVMFUNPACK( INTEGER4, isend3,nitens4,1,info )
       call PVMFUNPACK( INTEGER4, isend4,nitens4,1,info )
       call PVMFUNPACK( INTEGER4, isend5,nitens4,1,info )
       call PVMFUNPACK( INTEGER4, isend6,nitens4,1,info )
       if( info .lt. 0 ) write(*,*)' Erro em PVMFUNPACK() !'
        


   
c --------------------------------------------------
c Recupera Resultados da Simulacao de Contingencias 
c --------------------------------------------------

c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes:
    
      ndiv = isend2(0)
      nnconv = isend3(0)
      nyconv = isend4(0)
      nyconv = isend5(0)
      nyconv = isend6(0)
       


c Recuperando kdiverge()
      if( ndiv .ne. 0 ) then
          do j = 1, ndiv
            kdiverge(j) = isend2(j)
          enddo
      endif
        

c Recuperando knotconv()
      if( nnconv .ne. 0 ) then
          do j = 1, nnconv
            knotconv(j) = isend3(j)
          enddo
      endif
        

c Recuperando kconv(), kit1() e kit2()
      if( nyconv .ne. 0 ) then
          do j = 1, nyconv
            kconv(j) = isend4(j)
            kit1(j) = isend5(j)
            kit2(j) = isend6(j)
          enddo
      endif      


      ncrit = klasif(0)
      


c ----------------------------------
c Gera Relatorios de Saida em DISCO 
c ----------------------------------

      call output(kselect)      ! chama rotinas que criam relatorios de saida

cc      write(*,*)' --> MANAGER TERMINOU O PROCESSAMENTO DOS DADOS... ',
cc     &kselect


  




c ------------------------------------
c CONTINUAR Calculando ou FINALIZAR ?
c ------------------------------------
      ksinal = 1000                                ! CONTINUAR
      if( kselect .eq. kselmax ) then

c ----------------------------------------------------------------------------------
c Envia Sinal ao Processo GERENTE Informando Que o MANAGER Terminou o Processamento
c ----------------------------------------------------------------------------------

          ksinal = 2000                            ! FINALIZAR
          call PVMFINITSEND( PVMDEFAULT, info )
          call PVMFPACK( INTEGER4, ksinal,1,1,info )
          call PVMFSEND( mtid, msg11, info )
          write(*,*)' --> MANAGER ENVIA ULTIMO SINAL P/ GERENTE !!'
          goto 9
      endif
          






c -----------------------------------------------------------
c Envia Mensagem ao PCY Informando que o MANAGER Esta PRONTO
c -----------------------------------------------------------
      maviso = 1000                               ! CONTINUAR
      do i = 1, NPROC2
         kp = NPROC1 + i
         call PVMFINITSEND( PVMDEFAULT, info )
         call PVMFPACK( INTEGER4, maviso,1,1,info )
         call PVMFSEND( tids(kp), msg6, info )
      enddo
      if( kselect .lt. kselmax .or. ksinal .eq. 1000 ) goto 8  ! CONTINUAR (receber novos resultados)





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
          if( kselect .eq. kselmax) goto 9        ! Verificar o Buffer (receber mensagem para sair)  
      endif





 1050 continue                                    ! FINALIZAR

      write(*,*)' --> FIM DA SAIDA EM DISCO !!!'

c -------------------------  
c Abandona o grupo e o PVM
c -------------------------
c Processamento concluido. Assegure que todos os membros tenham terminado antes de sair.

      call PVMFBARRIER( 'contingencia', -1, info ) 
          
      call PVMFLVGROUP( 'contingencia', info )
               
      call PVMFEXIT( info )

      stop  
      end           ! Fim do programa MANAGER

