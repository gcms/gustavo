c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para calculo das injecoes ativa e reativa nas barras

      subroutine potencia    ! chamada no inicio da subrotina estado
c
      include 'globvar.inc'
      intrinsic sin,cos
c
      do 2 k=1,nb            ! loop 1 --> propria (kk)
        if(itipo(k).gt.0)v(k)=ves(k)
        vkk2=v(k)*v(k)      
        p(k)=gdiag(k)*vkk2
        q(k)=-bdiag(k)*vkk2
    2 continue 
       
      do 3 i=1,nl            ! loop 2 --> transferencia (kl)
       k=ni(i)
       l=nf(i)
       vkl=v(k)*v(l)
       g=gkl(i)
       b=bkl(i)
       tkl=tet(k)-tet(l)
       si=sin(tkl)
       co=cos(tkl)
       p(k)=p(k)+vkl*(-g*co-b*si)
       p(l)=p(l)+vkl*(-g*co+b*si)
       q(k)=q(k)+vkl*(-g*si+b*co)
       q(l)=q(l)+vkl*(g*si+b*co)
    3 continue

      return
      end        ! Fim da subrotina potencia

