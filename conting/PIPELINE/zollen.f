c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina que faz a ordenacao otima para minimizar quant. de "fill-in"

      subroutine orden(ce,cle)     ! chamada pela subrotina esparsa
c
      include 'globvar.inc'
c 
      dimension ce(max),cle(max)
      intrinsic MAX0

c
c Esta rotina confere integralmente com o fluxograma que esta no artigo
c do Zolenkopf  ... Pag. 89  (Conferido em 20/05/94)
c

c LEGENDA:
c ========
* kord = 0  ==> MD    (Tinney II)
* kord = 1  ==> MD-ML (Bettancourt)


c      write(*,*)' * orden *'
      
      nb1=nb-1
      lfm=0
      lfmax = 0
     
      do 190 j=1,nb1
      k=nseq(j)
c ---------------------------------------
      if( kord .ne. 0 ) minl = nprof(k)   ! MD-ML
c ---------------------------------------
      min=noze(k)
      m=j
      j1=j+1
      do 10 i=j1,nb
      k=nseq(i)
c ---------------------------------------
      if( kord .eq. 0 ) then
        if(noze(k).ge.min)goto 10
      else                                ! MD-ML
        if( noze(k)-min ) 220,210,10
210       if( nprof(k) .ge. minl ) goto 10
220       minl = nprof(k)                  
      endif
c ---------------------------------------
      min=noze(k)
      m=i
10      continue
      kp=nseq(m)
c ---------------------------------------
      if( kord .eq. 0 ) then
        nseq(m)=nseq(j)
      else                                ! MD-ML
        j2 = j + 1
        do 230 j1 = m,j2,-1
          j3 = j1 -1
          nseq(j1)=nseq(j3)
230     continue
      endif 
c ---------------------------------------   
      nseq(j)=kp
      lk=lcol(kp)
20      k=itag(lk)
      if(k.eq.kp)goto 130
      la=0
      li=lcol(kp)
      ip=itag(li)
      l=lcol(k)
      i=itag(l)
30      if(i-ip)40,60,140
40      la=l
      l=lnxt(l)                       ! AQUI
cc ------------------------------
      lfmax = MAX0(l,lfmax)
cc ------------------------------
      if(l.le.0)goto 50
      i=itag(l)
      goto 30
50      i=nb+1
      goto 30
60      if(i.ne.kp)goto 90
      ln=lnxt(l)                      ! AQUI
cc ------------------------------
      lfmax = MAX0(ln,lfmax)
cc ------------------------------
      if(la.gt.0)goto 70
      lcol(k)=ln
      goto 80
70      lnxt(la)=ln                   
80      lnxt(l)=lf
      ce(l)=0.
      cle(l)=0.
      lf=l
      noze(k)=noze(k)-1   
c ---------------------------------------   
      if( kord .ne. 0 ) then               ! MD-ML
        npro = nprof(kp) + 1
        if( npro .gt. nprof(k) ) nprof(k) = nprof(kp) + 1
      endif
c ---------------------------------------
      l=ln
      goto 100
90      la=l
      l=lnxt(l)                      ! AQUI
cc ------------------------------ 
      lfmax = MAX0(l,lfmax)
cc ------------------------------
100   if(l.gt.0)goto 110
      i=nb+1
      goto 120
110   i=itag(l)
120   li=lnxt(li)                    ! AQUI
cc ------------------------------
      lfmax = MAX0(li,lfmax)
cc ------------------------------
      if(li.le.0)goto 130
      ip=itag(li)
      goto 30
130   lk=lnxt(lk)                    ! AQUI
cc ------------------------------
      lfmax = MAX0(lk,lfmax)
cc ------------------------------
      if(lk)190,190,20
140   if(lf.le.0)goto 170
      ln=lf
      if(la.le.0)goto 150
      lnxt(la)=ln
      goto 160
150   lcol(k)=ln
160   lf=lnxt(ln)                    ! AQUI  
cc ------------------------------
      lfmax = MAX0(lf,lfmax)
cc ------------------------------        
      if(lfm.lt.lf)lfm=lf
      if(lfm.gt.max)goto 170 ! max e uma constante definida em 'globvar.inc'
      lnxt(ln)=l                   
      itag(ln)=ip
      noze(k)=noze(k)+1
      la=ln
      goto 120
170   write(*,*)'area reservada insuficiente '

190   continue

cc OBS: Baleeiro esta supondo que lfmax indica o fim da cadeia (11/11/94) 

c --------------
c POR SEGURANCA:
c --------------

      lfmax = lfmax + 10

      return
      end      ! Fim da subrotina orden


c
c ----------------------------------------------------------------------
c Subrotina para obter os fatores L, D e U de uma matriz dada

      subroutine redu(xce)   ! chamada pela subrotina estado
c 
      include 'globvar.inc' 
c
      dimension xce(max)
c
c      write(*,*)' * redu *'
c
      do 190 j=1,nb
      kp=nseq(j)
      lk=lcol(kp)
      lp=lf
 10     k=itag(lk)
      if(k-kp)20,30,20
 20   xce(lp)=xce(lk)
      goto 60
 30     continue
 50     d1=1./xce(lk)
      xce(lk)=d1
 60     lk=lnxt(lk)
      if(lk)80,80,70
 70     lp=lnxt(lp)
      goto 10
 80     lk=lcol(kp)
 90     k=itag(lk)
      if(k-kp)100,130,100
 100     cf1=d1*xce(lk)
      xce(lk)=-cf1
      lp=lf
      li=lcol(kp)
      ip=itag(li)
      l=lcol(k)
      i=itag(l)
 110     if(i-ip)120,150,170
 120     l=lnxt(l)
      if(l)130,130,140
 130     lk=lnxt(lk)
      if(lk)190,190,90
 140     i=itag(l)
      goto 110
 150     xce(l)=xce(l)-cf1*xce(lp)
      l=lnxt(l)
      if(l)130,130,160
 160     i=itag(l)
 170     li=lnxt(li)
      if(li)130,130,180
 180     ip=itag(li)
      lp=lnxt(lp)
      goto 110
 190     continue   

      return
      end


c
c ----------------------------------------------------------------------
c Subrotina para obter a solucao do sistema L.D.U.x = b

      subroutine solu(xce,u)   ! chamada pela subrotina estado
c
      include 'globvar.inc'
c
      dimension  u(nbmax)
      dimension xce(max)

c      write(*,*)' * solu *'
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
 70     continue
      return
      end      ! Fim da subrotina solu
