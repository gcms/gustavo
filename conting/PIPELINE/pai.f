c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCX  == SERVIDOR ]
c ------------------------------

      subroutine Pai(ce,impr,myinst,NPROC1,NPROC2,
     &NPROC3,NPROC,indice1,indice2,tids)
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
      dimension ce(max)
      integer*4 impr

c
c Declaracao Relativa ao PVM
c
      integer*4 myinst,info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


      dimension nsize2(63)                                  ! local
      dimension jklasif(nlmax)                              ! local


c ----------------------------------------------------
c Rotina MestreS(): Modo Paralelo Sincrono
c
c Rotinas MestreA() e Pai(): Modo Paralelo Assincrono
c ----------------------------------------------------

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      nsg1 = 21
      nsg2 = 31
      nsg3 = 41
      nsg4 = 51
      nsg5 = 61
      nsg6 = 71



c ----------------------------------------------------------------------      
c                    1.] CALCULA DC INCREMENTAL 
c ----------------------------------------------------------------------


c ------------------------------------------------
c       PROCESSAMENTO PARALELO ASSINCRONO
c ------------------------------------------------
c ------------------------------------------------
c O LOOP DC e Executado em Paralelo Assincrono: O 
c Processador Assim que Termina seu Caso Pega  um
c Novo Envelope de Casos para Executar 
c ------------------------------------------------
c O Controle e  Feito Somente pelo No de tids(1)   
c que Tambem Executa Calculos DC Enquanto Espera
c ------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )



c ------------------------------------------------
c Define o tamanho dos envelopes individuais
c ------------------------------------------------

      nbase = NPROC1
      nnn = nrc/NPROC1
      nrest = mod(nrc,NPROC1)

      kenv = 5
cc      kenv = nnn          ! mudanca em 29/02/96
      if( kenv .gt. nnn ) kenv = nnn
      write(*,*)' PAI PCX ! PARTICAO --> kenv = ',kenv

      do i = 1, nrc
        jklasif(i) = i
      enddo

      jklasif(nrc+1) = 0  ! cartao flag

      do i = 0,100
         kvel(i) = 0    
      enddo



c ------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para o DC Incremental
c ------------------------------------------------------------------------
      if( nbase .eq. 1 ) goto 950



c ------------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Dois ou Mais Processadores Para o DC Incremental
c ------------------------------------------------------------------------------

      kount     = 0          ! inicializa o contador geral de casos
      icont0    = 0          ! inicializa o contador de casos processados em "0"
      icontr    = 0          ! inicializa o contador de casos recebidos de outros hosts <> do de tids(1)
      kflag     = 0          ! "flag" para CONTINUAR ou FINALIZAR
      ioutlocal = 0          ! variavel inteira auxiliar
      init      = 0          ! posicao em kvel() onde inicia os casos de contingencia
      kpos      = 0          ! posicao em kvel() onde vai o TID
      kvel(0)   = tids(1)    ! nas kenv primeiras posicoes de kvel() vao as casos para o tids(1)

               
              

c ----------------------------------------------------
c Manda o primeiro envelope de casos para processar
c ----------------------------------------------------

      do i = 1, NPROC1-1
          kp = i + 1       ! exclui o de tids(1)
          j = 1
          kpos = kpos + kenv + 1             ! posicao no vetor kvel() onde vai o TID
          kvel(kpos) = tids(kp)              ! TID do processo para onde vai o envelope
          do while ( j .le. kenv )
             if( kount .lt. nrc ) then
                 kount = kount + 1
                 kvel(j+kpos) = jklasif(kount) 
             endif
             j = j + 1
          enddo          ! do while

          call PVMFINITSEND( PVMDEFAULT, info )
          call PVMFPACK( INTEGER4, kenv, 1, 1, info ) 
          call PVMFPACK( INTEGER4, kpos, 1, 1, info ) 
          call PVMFPACK( INTEGER4, kvel(kpos), kenv+1, 1, info )  
          call PVMFSEND( tids(kp), nsg2, info )   ! hostdest = i (i = 2, 3, ..., NPROC1)
      enddo
        
     
c -----------------------------------------------------------
c Processo de tids(1) pega 1 envelope de casos para analisar
c -----------------------------------------------------------

      kpos = 0
      do j = 1,kenv
         ioutlocal = 0
         if( kount .lt. nrc ) then
             kount = kount + 1
             kvel(j) = jklasif(kount)
             ioutlocal = kvel(j)
         endif

         if( ioutlocal .eq. 0 ) goto 700

         call contingenciaDC(ce,impr,myinst,NPROC1) ! esta rotina faz chamadas do DC Incremental
          
         icont0 = icont0 + 1                        ! incrementa o contador de casos feitos pelo mestre

c == Contabilidade:
         
         idp(kount) = tids(1)                       ! associa o caso analisado ao TID

         ioutlocal = 0
         kvel(j) = 0

  700    continue
      enddo

      

c ----------------------------------------
c Processo de tids(1) verifica o "buffer" 
c ----------------------------------------
      
      nrecv = 0
      ncalc = 0
      
    7 call PVMFNRECV( -1, nsg3, lnt )

      if ( lnt .gt. 0 ) then
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

               idp(ioutlocal) = ip  ! associa o caso analisado ao TID
 
               ioutlocal = 0
               kvel(j) = 0

 750           continue
        enddo



c == Flag para autorizar saida:

        if( icont0 + icontr .eq. nrc ) then
            nrecv = nrecv + 1
            kflag = 9999
        endif


c ------------------------------------------------
c Envia novo envelope de casos para "ip" analisar
c ------------------------------------------------

        j = 1
        kvel(kpos) = ip                 ! TID do processo para onde vai o envelope
        do while ( j .le. kenv )
           if( kount .lt. nrc ) then
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
        call PVMFSEND( ip, nsg4, info )    ! envia um novo envelope de casos para o escravo que terminou 

c == Testa saida:
     
        if( nrecv .eq. NPROC1-1 ) goto 2000 
        goto 7

      else

c -----------------------------------------------------------
c Processo de tids(1) pega 1 envelope de casos para analisar
c -----------------------------------------------------------

           if( calc .eq. 9999 ) goto 1500
           kpos = 0
           do j = 1,kenv
              ioutlocal = 0
              if( kount .lt. nrc ) then
                  kount = kount + 1
                  kvel(j) = jklasif(kount)
                  ioutlocal = kvel(j)
              endif
              if( kount .eq. nrc ) ncalc = 9999

              if( ioutlocal .eq. 0 ) goto 800 

                  call contingenciaDC(ce,impr,myinst,NPROC1)  ! esta rotina faz chamadas do DC Incremental

                  icont0 = icont0 + 1 ! incrementa o contador de casos feitos pelo Pai
              
c == Contabilidade:
         
                  idp(kount) = tids(1)                       ! associa o caso analisado ao TID

                  ioutlocal = 0
                  kvel(j) = 0

  800             continue
           enddo

 1500   continue

c == Testa saida:

        if( nrecv .eq. NPROC1-1 ) goto 2000
        goto 7
      endif                ! if ( lnt )

c -------------------------
c Termino do Processamento
c -------------------------   

 2000 continue             



c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c ---------------------------------------------------------
c Pega Resultados do DC Incremental do Processo de tids(1)
c ---------------------------------------------------------
      nilhasoma = nilha

c ---------------------------------------------------------
c Recebe Resultados do DC Incremental dos Demais Processos
c ---------------------------------------------------------


c == Indices:

      do i = 1, NPROC1-1
        ip = i + 1                 ! exclui o de tids(1)
        call PVMFRECV( tids(ip), nsg1, bufid )
        call PVMFUNPACK( INTEGER4,isend1,nrc+1,1,info )
        call PVMFUNPACK( REAL8,psend,isend1(0),1,info )   
        
        iconti = isend1(0)
        if( iconti .ne. 0 ) then
           do j = 1, iconti
             kk = isend1(j)
             pia(kk) = psend(j)
           enddo
        endif
      enddo



      
c == Ilhamentos: 

ccc      write(*,*)' NO | ILHAMENTOS '
ccc      write(*,10)myinst,nilha
ccc   10 format(2x,i2,4x,i5,/)
     

      do i = 1, NPROC1-1
        ip = i + 1                       ! exclui o de tids(1)
        call PVMFRECV( tids(ip), nsg5, bufid )
        call PVMFUNPACK( INTEGER4, isend2(0),1,1,info )
        nsize2(ip) = isend2(0)+1
      enddo

      do i = 1, NPROC1-1
        ip = i + 1            ! exclui o de tids(1)
        nitens4 = nsize2(ip) 

        call PVMFRECV( tids(ip), nsg6, bufid )
        call PVMFUNPACK( INTEGER4, isend2,nitens4,1,info )
        nilha = isend2(0)

       
c Recuperando kilha()
        if ( nilha .ne. 0 ) then
          do j = 1, nilha
            kilha(j+nilhasoma) = isend2(j)
          enddo
          nilhasoma = nilhasoma + nilha
        endif
      enddo

      nilha = nilhasoma



c ------------
c Da Um Salto
c ------------
      if( nbase .gt. 1 ) goto 1000
 
  


 
  950 continue
c ------------------------------------------------------------------------      
c                       1.] CALCULA DC INCREMENTAL 
c ------------------------------------------------------------------------
c ------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para o DC Incremental
c ------------------------------------------------------------------------
      do i = 1, nrc

          ioutlocal = jklasif(i)

       	  call contingenciaDC(ce,impr,myinst,NPROC1) ! esta rotina faz chamadas do DC Incremental

c == Contabilidade:
         
          idp(i) = tids(1)                           ! associa o caso analisado ao TID

      enddo
c ------------------------------------------------------------------------



 1000 continue


      return
      end                       ! Fim da rotina Pai()

