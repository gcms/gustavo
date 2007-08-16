c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCY  == SERVIDOR ]
c ------------------------------


      subroutine Son(ce,cle,impr,myinst,
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


c ---------------------------------------------------
c Rotina SlaveS(): Modo Paralelo Sincrono
c
c Rotinas SlaveA() e Son(): Modo Paralelo Assincrono
c ---------------------------------------------------

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




c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c ------------------------------------------------------------
c           PROCESSAMENTO PARALELO ASSINCRONO
c ------------------------------------------------------------
c ------------------------------------------------------------
c O LOOP AC eh Executado em Paralelo Assincrono: O  Processa-
c dor Assim que Termina seu  Envelope de Casos Pega  um Outro 
c para Executar 
c ------------------------------------------------------------
c O Controle eh Feito Somente pelo Processo de tids(NPROC1+1)   
c que Tambem Executa Calculos AC Enquanto Espera
c ------------------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )


      kpos  = 0            ! posicao em kvel() onde vai o TID
      init  = 0            ! posicao em kvel() onde inicia os casos de contingencia
      kflag = 0            ! "flag" para CONTINUAR ou FINALIZAR



c ----------------------------------------------------
c Recebe o primeiro envelope de casos para processar
c ----------------------------------------------------

      ktid = tids(NPROC1+1)    ! TID do processo destino
      kp = NPROC1+1            ! indice do tids() do pcy pai

      call PVMFRECV( tids(kp), ksg2, bufid )
      call PVMFUNPACK( INTEGER4, kenv, 1, 1, info ) 
      call PVMFUNPACK( INTEGER4, kpos, 1, 1, info ) 
      call PVMFUNPACK( INTEGER4, kvel(kpos), kenv+1, 1, info ) 



1000  continue
       
      if( kvel(kpos) .eq. mytid ) init = kpos + 1


      do 1500 j = init,init + kenv - 1
         
         ioutlocal = kvel(j)

         if ( ioutlocal .eq. 0 ) goto 1500  ! saida

         call contingenciaAC(ce,cle,impr,myinst,NPROC2,tids)  ! esta rotina faz chamadas da contingencia AC

         ioutlocal = 0
         kvel(j) = 0
 1500 continue



c -----------------------------------------------------------------------------------
c Envia sinal para o processo mestre informando que ja calculou o envelope de casos
c -----------------------------------------------------------------------------------

      kbusy(1) = mytid                   ! tids() do processo corrente 
      kbusy(2) = kpos                    ! caracteriza o envelope ja calculado
 
      call PVMFINITSEND( PVMDEFAULT, info )
      call PVMFPACK( INTEGER4, kbusy, 2, 1, info )
      call PVMFSEND( tids(kp), ksg3, info )       



c --------------------------------------
c Processo corrente verifica o "buffer" 
c --------------------------------------
       
   8  call PVMFNRECV( tids(kp), ksg4, lnt )
   
      if ( lnt .gt. 0 ) then
c ----------------------------------------------------
c Escravo recebe novo envelope de casos para analisar
c ----------------------------------------------------

        call PVMFUNPACK( INTEGER4, kflag, 1, 1, info )
        call PVMFUNPACK( INTEGER4, kenv, 1, 1, info ) 
        call PVMFUNPACK( INTEGER4, kpos, 1, 1, info ) 
        call PVMFUNPACK( INTEGER4, kvel(kpos), kenv+1, 1, info )  
        
c == Testa saida:

        if( kflag .eq. 9999 ) goto 2000    ! saida

        goto 1000             ! processa novo envelope de casos
      else 
        goto 8
      endif
      

c -------------------------
c Termino do Processamento
c -------------------------   

 2000 continue

   
 
c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------    
c ------------------------------------------------
c Envia Resultados da Simulacao de Contingencias 
c ao Processo de tids(NPROC1+1)
c ------------------------------------------------

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
      end                       ! Fim da rotina Son()

