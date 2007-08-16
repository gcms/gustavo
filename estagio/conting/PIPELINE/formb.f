c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                                ZOLENKOPF
c-----------------------------------------------------------------------
c Subrotina para incluir os enderecos na estrutura esparsa

      subroutine inclu(il,ik)   ! chamada pela subrotina esparsa

      include 'globvar.inc'
c
      k=ik
      l=il
   10 in=lcol(k)
      ina=0
   20 m=itag(in)
      if(l-m)30,70,60
   30 noze(k)=noze(k)+1
      if(ina.eq.0)go to 50
      inn=lnxt(ina)
      lnxt(ina)=lf
      ilf=lnxt(lf)
      lnxt(lf)=inn
   40 itag(lf)=l
      if(ilf.eq.0)go to 80
      lf=ilf
      go to 70
   50 lcol(k)=lf
      ilf=lnxt(lf)
      lnxt(lf)=in
      go to 40
   60 ina=in
      in=lnxt(in)
      if(in.ne.0)go to 20
      go to 30
   70 return
   80 write(*,100)
  100 format(/,1x,'memoria insuficiente - inclu***')
      write(*,*)'lf,max',lf,max
      end       ! Fim da subrotina inclu



c
c ----------------------------------------------------------------------
c Subrotina de preenchimento da matriz 

      subroutine preench(k,l,x,xce)  ! chamada pelas rotinas matb1 e matb2

      include 'globvar.inc'
c
      dimension xce(max)

      p1=lrd(k)
      p2=lrd(l)
      if(p1-p2)31,41,51
  31  le=l
      lc=lcol(k)
      go to 21
  51  le=k
      lc=lcol(l)
  21  ita=itag(lc)
      if(le-ita)100,71,81
  71  xce(lc)=x+xce(lc)
      go to 100
  81  lc=lnxt(lc)
      if(lc.gt.0) go to 21
      go to 100
 41   lc=lcol(k)
      le=k
      go to 21
  100 return
      end      ! Fim da subrotina preench


c
c ----------------------------------------------------------------------       
c Subrotina para eliminar barra "slack" ou barras "slack" e PV  

      subroutine elim(nn,xce)  ! chamada pela subrotina estado

      include 'globvar.inc'

      dimension xce(max)
c
c	nn=1 elimina barra slack
c	nn=0 elimina barras slack e PVs
c
c Valores que itipo() pode assumir:
* 0 - barra PQ
* 1 - barra PV
* 2 - barra "slack

      do 2 k=1,nb
        if(itipo(k).le.0)go to 2  
        if(itipo(k).eq.1.and.nn.eq.1)go to 2
        xce(k)=1.e+20         ! introduz numero grande na diagonal
    2 continue
      return
      end


c
c ----------------------------------------------------------------------
c Subrotina para montar a matriz B' explorando a estrutura esparsa

      subroutine matb1(ce)   ! monta a matriz B'

      include 'globvar.inc'  
c
      dimension ce(max)

c      write(*,*)' * subr. matb1 (BX) *'

      do 40 i=1,max
       ce(i)=0.
  40  continue

      do 20 i=1,nl
        k=ni(i)
        l=nf(i)
        ybl=b1kl(i)           ! elemento de B'
        ybl1=-ybl
        ce(k) = ce(k) + ybl1
        ce(l) = ce(l) + ybl1
        call preench(k,l,ybl,ce)
   20 continue

      return
      end      ! Fim da subrotina matb1

c ----------------------------------------------------------------------
c Subrotina para montar a matriz B'' explorando a estrutura esparsa

      subroutine matb2(cle)    ! monta a matriz B''

      include 'globvar.inc'
c
      dimension cle(max)

c      write(*,*)' * subr. matb2 (BX) *'
      do 40 i=1,max
       cle(i)=0.
  40  continue

      do 20 i=1,nl
        k=ni(i)
        l=nf(i)
cc        yb = -1./xkl(i)         ! comentado em 08/09/95
        yb = b2kl(i)              ! elemento de B''
        yb1=-yb
        if( tap(i) .eq. 0.0 ) then 
            yb1=-yb
        else
            t1 = 1./tap(i)
cc            yb1=-yb*t1          ! elemento de B'' (comentado em 08/9/95)
        endif 
        cle(k) = cle(k) + yb1
        cle(l) = cle(l) + yb1 
        call preench(k,l,yb,cle)
  20  continue

      do 21 i=1,nb
    	xb1=-2.*shu(i)      ! shu(.) e o shunt total
        call preench(i,i,xb1,cle)
  21  continue

      return     
      end      ! Fim da subrotina matb2

