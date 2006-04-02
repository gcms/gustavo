c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCX  == SERVIDOR ]
c ------------------------------

      subroutine MestreA(ce,impr,myinst,NPROC1,NPROC2,
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


c ---------------------------------------------------
c Rotina MestreS(): Modo Paralelo Sincrono
c
c Rotina MestreA() e Pai(): Modo Paralelo Assincrono
c ---------------------------------------------------


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


c ------------------------------------------------------
c          PROCESSAMENTO PARALELO ASSINCRONO
c ------------------------------------------------------
c ------------------------------------------------------
c O LOOP DC e Executado em Paralelo Assincrono: O  Pro-
c cessador Assim que Termina seu Caso Pega um Novo Caso
c para Executar 
c ------------------------------------------------------
c O Controle eh Feito Somente pelo Processo de  tids(1)   
c que Tambem Executa Calculos DC Enquanto Espera
c ------------------------------------------------------



c -------------------------------------
c Preparando a Carga dos Processadores
c -------------------------------------

      nbase = NPROC1

      if( nrc .lt. nbase ) then
          nndim = nrc
          write(*,*)' Toda a lista e menor do que o no. de hosts ! '
          write(*,*)' PCX) REDUZA O TAMANHO DA VM PARA ',nndim,' ! '
          return
      endif

      


      do i = 1, nrc
        jklasif(i) = i
      enddo
      jklasif(nrc+1) = 0  ! cartao flag



c ------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para o DC Incremental
c ------------------------------------------------------------------------
      if( nbase .eq. 1 ) goto 950


      
       
c ------------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Dois ou Mais Processadores Para o DC Incremental
c ------------------------------------------------------------------------------

      kount = 0           ! inicializa o contador geral de casos
      icont0 = 0          ! inicializa o contador de casos processados em tids(1)
      icontr = 0          ! inicializa o contador de casos recebidos de outros hosts <> PCX Pai
 

c -------------------------
c Manda os primeiros casos 
c -------------------------

      do i = 1, NPROC1-1
        kp = i + 1       ! exclui o de tids(1)
        if ( kount .lt. nrc ) then
          kount = kount + 1
          kbusy(1) = jklasif(kount) 
          kbusy(2) = kount
          call PVMFINITSEND( PVMDEFAULT, info )
          call PVMFPACK( INTEGER4, kbusy, 2, 1, info )  
          call PVMFSEND( tids(kp), nsg2, info )   ! hostdest = i (i = 2, 3, ..., NPROC1)
        endif
      enddo
        
     
c ----------------------------------------------
c Processo de tids(1) pega 1 caso para analisar
c ----------------------------------------------

      if ( kount .lt. nrc ) then
        kount = kount + 1
        ioutlocal = jklasif(kount)                  ! caso a ser analisado

       	call contingenciaDC(ce,impr,myinst,NPROC1)  ! esta rotina faz chamadas do DC Incremental

        icont0 = icont0 + 1

c == Contabilidade:
         
        idp(kount) = tids(1)                        ! associa o caso analisado ao TID 
      endif

      

c ----------------------------------------
c Processo de tids(1) verifica o "buffer" 
c ----------------------------------------
      
    7 call PVMFNRECV( -1, nsg3, lnt )

      if ( lnt .gt. 0 ) then
        call PVMFUNPACK( INTEGER4, kbusy, 2, 1, info )

        ip = kbusy(1)                              ! tid do hostdest = ip
        nrcaso = kbusy(2)
        icontr = icontr + 1

c == Contabilidade:
         
        idp(nrcaso) = ip  ! associa o caso analisado ao TID 

c -----------------------------------
c Envia novo caso para "ip" analisar
c -----------------------------------

        kount = kount + 1              
        kbusy(1) = jklasif(kount) 
        kbusy(2) = kount

        call PVMFINITSEND( PVMDEFAULT, info )
        call PVMFPACK( INTEGER4, kbusy, 2, 1, info )  
        call PVMFSEND( ip, nsg4, info )    ! envia um caso para o escravo que terminou  

        if ( icont0 + icontr .eq. nrc ) goto 2000
        goto 7
      else

c ----------------------------------------------
c Processo de tids(1) pega 1 caso para analisar
c ----------------------------------------------

        if( kount .lt. nrc ) then
          kount = kount + 1
          ioutlocal = jklasif(kount)                 ! caso a ser analisado

       	  call contingenciaDC(ce,impr,myinst,NPROC1) ! esta rotina faz chamadas do DC Incremental

          icont0 = icont0 + 1

c == Contabilidade:
         
          idp(kount) = tids(1)                       ! associa o caso analisado ao TID 

          if ( icont0 + icontr .eq. nrc ) goto 2000
          goto 7
        endif
        goto 7
      endif                ! if ( lnt )

     
c -------------------------
c Termino do Processamento
c -------------------------  

 2000 continue             ! ioutlocal = 0 ==> sinal para parar




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
        kp = i + 1                 ! exclui o de tids(1)
        call PVMFRECV( tids(kp), nsg1, bufid )
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
        kp = i + 1                       ! exclui o de tids(1)
        call PVMFRECV( tids(kp), nsg5, bufid )
        call PVMFUNPACK( INTEGER4, isend2(0),1,1,info )
        nsize2(kp) = isend2(0)+1
      enddo

      do i = 1, NPROC1-1
        kp = i + 1                       ! exclui o de tids(1)
        nitens4 = nsize2(kp) 

        call PVMFRECV( tids(kp), nsg6, bufid )
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
         
        idp(i) = tids(1)                             ! associa o caso analisado ao TID

      enddo
c ------------------------------------------------------------------------



 1000 continue


      return
      end                       ! Fim da rotina MestreA()








c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCX  == SERVIDOR ]
c ------------------------------

      subroutine MestreS(ce,impr,myinst,NPROC1,NPROC2,
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
      integer*4 myinst,mytid,mynode,info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


      dimension nsize2(63)                                  ! local


c ---------------------------------------------------
c Rotina MestreS(): Modo Paralelo Sincrono
c
c Rotina MestreA() e Pai(): Modo Paralelo Assincrono
c ---------------------------------------------------


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
c        PROCESSAMENTO PARALELO SINCRONO
c ------------------------------------------------
c ------------------------------------------------
c O LOOP DC e Executado em Paralelo Sincrono: Os 
c Casos Sao Previamente Divididos Entre Hosts na
c Forma Balanceada em Numero de Casos
c ------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )

c -------------------------------------------------------------
c Define o "instance number" deste processo no contexto de PCX
c -------------------------------------------------------------

c -------------------------------------------------------------
c OBS: Isto eh feito aqui exclusivamente com o fim de permitir
c      o processamento paralelo SINCRONO (sem erros).
c -------------------------------------------------------------

      do i = 1, NPROC1
         if( mytid .eq. tids(i) ) mynode = i-1
      enddo

      nbase = NPROC1
c234567
      if( nbase .eq. 1 ) then

c ------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Um Unico Processador Para o DC Incremental
c ------------------------------------------------------------------------
      
c --------------------------------------------
c Apenas o Processador "0" Executa [ serial ]
c --------------------------------------------

	do ll = 1, nrc

          ioutlocal = ll                              ! caso a ser analisado

          call contingenciaDC(ce,impr,myinst,NPROC1)  ! esta rotina faz chamadas do DC Incremental

c == Contabilidade:
         
          idp(ioutlocal) = tids(1)                    ! associa o caso analisado ao TID

	enddo

      else          ! if ( nbase )      


c --------------------------------------------------------------------------
c Situacao Em Que Se Dispoe de Mais de Um Processador Para o DC Incremental
c --------------------------------------------------------------------------

       
c ---------------------------------------------
c Todos os Processadores Executam [ paralelo ]
c ---------------------------------------------

        icont0 = 0                                    ! inicializa contador de casos calculados pelo processador "0"

        do kk = mynode + 1, nrc, nbase

	  ioutlocal = kk                              ! caso a ser analisado

          call contingenciaDC(ce,impr,myinst,NPROC1)  ! esta rotina faz chamadas do DC Incremental

          icont0 = icont0 + 1

c == Contabilidade:
         
          idp(ioutlocal) = tids(1)                    ! associa o caso analisado ao TID
       
        enddo




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
        kp = i + 1                 ! exclui o de tids(1)
        ktid = tids(kp)            ! TID do processo filho do qual se esta recebendo
        call PVMFRECV( tids(kp), nsg1, bufid )
        call PVMFUNPACK( INTEGER4,isend1,nrc+1,1,info )
        call PVMFUNPACK( REAL8,psend,isend1(0),1,info )   
        
        iconti = isend1(0)
        if( iconti .ne. 0 ) then
           do j = 1, iconti
             kk = isend1(j)
             pia(kk) = psend(j)

c == Contabilidade:
         
             idp(kk) = ktid       ! associa o caso analisado ao TID

           enddo
        endif
      enddo



      
c == Ilhamentos: 

ccc      write(*,*)' NO | ILHAMENTOS '
ccc      write(*,10)myinst,nilha
ccc   10 format(2x,i2,4x,i5,/)
     

      do i = 1, NPROC1-1
        kp = i + 1                       ! exclui o de tids(1)
        call PVMFRECV( tids(kp), nsg5, bufid )
        call PVMFUNPACK( INTEGER4, isend2(0),1,1,info )
        nsize2(kp) = isend2(0)+1
      enddo

      do i = 1, NPROC1-1
        kp = i + 1                       ! exclui o de tids(1)
        nitens4 = nsize2(kp) 

        call PVMFRECV( tids(kp), nsg6, bufid )
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


      endif          ! if ( nbase )      

      return
      end                       ! Fim da rotina MestreS()
