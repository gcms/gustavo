c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c            [ Arquivo com uma rotina para inverter matriz ]
c                   
c  SUBROTINA PARA INVERSAO DE MATRIZES 
      subroutine inversa(aa,nordem)
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension aa(mmax,mmax)
      real*8 cte
      integer*4 nordem     

c Inicializando diagnostico
      kdiag = 0          ! contingencia normal
     
      do 13 i=1,nordem
      cte=aa(i,i)
      aa(i,i)=1.
      if( DABS(cte) .lt. 1.D-6 ) goto 14   ! ILHAMENTO 
      do 15 j=1,nordem
15    aa(i,j)=aa(i,j)/cte
      do 913 k=1,nordem
      if(k.eq.i)  goto 9914
      cte=aa(k,i)
      aa(k,i)=0.0
      do 1013 j=1,nordem
      if( DABS(aa(i,j)) .ne. 0. ) aa(k,j) = aa(k,j) - cte*aa(i,j)
1013  continue
9914  continue
913   continue
13    continue
      goto 20

  14  kdiag = -1        ! matriz singular significa ILHAMENTO
      
cc14    write(*,340)
cc340   format(/,2x,'matriz singular')

20    return
      end              ! Fim da subrotina de inversao de matriz


