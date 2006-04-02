c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c
      subroutine clock(nhost,NPROC)
c
c  [ Funcao: Gerar saida de tempos "wall clock time" ]
c
c
      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'

      integer*4 nhost,NPROC
      double precision t_soma


      open( unit = 090,file = 'tempos', access='append' )
      t_soma = time(0) + time(1) + time(2) + time(3)
 
      if( modelo .eq. 0 ) write(090,12)
 12   format(///,22x,'** PROGRAMA PIPELINE SINCRONO **')
      if( modelo .eq. 1 ) write(090,13)
 13   format(///,18x,'** PROGRAMA PIPELINE ASSINCRONO 1 **')
      if( modelo .eq. 2 ) write(090,14)
 14   format(///,18x,'** PROGRAMA PIPELINE ASSINCRONO 2 **')

      write(090,15)

      write(090,16)time(0)
      write(090,17)time(1)
      write(090,18)time(2)
      write(090,19)time(3)
      write(090,26)t_soma
 16   format(10x,' Tempo para Inicializacao:                  ',f9.2)
 17   format(10x,' Tempo para Ler&Enviar Dados Iniciais/Rede: ',f9.2)
 18   format(10x,' Tempo para Processar Computacao Intensiva: ',f9.2)
 19   format(10x,' Tempo para Finalizar Processamento:        ',f9.2)
 21   format(55x,' ============ ')
 26   format(10x,' Tempo Total Gasto:                         ',f9.2)

      write(090,15)

      write(090,22)nhost 
 22   format(10x,' Numero de Workstations Usadas: ',i2)
      write(090,24)NPROC+1
 24   format(10x,' Numero Total de Processos: ',i2)
      write(090,23)
 23   format(10x,' FIM DO PROCESSAMENTO DA ANALISE DE CONTINGENCIA !',/)
      write(090,15)
 15   format(' ---------------------------------------------------------
     &--------------') 

      close(090)


      return
      end          ! Fim da subrotina clock()
