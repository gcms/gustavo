c23456781234567812345678123456781234567812345678123456781234567812345678
c   1       2       3       4       5       6       7       8       9
c ----------------------------------------------------------------------
c
c ----------------------------------------------------------------------
c Subrotina para calcular os fluxos de potencia nos ramos do sistema
c  <<< ATIVO >>>   <<< REATIVO >>>  <<< ATIVO >>>   <<< REATIVO >>>

      subroutine fluxos    ! chamada pelo programa principal
c
      
      include 'globvar.inc'
      include 'varcont.inc'

      intrinsic sin,cos
   
      do 30 jj=1,nl
        k=ni(jj)
        m=nf(jj)
        tkm=t(k)-t(m)
        g=gkl(jj)
        b=bkl(jj)
        co=cos(tkm)
        si=sin(tkm)
        vk=v(k)
        vkk=vk*vk
        vm=v(m)
        vmm=vm*vm 
        ta=tap(jj)
        ysh=ylin(jj)
        if(ta .eq. 0.0) then
          t1=1.0
          goto 90
        endif
        t1=1.0/ta
  90    bb=b+ysh        

        fpkm(jj)=    t1*g*vkk - vk*vm*(g*co+b*si)    ! fluxo ativo
        rqkm(jj)=  -t1*bb*vkk - vk*vm*(g*si-b*co)    ! fluxo reativo
        fpmk(jj)=  (g/t1)*vmm - vk*vm*(g*co-b*si)     
        rqmk(jj)=-(bb/t1)*vmm - vk*vm*(-g*si-b*co)

        fpjk(jj) = fpkm(jj)

  30  continue

      return
      end         ! Fim da subrotina de calculo de fluxos ativos e reativos

c ----------------------------------------------------------------------

c Subrotina para calcular fluxos de potencia ativa ramos do sistema
c   <<< ATIVO >>>    <<< ATIVO >>>   <<< ATIVO >>>   <<< ATIVO >>>

c     [  Somente para analise de contingencias  ]

      subroutine fluxativo     
c
      include 'globvar.inc'
      include 'varcont.inc'
      intrinsic sin,cos
   
      
      do 30 jj=1,nl
        k=ni(jj)
        m=nf(jj)
        tkm=t(k)-t(m)
        g=gkl(jj)
        b=bkl(jj)
        co=cos(tkm)
        si=sin(tkm)
        vk=v(k)
        vkk=vk*vk
        vm=v(m)
        vmm=vm*vm 
        ta=tap(jj)
        if(ta .eq. 0.0) then
          t1=1.0
          goto 90
        endif
        t1=1.0/ta
  90    continue        

        fpjk(jj)=    t1*g*vkk - vk*vm*(g*co+b*si)    ! fluxo ativo
cc        fpkj(jj)=  (g/t1)*vmm - vk*vm*(g*co-b*si) 

cc        print*,'Em fluxativo() --> barra ',k,' a barra ',m,' ramo ',jj
cc        print*,'Fluxo CB --> fpjk(',jj,') = ',fpjk(jj)

  30  continue

      return
      end         ! Fim da subrotina de calculo de fluxos ativos


