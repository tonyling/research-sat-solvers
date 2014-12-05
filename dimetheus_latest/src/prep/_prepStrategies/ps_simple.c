/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategy_simple_printHelp(){
	printf("c      %-3d: SIMPLE:\n", PREP_STRATEGY_SIMPLE);
    printf("c           Behavior: Performs SE, PLE, DL-FLD, STR, ELS, UNH.\n");
}

void prep_strategy_simple_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (SIMPLE)...\n");
	#endif
	pre_resetModule();
}

void prep_strategy_simple_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (SIMPLE)...\n");
	#endif
	pre_initModule();
	if (pre_returnCode != PRE_UNKNOWN){
		prep_returnCode = PREP_ERROR;
		return;
	}
}

void prep_strategy_simple_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (SIMPLE)...\n");
	#endif
	pre_disposeModule();
}

void prep_strategy_simple_cycleA_init(){
	uint32_t i;
	clause *c;
	variable *v;
	literal *l;

	//We assume all the queues and stacks are flushed (empty), but we force it just to make sure.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();

	//CLAUSE PRIORITY QUEUE
	//Set the touch clause method to be the one used by subsumption elimination.
	cQ_touchClause = &cQ_touchClause_se;
	//We enqueue all enabled clauses in the formula.
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		cQ_touchClause(c);
	}
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		cQ_touchClause(c);
	}

	//CLAUSE STACK
	//Set the touch clause method for the stack to be the one for strengthening.
	cS_touchClause = &cS_touchClause_str;
	//Stack all the clauses still enabled.
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		cS_touchClause(c);
	}
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		cS_touchClause(c);
	}

	//LITERAL PRIORITY QUEUE
	//Set the touch literal method to be the one used by failed literal detection.
	lQ_touchLiteral = &lQ_touchLiteral_fld;
	//We now grab all variables that are still active and put their literals on the literal queue.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];		//The variable.
		l = main_litData + v->id;	//The corresponding literal, positive.
		lQ_touchLiteral(l);
		l = main_litData - v->id;	//The corresponding literal, negative.
		lQ_touchLiteral(l);
	}

	//LITERAL STACK
	lS_touchLiteral = &lS_touchLiteral_null;

	//VARIABLE QUEUE
	vQ_touchVariable = &vQ_touchVariable_null;

	//VARIABLE STACK
	//Set the touch variable method to be used by pure literal elimination.
	vS_touchVariable = &vS_touchVariable_pl;
	//Add all the variables for an initial check if they are pure.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		 vS_touchVariable(v);
	}
}

void prep_strategy_simple_cycleA_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	SE		(1)
		//cS (clause stack):			STR     (4)
		//lQ (literal priority queue):	FLD		(3)
		//lS (literal stack):			(none)
		//vQ (variable priority queue):	(none)
		//vS (variable stack):			PLE		(2)

		//We store the current situation in terms of enabled clause and variable count. Preprocessing will take place as long
		//as this changes. We also have an extra trigger, changes, that can be used to keep the preprocessing going.
		oldNumC = f.m_eo_used + f.m_el_used;
		oldNumV = f.n_vars_e_used;
		changes = 0;

		//We perform subsumption elimination.
		pre_extern_SE();
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after SE. SATISFIABLE.\n");
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

		//We perform failed literal detection.
		pre_extern_FLD();
		if (pre_returnCode == PRE_UNSAT){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing ran into conflicts on both sides for a variable in FLD. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			break;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after FLD. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		if (cQ_leaf > 1 || vS_leaf > 1){
			++changes;
			continue;
		}

		//We perform strengthening.
		pre_extern_STR();
		if (pre_returnCode == PRE_UNSAT) {
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing found a unit clause during STR but UP failed. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			break;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after STR. SATISFIABLE.\n");
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
		if (cQ_leaf > 1 || vS_leaf > 1 || lS_leaf > 1) {
			++changes;
			continue;
		}

		//In case we have now more candidates for SE, PLE, or FLD, we restart the preprocessing.
		if (cQ_leaf > 1 || vS_leaf > 1 || lQ_leaf > 1) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_simple_cycleB_init(){
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

	//LITERAL QUEUE
	lQ_touchLiteral = &lQ_touchLiteral_null;

	//LITERAL STACK
	lS_touchLiteral = &lS_touchLiteral_null;

	//VARIABLE QUEUE
	vQ_touchVariable = &vQ_touchVariable_null;

	//VARIABLE STACK
	vS_touchVariable = &vS_touchVariable_null;
}

void prep_strategy_simple_cycleB_exec(){
	int32_t oldNumV = 0, changes = 1;

	while (changes){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	(none)
		//cS (clause stack):			(none)
		//lQ (literal priority queue):	(none)
		//lS (literal stack):			(none)
		//vQ (variable priority queue):	(none)
		//vS (variable stack):			(none)

		//We store the current situation in terms of enabled clause and variable count. Preprocessing will take place as long
		//as this changes. We also have an extra trigger, changes, that can be used to keep the preprocessing going.
		oldNumV = f.n_vars_e_used;
		changes = 0;

		//We perform UNHIDING.
		pre_extern_UNHIDING(param_prepUNHRounds);
		if (pre_returnCode == PRE_UNSAT){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing ran into conflicts in UNHIDING. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			return;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after UNHIDING. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			return;
		}
		//We continue with a new round of unhiding as long as UNHDINING is able to remove additional variables.
		if (f.n_vars_e_used < oldNumV){
			++changes;
		}

	}
}

void prep_strategy_simple_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (SIMPLE)...\n");
	#endif

	int32_t numVars;

	do {
		numVars = f.n_vars_e_used;
		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle A.\n");
		#endif
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_simple_cycleA_init();
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_simple_cycleA_exec();
		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle A done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
		#endif

		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle B.\n");
		#endif
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_simple_cycleB_init();
		if (prep_returnCode == PREP_UNKNOWN) prep_strategy_simple_cycleB_exec();
		#ifdef VERBOSE_PREP
		printf("c   PREP: Cycle B done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
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
