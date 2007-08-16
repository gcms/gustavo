c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para LER EM DISCO DADOS DA REDE ELETRICA
c23456789
c
c ------------------------------------
c   [ FAZ PARTE DO PROGRAMA GERENTE ]
c ------------------------------------
c
      subroutine input(kselect)
c
c
c  [     Le Dados da Rede no Formato PECO ou Formato CDF     ]
c
c  [ Funcao: Abre/fecha arquivos e chama a subrotina leitura ]
c
c
      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'


c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------


c === Seleciona a base de dados para analise de contingencias:
      write(*,*)'                 '
      write(*,*)' -------------------------------------'
      write(*,*)' --> LE DADOS... ',kselect
      write(*,*)' -------------------------------------'
      write(*,*)'                 '


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c === Entrada de dados:

      
c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad1.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad1.dat'

          goto 100

c ----------------------------------------------------------------------  
   10 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad2.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad2.dat'

          goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad3.dat')
      
          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad3.dat'

          goto 100


c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad4.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad4.dat'

          goto 100


c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c === Entrada de dados:


c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad5.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad5.dat'

          goto 100



c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c === Entrada de dados:


c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad6.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad6.dat'

          goto 100


c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad7.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad7.dat'

          goto 100


c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad8.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad8.dat'

          goto 100


c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c === Entrada de dados:


c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad9.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad9.dat'

          goto 100


c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c === Entrada de dados:
      

c Leitura de dados de barra e de linha
       
          open(unit=066,file='dad10.dat')

          call leitura                      ! le dados da rede e lista

          close(066)                        ! fecha arquivo 'dad10.dat'

          goto 100




c ----------------------------------------------------------------------

  100 continue          ! ponto comum a todas as bases de dados           

      return
      end          ! Fim da subrotina input()








c ----------------------------------------------------------------------
c Subrotina para LER EM DISCO DADOS DA REDE ELETRICA
c23456789
c
c ------------------------------------
c   [ FAZ PARTE DO PROGRAMA GERENTE ]
c ------------------------------------
c
      subroutine leitura
c
c
c  [  Le Dados da Rede no Formato PECO ou Formato CDF   ]
c
c
      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'


      if( kformat .eq. 0 ) call barra   ! formato PECO (dados de barras)
                     
      if( kformat .eq. 0 ) call rede    ! formato PECO (dados de ramos)


      if( kformat .eq. 1 ) call bus     ! formato CDF (dados de barras) 
  
      if( kformat .eq. 1 ) call line    ! formato CDF (dados de ramos)   


 

c Le a lista de ramos contingenciados < no arquivo 'dad1.dat' >
      
      call lista                        ! veja o arquivo entrada.f

      return
      end          ! Fim da subrotina leitura

