c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para atualizar os fatores L, D e U de uma matriz dada

      subroutine bennett(xce,ekm,b) ! chamada pela subrotina refatora
c 
      include 'globvar.inc'
      include 'varcont.inc' 
c
      dimension xce(max)
      dimension ekm(nbmax)
      real*8 dd,xll,c,b
          
c --------------------------------------------------------------------- 
    
      do 200 ki = 1,ifbt
c ----------------------------------
c Loop sobre o caminho de fatoracao
c ----------------------------------
        i = nfastfor(ki)    ! caminho de fatoracao --> define a coluna
        il = lcol(i) 

c23456789123456789

c -------------------------------
c Atualiza elementos da diagonal
c -------------------------------
        dd = 1./xce(i)           ! dd = elemento de D
          
        dd = dd + b*ekm(i)*ekm(i)
      

c ------------------------
c Para Detectar ILHAMENTO (26/9/95)
c ------------------------
        if( DABS(dd) .lt. 1.D-6 ) kdiag = -1
        
         
        c = b*ekm(i) 
             
        xce(i) = 1./dd           ! xce = elemento de D inv.


        do while (il .gt. 0 )

        l = itag(il)               ! l = linha da matriz
       
        if( l .ne. i ) then
c ----------------------------------
c Loop sobre elementos off-diagonal
c ----------------------------------       

          xll = -xce(il)           ! xll = elemento de L 

          ekm(l) = ekm(l) - ekm(i)*xll

          xll = xll + c*ekm(l)/dd 
    
          xce(il) = -xll           ! xce = elemento de -L

        endif

        il = lnxt(il)              ! caminha na coluna "i"

c23456789123456789

        enddo                      ! final do loop do do while
 
        b = b - c*c/dd
                     
  200 continue     ! final do loop sobre o caminho de fatoracao

 
c ----------------------------------
c Limpa as posicoes do vetor ekm(.)
c ----------------------------------
      do k1 = 1,nb
        ekm(k1) = 0.0        
      enddo

      return
      end                ! Fim da subrotina bennett
        
