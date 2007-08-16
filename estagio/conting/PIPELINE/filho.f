c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ------------------------------
c [ PROGRAMA PCX  == SERVIDOR ]
c ------------------------------

      subroutine Filho(ce,impr,myinst,NPROC1,NPROC2,
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
      integer*4 myinst,mytid,info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3
      integer*4 indice1(20),indice2(20),tids(20)


c -------------------------------------------------------
c Rotina EscravoS(): Modo Paralelo Sincrono
c
c Rotinas EscravoA() e Filho(): Modo Paralelo Assincrono
c -------------------------------------------------------

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


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c ----------------------------------------------------
c         PROCESSAMENTO PARALELO ASSINCRONO
c ----------------------------------------------------
c ----------------------------------------------------
c O LOOP DC eh Executado em Paralelo Assincrono:     O 
c Processador Assim que Termina seu Caso Pega um  Novo
c Envelope de Casos para Executar 
c ----------------------------------------------------
c O Controle eh Feito Somente pelo Processo de tids(1)   
c que Tambem Executa Calculos DC Enquanto Espera
c ----------------------------------------------------


c -----------------------------------
c Retorna o TID do processo corrente
c -----------------------------------

      call PVMFMYTID( mytid )



      iconti = 0       ! contador de indices de severidade
      kpos   = 0       ! posicao em kvel() onde vai o TID
      init   = 0       ! posicao em kvel() onde inicia os casos de contingencia
      kflag  = 0       ! "flag" para CONTINUAR ou FINALIZAR


c ----------------------------------------------------
c Recebe o primeiro envelope de casos para processar
c ----------------------------------------------------
      kp = 1                             ! indice do tids() do PCX pai


      call PVMFRECV( tids(kp), nsg2, bufid )
      call PVMFUNPACK( INTEGER4, kenv, 1, 1, info ) 
      call PVMFUNPACK( INTEGER4, kpos, 1, 1, info ) 
      call PVMFUNPACK( INTEGER4, kvel(kpos), kenv+1, 1, info ) 



1000  continue


      if( kvel(kpos) .eq. mytid ) init = kpos + 1

      do 1500 j = init,init + kenv - 1
         
         ioutlocal = kvel(j)

         if ( ioutlocal .eq. 0 ) goto 1500  ! saida

         call contingenciaDC(ce,impr,myinst,NPROC1) ! esta rotina faz chamadas do DC Incremental

         iconti = iconti + 1
	 isend1(iconti) = ioutlocal                 ! no. do caso
	 psend(iconti) = pia(ioutlocal)             ! indice de severidade
         ioutlocal = 0
         kvel(j) = 0
 1500 continue




c -------------------------------------------------------------------------------
c Envia sinal para o processo Pai informando que ja calculou o envelope de casos
c -------------------------------------------------------------------------------

      kbusy(1) = mytid                   ! tids() do processo corrente 
      kbusy(2) = kpos                    ! caracteriza o envelope ja calculado
 
      call PVMFINITSEND( PVMDEFAULT, info )
      call PVMFPACK( INTEGER4, kbusy, 2, 1, info ) 
      call PVMFSEND( tids(kp), nsg3, info )       



c ----------------------------------
c Host corrente verifica o "buffer" 
c ----------------------------------
       
   8  call PVMFNRECV( tids(kp), nsg4, lnt )
   
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

        goto 1000             ! processa novo caso
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
c Envia Resultados da Simulacao do DC Incremental
c ao Processo de tids(1)
c ------------------------------------------------

c == Indices: 

      isend1(0) = iconti


c --------------------------------------------------------------
c Envia Resultados do DC Incremental para o Processo de tids(1)
c --------------------------------------------------------------
 
      call PVMFINITSEND( PVMDEFAULT, info )
      call PVMFPACK( INTEGER4,isend1,nrc+1,1,info )
      call PVMFPACK( REAL8,psend,isend1(0),1,info )
      call PVMFSEND( tids(kp), nsg1, info )
   


c == Ilhamentos: 
          
ccc      write(*,*)' NO | ILHAMENTOS '
ccc      write(*,10)myinst,nilha
ccc   10 format(2x,i2,4x,i5)
      

      do j = 1, nilha
        isend2(j) = kilha(j) 
      enddo
      

      isend2(0) = nilha
      nitens4 = isend2(0)+1

c == Envia os tamanhos dos vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, isend2(0),1,1,info )
      call PVMFSEND( tids(1), nsg5, info )

c == Envia os vetores:

      call PVMFINITSEND( PVMRAW, info )
      call PVMFPACK( INTEGER4, isend2,nitens4,1,info )
      call PVMFSEND( tids(1), nsg6, info )


      return
      end                       ! Fim da rotina Filho()

