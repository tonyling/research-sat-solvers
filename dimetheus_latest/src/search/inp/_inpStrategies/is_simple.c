/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inpStrategies.h"

void inp_strategy_simple_printHelp(){
	printf("c      %-3d: SIMPLE\n", INP_STRATEGY_SIMPLE);
    printf("c           Behavior: Reduces the clause database, and performs TER, UNH, TRL, ELS, and SE.\n");
}

void inp_strategy_simple_reset(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy reset (SIMPLE)...\n");
	#endif
	in_resetModule();
}

void inp_strategy_simple_init(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy init (SIMPLE)...\n");
	printf("c     INP:       param_inpTernaryLitMax = %d\n", param_inpTernaryLitMax);
	printf("c     INP:       param_inpTernaryMax    = %d\n", param_inpTernaryMax);
	printf("c     INP:       param_inpUNHRounds     = %d\n", param_inpUNHRounds);
	#endif
	in_initModule();
	if (in_returnCode != IN_UNKNOWN){
		inp_returnCode = INP_ERROR;
		return;
	}
}

void inp_strategy_simple_dispose(){
	#ifdef VERBOSE_INP
	printf("c     INP: Strategy dispose (SIMPLE)...\n");
	#endif
	in_disposeModule();
}

void inp_strategy_simple_execute(){
	#ifdef VERBOSE_INP
	int32_t initNumV = f.n_vars_e_used;
	int32_t initNumC = f.m_el_used+ f.m_eo_used;
	printf("c     INP:   In-Processing [SIMPLE]: Starting to in-process...\n");
	printf("c     INP:   In-Processing [SIMPLE]:   Remaining variables: %d\n", initNumV);
	printf("c     INP:   In-Processing [SIMPLE]:   Remaining clauses:   %d\n", initNumC);
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
	uint32_t currentNumVars, unhSuccess;

	#ifdef VERBOSE_IN
	printf("c       IN: ");
	#endif

	//TERNARY
	vS_touchVariable = &vS_touchVariable_inp_ternary;
	for (i = 0; i < f.n_vars_e_used; ++i){
		vS_touchVariable(f.vars_e[i]);
	}
	in_extern_TERNARY();
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
	vS_touchVariable = &vS_touchVariable_null;

	do {
		//UNHIDING
		currentNumVars = f.n_vars_e_used;
		in_extern_UNHIDING(param_inpUNHRounds);
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
		unhSuccess = 0;
		if (currentNumVars > f.n_vars_e_used){
			unhSuccess = 1;
		}

		//TREELOOK
		in_extern_TREELOOK();
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

		//ELS
		in_extern_ELS();
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

		#ifdef VERBOSE_IN
		printf("\nc       IN: ");
		#endif
	} while (unhSuccess);

	//SE
	cQ_touchClause = &cQ_touchClause_inp_se;
	for (i = 0; i < f.m_el_used; ++i){
		cQ_touchClause(f.clauses_el[i]);
	}
	for (i = 0; i < f.m_eo_used; ++i){
		cQ_touchClause(f.clauses_eo[i]);
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
	cQ_touchClause = &cQ_touchClause_null;

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
	printf("c     INP:   In-Processing [SIMPLE]:   Removed variables: %d (%d remaining)\n",
			initNumV - f.n_vars_e_used, f.n_vars_e_used);
	printf("c     INP:   In-Processing [SIMPLE]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(f.m_el_used+ f.m_eo_used), f.m_el_used + f.m_eo_used);
	#endif
}
