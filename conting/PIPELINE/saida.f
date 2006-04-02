c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para emitir relatorio do estado do sistema << resultado 1 >>

      subroutine relat1   ! chamada pelo programa principal
c
      include 'globvar.inc'
      include 'varcont.inc'
c
      intrinsic ATAN
c     
      real*8 pi
      pi = 4.0*ATAN(1.0)

      write(050,*)	
cc      write(050,*)'                                            Arquivo
cc     &  relat0.sai'
      write(050,*)	
      write(050,110)
  110 format(35x,' E S T A D O    D O    S I S T E M A ',/)
      write(050,120)nb,nl
  120 format(39x,i5,' BARRAS  / ',i6,' RAMOS',/) 
      write(050,171)
  171 format('--------------------+------------------+------------------
     &-+-----------------+--------------------')
      write(050,101)
  101 format(4x,'BARRAMENTO',6x,'|',6x,'TENSAO',6x,'|',6x,'GERACAO',6x,
     &'|',6x,'CARGA',6x,'|',3x,'INJECAO LIQUIDA')
      write(050,171)
      write(050,102)
  102 format(2x,'No. EXT.',2x,'TIPO',4x,'|',2x,'MODULO',2x,'ANGULO',2x,
     &'|',4x,'[MW]',3x,'[MVAr]',2x,'|',3x,'[MW]',3x,'[MVAr]',1x,'|',3x,
     &'[MW]',5x,'[MVAr]')
      write(050,171)

      do 10 k=1,nb
        atet=tet(k)*(180.0/pi)    ! aqui
        pc=pca(k)*100.0
        qc=qca(k)*100.0
        pl=p(k)*100.0
        ql=q(k)*100.0
        pg=pl+pc
        qg=ql+qc
 
        write(050,103)nex(k),itipo(k),v(k),atet,pg,qg,pc,qc,pl,ql
  103   format(1x,i5,5x,i3,8x,f6.3,2x,f6.2,4x,f8.2,1x,f8.2,2x,f8.2,
     &1x,f8.2,1x,f8.2,3x,f8.2)

   10 continue
      if(ncaso .ne. 0 ) mcont = kcont(ncaso,0)     ! pega o valor de mcont
      write(050,172)
  172 format('----------------------------------------------------------
     &----------------------------------------')

      write(050,393)tol,nitea,niter      
      write(050,394)carga      
  393 format(15x,'TOLERANCIA (CASO BASE) =',f9.5,2x,'IT.ATIVAS =',i3,1x,
     &'IT.REATIVAS =',i3)   
  394 format(15x,'CARGA TOTAL DO SISTEMA [MW]=',f10.02)
      write(050,172)
 
      if(icont .ne. 0) then
        write(050,206)nb,ng,nl-mcont  ! mcont = no. de contingencias p/ caso
        
        if( mcont .eq. 1 ) then
          write(050,207)ncaso,fcont   ! ncaso = kk do loop1 do programa principal
  207     format(15x,'CONTINGENCIA SIMPLES: CASO ',i4,'  FATOR DE CONTIN
     &GENCIAS = ',f14.7)
        else
          write(050,208)ncaso,fcont   ! ncaso = kk do loop1 do programa principal
  208     format(15x,'CONTINGENCIA MULTIPLA: CASO ',i4,' FATOR DE CONTIN
     &GENCIAS = ',f14.7)
        endif

      else
        write(050,206)nb,ng,nl
      endif
  206 format(15x,'No DE BARRAS:',i4,8x,'No DE GERADORES:',i4,8x,'No DE R
     &AMOS:',i4)
           
c      write(050,206)
      write(050,172)
      
      return
      end       ! Fim da subrotina relat1


c ----------------------------------------------------------------------
c Subrotina para emitir relatorio de fluxos do sistema << resultado 2 >>

      subroutine relat2   ! chamada pelo programa principal

      include 'globvar.inc'
      include 'varcont.inc'
c
      write(052,*)'                                 Arquivo  relat2.sai'
      write(052,60)
 60   format('  ---------------------------------+----------------------
     &------------+-------------------')
      write(052,61)
 61   format('    BARRAS           FLUXOS        |   BARRAS           FL
     &UXOS        |       PERDAS      ')
      write(052,60)
      write(052,62)
 62   format('  INI.  FIM     P(MW)      Q(MVAr) |  INI.  FIM    P(MW)  
     &    Q(MVAr) |   P(MW)    Q(MVAr)')
      write(052,60)

c                 'bbxxxxbbxxxxbbxxxxx.xxxbbxxxxx.xxx'|'xxxxbbxxxxbbxxxx
cx.xxxbbxxxxx.xxx'|'xxxx.xxxbxxxx.xxx '      
      do i = 1,nl
          k=ni(i)            ! numeracao interna
          m=nf(i)
          k1 = nex(k)        ! numeracao externa
          m1 = nex(m)
          pp = (fpkm(i) + fpmk(i))*100.0
          pq = (rqkm(i) + rqmk(i))*100.0
          write(052,220)k1,m1,fpkm(i)*100.0,rqkm(i)*100.0,m1,k1,
     &fpmk(i)*100.0,rqmk(i)*100.0,pp,pq
  220      format(i4,2x,i4,2x,f9.3,2x,f9.3,2x,' |',i4,2x,i4,1x,
     &f9.3,2x,f9.3,'   |',f8.3,1x,f8.3)
      enddo
      
      mcont = kcont(ncaso,0)  ! pega o valor de mcont
      write(052,171)
      if(icont .ne. 0) then
        write(052,206)nb,ng,nl-mcont  ! mcont = no. de contingencias p/ caso
        
        if( mcont .eq. 1 ) then
          write(052,207)ncaso    ! ncaso = kk do loop1 do programa principal
  207     format(9x,'CONTINGENCIA SIMPLES: CASO ',i4)
        else
          write(052,208)ncaso    ! ncaso = kk do loop1 do programa principal
  208     format(9x,'CONTINGENCIA MULTIPLA: CASO ',i4 )
        endif

      else
        write(052,206)nb,ng,nl
      endif
  206 format(9x,'No DE BARRAS:',i4,8x,'No DE GERADORES:',i4,8x,'No DE LI
     &NHAS:',i4)
           
c      write(052,206)
      write(052,171)
  171 format('  ---------------------------------------------------------
     &--------------------------------')

      return
      end       ! Fim da subrotina relat2 


c ----------------------------------------------------------------------
      subroutine relat3(kselect)

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'




c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat31.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat32.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat33.sai')

         goto 100  
     

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat34.sai')

         goto 100  


c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat35.sai')

         goto 100  

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat36.sai')

         goto 100  

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat37.sai')

         goto 100  

c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat38.sai')

         goto 100  


c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat39.sai')

         goto 100  


c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=053,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat310.sai')


c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados           


             
 
      write(053,9)
      write(053,11)
  9   format(///,15x,'** RANKING  PRELIMINAR  DE  SEVERIDADE **')
 11   format(15x,'**    DE CONTINGENCIAS DC INCREMENTAL  **',/)
       	
      write(053,12)
 12   format(2x,'-------------------------------------------------------
     &---------------')
      write(053,19)
      write(053,26)
      write(053,12)
 19   format(2x,' Ordem de ',3x,'  Numero do ',3x,' Numero do ',4x,' Ind
     &ice  de',2x,'Quem Calculou?')
 26   format(2x,'Severidade',3x,'    Caso    ',3x,'   Ramo    ',4x,' Sev
     &eridade',2x,'     (TID)    ')

      do 31 ii = 1,nsobr
        ll = klasif(ii)         ! classificado < ordenado >
        mcont = kcont(ll,0)     ! pega o valor de mcont
      do 41 jj=1,mcont
        ll1 = kcont(ll,jj)

        if( jj .ne. mcont) write(053,36)ll1
        if( jj .eq. mcont) write(053,37)ii,ll,ll1,pia(ii),idp(ll)   ! mudei aqui em 4/7
 36     format(9x,9x,5x,9x,i5,10x,'       ')
 37     format(4x,i5,9x,i5,9x,i5,6x,f12.5,2x,i12)
 41   continue
      write(053,12)
 31   continue
      
      write(053,17)nsobr,ncrit
 17   format(15x,'LISTA INICIAL = ',i4,' --> LISTA REDUZIDA = ',i4)
      write(053,12)

      close(053)
      return
      end                ! Fim de relat3

c ----------------------------------------------------------------------
      subroutine relat4(kselect)

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'


c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat41.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat42.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat43.sai')

         goto 100 


c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat44.sai')

         goto 100  

 
c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat45.sai')

         goto 100  

 

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat46.sai')

         goto 100  

 

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat47.sai')

         goto 100  

 
c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat48.sai')

         goto 100  

 

c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat49.sai')

         goto 100  



c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=054,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat410.sai')



c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados           



            

      write(054,9)
  9   format(///,15x,'  ** ILHAMENTOS **',/)
 
       	
      write(054,12)
 12   format(2x,'-----------------------------------------------')
      write(054,19)
      write(054,26)
      write(054,12)
 19   format(8x,'  Numero do ',10x,' Numero do ')
 26   format(8x,'    Caso    ',10x,'   Ramo    ')

      do 31 ii = 1,nilha
        ll=kilha(ii)         ! lista de ilhamentos
        mcont = kcont(ll,0)  ! pega o valor de mcont
      do 41 jj=1,mcont
        ll1 = kcont(ll,jj)

        if( jj .ne. mcont) write(054,36)ll1
        if( jj .eq. mcont) write(054,37)ll,ll1
 36     format(32x,i5)
 37     format(11x,i5,16x,i5)

 41   continue
      write(054,12)
 31   continue
      
      write(054,18)nilha
 18   format(5x,'QUANT. DE ILHAMENTO(S) OCORRIDO(S): ',i5)
      
      write(054,12)

      close(054)
      return
      end                ! Fim de relat4


c ----------------------------------------------------------------------
      subroutine relat5(kselect)

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'
            


c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat51.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat52.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat53.sai')

         goto 100

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat54.sai')

         goto 100


c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat55.sai')

         goto 100



c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat56.sai')

         goto 100


c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat57.sai')

         goto 100



c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat58.sai')

         goto 100


c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat59.sai')

         goto 100


c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=055,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat510.sai')


c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados           


      write(055,9)
  9   format(///,16x,' ** DIVERGENCIAS **',/)
 
       	
      write(055,12)
 12   format(2x,'-------------------------------------------------')
      write(055,19)
      write(055,26)
      write(055,12)
 19   format(3x,'  Numero do ',8x,' Numero do ',3x,'Quem Calculou?')
 26   format(3x,'    Caso    ',8x,'   Ramo    ',3x,'     (TID)    ')
      do 31 ii = 1,ndiv
        ll = kdiverge(ii)      ! lista de divergencias
        mcont = kcont(ll,0)    ! pega o valor de mcont  
      do 41 jj = 1,mcont
        ll1 = kcont(ll,jj)

        if( jj .ne. mcont) write(055,36)ll1
        if( jj .eq. mcont) write(055,37)ll,ll1,idt(ll)
 36     format(25x,i5)
 37     format(6x,i5,14x,i5,5x,i12)

 41   continue
      write(055,12)
 31   continue
      
      write(055,18)ndiv
 18   format(8x,'QUANT. DE DIVERGENCIA(S) OCORRIDA(S): ',i5)
      
      write(055,12)

      close(055)
      return
      end                ! Fim de relat5


c ----------------------------------------------------------------------
      subroutine relat6(kselect)

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'
            




c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat61.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat62.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat63.sai')

         goto 100  

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat64.sai')

         goto 100  

c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat65.sai')

         goto 100  

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat66.sai')

         goto 100  

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat67.sai')

         goto 100  

c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat68.sai')

         goto 100  

c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat69.sai')

         goto 100  

c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=056,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat610.sai')
  

c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados           

  

      write(056,11)
 11   format(///,14x,'** NAO CONVERGENCIAS **',/)
 
       	
      write(056,12)
 12   format(2x,'-------------------------------------------------')
      write(056,14)
      write(056,16)
      write(056,12)
 14   format(3x,'  Numero do ',8x,' Numero do ',3x,'Quem Calculou?')
 16   format(3x,'    Caso    ',8x,'   Ramo    ',3x,'     (TID)    ')

      do 31 ii = 1,nnconv
        ll=knotconv(ii)      ! lista de nao convergencias
        mcont = kcont(ll,0)  ! pega o valor de mcont  
      do 41 jj=1,mcont
        ll1 = kcont(ll,jj)

        if( jj .ne. mcont) write(056,36)ll1
        if( jj .eq. mcont) write(056,37)ll,ll1,idt(ll)
 36     format(25x,i5)
 37     format(6x,i5,14x,i5,5x,i12)
       
 41   continue
      write(056,12)
 31   continue
      
      write(056,18)nnconv
      write(056,21)2*(limite+1)
      write(056,22)toler   
 18   format(8x,'QUANT. DE NAO CONVERGENCIA(S): ',i5,/)
 21   format(8x,'N0. ESPECIFICADO DE ITERACOES: ',i2,/)
 22   format(8x,'TOLERANCIA ESPECIFICADA: ',f12.8)
      
      write(056,12)

      close(056)
      return
      end                ! Fim de relat6

c ----------------------------------------------------------------------
      subroutine relat7(kselect)

      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'



c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat71.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat72.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat73.sai')

         goto 100  

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat74.sai')

         goto 100  

c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat75.sai')

         goto 100  

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat76.sai')

         goto 100

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat77.sai')

         goto 100

c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat78.sai')

         goto 100

c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat79.sai')

         goto 100

c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=057,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat710.sai')

  
c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados           



      write(057,9)
      write(057,11)
  9   format(///,22x,'** RESULTADO DA SIMULACAO AC **')
 11   format(20x,'**   [ CASOS QUE CONVERGIRAM ]   **',/)
       	
      write(057,12)
 12   format(2x,'-------------------------------------------------------
     &-------------------')
      write(057,24)
      write(057,26)
      write(057,12)
c ----------------------------------------------------------------------
 24   format(2x,'Numero do ',3x,' Numero do ',3x,' Indice de',2x,'  No. 
     &de Iteracoes',2x,' Quem Calculou?')
 26   format(2x,'  Caso    ',3x,'   Ramo    ',3x,' Severidade',2x,' Ativ
     &as  Reativas',2x,'      (TID)    ')

      do 31 ii = 1,nyconv
        ll = kconv(ii)         ! lista de casos que convergiram
        mcont = kcont(ll,0)    ! pega o valor de mcont
        it1 = kit1(ii)
        it2 = kit2(ii)
      do 41 jj=1,mcont
        ll1 = kcont(ll,jj)

        write(057,37)ll,ll1,piaux(ll),it1,it2,idt(ll)     ! mudei aqui em 04/7
 37     format(2x,i5,9x,i5,8x,f9.5,7x,i2,6x,i2,5x,i12)
 41   continue
      write(057,12)
 31   continue
      
      write(057,18)nyconv
      write(057,21)2*(limite+1)
      write(057,22)toler   
 18   format(16x,'QUANT. DE CONVERGENCIA(S) OCORRIDA(S): ',i5,/)
 21   format(16x,'N0. ESPECIFICADO DE ITERACOES: ',i2,/)
 22   format(16x,'TOLERANCIA ESPECIFICADA: ',f12.8)
      write(057,12)

      close(057)
      return
      end                ! Fim de relat7


c ----------------------------------------------------------------------
      subroutine relat8(nodes,kselect)
c
      include 'globvar.inc'
      include 'varcont.inc' 
      include 'varsel.inc'
      include 'tempos.inc'   
c
      double precision  t0,t1,t2,t3,t4,t5,t6,t7,ttotal




c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat81.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat82.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat83.sai')

         goto 100  

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat84.sai')

         goto 100  

c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat85.sai')

         goto 100  

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat86.sai')

         goto 100  

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat87.sai')

         goto 100  

c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat88.sai')

         goto 100  

c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat89.sai')

         goto 100

c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------
c
         open(unit=058,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat810.sai')


c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados      



      t0 = time(0)             ! entrada de dados
      t1 = time(3) + time(10)  ! relatorios de saida < nao incluidos aqui relat1 e relat2: ver outage.f >
c
c relat0.sai; relat1.sai e relat2.sai sao arquivos usados somente na fase de testes.
c
      t2 = time(2)             ! time(2) = calculo do estado do caso base
      t3 = time(5)             ! time(5) = obtencao da tabela de caminhos    
      t4 = time(4) + time(6)   ! time(6) = calc. DC Incr. - time(4) = fluxos ativos caso base
      t5 = time(7)             ! time(7) = ordencao e red. da lista
      t6 = time(8)             ! time(8) = calc. de contingencias AC
      t7 = time(1) + time(9)   ! time(1) = broadcast dos dados - time(9) = receb. de resultados
      
      ttotal = t0+t1+t2+t3+t4+t5+t6+t7  ! tempo soma-total
      
      if( modelo .eq. 0 ) then       
        write(058,7)
  7     format(///,25x,'** PROGRAMA PARALELO SINCRONO **')
      else
        write(058,8)
  8     format(///,24x,'** PROGRAMA PARALELO ASSINCRONO **')
      endif

      write(058,9)nodes
  9   format(28x,'No. DE WORKSTATIONS  = ',i2)

      write(058,11)
      write(058,12)
 11   format(/,18x,'** TEMPOS EM [s]  MEDIDOS EM DIVERSOS PONTOS **')
 12   format(18x,'**  DO PROGRAMA DE ANALISE DE CONTINGENCIAS  **',/)
c ----------------------------------------------------------------------
                                                  
      write(058,59)
 59   format(' -------------+------------+---------------+--------------
     &----------+-----------+-------------')
      write(058,61)
 61   format('       I/O    |FCAC C. BASE|TABELA/CAMINHOS|       "SCREEN
     &ING"      |CONTING. AC| COMUNICACAO ')
      write(058,59)
      write(058,62)
 62   format(' DADOS |RESUL.| [calculo]  | DE FATORACAO  |DC INCREMENTAL
     &|ORDENACAO|[simulacao]| DADOS&RESUL.')
      write(058,59)
              
      write(058,14)t0,t1,t2,t3,t4,t5,t6,t7
 14   format(f6.1,1x,f6.1,1x,f8.1,6x,f8.1,9x,f8.1,4x,f8.1,4x,f8.1,3x,
     &f8.1)

      write(058,63)
 63   format(' ---------------------------------------------------------
     &------------------------------------')

      if ( nrc .gt. 0 ) then
        write(058,16)t4/float(nrc)
 16     format(12x,'TEMPO MEDIO P/ CONTINGENCIA DC [s]:  ',f8.1)
      endif

      if ( ncrit .gt. 0 ) then
        write(058,18)t6/float(ncrit)
 18     format(12x,'TEMPO MEDIO P/ CONTINGENCIA AC [s]:  ',f8.1)
      endif
      
      write(058,19)ttotal
 19   format(12x,'TEMPO TOTAL-INCLUINDO I/O & COM.-[s]:  ',f8.1)

      write(058,63)
     
      write(058,22)nb,ng,nl 
 22   format(12x,'No DE BARRAS:',i4,8x,'No DE GERADORES:',i4,8x,'No DE L
     &INHAS:',i4,/)
   
      write(058,63)
     
      write(058,26)nsobr,ncrit
      write(058,28)nilha
      write(058,31)ndiv
      write(058,32)nnconv
      write(058,34)nyconv
 26   format(12x,'LISTA INICIAL = ',i4,' --> LISTA REDUZIDA = ',i4,/)
 28   format(12x,'QUANT. DE ILHAMENTO(S): ',i5,/)
 31   format(12x,'QUANT. DE DIVERGENCIA(S): ',i5,/)
 32   format(12x,'QUANT. DE NAO CONVERGENCIA(S): ',i5,/)
 34   format(12x,'QUANT. DE CONVERGENCIA(S): ',i5,/)
     
      write(058,63)

      close(058)
      return
      end                ! Fim de relat8

c
c ----------------------------------------------------------------------
c Subrotina para escolha de alternativas < menu >
c
c  [ This subroutine loops until you type "Y":]
c
      subroutine escolha(impr)
c   
      integer*4 impr
      character*1 choose
      
      write(*,*)' Voce Deseja a Saida do Estado de Alguma Contingencia?[
     &s/n]'
   10 read(*,'(A)')choose
      if(( choose .eq. 'S' ) .or. ( choose .eq. 's' ))  then
        print*,' Entre com No. do CASO CONTINGENCIADO (Estado/Fluxos):'
        read(*,*)impr
        return
      elseif(( choose .eq. 'N') .or. ( choose .eq. 'n')) then
        return
      else
        goto 10
      endif
      end          ! Fim da subrotina escolha


c
c ----------------------------------------------------------------------
c Subrotina p/ receber estado/fluxos pos-contingencia e chamar relat. saidas   
      
      subroutine salida(impr,kselect)

      include '/opt/pvm3/include/fpvm3.h'

      include 'globvar.inc'
      include 'varcont.inc'

c -----------------------------------------------
c  PVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVMPVM
c -----------------------------------------------

      
cc      msgtype0 = 11
cc      call PVMFRECV( -1, msgtype0, info )
cc      call PVMFUNPACK( INTEGER4, icont, 1, 1, info )
cc      call PVMFUNPACK( INTEGER4, kdiag, 1, 1, info )
cc      call PVMFUNPACK( INTEGER4, nitea, 1, 1, info )
cc      call PVMFUNPACK( INTEGER4, niter, 1, 1, info )

cc      call PVMFUNPACK( REAL8, vsend, nb, 1, info )
cc      call PVMFUNPACK( REAL8, tsend, nb, 1, info )

cc      call PVMFUNPACK( REAL8, p, nb, 1, info )
cc      call PVMFUNPACK( REAL8, q, nb, 1, info ) 
cc      call PVMFUNPACK( REAL8, fpkm, nl, 1, info )
cc      call PVMFUNPACK( REAL8, fpmk, nl, 1, info )
cc      call PVMFUNPACK( REAL8, rqkm, nl, 1, info )
cc      call PVMFUNPACK( REAL8, rqmk, nl, 1, info )

cc      do iii = 1,nb   
cc         v(iii) = vsend(iii)  
cc         t(iii) = tsend(iii)
cc      enddo



c Saida da analise de contingencia  < estado/fluxos -- opcional >

      if(( impr .eq. icont ) .and. ( kdiag .ne. -1 )) then


c === Seleciona os resultados da analise de contingencias:


      goto ( 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 ) kselect
c ----------------------------------------------------------------------
    5 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat11.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat21.sai')

         goto 100  
     
c ----------------------------------------------------------------------
   10 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat12.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat22.sai')

         goto 100

c ----------------------------------------------------------------------
   15 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat13.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat23.sai')

         goto 100  

c ----------------------------------------------------------------------
   20 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat14.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat24.sai')

         goto 100  

c ----------------------------------------------------------------------
   25 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat15.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat25.sai')

         goto 100  

c ----------------------------------------------------------------------
   30 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat16.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat26.sai')

         goto 100

c ----------------------------------------------------------------------
   35 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat17.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat27.sai')

         goto 100

c ----------------------------------------------------------------------
   40 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat18.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat28.sai')

         goto 100

c ----------------------------------------------------------------------
   45 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat19.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat29.sai')

         goto 100

c ----------------------------------------------------------------------
   50 continue
c ----------------------------------------------------------------------

         open(unit=050,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat110.sai')
         open(unit=052,file=
     &'/home/piava/baleeiro/pvm3/bin/SUN4SOL2/relat210.sai')


c ----------------------------------------------------------------------
  100 continue          ! ponto comum a todos os resultados      


  
          ncaso = impr       ! no. do caso
c ----------------------------------------------------------------------
          write(050,101)
  101     format('                                                Arquiv
     &o  relat1.sai')
          
          call relat1        ! estado
        
          call relat2        ! fluxos

          close(050)
          close(052)
      endif
    
      return
      end     ! Fim da subrotina salida

  

c
c ----------------------------------------------------------------------
c Subrotina para chamadas de relatorios de saida
c
      subroutine output(kselect)
c
      include 'globvar.inc'
      include 'varcont.inc'
      include 'varsel.inc'
      include 'tempos.inc'


c Gera arquivo com a lista reduzida em funcao da severidade da contingencia

      if( nilha .ne. nrc ) call relat3(kselect) ! lista ordenada < reduzida >

c Gera arquivo com a lista de ilhamentos detectados

      if( nilha .ne. 0 ) call relat4(kselect)   ! lista de ilhamentos

c Gera arquivo com a lista de divergencias ocorridas

      if( ndiv .ne. 0 ) call relat5(kselect)    ! lista de divergencias

c Gera arquivo com a lista de nao convergencias ocorridas

      if( nnconv .ne. 0 ) call relat6(kselect)  ! lista de nao convergencias

c Gera arquivo com a lista de convergencias ocorridas

      if( nyconv .ne. 0 ) call relat7(kselect)  ! lista de convergencias


      return
      end                ! Fim de output
