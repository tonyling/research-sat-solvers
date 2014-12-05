/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategy_rand_printHelp(){
	printf("c      %-3d: RAND:\n", PREP_STRATEGY_RAND);
    printf("c           Behavior: Performs actions sometimes helpful on random instances SE, PLE, DL-FLD\n");
}

void prep_strategy_rand_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (RAND)...\n");
	#endif
	pre_resetModule();
}

void prep_strategy_rand_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (RAND)...\n");
	#endif
	pre_initModule();
	if (pre_returnCode != PRE_UNKNOWN){
		prep_returnCode = PREP_ERROR;
		return;
	}
}

void prep_strategy_rand_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (RAND)...\n");
	#endif
	pre_disposeModule();
}

void prep_strategy_rand_cycleA_init(){
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
	cS_touchClause = &cS_touchClause_null;

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

void prep_strategy_rand_cycleA_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	SE		(1)
		//cS (clause stack):			(none)
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

		//In case we have now more candidates for SE, PLE, or FLD, we restart the preprocessing.
		if (cQ_leaf > 1 || vS_leaf > 1 || lQ_leaf > 1) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_rand_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (RAND)...\n");
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle A.\n");
	#endif
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_rand_cycleA_init();
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_rand_cycleA_exec();
	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle A done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
	#endif


	//We make sure that all queues and stacks are empty after preprocessing.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();
}
