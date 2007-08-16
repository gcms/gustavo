c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para guardar/restabelecer Yo e estado do caso base 
     
      subroutine ricupero(info)

      include 'globvar.inc'
      include 'varcont.inc'
c
      character*8 info

  
      if( info .eq. 'guarda' ) then

c Guarda a matriz Yo = Go + j.Bo do caso base (para posterior utilizacao)

      do 6 kbus = 1,nb                 ! Diagonal
        gdiagbase(kbus) = gdiag(kbus)   
        bdiagbase(kbus) = bdiag(kbus) 
    6 continue
     
      do 7 li = 1,nl                   ! Elementos de ramo 
        gklbase(li) = gkl(li)          
        bklbase(li) = bkl(li)
       
cc        print*,'GUARDANDO .... bklbase(',li,') =',bklbase(li)
cc        print*,'GUARDANDO .... bkl(',li,') =',bkl(li)
         
    7 continue


c Guarda o estado do caso base <para posterior utilizacao>

      do iii = 1,nb   ! guarda o caso base em vb() e tetab()
        vb(iii) = v(iii)
        tetab(iii) = tet(iii)
      enddo
     
      endif
      
      if( info .eq. 'recupera' ) then

c Restabelece a matriz Yo = Go + j.Bo a sua forma original

      do 60 kbus = 1,nb                 ! Diagonal
        gdiag(kbus) = gdiagbase(kbus)   
        bdiag(kbus) = bdiagbase(kbus)
   60 continue
     
      do 70 li = 1,nl                   ! Elementos de ramo
        gkl(li) = gklbase(li)         
        bkl(li) = bklbase(li)
        
cc        print*,'RECUPERANDO .... bklbase(',li,') =',bklbase(li)
cc        print*,'RECUPERANDO .... bkl(',li,') =',bkl(li)
       
   70 continue


c Restabelece o estado do caso base <para utilizacao no proximo caso>

      do iii = 1,nb         
        v(iii) = vb(iii)
        tet(iii) = tetab(iii)
      enddo

      endif

      return
      end         ! Fim de ricupero

c
c ----------------------------------------------------------------------
c Subrotina para modificar a matriz Y em funcao da contingencia

      subroutine modify(j1,k1,ii) 
c
      include 'globvar.inc'
      include 'varcont.inc' 

c OBS:  fcont e o fator de contingencias de ramo [-1.<= fcont =<+1.] <var. global>
c       fcont e introduzido para permitir a retirada de uma fracao da admitancia 
c       do ramo. Se fcont = 1.0 tira-se o ramo integralmente.

c OBS: ramos paralelos (transformador ou linha) sao armazenados individualmente
c      nos vetores gdiag(), bdiag(), gkl() e bkl()

c OBS: ter bastante cuidado com a maneira como estao armazenados os parametros do
c      circuito PI de transformadores em-fase

     
      if( tap(ii) .eq. 0.0 ) then            ! linha de transmissao
          gdiag(j1) = gdiag(j1) - fcont*gkl(ii)
          gdiag(k1) = gdiag(k1) - fcont*gkl(ii)
          bdiag(j1) = bdiag(j1) - fcont*bkl(ii) - fcont*ylin(ii)
          bdiag(k1) = bdiag(k1) - fcont*bkl(ii) - fcont*ylin(ii)
      else                                   ! transformador
          t1 = 1./tap(ii)
          gdiag(j1) = gdiag(j1) - fcont*gkl(ii)*t1
          gdiag(k1) = gdiag(k1) - fcont*gkl(ii)/t1
          bdiag(j1) = bdiag(j1) - fcont*bkl(ii)*t1
          bdiag(k1) = bdiag(k1) - fcont*bkl(ii)/t1                    
      endif

      gkl(ii) = gkl(ii) - fcont*gkl(ii)       
      bkl(ii) = bkl(ii) - fcont*bkl(ii)                     

      return
      end          ! Fim da subrotina modify
