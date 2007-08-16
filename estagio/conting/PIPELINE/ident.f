c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c                     Refere-se a Matriz B1linha
c ----------------------------------------------------------------------
c Subrotina para identificar o ramo e definir os parametros de alteracao
c [ Versao p/ Contingencia Simp./Multip. Pos/Mid Compensation/DC Incr. ] 
c   
      subroutine identdc(jj,kk,ii)
c  
      include 'globvar.inc'
      include 'varcont.inc' 
     
      ej1 = 0.0
      ek1 = 0.0 
      
c23456789     << Seja linha ou transformador >>
      if( itipo(jj) .le. 1 .and. itipo(kk) .le. 1 ) then ! cobre toda combinacao sem "slack"
         ej1 = 1.
         ek1 = -1.
      else                     ! cobre as combinacoes que contenham "slack"
        if( itipo(jj) .eq. 2 ) then
          if( itipo(kk) .eq. 0 .or. itipo(kk) .eq. 1 ) then
            ek1 = -1.      ! ref. |-----| PQ  e  ref. |-----| PV               
          endif
        else
          if( itipo(jj) .eq. 0 .or. itipo(jj) .eq. 1 ) then
            ej1 = 1.       ! PQ |-----| ref.  e  PV |-----| ref. 
          endif
        endif       
      endif
                     
c Atribuicao wjk1() <-- ej1 & ek1

      wjk1(jj) = ej1
      wjk1(kk) = ek1
      
      return
      end    ! Fim da subrotina identdc  (p/ Matriz Blinha)


c ----------------------------------------------------------------------
c                     Refere-se a Matriz B2linhas
c ----------------------------------------------------------------------
c Subrotina para identificar o ramo e definir os parametros de alteracao
c   [ Versao para Contingencia Multipla CA <Fluxo> Mid Compensation ]
c  
      subroutine identca(jj,kk,ii)
c  
      include 'globvar.inc'
      include 'varcont.inc'  

      ej2 = 0.0
      ek2 = 0.0
      
c23456789
      xjk(ii) = 1.e+20
      if( itipo(jj) .eq. 0 ) then 
        ej2 = 1.
        xjk(ii) = xkl(ii)        ! verificar atraves de testes se e realmente necessario
      endif
      if( itipo(kk) .eq. 0 ) then 
        ek2 = -1.
        xjk(ii) = xkl(ii)        ! verificar atraves de testes se e realmente necessario
      endif
      if( tap(ii) .ne. 0. ) then
        if( itipo(jj) .eq. 0 ) ej2 = 1./tap(ii)    ! PQ |-----| ref., PV, PQ
      endif

                    
c Atribuicao  wjk2() <-- ej2 & ek2
      wjk2(jj) = ej2
      wjk2(kk) = ek2
     
      return
      end    ! Fim da subrotina identca  (p/ Matriz B2linhas)




