c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para calcular fluxo de carga pelo metodo FDLF - Versao BX
c23456789
c
c ----------------------------------
c [ FAZ PARTE DO PROGRAMA GERENTE ]
c ----------------------------------
c
      subroutine fcarga(ce,cle)
c
c ----------------------------------------------
c      Fluxo de Carga Desacoplado Rapido
c          (Implementacao Simetrica)
c ----------------------------------------------
c
c  [ Funcao: Calcular o estado da rede para o caso base ]
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


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------




c === Processamento do FLUXO DE CARGA DO CASO BASE:

c Etapas preparativas para explorar a esparsidade das sub-matrizes Jacobianas

      call esparsa(ce,cle)        ! esparsidade sem as matrizes



c Processo iterativo do fluxo de carga

      call estado(ce,cle)         ! solucao do subsistema 1


      do iii = 1,nb   
        vsend(iii) = v(iii)  
        tsend(iii) = t(iii)
      enddo

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

c Decide se faz analise de contingencias  


      if( nconv .eq. 1 ) then     ! veja nconv na subrotina estado
        write(*,*)' --> O CASO BASE NAO CONVERGE !!!'
        call PVMFHALT( info )
        if ( info .gt. 0 ) write(*,*)' --> SHUTS DOWN PVM... '
        stop
      endif
      
 
c Obtem a tabela geral de caminhos de fatoracao [ util para contingencias ]

      call grafo          ! gera a tabela next() c/ nb elementos


      return
      end          ! Fim da subrotina fcarga()






