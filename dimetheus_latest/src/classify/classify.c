/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CNFCLASSES_C_
#define CNFCLASSES_C_

#include "classify.h"

void classify_intern_simplify(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	uint32_t origVars = f.n_vars_e_used;
	#endif
	#ifdef VERBOSE_CLASSIFY
	printf("c     CLASSIFY: Performing simplifications.\n");fflush(stdout);
	#endif
	//We will now use the simple unit propagation to assign all the literals that can be enforced.
	clause *c;
	uint32_t i;
	int32_t litToEnforce;

	for (i=0; i < classify_numLitsToEnforceUsed; ++i){
		litToEnforce = classify_litsToEnforce[i];
		if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(litToEnforce)){
			#ifdef VERBOSE_CLASSIFY
			printf("c       CLASSIFY: Literal %d is in queue with opposite sign. Global conflict.\n", litToEnforce);
			#endif
			classify_returnCode = CLASSIFY_UNSAT;
			break;
		} else if (!IS_LIT_IN_SIMPLEUPQUEUE(litToEnforce)){
			ADD_LIT_TO_SIMPLEUPQUEUE(litToEnforce);
		}
	}

	c = main_simpleUP_propagate_final();
	if (c != NULL){
		#ifdef VERBOSE_CLASSIFY
		printf("c       CLASSIFY: The unit propagation ran into a conflict in clause %p.\n", (void*)c);
		#endif
		classify_returnCode = CLASSIFY_UNSAT;
	} else if (f.m_eo_used + f.m_el_used == 0){
		#ifdef VERBOSE_CLASSIFY
		printf("c       CLASSIFY: There are no further enabled clauses. We found a satisfying assignment.\n");
		#endif
		classify_returnCode = CLASSIFY_SAT;
	} else {
		#ifdef VERBOSE_CLASSIFY
		printf("c       CLASSIFY: CNF has %d variables and %d clauses remaining.\n", f.n_vars_e_used, f.m_eo_used);
		#endif
		f.n_afterPrep = f.n_vars_e_used;
	}

	#ifdef VERBOSE_CLASSIFY
	printf("c     CLASSIFY: Done with simplifications.\n");fflush(stdout);
	#endif
	#ifdef COLLECTSTATS
	stats_classify_assignedVars = origVars - f.n_vars_e_used;
	stats_classify_time_simplify += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void classify_intern_simplify_addLitToEnforce(int32_t theLit){
	classify_litsToEnforce[classify_numLitsToEnforceUsed++] = theLit;

	//Check if the amount of memory to store literals must be increased.
	if (classify_numLitsToEnforceUsed == classify_numLitsToEnforceAvail){
		//Yes, we do so right away.
		classify_numLitsToEnforceAvail += 10*BLOCKSIZE;
		classify_litsToEnforce = realloc(classify_litsToEnforce, sizeof(int32_t) * classify_numLitsToEnforceAvail);
	}
}

void classify_intern_voteForClass(uint32_t classID){
	if (classID > CNFCLASSES_CLASS_MINNUM && classID < CNFCLASSES_CLASS_MAXNUM){
		classify_classVotes[classID - CNFCLASSES_CLASS_MINNUM]++;
		if (classID > CNFCLASSES_RES_RAN_MINVALUE && classID < CNFCLASSES_RES_RAN_MAXVALUE) {
			++classify_domainVotes_random;
		} else if (classID > CNFCLASSES_RES_CRA_MINVALUE && classID < CNFCLASSES_RES_CRA_MAXVALUE){
			++classify_domainVotes_crafted;
		} else if (classID > CNFCLASSES_RES_APP_MINVALUE && classID < CNFCLASSES_RES_APP_MAXVALUE){
			++classify_domainVotes_application;
		}
	}
}

void classify_intern_setPrecision(){
	//This method sets the floating point precision to a fixed number of after-the-comma values. This is done in order to align
	//the precision to the precision used during the learning of the random forest.
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Setting precision of all attributes to %d float digits.\n", param_classifyFloatPrec);fflush(stdout);
	#endif
	float_ty scaleFactor = pow(10.0, param_classifyFloatPrec);
	MAIN_FATT_SETPREC(scaleFactor);
}

void classify_intern_determineClass(){
	//This method looks at all the class values and determines which class has the highest count.
	uint32_t i;
	uint32_t maxVote = 0, secondMaxVote = 0, theClass = 0, totalVote = 0;
	float_ty certainty;

	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Determining class.\n");fflush(stdout);
	#endif

	for (i=0; i < classify_numClasses; ++i){
		totalVote += classify_classVotes[i];
		#ifdef VERBOSE_CLASSIFY
		if (classify_classVotes[i] > 0){
			printf("c   CLASSIFY:   Class %d (", i+CNFCLASSES_CLASS_MINNUM);
			tools_cnfClasses_printClassName(i+CNFCLASSES_CLASS_MINNUM);
			printf(") has votes: %d.\n", classify_classVotes[i]);
		}
		#endif
		if (classify_classVotes[i] > maxVote){
			secondMaxVote = maxVote;
			maxVote = classify_classVotes[i];
			theClass = i;
		} else if (classify_classVotes[i] > secondMaxVote){
			secondMaxVote = classify_classVotes[i];
		}
	}
	if (maxVote == 0) {
		//No votes were distributed.
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY:   No class received a vote. Setting the value to UNRECOGNIZED.\n");fflush(stdout);
		#endif
		MAIN_SET_FATT_CLASS(CNFCLASSES_RES_SPE_UNRECOGNIZED);
		MAIN_SET_FATT_CERTAINTY(ZERO);
		MAIN_SET_FATT_CERTAINTYDIST(ZERO);
	} else {
		//The class with the maximum number of votes has been determined. We now set the formula class.
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY:     Class votes (total, best, second): %d, %d, %d\n", totalVote, maxVote, secondMaxVote);
		#endif
		theClass += CNFCLASSES_CLASS_MINNUM;
		MAIN_SET_FATT_CLASS(theClass);
		certainty = ((float_ty)maxVote) / ((float_ty)totalVote);
		MAIN_SET_FATT_CERTAINTY(certainty);
		certainty = ((float_ty)(maxVote - secondMaxVote)) / ((float_ty)maxVote);
		MAIN_SET_FATT_CERTAINTYDIST(certainty);
	}
}

void classify_extern_classify(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_classify_component_totalCalls;
	#endif

	if (param_classifyStrategy != CLASSIFY_STRATEGY_NULL){
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: CLASSIFICATION...\n");fflush(stdout);
		#endif

		classify_strategy_reset();
		classify_strategy_init();
		if (classify_returnCode == CLASSIFY_UNKNOWN){
			classify_strategy_execute();
		}
		if (classify_returnCode == CLASSIFY_UNKNOWN){
			classify_intern_determineClass();
		}
		classify_strategy_dispose();
		#ifdef VERBOSE_CLASSIFY
		printf("c   CLASSIFY: Done with classification.\n");fflush(stdout);
		#endif
	}

	if (classify_returnCode == CLASSIFY_UNKNOWN){
		#ifdef VERBOSE_CLASSIFY_ATTVALUE
		printf("c   CLASSIFY: FORMULA-ATTRIBUTES: ");
		if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_GENERIC) {
			MAIN_PRINT_FATT_GENERIC(); printf("\n");
		} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_RANDOM){
			MAIN_PRINT_FATT_RANDOM(); printf("\n");
		} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_CRAFTED){
			MAIN_PRINT_FATT_CRAFTED(); printf("\n");
		} else if (param_classifyInputDomain == CLASSIFY_INPUTDOMAIN_APPLICATION){
			MAIN_PRINT_FATT_APPLICATION(); printf("\n");
		} else {
			printf("UNKNOWN DOMAIN\n");
		}
		#endif
		#ifdef VERBOSE_CLASSIFY_ATTCLASS
		printf("c   CLASSIFY: DOMAIN-VOTES: (RAN, CRA, APP): %d, %d, %d\n",
				classify_domainVotes_random, classify_domainVotes_crafted,classify_domainVotes_application);
		printf("c   CLASSIFY: FORMULA-CLASSID: %d (", MAIN_GET_FATT_CLASS());
		tools_cnfClasses_printClassName(MAIN_GET_FATT_CLASS());
		printf(")\n");
		printf("c   CLASSIFY: FORMULA-CLASS-CERTAINTY: %f %f\n", MAIN_GET_FATT_CERTAINTY(), MAIN_GET_FATT_CERTAINTYDIST());
		#endif
		if (param_classifySimplify == 1){
			classify_intern_simplify();
		}
	}

	#ifdef COLLECTSTATS
	stats_classify_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void classify_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_classify_component_totalCalls;
	#endif
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: RESET...\n");
	#endif
	//Reset the classification.
	classify_returnCode = CLASSIFY_UNKNOWN;
	classify_strategies_pointer_reset();

	classify_numLitsToEnforceUsed = 0;
	classify_numLitsToEnforceAvail = 0;
	classify_litsToEnforce = NULL;

	classify_domainVotes_random = 0;
	classify_domainVotes_crafted = 0;
	classify_domainVotes_application = 0;

	classify_numClasses = 0;
	classify_classVotes = NULL;
	#ifdef COLLECTSTATS
	stats_classify_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void classify_initPhase(){
	//Initialize the classification.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_classify_component_totalCalls;
	#endif

	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: INITIALIZE...\n");
	#endif
	uint32_t i;
	classify_returnCode = CLASSIFY_UNKNOWN;
	classify_strategies_pointer_init();

	classify_numLitsToEnforceUsed = 0;
	classify_numLitsToEnforceAvail = BLOCKSIZE;
	classify_litsToEnforce = malloc(sizeof(int32_t) * classify_numLitsToEnforceAvail);
	if (classify_litsToEnforce == NULL){
		printf("c ERROR. Classify was unable to allocate the array to enforce literals. Out of memory?\n");
		classify_returnCode = CLASSIFY_ERROR;
		return;
	}

	classify_numClasses = CNFCLASSES_CLASS_MAXNUM - CNFCLASSES_CLASS_MINNUM;
	classify_classVotes = malloc(sizeof(uint32_t) * classify_numClasses);
	if (classify_classVotes == NULL){
		printf("c ERROR. Classify was unable to allocate the array to vote for classes. Out of memory?\n");
		classify_returnCode = CLASSIFY_ERROR;
		return;
	}
	for (i = 0; i < classify_numClasses; ++i){
		classify_classVotes[i] = 0;
	}

	classify_domainVotes_random = 0;
	classify_domainVotes_crafted = 0;
	classify_domainVotes_application = 0;

	#ifdef COLLECTSTATS
	stats_classify_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void classify_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_classify_component_totalCalls;
	#endif
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: DISPOSE...\n");
	#endif
	//For freeing all classification related memory.
	classify_returnCode = CLASSIFY_UNKNOWN;
	classify_strategies_pointer_dispose();

	classify_numLitsToEnforceUsed = 0;
	classify_numLitsToEnforceAvail = 0;
	if (classify_litsToEnforce != NULL){
		free(classify_litsToEnforce);
		classify_litsToEnforce = NULL;
	}

	classify_numClasses = 0;
	if (classify_classVotes != NULL){
		free(classify_classVotes);
		classify_classVotes = NULL;
	}
	#ifdef COLLECTSTATS
	stats_classify_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

#endif /* CNFCLASSES_C_ */
