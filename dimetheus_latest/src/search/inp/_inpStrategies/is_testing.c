/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "inpStrategies.h"

void inp_strategy_testing_printHelp(){
	printf("c      %-3d: TESTING\n", INP_STRATEGY_TESTING);
    printf("c           Behavior: Subject to changes in the source-code. Do not use.\n");
}

void inp_strategy_testing_reset(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy reset (TESTING)...\n");
	#endif
}

void inp_strategy_testing_init(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy init (TESTING)...\n");
	#endif
}

void inp_strategy_testing_dispose(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy dispose (TESTING)...\n");
	#endif
}

void inp_strategy_testing_execute(){
	#ifdef VERBOSE_INP
	int32_t initNumV = f.n_vars_e_used;
	int32_t initNumC = f.m_el_used+ f.m_eo_used;
	printf("c     INP:   In-Processing [TESTING]: Starting to in-process...\n");
	printf("c     INP:   In-Processing [TESTING]:   Remaining variables: %d\n", initNumV);
	printf("c     INP:   In-Processing [TESTING]:   Remaining clauses:   %d\n", initNumC);
	#endif

	//Does nothing at the moment.

	#ifdef VERBOSE_INP
	printf("c     INP:   In-Processing [TESTING]:   Removed variables: %d (%d remaining)\n",
			initNumV - f.n_vars_e_used, f.n_vars_e_used);
	printf("c     INP:   In-Processing [TESTING]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(f.m_el_used+ f.m_eo_used), f.m_el_used+ f.m_eo_used);
	#endif
}
