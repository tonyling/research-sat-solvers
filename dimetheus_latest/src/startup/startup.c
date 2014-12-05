/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "startup.h"

void startup_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_startup_component_totalCalls;
	#endif
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP: RESET...\n");
	#endif
	startup_returnCode = STARTUP_UNKNOWN;

	startup_cnfClauses = NULL;
	startup_cnfClausesNumAvail = 0;
	startup_cnfClausesNumUsed = 0;

	#ifdef COLLECTSTATS
	stats_startup_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void startup_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_startup_component_totalCalls;
	#endif
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP: INITIALIZE...\n");
	#endif
	startup_returnCode = STARTUP_UNKNOWN;

	startup_cnfClausesNumAvail = BLOCKSIZE;
	startup_cnfClausesNumUsed = 0;
	startup_cnfClauses = malloc(sizeof(int32_t*) * startup_cnfClausesNumAvail);
	if (startup_cnfClauses == NULL){
		printf("c ERROR. STARTUP was unable to allocate memory for the CNF.\n");
		startup_returnCode = STARTUP_ERROR;
		return;
	}

	#ifdef COLLECTSTATS
	stats_startup_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void startup_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_startup_component_totalCalls;
	#endif
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP: DISPOSE...\n");
	#endif
	uint32_t i;

	if (param_startupOutput != NULL){
		fclose(param_startupOutput);
		param_startupOutput = NULL;
	}

	startup_returnCode = STARTUP_UNKNOWN;

	if (startup_cnfClauses != NULL){
		for (i = 0; i < startup_cnfClausesNumUsed; ++i){
			if (startup_cnfClauses[i] != NULL){
				free(startup_cnfClauses[i]);
				startup_cnfClauses[i] = NULL;
			}
		}
		free(startup_cnfClauses);
		startup_cnfClauses = NULL;
	}
	startup_cnfClausesNumAvail = 0;
	startup_cnfClausesNumUsed = 0;

	#ifdef COLLECTSTATS
	stats_startup_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

#ifdef VERBOSE_STARTUP
void startup_printCNFClausesArray(){
	printf("c   STARTUP:   startup_cnfClauses: [%p, %d, %d]\n",
			(void*) startup_cnfClauses, startup_cnfClausesNumUsed, startup_cnfClausesNumAvail);

	if (startup_cnfClauses == NULL) return;
	int32_t lit;
	uint32_t i, j;

	for (i=0; i < startup_cnfClausesNumUsed; ++i){
		j = 0;
		printf("c   STARTUP:     CNF Array: Clause %-6d: ", i);
		while ((lit = startup_cnfClauses[i][j]) != 0){
			printf("%d ", lit);
			++j;
		}
		printf("\n");
	}
}
void startup_printVARNames(){
	printf("c   STARTUP:   main_varNames: [%p, %d]\n", (void*)main_varNames, f.n_initial);
	int32_t i;
	if (main_varNames == NULL) return;
	for (i=1; i<f.n_initial+1; ++i){
		printf("c   STARTUP:   main_varNames: Variable %-6d: ", i);
		if (main_varNames[i] == NULL){
			printf("NULL\n");
		} else {
			printf("%s\n", main_varNames[i]);
		}
	}
}
#endif

void startup_intern_outputToFile(){
	int32_t i,j, *upCurrent = main_simpleUPQueue, numUnits;
	clause *c;
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP: Writing formula to file... ");
	#endif

	fprintf(param_startupOutput, "c DIMETHEUS SAT Solver -- Startup output.\n");
	fprintf(param_startupOutput, "c   Version: %d.%d.%d\n", VERSION_MA, VERSION_MI, VERSION_RE);
	fprintf(param_startupOutput, "c\n");

	//We first check how many units we have and forcibly output unit clauses for them.
	fprintf(param_startupOutput, "c Initial Units: ");
	numUnits = 0;
	while (upCurrent < main_simpleUPQueue_current){
		++numUnits;
		fprintf(param_startupOutput, "%d ", *upCurrent);
		++upCurrent;
	}

	fprintf(param_startupOutput, "(%d total)\n", numUnits);

	fprintf(param_startupOutput, "p cnf %d %d\n", f.n_initial, f.m_el_used + f.m_eo_used + numUnits);
	//Unit clauses first.
	upCurrent = main_simpleUPQueue;
	while (upCurrent < main_simpleUPQueue_current){
		fprintf(param_startupOutput, "%d 0\n", *upCurrent);
		++upCurrent;
	}
	//Original clauses.
	for (j = 0; j < f.m_eo_used; ++j){
		c = f.clauses_eo[j];
		for (i = 0; i < c->size; ++i){
			fprintf(param_startupOutput, "%d ", c->lits[i]);
		}
		fprintf(param_startupOutput, "0\n");
	}
	//Learned clauses.
	for (j = 0; j < f.m_el_used; ++j){
		c = f.clauses_el[j];
		for (i = 0; i < c->size; ++i){
			fprintf(param_startupOutput, "%d ", c->lits[i]);
		}
		fprintf(param_startupOutput, "0\n");
	}

	#ifdef VERBOSE_STARTUP
	printf(" done.\n");
	#endif
}

void startup_extern_start(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_startup_component_totalCalls;
	#endif

	//We first initialize som basic stuff, like the random number generator and the statistical data or scoring data.
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Initializing basics...\n");
	printf("c   STARTUP:     Random number generator...\n");
	#endif
	#ifndef COMPILEASLIBRARY
	srand(param_seed);
	#endif
	rand_init();
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     ");rand_printConfig_inline();printf("\n");
	#endif
	#ifdef COLLECTSTATS
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Statistical data...\n");
	#endif
	stats_init();			if (startup_returnCode != STARTUP_UNKNOWN){ return; }
	#endif
	#ifdef SCORING
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Scoring...\n");
	#endif
	scoring_init();			if (startup_returnCode != STARTUP_UNKNOWN){ return; }
	#endif

	//We then determine the input format that was given to the solver.
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Determining input format...\n");
	#endif
	determ_resetModule();					if (determ_returnCode != DETERM_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	determ_initModule();					if (determ_returnCode != DETERM_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	determ_extern_determineInputFormat();
	if (determ_returnCode != DETERM_UNKNOWN){
		printf("c ERROR. The solver cannot handle this type of input format.\n");
		startup_returnCode = STARTUP_BADINPUTFORMAT;
		return;
	}
	determ_disposeModule();					if (determ_returnCode != DETERM_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }

	//Depending on the problem type, we now take the proper actions of initializing the solvers data structure.
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Determining problem specifications...\n");
	#endif
	speccer_resetModule();					if (speccer_returnCode != SPECCER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	speccer_initModule();					if (speccer_returnCode != SPECCER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	speccer_extern_determineSpecification();
	if (speccer_returnCode == SPECCER_ERROR){
		printf("c ERROR. The solver was unable to acquire the problem specifications.\n");
		startup_returnCode = STARTUP_BADFORMULA;
		return;
	}
	speccer_disposeModule();				if (speccer_returnCode != SPECCER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }

	#ifdef VERBOSE_STARTUP
	//startup_printVARNames();
	//startup_printCNFClausesArray();
	#endif

	//Based on the problem specifications, we can now initialize the data structures.
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Initializing main data-structures...\n");
	#endif
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Formula...\n");fflush(stdout);
	#endif
	if (!formula_init())		{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Variables...\n");fflush(stdout);
	#endif
	if (!variables_init())		{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Literals...\n");fflush(stdout);
	#endif
	if (!literals_init())		{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Simple UP...\n");fflush(stdout);
	#endif
	if (!main_simpleUP_init())	{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Decision stack...\n");fflush(stdout);
	#endif
	if (!main_decStack_init())	{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Complex UP...\n");fflush(stdout);
	#endif
	if (!main_complexup_init())	{startup_returnCode = STARTUP_ERROR; return;}

	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Clauses priority queue and stacks...\n");fflush(stdout);
	#endif
	if (!cQ_init())				{startup_returnCode = STARTUP_ERROR; return;}
	if (!cS_init())				{startup_returnCode = STARTUP_ERROR; return;}
	if (!cSS0_init())			{startup_returnCode = STARTUP_ERROR; return;}
	if (!cSS1_init())			{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Literals priority queue and stack...\n");fflush(stdout);
	#endif
	if (!lQ_init())				{startup_returnCode = STARTUP_ERROR; return;}
	if (!lS_init())				{startup_returnCode = STARTUP_ERROR; return;}
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:     Variables priority queue and stack...\n");fflush(stdout);
	#endif
	if (!vQ_init())				{startup_returnCode = STARTUP_ERROR; return;}
	if (!vS_init())				{startup_returnCode = STARTUP_ERROR; return;}

	//We must now load the problem into the CNF formula.
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Loading the problem...\n");
	#endif
	loader_resetModule();					if (loader_returnCode != LOADER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	loader_initModule();					if (loader_returnCode != LOADER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }
	loader_extern_loadAsCNF();
	if (loader_returnCode == LOADER_ERROR){
		startup_returnCode = STARTUP_ERROR; return;
	} else if (loader_returnCode == LOADER_SAT){
		startup_returnCode = STARTUP_SAT; return;
	} else if (loader_returnCode == LOADER_UNSAT){
		startup_returnCode = STARTUP_UNSAT; return;
	}
	loader_disposeModule();				if (loader_returnCode != LOADER_UNKNOWN){ startup_returnCode = STARTUP_ERROR; return; }

	#ifndef COMPILEASLIBRARY
	#ifdef VERBOSE_STARTUP
	printf("c   STARTUP:   Closing formula file...\n");
	#endif
	fclose(param_formula);
	param_formula = NULL;
	#endif

	if (param_startupOutput != NULL) startup_intern_outputToFile();

	if (startup_returnCode == STARTUP_UNKNOWN){
		#ifdef VERBOSE_STARTUP
		printf("c   STARTUP:   Initial unit propagation...\n");
		#endif
		clause* c = main_simpleUP_propagate_final();
		if (c != NULL){
			#ifdef VERBOSE_STARTUP
			printf("c   STARTUP:     The initial unit propagation ran into a conflict in clause %p.\n", (void*)c);
			#endif
			startup_returnCode = STARTUP_UNSAT;
		} else if (f.m_eo_used + f.m_el_used == 0){
			#ifdef VERBOSE_STARTUP
			printf("c   STARTUP:     There are no further enabled clauses. We found a satisfying assignment.\n");
			#endif
			startup_returnCode = STARTUP_SAT;
		} else {
			#ifdef VERBOSE_STARTUP
			printf("c   STARTUP:     CNF has %d variables and %d clauses remaining.\n", f.n_vars_e_used, f.m_eo_used);
			#endif
			f.n_afterPrep = f.n_vars_e_used;
		}
		if (startup_returnCode == STARTUP_UNKNOWN) {
			#ifdef VERBOSE_STARTUP
			printf("c   STARTUP:   Resetting the fundamental variable activities and phases based on occurrences.\n");
			#endif
			variables_resetActivitiesAndPhases();
		}
	} else {
		#ifdef VERBOSE_STARTUP
		printf("c   STARTUP:   Initial unit propagation skipped. The satisfiability has already been decided.\n");
		#endif
	}

	#ifdef COLLECTSTATS
	stats_main_time_init = STATS_GET_CURRENT_TIME_IN_SEC();
	stats_startup_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
