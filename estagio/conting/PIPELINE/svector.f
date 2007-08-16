c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c [ Arquivo com as rotinas relativas ao metodo dos vetores esparsos ]
c
      subroutine grafo

      include 'globvar.inc'
      include 'varcont.inc'

c Construir a lista next(i) ==> Tabela de caminhos de fatoracao --> obtida do Sato  

      do k = 1,nb
        il = lcol(k)
        min = nb+1
        do while (il .ne. 0)
          m = itag(il)
          if(m .ne. k)then
            j = lrd(m)     ! ordem em que a barra foi fatorada
            if(j .lt. min) then
              min = j
              mnxt = m
            endif
          endif
          if(min .ne. (nb+1))then
            next(k) = mnxt
          else
            next(k) = 0    ! existe apenas um elemento (a diagonal)
          endif
          il = lnxt(il)
        enddo       ! do while
      enddo         ! k = 1,nb


c ----------------------------------------------------------------------
c      WRITE(*,*)'      TABELA DE CAMINHOS '
c      WRITE(*,7999)
c7999  FORMAT(/,2x,'      I     NEXT(I)',/)
c      do 8000 i = 1,nb
c8000  WRITE(*,8001)i,next(i)
c8001  FORMAT(6x,i3,6x,i3)
c ----------------------------------------------------------------------

      return
      end          ! Fim da subrotina grafo  (lista geral)


c ----------------------------------------------------------------------
      subroutine caminho(j1,k1)


      include 'globvar.inc'
      include 'varcont.inc'

      dimension ntex(nbmax),iaux(nbmax),nv(2)

      do 6 i = 1,nbmax
        ntex(i) = 0
        iaux(i) = 0
   6  continue

c Obtem os caminhos parciais de fatoracao para as barras j e k 
c  =========== [ CUIDADO ao fazer mudancas aqui! ] ===========

      nv(1) = j1
      nv(2) = k1
     
      ip = 0
      do 8 j = 1,2      ! 2 caminhos parciais
        k = nv(j)

   7    continue

        ip = ip + 1
        ntex(k) = ip    ! o indice de ntex() e o caminho parcial
        kam = next(k)
        k = kam
        if( kam .ne. 0 ) goto 7
   8  continue

c Obtem a uniao dos caminhos parciais na ordem de fatoracao das barras

      ifb = 0
     
c ---------------------------------------------------------------

cc      write(*,3014)
cc      write(*,3051)
cc3014  format(/,2x,'  UNIAO DOS SUBGRAFOS  ')
cc3051  format(/,'   I         NFASTFOR(I) ')
c ---------------------------------------------------------------


      do 12 iii = 1,nb
        if(ntex(iii) .eq. 0) goto 12
        ifb = ifb + 1
        iaux(ifb) = ntex(iii)       
  12  continue

      ifbt = ifb       ! total de elementos de iaux()
     
c Ordena vetor em ordem crescente
      
      call bubble(iaux,ifb)            
      
      do 70 ik = 1,ifbt
        do 71 jk = 1,nb     
          if(iaux(ik) .eq. ntex(jk)) goto 72
  71    continue

  72    continue
        nfastfor(ik) = jk       ! caminho resultante da uniao
cc        write(*,3061)ik,nfastfor(ik)

  70  continue

cc3061  format(i4,9x,i4)

      return
      end                    ! Fim de caminho

c ---------------------------------------------------------------

      subroutine bubble(ix,j)  ! subrotina para ordenar um vetor

      include 'globvar.inc'
      include 'varcont.inc'

      dimension ix(nbmax)

      if(j.le.1)go to 100
      do 200 ka1 = 1,j-1
      do 300 ka2 = ka1+1,j
      if( ix(ka1) .le. ix(ka2) ) goto 400
          ktemp = ix(ka1)
          ix(ka1) = ix(ka2)
          ix(ka2) = ktemp
400   continue
300   continue
200   continue
100   continue

      return
      end          ! Fim da subrotina bubble

c ---------------------------------------------------------------------
      subroutine soluve(xce,u)
c
      include 'globvar.inc'
      include 'varcont.inc'
c     
      dimension xce(max),u(nbmax)       

c Fast Forward
      
      do ki = 1,ifbt
        k = nfastfor(ki)      
        cf=u(k)
        u(k)=0.
        l=lcol(k)
            do while (l.gt.0)
              i=itag(l)
              u(i)=u(i)+cf*xce(l)
              l = lnxt(l)
            enddo
      enddo

      
c Fast Backward

c      do  j = ifbt,1,-1
c       k = nfastfor(j)
c       sum=u(k)
c       l=lcol(k)
c           do while (l.gt.0)
c             i=itag(l)
c             if(i.ne.k) then
c              sum=sum+u(i)*xce(l)
c             endif
c             l=lnxt(l)
c           enddo
c           u(k)=sum
c      enddo
 
C**********************************************
C**         BACKWARD CONVENCIONAL
C**********************************************    
      nb1=nb-1
      do 70 jj=1,nb1
      j=nb-jj
      k=nseq(j)
      sum=u(k)
      l=lcol(k)
 30     i=itag(l)
      if(i-k)40,50,40
 40     x1=xce(l)

c --------------
c POR SEGURANCA:
c --------------
        
        if( i .ne. 0 ) goto 1500
        print*,' xcel(',l,') =',x1
        print*,' nseq(',j,') =',k
        print*,' lcol(',k,') =',l
        print*,' itag(',l,') =',i
 1500 continue

      x2=u(i)
      sum=sum+x1*x2
 50     l=lnxt(l)
      if(l)60,60,30
 60     u(k)=sum
 70   continue

      return
      end     ! Fim da subrotina soluve (para vetor esparso)


c ---------------------------------------------------------------------
      subroutine soluffs(xce,u)
c
      include 'globvar.inc'
      include 'varcont.inc'
c     
      dimension xce(max),u(nbmax)

c Fast Forward
      
      do ki = 1,ifbt
        k = nfastfor(ki)      
        cf=u(k)
        u(k)=0.
        l=lcol(k)
            do while (l.gt.0)
              i=itag(l)
              u(i)=u(i)+cf*xce(l)
              l = lnxt(l)
            enddo
      enddo
      
      return
      end     ! Fim da subrotina soluffs (FAST-FORWARD SUBSTITUTION apenas)


c ---------------------------------------------------------------------
      subroutine solufs(xce,u)
c
      include 'globvar.inc'
c
      dimension xce(max),u(nbmax)
     

c Forward Convencional

      do 20 j=1,nb
      k=nseq(j)
      cf=u(k)
      u(k)=0.
      l=lcol(k)
 10     i=itag(l)
      x1=xce(l)
      u(i)=u(i)+cf*x1
      l=lnxt(l)
      if(l)20,20,10
 20     continue
     
      return
      end      ! Fim da subrotina solufs (FORWARD SUBSTITUTION apenas)


c ---------------------------------------------------------------------
      subroutine solubs(xce,u)
c
      include 'globvar.inc'
      include 'varcont.inc'
c     
      dimension xce(max),u(nbmax)

c Backward Convencional

      nb1=nb-1
      do 70 jj=1,nb1
      j=nb-jj
      k=nseq(j)
      sum=u(k)
      l=lcol(k)
 30     i=itag(l)
      if(i-k)40,50,40
 40     x1=xce(l)
      x2=u(i)
      sum=sum+x1*x2
 50     l=lnxt(l)
      if(l)60,60,30
 60     u(k)=sum
 70   continue
   
      return
      end     ! Fim da subrotina solubs (BACKWARD SUBSTITUTION apenas)
