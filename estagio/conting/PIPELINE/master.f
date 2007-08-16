c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------

      subroutine MasterA(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)
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
      integer*4 myinst,info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


      dimension nsize2(63),nsize3(63),nsize4(63)            ! local
      dimension jklasif(nlmax)                              ! local


c ------------------------------------------------------
c Rotina MasterS(): Modo Paralelo Sincrono
c
c Rotina MasterA() e Father(): Modo Paralelo Assincrono
c ------------------------------------------------------


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      ksg0 = 11
      ksg1 = 21
      ksg2 = 31
      ksg3 = 41
      ksg4 = 51
      ksg5 = 61
      ksg6 = 71
      ksg7 = 81
      ksg8 = 91
      ksg9 = 101



c ----------------------------------------------------------------------      
c                    2.] CALCULA CONTINGENCIAS AC 
c ----------------------------------------------------------------------


c ------------------------------------------------------------
c             PROCESSAMENTO PARALELO ASSINCRONO
c ------------------------------------------------------------
c ------------------------------------------------------------
c O LOOP AC eh Executado em Paralelo Assincrono: O Processa-
c dor Assim que Termina seu Caso  Pega  um  Novo  Caso para 
c Executar
c ------------------------------------------------------------
c O Controle eh Feito Somente pelo Processo de tids(NPROC1+1)   
c que Tambem Executa Calculos AC Enquanto Espera
c ------------------------------------------------------------



c -------------------------------------
c Preparando a Carga dos Processadores
c -------------------------------------

      nbase = NPROC2
      ktid  = tids(NPROC1+1)

      if( nrc .lt. nbase ) then
          nndim = nrc
          write(*,*)' Toda a lista e menor do que o no. de hosts ! '
          write(*,*)' PCY) REDUZA O TAMANHO DA VM PARA ',nndim,' ! '
          return
      endif

      kdif = 0
      if( ncrit .lt. nbase ) then
          write(*,*)' Voce tem menos casos do que o no. de hosts ! '
          kdif = NPROC2 - ncrit       ! numero de casos artificiais para completar a carga
      endif




      do i = 1, nrc
        jklasif(i) = klasif(i)
      enddo

      inic = kdif + ncrit + 1
      do i = inic, nrc
        jklasif(i) = 0
      enddo
      


c ----------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para a Simulacao AC
c ----------------------------------------------------------------------

      if( nbase .eq. 1 ) goto 950



c ----------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Dois ou Mais Processadores Para a Simulacao AC
c ----------------------------------------------------------------------------
       
      kount  = 0          ! inicializa o contador geral de casos
      icont0 = 0          ! inicializa o contador de casos processados em "0"
      icontr = 0          ! inicializa o contador de casos recebidos de outros hosts <> PCY Pai
 

c -------------------------
c Manda os primeiros casos 
c -------------------------

      do i = 1, NPROC2-1
        kp = NPROC1 + i + 1       ! exclui o de tids(menor)
        if ( kount .lt. ncrit + kdif ) then
          kount = kount + 1
          kbusy(1) = jklasif(kount) 
          kbusy(2) = kount

          call PVMFINITSEND( PVMDEFAULT, info )
          call PVMFPACK( INTEGER4, kbusy, 2, 1, info )  
          call PVMFSEND( tids(kp), ksg2, info )   ! hostdest = i (i = 2, 3, ..., NPROC2-1)
        endif
      enddo
        

c -----------------------------------------------------
c Processo de tids(NPROC1+1) pega 1 caso para analisar
c -----------------------------------------------------

      if ( kount .lt. ncrit + kdif ) then
        kount = kount + 1
        ioutlocal = jklasif(kount)                           ! caso a ser analisado
        call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC
        icont0 = icont0 + 1 

c == Contabilidade:
         
        idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID
      endif

      

c -----------------------------------------------
c Processo de tids(NPROC1+1) verifica o "buffer" 
c -----------------------------------------------
      
    7 call PVMFNRECV( -1, ksg3, knt )

      if ( knt .gt. 0 ) then
        call PVMFUNPACK( INTEGER4, kbusy, 2, 1, info )

        ip = kbusy(1)                            ! tid do hostdest = ip
        nrcaso = kbusy(2)
        icontr = icontr + 1

c == Contabilidade:
         
        idt(jklasif(nrcaso)) = ip                ! associa o caso analisado ao TID

c -----------------------------------
c Envia novo caso para "ip" analisar
c -----------------------------------

        kount = kount + 1              
        kbusy(1) = jklasif(kount) 
        kbusy(2) = kount

        call PVMFINITSEND( PVMDEFAULT, info )
        call PVMFPACK( INTEGER4, kbusy, 2, 1, info )  
        call PVMFSEND( ip, ksg4, info )    ! envia um caso para o escravo que terminou 
 
        if ( icont0 + icontr .eq. ncrit + kdif ) goto 2000
        goto 7
      else

c -----------------------------------------------
c No de tids(NPROC1+1) pega 1 caso para analisar
c -----------------------------------------------

        if( kount .lt. ncrit + kdif ) then
          kount = kount + 1
          ioutlocal = jklasif(kount)                           ! caso a ser analisado

          call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

          icont0 = icont0 + 1

c == Contabilidade:
         
          idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID

          if( icont0 + icontr .eq. ncrit + kdif ) goto 2000
          goto 7
        endif
        goto 7
      endif                ! if ( knt )

      
c -------------------------
c Termino do Processamento
c -------------------------      

 2000 continue             ! ioutlocal = 0 ==> sinal para parar



c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------------------------
c Pega Resultados da Simulacao de Contingencias do
c Processo de tids(NPROC1+1)
c ------------------------------------------------
      ndivsoma = ndiv
      nnconvsoma = nnconv
      nyconvsoma = nyconv

c ------------------------------------------------
c Recebe Resultados da Simulacao de Contingencias 
c dos Demais Processos
c ------------------------------------------------
      
c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes: 

cc      write(*,*)' NO | DIVERG. | CONVERG. | NAO-CONVERG.'
cc      write(*,10)myinst,ndiv,nyconv,nnconv
cc   10 format(2x,i2,5x,i5,7x,i5,7x,i5,/)
     

      do i = 1, NPROC2-1
        kp = NPROC1 + i + 1            ! exclui o de tids(menor)
        call PVMFRECV( tids(kp), ksg5, bufid )
        call PVMFUNPACK( INTEGER4, isend2(0),1,1,info )
        call PVMFUNPACK( INTEGER4, isend3(0),1,1,info )
        call PVMFUNPACK( INTEGER4, isend4(0),1,1,info )
        nsize2(kp) = isend2(0)+1
        nsize3(kp) = isend3(0)+1
        nsize4(kp) = isend4(0)+1
      enddo

      do i = 1, NPROC2-1
        kp = NPROC1 + i + 1            ! exclui o de tids(menor)
        nitens4 = nsize2(kp) 
        nitens5 = nsize3(kp)
        nitens6 = nsize4(kp) 
        call PVMFRECV( tids(kp), ksg6, bufid )
        call PVMFUNPACK( INTEGER4, isend2,nitens4,1,info )
        call PVMFUNPACK( INTEGER4, isend3,nitens5,1,info )
        call PVMFUNPACK( INTEGER4, isend4,nitens6,1,info )
        call PVMFUNPACK( INTEGER4, isend5,nitens6,1,info )
        call PVMFUNPACK( INTEGER4, isend6,nitens6,1,info )
        ndiv   = isend2(0)
        nnconv = isend3(0)
        nyconv = isend4(0)
        nyconv = isend5(0)
        nyconv = isend6(0)

       

c Recuperando kdiverge()
        if ( ndiv .ne. 0 ) then
          do j = 1, ndiv
            kdiverge(j+ndivsoma) = isend2(j)
          enddo
          ndivsoma = ndivsoma + ndiv
        endif
        

c Recuperando knotconv()
        if ( nnconv .ne. 0 ) then
          do j = 1, nnconv
            knotconv(j+nnconvsoma) = isend3(j)
          enddo
          nnconvsoma = nnconvsoma + nnconv
        endif
        

c Recuperando kconv(), kit1() e kit2()
        if ( nyconv .ne. 0 ) then
          do j = 1, nyconv
            kconv(j+nyconvsoma) = isend4(j)
            kit1(j+nyconvsoma) = isend5(j)
            kit2(j+nyconvsoma) = isend6(j)
          enddo
          nyconvsoma = nyconvsoma + nyconv
        endif
      enddo

      ndiv   = ndivsoma
      nnconv = nnconvsoma
      nyconv = nyconvsoma



c ------------
c Da Um Salto
c ------------
      if( nbase .gt. 1 ) goto 1000
 
  

 
  950 continue
c ----------------------------------------------------------------------     
c                      2.] CALCULA CONTINGENCIAS AC 
c ----------------------------------------------------------------------
c ----------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para a Simulacao AC
c ----------------------------------------------------------------------
      do i = 1, ncrit

          ioutlocal = jklasif(i)
ccc          print*,' NR do Caso a Ser Analisado = ',ioutlocal

          call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

c == Contabilidade:
         
          idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID

      enddo
c ------------------------------------------------------------------------



 1000 continue


      return
      end                       ! Fim da rotina MasterA()








c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------

      subroutine MasterS(ce,cle,impr,myinst,
     &NPROC1,NPROC2,NPROC3,NPROC,indice1,indice2,tids)
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
      integer*4 myinst,mytid,mynode,info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


      dimension nsize1(63),nsize2(63),nsize3(63),nsize4(63) ! local


c ------------------------------------------------------
c Rotina MasterS(): Modo Paralelo Sincrono
c
c Rotina MasterA() e Father(): Modo Paralelo Assincrono
c ------------------------------------------------------


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      ksg0 = 11
      ksg1 = 21
      ksg2 = 31
      ksg3 = 41
      ksg4 = 51
      ksg5 = 61
      ksg6 = 71
      ksg7 = 81
      ksg8 = 91
      ksg9 = 101



c ----------------------------------------------------------------------      
c                    2.] CALCULA CONTINGENCIAS AC 
c ----------------------------------------------------------------------


c ------------------------------------------------------
c           PROCESSAMENTO PARALELO SINCRONO
c ------------------------------------------------------
c ------------------------------------------------------
c O LOOP AC e Executado em Paralelo Sincrono: Os Casos 
c Sao Previamente Divididos Entre Hosts na Forma Balan-
c ceada em Numero de Casos 
c ------------------------------------------------------



c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )

c -------------------------------------------------------------
c Define o "instance number" deste processo no contexto de PCY
c -------------------------------------------------------------

c -------------------------------------------------------------
c OBS: Isto eh feito aqui exclusivamente com o fim de permitir
c      o processamento paralelo SINCRONO (sem erros).
c -------------------------------------------------------------

      do i = 1, NPROC2
         kp = NPROC1 + i
         if( mytid .eq. tids(kp) ) mynode = i-1
      enddo

      nbase = NPROC2
      ktid = tids(NPROC1+1)
c234567
      if( nbase .eq. 1 ) then

c ----------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para a Simulacao AC
c ----------------------------------------------------------------------
      
c --------------------------------------------
c Apenas o Processador "0" Executa [ serial ]
c --------------------------------------------

	do ll = 1, ncrit

          ioutlocal = klasif(ll)                               ! caso a ser analisado

          call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

c == Contabilidade:
         
          idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID

	enddo

      else          ! if ( nbase )      


c ------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Mais de Um Processador Para a Simulacao AC
c ------------------------------------------------------------------------

       
c ---------------------------------------------
c Todos os Processadores Executam [ paralelo ]
c ---------------------------------------------

        do kk = mynode + 1, ncrit, nbase

	  ioutlocal = klasif(kk)                              ! caso a ser analisado

          call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids) ! esta rotina faz chamadas da contingencia AC

c == Contabilidade:
         
          idt(ioutlocal) = ktid                               ! associa o caso analisado ao TID
       
        enddo




c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------------------------
c Pega Resultados da Simulacao de Contingencias do
c Processo de tids(NPROC1+1)
c ------------------------------------------------
      ndivsoma   = ndiv
      nnconvsoma = nnconv
      nyconvsoma = nyconv

c ------------------------------------------------
c Recebe Resultados da Simulacao de Contingencias 
c dos Demais Processos
c ------------------------------------------------
      
c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes: 

cc      write(*,*)' NO | DIVERG. | CONVERG. | NAO-CONVERG.'
cc      write(*,10)myinst,ndiv,nyconv,nnconv
cc   10 format(2x,i2,5x,i5,7x,i5,7x,i5,/)
     

      do i = 1, NPROC2-1
        kp = NPROC1 + i + 1            ! exclui o de tids(menor)
        call PVMFRECV( tids(kp), ksg5, bufid )
        call PVMFUNPACK( INTEGER4, isend1(0),1,1,info )  
        call PVMFUNPACK( INTEGER4, isend2(0),1,1,info )
        call PVMFUNPACK( INTEGER4, isend3(0),1,1,info )
        call PVMFUNPACK( INTEGER4, isend4(0),1,1,info )
        nsize1(kp) = isend1(0)+1
        nsize2(kp) = isend2(0)+1
        nsize3(kp) = isend3(0)+1
        nsize4(kp) = isend4(0)+1
      enddo

      do i = 1, NPROC2-1
        kp = NPROC1 + i + 1            ! exclui o de tids(menor)
        nitens3 = nsize1(kp) 
        nitens4 = nsize2(kp) 
        nitens5 = nsize3(kp)
        nitens6 = nsize4(kp) 
        call PVMFRECV( tids(kp), ksg6, bufid )
        call PVMFUNPACK( INTEGER4, ksend,nitens3,1,info )
        call PVMFUNPACK( INTEGER4, isend1,nitens3,1,info )
        call PVMFUNPACK( INTEGER4, isend2,nitens4,1,info )
        call PVMFUNPACK( INTEGER4, isend3,nitens5,1,info )
        call PVMFUNPACK( INTEGER4, isend4,nitens6,1,info )
        call PVMFUNPACK( INTEGER4, isend5,nitens6,1,info )
        call PVMFUNPACK( INTEGER4, isend6,nitens6,1,info )
        iconti = isend1(0)
        ndiv   = isend2(0)
        nnconv = isend3(0)
        nyconv = isend4(0)
        nyconv = isend5(0)
        nyconv = isend6(0)

       
c Recuperando idt()
        if ( iconti .ne. 0 ) then
          do j = 1, iconti
             kk = isend1(j)
             idt(kk) = ksend(j)
          enddo
        endif


c Recuperando kdiverge()
        if ( ndiv .ne. 0 ) then
          do j = 1, ndiv
            kdiverge(j+ndivsoma) = isend2(j)
          enddo
          ndivsoma = ndivsoma + ndiv
        endif
        

c Recuperando knotconv()
        if ( nnconv .ne. 0 ) then
          do j = 1, nnconv
            knotconv(j+nnconvsoma) = isend3(j)
          enddo
          nnconvsoma = nnconvsoma + nnconv
        endif
        

c Recuperando kconv(), kit1() e kit2()
        if ( nyconv .ne. 0 ) then
          do j = 1, nyconv
            kconv(j+nyconvsoma) = isend4(j)
            kit1(j+nyconvsoma) = isend5(j)
            kit2(j+nyconvsoma) = isend6(j)
          enddo
          nyconvsoma = nyconvsoma + nyconv
        endif
      enddo

      ndiv   = ndivsoma
      nnconv = nnconvsoma
      nyconv = nyconvsoma


      endif          ! if ( nbase )      

      return
      end                       ! Fim da rotina MasterS()
