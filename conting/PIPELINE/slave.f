c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------

      subroutine SlaveA(ce,cle,impr,myinst,
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
      integer*4 myinst,mytid,info,bufid
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


c --------------------------------------------------
c Rotina SlaveS(): Modo Paralelo Sincrono
c
c Rotina SlaveA() e Son(): Modo Paralelo Assincrono
c --------------------------------------------------

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



c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c -----------------------------------------------------------
c            PROCESSAMENTO PARALELO ASSINCRONO
c -----------------------------------------------------------
c -----------------------------------------------------------
c O LOOP AC eh Executado em Paralelo Assincrono: O Processa-
c dor Assim que Termina seu Caso Pega  um  Novo  Caso  para
c Executar 
c -----------------------------------------------------------
c O Controle eh Feito Somente pelo Processo de tids(NPROC1+1)   
c que Tambem Executa Calculos AC Enquanto Espera
c -----------------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )


c --------------------------
c Recebe os primeiros casos 
c --------------------------

      kp = NPROC1+1              ! indice do tids() do pcy pai

      call PVMFRECV( tids(kp), ksg2, bufid )
      call PVMFUNPACK( INTEGER4, kbusy, 2, 1, info )
 
      ioutlocal = kbusy(1)               ! caso a ser analisado
      nrcaso = kbusy(2)                  ! no. do caso recebido

1000  continue
       
      call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

      kbusy(1) = mytid                   ! tids() do processo corrente 
      kbusy(2) = nrcaso                  ! no. do caso ja calculado 

      call PVMFINITSEND( PVMDEFAULT, info )
      call PVMFPACK( INTEGER4, kbusy, 2, 1, info )  
      call PVMFSEND( tids(kp), ksg3, info )       


c --------------------------------------
c Processo corrente verifica o "buffer" 
c --------------------------------------
       
   8  call PVMFNRECV( tids(kp), ksg4, lnt )
   
      if ( lnt .gt. 0 ) then
c ---------------------------------------
c Escravo recebe novo caso para analisar
c ---------------------------------------

        call PVMFUNPACK( INTEGER4, kbusy, 2, 1, info )

        ioutlocal = kbusy(1)               ! caso a ser analisado
        nrcaso = kbusy(2)                  ! no. do caso recebido
     
        if ( ioutlocal .eq. 0 ) goto 2000  ! saida  
 
        goto 1000             ! processa novo caso
      else 
        goto 8
      endif

c -------------------------
c Termino do Processamento
c ------------------------- 
  
 2000 continue                ! ioutlocal = 0 --> sinal para parar
   
   
            
c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------    
c ---------------------------------------------------------------
c Envia Resultados da Simulacao AC ao Processo de tids(NPROC1+1)
c ---------------------------------------------------------------

c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes: 
          
cc      write(*,*)' NO | DIVERG. | CONVERG. | NAO-CONVERG.'
cc      write(*,10)myinst,ndiv,nyconv,nnconv
cc   10 format(2x,i2,5x,i5,7x,i5,7x,i5)
      

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
      nitens4 = isend2(0)+1
      nitens5 = isend3(0)+1
      nitens6 = isend4(0)+1

c == Envia os tamanhos dos vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, isend2(0),1,1,info )
      call PVMFPACK( INTEGER4, isend3(0),1,1,info )
      call PVMFPACK( INTEGER4, isend4(0),1,1,info )
      call PVMFSEND( tids(kp), ksg5, info )

c == Envia os vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, isend2,nitens4,1,info )
      call PVMFPACK( INTEGER4, isend3,nitens5,1,info )
      call PVMFPACK( INTEGER4, isend4,nitens6,1,info )
      call PVMFPACK( INTEGER4, isend5,nitens6,1,info )
      call PVMFPACK( INTEGER4, isend6,nitens6,1,info )
      call PVMFSEND( tids(kp), ksg6, info )


      return
      end                       ! Fim da rotina SlaveA()










c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------

      subroutine SlaveS(ce,cle,impr,myinst,
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


c --------------------------------------------------
c Rotina SlaveS(): Modo Paralelo Sincrono
c
c Rotina SlaveA() e Son(): Modo Paralelo Assincrono
c --------------------------------------------------

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



c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


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
      ktid = tids(NPROC1+1)        ! TID do processo destino
      iconti = 0    ! inicializa contador de casos calculados pelo processador corrente



c234567
      do kk = mynode + 1, ncrit, nbase

	ioutlocal = klasif(kk)     ! caso a ser analisado

        call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids) ! esta rotina faz chamadas da contingencia AC

        iconti = iconti + 1
        
c == Contabilidade:
         
        idt(ioutlocal) = mytid     ! associa o caso analisado ao TID
	isend1(iconti) = ioutlocal        ! no. do caso
        ksend(iconti) = idt(ioutlocal)    ! TID do processo vs caso analisado

      enddo


 
c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------    
c ------------------------------------------------
c Envia Resultados da Simulacao de Contingencias 
c ao Processo de tids(NPROC1+1)
c ------------------------------------------------


c == Quantidade de contingencias no processo corrente: 

      isend1(0) = iconti


c == Divergencias/Convergencias/Nao-Convergencias/Iteracoes: 
          
cc      write(*,*)' NO | DIVERG. | CONVERG. | NAO-CONVERG.'
cc      write(*,10)myinst,ndiv,nyconv,nnconv
cc   10 format(2x,i2,5x,i5,7x,i5,7x,i5)
      

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
      nitens3 = isend1(0)+1
      nitens4 = isend2(0)+1
      nitens5 = isend3(0)+1
      nitens6 = isend4(0)+1

c == Envia os tamanhos dos vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, isend1(0),1,1,info )
      call PVMFPACK( INTEGER4, isend2(0),1,1,info )
      call PVMFPACK( INTEGER4, isend3(0),1,1,info )
      call PVMFPACK( INTEGER4, isend4(0),1,1,info )
      call PVMFSEND( ktid, ksg5, info )

c == Envia os vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, ksend,nitens3,1,info )
      call PVMFPACK( INTEGER4, isend1,nitens3,1,info )
      call PVMFPACK( INTEGER4, isend2,nitens4,1,info )
      call PVMFPACK( INTEGER4, isend3,nitens5,1,info )
      call PVMFPACK( INTEGER4, isend4,nitens6,1,info )
      call PVMFPACK( INTEGER4, isend5,nitens6,1,info )
      call PVMFPACK( INTEGER4, isend6,nitens6,1,info )
      call PVMFSEND( ktid, ksg6, info )


      return
      end                       ! Fim da rotina SlaveS()

