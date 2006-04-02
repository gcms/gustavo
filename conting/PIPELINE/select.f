c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotinas associadas a selecao de contingencias < origem: pl5.f >
c ----------------------------------------------------------------------
      subroutine rank2(linre)
c
      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'
c
      dimension linre(nlmax) 
      dimension lista(nlmax)
      dimension aa(nlmax)

	
      modo=1                 ! ordem decrescente
      do 38 i=1,nsobr
        lista(i)=i
        piaux(i)=pia(i)
        aa(i)=pia(i)
   38 continue
      call ordena( nsobr, nsobr, aa, lista, modo )
      
      do 37 i = 1,nsobr
	li = lista(i)
	lir = linre(li)
	klasif(i) = lir 
   	            
	aax = piaux(li)
	pia(i) = aax

   37 continue
	
      return
      end


c ----------------------------------------------------------------------
c	[ reordena ranking  ]

      subroutine reordena
c
      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'		
c        		
      dimension linre(nlmax)
c
      do i = 1,nsobr
	ii = klasif(i)
 	linre(i) = ii
      enddo

      call rank2(linre)

      
c  A.] Corte da lista pelo indice de severidade
      do i = 1,nsobr
        if( pia(i) .ge. corte ) ncrit = ncrit + 1  ! corte da lista 
      enddo

c  B.] Corte da lista pelo tamanho  

      if( ncrit .ge. ncorte ) ncrit = ncorte       ! corte pelo tamanho da lista
cc      write(*,*)' Lista Original: nrc   = ',nsobr     
cc      write(*,*)' Lista Reduzida: ncrit = ',ncrit     
 
      return
      end                ! Fim da subrotina reordena

c ----------------------------------------------------------------------

      subroutine ordena( n, k, aa, lista, modo )
c
c      Algoritmo Heapsort Knuth Vol. 3

c      Rotina que obtem os indices correspondentes a 
c      ordenacao parcial do vetor lista.
c
c      n      - no. de elementos que se deseja ordenar.
c      k      - no. de elementos dos vetores a e lista
c      aa     - vetor real a ordenar (nao e modificado)
c      lista  - vetor inteiro que retorna os indices 
c               ordenados do  vetor real aa.
c      modo   - chave que determina ordem:
c             = 0 (crescente)
c             = 1 (decrescente)
c
c      (obs: inicializar o vetor lista com 1 ate' n.)
c

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'

      dimension lista(nlmax)
      dimension aa(nlmax)
  

      if( n .ne. 0 )then
      l = k/2 + 1
      m = k
      nkl = k - n              ! nkl chamava-se nl --> foi substituido


20    if( l .ne. 1 )then 

      l = l - 1
      ir = lista(l)
      xk = aa(ir)
      go to 40
      endif

30       ir = lista(m)
         xk = aa(ir)
         lista(m) = lista(1)
         m = m - 1
         if( m .eq. nkl .or. m .eq. 1 ) then
            lista(1) = ir
            go to 100
         end if
40    j = l
50    i = j
      j = 2*j
      if( j - m ) 60, 70, 80
60       if( modo .eq. 0 ) then
            if(aa(lista(j)).gt.aa(lista(j+1))) j = j + 1
         else
            if(aa(lista(j)).lt.aa(lista(j+1))) j = j + 1
         end if
70          if( modo .eq. 0 ) then
               if(xk.le.aa(lista(j)))go to 80
            else
               if(xk.ge.aa(lista(j)))go to 80
            end if
            lista(i)=lista(j)
            go to 50
80             lista(i)=ir
               go to 20
c
100   continue
      do 90 i = k, (int(k/2) + 1), -1
         ir = lista(k-i+1)
         lista(k-i+1) = lista(i)
         lista(i) = ir
90    continue

	endif

1000  continue
      return
      end

