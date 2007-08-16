c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para transmitir dados e resultados aos processos PCX
c23456789
c
c ----------------------------------
c [ FAZ PARTE DO PROGRAMA GERENTE ]
c ----------------------------------
c
      subroutine comunica(ce,cle,NPROC,NPROC1,NPROC2,NPROC3,kselect,
     &tids)
c
c
c  [ Funcao: Transmitir dados e resultados do caso base ]
c
c
      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'
c
c Vetores de Zolenkopf com os elementos nao-nulos das matrizes B' e B''
c
      dimension ce(max),cle(max)
c
c Declaracao relativa ao PVM
c
      integer*4 info
      integer*4 NPROC,NPROC1,NPROC2,NPROC3,NPROC
      integer*4 tids(20)


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------



c === Transmite a base de dados para analise de contingencias:
      write(*,*)'                              '
      write(*,*)' --> TRANSMISSAO DOS DADOS... ',kselect
      write(*,*)'                              '



c ------------------------------
c Rotulos das mensagens [types]
c ------------------------------
      msg0 = 11



c ------------------------------------------------
c Faz o Broadcast dos Dados Para os Processos PCX
c ------------------------------------------------

      do 550 ip = 1, NPROC1
      call PVMFINITSEND( PVMDEFAULT, info )
   
      call PVMFPACK( INTEGER4, NPROC, 1, 1, info )
      call PVMFPACK( INTEGER4, tids, NPROC, 1, info )

      call PVMFPACK( INTEGER4, kselect, 1, 1, info )

      call PVMFPACK( INTEGER4, nb, 1, 1, info )
      call PVMFPACK( INTEGER4, lfmax, 1, 1, info )

      call PVMFPACK( REAL8, vsend, nb, 1, info )
      call PVMFPACK( REAL8, tsend, nb, 1, info )
      call PVMFPACK( REAL8, p, nb, 1, info )
      call PVMFPACK( REAL8, q, nb, 1, info )
      
      call PVMFPACK( REAL8, ce, lfmax, 1, info )
      call PVMFPACK( REAL8, cle, lfmax, 1, info )

      call PVMFPACK( INTEGER4, lcol, nb, 1, info )
      call PVMFPACK( INTEGER4, noze, nb, 1, info )
      call PVMFPACK( INTEGER4, nseq, nb, 1, info )
      call PVMFPACK( INTEGER4, lrd, nb, 1, info )
      call PVMFPACK( INTEGER4, itag, lfmax, 1, info )
      call PVMFPACK( INTEGER4, lnxt, lfmax, 1, info )
            
      call PVMFPACK( INTEGER4, nrc, 1, 1, info )
      call PVMFPACK( INTEGER4, nsobr, 1, 1, info )
      call PVMFPACK( INTEGER4, nso, 1, 1, info )

      call PVMFPACK( INTEGER4, klasif, nrc+1, 1, info )

      do i = 0, mmax
       call PVMFPACK( INTEGER4, kcont(1,i), nrc, 1, info )
      enddo

      call PVMFPACK( INTEGER4, next, nb, 1, info )

      call PVMFSEND( tids(ip), msg0, info )


 550  continue



      return
      end          ! Fim da subrotina comunica()






