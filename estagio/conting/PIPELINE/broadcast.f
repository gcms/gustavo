c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para transmitir dados iniciais e topologicos da rede para 
c todos os processos
c23456789
c
c ----------------------------------
c [ FAZ PARTE DO PROGRAMA GERENTE ]
c ----------------------------------
c
      subroutine broadcast(NPROC,NPROC1,NPROC2,NPROC3,indice1,indice2,
     &tids)
c
c
c  [ Funcao: Transmitir dados iniciais e topologicos da rede ]
c
c
      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'

c
c Declaracao relativa ao PVM
c
      integer*4 info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3,NPROC
      integer*4 indice1(20),indice2(20),tids(20)


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------



c === Transmite a base de dados para analise de contingencias:
      write(*,*)'                                       '
      write(*,*)' --> TRANSMISSAO DOS DADOS INICIAIS... '
      write(*,*)'                                       '



c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      msgtype1 = 1


c ---------------------------------------------------
c Faz o Broadcast dos Dados Para Todos os Processos 
c ---------------------------------------------------

      call PVMFINITSEND( PVMDEFAULT, info )

      call PVMFPACK( INTEGER4, kselmax, 1, 1, info )
      call PVMFPACK( INTEGER4, NPROC1, 1, 1, info )
      call PVMFPACK( INTEGER4, NPROC2, 1, 1, info )
      call PVMFPACK( INTEGER4, NPROC3, 1, 1, info )
      call PVMFPACK( INTEGER4, NPROC, 1, 1, info )
      call PVMFPACK( INTEGER4, tids, NPROC, 1, info )
      call PVMFPACK( INTEGER4, indice1, NPROC1, 1, info )
      call PVMFPACK( INTEGER4, indice2, NPROC2, 1, info )

      call PVMFPACK( INTEGER4, modelo, 1, 1, info )         ! de 'opcoes.dat'
      call PVMFPACK( INTEGER4, lim, 1, 1, info )
      call PVMFPACK( INTEGER4, limite, 1, 1, info )
      call PVMFPACK( INTEGER4, ncorte, 1, 1, info )
      call PVMFPACK( INTEGER4, kformat, 1, 1, info )

      call PVMFPACK( REAL8, tol, 1, 1, info )
      call PVMFPACK( REAL8, toler, 1, 1, info )

      call PVMFPACK( REAL8, fcont, 1, 1, info )  
      call PVMFPACK( REAL8, corte, 1, 1, info )

      call PVMFPACK( INTEGER4, nb, 1, 1, info )
      call PVMFPACK( INTEGER4, ncb, 1, 1, info )
      call PVMFPACK( INTEGER4, ng, 1, 1, info )

      call PVMFPACK( INTEGER4, itipo, nb, 1, info )
      call PVMFPACK( INTEGER4, nex, nb, 1, info )

      call PVMFPACK( REAL8, tet, nb, 1, info )
      call PVMFPACK( REAL8, ves, nb, 1, info )
      call PVMFPACK( REAL8, pes, nb, 1, info )
      call PVMFPACK( REAL8, qes, nb, 1, info )
      call PVMFPACK( REAL8, pca, nb, 1, info )
      call PVMFPACK( REAL8, qca, nb, 1, info )
      call PVMFPACK( REAL8, qmin, nb, 1, info )
      call PVMFPACK( REAL8, qmax, nb, 1, info )
      call PVMFPACK( REAL8, gdiag, nb, 1, info )
      call PVMFPACK( REAL8, bdiag, nb, 1, info )
      call PVMFPACK( REAL8, shu, nb, 1, info )

      call PVMFPACK( REAL8, carga, 1, 1, info )

      call PVMFPACK( INTEGER4, nl, 1, 1, info )
      call PVMFPACK( INTEGER4, ncl, 1, 1, info ) 

      call PVMFPACK( INTEGER4, ni, nl, 1, info )
      call PVMFPACK( INTEGER4, nf, nl, 1, info )

      call PVMFPACK( REAL8, xkl, nl, 1, info )
      call PVMFPACK( REAL8, gkl, nl, 1, info )
      call PVMFPACK( REAL8, bkl, nl, 1, info )
      call PVMFPACK( REAL8, b1kl, nl, 1, info )
      call PVMFPACK( REAL8, b2kl, nl, 1, info )
      call PVMFPACK( REAL8, fmi, nl, 1, info )
      call PVMFPACK( REAL8, fma, nl, 1, info )
      call PVMFPACK( REAL8, ylin, nl, 1, info )
      call PVMFPACK( REAL8, tap, nl, 1, info )
      
      if( info .lt. 0 ) write(*,*)' ERRO NO EMPACOTAMENTO' 

cc      call PVMFBCAST( 'contingencia', msgtype1, info )
      call PVMFMCAST( NPROC,tids,msgtype1,info )
      if( info .lt. 0 ) write(*,*)' ERRO NO MCAST'



      return
      end          ! Fim da subrotina broadcast()






