c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para entrada de dados iniciais

      subroutine menu

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
c
c
      write(*,6)
c ----------------------------------------------------------------------
    6 format(//,11x,'***********************************************',/,
     &'           *     PROGRAMA DE ANALISE DE CONTINGENCIA     * ',/,
     &'           *     VERSAO PARALELA/DISTRIBUIDA C/ PVM3     * ',/,
     &'           *      MODELO  P I P E L I N E  DINAMICO      * ',/,
     &'           *      FDLF-Screening-Post-Mid-Comp.-PMR      * ',/, 
     &'           *         [ Inicio: Janeiro - 1995 ]          * ',/,
     &'           *         [ Adapta: Fevereiro-1996 ]          * ',/,
     &'           *     Autor: Antonio Cesar Baleeiro Alves     * ',/,
     &'           *     Orientador: Alcir Jose Monticelli       * ',/,
     &'           ***********************************************',//)

c      write(*,10)
   10 format(5x,'Limite maximo permitido de iteracoes <fc>... : ')
      read(065,15)lim
   15 format(i2)

c      write(*,12)
   12 format(5x,'Limite maximo permitido de iteracoes <cont>... : ')
      read(065,13)limite
   13 format(i2)

c      write(*,20)
   20 format(5x,'Tolerancia [default=0.01 MW/MVAr] <fc> ... : ')
      read(065,25)tol
   25 format(f9.5)
      if(tol.eq.0.)tol=0.0001

c      write(*,22)
   22 format(5x,'Tolerancia [default=0.1 MW/MVAr] <cont> ... : ')
      read(065,23)toler
   23 format(f9.5)
      if(toler.eq.0.)tol=0.001 

c      write(*,40)
   40 format(//,5x,'Fator para resistencias [<enter>para default]: ')
      read(065,45)fres
   45 format(f12.6)
      if(fres.eq.0.)fres=1.0

c      write(*,50)
   50 format(//,5x,'Fator para reatancias [<enter>para default]: ')
      read(065,55)frea
   55 format(f12.6)
      if(frea.eq.0.)frea=1.0

c      write(*,60)
   60 format(/,5x,'Fator para cargas ativas [<enter>para default]: ')
      read(065,65)fpca
   65 format(f12.6)
      if(fpca.eq.0.)fpca=1.0

c      write(*,70)
   70 format(/,5x,'Fator para geracoes ativas [<enter>para default]: ')
      read(065,75)fpger
   75 format(f12.6)
      if(fpger.eq.0.)fpger=1.0

c      write(*,80)
   80 format(/,5x,'Fator para fluxos ativos [<enter>para default]: ')
      read(065,85)ffnor
   85 format(f12.6)
      if(ffnor.eq.0.)ffnor=1.0

c      write(*,90)
   90 format(/,5x,'Fator para contingencias [<enter>para default]: ')
      read(065,95)fcont
   95 format(f12.6)
      if(fcont.eq.0.)fcont=1.0

c      write(*,100)
  100 format(/,5x,'Tipo de ordenacao [<enter>para default]: ')
      read(065,105)kord
  105 format(i1)
      if(kord.eq.0)kord=0

c      write(*,110)
  110 format(/,5x,'Quant. de corte para contingencias [<enter>para defau
     &lt]: ')
      read(065,115)ncorte
  115 format(i5)
      if(ncorte.eq.0.)ncorte=0

c      write(*,120)
  120 format(/,5x,'Indice de corte para contingencias [<enter>para defau
     &lt]: ')
      read(065,125)corte
  125 format(f12.6)
      if(corte.eq.0.)corte=0.001

c      write(*,130)
  130 format(/,5x,'Formato de leitura [<enter>para default]: ')
      read(065,135)kformat
  135 format(i1)
      if(kformat.eq.0)kformat=0

c      write(*,140)
  140 format(/,5x,'Flag para modo de processamento paralelo [0/1]: ')
      read(065,145)modelo
  145 format(i2)
      

cc      write(*,*)' --> Fluxo de Carga CA do Caso Base ... Espere !'
cc      write(*,*)'                                                '

      return
      end      ! Fim da subrotina menu


c -------------
c Formato PECO:
c -------------

c ----------------------------------------------------------------------
c Subrotina para leitura dos dados de barras

      subroutine barra

      include 'globvar.inc' 
c
c      write(*,*)' * barra* '
      do 5 k=1,nb
      bdiag(k)=0.
    5 continue

      k=1
      carga=0.	
      do 1 i=1,10000
      read(066,10)jex,inact,iti,aa1,aa2,aa3,vx,tt,pg,qg,qgmi,qgma,
     &pc,qc,bsh
  10  format(i4,1x,i1,1x,i1,1x,3a4,f5.3,f4.0,4f5.0,5x,3f5.0)
      if(jex.eq.9999) goto 2
      itipo(i)=iti
c     [ considera apenas geracoes controlaveis (inact=0) ]
      if( iti .ge. 1 .and. inact .eq. 0 ) then
	k=k+1
      endif	
      nex(i)=jex

c    [ flat start ]
      v(i)=1.000   ! importante para inicializar o problema
      tet(i)=0.0   ! veja esta importancia na subrotina potencia
c
      ves(i)=vx
      pg=pg*fpger
      pc=pc*fpca
      pes(i)=(pg-pc)/100. 
      qes(i)=(qg-qc)/100.
      pca(i)=pc/100.
      qca(i)=qc/100.
      carga=pc+carga
      if( iti .eq. 0 ) carga=carga-pg	
      if( iti .eq. 0 ) goto 740
      qmin(i)=(qgmi-qc)/100.
      qmax(i)=(qgma-qc)/100.
      if(qgmi.eq.0.) qmin(i)=-10. ! impondo limites
      if(qgma.eq.0) qmax(i)=10.
 740  bdiag(i)=bsh/100.
      shu(i)=bsh/100.    ! shunt de barra
   1  continue
   2  nb=i-1
      ncb = nb
      ng=k-1
	
      return
      end    ! Fim da subrotina barra

c ----------------------------------------------------------------------
c Function para gerar numeracao interna a partir da externa

      function nin(k)   ! esta function e chamada na subrotina rede

      include 'globvar.inc'

      do 10 i=1,nb
      if(nex(i).eq.k)go to 20
   10 continue
      write(*,111)k

   20 nin=i      ! numeracao interna retornada
      return
  111 format(2x,'nao encontrada barra:',i5)
      end        ! Fim da function nin()
 

c -------------
c Formato PECO:
c -------------

c ----------------------------------------------------------------------
c Subrotina para leitura dos dados de linhas

      subroutine rede

      include 'globvar.inc'
c
c Valores que itipo() pode assumir:
* 0 - barra PQ
* 1 - barra PV
* 2 - barra "slack"
    
c      write(*,*)'  *rede*'
      do 5 k=1,nb
      gdiag(k)=0.
    5 continue

      do 1 i=1,10000
      read(066,10)lni,lnf,r,xx,ysh,ta,ta1,ta2,fnorm,femer
   10 format(i4,4x,i4,5x,2f6.2,f6.3,3f5.3,10x,2f4.0)
      if( lni .eq. 9999 ) goto 2   ! ysh = shunt de linha (modelo PI)
      ni(i)=nin(lni)    ! transforma numeracao externa em interna
      nf(i)=nin(lnf)
      r=r*fres/100.
      xx=xx*frea/100.
      xkl(i) = xx       ! reatancia serie do ramo i p/ contingencia
      ysh=ysh/100.

c	[ tratamento para linhas extremamente curtas ]
c Definido dessa forma para evitar problemas de convergencia
c
      if( ABS(xx) .lt. 5.D-4 ) then
	  k=ni(i)
	  m=nf(i)
          if( itipo(k) .gt. 0 )then
              v(m)=ves(k)
          elseif( itipo(m) .gt. 0 )then
	      v(k)=ves(m)
          endif
      endif
c
      aa=r*r+xx*xx

c == Elementos da Matriz Admitancia Nodal (completando abaixo)

      gkl(i)=r/aa       ! condutancia serie do ramo kl
      bkl(i)=-xx/aa     ! susceptancia serie do ramo kl

c == Elementos das Matrizes B' e B'' (completando abaixo)

      b1kl(i) = bkl(i)
      b2kl(i) = -1./xkl(i)


c -------------------------------------------------------
c Mudado aqui para rodar o IEEE118 e o IEEE57 - 27/02/95
c -------------------------------------------------------
      if( nb .eq. 118 .or. nb .eq. 57 ) then
        if( fnorm .eq. 0. ) then
          k=ni(i)
	  m=nf(i)
          fnorm = 0.1d0*(v(k)*v(m)/xx)*100.
        endif
      endif
      if( fnorm .eq. 0. ) fnorm=9999.  ! atribui um valor altissimo ao fluxo maximo se for lido zero

      fnorm=fnorm*ffnor

     
      fmi(i)=-fnorm/100.
      fma(i)=fnorm/100.
      ylin(i)=ysh/2.

c Warning: please to modify for taps control

      if( ta .eq. 1. ) ta=0.
      tap(i)=ta
      k=ni(i)
      l=nf(i)
      if(ta.eq.0.)then                     ! elementos das matrizes G e B
        gdiag(k)=gdiag(k)+gkl(i)
        gdiag(l)=gdiag(l)+gkl(i)
        bdiag(k)=bdiag(k)+bkl(i)+ysh/2.
        bdiag(l)=bdiag(l)+bkl(i)+ysh/2.
        shu(k)=shu(k)+ysh/2.               ! shunt de linha
        shu(l)=shu(l)+ysh/2.
      else
c **** tap fixo *****
        t1=1./ta
        bdiag(k)=bdiag(k)+bkl(i)*t1*t1  
        bdiag(l)=bdiag(l)+bkl(i)
        gdiag(k)=gdiag(k)+gkl(i)*t1*t1
        gdiag(l)=gdiag(l)+gkl(i)
        shu(k)=shu(k)+bkl(i)*t1*(t1-1.)/2. ! shunt de trafo
        shu(l)=shu(l)+bkl(i)*(1.-t1)/2. 
        gkl(i)=gkl(i)*t1
        bkl(i)=bkl(i)*t1 
cc        b2kl(i) = b2kl(i)*t1        ! elemento de B'' (comentado em 08/9/95)                     
      endif
    1 continue
    2 nl=i-1
      ncl=nl

      return
      end      ! Fim da subrotina rede



c ------------
c Formato CDF:
c ------------

c ----------------------------------------------------------------------
c Subrotina para leitura dos dados de barras

      subroutine bus

      include 'globvar.inc' 

      intrinsic ATAN

      pi = 4.d0*ATAN(1.d0)

c      write(*,*)' * bus * '

c
c Lendo dados de barra no FORMATO CDF:
c


c -------------------------------------------
c Convencao de Tipos de Barras (FORMATO CDF):
c
* 3  ---> SLACK
* 2  ---> PV
* 1  ---> Tensao Controlada
* 0  ---> PQ
c -------------------------------------------

      GRAU = pi/180.0
      ng = 0
      carga=0.


c
c Lendo o primeiro cartao de dados:
c
      
      read(066,5)bmva              ! Potencia-base de MVA
   5  format(31x,f6.2)
      read(066,*)                  ! BUS DATA FOLLOWS


      do 1 i=1,10000

      read(066,10)jex,s12,lfan,lzn,iti,vf,tf,pc,qc,pg,qg,
     &vb,vx,qgma,qgmi,gsh,bsh,nbrc
  10  format(i4,1x,a12,1x,i2,i3,1x,i2,1x,f6.4,f7.2,f9.2,f10.2,f8.2,f8.2,
     &1x,f7.2,1x,f6.4,3f8.4,f8.4,1x,i4)  
      
    
      if(jex .eq. -999) goto 2
       
      itipo(i)=0
      ves(i) = vf         ! importante para inicializar o problema
      tet(i) = tf*GRAU

      pg=pg*fpger
      pc=pc*fpca


c Atribui Valores Conforme Tipo de Barra:
      if( iti .eq. 1 .or. iti .eq. 2 ) itipo(i) = 1    ! 1 = barra PV (PECO)
      if( iti .eq. 3 ) itipo(i) = 2                    ! 2 = barra SLACK (PECO)
  
      if( iti .eq. 0 ) then 
          itipo(i) = 0                                 ! 0 = barra PQ (PECO)
          pes(i)=(pg-pc)/bmva 
          qes(i)=(qg-qc)/bmva
ccc          ves(i) = 1.000
      else
          ves(i) = vx
          qmin(i)=qgmi/bmva
          qmax(i)=qgma/bmva
          if( iti .ne. 3 ) then              
              itipo(i) = 1                             
              pes(i)=(pg-pc)/bmva
              qes(i)=0.0
          else
              tet(i) = tf*GRAU
          endif
          ng = ng + 1
      endif
    

      nex(i)=jex


c    [ flat start ]
      v(i)=1.000     ! importante para inicializar o problema
      tet(i)=0.0     ! veja esta importancia na subrotina potencia
      

      pca(i)=pc/bmva   ! entra em MW
      qca(i)=qc/bmva   ! entra em MVAr

      carga=pc+carga
      if( itipo(i) .eq. 0 ) carga=carga-pg
	
      bdiag(i)=bsh     ! entra em pu
      gdiag(i)=gsh     ! entra em pu
      shu(i)=bsh       ! shunt de barra diferente do formato PECO
   1  continue
   2  nb=i-1
      ncb = nb
	
      return
      end            ! Fim da subrotina bus



c ------------
c Formato CDF:
c ------------
 
c ----------------------------------------------------------------------
c Subrotina para leitura dos dados de linhas

      subroutine line

      include 'globvar.inc'
    
c      write(*,*)' * line *'

c
c Lendo dados de ramo no FORMATO CDF:
c

c -------------------------------------------
c Convencao de Tipos de Barras (FORMATO CDF):
c
* 3  ---> SLACK
* 2  ---> PV
* 1  ---> Tensao Controlada
* 0  ---> PQ
c -------------------------------------------

c
c Limites de Fluxos em Ramos: O CDF NAO tem esses dados
c


c
c Lendo o primeiro cartao de dados:
c

      read(066,*)                  ! BRANCH DATA FOLLOWS


      do 1 i = 1, 10000
      
      read(066,10)lni,lnf,lfa,lz,nc,ntc,r,xx,ysh,lrat1,
     &lrat2,lrat3,nbc,kside,ta,angletf,mintap,maxtap,step,vmin,vmax
   10 format(i4,1x,i4,2(1x,i2),i2,1x,i1,f10.6,f11.6,f10.5,3(f5.0,1x),
     &  i4,1x,i1,2x,f6.4,1x,3f7.4,f7.4,f8.4,f7.4)
c
 
c
c r e xx entram em pu
c
c ysh = Line charging B (in pu unit) (total line charging, +B)
c

      if( lni .eq. -999 ) goto 2
      ni(i)=nin(lni)                   ! transforma numeracao externa em interna
      nf(i)=nin(lnf)
      r=r*fres                         ! diferente do formato PECO
      xx=xx*frea                       ! diferente do formato PECO
      xkl(i) = xx                      ! reatancia serie do ramo i p/ contingencia
      fnorm = 0.0
   

c	[ tratamento para linhas extremamente curtas ]
c Definido dessa forma para evitar problemas de convergencia
c
      if( ABS(xx) .lt. 5.D-6 ) then
	  k=ni(i)
	  m=nf(i)
          if( itipo(k) .gt. 0 )then
	      v(m)=ves(k)
          elseif( itipo(m) .gt. 0 )then
	      v(k)=ves(m)
          endif
      endif
c
      aa=r*r+xx*xx


c == Elementos da Matriz Admitancia Nodal (completando abaixo)

      gkl(i)=r/aa       ! condutancia serie do ramo kl
      bkl(i)=-xx/aa     ! susceptancia serie do ramo kl

c == Elementos das Matrizes B' e B'' (completando abaixo)

      b1kl(i) = bkl(i)
      b2kl(i) = -1./xkl(i)

c --------------------------------------------
c Mudado aqui para rodar o IEEE300 - 19/07/95
c --------------------------------------------
      if( nb .eq. 300 ) then
        if( fnorm .eq. 0.0 ) then
          k=ni(i)
	  m=nf(i)
          fnorm = 0.1*(v(k)*v(m)/xx)*bmva
        endif
      endif
      if( fnorm .eq. 0.0 ) fnorm=9999.0  ! atribui um valor altissimo ao fluxo maximo se for lido zero


      fnorm=fnorm*ffnor

      fmi(i)=-fnorm/bmva
      fma(i)=fnorm/bmva

      ylin(i)=ysh/2.                     ! ysh entra em pu              

c Warning: please to modify for taps control

      if( ta .eq. 1.0 ) ta=0.0
      tap(i)=ta
      k=ni(i)
      l=nf(i)
c  ntc:
c  0 = transmission line
c  1 = fixed tap
c  2 = TCUL or LTC
c  3 = variable tap (turns ratio)
c  4 = phase shifter

      if( ta .eq. 0.0 ) then                 ! elementos das matrizes G e B
        gdiag(k)=gdiag(k)+gkl(i)
        gdiag(l)=gdiag(l)+gkl(i)
        bdiag(k)=bdiag(k)+bkl(i) + ysh/2.
        bdiag(l)=bdiag(l)+bkl(i) + ysh/2.
        shu(k)=shu(k)+ysh/2.                 ! shunt de linha  (ysh = shunt de linha do modelo PI)
        shu(l)=shu(l)+ysh/2.
      else
c **** tap fixo *****
        t1=1./ta
        bdiag(k)=bdiag(k)+bkl(i)*t1*t1  
        bdiag(l)=bdiag(l)+bkl(i)
        gdiag(k)=gdiag(k)+gkl(i)*t1*t1
        gdiag(l)=gdiag(l)+gkl(i)
        shu(k)=shu(k)+bkl(i)*t1*(t1-1.)/2.      ! shunt de trafo
        shu(l)=shu(l)+bkl(i)*(1.-t1)/2.  
        gkl(i)=gkl(i)*t1
        bkl(i)=bkl(i)*t1
cc        b2kl(i) = b2kl(i)*t1        ! elemento de B'' (comentado em 08/9/95)                    
      endif
    
    1 continue

    2 continue

c
c Lendo os tres ultimos cartoes de dados:
c

      read(066,15)                  ! LOSS ZONES FOLLOWS  -99  END OF DATA
   15 format(//)

      nl=i-1
      ncl=nl

      return
      end      ! Fim da subrotina line





c ----------------------------------------------------------------------
c Subrotina para leitura da lista de ramos contingenciados

      subroutine lista

      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
c
      dimension kk(0:mmax+1)      ! dimensionado para retirada simultanea de ate 10 ramos.
      
c   Read until end of file

      do 1 i = 1,10000
         read(066,10)(kk(j),j=0,mmax)
   10    format(11(i4,2x))
        
         j = 0
         do while ( (kk(j) .ne. 0 ) .and. ( kk(0) .ne. 9999) )
             kcont(i,j) = kk(j) ! lista de ramos -- kcont(i,0) = ordem da contigencia
             klasif(i) = i      ! lista c/ a ordem original de leitura
             j = j + 1
         enddo

         if( kk(0) .eq. 9999 ) goto 2
    1 continue     
    2 nrc = i - 1      ! no. total de ramos contingenciados
      nsobr = nrc
      nso = nsobr
      
      return
      end      ! Fim da subrotina lista
