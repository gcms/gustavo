c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------

      subroutine Father(ce,cle,impr,myinst,
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



c -------------------------------------------------------
c Rotina MasterS(): Modo Paralelo Sincrono
c
c Rotinas MasterA() e Father(): Modo Paralelo Assincrono
c -------------------------------------------------------

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      ksg0 = 110
      ksg1 = 210
      ksg2 = 310
      ksg3 = 410
      ksg4 = 510
      ksg5 = 610
      ksg6 = 710
      ksg7 = 810
      ksg8 = 910
      ksg9 = 1010



c ----------------------------------------------------------------------      
c                    2.] CALCULA CONTINGENCIAS AC 
c ----------------------------------------------------------------------


c ----------------------------------------------------------
c           PROCESSAMENTO PARALELO ASSINCRONO
c ----------------------------------------------------------
c ----------------------------------------------------------
c O LOOP AC eh Executado em Paralelo Assincrono: O Processa-
c dor Assim que Termina seu Envelope de Casos Pega um Outro 
c para Executar 
c ----------------------------------------------------------
c O Controle eh Feito Somente pelo No de tids(NPROC1+1) que 
c Tambem Executa Calculos AC Enquanto Espera
c ----------------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )

      ktid  = tids(NPROC1+1)
      nbase = NPROC2




c -------------------------------------
c Preparando a Carga dos Processadores
c -------------------------------------

      if ( nrc .lt. nbase ) then
           nndim = nrc
           write(*,*)' Toda a lista e menor do que o no. de hosts ! '
           write(*,*)' PCY) REDUZA O TAMANHO DA VM PARA ',nndim,' ! '
           return
      endif

      kdif = 0
      if ( ncrit .lt. nbase ) then
           write(*,*)' Voce tem menos casos do que o no. de hosts ! '
           kdif = NPROC2 - ncrit       ! numero de casos artificiais para completar a carga
      endif


c ------------------------------------------------
c Define o tamanho dos envelopes individuais
c ------------------------------------------------

      nnn = (ncrit+kdif)/NPROC2
      nrest = mod(ncrit+kdif,NPROC2)

      kenv = 2
cc      kenv = nnn      ! mudanca em 29/02/96
      if( kenv .gt. nnn ) kenv = nnn
      write(*,*)' PAI PCY ! PARTICAO --> kenv = ',kenv

      do i = 1, nrc
        jklasif(i) = klasif(i)
      enddo

      inic = ncrit+kdif+1
      do i = inic, nrc
        jklasif(i) = 0
      enddo

      do i = 0, 100
        kvel(i) = 0
      enddo



c ----------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para a Simulacao AC
c ----------------------------------------------------------------------

      if( nbase .eq. 1 ) goto 950



       
c ----------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Dois ou Mais Processadores Para a Simulacao AC
c ----------------------------------------------------------------------------
 
      kount     = 0          ! inicializa o contador geral de casos
      icont0    = 0          ! inicializa o contador de casos processados em "0"
      icontr    = 0          ! inicializa o contador de casos recebidos de outros hosts <> do de tids(NPROC1+1)
      kflag     = 0          ! "flag" para CONTINUAR ou FINALIZAR
      ioutlocal = 0          ! variavel inteira auxiliar
      init      = 0          ! posicao em kvel() onde inicia os casos de contingencia
      kpos      = 0          ! posicao em kvel() onde vai o TID
      kvel(0)   = ktid       ! nas kenv primeiras posicoes de kvel() vao as casos para o tids(NPROC1+1)
 
        
c ----------------------------------------------------
c Manda o primeiro envelope de casos para processar
c ----------------------------------------------------
    
      do i = 1, NPROC2-1
          kp = NPROC1 + i + 1                ! exclui o de tids(menor)
          j = 1
          kpos = kpos + kenv + 1             ! posicao no vetor kvel() onde vai o TID
          kvel(kpos) = tids(kp)              ! TID do processo para onde vai o envelope
          do while ( j .le. kenv )
             if( kount .lt. ncrit + kdif ) then
                 kount = kount + 1
                 kvel(j+kpos) = jklasif(kount) 
             endif
             j = j + 1
          enddo          ! do while

          call PVMFINITSEND( PVMDEFAULT, info )
          call PVMFPACK( INTEGER4, kenv, 1, 1, info ) 
          call PVMFPACK( INTEGER4, kpos, 1, 1, info ) 
          call PVMFPACK( INTEGER4, kvel(kpos), kenv+1, 1, info )  
          call PVMFSEND( tids(kp), ksg2, info )   ! hostdest = i (i = 2, ..., NPROC2-1)   
      enddo


c ------------------------------------------------------------------
c Processo de tids(NPROC1+1) pega 1 envelope de casos para analisar
c ------------------------------------------------------------------

      kpos = 0
      do j = 1,kenv
         ioutlocal = 0
         if( kount .lt. ncrit + kdif ) then
             kount = kount + 1
             kvel(j) = jklasif(kount) 
             ioutlocal = kvel(j)   
         endif

         if( ioutlocal .eq. 0 ) goto 700

             call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

             icont0 = icont0 + 1

c == Contabilidade:
         
             idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID
         
             ioutlocal = 0
             kvel(j) = 0

  700        continue
      enddo


      

c -----------------------------------------------
c Processo de tids(NPROC1+1) verifica o "buffer" 
c -----------------------------------------------
      
      nrecv = 0
      ncalc = 0

    7 call PVMFNRECV( -1, ksg3, knt )

      if ( knt .gt. 0 ) then
        call PVMFUNPACK( INTEGER4, kbusy, 2, 1, info )

        ip = kbusy(1)                            ! tid do hostdest = ip
        kpos = kbusy(2)
        kbusy(1) = 0
        kbusy(2) = 0


c == Contabilidade:

        init = kpos + 1

        do j = init,init + kenv - 1
           ioutlocal = kvel(j)
            
           if( ioutlocal .eq. 0 ) goto 750

               icontr = icontr + 1

               idt(ioutlocal) = ip  ! associa o caso analisado ao TID

               ioutlocal = 0
               kvel(j) = 0

 750           continue
        enddo
        

c == Flag para autorizar saida:

        if( icont0 + icontr .eq. ncrit + kdif ) then
            nrecv = nrecv + 1
            kflag = 9999
        endif


c ------------------------------------------------
c Envia novo envelope de casos para "ip" analisar
c ------------------------------------------------

       j = 1
        kvel(kpos) = ip                ! TID do processo para onde vai o envelope
        do while ( j .le. kenv )
           if( kount .lt. ncrit + kdif ) then
               kount = kount + 1
               kvel(j+kpos) = jklasif(kount) 
           endif
           j = j + 1
        enddo          ! do while

        call PVMFINITSEND( PVMDEFAULT, info )
        call PVMFPACK( INTEGER4, kflag, 1, 1, info ) 
        call PVMFPACK( INTEGER4, kenv, 1, 1, info ) 
        call PVMFPACK( INTEGER4, kpos, 1, 1, info ) 
        call PVMFPACK( INTEGER4, kvel(kpos), kenv+1, 1, info )  
        call PVMFSEND( ip, ksg4, info )    ! envia um caso para o escravo que terminou
 
    
c == Testa saida:

        if( nrecv .eq. NPROC2-1 ) goto 2000
        goto 7

      else

c ------------------------------------------------------------------
c Processo de tids(NPROC1+1) pega 1 envelope de casos para analisar
c ------------------------------------------------------------------

            if( calc .eq. 9999 ) goto 1500
            kpos = 0
            do j = 1,kenv
              ioutlocal = 0
              if( kount .lt. ncrit + kdif ) then
                  kount = kount + 1
                  kvel(j) = jklasif(kount)
                  ioutlocal = kvel(j)
              endif
              if( kount .eq. ncrit + kdif ) ncalc = 9999

              if( ioutlocal .eq. 0 ) goto 800

                  call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

                  icont0 = icont0 + 1

c == Contabilidade:
         
                  idt(ioutlocal) = ktid                                ! associa o caso analisado ao TID

                  ioutlocal = 0
                  kvel(j) = 0

  800             continue
            enddo

 1500   continue

c == Testa saida:

        if( nrecv .eq. NPROC2-1 ) goto 2000
        goto 7
      endif                ! if ( knt )

c -------------------------
c Termino do Processamento
c -------------------------   

 2000 continue            
 


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c --------------------------------------------------------------
c Pega Resultados da Simulacao AC do Processo de tids(NPROC1+1)
c --------------------------------------------------------------
      ndivsoma = ndiv
      nnconvsoma = nnconv
      nyconvsoma = nyconv

c -------------------------------------------------------
c Recebe Resultados da Simulacao AC dos Demais Processos
c -------------------------------------------------------
      
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
        ndiv = isend2(0)
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

      ndiv = ndivsoma
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
      end                       ! Fim da rotina Father()
