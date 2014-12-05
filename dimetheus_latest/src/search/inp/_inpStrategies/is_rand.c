/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inpStrategies.h"

void inp_strategy_rand_printHelp(){
	printf("c      %-3d: RAND\n", INP_STRATEGY_RAND);
    printf("c           Behavior: Reduces the clause database, and performs PLE, FLD, and SE.\n");
}

void inp_strategy_rand_reset(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy reset (RAND)...\n");
	#endif
	in_resetModule();
}

void inp_strategy_rand_init(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy init (RAND)...\n");
	#endif
	in_initModule();
	if (in_returnCode != IN_UNKNOWN){
		inp_returnCode = INP_ERROR;
		return;
	}
}

void inp_strategy_rand_dispose(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy dispose (RAND)...\n");
	#endif
	in_disposeModule();
}

void inp_strategy_rand_execute(){
	#ifdef VERBOSE_INP
	int32_t initNumV = f.n_vars_e_used;
	int32_t initNumC = f.m_el_used+ f.m_eo_used;
	printf("c     INP:   In-Processing [RAND]: Starting to in-process...\n");
	printf("c     INP:   In-Processing [RAND]:   Remaining variables: %d\n", initNumV);
	printf("c     INP:   In-Processing [RAND]:   Remaining clauses:   %d\n", initNumC);
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
	int32_t i;
	variable *v;
	literal *lPos, *lNeg;

	#ifdef VERBOSE_IN
	printf("c       IN: ");
	#endif

	//PREPARE FOR PLE
	vS_touchVariable = &vS_touchVariable_inp_pl;
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		lPos = main_litData + GET_VAR_ID(v);
		lNeg = main_litData - GET_VAR_ID(v);
		if (GET_LIT_NUMOCCS_USED(lPos) == 0 || GET_LIT_NUMOCCS_USED(lNeg) == 0){
			vS_touchVariable(v);
		}
	}

	//PREPARE FOR FLD
	lQ_touchLiteral = &lQ_touchLiteral_inp_fld;
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		lPos = main_litData + GET_VAR_ID(v);
		lNeg = main_litData - GET_VAR_ID(v);
		if (GET_LIT_NUMIMP_USED(lPos) > 0){
			lQ_touchLiteral(lPos);
		}
		if (GET_LIT_NUMIMP_USED(lNeg) > 0){
			lQ_touchLiteral(lNeg);
		}
	}

	//PREPARE FOR SE
	cQ_touchClause = &cQ_touchClause_inp_se;
	for (i = 0; i < f.m_el_used; ++i){
		cQ_touchClause(f.clauses_el[i]);
	}
	for (i = 0; i < f.m_eo_used; ++i){
		cQ_touchClause(f.clauses_eo[i]);
	}

	do {
		in_extern_PLE();
		if (in_returnCode == IN_SAT){
			#ifdef VERBOSE_IN
			printf("SAT.\n");
			#endif
			inp_returnCode = INP_SAT;
			return;
		}
		if (in_returnCode == IN_UNSAT){
			#ifdef VERBOSE_IN
			printf("UNSAT.\n");
			#endif
			inp_returnCode = INP_UNSAT;
			return;
		}

		in_extern_FLD();
		if (in_returnCode == IN_SAT){
			#ifdef VERBOSE_IN
			printf("SAT.\n");
			#endif
			inp_returnCode = INP_SAT;
			return;
		}
		if (in_returnCode == IN_UNSAT){
			#ifdef VERBOSE_IN
			printf("UNSAT.\n");
			#endif
			inp_returnCode = INP_UNSAT;
			return;
		}

		in_extern_SE();
		if (in_returnCode == IN_SAT){
			#ifdef VERBOSE_IN
			printf("SAT.\n");
			#endif
			inp_returnCode = INP_SAT;
			return;
		}
		if (in_returnCode == IN_UNSAT){
			#ifdef VERBOSE_IN
			printf("UNSAT.\n");
			#endif
			inp_returnCode = INP_UNSAT;
			return;
		}
	} while (vS_leaf > 0 || lQ_leaf > 0 || cQ_leaf > 0);

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
	#ifdef VERBOSE_IN
	printf("\n");
	#endif
	#ifdef VERBOSE_INP
	printf("c     INP:   In-Processing [RAND]:   Removed variables: %d (%d remaining)\n",
			initNumV - f.n_vars_e_used, f.n_vars_e_used);
	printf("c     INP:   In-Processing [RAND]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(f.m_el_used+ f.m_eo_used), f.m_el_used + f.m_eo_used);
	#endif
}
