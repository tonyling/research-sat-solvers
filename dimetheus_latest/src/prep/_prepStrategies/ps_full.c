/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "prepStrategies.h"

void prep_strategy_full_printHelp(){
	printf("c      %-3d: FULL:\n", PREP_STRATEGY_FULL);
    printf("c           Behavior: Full preprocessing, ELS, NHBR, UNH, SE, STR, PLE, DL-FLD, NIVER, GE, ACCE.\n");
}

void prep_strategy_full_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (FULL)...\n");
	#endif
	pre_resetModule();
}

void prep_strategy_full_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (FULL)...\n");
	#endif
	pre_initModule();
	if (pre_returnCode != PRE_UNKNOWN){
		prep_returnCode = PREP_ERROR;
	}
}

void prep_strategy_full_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (FULL)...\n");
	#endif
	pre_disposeModule();
}

void prep_strategy_full_cycleA_init(){
	int32_t i;
	uint32_t random, numToPermute;
	clause *c;
	literal *l, *lPos, *lNeg;
	variable *v;

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

	//LITERAL STACK.
	//We perform ternary resolution with the literal stack.
	lS_touchLiteral = &lS_touchLiteral_ternary;
	numToPermute = f.n_vars_e_used;
	perm_LFSR_init(numToPermute);
	for (i = 0; i < perm_lfsr_perLen; ++i){
		random = perm_LFSR_nextState();
		if (random >= numToPermute) continue;
		v = f.vars_e[random];				//The variable.
		lPos = main_litData + v->id;	//The corresponding literal, positive.
		lNeg = main_litData - v->id;	//The corresponding literal, negative.
		if (GET_LIT_NUMOCCS_USED(lPos) > 0 && GET_LIT_NUMOCCS_USED(lNeg) > 0){
			if (GET_LIT_NUMOCCS_USED(lPos) > GET_LIT_NUMOCCS_USED(lNeg)){
				lS_touchLiteral(lPos);
			} else {
				lS_touchLiteral(lNeg);
			}
		}
	}

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

void prep_strategy_full_cycleA_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;
	clause* c;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	SE		(1)
		//cS (clause stack):			STR		(5)
		//lQ (literal priority queue):	FLD		(4)
		//lS (literal stack):			TERNARY (3)
		//vQ (variable priority queue):	NIVER	(6)
		//vS (variable stack):			PLE		(2)

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

		//We perform the learning of ternary resolvents.
		pre_extern_TERNARY();

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

		//In case we have now more candidates for subsumption or or pure literal elimination, we restart the preprocessing.
		if (cQ_leaf > 1 || vS_leaf > 1 || lS_leaf > 1) {
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

		//In case we have now more candidates for SE, PLE, or FLD, we restart the preprocessing.
		if (cQ_leaf > 1 || vS_leaf > 1 || lQ_leaf > 1) {
			++changes;
			continue;
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

		if (cQ_leaf > 1 || vS_leaf > 1 || lQ_leaf > 1) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_full_cycleB_init(){
	int32_t i;
	uint32_t random, numToPermute;
	variable *v;
	literal *lPos, *lNeg;
	clause *c;

	//We assume all the queues and stacks are flushed (empty), but we force it just to make sure.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();

	//CLAUSE PRIORITY QUEUE
	//We do SE.
	cQ_touchClause = &cQ_touchClause_se;
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		cQ_touchClause(c);
	}
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		cQ_touchClause(c);
	}

	//CLAUSE STACK
	//We do STR.
	cS_touchClause = &cS_touchClause_str;
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		cS_touchClause(c);
	}
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		cS_touchClause(c);
	}

	//LITERAL PRIORITY QUEUE
	//We do FLD.
	lQ_touchLiteral = &lQ_touchLiteral_fld;

	//LITERAL STACK
	//We perform ternary resolution with the literal stack.
	lS_touchLiteral = &lS_touchLiteral_ternary;
	numToPermute = f.n_vars_e_used;
	perm_LFSR_init(numToPermute);
	for (i = 0; i < perm_lfsr_perLen; ++i){
		random = perm_LFSR_nextState();
		if (random >= numToPermute) continue;
		v = f.vars_e[random];				//The variable.
		lPos = main_litData + v->id;	//The corresponding literal, positive.
		lNeg = main_litData - v->id;	//The corresponding literal, negative.
		if (GET_LIT_NUMOCCS_USED(lPos) > 0 && GET_LIT_NUMOCCS_USED(lNeg) > 0){
			if (GET_LIT_NUMOCCS_USED(lPos) > GET_LIT_NUMOCCS_USED(lNeg)){
				lS_touchLiteral(lPos);
			} else {
				lS_touchLiteral(lNeg);
			}
		}
	}

	//VARIABLE PRIORITY QUEUE
	//We do GE.
	vQ_touchVariable = &vQ_touchVariable_ge;
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];		//The variable.
		vQ_touchVariable(v);
	}

	//VARIABLE STACK
	//We do nothing.
	vS_touchVariable = &vS_touchVariable_null;

}

void prep_strategy_full_cycleB_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;
	clause *c;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	SE		(2)
		//cS (clause stack):			STR		(3)
		//lQ (literal priority queue):	FLD		(5)
		//lS (literal stack):			TERNARY (4)
		//vQ (variable priority queue):	GE		(1)
		//vS (variable stack):			(none)

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

		//We perform gate extraction.
		pre_extern_GE();
		if (pre_returnCode == PRE_UNSAT){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing found a unit clause during GE but UP failed. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			break;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after GE. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		//We perform subsumption elimination.
		pre_extern_SE();
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after SE. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		if (vQ_leaf > 1) {
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

		if (vQ_leaf > 1 || cQ_leaf > 1) {
			++changes;
			continue;
		}

		//We do ternary resolution.
		pre_extern_TERNARY();

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

		if (vQ_leaf > 1 || cQ_leaf > 1 || cS_leaf > 1 || lS_leaf > 1) {
			++changes;
			continue;
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
		if (vQ_leaf > 1 || cQ_leaf > 1 || cS_leaf > 1 || lS_leaf > 1) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_full_cycleC_init(){
	//We assume all the queues and stacks are flushed (empty), but we force it just to make sure.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();

	int32_t i;
	uint32_t numToPermute, random;
	variable *v;
	literal *l, *lPos, *lNeg;

	//CLAUSE PRIORITY QUEUE
	cQ_touchClause = &cQ_touchClause_se;
	//We do not fill this queue. TERNARY and NHBR, AND ELS do that for us.

	//CLAUSE STACK
	cS_touchClause = &cS_touchClause_null;

	//LITERAL QUEUE
	//Set the touch literal method to be the one used by failed literal detection.
	lQ_touchLiteral = &lQ_touchLiteral_nhbr;
	//We now grab all variables that are still active and put their literals on the literal queue.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];		//The variable.
		l = main_litData + v->id;	//The corresponding literal, positive.
		lQ_touchLiteral(l);
		l = main_litData - v->id;	//The corresponding literal, negative.
		lQ_touchLiteral(l);
	}

	//LITERAL STACK
	//We perform ternary resolution with the literal stack.
	lS_touchLiteral = &lS_touchLiteral_ternary;
	numToPermute = f.n_vars_e_used;
	perm_LFSR_init(numToPermute);
	for (i = 0; i < perm_lfsr_perLen; ++i){
		random = perm_LFSR_nextState();
		if (random >= numToPermute) continue;
		v = f.vars_e[random];				//The variable.
		lPos = main_litData + v->id;	//The corresponding literal, positive.
		lNeg = main_litData - v->id;	//The corresponding literal, negative.
		if (GET_LIT_NUMOCCS_USED(lPos) > 0 && GET_LIT_NUMOCCS_USED(lNeg) > 0){
			if (GET_LIT_NUMOCCS_USED(lPos) > GET_LIT_NUMOCCS_USED(lNeg)){
				lS_touchLiteral(lPos);
			} else {
				lS_touchLiteral(lNeg);
			}
		}
	}

	//VARIABLE QUEUE
	vQ_touchVariable = &vQ_touchVariable_null;

	//VARIABLE STACK
	vS_touchVariable = &vS_touchVariable_null;
}

void prep_strategy_full_cycleC_exec(){
	uint32_t changes = 1, oldNumV;

	while (changes){
		changes = 0;
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	SE
		//cS (clause stack):			(none)
		//lQ (literal priority queue):	NHBR
		//lS (literal stack):			TERNARY
		//vQ (variable priority queue):	(none)
		//vS (variable stack):			(none)

		//We perform subsumption elimination.
		pre_extern_SE();
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after SE. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			break;
		}

		//We perform ternary resolution.
		pre_extern_TERNARY();
		if (cQ_leaf > 1) {
			++changes;
			continue;
		}

		//We perform non-transitive hyper binary resolution.
		pre_extern_NHBR();
		if (pre_returnCode == PRE_UNSAT){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing ran into conflicts in NHBR. UNSATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_UNSAT;
			return;
		}
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after NHBR. SATISFIABLE.\n");
			#endif
			prep_returnCode = PREP_SAT;
			return;
		}

		oldNumV = f.n_vars_e_used;

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

		if (oldNumV > f.n_vars_e_used) {
			++changes;
			continue;
		}
	}
}

void prep_strategy_full_cycleD_init(){
	int32_t i;
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
	//We do nothing.
	cQ_touchClause = &cQ_touchClause_null;

	//CLAUSE STACK
	//We do nothing.
	cS_touchClause = &cS_touchClause_null;

	//LITERAL PRIORITY QUEUE
	//We do ACCE.
	lQ_touchLiteral = &lQ_touchLiteral_acce;
	//Add all the literals that are still active to the queue.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];		//The variable.
		l = main_litData + v->id;	//The corresponding literal, positive.
		lQ_touchLiteral(l);
		l = main_litData - v->id;	//The corresponding literal, negative.
		lQ_touchLiteral(l);
	}

	//LITERAL STACK
	//We do nothing.
	lS_touchLiteral = &lS_touchLiteral_null;

	//VARIABLE PRIORITY QUEUE
	//We do NIVER.
	vQ_touchVariable = &vQ_touchVariable_niver;

	//VARIABLE STACK
	//Set the touch variable method to be used by pure literal elimination.
	vS_touchVariable = &vS_touchVariable_pl;
	//Add all the variables for an initial check if they are pure.
	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		 vS_touchVariable(v);
	}

}

void prep_strategy_full_cycleD_exec(){
	int32_t oldNumC = 0, oldNumV = 0, changes = 0;
	clause *c;

	while (changes || (oldNumC != f.m_eo_used + f.m_el_used) || (oldNumV != f.n_vars_e_used)){
		//Currently, the following queues and stacks are used for these operations:
		//cQ (clause priority queue):	(none)
		//cS (clause stack):			(none)
		//lQ (literal priority queue):	ACCE (1)
		//lS (literal stack):			(none)
		//vQ (variable priority queue):	NIVER (3)
		//vS (variable stack):			PLE (2)

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

		//We perform asymmetric covered clause elimination.
		pre_extern_ACCE();
		if (f.m_eo_used + f.m_el_used == 0 || f.n_vars_e_used == 0){
			#ifdef VERBOSE_PREP
			printf("c   PREP: Preprocessing removed all clauses or variables after ACCE. SATISFIABLE.\n");
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

		if (lQ_leaf > 1){
			changes = 1;
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

		if (lQ_leaf > 1 || vS_leaf > 1){
			changes = 1;
			continue;
		}
	}
}

void prep_strategy_full_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (FULL)...\n");
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle A.\n");
	#endif
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleA_init();
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleA_exec();
	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle A done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle B.\n");
	#endif
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleB_init();
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleB_exec();
	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle B done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle C.\n");
	#endif
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleC_init();
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleC_exec();
	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle C done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
	#endif

	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle D.\n");
	#endif
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleD_init();
	if (prep_returnCode == PREP_UNKNOWN) prep_strategy_full_cycleD_exec();
	#ifdef VERBOSE_PREP
	printf("c   PREP: Cycle D done. Variables %u, Clauses %u.\n", f.n_vars_e_used, f.m_el_used + f.m_eo_used);
	#endif

	//We make sure that all queues and stacks are empty after preprocessing.
	cQ_flush();
	cS_flush();
	lQ_flush();
	lS_flush();
	vQ_flush();
	vS_flush();
}
