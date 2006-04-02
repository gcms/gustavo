/*-------------------------------------inipvm.c-------------------------------------
-------------------------ARQUIVO DE INICIALIZACAO DE PROCESSOS PVM  ---------------- / 
/* PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM * PVM */

/*-----------------------------------------------
  Bibliotecas e  arquivos que devem ser incluidos
------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "vari.h"		  //arquivo contem as declaracoes de variaveis globais

#include "variaveispvm.h"  //variaveis do pvm = > necessario para passar os					  //parametros de volta ao processo chamador

#include "prototipos.h"	//arquivo de definicao dos prototipos das funcoes usadas

#include <pvm3.h>



/*----------------------------- inicio da rotina ----------------------------- */

void inicio(void) {
	int i;
	int k;
	int mytid, info, ntid, numt, bufid;

	struct pvmhostinfo *hostp;
	int nhost, narch, hi_tid;

	/*----------------------------------------------------------------
	 PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM PVM
	 ----------------------------------------------------------------*/

	/*----------------------------------------------
	 Envolve o programa no PVM e junta-o a um grupo
	 ---------------------------------------------*/

	// Define o tid de cada um
	mytid = pvm_mytid();
   
	if (mytid < 0) {
		printf(" Nao foi possivel obter o PID");
		pvm_halt();
	}

	// Define o processo PAI
	tids[0] = pvm_parent();

	if (tids[0] < 0) {
		tids[0] = mytid;
		myinst = 0;
	}
	// Obtenho informacoes sobre a configuracao da presente maquina virtual
	info = pvm_config(&nhost, &narch, &hostp);

	NPROC = nhost;	
	// No. total de processos = no. de maquinas
	if( tids[0] == mytid )			 // [EXCLUSIVO DO PROCESSO PAI]
	{
		if (NPROC > 1) {
			// Pai dispara processos para os filhos
			for (i = 1; i < NPROC; i++) {
				ntid = hostp[i].hi_tid;

				printf("%d\t%s\t%s\t%d\n", hostp[i].hi_tid, hostp[i].hi_name,
						hostp[i].hi_arch, hostp[i].hi_speed);
		
				if (mytid != ntid) {
				//-----------------------------------------------------------
					numt = pvm_spawn("contingencia", (char **) NULL,
							PvmTaskDefault, 0, NPROC - 1, &tids[i]);
				//--------------------------------------------------------
				}//fim if
			}//fim for

			for (i = 1; i < NPROC; i++) {
				bufid = pvm_initsend(PvmDataDefault);
				info = pvm_pkint(tids, NPROC, 1);
				info = pvm_send(tids[i], 1957);
			}
		 
			if (numt < NPROC - 1) {
				printf("Problema na DESOVA DOS PROCESSOS. Verifique o tids !");
				pvm_exit();
			}   //fim if

		}  //fim if
   }	//fim if
   else
   {
		bufid = pvm_recv(tids[0], 1957);  //msgtag = 1957
		info = pvm_upkint(tids, NPROC, 1);

		for (i = 0; i < NPROC; i++)
		{
			if (mytid == tids[i])
				myinst = i;
		}
	}
	return;
}
/*------------------------ Fim da rotina inicio ------------------------------- */
