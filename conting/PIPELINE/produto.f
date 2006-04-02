c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por vetor  <xmatriz.vetor = prod>
c                        [ Para Matriz Quadrada ]

      subroutine multmv1(xmatriz,vetor,nordem,prod) 
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension xmatriz(mmax,mmax)
      dimension vetor(mmax),prod(mmax)
      real*8 soma
c
      do i = 1,nordem   ! no. linhas da matriz
        soma = 0.0
      do l = 1,nordem   ! no. colunas da matriz
        soma = soma + xmatriz(i,l)*vetor(l)
      enddo
        prod(i) = soma
      enddo
c
      return
      end            ! Fim de multmv1 --> matriz quadrada (mxm)

c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por vetor  <xmatriz.vetor = prod>
c                     [ Para Matriz Retangular ]

      subroutine multmv2(xmatriz,vetor,nlinx,ncolx,prod) 
c
      include 'globvar.inc'
      include 'varcont.inc'

      dimension xmatriz(nbmax,mmax)
      dimension vetor(mmax),prod(nbmax)
      real*8 soma
c
      do i = 1,nlinx   ! no. linhas da matriz
        soma = 0.0
      do l = 1,ncolx   ! no. colunas da matriz
        soma = soma + xmatriz(i,l)*vetor(l)
      enddo
        prod(i) = soma
      enddo
c
      return
      end            ! Fim de multmv2 --> matriz (nbxm)

c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por vetor  <xmatriz.vetor = prod>
c   [ Para Matriz Retangular Armazenada Compacta no Vetor elem() ]
c   [        Mudanca no Dimensionamento Feita em 11/10/95        ]

      subroutine multmv3(elem,iplin,kcoluna,vetor,nlinx,prod) 
c
      include 'globvar.inc'
      include 'varcont.inc' 
c
      dimension elem(2*mmax)                      ! vetor que contem a matriz Mt esparsa
      integer*4 iplin(2*mmax+1),kcoluna(2*mmax+1)  ! vetor c/ apontadores e indicador da coluna
c
      dimension vetor(nbmax),prod(mmax)
      real*8 soma
c


      do i = 1,nlinx   ! no. linhas da matriz
        soma = 0.0
        inicio = iplin(i)
        ifim   = iplin(i+1) - 1
      do ip = 1,inicio,ifim           ! percorrendo elementos da linha i da matriz
        m = kcoluna(ip)
        soma = soma + elem(ip)*vetor(m)
      enddo
        prod(i) = soma
      enddo
c
      return
      end            ! Fim de multmv3 --> matriz (mxnb) no vetor elem()


c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por vetor  <xmatriz.vetor = prod>
c                     [ Para Matriz Retangular ]

      subroutine multmv4(xmatriz,vetor,nlinx,ncolx,prod) 
c
      include 'globvar.inc'
      include 'varcont.inc'

      dimension xmatriz(mmax,nbmax)
      dimension vetor(nbmax),prod(mmax)
      real*8 soma
c
      do i = 1,nlinx   ! no. linhas da matriz
        soma = 0.0
      do l = 1,ncolx   ! no. colunas da matriz
        soma = soma + xmatriz(i,l)*vetor(l)
      enddo
        prod(i) = soma
      enddo
c
      return
      end            ! Fim de multmv4 --> matriz (mxnb)

c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por matriz < xmatriz*ymatriz=zmatriz >
c                     [  Para matrizes Quadradas ]

      subroutine multmm1(xmatriz,ymatriz,nordem,zmatriz) 
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension xmatriz(mmax,mmax),ymatriz(mmax,mmax)
      dimension zmatriz(mmax,mmax)
      real*8 soma
c 
      do j = 1,nordem       ! colunas da 2a. matriz
      do i = 1,nordem       ! linhas da 1a. matriz
        soma = 0.0
      do k = 1,nordem       ! colunas da 1a. matriz
        soma = soma + xmatriz(i,k)*ymatriz(k,j)
      enddo
        zmatriz(i,j) = soma
      enddo
      enddo
c
      return
      end                ! Fim de multmm1 --> matriz quadrada (mxm)

c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por matriz < xmatriz*ymatriz=zmatriz >
c              [ Para Produto de Matrizes Tipo (nbxm).(mxm) ]
      subroutine multmm2(xmatriz,ymatriz,nlinx,ncolx,zmatriz) 
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension xmatriz(nbmax,mmax),ymatriz(mmax,mmax)
      dimension zmatriz(nbmax,mmax)
      real*8 soma
c 
      do j = 1,ncolx       ! colunas da 2a. matriz
      do i = 1,nlinx       ! linhas da 1a. matriz
        soma = 0.0
      do k = 1,ncolx       ! colunas da 1a. matriz
        soma = soma + xmatriz(i,k)*ymatriz(k,j)
      enddo
        zmatriz(i,j) = soma
      enddo
      enddo
c
      return
      end         ! Fim de multmm2 --> prod. matrizes tipo (nbxm).(mxm)
     
c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por matriz < xmatriz*ymatriz=zmatriz >
c              [ Para Produto de Matrizes Tipo (mxnb).(nbxm) ]
      subroutine multmm3(xmatriz,ymatriz,nlinx,ncolx,zmatriz) 
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension xmatriz(mmax,nbmax),ymatriz(nbmax,mmax)
      dimension zmatriz(mmax,mmax)
      real*8 soma
c 
      do j = 1,nlinx       ! colunas da 2a. matriz
      do i = 1,nlinx       ! linhas da 1a. matriz
        soma = 0.0
      do k = 1,ncolx       ! colunas da 1a. matriz
        soma = soma + xmatriz(i,k)*ymatriz(k,j)
      enddo
        zmatriz(i,j) = soma
      enddo
      enddo
c
      return
      end         ! Fim de multmm3 --> prod. matrizes tipo (mxnb).(nbxm)

c
c ----------------------------------------------------------------------
c Subrotina para multiplicar matriz por matriz < xmatriz*ymatriz=zmatriz >
c              [ Para Produto de Matrizes Tipo (mxnb).(nbxm) ]
c        [ Primeira Matriz Armazenada Compactamente no Vetor elem()] 
      subroutine multmm4(elem,iplin,kcoluna,ymatriz,nlinx,zmatriz) 
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension elem(2*mmax)                      ! vetor que contem a matriz Mt esparsa
      integer*4 iplin(2*mmax+1),kcoluna(2*mmax+1)  ! vetor c/ apontadores e indicador da coluna
c
      dimension ymatriz(nbmax,mmax)
      dimension zmatriz(mmax,mmax)
      real*8 soma
c 
      do j = 1,nlinx       ! colunas da 2a. matriz
        
      do i = 1,nlinx       ! linhas da 1a. matriz
        soma = 0.0
        inicio = iplin(i)
        ifim = iplin(i + 1) - 1
      do ip = inicio,ifim       
        m = kcoluna(ip)
        soma = soma + elem(ip)*ymatriz(m,j)
      enddo
        zmatriz(i,j) = soma
      enddo

      enddo
c
      return
      end         ! Fim de multmm4 --> prod. matrizes tipo (mxnb).(nbxm)



