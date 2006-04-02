NCC    = gcc
FPO = contingencia
FPI = fpi
FFLAGS = -O -c

#Modificações dia 13/06-------------------------------------------
SDIR = 		  ..
INCLUDE = 	-I/usr/local/include
LIBS    = 	/usr/local/lib/libpvm3.a
#-----------------------------------------------------------------



# Arquivos include

DEPEND = global.h vari.h variaveispvm.h prototipos.h 

#OBJ =     contingencia.o entrada.o prepara-dados.o monta_matriz.o\
#          fcarga.o estado.o zollen.o fluxos.o saida.o\


OBJ =     contingencia.o entrada.o prepara-dados.o zollen.o \
	  monta_matriz.o fcarga.o estado.o saida.o  \
          ident.o  injecao.o bennett.o modif.o produto.o \
          analise.o svector.o fluxos.o inversa.o \
          pqdir.o mpqdir.o select.o outage.o inipvm.o \
          mestre.o escravo.o state.o  \

# Opcoes do Compilador 


.SUFFIXES: .c .o

.c.o:; $(NCC) $(FFLAGS) $< ${INCLUDE}
#.c.o:; $(NCC) $(FLAGS) ${INCLUDE}
all:	${SDIR}/$(FPO)

$(SDIR)/$(FPO): $(OBJ) $(EXTRASM) Makefile
	$(NCC)  $(OBJ)  ${INCLUDE} ${LIBS} -o $(SDIR)/$(FPO) -lm
