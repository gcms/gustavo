c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c
c ----------------------------------------------------------------------
c Subrotina de preparacao para explorar a esparsidade das matrizes

      subroutine esparsa(ce,cle)   ! chamada no programa principal

      include 'globvar.inc'
c
      dimension ce(max), cle(max)
c
c Formacao da estrutura da matriz sem elementos
c
c     [1.- preparacao da estrutura esparsa]
      do 103 i=1,max
         cle(i)=0.
         ce(i)=0.
         itag(i)=0
         lnxt(i)=i+1
  103 continue
      do 105 i=1,nb
         lcol(i)=i
         nseq(i)=i       ! util no processo de ordenacao
         noze(i)=1
         itag(i)=i
         lnxt(i)=0
  105 continue

      lnxt(max)=0
      lf=1+nb
c
c     [2.- inclusao do "endereco" dos elementos]
c     [		  na estrutura esparsa         ]
      do 109 i=1,nl
         k=ni(i)          ! estes vetores trazem a topologia da rede
         l=nf(i)
         call inclu(k,l)  ! Ari organizou de forma mais inteligente
         call inclu(l,k)  
  109 continue
c 
c Neste ponto, os vetores da bi-fatorizacao contem os enderecos dos elementos
c nao-nulos das matrizes de rede armazenadas em forma compacta, sem ordenacao
c
      call orden(ce,cle)  ! ordenacao para minimizar quantidade de "fill-in"

      do 113 i=1,nb
         ki=nseq(i)      ! contem a sequencia de pivoteamento (ou fatoracao)
         lrd(ki)=i       ! indice de nseq()
  113 continue

      return   
      end    ! Fim da subrotina esparsa

