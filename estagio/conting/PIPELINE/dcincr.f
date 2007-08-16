c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para simular contingencia simples [ Metodo DC Incremental]
      
      subroutine outage_single(ce,kk,mcont,impr)

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'

      dimension ce(max)
c
      integer*4 impr,kk,mcont
c
      character*8 info

c Inicializa vetores

      do k1 = 1,nb
        wjk1(k1) = 0.0
      enddo

c Guarda informacoes do caso base

      info = 'guarda'
      call ricupero(info)

c Identifica ramo j--k do caso contingenciado

      i = kcont(kk,1)
      j = ni(i)             ! numeracao interna
      k = nf(i)
ccc      jx = nex(j)           ! numeracao externa
ccc      kx = nex(k)
      icont = kk
      
ccc      print*,'(Ramo:',i,') Saida do ramo <numeracao externa>',jx,'--',kx

c Verifica se o ramo j--k e trafo ou linha ==> define ejk1()

      call identdc(j,k,i)   ! definicao em funcao dos tipos das barras

c Obtem caminhos parciais e faz uniao < do metodo dos vetores esparsos >

      call caminho(j,k)    ! une os caminhos dos "singletons"

c Solucao da equacao (22) <do "paper"> pelo metodo dos vetores esparsos

      call soluve(ce,wjk1)  ! faz "FFS & NBS"

c Obtem o equivalente de Thevenin da rede vista de j--k

      wt = ej1*wjk1(j) + ek1*wjk1(k)  ! wt (= escalar) impedancia equivalente

c Define a mudanca no ramo j--k
      
      wb = 1./(fcont*b1kl(i))   ! retirada de todo o ramo ==> fcont = 1.

cc      wb = - xkl(i)/fcont       ! retirada de todo o ramo ==> fcont = 1.



    

c Teste de ILHAMENTO <comparar wt com a impedancia de ramo wb --> retirada>
     
      if( ABS(wt + wb) .lt. 1.D-6 ) then
cc         print*,'OCORREU ILHAMENTO !!!!! '
         kdiag = -1         ! diagnostico
         nilha = nilha + 1
         kilha(nilha) = kk  ! vetor que guarda os ilhamentos ocorridos
         goto 100           ! ocorreu ilhamento
      else
         kdiag = 0          ! contingencia normal
      endif
   
c Altera os elementos da matriz Yo = Go + j.Bo em funcao da contingencia

c Modifica Yo:
      call modify(j,k,i)  

c Calcula o incremento angular <solucao da equacao (11) do "paper">

      expressao = (wb/(wt + wb))*fpjk(i)    ! retirada do ramo j--k
      do k2 = 1,nb
        deltateta =  wjk1(k2)*expressao         
        tet(k2) = tet(k2) + deltateta
        t(k2) = tet(k2)
      enddo

   

c Calculo do indice de severidade
      
      call flux            ! fluxo DC
      pia(kk) = rval       ! indice  de severidade       


  100 continue            ! ocorreu ilhamento
       

c Restabelece informacoes do caso base
     
      info = 'recupera'
      call ricupero(info)  
       
      return
      end              ! Fim da rotina outage_single [ DC Incremental ]


c
c ----------------------------------------------------------------------
c Subrotina para simular contingencia multipla [ Metodo DC Incremental]
      
      subroutine outage_multiple(ce,kk,mcont,impr)

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
c
      dimension ce(max)
c
      dimension A(mmax,mmax),WB(mmax)
      dimension W(nbmax,mmax),WTH(mmax,mmax)
c
      dimension elem(2*mmax)                      ! vetor que contem a matriz M transposta
      integer*4 iplin(2*mmax+1),icoluna(2*mmax+1)  ! vetor c/ apontadores e indicador da coluna
c    
      dimension dteta(nbmax),pjk(mmax),dpjk(mmax)
c
      integer*4 impr,kk,mcont
c
      character*8 info


c Inicializa vetores e matrizes

      do k1 = 1,nb           ! melhorar isto
        wjk1(k1) = 0.0
      enddo

      do k3 = 1,mcont
        pjk(k3)  = 0.0 
        dpjk(k3)  = 0.0
        WB(k3) = 0.0
      enddo

      do k1 = 1,nb
      do k3 = 1,mcont
        W(k1,k3) = 0.0
      enddo
      enddo
      
      do k1 = 1,mcont
      do k3 = 1,mcont
        WTH(k1,k3) = 0.0
        A(k1,k3) = 0.0
      enddo
      enddo
       
      icontador = 0
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

ccc      print*,'(Ramo:',i,') Saida do ramo <numeracao externa>',jx,
ccc     >'--',kx,' Caso ',kk

c Verifica se o ramo j--k e trafo ou linha ==> define ejk1()

      call identdc(j,k,i)   ! definicao em funcao dos tipos das barras

c Armazenamento Compacto da Matriz M transposta <WTH = Mtransposta.W>
c ------------------------------------------------------------------------
        kchavej = 0
        kchavek = 0

        if ( ej1 .eq. 1. ) then
          icontador = icontador + 1
 
          elem(icontador) = ej1
          icoluna(icontador) = j
 
          kchavej = 1    
        endif

        if ( ek1 .eq. -1. ) then
          icontador = icontador + 1
           
          elem(icontador) = ek1 
          icoluna(icontador) = k
          
          kchavek = 1
        endif

        if ( kchavej + kchavek .eq. 2 ) iplin(jj) = icontador - 1  
        if ( kchavej + kchavek .eq. 1 ) iplin(jj) = icontador 
c ------------------------------------------------------------------------             
c Obtem caminhos parciais e faz uniao < do metodo dos vetores esparsos >

      call caminho(j,k)         ! une os caminhos dos "singletons"
      
c Solucao da equacao (22) <do "paper"> pelo metodo dos vetores esparsos

      call soluve(ce,wjk1)       ! faz "FFS & NBS" 

c Gera a matriz W <ordem (nbxm)> a partir da solucao de (22)

      do l = 1,nb
         W(l,jj) = wjk1(l)      
         wjk1(l) = 0.0
      enddo

c Gera a matriz diagonal de alteracoes < num vetor >

      WB(jj) = 1./(fcont*b1kl(i))   ! retirada de ramo ==> fcont = 1.

cc      WB(jj) = - xkl(i)/fcont        ! retirada de ramo ==> fcont = 1. (alternativa)

c Atribui fluxo ativo do caso base num vetor  [1..mcont]

      pjk(jj) = fpjk(i)  ! fluxo ativo do caso base nos ramos contingenciados

c Altera os elementos da matriz Yo = Go + j.Bo em funcao da contingencia

c Modifica Yo:
      call modify(j,k,i) 
      
   10 continue             ! fim do loop 10 

c ------------------------------------------------------------------------     
      iplin(mcont + 1) = icontador + 1   ! ultimo elemento do apontador
c ------------------------------------------------------------------------

c Geracao da matriz WTH de Thevenin

c Multiplica as matrizes Mt.W  <retorna resultado em WTH>

c Obtem o equivalente de Thevenin  < matriz WTH >:

      call multmm4(elem,iplin,icoluna,W,mcont,WTH)     ! xM.W = WTH <--> Thevenin 

      do 30 linha = 1,mcont            
        WTH(linha,linha) = WTH(linha,linha) + WB(linha)   ! so elementos da diagonal
   30 continue            ! fim do loop 30

      do 35 ncoluna = 1,mcont
      do 35 linha = 1,mcont
            
        A(linha,ncoluna) = WTH(linha,ncoluna)
         
   35 continue            ! fim do loop 35

c Inversa da matriz A <ordem (mxm)> por pivoteamento <<Metodo P/ Matriz Simetrica Tipo Rede>>
c     [ Singularidade de A ==> ILHAMENTO ]

      call inversa(A,mcont)     ! inversa retornada em A 
      
c Teste de ILHAMENTO <Singularide de WTH + WB>

      if( kdiag .eq. -1 ) then
cc        print*,'OCORREU ILHAMENTO !!!!! '
        nilha = nilha + 1
        kilha(nilha) = kk  ! vetor que guarda os ilhamentos ocorridos
        goto 100           ! valor de kdiag vem da rotina inversa()
      else
        kdiag = 0          ! contingencia normal
      endif

c Multiplica as matrizes quadradas A.WB [WB e uma matriz diagonal] <retorna resultado em A>

      do ncoluna = 1,mcont
      do linha = 1,mcont
        A(linha,ncoluna) = A(linha,ncoluna)*WB(ncoluna)
      enddo
      enddo


c Multiplica a matriz A resultante pelo vetor pjk()  <equacao (15) do "paper">
      
      call multmv1(A,pjk,mcont,dpjk)   ! retorna resultado em dpjk() 

c Calcula o incremento angular <solucao da equacao (16) do "paper">
 
      call multmv2(W,dpjk,nb,mcont,dteta)
      
c Atualiza angulos

      do k5 = 1,nb
        tet(k5) = tet(k5) + dteta(k5)
        t(k5) = tet(k5)
      enddo

c Calculo do indice de severidade
      
      call flux            ! fluxo DC nos ramos
      pia(kk) = rval       ! indice  de severidade

      
  100 continue            ! ocorreu ilhamento
     

c Restabelece informacoes do caso base

      info = 'recupera'
      call ricupero(info)

      return
      end          ! Fim de outage_multiple  [ DC Incremental ]

c
c ----------------------------------------------------------------------
c     [ Subrotina para calculo de fluxos ativos no modelo DC ]
c     [  Calcula a sobrecarga TOTAL para cada contingencia   ]
c
c
      subroutine flux

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'

      sobre = 0.
      do 2 j = 1,nl

        k = ni(j)
        m = nf(j)
        tkm = t(k) - t(m)
	b = bkl(j)
	pkm = -b*tkm        ! veja o "paper" do Irisarri, Sasson & Levner

        vainf = fmi(j)*.9
        vasup = fma(j)*.9

        if( pkm .gt. vasup ) then
	  	dife = pkm/vasup
        	sobre = sobre + dife*dife
        elseif( pkm .lt. vainf ) then
	  	dife= pkm/vainf
          	sobre = sobre + dife*dife
        endif

    2 continue

c   [ sobrecarga total ]

      rval = sobre

      return
      end
