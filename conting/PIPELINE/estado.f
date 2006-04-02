c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para processar as iteracoes do calculo de fluxo de carga AC
c -----------FAZ CHAMADAS RELATIVAS AS ROTINAS DE ZOLLENKOPF------------

c
c FONTE DESTE ALGORITMO: Fluxograma da pag. 95 do Livro do A. Monticelli
c


      subroutine estado(ce,cle)           ! chamada no programa principal
c
      include 'globvar.inc'
c
      dimension  ce(max),cle(max)
      dimension  dx1(nbmax),dx2(nbmax)
c
      intrinsic DMAX1
     

c ----------------------------------
c Inicializando contadores e chaves
c ----------------------------------

      nitea = 0            ! contadores de iteracoes
      niter = 0
      ntot  = 0

      kp = 1           ! chaves
      kq = 1


     
c ---------------------------------------------------------
c Montando as Sub-matrizes Jacobianas B' e B'' (Versao BX)
c ---------------------------------------------------------
   
c
c Formando a matriz B'
c   

      call matb1(ce)
      nn = 1
      call elim(nn,ce)

c
c Formando a matriz B''
c 

      call matb2(cle)
      nn = 0
      call elim(nn,cle)

      

c ------------------------------------------------------------------
c Obtendo Fatores Triangulares das Sub-matrizes Jacobianas B' e B''
c ------------------------------------------------------------------

c
c  Chamada da rotina redu() - Fatoracao LDU
c
      call redu(ce)




c
c  Chamada da rotina redu() - Fatoracao LDU
c
      call redu(cle)





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

      dpmax = 0.0
      do k = 1,nb
         dx1(k) = pes(k) - p(k) 
         
cc         write(*,*)' dx1(',k,') = ',dx1(k)
 
         dp = ABS(dx1(k))
         dx1(k) = dx1(k)/v(k)
         if( itipo(k) .ne. 2 ) dpmax = DMAX1(dpmax,dp)
      enddo 



c == Teste de convergencia (fase ATIVA)

      if( dpmax .gt. 1.d+10 ) then
          write(*,*)' DIVERGENCIA !!'
      endif

      if( dpmax .le. tol ) then
          kp = 0
          if( kq .eq. 0 ) then
              goto 1000           ! encontrada solucao 
          else
              goto 1050
          endif         
      endif


c == Teste de iteracoes (fase ATIVA) 

      if( nitea .gt. lim ) goto 999 

 
c == Solucao do sistema linear P-TETA para MISMATCH ATIVO    
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B'.dtet = dx1
c

      call solu(ce,dx1)


c == Atualiza o Vetor de Angulos Nodais TETA 
     
      
      do k = 1,nb
         tet(k) = tet(k) + dx1(k)
         t(k) = tet(k)
      enddo 

  
c == Incrementa o Contador de Iteracoes Ativas

      nitea = nitea + 1

cc      write(*,*)' Atualiza Iteracao Ativa No. ',nitea

      
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

      dqmax = 0.0
      do k = 1,nb
         dx2(k) = qes(k) - q(k)

cc         write(*,*)' dx2(',k,') = ',dx2(k)        

         dq = ABS(dx2(k))
         if( itipo(k) .le. 0 ) then
             dqmax = DMAX1(dqmax,dq)
             dx2(k) = dx2(k)/v(k) 
         endif
      enddo 



c == Teste de convergencia (fase REATIVA)

      if( dqmax .gt. 1.d+10 ) then
          write(*,*)' DIVERGENCIA !!'
      endif

      if( dqmax .le. tol ) then
          kq = 0
          if( kp .eq. 0 ) then
              goto 1000              ! encontrada solucao 
          else
              goto 2050
          endif         
      endif


c == Teste de iteracoes (fase REATIVA) 

      if( niter .gt. lim ) goto 999 

 
c == Solucao do sistema linear Q-V para MISMATCH REATIVO    
     
c
c  Chamada da rotina solu() - Solucao do Sist. Linear B''.dv = dx2
c

      call solu(cle,dx2)




c == Atualiza o Vetor de Magnitudes Nodais V 
     
      
      do k = 1,nb
         v(k) = v(k) + dx2(k)
      enddo 

  
c == Incrementa o Contador de Iteracoes Reativas

      kq = 1             ! Baleeiro estudou isto em 01/09/95
      niter = niter + 1

cc      write(*,*)' Atualiza Iteracao Reativa No. ',niter



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
      nconv = 1
      write(*,555)ntot
  555 format(5x,' O Processo Nao CONVERGE com ',i4,' Meias Iteracoes!!')

      
      
c --------------------------------------
c Fim do Processo Iterativo com Sucesso
c -------------------------------------- 
     
 1000 continue   
      return 
      end               ! Fim da subrotina estado

