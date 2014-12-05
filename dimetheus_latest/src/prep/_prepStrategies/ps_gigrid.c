/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategy_gigrid_printHelp(){
	printf("c      %-3d: GIGRID:\n", PREP_STRATEGY_GIGRID);
    printf("c           Behavior: Performs PLE, SLFLD, NIVER, ELS.\n");
}

void prep_strategy_gigrid_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (GIGRID)...\n");
	#endif
	pre_resetModule();
}

void prep_strategy_gigrid_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (GIGRID)...\n");
	#endif
	pre_initModule();
	if (pre_returnCode != PRE_UNKNOWN){
		prep_returnCode = PREP_ERROR;
		return;
	}
}

void prep_strategy_gigrid_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (GIGRID)...\n");
	#endif
	pre_disposeModule();
}

void prep_strategy_gigrid_cycleA_init(){
	int32_t i;
	variable *v;

	//We assume all the queues and stacks are flushed (empty), but we force it just to make sure.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();

	//CLAUSE PRIORITY QUEUE
	cQ_touchClause = &cQ_touchClause_null;

	//CLAUSE STACK
	cS_touchClause = &cS_touchClause_null;

	//LITERAL PRIORITY QUEUE
	lQ_touchLiteral = &lQ_touchLiteral_null;

	//LITERAL STACK.
	lS_touchLiteral = &lS_touchLiteral_null;

	//VARIABLE PRIORITY QUEUE
	//Set the touch variable method to be the one used by non increasing variable elimination by resolution.
	vQ_touchVariable = &vQ_touchVariable_niver;
	//We now grab all the variables that are still active and put them on the variable queue.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];		//The variable.
		vQ_touchVariable(v);
	}

	//VARIABLE STACK
	//Set the touch variable method to be used by pure literal elimination.
	vS_touchVariable = &vS_touchVariable_pl;
	//Add all the variables for an initial check if they are pure.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		 vS_touchVariable(v);
	}
}

void prep_strategy_gigrid_cycleA_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;
	clause* c;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	none
		//cS (clause stack):			none
		//lQ (literal priority queue):	SLFLD
		//lS (literal stack):			none
		//vQ (variable priority queue):	NIVER
		//vS (variable stack):			PLE

		//We store the current situation in terms of enabled clause and variable count. Preprocessing will take place as long
		//as this changes. We also have an extra trigger, changes, that can be used to keep the preprocessing going.
		oldNumC = f.m_eo_used + f.m_el_used;
		oldNumV = f.n_vars_e_used;
		changes = 0;

		//First of all, we will propagate everything we have found so far.
		c = main_simpleUP_propagate_prep();
		if (c != NULL){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing ran into a conflict in a clause during UP. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			break;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after UP. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		//We perform pure literal elimination.
		pre_extern_PLE();
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after PLE. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		//We perform ELS.
		pre_extern_ELS();
		if (pre_returnCode == PRE_UNSAT){
			prep_returnCode = PREP_UNSAT;
			return;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after ELS. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			return;
		}
		if (lQ_leaf > 1 || vS_leaf > 1){
			++changes;
			continue;
		}

		//We perform non increasing variable elimination by resolution.
		pre_extern_NIVER();
		if (pre_returnCode == PRE_UNSAT){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing found a unit clause during NIVER but UP failed. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			break;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after NIVER. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		if (lQ_leaf > 1 || vS_leaf > 1) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_gigrid_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (GIGRID)...\n");
	#endif

	int32_t numVars;

	do {
		numVars = f.n_vars_e_used;
		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle A.\n");
		#endif
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_gigrid_cycleA_init();
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_gigrid_cycleA_exec();
		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle A done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
		#endif
	} while (numVars > f.n_vars_e_used);

	//We make sure that all queues and stacks are empty after preprocessing.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();
}
