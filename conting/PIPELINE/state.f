c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para processar as iteracoes do fluxo de carga pos-compensado

      subroutine state_pos(ce,cle,jj,kk,mm)
c
      include 'globvar.inc'
      include 'varcont.inc'     
c
      dimension dtet(nbmax),dv(nbmax)
      dimension dta(nbmax),dva(nbmax)
      dimension ce(max),cle(max) 
c
      intrinsic DMAX1


c ----------------------------------
c Inicializando contadores e chaves
c ---------------------------------- 

      kdiag = 0      ! variavel que fornece o diagnostico     
      nitea = 0
      niter = 0
      ntot  = 0

      kp = 1         ! chaves
      kq = 1


c ----------------------------
c Inicia o processo iterativo
c ----------------------------

 333  continue


c -----------------
c ITERACOES ATIVAS
c -----------------

c == Calculo das injecoes de potencia nas barras

 203  continue
      call potencia

 
c == "Zerando" posicoes de vetores
 
      do ix = 1,nb 
        dta(ix) = 0.0
        dva(ix) = 0.0
      enddo 
 
    
c == Calculo do MISMATCH ATIVO e do seu valor maximo

      dtpmax = 0.0
      do k = 1,nb
         dta(k) = pes(k) - p(k) 
         dtp = DABS(dta(k))
         dta(k) = dta(k)/v(k)
         if( itipo(k) .ne. 2 ) dtpmax = DMAX1(dtpmax,dtp)
      enddo 


c == Teste de convergencia (fase ATIVA)

      if( dtpmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!:contingencia --> ',icont
          ndiv = ndiv + 1
          kdiverge(ndiv) = mm  ! vetor que guarda as divergencias ocorridas
          kdiag = 2            ! significa divergencia
          goto 1000
      endif

      if( dtpmax .le. toler ) then
          kp = 0
          if( kq .eq. 0 ) then
              goto 1000           ! encontrada solucao 
          else
              goto 1050
          endif         
      endif


c == Teste de iteracoes (fase ATIVA) 

      if( nitea .gt. limite ) goto 999 


c == Solucao do sistema linear P-TETA para MISMATCH ATIVO   
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B'.dta = dp/v
c

      call solu(ce,dta)

       
c
c Compensando os incrementos de angulos
c
      dtc = - (ej1*dta(jj) + ek1*dta(kk))*c1p
      do l = 1,nb
         dtet(l) = dta(l) + dtc*wjk1(l)       	
      enddo


c == Atualiza o Vetor de Angulos Nodais TETA
 
      do k = 1,nb
        tet(k) = tet(k) + dtet(k)  ! atualiza os angulos
	t(k) = tet(k)
      enddo


c == Incrementa o Contador de Iteracoes Ativas

      nitea = nitea + 1

      
c == Retorna a chave REATIVA ao valor inicial

      kq = 1
      

     
   
 1050 continue


c -------------------
c ITERACOES REATIVAS
c -------------------  


c == Calculo das injecoes de potencia nas barras

 303  continue
      call potencia 


c == "Zerando" posicoes de vetores

      do ix = 1,nb 
         dta(ix) = 0.0
         dva(ix) = 0.0
      enddo 


c == Calculo do MISMATCH REATIVO e do seu valor maximo

      dtqmax = 0.0
      do k = 1,nb
         dva(k) = qes(k) - q(k) 
         dtq = DABS(dva(k))
         if( itipo(k) .le. 0 ) then
             dtqmax = DMAX1(dtqmax,dtq)
             dva(k) = dva(k)/v(k) 
         endif
      enddo 


c == Teste de convergencia (fase REATIVA)

      if( dtqmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!:contingencia --> ',icont
          ndiv = ndiv + 1
          kdiverge(ndiv) = mm  ! vetor que guarda as divergencias ocorridas
          kdiag = 2            ! significa divergencia
          goto 1000
      endif

      if( dtqmax .le. toler ) then
          kq = 0
          if( kp .eq. 0 ) then
              goto 1000           ! encontrada solucao 
          else
              goto 2050
          endif         
      endif


c == Teste de iteracoes (fase REATIVA) 

      if( niter .gt. limite ) goto 999 


c == Solucao do sistema linear Q-V para MISMATCH REATIVO   
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B''.dva = dq/v
c

      call solu(cle,dva)

  
c
c Compensando os incrementos de modulos de tensoes
c
      dvc = - (ej2*dva(jj) + ek2*dva(kk))*c2p
      do l=1,nb
        dv(l) =  dva(l) + dvc*wjk2(l)    	
      enddo
     


c == Atualiza o Vetor de Magnitudes Nodais V

      do 17 k = 1,nb
        v(k) = v(k) + dv(k) ! atualiza as tensoes
  17  continue


c == Incrementa o Contador de Iteracoes Reativas

      kq = 1             ! Baleeiro estudou isto em 01/09/95
      niter = niter + 1

      
c == Retorna a chave ATIVA ao valor inicial

      kp = 1
      

c --------------------------------------
c Volta ao Inicio do Processo Iterativo
c --------------------------------------

 2050 continue
      goto 333 
    

c -----------------------------------------------------------------
c Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
c -----------------------------------------------------------------

  999 continue
      ntot = nitea + niter
      nnconv = nnconv + 1 
      knotconv(nnconv) = mm       ! lista de nao convergencias
      kdiag = 1                   ! significa nao convergencia
cc      write(*,555)ntot
cc  555 format(5x,'O Fluxo CA Pos-Compensado Nao Converge Com ',i4, 
cc     &' Meias Iteracoes!!')


c --------------------------------------
c Fim do Processo Iterativo com Sucesso
c -------------------------------------- 
     
 1000 continue   
      return
      end    ! Fim da subrotina state_pos() (que calcula estado pos-compensado)




c ----------------------------------------------------------------------
c Subrotina para processar as iteracoes do fluxo de carga mid-compensado

      subroutine state_mid(ce,cle,kk,mcont)
c
      include 'globvar.inc'
      include 'varcont.inc'     
c
      dimension dtet(nbmax),dv(nbmax)
      dimension ce(max),cle(max) 
c
      dimension fc1(nbmax),f1(nbmax),deltaf1(nbmax)
      dimension fc2(nbmax),f2(nbmax),deltaf2(nbmax)
      dimension prod(mmax)
c
      intrinsic DMAX1


c ----------------------------------
c Inicializando contadores e chaves
c ---------------------------------- 

      kdiag = 0      ! variavel que fornece o diagnostico     
      nitea = 0
      niter = 0
      ntot  = 0

      kp = 1         ! chaves
      kq = 1


c ----------------------------
c Inicia o processo iterativo
c ----------------------------

 333  continue


c -----------------
c ITERACOES ATIVAS
c -----------------

c == Calculo das injecoes de potencia nas barras

 203  continue
      call potencia

 
c == "Zerando" posicoes de vetores
 
      do ix = 1,nb       
        deltaf1(ix) = 0.0
        dtet(ix)    = 0.0
        deltaf2(ix) = 0.0
        dv(ix)      = 0.0        
      enddo
 
 
    
c == Calculo do MISMATCH ATIVO e do seu valor maximo

      dtpmax = 0.0
      do k = 1,nb
         fc1(k) = pes(k) - p(k)
         dtp = DABS(fc1(k))
         fc1(k) = fc1(k)/v(k)
         if( itipo(k) .ne. 2 ) dtpmax = DMAX1(dtpmax,dtp)
      enddo 


c == Teste de convergencia (fase ATIVA)

      if( dtpmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!:contingencia --> ',icont
          ndiv = ndiv + 1
          kdiverge(ndiv) = kk  ! vetor que guarda as divergencias ocorridas
          kdiag = 2            ! significa divergencia
          goto 1000
      endif

      if( dtpmax .le. toler ) then
          kp = 0
          if( kq .eq. 0 ) then
              goto 1000           ! encontrada solucao 
          else
              goto 1050
          endif         
      endif


c == Teste de iteracoes (fase ATIVA) 

      if( nitea .gt. limite ) goto 999


c == Solucao do sistema linear P-TETA para MISMATCH ATIVO

c
c Solucao do sistema L'.D'.fc1 = dp/v 
c
      call solufs(ce,fc1)

c
c Calculo do vetor de compensacao deltaf1()
c
      call multmv4(W1t,fc1,mcont,nb,prod)


      call multmv2(RESULT1,prod,nb,mcont,deltaf1)

 
c
c Compensando ...
c 
      
      do l = 1,nb
         f1(l) = fc1(l) - deltaf1(l)    
         dtet(l) = f1(l)     	
      enddo

c
c Completando a solucao da rede ...
c

c
c Resolve o sistema U'.dtet = f1
c
      call solubs(ce,dtet)

    

c == Atualiza o Vetor de Angulos Nodais TETA
 
      do k = 1,nb
        tet(k) = tet(k) + dtet(k)  ! atualiza os angulos
	t(k) = tet(k)
      enddo


c == Incrementa o Contador de Iteracoes Ativas

      nitea = nitea + 1

c      write(*,*)' Atualiza Iteracao Ativa No. ',nitea


      
c == Retorna a chave REATIVA ao valor inicial

      kq = 1
      

     
   
 1050 continue


c -------------------
c ITERACOES REATIVAS
c -------------------  


c == Calculo das injecoes de potencia nas barras

 303  continue
      call potencia 


c == "Zerando" posicoes de vetores
 
      do ix = 1,nb       
        deltaf1(ix) = 0.0
        dtet(ix)    = 0.0
        deltaf2(ix) = 0.0
        dv(ix)      = 0.0        
      enddo
 
 
    
c == Calculo do MISMATCH REATIVO e do seu valor maximo

      dtqmax = 0.0
      do k = 1,nb
         fc2(k) = qes(k) - q(k) 
         dtq = DABS(fc2(k))
         if( itipo(k) .le. 0 ) then
             dtqmax = DMAX1(dtqmax,dtq)
             fc2(k) = fc2(k)/v(k)
         endif
      enddo 


c == Teste de convergencia (fase REATIVA)

      if( dtqmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!: contingencia --> ',icont
          ndiv = ndiv + 1
          kdiverge(ndiv) = kk ! vetor que guarda as divergencias ocorridas
          kdiag = 2           ! significa divergencia
          goto 1000
      endif

      if( dtqmax .le. toler ) then
          kq = 0
          if( kp .eq. 0 ) then
              goto 1000              ! encontrada solucao 
          else
              goto 2050
          endif         
      endif


c == Teste de iteracoes (fase REATIVA) 

      if( niter .gt. limite ) goto 999


c == Solucao do sistema linear Q-V para MISMATCH REATIVO

c
c Solucao do sistema L''.D''.fc2 = dq/v 
c
      call solufs(cle,fc2)

c
c Calculo do vetor de compensacao deltaf2()
c
      call multmv4(W2t,fc2,mcont,nb,prod)


      call multmv2(RESULT2,prod,nb,mcont,deltaf2)

 
c
c Compensando ...
c 
      
      do l = 1,nb
         f2(l) = fc2(l) - deltaf2(l)    
         dv(l) = f2(l)     	
      enddo

c
c Completando a solucao da rede ...
c

c
c Resolve o sistema U''.dv = f2
c
      call solubs(cle,dv)

    

c == Atualiza o Vetor de Magnitudes Nodais V 
 
      do k = 1,nb
        v(k) = v(k) + dv(k)   ! atualiza as tensoes
      enddo

 

c == Incrementa o Contador de Iteracoes Reativas

      kq = 1             ! Baleeiro estudou isto em 01/09/95
      niter = niter + 1

c      write(*,*)' Atualiza Iteracao Reativa No. ',niter

      
c == Retorna a chave ATIVA ao valor inicial

      kp = 1
      

     
   
 2050 continue
      goto 333 
    

c -----------------------------------------------------------------
c Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
c -----------------------------------------------------------------

  999 continue
      ntot = nitea + niter
      nnconv = nnconv + 1 
      knotconv(nnconv) = kk       ! lista de nao convergencias
      kdiag = 1                   ! significa nao convergencia
cc      write(*,555)ntot
cc  555 format(5x,'O Fluxo CA Mid-Compensado Nao Converge Com ',i4, 
cc     &' Meias Iteracoes!!')


c --------------------------------------
c Fim do Processo Iterativo com Sucesso
c -------------------------------------- 
     
 1000 continue   
      return
      end    ! Fim da subrotina state_mid() (que calcula estado mid-compensado)




c ----------------------------------------------------------------------
c Subrotina para processar as iteracoes do fluxo de carga parcial refatorado

      subroutine state_normal(cebase,clebase,kk)
c
      include 'globvar.inc'
      include 'varcont.inc'     
c
      dimension dtet(nbmax),dv(nbmax)   
      dimension cebase(max),clebase(max) 
c
      intrinsic DMAX1
     

c ----------------------------------
c Inicializando contadores e chaves
c ----------------------------------

      kdiag = 0
      nitea = 0            ! contadores de iteracoes
      niter = 0
      ntot  = 0

      kp = 1           ! chaves
      kq = 1



c ----------------------------
c Inicia o processo iterativo
c ----------------------------

 333  continue


c -----------------
c ITERACOES ATIVAS
c -----------------

c == Calculo das injecoes de potencia nas barras

 203  continue
      call potencia 



c == Calculo do MISMATCH ATIVO e do seu valor maximo 

cc      write(*,*) 
cc      write(*,*)' Iteracao Ativa No. ',nitea

      dtpmax = 0.0
      do k = 1,nb
         dtet(k) = pes(k) - p(k) 
         dtp = DABS(dtet(k))
         dtet(k) = dtet(k)/v(k)
         if( itipo(k) .ne. 2 ) dtpmax = DMAX1(dtpmax,dtp)
      enddo 



c == Teste de convergencia (fase ATIVA)

      if( dtpmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!: contingencia --> ',icont
           ndiv = ndiv + 1
           kdiverge(ndiv) = kk ! vetor que guarda as divergencias ocorridas
           kdiag = 2           ! significa divergencia 
           goto 1000
      endif

      if( dtpmax .le. toler ) then
          kp = 0
          if( kq .eq. 0 ) then
              goto 1000           ! encontrada solucao 
          else
              goto 1050
          endif         
      endif


c == Teste de iteracoes (fase ATIVA) 

      if( nitea .gt. limite ) goto 999 

 
c == Solucao do sistema linear P-TETA para MISMATCH ATIVO    
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B'.dtet = dp/v
c

      call solu(cebase,dtet)


c == Atualiza o Vetor de Angulos Nodais TETA 
     
      
      do k = 1,nb
         tet(k) = tet(k) + dtet(k)
         t(k) = tet(k)
      enddo 

  
c == Incrementa o Contador de Iteracoes Ativas

      nitea = nitea + 1

c      write(*,*)' Atualiza Iteracao Ativa No. ',nitea

      
c == Retorna a chave REATIVA ao valor inicial

      kq = 1
      

     
   
 1050 continue
   
c -------------------
c ITERACOES REATIVAS
c -------------------  


c == Calculo das injecoes de potencia nas barras

 303  continue
      call potencia 



c == Calculo do MISMATCH REATIVO e do seu valor maximo 

cc      write(*,*) 
cc      write(*,*)' Iteracao Reativa No. ',niter

      dtqmax = 0.0
      do k = 1,nb
         dv(k) = qes(k) - q(k)
         dtq = DABS(dv(k))
         if( itipo(k) .le. 0 ) then
             dtqmax = DMAX1(dtqmax,dtq)
             dv(k) = dv(k)/v(k) 
         endif
      enddo 



c == Teste de convergencia (fase REATIVA)

      if( dtqmax .gt. 1.D+10 ) then
c          write(*,*)' DIVERGENCIA !!: contingencia --> ',icont
          ndiv = ndiv + 1
          kdiverge(ndiv) = kk ! vetor que guarda as divergencias ocorridas
          kdiag = 2           ! significa divergencia
          goto 1000
      endif

      if( dtqmax .le. toler ) then
          kq = 0
          if( kp .eq. 0 ) then
              goto 1000              ! encontrada solucao 
          else
              goto 2050
          endif         
      endif


c == Teste de iteracoes (fase REATIVA) 

      if( niter .gt. limite ) goto 999 

 
c == Solucao do sistema linear Q-V para MISMATCH REATIVO    
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B''.dv = dq/v
c

      call solu(clebase,dv)




c == Atualiza o Vetor de Magnitudes Nodais V 
     
      
      do k = 1,nb
         v(k) = v(k) + dv(k)
      enddo 

  
c == Incrementa o Contador de Iteracoes Reativas

      kq = 1             ! Baleeiro estudou isto em 01/09/95
      niter = niter + 1

c      write(*,*)' Atualiza Iteracao Reativa No. ',niter



c == Retorna a chave ATIVA ao valor inicial

      kp = 1
 
  
     
c --------------------------------------
c Volta ao Inicio do Processo Iterativo
c --------------------------------------

 2050 continue
      goto 333 
 

c -----------------------------------------------------------------
c Fim do Processo Iterativo por Exceder o Limite Esp. de Iteracoes
c -----------------------------------------------------------------

  999 continue
      ntot = nitea + niter
      nnconv = nnconv + 1 
      knotconv(nnconv) = kk       ! lista de nao convergencias
      kdiag = 1                   ! significa nao convergencia
cc      write(*,555)ntot
cc  555 format(5x,'O Fluxo CA Parcial-Refatorado Nao Converge Com ',i4, 
cc     &' Meias Iteracoes!!')

      
      
c --------------------------------------
c Fim do Processo Iterativo com Sucesso
c -------------------------------------- 
     
 1000 continue   
      return
      end    ! Fim da subrotina state_normal() (que calcula estado por refatoracao parcial)


