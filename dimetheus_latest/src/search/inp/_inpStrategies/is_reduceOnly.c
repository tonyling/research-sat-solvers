/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inpStrategies.h"

void inp_strategy_reduceOnly_printHelp(){
	printf("c      %-3d: REDUCEONLY\n", INP_STRATEGY_REDUCEONLY);
    printf("c           Behavior: Only reduces the clause database using new DL0 assignments.\n");
}

void inp_strategy_reduceOnly_reset(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy reset (REDUCEONLY)...\n");
	#endif
}

void inp_strategy_reduceOnly_init(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy init (REDUCEONLY)...\n");
	#endif
}

void inp_strategy_reduceOnly_dispose(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy dispose (REDUCEONLY)...\n");
	#endif
}

void inp_strategy_reduceOnly_execute(){
	#ifdef VERBOSE_INP
	int32_t initNumV = f.n_vars_e_used;
	int32_t initNumC = f.m_el_used+ f.m_eo_used;
	printf("c     INP:   In-Processing [REDUCEONLY]: Starting to in-process...\n");
	printf("c     INP:   In-Processing [REDUCEONLY]:   Remaining variables: %d\n", initNumV);
	printf("c     INP:   In-Processing [REDUCEONLY]:   Remaining clauses:   %d\n", initNumC);
	#endif
	cQ_flush();
	cQ_touchClause = &cQ_touchClause_null;
	cS_flush();
	cS_touchClause = &cS_touchClause_null;
	cQ_flush();
	lQ_touchLiteral = &lQ_touchLiteral_null;
	lS_flush();
	lS_touchLiteral = &lS_touchLiteral_null;
	vS_flush();
	vS_touchVariable = &vS_touchVariable_null;
	vQ_flush();
	vQ_touchVariable = &vQ_touchVariable_null;
	inp_intern_reduceDB();
	#ifdef VERBOSE_INP
	printf("c     INP:   In-Processing [REDUCEONLY]:   Removed variables: %d (%d remaining)\n",
			initNumV - f.n_vars_e_used, f.n_vars_e_used);
	printf("c     INP:   In-Processing [REDUCEONLY]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(f.m_el_used+ f.m_eo_used), f.m_el_used + f.m_eo_used);
	#endif

}

