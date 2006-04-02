c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para simular contingencia simples [ Metodo Pos Compensacao ]
      
      subroutine conting_single(ce,cle,kk,mcont,impr,tids)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension ce(max),cle(max)
c
      integer*4 impr,kk,mcont
c
      character*8 info
c
c Declaracao relativa ao PVM
c
      integer*4 tids(0:50)
    
c
c Inicializa vetores
c
      do k1 = 1,nb
        wjk1(k1) = 0.
        wjk2(k1) = 0.
      enddo

c Guarda informacoes do caso base

      info = 'guarda'
      call ricupero(info)
c
c Identifica o ramo contingenciado
c
      i = kcont(kk,1)
      j = ni(i)             ! numeracao interna
      k = nf(i)
ccc      jx = nex(j)           ! numeracao externa
ccc      kx = nex(k)
      icont = kk

ccc      write(*,*)'(Ramo:',i,') Saida do ramo <numeracao externa>',jx,
ccc     &'--',kx,' Caso ',kk
      
c Verifica se o ramo j--k e trafo ou linha ==> define ejk1()

      call identdc(j,k,i)   ! definicao em funcao dos tipos das barras < Blinha >

      call identca(j,k,i)   ! definicao em funcao dos tipos das barras < B2linhas >
c      
c Rotinas de vetores esparsos
c 
       call caminho(j,k)              ! obtem caminhos parciais e faz uniao
     
c
c Obtem os vetores wjk1() e wjk2() 
c   
      call soluve(ce,wjk1) 
   
      call soluve(cle,wjk2)

c Altera os elementos da matriz Yo = Go + j.Bo em funcao da contingencia

c Modifica Yo:

      call modify(j,k,i)

c
c Obtem os escalares c1p e c2p <--> saida simples
c
      wb1 = 1./(fcont*b1kl(i))   ! retirada de todo o ramo ==> fcont = 1

      wb2 = - xjk(i)/fcont       ! retirada de todo o ramo ==> fcont = 1.
     

      c1p = (wb1 + ej1*wjk1(j) + ek1*wjk1(k)) ! wb1 = elemento que saiu <impedancia>

      c2p = (wb2 + ej2*wjk2(j) + ek2*wjk2(k)) ! wb2 = elemento que saiu

c
c Teste de ilhamento  < Ja foi feito na Fase DC >
c
      if( DABS(c1p) .lt. 1.D-6 .or. DABS(c2p) .lt. 1.D-6) then
ccc        print*,'OCORREU ILHAMENTO !!!!! '
        kdiag = -1
        goto 100
      else
        c1p = 1./c1p
        c2p = 1./c2p
        kdiag = 0            ! contingencia normal
      endif 


c Inicializa o estado   < outra alternativa >
cc      do iii = 1,nb   
cc        v(iii) = 1.0
cc        tet(iii) = 0.0
cc      enddo
      
      
c
c Inicio do processo iterativo < contingencia: fluxo de carga CA compensado >
c       
      call state_pos(ce,cle,j,k,kk)


c Forma lista de casos que convergiram < contingencia CA >

      if( kdiag .eq. 0 ) then
        nyconv = nyconv + 1
        kconv(nyconv) = kk
        kit1(nyconv) = nitea            ! no. de iteracoes ativas
        kit2(nyconv) = niter            ! no. de iteracoes reativas
      endif


c Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==> comentar p/ tomar tempos

      if( impr .eq. icont ) then

        call fluxos        ! fluxos ativos e reativos nos ramos

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c ----------------------------------------------- 

        do iii = 1,nb   
          vsend(iii) = v(iii)  
          tsend(iii) = t(iii)
        enddo
        

        msgtype0 = 11
        call PVMFINITSEND( PVMRAW, iflag )
        call PVMFPACK( INTEGER4, icont, 1, 1,  iflag )
        call PVMFPACK( INTEGER4, kdiag, 1, 1, iflag )
        call PVMFPACK( INTEGER4, nitea, 1, 1, iflag )
        call PVMFPACK( INTEGER4, niter, 1, 1, iflag )

        call PVMFPACK( REAL8, vsend, nb, 1, iflag )
        call PVMFPACK( REAL8, tsend, nb, 1, iflag )
         
        call PVMFPACK( REAL8, p, nb, 1, iflag )
        call PVMFPACK( REAL8, q, nb, 1, iflag ) 
        call PVMFPACK( REAL8, fpkm, nl, 1, iflag )
        call PVMFPACK( REAL8, fpmk, nl, 1, iflag ) 
        call PVMFPACK( REAL8, rqkm, nl, 1, iflag )
        call PVMFPACK( REAL8, rqmk, nl, 1, iflag )
        call PVMFSEND( tids(0), msgtype0, iflag )

      endif
      
  100 continue            ! ocorreu ilhamento
     

c Restabelece informacoes do caso base

      info = 'recupera'
      call ricupero(info)


      return
      end          ! Fim de conting_single  [ Pos Compensacao ]

c
c ----------------------------------------------------------------------
c Subrotina para simular contingencia multipla [ Metodo Mid Compensation]
      
      subroutine conting_multiple(ce,cle,kk,mcont,impr,tids)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension ce(max),cle(max)
c
      dimension C1(mmax,mmax),WTH1(mmax,mmax),WB1(mmax)
      dimension C2(mmax,mmax),WTH2(mmax,mmax),WB2(mmax)
      dimension Wtil1(nbmax,mmax),W1(nbmax,mmax)
      dimension Wtil2(nbmax,mmax),W2(nbmax,mmax)          
c
      integer*4 impr,kk,mcont
c
      character*8 info
c
c Declaracao relativa ao PVM
c
      integer*4 tids(0:50)


c Inicializa vetores e matrizes

      do k1 = 1,nb           ! melhorar isto
        wjk1(k1) = 0.0
        wjk2(k1) = 0.0
      enddo
      
      do k1 = 1,mcont
        WB1(k1) = 0.0
        WB2(k1) = 0.0
      enddo

      do k1 = 1,nb
      do k3 = 1,mcont
        W1(k1,k3)     = 0.0
        W2(k1,k3)     = 0.0
        Wtil1(k1,k3)  = 0.0
        Wtil2(k1,k3)  = 0.0
        W1t(k3,k1) = 0.0
        W2t(k3,k1) = 0.0
      enddo
      enddo
      
      do k1 = 1,mcont
      do k3 = 1,mcont
        WTH1(k1,k3) = 0.0
        C1(k1,k3) = 0.0
        WTH2(k1,k3) = 0.0
        C2(k1,k3) = 0.0
      enddo
      enddo
       
c Guarda informacoes do caso base

      info = 'guarda'
      call ricupero(info)

c Geracao de vetores e matrizes <ramo-a-ramo para cada caso>

 
      do 10 jj = 1,mcont        ! inicio do loop 10


c Identifica ramo j--k do caso contingenciado

      i = kcont(kk,jj)
      j = ni(i)             ! numeracao interna
      k = nf(i)
ccc      jx = nex(j)           ! numeracao externa
ccc      kx = nex(k)
      icont = kk

ccc      write(*,*)'(Ramo:',i,') Saida do ramo <numeracao externa>',jx,
ccc     &'--',kx,' Caso ',kk

c Verifica se o ramo j--k e trafo ou linha ==> define ejk1()

      call identdc(j,k,i)   ! definicao em funcao dos tipos das barras < Blinha >

      call identca(j,k,i)   ! definicao em funcao dos tipos das barras < B2linhas >

             
c Obtem caminhos parciais e faz uniao < do metodo dos vetores esparsos >

      call caminho(j,k)         ! une os caminhos dos "singletons"

c Solucao pelo metodo dos vetores esparsos 

      call soluffs(ce,wjk1)        ! faz somente "FFS"

      call soluffs(cle,wjk2)       ! faz somente "FFS"

c Gera as matrizes Wtil1 e Wtil2 <ordem (nbxm)>

      do l = 1,nb
         Wtil1(l,jj) = wjk1(l)
         Wtil2(l,jj) = wjk2(l)      
         wjk1(l) = 0.0
         wjk2(l) = 0.0
      enddo

c Altera os elementos da matriz Yo = Go + j.Bo em funcao da contingencia

c Modifica Yo:

      call modify(j,k,i) 

c Define matrizes diagonais (de alteracao da rede)

      WB1(jj) = 1./(fcont*b1kl(i))     ! retirada de ramo ==> fcont = 1.


      WB2(jj) = - xjk(i)/fcont            ! retirada de ramo ==> fcont = 1. 
      
   10 continue             ! fim do loop 10 
     

c Obtem as matrizes W1 e W2 a partir de Wtil1 e Wtil2 (e as transpostas: W1t e W2t)    

      do k1 = 1,mcont                           ! colunas
      do k2 = 1,nb                              ! linhas
        W1(k2,k1) = (1./ce(k2))*Wtil1(k2,k1)
        W2(k2,k1) = (1./cle(k2))*Wtil2(k2,k1)
        W1t(k1,k2) = W1(k2,k1)                  ! W1 transposta
        W2t(k1,k2) = W2(k2,k1)                  ! W2 transposta
      enddo
      enddo


c Realizando o produto (Wtil transposta) x W para obter WTH

c 1.] Geracao da matriz WTH1 de Thevenin

      call multmm3(W1t,Wtil1,mcont,nb,WTH1)     ! W1_transposta.Wtil_1 = WTH1

c 2.] Geracao da matriz WTH2 de Thevenin

      call multmm3(W2t,Wtil2,mcont,nb,WTH2)     ! W2_transposta.Wtil_2 = WTH2  


      do 20 linha = 1, mcont
        WTH1(linha,linha) = WTH1(linha,linha) + WB1(linha)
        WTH2(linha,linha) = WTH2(linha,linha) + WB2(linha)
   20 continue


      do 30 ncoluna = 1,mcont
      do 30 linha = 1,mcont
           
        C1(linha,ncoluna) = WTH1(linha,ncoluna)
        C2(linha,ncoluna) = WTH2(linha,ncoluna)

   30 continue            ! fim do loop 30


c Inversa da matriz C1 <ordem (mxm)> por pivoteamento <<Metodo P/ Matriz Simetrica Tipo Rede>>
c     [ Singularidade de C1 ==> ILHAMENTO ]

      call inversa(C1,mcont)     ! inversa retornada em C1 
      
c Teste de ILHAMENTO <Singularide de WB1 + WTH1>  < Ja foi feito na Fase DC >

      if( kdiag .eq. -1 ) then
ccc        print*,'OCORREU ILHAMENTO !!!!! '
        goto 100                   ! valor de kdiag vem da rotina inversa()
      else
        call inversa(C2,mcont)     ! inversa retornada em C2
        kdiag = 0                  ! contingencia normal
      endif
      
c Prepara para fazer compensacao ...

      call multmm2(Wtil1,C1,nb,mcont,RESULT1)   ! retorna resultado em RESULT1

      call multmm2(Wtil2,C2,nb,mcont,RESULT2)   ! retorna resultado em RESULT2



c Inicializa o estado   < outra alternativa >
cc      do iii = 1,nb   
cc        v(iii) = 1.0
cc        tet(iii) = 0.0
cc      enddo

       
c
c Inicio do processo iterativo < contingencia: fluxo de carga CA mid-compensation >
c 
      call state_mid(ce,cle,kk,mcont)


c Forma lista de casos que convergiram < contingencia CA >

      if( kdiag .eq. 0 ) then
        nyconv = nyconv + 1
        kconv(nyconv) = kk
        kit1(nyconv) = nitea            ! no. de iteracoes ativas
        kit2(nyconv) = niter            ! no. de iteracoes reativas
      endif
     

c Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==> comentar p/ tomar tempos

      if( impr .eq. icont ) then

        call fluxos        ! fluxos ativos e reativos nos ramos

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c ----------------------------------------------- 

        do iii = 1,nb   
          vsend(iii) = v(iii)  
          tsend(iii) = t(iii)
        enddo

        msgtype0 = 11
        call PVMFINITSEND( PVMRAW, iflag )
        call PVMFPACK( INTEGER4, icont, 1, 1,  iflag )
        call PVMFPACK( INTEGER4, kdiag, 1, 1, iflag )
        call PVMFPACK( INTEGER4, nitea, 1, 1, iflag )
        call PVMFPACK( INTEGER4, niter, 1, 1, iflag )

        call PVMFPACK( REAL8, vsend, nb, 1, iflag )
        call PVMFPACK( REAL8, tsend, nb, 1, iflag )
         
        call PVMFPACK( REAL8, p, nb, 1, iflag )
        call PVMFPACK( REAL8, q, nb, 1, iflag ) 
        call PVMFPACK( REAL8, fpkm, nl, 1, iflag )
        call PVMFPACK( REAL8, fpmk, nl, 1, iflag ) 
        call PVMFPACK( REAL8, rqkm, nl, 1, iflag )
        call PVMFPACK( REAL8, rqmk, nl, 1, iflag )
        call PVMFSEND( tids(0), msgtype0, iflag )

      endif

      
  100 continue            ! ocorreu ilhamento
     

c Restabelece informacoes do caso base

      info = 'recupera'
      call ricupero(info)

      return
      end          ! Fim de conting_multiple  [ Mid-compensation ]

c
c ----------------------------------------------------------------------
c Subrotina para simular contingencia multipla [ Metodo Refatoracao Parcial]

      subroutine refatora(ce,cle,kk,mcont,impr,tids)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'
c
      dimension ce(max),cle(max)
      dimension cebase(max),clebase(max)
      dimension ekm(nbmax)             
c
      integer*4 impr,kk,mcont
c
      character*8 info
c
c Declaracao relativa ao PVM
c
      integer*4 tids(0:50)

          
c Guarda informacoes do caso base

      info = 'guarda'
      call ricupero(info)

c Guarda matrizes B' e B'' do caso base

      do k1 = 1,lfmax      ! veja a def. de lfmax no arquivo orden.f < o mais seguro e usar max >
        cebase(k1) = ce(k1)      ! diagonal e off-diagonal
        clebase(k1) = cle(k1)
      enddo
      
c Geracao de vetores e matrizes <ramo-a-ramo para cada caso>

      do 10 jj = 1,mcont        ! inicio do loop 10

c Identifica ramo j--k do caso contingenciado

      i = kcont(kk,jj)
      j = ni(i)             ! numeracao interna
      k = nf(i)
ccc      jx = nex(j)           ! numeracao externa
ccc      kx = nex(k)
      icont = kk

ccc      write(*,*)'(Ramo:',i,') Saida do ramo <numeracao externa>',jx,
ccc     &'--',kx,' Caso ',kk

c Verifica se o ramo j--k e trafo ou linha ==> define ejk1()

      call identdc(j,k,i)   ! definicao em funcao dos tipos das barras < Blinha >

      call identca(j,k,i)   ! definicao em funcao dos tipos das barras < B2linhas >
      
c Obtem caminhos parciais e faz uniao < do metodo dos vetores esparsos >

      call caminho(j,k)         ! une os caminhos dos "singletons"


c Define os elementos de alteracao da rede < admitancia >

      WB1 =   fcont*b1kl(i)    ! retirada de todo o ramo ==> fcont = 1. 

      WB2 =  - fcont/xjk(i)       ! retirada de todo o ramo ==> fcont = 1. 

c Atualiza os fatores da matriz B'

      ekm(j) = ej1
      ekm(k) = ek1

      call bennett(cebase,ekm,WB1)

c Atualiza os fatores da matriz B''

      ekm(j) = ej2
      ekm(k) = ek2

      call bennett(clebase,ekm,WB2)   

c Altera os elementos da matriz Yo = Go + j.Bo em funcao da contingencia

c Modifica Yo:

      call modify(j,k,i) 
   
   10 continue             ! fim do loop 10 



c Inicializa o estado   < outra alternativa >
cc      do iii = 1,nb   
cc        v(iii) = 1.0
cc        tet(iii) = 0.0
cc      enddo
       
     
c
c Inicio do processo iterativo < contingencia: fluxo de carga CA >
c 
      call state_normal(cebase,clebase,kk)


c Forma lista de casos que convergiram < contingencia CA >

      if( kdiag .eq. 0 ) then
        nyconv = nyconv + 1
        kconv(nyconv) = kk
        kit1(nyconv) = nitea            ! no. de iteracoes ativas
        kit2(nyconv) = niter            ! no. de iteracoes reativas
      endif     
 


c Saida da analise de contingencia <diagnostico e/ou estado/fluxos> ==> comentar p/ tomar tempos

      if( impr .eq. icont ) then

        call fluxos        ! fluxos ativos e reativos nos ramos

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c ----------------------------------------------- 

        do iii = 1,nb   
          vsend(iii) = v(iii)  
          tsend(iii) = t(iii)
        enddo

        msgtype0 = 11
        call PVMFINITSEND( PVMRAW, iflag )
        call PVMFPACK( INTEGER4, icont, 1, 1,  iflag )
        call PVMFPACK( INTEGER4, kdiag, 1, 1, iflag )
        call PVMFPACK( INTEGER4, nitea, 1, 1, iflag )
        call PVMFPACK( INTEGER4, niter, 1, 1, iflag )

        call PVMFPACK( REAL8, vsend, nb, 1, iflag )
        call PVMFPACK( REAL8, tsend, nb, 1, iflag )
         
        call PVMFPACK( REAL8, p, nb, 1, iflag )
        call PVMFPACK( REAL8, q, nb, 1, iflag ) 
        call PVMFPACK( REAL8, fpkm, nl, 1, iflag )
        call PVMFPACK( REAL8, fpmk, nl, 1, iflag ) 
        call PVMFPACK( REAL8, rqkm, nl, 1, iflag )
        call PVMFPACK( REAL8, rqmk, nl, 1, iflag )
        call PVMFSEND( tids(0), msgtype0, iflag )

      endif


      
  100 continue            ! ocorreu ilhamento
     

c Restabelece informacoes do caso base

      info = 'recupera'
      call ricupero(info)

      return
      end            ! Fim de refatora    [ Refatoracao Parcial ]

