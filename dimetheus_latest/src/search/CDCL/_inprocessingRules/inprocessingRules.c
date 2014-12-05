/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inprocessingRules.h"

void cdcl_extern_inprocessingRules_switch(int32_t switchTo){
	//Use to switch the in-processing rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_inprocessingRules_disposePlugin();
	//We override the parameter to use the rule indicated.
	param_cdclInprocessingRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_inprocessingRules_initPlugin();
}

void cdcl_inprocessingRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting In-Processing rule (IR) plug-in...\n");
	#endif
	cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_null_inprocess;
	cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_null_prepare;

	//Reset IR data-structures here.
	cdcl_inprocessingRules_reduceFrom 				= NULL;
	cdcl_inprocessingRules_lastConflict				= 0;
	cdcl_inprocessingRules_res						= NULL;
	cdcl_inprocessingRules_resAvail 				= 0;
	cdcl_inprocessingRules_resUsed 					= 0;
	cdcl_inprocessingRules_resMemAvail 				= NULL;
	cdcl_inprocessingRules_resMemUsed 				= NULL;
	cdcl_inprocessingRules_G_negV 					= NULL;
	cdcl_inprocessingRules_G_negVAvail 				= 0;
	cdcl_inprocessingRules_G_negVUsed 				= 0;
	cdcl_inprocessingRules_G_posV 					= NULL;
	cdcl_inprocessingRules_G_posVAvail 				= 0;
	cdcl_inprocessingRules_G_posVUsed 				= 0;
	cdcl_inprocessingRules_unhiding_SPlus 			= NULL;
	cdcl_inprocessingRules_unhiding_SMinus 			= NULL;
	cdcl_inprocessingRules_litPerm 					= NULL;
	cdcl_inprocessingRules_unhiding_lStack 			= NULL;
	cdcl_inprocessingRules_unhiding_lStack_current 	= NULL;
	cdcl_inprocessingRules_els_classes 				= NULL;
	cdcl_inprocessingRules_els_classes_avail 		= 0;
	cdcl_inprocessingRules_els_classes_used 		= 0;
	cdcl_inprocessingRules_els_classes_memUsed		= NULL;
	cdcl_inprocessingRules_els_lStack 				= NULL;
	cdcl_inprocessingRules_els_lStack_current 		= NULL;
	cdcl_inprocessingRules_trl_lQ					= NULL;
	cdcl_inprocessingRules_trl_lQ_c					= NULL;
	cdcl_inprocessingRules_trl_lQ_e					= NULL;
	cdcl_inprocessingRules_trl_faileds				= NULL;
}

void cdcl_inprocessingRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the In-Processing rule... ");
	#endif
	uint32_t i;
	//Pick the In-Processing rule.
	if (param_cdclInprocessingRule == CDCL_INPROCESSINGRULE_NULL){
		#ifdef VERBOSE_CDCL
		printf("NULL");
		#endif
		cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_null_inprocess;
		cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_null_prepare;
	} else if (param_cdclInprocessingRule == CDCL_INPROCESSINGRULE_REDUCEONLY){
		#ifdef VERBOSE_CDCL
		printf("REDUCEONLY");
		#endif
		cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_reduceonly_inprocess;
		cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_reduceonly_prepare;
	} else if (param_cdclInprocessingRule == CDCL_INPROCESSINGRULE_SIMPLE){
		#ifdef VERBOSE_CDCL
		printf("SIMPLE");
		#endif
		cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_simple_inprocess;
		cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_simple_prepare;
	} else if (param_cdclInprocessingRule == CDCL_INPROCESSINGRULE_FULL){
		#ifdef VERBOSE_CDCL
		printf("FULL");
		#endif
		cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_full_inprocess;
		cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_full_prepare;
	} else if (param_cdclInprocessingRule == CDCL_INPROCESSINGRULE_TESTING){
		#ifdef VERBOSE_CDCL
		printf("TESTING");
		#endif
		cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_testing_inprocess;
		cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_testing_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize IR data-structures here.
	cdcl_inprocessingRules_reduceFrom = cdcl_decStackPtrs[0];
	cdcl_inprocessingRules_lastConflict = 0;

	//We initialize the resolvents and the corresponding data-structures.
	cdcl_inprocessingRules_resUsed = 0;
	cdcl_inprocessingRules_resAvail = 2*BLOCKSIZE;
	cdcl_inprocessingRules_res = malloc(sizeof(int32_t*) * cdcl_inprocessingRules_resAvail);
	if (cdcl_inprocessingRules_res == NULL){
		printf("c ERROR. CDCL IR failed to initialize the resolvents array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_resMemAvail = malloc(sizeof(int32_t) * cdcl_inprocessingRules_resAvail);
	if (cdcl_inprocessingRules_resMemAvail == NULL){
		printf("c ERROR. CDCL IR failed to initialize the resolvents memory availability array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_resMemUsed = malloc(sizeof(int32_t) * cdcl_inprocessingRules_resAvail);
	if (cdcl_inprocessingRules_resMemAvail == NULL){
		printf("c ERROR. CDCL IR failed to initialize the resolvents memory usage array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	if (cdcl_returnCode == CDCL_ERROR) {
		if (cdcl_inprocessingRules_res != NULL){
			for (i = 0; i < cdcl_inprocessingRules_resAvail; ++i){	cdcl_inprocessingRules_res[i] = NULL; }
		}
		return;
	}
	for (i = 0; i < cdcl_inprocessingRules_resAvail; ++i){
		if (cdcl_returnCode == CDCL_UNKNOWN){
			cdcl_inprocessingRules_res[i] = NULL;
			cdcl_inprocessingRules_res[i] = malloc(sizeof(int32_t)*BLOCKSIZE);
			if (cdcl_inprocessingRules_res[i] == NULL) {
				cdcl_returnCode = CDCL_ERROR;
				cdcl_inprocessingRules_resMemAvail[i] = 0;
				cdcl_inprocessingRules_resMemUsed[i] = 0;
			} else {
				cdcl_inprocessingRules_resMemAvail[i] = BLOCKSIZE;
				cdcl_inprocessingRules_resMemUsed[i] = 0;
			}
		} else {
			cdcl_inprocessingRules_res[i] = NULL;
			cdcl_inprocessingRules_resMemAvail[i] = 0;
			cdcl_inprocessingRules_resMemUsed[i] = 0;
		}
	}

	if (cdcl_returnCode == CDCL_ERROR){
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_G_negVAvail = BLOCKSIZE;
	cdcl_inprocessingRules_G_negVUsed = 0;
	cdcl_inprocessingRules_G_negV = malloc(sizeof(cdclClause*)*cdcl_inprocessingRules_G_negVAvail);
	if (cdcl_inprocessingRules_G_negV == NULL){
		printf("c ERROR. CDCL IR failed to initialize the gate extraction array G_negV. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_G_posVAvail = BLOCKSIZE;
	cdcl_inprocessingRules_G_posVUsed = 0;
	cdcl_inprocessingRules_G_posV = malloc(sizeof(cdclClause*)*cdcl_inprocessingRules_G_posVAvail);
	if (cdcl_inprocessingRules_G_posV == NULL){
		printf("c ERROR. CDCL IR failed to initialize the gate extraction array G_posV. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_unhiding_SPlus = malloc(sizeof(cdclLiteral*)*(f.n_afterPrep+2));
	if (cdcl_inprocessingRules_unhiding_SPlus == NULL){
		printf("c ERROR. The CDCL IR Plug-in was unable to allocate the SPlus array for Unhiding. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_unhiding_SMinus = malloc(sizeof(cdclLiteral*)*(f.n_afterPrep+2));
	if (cdcl_inprocessingRules_unhiding_SMinus == NULL){
		printf("c ERROR. The CDCL IR Plug-in was unable to allocate the SMinus array for Unhiding. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_unhiding_lStack = malloc(sizeof(cdclLiteral*)*(f.n_afterPrep+2));
	if (cdcl_inprocessingRules_unhiding_lStack == NULL){
		printf("c ERROR. The CDCL IR Plug-in was unable to allocate the UNH Stack array for Unhiding. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_unhiding_lStack[0] = NULL;
	cdcl_inprocessingRules_unhiding_lStack_current = cdcl_inprocessingRules_unhiding_lStack + 1;

	cdcl_inprocessingRules_litPerm = malloc(sizeof(cdclLiteral*)*(2*f.n_afterPrep+1));
	if (cdcl_inprocessingRules_litPerm == NULL){
		printf("c ERROR. The CDCL IR Plug-in was unable to allocate the SMinus array for Unhiding. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_els_classes_avail = BLOCKSIZE;
	cdcl_inprocessingRules_els_classes_used = 0;

	cdcl_inprocessingRules_els_classes = malloc(sizeof(cdclLiteral**) * cdcl_inprocessingRules_els_classes_avail);
	if (cdcl_inprocessingRules_els_classes == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the ELS classes array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_els_classes_memUsed = malloc(sizeof(uint32_t) * cdcl_inprocessingRules_els_classes_avail);
	if (cdcl_inprocessingRules_els_classes_memUsed == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the ELS memory usage array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_els_classes_memAvail = malloc(sizeof(uint32_t) * cdcl_inprocessingRules_els_classes_avail);
	if (cdcl_inprocessingRules_els_classes_memAvail == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the ELS memory availability array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		return;
	}

	for (i = 0; i < cdcl_inprocessingRules_els_classes_avail; ++i){
		if (cdcl_returnCode == CDCL_UNKNOWN){
			cdcl_inprocessingRules_els_classes[i] = NULL;
			cdcl_inprocessingRules_els_classes[i] = malloc(sizeof(cdclLiteral*) * BLOCKSIZE);
			if (cdcl_inprocessingRules_els_classes[i] == NULL){
				cdcl_returnCode = CDCL_ERROR;
				cdcl_inprocessingRules_els_classes_memUsed[i] = 0;
				cdcl_inprocessingRules_els_classes_memAvail[i] = 0;
			} else {
				cdcl_inprocessingRules_els_classes_memUsed[i] = 0;
				cdcl_inprocessingRules_els_classes_memAvail[i] = BLOCKSIZE;
			}
		} else {
			cdcl_inprocessingRules_els_classes[i] = NULL;
			cdcl_inprocessingRules_els_classes_memUsed[i] = 0;
			cdcl_inprocessingRules_els_classes_memAvail[i] = 0;
		}
	}
	if (cdcl_returnCode == CDCL_ERROR) {
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_inprocessingRules_els_lStack = malloc(sizeof(cdclLiteral*)*(2*f.n_afterPrep + 2));
	if (cdcl_inprocessingRules_els_lStack == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the ELS literal stack array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_els_lStack[0] = NULL;
	cdcl_inprocessingRules_els_lStack_current = cdcl_inprocessingRules_els_lStack + 1;

	cdcl_inprocessingRules_trl_lQ = malloc(sizeof(cdclLiteral*)*(4*f.n_afterPrep + 4));
	if (cdcl_inprocessingRules_trl_lQ == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the TREELOOK literal queue. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_inprocessingRules_trl_lQ_c = cdcl_inprocessingRules_trl_lQ;
	cdcl_inprocessingRules_trl_lQ_e = cdcl_inprocessingRules_trl_lQ;

	cdcl_inprocessingRules_trl_faileds = malloc(sizeof(cdclLiteral*)*(f.n_afterPrep + 1));
	if (cdcl_inprocessingRules_trl_lQ == NULL){
		printf("c ERROR. The CDCL IR Plug-in failed to initialize the TREELOOK failed literal array. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

}

void cdcl_inprocessingRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing In-Processing rule (IR) plug-in...\n");
	#endif
	uint32_t i;
	cdcl_inprocessingRule_inprocess  	= &cdcl_inprocessingRule_null_inprocess;
	cdcl_inprocessingRule_prepare    	= &cdcl_inprocessingRule_null_prepare;

	//Dispose IR data-structures here.
	cdcl_inprocessingRules_reduceFrom = NULL;
	cdcl_inprocessingRules_lastConflict = 0;

	if (cdcl_inprocessingRules_res != NULL){
		for (i = 0; i < cdcl_inprocessingRules_resAvail; ++i){
			if (cdcl_inprocessingRules_res[i] != NULL){
				free(cdcl_inprocessingRules_res[i]);
				cdcl_inprocessingRules_res[i] = NULL;
			}
		}
		free(cdcl_inprocessingRules_res);
		cdcl_inprocessingRules_res = NULL;
	}
	cdcl_inprocessingRules_resAvail = 0;
	cdcl_inprocessingRules_resUsed = 0;

	if (cdcl_inprocessingRules_resMemAvail != NULL){
		free(cdcl_inprocessingRules_resMemAvail);
		cdcl_inprocessingRules_resMemAvail = NULL;
	}

	if (cdcl_inprocessingRules_resMemUsed != NULL){
		free(cdcl_inprocessingRules_resMemUsed);
		cdcl_inprocessingRules_resMemUsed = NULL;
	}

	if (cdcl_inprocessingRules_G_negV != NULL){
		free(cdcl_inprocessingRules_G_negV);
		cdcl_inprocessingRules_G_negV = NULL;
	}
	if (cdcl_inprocessingRules_G_posV != NULL){
		free(cdcl_inprocessingRules_G_posV);
		cdcl_inprocessingRules_G_posV = NULL;
	}
	cdcl_inprocessingRules_G_negVAvail 				= 0;
	cdcl_inprocessingRules_G_negVUsed 				= 0;
	cdcl_inprocessingRules_G_posVAvail 				= 0;
	cdcl_inprocessingRules_G_posVUsed 				= 0;

	if (cdcl_inprocessingRules_unhiding_SPlus != NULL){
		free(cdcl_inprocessingRules_unhiding_SPlus);
		cdcl_inprocessingRules_unhiding_SPlus = NULL;
	}
	if (cdcl_inprocessingRules_unhiding_SMinus != NULL){
		free(cdcl_inprocessingRules_unhiding_SMinus);
		cdcl_inprocessingRules_unhiding_SMinus = NULL;
	}
	if (cdcl_inprocessingRules_unhiding_lStack != NULL){
		free(cdcl_inprocessingRules_unhiding_lStack);
		cdcl_inprocessingRules_unhiding_lStack = NULL;
		cdcl_inprocessingRules_unhiding_lStack_current = NULL;
	}
	if (cdcl_inprocessingRules_litPerm != NULL){
		free(cdcl_inprocessingRules_litPerm);
		cdcl_inprocessingRules_litPerm = NULL;
	}

	if (cdcl_inprocessingRules_els_classes != NULL){
		for (i = 0; i < cdcl_inprocessingRules_els_classes_avail; ++i){
			if (cdcl_inprocessingRules_els_classes[i] != NULL){
				free(cdcl_inprocessingRules_els_classes[i]);
				cdcl_inprocessingRules_els_classes[i] = NULL;
			}
		}
		free(cdcl_inprocessingRules_els_classes);
		cdcl_inprocessingRules_els_classes = NULL;
	}
	cdcl_inprocessingRules_els_classes_avail = 0;
	cdcl_inprocessingRules_els_classes_used = 0;

	if (cdcl_inprocessingRules_els_classes_memUsed != NULL){
		free(cdcl_inprocessingRules_els_classes_memUsed);
		cdcl_inprocessingRules_els_classes_memUsed = NULL;
	}
	if (cdcl_inprocessingRules_els_classes_memAvail != NULL){
		free(cdcl_inprocessingRules_els_classes_memAvail);
		cdcl_inprocessingRules_els_classes_memAvail = NULL;
	}

	if (cdcl_inprocessingRules_els_lStack != NULL){
		free(cdcl_inprocessingRules_els_lStack);
		cdcl_inprocessingRules_els_lStack = NULL;
		cdcl_inprocessingRules_els_lStack_current = NULL;
	}

	if (cdcl_inprocessingRules_trl_lQ != NULL){
		free(cdcl_inprocessingRules_trl_lQ);
		cdcl_inprocessingRules_trl_lQ = NULL;
		cdcl_inprocessingRules_trl_lQ_c = NULL;
		cdcl_inprocessingRules_trl_lQ_e = NULL;
	}

	if (cdcl_inprocessingRules_trl_faileds != NULL){
		free(cdcl_inprocessingRules_trl_faileds);
		cdcl_inprocessingRules_trl_faileds = NULL;
	}

}

cdclVariable* cdcl_inprocessingRules_intern_getRootRepresentative(cdclVariable* v){
	cdclVariable *anc;
	int32_t ancID = GET_CDCL_VAR_PTR_ISEQUIVTO(v);

	if (ancID == 0){
		return v;
	}
	anc = cdcl_variables + abs(ancID);
	return cdcl_inprocessingRules_intern_getRootRepresentative(anc);
}

void cdcl_inprocessingRules_extern_reconstructSolution(){
	if (cdcl_cls_r_used == 0){
		#ifdef VERBOSE_CDCL
		printf("c     CDCL: In-Processing: Solution reconstruction not necessary.\n");
		#endif
		return;
	}

	#ifdef VERBOSE_CDCL
	printf("c     CDCL: In-Processing: Solution reconstruction necessary...\n");
	printf("c     CDCL: In-Processing: Preparing equivalent variables data-structure...");
	#endif

	remCdclClause *rc;
	cdclVariable *v, *root;
	uint32_t rootID, isSat;
	int32_t i, j, lit;
	//First, we initialize the equivalence classes.
	for (i = 1; i < f.n_initial+1; ++i){
		v = cdcl_variables + i;
		cdcl_eqVars[i].representedBy = NULL;
		cdcl_eqVars[i].numRepresents = 1;
		cdcl_eqVars[i].equiv = realloc(cdcl_eqVars[i].equiv, sizeof(cdclVariable*)*1);
		cdcl_eqVars[i].equiv[0] = v;
	}
	//Now, we build the equivalence classes.
	for (i = 1; i < f.n_initial+1; ++i){
		//We ignore variables that are disabled in the outer formula.
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) continue;
		v = cdcl_variables + i;
		//First, we get the root equivalence representative for this variable.
		root = cdcl_inprocessingRules_intern_getRootRepresentative(v);
		//If the variable is its own representative, then nothing is to be done.
		if (root == v) continue;
		//Otherwise, we must ensure that v knows it is represented by the root, and add v to the equivalence set of root.
		rootID = GET_CDCL_VAR_PTR_ID(root);
		cdcl_eqVars[i].representedBy = cdcl_eqVars + rootID;
		cdcl_eqVars[rootID].equiv = realloc(cdcl_eqVars[rootID].equiv, sizeof(cdclVariable*)*(cdcl_eqVars[rootID].numRepresents+1));
		cdcl_eqVars[rootID].equiv[cdcl_eqVars[rootID].numRepresents++] = v;
	}

	#ifdef VERBOSE_CDCL
	printf("done.\nc     CDCL: In-Processing:   Flipping: ");fflush(stdout);
	#endif

	//We will now check through all the removed clauses if they are satisfied. If not, we will flip their litOfInterest.
	for (i = cdcl_cls_r_used-1; i > -1; --i){
		rc = cdcl_cls_r[i];
		isSat = 0;
		for (j = 0; j < rc->size; ++j){
			lit = rc->lits[j];
			if (IS_CDCL_LIT_SAT(lit)){
				isSat = 1;
				break;
			}
		}
		if (!isSat){
			//The clause is not satisfied. We must flip its literal of interest.
			v = cdcl_variables + abs(rc->litOfInterest);
			#ifdef VERBOSE_CDCL
			printf("R[%i]%p(", (int32_t)rc->byAction, (void*)rc);
			#endif
			cdcl_variables_flipVarWithEquivs(v);
			#ifdef VERBOSE_CDCL
			printf(") ");
			#endif
		}
	}

	#ifdef VERBOSE_CDCL
	printf("done.\n");
	printf("c     CDCL:   In-Processing: Solution reconstruction successful.\n");
	#endif
}

void cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(cdclClause* c){
	//Handle all the queue and stack updates if a clause is deleted.
	uint32_t i;
	cdclVariable *v;

	//CLAUSE PRIORITY QUEUE.
	cdcl_clauses_Q_delete(c);
	//We touch the necessary clauses.
	if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_se){
		//During subsumption elimination, dropping a clause does not result in any touch operations.
	} else if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_str){
		//During strengthening, dropping a clause does not result in any touch operations.
	} else if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_treelook){
		//During tree-look, dropping a clause does not result in any touch operations.
	}

	//CLAUSE STACK.
	cdcl_clauses_S_delete(c);

	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_fld){
		//There is nothing to be added to the literal priority queue as long as a clause is removed.
	} else if (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_nhbr){
		//Update the literals if they are in the queue and the disabled clause is binary.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			if (GET_CDCL_LIT_QPOS(-GET_CDCL_CLS_LITNUM(c,0)) > 0) cdcl_literals_Q_touchLiteral(cdcl_literals-GET_CDCL_CLS_LITNUM(c,0));
			if (GET_CDCL_LIT_QPOS(-GET_CDCL_CLS_LITNUM(c,0)) > 0) cdcl_literals_Q_touchLiteral(cdcl_literals-GET_CDCL_CLS_LITNUM(c,1));
		}
	} else if  (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_niver){
		//We must update all the literals in the queue that participated in the clause.
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c, i));
			if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c, i));
			}
		}
	}

	//LITERAL STACK.
	if (cdcl_literals_S_touchLiteral == cdcl_literals_S_touchLiteral_inprocessing_ternary){
		//No actions are performed if a clause is dropped.
	}

	//VARIABLE STACK.
	if (cdcl_variables_S_touchVariable == cdcl_variables_S_touchVariable_inprocessing_pl){
		//We touch all the variables for the variable stack, because they now might have only one occurrence in one literal.
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c, i));
			if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				cdcl_variables_S_touchVariable(v);
			}
		}
	}

}

void cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(cdclClause* c){
	//Queue and stack updates.
	//Handle all the queue and stack updates if a clause is added.

	cdclLiteral *l;
	int32_t i;


	//CLAUSE PRIORITY QUEUE.
	//We touch the necessary clauses.
	cdcl_clauses_Q_touchClause(c);
	if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_se){
		//Do nothing.
	} else 	if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_str){
		//Do nothing.
	} else if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_treelook){
		//Do nothing.
	}

	//CLAUSE STACK.
	cdcl_clauses_S_touchClause(c);

	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_fld){
		//If the clause has size 2 or 3, we add its literals with opposite sign to the FLD queue.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
		} else if (GET_CDCL_CLS_SIZE(c) == 3){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,2));
		}
	} else if (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_nhbr){
		//If the clause is binary, we touch all the opposite literals to add it to the NHBR queue.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
		}
	} else if  (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_niver){
		//We must update all the literals in the queue that participate in the clause.
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			l = cdcl_literals + GET_CDCL_CLS_LITNUM(c, i);
			if (GET_CDCL_LIT_PTR_QPOS(l) > 0) cdcl_literals_Q_touchLiteral(l);
		}
	}

	//LITERAL STACK.
	if (cdcl_literals_S_touchLiteral == cdcl_literals_S_touchLiteral_inprocessing_ternary){
		//If we enable a ternary clause we add all its literals to the literal stack.
		if (GET_CDCL_CLS_SIZE(c) == 3){
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,1));
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,2));
		}
	}

	//VARIABLE STACK.
	if (cdcl_variables_S_touchVariable == cdcl_variables_S_touchVariable_inprocessing_pl){
		//Do nothing.
	}
}

void cdcl_inprocessingRules_intern_QSUpdateLiteralDeletion(cdclClause* c, int32_t lit){
	//Queue and stack updates.
	//Handle all the queue and stack updates if a clauses literal is deleted.

	//CLAUSE PRIORITY QUEUE.
	//We touch all the necessary clauses.
	if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_se){
		cdcl_clauses_Q_touchClause(c);
	} else if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_str){
		cdcl_clauses_Q_touchClause(c);
	} else if (cdcl_clauses_Q_touchClause == cdcl_clauses_Q_touchClause_inprocessing_treelook){
		//A clause that loses a literal must be re-checked if it is unsatisfied under the tree-look assignment.
		cdcl_clauses_Q_touchClause(c);
	}

	//CLAUSE STACK.

	//LITERAL PRIORITY QUEUE.
	//Then, we touch all the literals.
	if (lQ_touchLiteral == lQ_touchLiteral_fld){
		//If the clause has size 2 or 3, we add all its literals with opposite sign to the FLD queue.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
		} else if (GET_CDCL_CLS_SIZE(c) == 3){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,2));
		}
	} else if (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_nhbr){
		//If the clause became binary, we touch all the literals in it with opposite sign.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_Q_touchLiteral(cdcl_literals - GET_CDCL_CLS_LITNUM(c,1));
		}
	} else if  (cdcl_literals_Q_touchLiteral == cdcl_literals_Q_touchLiteral_inprocessing_niver){
		//We must update the literal that has been removed from the clause.
		if (GET_CDCL_LIT_PTR_QPOS((cdcl_literals + lit)) > 0) cdcl_literals_Q_touchLiteral(cdcl_literals + lit);
	}

	//LITERAL STACK.
	if (cdcl_literals_S_touchLiteral == cdcl_literals_S_touchLiteral_inprocessing_ternary){
		//If the clause is now ternary, we want to add all its literals to the literal stack.
		if (GET_CDCL_CLS_SIZE(c) == 3){
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,0));
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,1));
			cdcl_literals_S_touchLiteral(cdcl_literals + GET_CDCL_CLS_LITNUM(c,2));
		}
	}

	//VARIABLE STACK.
	if (cdcl_variables_S_touchVariable == cdcl_variables_S_touchVariable_inprocessing_pl){
		//We touch the variable for the variable stack, as the variable might now have lost all occurrences for one literal.
		if (IS_CDCL_VAR_UNASSIGNED(abs(lit))){
			cdcl_variables_S_touchVariable(cdcl_variables + abs(lit));
		}
	}
}


cdclClause* cdcl_inprocessingRules_upPropagate(){
	cdclLiteral* l;  			//The current literal to propagate.
	cdclVariable* v; 			//The corresponding variable.
	cdclClause* c;	 			//The clause we currently work on when updating the watchers.
	int32_t i,lid, lit, litPos;	//The integer representation of the literal and a temporary literal and a literal position.
	int32_t offset,litToRemove; //The offset in the watcher list for the clause we currently work on.
	int32_t litID, impLitA, impLitB;
	uint32_t occLitPos;
	uint64_t sig;
	//This method will perform unit propagation from the current position in the decision stack. We assume that the UP is
	//initialized correctly, that is the cdcl_up_current pointer points to the position in the decision stack that we want to
	//continue a unit propagation from. Additionally, we assume that the cdcl_up_end pointer points to the next position
	//in the decision stack where we want to append new elements at. We also assume that the cdcl_up_stamp is fresh.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	cdclLiteral **reduceFrom = cdcl_up_current, **reduce;

	//We propagate as long as we still have elements left.
	while (cdcl_up_current < cdcl_up_end){
		//We grab the next literal for which we want to assign the variable.
		l = *(cdcl_up_current++);
		lid = GET_CDCL_LIT_ID_FROM_PTR(l);

		//We assign the corresponding variable.
		if (lid < 0){
			//The literal is negative. The variable must be assigned to 0.
			v = cdcl_variables - lid;
			SET_CDCL_VAR_PTR_ASS_0(v);
		} else {
			//the literal is positive. The variable must be assigned to 1.
			v = cdcl_variables + lid;
			SET_CDCL_VAR_PTR_ASS_1(v);
		}
		//We disable the variable, such that it does not appear in the variable priority queue anymore.
		cdcl_inprocessingRules_intern_disableVariable(v);
		#ifdef COLLECTSTATS
		++stats_cdcl_numPropagations;
		#endif

		//Walk through the implication list of l. All the implications must be put on the stack. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i+=2){
			lit = GET_CDCL_LIT_PTR_IMPNUM(l, i);
			v = cdcl_variables + abs(lit);
			if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					#ifdef COLLECTSTATS
					stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
					#endif
					return GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i);
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					CDCL_UP_ADD_IMP(lit , v, GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i));
				} //Else, the literal is already enqueued.
			}
		}

		//We must now walk through the watcher lists of the literal that becomes false, that is, -l is checked.
		l = cdcl_literals - lid;
		//Walk through the watcher lists of l (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the CDCL UP as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (GET_CDCL_LIT_PTR_NUMW1_USED(l) > offset){
			c = GET_CDCL_LIT_PTR_W1_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CDCL_CLS_REPLIT(c);
			if (IS_CDCL_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CDCL_CLS_WHEREW1(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
				lit = GET_CDCL_CLS_LITNUM(c,i);
				if (IS_CDCL_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CDCL_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the second watcher.
				if (lit == GET_CDCL_CLS_W2(c)) continue;
				//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
				if (IS_CDCL_VAR_UNASSIGNED(abs(lit))){
					//We have found an unassigned literal that can serve as watcher.
					SET_CDCL_CLS_W1(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CDCL_CLS_W1(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CDCL_CLS_FROM_LIT_W1LIST(-lid, litPos);
				ADD_CDCL_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CDCL_CLS_W2(c);
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					#ifdef COLLECTSTATS
					stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
					#endif
					return c;
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = cdcl_variables + abs(lit);
					CDCL_UP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}

		//WL2
		offset = 0;
		while (GET_CDCL_LIT_PTR_NUMW2_USED(l) > offset){
			c = GET_CDCL_LIT_PTR_W2_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CDCL_CLS_REPLIT(c);
			if (IS_CDCL_LIT_SAT(lit)){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CDCL_CLS_WHEREW2(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
				lit = GET_CDCL_CLS_LITNUM(c,i);
				if (IS_CDCL_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CDCL_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the first watcher.
				if (lit == GET_CDCL_CLS_W1(c)) continue;
				//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
				if (IS_CDCL_VAR_UNASSIGNED(abs(lit))){
					//We have found an unassigned literal that can serve as watcher.
					SET_CDCL_CLS_W2(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CDCL_CLS_W2(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CDCL_CLS_FROM_LIT_W2LIST(-lid, litPos);
				ADD_CDCL_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CDCL_CLS_W1(c);
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					#ifdef COLLECTSTATS
					stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
					#endif
					return c;
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = cdcl_variables + abs(lit);
					CDCL_UP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}
	}

	//We now reduce the clause database using the satisfied and unsatisfied propagations made.
	reduce = reduceFrom;
	while (reduce < cdcl_up_end){
		l = *(reduce++);
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence.
			c = GET_CDCL_LIT_PTR_OCCNUM(l, 0);
			//We first disable the clause, then signal the disabling to the queues and stack, and then delete the clause.
			cdcl_inprocessingRules_intern_disableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
			cdcl_inprocessingRules_intern_deleteClause(c);
		}
	}

	//We now reduce the clauses by removing false literals.
	reduce = reduceFrom;
	while (reduce < cdcl_up_end){
		l = *(reduce++);
		litToRemove = -GET_CDCL_LIT_ID_FROM_PTR(l);
		l = cdcl_literals + litToRemove;
		//As long as literal l has more occurrences, we must continue to remove this literal from the occurrence.
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence. Grab the position of the literal in this occurrence.
			c = GET_CDCL_LIT_PTR_OCCNUM(l, 0);
			occLitPos = GET_CDCL_LIT_PTR_OCCLITPOSNUM(l, 0);
			//First, we remove the clause from the occurrence list of l. Clause c must be found at position 0 in the
			//occurrence list of l.
			REM_CDCL_LIT_PTR_OCC(l, 0);
			//We must now remove l at position occLitPos in clause c.
			REM_CDCL_CLS_LIT_ID_AT_POS(c, occLitPos);
			//If the literal was the representative, we must replace it.
			if (GET_CDCL_CLS_REPLIT(c) == litToRemove){
				SET_CDCL_CLS_REPLIT(c, GET_CDCL_CLS_LITNUM(c,0));
			}
			//If the clause became binary, we must now remove it from the watcher lists and add the implication the clause
			//imposes to the implication lists.
			if (GET_CDCL_CLS_SIZE(c) == 2){
				//Yes, the clause is now binary.
				litID = GET_CDCL_CLS_W1(c);
				litPos = GET_CDCL_CLS_WHEREW1(c);
				REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
				litID = GET_CDCL_CLS_W2(c);
				litPos = GET_CDCL_CLS_WHEREW2(c);
				REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
				ADD_CDCL_LIT_IMPLICATIONS( c, impLitA , impLitB );
			} else {
				//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
				//need to find a replacement.
				if (GET_CDCL_CLS_W1(c) == litToRemove){
					//Yes, we killed the first watcher and must replace it with something not the second watcher.
					litID = GET_CDCL_CLS_W1(c);
					litPos = GET_CDCL_CLS_WHEREW1(c);
					REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
						litID = GET_CDCL_CLS_LITNUM(c,litPos);
						if (litID == GET_CDCL_CLS_W2(c) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old first watcher.
						SET_CDCL_CLS_W1(c, litID);
						ADD_CDCL_CLS_TO_LIT_W1LIST(c, litID);
						break;
					}
				} else if (GET_CDCL_CLS_W2(c) == litToRemove){
					//Yes, we killed the second watcher and must replace it with something not the first watcher.
					litID = GET_CDCL_CLS_W2(c);
					litPos = GET_CDCL_CLS_WHEREW2(c);
					REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
						litID = GET_CDCL_CLS_LITNUM(c,litPos);
						if (litID == GET_CDCL_CLS_W1(c) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old second watcher.
						SET_CDCL_CLS_W2(c, litID);
						ADD_CDCL_CLS_TO_LIT_W2LIST(c, litID);
						break;
					}
				}
			}
			//We have removed the literal occurrence of l. We must now update the signature of the reduced clause.
			sig = 0ULL;
			for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c,litPos));
				v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,litPos));
				if (GET_CDCL_VAR_PTR_OFFSET(v) > 0U) cdcl_selectVarRule_incAct(v);
			}
			SET_CDCL_CLS_SIG(c, sig);
			if (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_LBD(c)){
				SET_CDCL_CLS_LBD(c, GET_CDCL_CLS_SIZE(c));
			}
			//We signal to all queues and stacks that this clause lost the literal.
			cdcl_inprocessingRules_intern_QSUpdateLiteralDeletion(c,litToRemove);
		}
	}

	cdcl_inprocessingRules_reduceFrom = cdcl_up_end;

	//In case no conflict occurred, we can terminate the current decision level and return NULL (no conflict clause).
	CDCL_UP_TERMINATE_CURRENT();
	#ifdef COLLECTSTATS
	stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
	return NULL;
}

void cdcl_inprocessingRules_intern_reduceDB(){
	//This method assumes that we just propagated some variables in DL0. It check on all the assignments made in DL0 from
	//the reduceFrom position onwards. For each such assignment, we will remove all the satisfied clauses from the lists. Then
	//we check on all the clauses containing a literal with opposite sign and remove those literals from the clauses (which
	//means that we effectively remove the falsified literals from the formula).

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	uint64_t numRemCls = 0;
	uint64_t numRemLit = 0;
	++stats_cdcl_numReductions;
	#endif

	cdclClause *c;
	cdclLiteral *l, **dlPos = cdcl_inprocessingRules_reduceFrom;
	cdclVariable *v;
	uint64_t sig;
	uint32_t litPos, occLitPos;
	int32_t litID, impLitA, impLitB, litToRemove;

	//If nothing can be reduced, then we return right away.
	if (*cdcl_inprocessingRules_reduceFrom == NULL) return;

	//First, we handle the removal of satisfied clauses. Grab one assignment after the other to remove the clauses in which
	//the literal appears satisfied.
	while ((l = *dlPos) != NULL){
		//As long as literal l has more occurrences, we must continue to remove occurrences.
		#ifdef COLLECTSTATS
		numRemCls += GET_CDCL_LIT_PTR_NUMOCCS_USED(l);
		#endif
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence.
			c = GET_CDCL_LIT_PTR_OCCNUM(l, 0);
			cdcl_inprocessingRules_intern_disableClause(c);
			cdcl_inprocessingRules_intern_deleteClause(c);
		}

		//Go on with the next DL0 assignment.
		++dlPos;
	}

	//Second, we handle the removal of literals that became false from the formula. Since we already removed all the clauses
	//that are satisfied, we must not worry that this literal is a false literal in a binary clauses (these clauses must
	//be satisfied by definition and have been removed by the loop above). In other words, all the clauses that lose a literal
	//now are not satisfied and have at least size 3. Furthermore, all these clauses have at least two unassigned literals.
	//It can, however, happen that the literal removal creates a binary clause, in which case we must remove this clause
	//from the watcher lists and add the implications it imposes.
	//Additionally, if a literal is removed, we must check if the literal was the representative. If so, we just override the
	//representative with the first literal in the clause.
	//All the clauses that receive an update here are subject to further tests and are put into the clause stack.

	//We again check all the assignments made in DL0 from the check position onwards.
	dlPos = cdcl_inprocessingRules_reduceFrom;
	while ((l = *dlPos) != NULL){
		//We invert the sign of the literal. The literal l is now the falsified literal.
		litToRemove = -GET_CDCL_LIT_ID_FROM_PTR(l);
		l = cdcl_literals + litToRemove;
		#ifdef COLLECTSTATS
		numRemLit += GET_CDCL_LIT_PTR_NUMOCCS_USED(l);
		#endif
		//As long as literal l has more occurrences, we must continue to remove this literal from the occurrence.
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) > 0){
			//Grab the first remaining occurrence. Grab the position of the literal in this occurrence.
			c = GET_CDCL_LIT_PTR_OCCNUM(l, 0);
			occLitPos = GET_CDCL_LIT_PTR_OCCLITPOSNUM(l, 0);
			//First, we remove the clause from the occurrence list of l. Clause c must be found at position 0 in the
			//occurrence list of l.
			REM_CDCL_LIT_PTR_OCC(l, 0);
			//We must now remove l at position occLitPos in clause c.
			REM_CDCL_CLS_LIT_ID_AT_POS(c, occLitPos);
			//If the literal was the representative, we must replace it.
			if (GET_CDCL_CLS_REPLIT(c) == litToRemove){
				SET_CDCL_CLS_REPLIT(c, GET_CDCL_CLS_LITNUM(c,0));
			}
			//If the clause became binary, we must now remove it from the watcher lists and add the implication the clause
			//imposes to the implication lists.
			if (GET_CDCL_CLS_SIZE(c) == 2){
				//Yes, the clause is now binary.
				litID = GET_CDCL_CLS_W1(c);
				litPos = GET_CDCL_CLS_WHEREW1(c);
				REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
				litID = GET_CDCL_CLS_W2(c);
				litPos = GET_CDCL_CLS_WHEREW2(c);
				REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
				ADD_CDCL_LIT_IMPLICATIONS( c, impLitA , impLitB );
			} else {
				//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
				//need to find a replacement.
				if (GET_CDCL_CLS_W1(c) == litToRemove){
					//Yes, we killed the first watcher and must replace it with something not the second watcher.
					litID = GET_CDCL_CLS_W1(c);
					litPos = GET_CDCL_CLS_WHEREW1(c);
					REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
						litID = GET_CDCL_CLS_LITNUM(c,litPos);
						if (litID == GET_CDCL_CLS_W2(c) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old first watcher.
						SET_CDCL_CLS_W1(c, litID);
						ADD_CDCL_CLS_TO_LIT_W1LIST(c, litID);
						break;
					}
				} else if (GET_CDCL_CLS_W2(c) == litToRemove){
					//Yes, we killed the second watcher and must replace it with something not the first watcher.
					litID = GET_CDCL_CLS_W2(c);
					litPos = GET_CDCL_CLS_WHEREW2(c);
					REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
					//Search for a literal in the clause not yet assigned and not the second watcher.
					for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
						litID = GET_CDCL_CLS_LITNUM(c,litPos);
						if (litID == GET_CDCL_CLS_W1(c) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
						//The litID literal is now a replacement for the old second watcher.
						SET_CDCL_CLS_W2(c, litID);
						ADD_CDCL_CLS_TO_LIT_W2LIST(c, litID);
						break;
					}
				}
			}
			//We have removed the literal occurrence of l. We must now update the signature of the reduced clause.
			sig = 0ULL;
			for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c,litPos));
				v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,litPos));
				if (GET_CDCL_VAR_PTR_OFFSET(v) > 0U) cdcl_selectVarRule_incAct(v);
			}
			SET_CDCL_CLS_SIG(c, sig);
			if (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_LBD(c)){
				SET_CDCL_CLS_LBD(c, GET_CDCL_CLS_SIZE(c));
			}
			cdcl_inprocessingRules_intern_QSUpdateLiteralDeletion(c,litToRemove);
		}
		//We have now removed literal l from all clauses, replaced the watchers or moved the binary clauses to the implication
		//lists. We also updated the representative literal of the clauses. Go on with the next literal.
		++dlPos;
	}

	//After the reduction is completed, we set the new reduction position to the end of DL0.
	cdcl_inprocessingRules_reduceFrom = dlPos;

	#ifdef COLLECTSTATS
	stats_cdcl_numRedSatCls += numRemCls;
	stats_cdcl_numRedFalsLits += numRemLit;
	stats_cdcl_time_reduce += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}

void cdcl_inprocessingRules_intern_disableClause(cdclClause* c){
	//This is used to temporarily disable a clause. In this case, disabling means that the clause is removed from all the
	//watcher, occurrence and implication lists. This must only be called on ENABLED clauses.
	cdclLiteral *litInC;
	uint32_t litPos, occPos;
	int32_t litID;

	//First, we handle the occurrence lists for all the literals in c.
	for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
		//Grab the literal in clause c at position litPos.
		litInC = cdcl_literals + GET_CDCL_CLS_LITNUM(c, litPos);
		//Grab the position of clause c in the occurrence list of literal litInC.
		occPos = GET_CDCL_CLS_OCCPOSNUM(c, litPos);
		//Remove the clause from the occurrence list of litInC.
		REM_CDCL_LIT_PTR_OCC(litInC, occPos);
	}
	//Clause c has now been removed from all occurrence lists. If clause c has size two, we must remove it from the
	//implication lists. Otherwise, we must remove it from the watcher lists.
	if (GET_CDCL_CLS_SIZE(c) == 2){
		//The clause is binary, we remove it from the implication lists.
		REM_CDCL_LIT_IMPLICATIONS(c, litID, litPos);
	} else {
		//The clause is not binary. We remove it from the watcher lists.
		litID = GET_CDCL_CLS_W1(c);
		litPos = GET_CDCL_CLS_WHEREW1(c);
		REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
		litID = GET_CDCL_CLS_W2(c);
		litPos = GET_CDCL_CLS_WHEREW2(c);
		REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
	}
}

void cdcl_inprocessingRules_intern_enableClause(cdclClause* c){
	//This is used to re-enable a clause. It is again added to all the lists. This must only be called on DISABLED clauses.
	uint32_t litPos;
	int32_t litID, impLitA, impLitB;

	//First, we handle the occurrence lists for all the literals in c.
	for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(c); ++litPos){
		//Grab the literal in clause c at position litPos.
		litID = GET_CDCL_CLS_LITNUM(c, litPos);
		//Add clause c to the occurrence list and mark the position of the literal at litPos.
		ADD_CDCL_CLS_TO_LIT_OCCS(c, litID, litPos);
	}

	//Depending on the size of the clause, we must either add it to the implications or to the watcher lists.
	if (GET_CDCL_CLS_SIZE(c) == 2){
		//The clause is binary. We must add it to the implication lists.
		ADD_CDCL_LIT_IMPLICATIONS(c, impLitA, impLitB);
	} else {
		//The clause is large-size. We must add it to the watcher lists.
		litID = GET_CLS_W1(c);
		ADD_CDCL_CLS_TO_LIT_W1LIST(c, litID);
		litID = GET_CLS_W2(c);
		ADD_CDCL_CLS_TO_LIT_W2LIST(c, litID);
	}
	//The clause remains at its current position in the clauses array.
}

void cdcl_inprocessingRules_intern_makeClauseRemoved(cdclClause *c, int32_t lit, uint32_t byAction){
	//This method makes the given clause a removed one. This clause MUST BE DISABLED for this to work.
	//We first ensure that we can add this clause to the removed clauses array.
	remCdclClause *rc;
	uint32_t i;
	ENSURE_CDCL_CLS_REMOVED_AVAIL_FOR(1U, i, rc);

	rc = cdcl_cls_r[cdcl_cls_r_used++];
	rc->byAction = byAction;
	rc->litOfInterest = lit;
	rc->size = c->size;
	rc->lits = realloc(rc->lits, sizeof(int32_t)*(rc->size));
	for (i=0; i < GET_CDCL_CLS_SIZE(c); ++i){
		rc->lits[i] = c->lits[i];
	}
}

void cdcl_inprocessingRules_intern_addLearnedBinaryClause(int32_t lit1, int32_t lit2){
	//Adds a new binary as learned clause.
	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;

	//First, we ensure that we can learn a new clause of sufficient size.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, 3);
	SET_CDCL_CLS_SIZE(newClause, 2);
	SET_CDCL_CLS_ACTIVITY(newClause, ONE);
	SET_CDCL_CLS_LBD(newClause, 2);

	//Set the clauses literals. Terminate with 0.
	SET_CDCL_CLS_LITNUM(newClause, 0, lit1);
	SET_CDCL_CLS_LITNUM(newClause, 1, lit2);
	SET_CDCL_CLS_LITNUM(newClause, 2, 0);

	//Set the clauses representative.
	SET_CDCL_CLS_REPLIT(newClause, lit1);

	//Set the clauses signature.
	sig |= GET_CDCL_LIT_SIG(lit1);
	sig |= GET_CDCL_LIT_SIG(lit2);
	SET_CDCL_CLS_SIG(newClause, sig);

	//Add the clause to the occurrence lists.
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit1, 0);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit2, 1);

	//Add the clauses implications.
	ADD_CDCL_LIT_IMPLICATIONS(newClause, lit1, lit2);

}

void cdcl_inprocessingRules_intern_addLearnedTernaryClause(int32_t lit1, int32_t lit2, int32_t lit3){
	//Adds a new learned clause of size 3.

	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;

	//First, we ensure that we can learn a new clause of sufficient size.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, 4);
	SET_CDCL_CLS_SIZE(newClause, 3);
	SET_CDCL_CLS_ACTIVITY(newClause, ONE);
	SET_CDCL_CLS_LBD(newClause, 3);

	//Set the clauses literals. Terminate with 0.
	SET_CDCL_CLS_LITNUM(newClause, 0, lit1);
	SET_CDCL_CLS_LITNUM(newClause, 1, lit2);
	SET_CDCL_CLS_LITNUM(newClause, 2, lit3);
	SET_CDCL_CLS_LITNUM(newClause, 3, 0);

	//Set the clauses representative.
	SET_CDCL_CLS_REPLIT(newClause, lit1);

	//Set the clauses signature.
	sig |= GET_CDCL_LIT_SIG(lit1);
	sig |= GET_CDCL_LIT_SIG(lit2);
	sig |= GET_CDCL_LIT_SIG(lit3);
	SET_CDCL_CLS_SIG(newClause, sig);

	//Add the clause to the occurrence lists.
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit1, 0);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit2, 1);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit3, 2);

	//Add the clauses watchers.
	SET_CDCL_CLS_W1(newClause, lit1);
	ADD_CDCL_CLS_TO_LIT_W1LIST(newClause, lit1);
	SET_CDCL_CLS_W2(newClause, lit2);
	ADD_CDCL_CLS_TO_LIT_W2LIST(newClause, lit2);
	cdcl_maintenanceRule_incAct(newClause);
}

void cdcl_inprocessingRules_intern_addLearnedQuadriaryClause(int32_t lit1, int32_t lit2, int32_t lit3, int32_t lit4){
	//Adds a new learned clause of size 4.

	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;

	//First, we ensure that we can learn a new clause of sufficient size.
	ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_l[cdcl_cls_l_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, 5);
	SET_CDCL_CLS_SIZE(newClause, 4);
	SET_CDCL_CLS_ACTIVITY(newClause, ONE);
	SET_CDCL_CLS_LBD(newClause, 4);

	//Set the clauses literals. Terminate with 0.
	SET_CDCL_CLS_LITNUM(newClause, 0, lit1);
	SET_CDCL_CLS_LITNUM(newClause, 1, lit2);
	SET_CDCL_CLS_LITNUM(newClause, 2, lit3);
	SET_CDCL_CLS_LITNUM(newClause, 3, lit4);
	SET_CDCL_CLS_LITNUM(newClause, 4, 0);

	//Set the clauses representative.
	SET_CDCL_CLS_REPLIT(newClause, lit1);

	//Set the clauses signature.
	sig |= GET_CDCL_LIT_SIG(lit1);
	sig |= GET_CDCL_LIT_SIG(lit2);
	sig |= GET_CDCL_LIT_SIG(lit3);
	sig |= GET_CDCL_LIT_SIG(lit4);
	SET_CDCL_CLS_SIG(newClause, sig);

	//Add the clause to the occurrence lists.
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit1, 0);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit2, 1);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit3, 2);
	ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lit4, 3);

	//Add the clauses watchers.
	SET_CDCL_CLS_W1(newClause, lit1);
	ADD_CDCL_CLS_TO_LIT_W1LIST(newClause, lit1);
	SET_CDCL_CLS_W2(newClause, lit2);
	ADD_CDCL_CLS_TO_LIT_W2LIST(newClause, lit2);
	cdcl_maintenanceRule_incAct(newClause);
}

void cdcl_inprocessingRules_intern_addOriginalClause(int32_t *lits,int32_t size){
	//Adds a new clause of arbitrary size as original clause.
	cdclClause *newClause;
	uint64_t sig = 0ULL;
	uint32_t count;
	int32_t lit1, lit2;

	//First, we ensure that we can add a new clause of sufficient size.
	ENSURE_CDCL_CLS_ORIGINAL_AVAIL_FOR(1U, count, newClause);
	newClause = cdcl_cls_o[cdcl_cls_o_used++];
	REALLOC_CDCL_CLS_TO_SIZE(newClause, size+1);
	SET_CDCL_CLS_SIZE(newClause, size);
	SET_CDCL_CLS_ACTIVITY(newClause, ZERO);
	SET_CDCL_CLS_LBD(newClause, size);

	//We distinct between the two cases of a binary and large clause.

	if (size > 2){
		//Set the clauses literals. Terminate with 0.
		for (count = 0; count < size; ++count){
			SET_CDCL_CLS_LITNUM(newClause, count, lits[count]);
			sig |= GET_CDCL_LIT_SIG(lits[count]);
			ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lits[count], count);
		}
		SET_CDCL_CLS_LITNUM(newClause, size, 0);

		//Set the clauses signature and representative literal.
		SET_CDCL_CLS_SIG(newClause, sig);
		SET_CDCL_CLS_REPLIT(newClause, lits[0]);

		//Set the clauses first watcher.
		SET_CDCL_CLS_W1(newClause, lits[0]);
		ADD_CDCL_CLS_TO_LIT_W1LIST(newClause, lits[0]);

		//Set the clauses second watcher.
		SET_CDCL_CLS_W2(newClause, lits[1]);
		ADD_CDCL_CLS_TO_LIT_W2LIST(newClause, lits[1]);
	} else {
		//Set the clauses literals and terminate with 0.
		SET_CDCL_CLS_LITNUM(newClause, 0, lits[0]);
		sig |= GET_CDCL_LIT_SIG(lits[0]);
		ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lits[0], 0);
		SET_CDCL_CLS_LITNUM(newClause, 1, lits[1]);
		sig |= GET_CDCL_LIT_SIG(lits[1]);
		ADD_CDCL_CLS_TO_LIT_OCCS(newClause, lits[1], 1);
		SET_CDCL_CLS_LITNUM(newClause, 2, 0);

		//Set the clauses signature and representative literal.
		SET_CDCL_CLS_SIG(newClause, sig);
		SET_CDCL_CLS_REPLIT(newClause, lits[0]);

		//Add the binary clause implications.
		ADD_CDCL_LIT_IMPLICATIONS(newClause, lit1, lit2);
	}
}

void cdcl_inprocessingRules_intern_deleteClause(cdclClause* c){
	//This is used to delete a clause. We can only delete learned clauses. Originals are not touched by this. The clause MUST
	//be disabled for this to work!
	cdclClause *d;
	uint32_t occPos;
	if (IS_CDCL_CLS_LEARNED(c)){
		//Clause c was learned. We must remove clause c. If clause c is the last learned clause, we just shrink it
		//and thats it. If it is not the last learned clause, we must shuffle around a replacement clause.
		occPos = GET_CDCL_CLS_OFFSET(c);
		d = cdcl_cls_l[--cdcl_cls_l_used];
		if (c == d){
			//Clause c is the last learned clause. It suffices to shrink it in place.
			SHRINK_LEARNED_CDCL_CLS_INPLACE(c);
		} else {
			//Clause c is not the last learned clause. We shrink clause c such that it appears freshly initialized
			//and place it to where d was (to the end).
			SHRINK_LEARNED_CDCL_CLS(c, cdcl_cls_l_used);
			//Finally, we put d to the position where c was.
			cdcl_cls_l[occPos] = d;
			SET_CDCL_CLS_OFFSET(d, occPos);
		}
	} else {
		//Clause c was original. We must remove clause c. If clause c is the last original clause, we just shrink it
		//and thats it. If it is not the last original clause, we must shuffle around a replacement.
		occPos = GET_CDCL_CLS_OFFSET(c);
		d = cdcl_cls_o[--cdcl_cls_o_used];
		if (c == d){
			//Clause c is the last original clause. It suffices to shrink it in place.
			SHRINK_ORIGINAL_CDCL_CLS_INPLACE(c);
		} else {
			//Clause c is not the last original clause. We shrink clause c such that it appears freshly initialized
			//and place it to where d was (to the end).
			SHRINK_ORIGINAL_CDCL_CLS(c, cdcl_cls_o_used);
			//Finally, we put d to the position where c was.
			cdcl_cls_o[occPos] = d;
			SET_CDCL_CLS_OFFSET(d, occPos);
		}
	}
	//The clause has been removed from the clauses array.
}

void cdcl_inprocessingRules_intern_disableVariable(cdclVariable* v){
	cdcl_literals_Q_delete(cdcl_literals + GET_CDCL_VAR_PTR_ID(v));
	cdcl_literals_Q_delete(cdcl_literals - GET_CDCL_VAR_PTR_ID(v));
	cdcl_literals_S_delete(cdcl_literals + GET_CDCL_VAR_PTR_ID(v));
	cdcl_literals_S_delete(cdcl_literals - GET_CDCL_VAR_PTR_ID(v));
	cdcl_variables_S_delete(v);
	cdcl_variables_Q_delete(v);
}

void cdcl_inprocessingRules_intern_se(){
	//Performs SE with all the clauses in the clause queue.
	cdclClause *c, *d;
	cdclLiteral *l = NULL;
	uint32_t num, counted, numD, occSize = UINT32_MAX;

	#ifdef VERBOSE_CDCL
	printf("SE");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	while ((c = cdcl_clauses_Q_dequeueBest()) != NULL){
		//Search for the smallest occurrence list in clause c.
		++cdcl_litStamp;
		for (num = 0; num < GET_CDCL_CLS_SIZE(c); ++num){
			SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,num), cdcl_litStamp);
			if (GET_CDCL_LIT_NUMOCCS_USED(GET_CDCL_CLS_LITNUM(c,num)) < occSize){
				l = cdcl_literals + GET_CDCL_CLS_LITNUM(c,num);
				occSize = GET_CDCL_LIT_PTR_NUMOCCS_USED(l);
			}
		}
		//Literal l is now the literal with smallest occurrence list. We now walk through this list and disable/delete all clauses
		//that clause c subsumes (ignoring c itself of course).
		for (num = 0; num < GET_CDCL_LIT_PTR_NUMOCCS_USED(l); ++num){
			d = GET_CDCL_LIT_PTR_OCCNUM(l, num);
			//In order to fast rule out some clauses that cannot be subsumed with c, we check the size and signature of d.
			if (c == d || (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_SIZE(d)) || (GET_CDCL_CLS_SIG(c) & ~GET_CDCL_CLS_SIG(d)) != 0ULL ){
				//Either c is larger than d or the signature of c contains something not found in d. Either case: no subsumption.
				continue;
			}
			//So far, none of the checks ruled out that c could subsume d. We now count the literals in d that are found in c.
			//If this count equals the number of literals we have in c we have found all literals in c in clause d.
			counted = 0;
			for (numD = 0; numD < GET_CDCL_CLS_SIZE(d); ++numD){
				if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d, numD)) == cdcl_litStamp){
					if (++counted == GET_CDCL_CLS_SIZE(c)) break;
				}
			}

			//Check if the count equals the size of c.
			if (counted != GET_CDCL_CLS_SIZE(c)){
				//No, at least one literal was not found.
				continue;
			}

			//We did not abort early which means that all the literals in c where found in d as well. Subsumption.
			//We must now distinct between two different cases.
			if ((IS_CDCL_CLS_LEARNED(c) && !IS_CDCL_CLS_LEARNED(d))){
				#ifdef VERBOSE_CDCL
				remLits += (GET_CDCL_CLS_SIZE(d) - GET_CDCL_CLS_SIZE(c));
				#endif
				//A learned clause is able to subsume an original clause. We align the original clause to be equal to learned
				//clause that is able to subsume it. Afterwards, we drop the learned clause c, add d to the clause stack for
				//further investigations and return. We disable the original clause temporarily, remove all the literals that
				//are not stamped, and enable the clause again.
				cdcl_inprocessingRules_intern_disableClause(d);
				//Remove literals not stamped.
				for (numD = 0; numD < GET_CDCL_CLS_SIZE(d); ++numD){
					//If the variable is not stamped, we do not want to keep it in this clause. We override it with the last.
					if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d, numD)) != cdcl_litStamp){
						SET_CDCL_CLS_SIZE_DEC(d);
						SET_CDCL_CLS_LITNUM(d, numD, GET_CDCL_CLS_LITNUM(d, GET_CDCL_CLS_SIZE(d)));
						--numD;
					}
				}
				//We update the clause data: representative and watchers.
				SET_CDCL_CLS_SIG(d, GET_CDCL_CLS_SIG(c));
				SET_CDCL_CLS_W1(d, GET_CDCL_CLS_W1(c));
				SET_CDCL_CLS_W2(d, GET_CDCL_CLS_W2(c));
				SET_CDCL_CLS_REPLIT(d, GET_CDCL_CLS_REPLIT(c));
				cdcl_inprocessingRules_intern_enableClause(d);
				cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(d);
				cdcl_inprocessingRules_intern_disableClause(c);
				cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
				cdcl_inprocessingRules_intern_deleteClause(c);
				break;
			} else {
				cdcl_inprocessingRules_intern_disableClause(d);
				cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(d);
				cdcl_inprocessingRules_intern_deleteClause(d);
			}
			//Since we removed the occurrence of literal l, we must decrease the counter.
			--num;
		}
	}

	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
}

cdclClause* cdcl_inprocessingRules_intern_str(){
	//Performs STR with all the clauses in the clause queue.
	cdclClause *c, *d, *conflict = NULL;
	cdclLiteral *lOpp;
	cdclVariable *v;
	uint64_t sig, sigNew, litSig;
	int32_t lit, litID, impLitA, impLitB;
	uint32_t num, occNum, numC, numD, occLitPos, litPos, found, unitFound;

	#ifdef VERBOSE_CDCL
	printf("STR");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	while ((c = cdcl_clauses_Q_dequeueBest()) != NULL){
		//For each literal in clause c.
		unitFound = 0;
		for (num = 0; num < GET_CDCL_CLS_SIZE(c); ++num){
			//Grab the opposite literal.
			lit = GET_CDCL_CLS_LITNUM(c, num);
			litSig = GET_CDCL_LIT_SIG(lit);
			lOpp = cdcl_literals - lit;
			//We create a signature that does not contain the literal we resolve on.
			sig = GET_CDCL_CLS_SIG(c);
			if (GET_CDCL_CLS_SIZE(c) == POPCOUNT_UINT64(sig)){
				//Since each literal has its own bit in the signature, we can kill the bit of resLit.
				sig &= ~litSig;
			} else {
				//We must find out the hard way if there is a literal in c different from resLit that has the same signature.
				found = 0;
				for (numC = 0; numC < GET_CDCL_CLS_SIZE(c); ++numC){
					if (GET_CDCL_CLS_LITNUM(c,numC) != lit && GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c,numC)) == litSig){
						++found;
						break;
					}
				}
				if (!found){
					//We have not found another literal that causes the same signature as lit. We can kill the bit.
					sig &= ~litSig;
				}
			}
			//For all occurrences of the opposite literal.
			for (occNum = 0; occNum < GET_CDCL_LIT_PTR_NUMOCCS_USED(lOpp); ++occNum){
				//Grab the occurrence of the opposite literal to get a possible resolution partner.
				d = GET_CDCL_LIT_PTR_OCCNUM(lOpp, occNum);
				//Check that c is not too large and that the signature of c does not contain anything not in d.
				if (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_SIZE(d) ||(sig & ~GET_CDCL_CLS_SIG(d))!=0ULL){
					continue;
				}
				//The simple checks did not rule out clause d. We must check if all the literals from c if they are found in d.
				found = 0;
				for (numC = 0; numC < GET_CDCL_CLS_SIZE(c); ++numC){
					if (GET_CDCL_CLS_LITNUM(c,numC) == lit) {
						continue;//We ignore the literal we resolve on.
					}
					found = 0;
					for (numD = 0; numD < GET_CDCL_CLS_SIZE(d); ++numD){
						if (GET_CDCL_CLS_LITNUM(c,numC) == -GET_CDCL_CLS_LITNUM(d,numD)){
							//The literal from c, which is not the literal to resolve on, has also been found in d with
							//opposite sign. This strengthening would end in a tautology.
							found = 0; break;
						} else if (GET_CDCL_CLS_LITNUM(c,numC) == GET_CDCL_CLS_LITNUM(d,numD)){
							//We have found the literal.
							++found;
							break;
						}
					}
					if (!found){
						break;
					}
				}
				if (!found) continue;
				//Here a strengthening operation is possible by removing -lit from d.
				//We check what the result will be -- especially if d is of size two, we get a unit from it.
				if (GET_CDCL_CLS_SIZE(d) == 2){
					//Yes, the result will be a unit. We first pick the variable to propagate (being not -lit).
					if (GET_CDCL_CLS_LITNUM(d,0) == -lit){
						litID = GET_CDCL_CLS_LITNUM(d,1);
					} else {
						litID = GET_CDCL_CLS_LITNUM(d,0);
					}
					v = cdcl_variables + abs(litID);
					cdcl_inprocessingRules_backJump(0,0);
					CDCL_UP_ADD_DEC(litID,v);
					conflict = cdcl_inprocessingRules_upPropagate();
					unitFound = 1;
					break;
				} else {
					//The result will not be unit.
					//First, we remove the clause from the occurrence list of -lit.
					occLitPos = GET_CDCL_LIT_PTR_OCCLITPOSNUM(lOpp, occNum);
					REM_CDCL_LIT_OCC(-lit, occNum);
					//We must now remove -lit at position occLitPos in clause c.
					REM_CDCL_CLS_LIT_ID_AT_POS(d, occLitPos);
					//If the literal was the representative, we must replace it.
					if (GET_CDCL_CLS_REPLIT(d) == -lit){
						SET_CDCL_CLS_REPLIT(d, GET_CDCL_CLS_LITNUM(d,0));
					}

					#ifdef VERBOSE_CDCL
					++remLits;
					#endif
					//The clause just lost its literal.
				}

				//Handle the clause according to its new size.
				if (GET_CDCL_CLS_SIZE(d) == 2){
					//Yes, the clause is now binary.
					litID = GET_CDCL_CLS_W1(d);
					litPos = GET_CDCL_CLS_WHEREW1(d);
					REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
					litID = GET_CDCL_CLS_W2(d);
					litPos = GET_CDCL_CLS_WHEREW2(d);
					REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
					ADD_CDCL_LIT_IMPLICATIONS(d, impLitA , impLitB );
				} else {
					//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
					//need to find a replacement.
					if (GET_CDCL_CLS_W1(d) == -lit){
						//Yes, we killed the first watcher and must replace it with something not the second watcher.
						litID = GET_CDCL_CLS_W1(d);
						litPos = GET_CDCL_CLS_WHEREW1(d);
						REM_CDCL_CLS_FROM_LIT_W1LIST(litID, litPos);
						//Search for a literal in the clause not yet assigned and not the second watcher.
						for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(d); ++litPos){
							litID = GET_CDCL_CLS_LITNUM(d,litPos);
							if (litID == GET_CDCL_CLS_W2(d) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
							//The litID literal is now a replacement for the old first watcher.
							SET_CDCL_CLS_W1(d, litID);
							ADD_CDCL_CLS_TO_LIT_W1LIST(d, litID);
							break;
						}
					} else if (GET_CDCL_CLS_W2(d) == -lit){
						//Yes, we killed the second watcher and must replace it with something not the first watcher.
						litID = GET_CDCL_CLS_W2(d);
						litPos = GET_CDCL_CLS_WHEREW2(d);
						REM_CDCL_CLS_FROM_LIT_W2LIST(litID, litPos);
						//Search for a literal in the clause not yet assigned and not the second watcher.
						for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(d); ++litPos){
							litID = GET_CDCL_CLS_LITNUM(d,litPos);
							if (litID == GET_CDCL_CLS_W1(d) || IS_CDCL_LIT_ASSIGNED(litID)) continue;
							//The litID literal is now a replacement for the old second watcher.
							SET_CDCL_CLS_W2(d, litID);
							ADD_CDCL_CLS_TO_LIT_W2LIST(d, litID);
							break;
						}
					}
				}
				//We have removed the literal occurrence of -lit. We must now update the signature of the reduced clause.
				if (POPCOUNT_UINT64(GET_CDCL_CLS_SIG(d)) == GET_CDCL_CLS_SIZE(d)+1){
					sigNew = GET_CDCL_CLS_SIG(d) & ~GET_CDCL_LIT_SIG(-lit);
				} else {
					sigNew = 0ULL;
					for (litPos = 0; litPos < GET_CDCL_CLS_SIZE(d); ++litPos){
						sigNew |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(d,litPos));
					}
				}
				SET_CDCL_CLS_SIG(d, sigNew);
				if (GET_CDCL_CLS_SIZE(d) > GET_CDCL_CLS_LBD(d)){
					SET_CDCL_CLS_LBD(d, GET_CDCL_CLS_SIZE(d));
				}
				cdcl_inprocessingRules_intern_QSUpdateLiteralDeletion(d, -lit);
				--occNum;
			}
			if (unitFound) break;
		}
		if (conflict != NULL) break;
	}

	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}

cdclClause* cdcl_inprocessingRules_intern_ple(){
	//This method performs pure literal elimination. First, we back-jump to decision level 0 in APPEND mode. All assignments
	//that we will now identify will be propagated at the end of this method. This can, of course, not end in a conflict.
	cdclVariable *v;
	cdclClause *conflict;
	cdclLiteral *lPos, *lNeg;

	#ifdef VERBOSE_CDCL
	printf("PLE");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	//First, we back-jump to decision level 0. This also updates the UP stamp.
	cdcl_inprocessingRules_backJump(0,0);

	//We now check for all the variables that are still enabled (not assigned), if they are pure. If so, we will put them on
	//the stack. Since PLE cannot end in a conflict, we do not have to worry about this.
	while ((v = cdcl_variables_S_pop()) != NULL){
		//Grab the next variable. If this variable has no occurrences to the positive or negative side, then propagate the
		//respectively other side.
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		//We do not bother working with v if it has no occurrences anymore.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) == 0U && GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg) == 0U) continue;
		//We first check, if the positive occurrences are empty.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) == 0U){
			//The positive side has no occurrences, we propagate the negative side.
			CDCL_UP_ADD_DEC_PTR(lNeg, v);
		} else if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg) == 0U){
			//The negative side has no occurrences, we propagate the positive side.
			CDCL_UP_ADD_DEC_PTR(lPos, v);
		}
	}

	//After collecting variables (or not), we call for UP to assign the variables and terminate DL0 again.
	conflict = cdcl_inprocessingRules_upPropagate();
	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}

cdclClause* cdcl_inprocessingRules_intern_fld_testPropagate(cdclLiteral* l){
	//This method performs a test-wise unit propagation of literal l. We assume that the current DL is empty and use it for
	//the test propagations.
	cdclVariable *v;
	cdclClause *c;
	cdclLiteral **tempUPcurrent = cdcl_up_current;
	cdclLiteral **tempUPend = cdcl_up_end;
	int32_t lid, lit, i, offset, litPos;

	++cdcl_up_stamp;

	//Add our test-literal to the stack.
	*(tempUPend++) = l;
	SET_CDCL_LIT_PTR_UPSTAMP(l, cdcl_up_stamp);

	while (tempUPcurrent < tempUPend){
		//We grab the next literal from the stack and assign it accordingly.
		l = *(tempUPcurrent++);
		lid = GET_CDCL_LIT_ID_FROM_PTR(l);
		if (lid < 0){
			//The literal is negative. The variable must be assigned to 0.
			v = cdcl_variables - lid;
			SET_CDCL_VAR_PTR_ASS_0(v);
		} else {
			//The literal is positive. The variable must be assigned to 1.
			v = cdcl_variables + lid;
			SET_CDCL_VAR_PTR_ASS_1(v);
		}

		//We check all the implications of this literal.
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i+=2){
			lit = GET_CDCL_LIT_PTR_IMPNUM(l, i);
			v = cdcl_variables + abs(lit);
			if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					c = GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i);
					//Clean-up.
					while (tempUPcurrent > cdcl_up_current){
						l = *(--tempUPcurrent);
						lid = GET_CDCL_LIT_ID_FROM_PTR(l);
						if (lid < 0){
							v = cdcl_variables - lid;
						} else {
							v = cdcl_variables + lid;
						}
						SET_CDCL_VAR_PTR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++cdcl_up_stamp;
					return c;
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_CDCL_LIT_UPSTAMP(lit, cdcl_up_stamp);
					*(tempUPend++) = ( cdcl_literals + lit );
					//No need to propagate a literal more than once.
					cdcl_literals_Q_delete(cdcl_literals + lit);
				} //Else, the literal is already enqueued.
			}
		}

		//We check the watcher lists of the falsified literal.
		l = cdcl_literals - lid;

		//WL1
		offset = 0;
		while (GET_CDCL_LIT_PTR_NUMW1_USED(l) > offset){
			c = GET_CDCL_LIT_PTR_W1_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CDCL_CLS_REPLIT(c);
			if (IS_CDCL_LIT_SAT(lit) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W2(c))){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CDCL_CLS_WHEREW1(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
				lit = GET_CDCL_CLS_LITNUM(c,i);
				if (IS_CDCL_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CDCL_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the second watcher.
				if (lit == GET_CDCL_CLS_W2(c)) continue;
				//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
				if (IS_CDCL_VAR_UNASSIGNED(abs(lit))){
					//We have found an unassigned literal that can serve as watcher.
					SET_CDCL_CLS_W1(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CDCL_CLS_W1(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CDCL_CLS_FROM_LIT_W1LIST(-lid, litPos);
				ADD_CDCL_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CDCL_CLS_W2(c);
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					//Clean-up.
					while (tempUPcurrent > cdcl_up_current){
						l = *(--tempUPcurrent);
						lid = GET_CDCL_LIT_ID_FROM_PTR(l);
						if (lid < 0){
							v = cdcl_variables - lid;
						} else {
							v = cdcl_variables + lid;
						}
						SET_CDCL_VAR_PTR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++cdcl_up_stamp;
					return c;
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_CDCL_LIT_UPSTAMP(lit, cdcl_up_stamp);
					*(tempUPend++) = ( cdcl_literals + lit );
				} //Else, the literal is already on the stack.
			}
		}

		//WL2
		offset = 0;
		while (GET_CDCL_LIT_PTR_NUMW2_USED(l) > offset){
			c = GET_CDCL_LIT_PTR_W2_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CDCL_CLS_REPLIT(c);
			if (IS_CDCL_LIT_SAT(lit) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W1(c))){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CDCL_CLS_WHEREW2(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
				lit = GET_CDCL_CLS_LITNUM(c,i);
				if (IS_CDCL_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CDCL_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the first watcher.
				if (lit == GET_CDCL_CLS_W1(c)) continue;
				//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
				if (IS_CDCL_VAR_UNASSIGNED(abs(lit))){
					//We have found an unassigned literal that can serve as watcher.
					SET_CDCL_CLS_W2(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CDCL_CLS_W2(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CDCL_CLS_FROM_LIT_W2LIST(-lid, litPos);
				ADD_CDCL_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CDCL_CLS_W1(c);
				if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					//Clean-up.
					while (tempUPcurrent > cdcl_up_current){
						l = *(--tempUPcurrent);
						lid = GET_CDCL_LIT_ID_FROM_PTR(l);
						if (lid < 0){
							v = cdcl_variables - lid;
						} else {
							v = cdcl_variables + lid;
						}
						SET_CDCL_VAR_PTR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++cdcl_up_stamp;
					return c;
				} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_CDCL_LIT_UPSTAMP(lit, cdcl_up_stamp);
					*(tempUPend++) = ( cdcl_literals + lit );
				} //Else, the literal is already on the stack.
			}
		}

	}

	//We must undo all assignments in this test-propagation.
	//Clean-up.
	while (tempUPcurrent > cdcl_up_current){
		l = *(--tempUPcurrent);
		lid = GET_CDCL_LIT_ID_FROM_PTR(l);
		if (lid < 0){
			v = cdcl_variables - lid;
		} else {
			v = cdcl_variables + lid;
		}
		SET_CDCL_VAR_PTR_ASS_2(v);
	}
	*tempUPcurrent = NULL;
	*(tempUPcurrent+1) = NULL;
	++cdcl_up_stamp;
	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_fld(){
	//This method performs failed literal detection. For each of the enabled variables we check the literal if it has
	//implications. If so, we assign such that the implications are triggered in a test propagation. If this results in a
	//conflict, we must finally propagate the opposite. This can run into a conflict and if that happens, we must return
	//the conflict clause.

	cdclVariable *v;
	cdclLiteral *l;
	cdclClause *c = NULL;

	#ifdef VERBOSE_CDCL
	printf("FLD");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	while ((l = cdcl_literals_Q_dequeueBest()) != NULL){
		//Yes, we can test propagate this literal.
		if (GET_CDCL_LIT_PTR_NUMIMPS_USED(l) == 0) continue;
		c = cdcl_inprocessingRules_intern_fld_testPropagate(l);
		if (c != NULL){
			//Yes, the opposite MUST be enforced. We back-jump to DL0 in APPEND mode, append the literal and propagate.
			cdcl_inprocessingRules_backJump(0,0);
			l = cdcl_literals - GET_CDCL_LIT_ID_FROM_PTR(l);
			v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(l));
			CDCL_UP_ADD_DEC_PTR(l, v);
			c = cdcl_inprocessingRules_upPropagate();
			//Check if this resulted in a conflict.
			if (c != NULL){
				//Yes, the enforced assignment resulted in a conflict. We can stop.
				break;
			}
		}

	}

	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif

	return c;
}

int32_t cdcl_inprocessingRules_intern_unhiding_SSet_incDSC(const void* a, const void* b){
	//Sorts the literals with increasing discovery time. We swap if a has a larger discovery time.
	if ( GET_CDCL_LIT_PTR_DSC((*(cdclLiteral**)a)) > GET_CDCL_LIT_PTR_DSC((*(cdclLiteral**)b)) ) return 1;
	return -1;
}

int32_t cdcl_inprocessingRules_intern_unhiding_SSet_decDSC(const void* a, const void* b){
	//Sorts the literals with decreasing discovery time. We swap if a has a smaller discovery time.
	if ( GET_CDCL_LIT_PTR_DSC((*(cdclLiteral**)a)) < GET_CDCL_LIT_PTR_DSC((*(cdclLiteral**)b)) ) return 1;
	return -1;
}

void cdcl_inprocessingRules_intern_unhiding_uhle(cdclClause *c){
	//This performs an UHLE check on the given clause (which must be disabled). We begin by creating the SPos array in
	//reversed order.
	cdclLiteral *l;
	uint64_t sig = 0ULL;
	uint32_t finished, i, oldSize = GET_CDCL_CLS_SIZE(c), maxSize = GET_CDCL_CLS_SIZE(c), writePos;
	int32_t litID;
	for (i = 0; i < maxSize; ++i){
		l = cdcl_literals + GET_CDCL_CLS_LITNUM(c, i);
		cdcl_inprocessingRules_unhiding_SPlus[i] = l;
	}
	//We now sort the SPlus array to hold the literals with decreasing discovery time.
	qsort(cdcl_inprocessingRules_unhiding_SPlus, maxSize, sizeof(cdclLiteral*), cdcl_inprocessingRules_intern_unhiding_SSet_decDSC);
	finished = GET_CDCL_LIT_PTR_FIN(cdcl_inprocessingRules_unhiding_SPlus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = cdcl_inprocessingRules_unhiding_SPlus[i];
		if (GET_CDCL_LIT_PTR_FIN(l) > finished){
			//We do not keep the element in SPlus at position i. We just go on with the next literal in SPos.
		} else {
			//We want to keep l. We write it at the current writePos and go on.
			cdcl_inprocessingRules_unhiding_SPlus[writePos++] = l;
			finished = GET_CDCL_LIT_PTR_FIN(l);
		}
	}
	//We update the maxSize with the elements written (number of retained elements).
	maxSize = writePos;
	//We now create SMinus with all the retained elements, but invert their sign. We sort with increasing discovery time.
	for(i = 0; i < maxSize; ++i){
		cdcl_inprocessingRules_unhiding_SMinus[i] = GET_CDCL_LIT_PTR_OPPOSITE(cdcl_inprocessingRules_unhiding_SPlus[i]);
	}
	qsort(cdcl_inprocessingRules_unhiding_SMinus, maxSize, sizeof(cdclLiteral*), cdcl_inprocessingRules_intern_unhiding_SSet_incDSC);
	finished = GET_CDCL_LIT_PTR_FIN(cdcl_inprocessingRules_unhiding_SMinus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = cdcl_inprocessingRules_unhiding_SMinus[i];
		if (GET_CDCL_LIT_PTR_FIN(l) < finished){
			//We do not keep the element in SMinus at position i. We just go on with the next literal in SMinus.
		} else {
			//We want to keep l. Write it at the current writePos and go on.
			cdcl_inprocessingRules_unhiding_SMinus[writePos++] = l;
			finished = GET_CDCL_LIT_PTR_FIN(l);
		}
	}
	//The amounts of literals written is now the new size of the clause. We check if we did indeed remove something from c.
	maxSize = writePos;
	if (oldSize > maxSize){
		//Yes, a removal happened. We will now copy all the retained elements from SMinus with opposite sign to the clause.
		//We update the signature of the clause and its size and watcher information.
		for(i = 0; i < maxSize; ++i){
			l = GET_CDCL_LIT_PTR_OPPOSITE(cdcl_inprocessingRules_unhiding_SMinus[i]);
			litID = GET_CDCL_LIT_ID_FROM_PTR(l);
			sig |= GET_CDCL_LIT_SIG(litID);
			SET_CDCL_CLS_LITNUM(c, i, litID);
		}
		SET_CDCL_CLS_LITNUM(c, i, 0);
		SET_CDCL_CLS_SIZE(c, i);
		SET_CDCL_CLS_SIG(c, sig);
		SET_CDCL_CLS_W1(c, GET_CDCL_CLS_LITNUM(c, 0));
		SET_CDCL_CLS_WHEREW1(c, 0);
		SET_CDCL_CLS_W2(c, GET_CDCL_CLS_LITNUM(c, 1));
		SET_CDCL_CLS_WHEREW2(c, 0);
		SET_CDCL_CLS_REPLIT(c, GET_CDCL_CLS_LITNUM(c, 0));
		if (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_LBD(c)){
			SET_CDCL_CLS_LBD(c, GET_CDCL_CLS_SIZE(c));
		}
	}
	//We have now reduced the clause as much as possible.
}

uint32_t cdcl_inprocessingRules_intern_unhiding_uhte(cdclClause *c){
	//This performs an UHTE check on the given clause (which must be disabled). This method returns 1 if the clause is a HT.
	//We first create the SPlus and SMinus sets such that they hold the respective literals or opposite literals in increasing
	//discovery time order.
	cdclLiteral *lPos, *lNeg;
	uint32_t i, lastEltPos = GET_CDCL_CLS_SIZE(c) - 1, currentEltPosPlus, currentEltPosMinus;
	//We create SPlus first by copying all the literals from c with the same sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		lPos = cdcl_literals + GET_CDCL_CLS_LITNUM(c, i);
		cdcl_inprocessingRules_unhiding_SPlus[i] = lPos;
	}
	qsort(cdcl_inprocessingRules_unhiding_SPlus, GET_CDCL_CLS_SIZE(c), sizeof(cdclLiteral*), cdcl_inprocessingRules_intern_unhiding_SSet_incDSC);
	lPos = cdcl_inprocessingRules_unhiding_SPlus[0];
	//We then create SMinus by copying all the literals form c with the opposite sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		lNeg = cdcl_literals - GET_CDCL_CLS_LITNUM(c, i);
		cdcl_inprocessingRules_unhiding_SMinus[i] = lNeg;
	}
	qsort(cdcl_inprocessingRules_unhiding_SMinus, GET_CDCL_CLS_SIZE(c), sizeof(cdclLiteral*), cdcl_inprocessingRules_intern_unhiding_SSet_incDSC);
	lNeg = cdcl_inprocessingRules_unhiding_SMinus[0];

	currentEltPosPlus = 0;
	currentEltPosMinus = 0;
	while(1){
		if (GET_CDCL_LIT_PTR_DSC(lNeg) > GET_CDCL_LIT_PTR_DSC(lPos)){
			if (currentEltPosPlus == lastEltPos) return 0;
			lPos = cdcl_inprocessingRules_unhiding_SPlus[++currentEltPosPlus];
		} else if ( (GET_CDCL_LIT_PTR_FIN(lNeg) < GET_CDCL_LIT_PTR_FIN(lPos))
				//|| ( GET_CLS_SIZE(c) == 2 && (lPos == GET_LIT_OPPOSITE(lNeg) || GET_LIT_PRT(lPos) == lNeg) )
				//The above check on clauses with size two is unnecessary, as transitive edges are removed on the fly
				//by the advanced stamping scheme.
					){
			if (currentEltPosMinus == lastEltPos) return 0;
			lNeg = cdcl_inprocessingRules_unhiding_SMinus[++currentEltPosMinus];
		} else {
			return 1;
		}
	}

	return 0;
}

uint32_t cdcl_inprocessingRules_intern_unhiding_stamp(cdclLiteral *l, uint32_t stamp, cdclClause** conflict){
	//This method implements the advanced stamping procedure.
	cdclLiteral *impLit, *impLitOpp, *failedLit;
	cdclClause *c;
	cdclVariable *v;
	uint32_t flagSCC = 1, impNum,i;	//The flag captures if l represents a strongly connected component (SCC).

	++stamp;
	SET_CDCL_LIT_PTR_DSC(l, stamp);
	SET_CDCL_LIT_PTR_OBS(l, stamp);
	//Push the literal to the literal stack.
	*(cdcl_inprocessingRules_unhiding_lStack_current++) = l;
	//We now check all binary clauses in which -l occurs. This is already captured in the implication list of l.
	for (impNum = 0; impNum < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); impNum += 2){
		impLit = cdcl_literals + GET_CDCL_LIT_PTR_IMPNUM(l, impNum);
		c = GET_CDCL_LIT_PTR_IMPNUM_REASON(l, impNum);
		//First, we check if we can remove this implication.
		if (GET_CDCL_LIT_PTR_DSC(l) < GET_CDCL_LIT_PTR_OBS(impLit)) {
			//The binary clause can be removed (transitive reduction). This will also remove the implication from the list
			//of l, we therefore must reduce the impNum counter by 2 to stay at this specific position in the list.
			cdcl_inprocessingRules_intern_disableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
			cdcl_inprocessingRules_intern_deleteClause(c);
			impNum -= 2;
			continue;
		}
		//Next, we perform a failed literal detection.
		impLitOpp = GET_CDCL_LIT_PTR_OPPOSITE(impLit);
		if (GET_CDCL_LIT_PTR_DSC(GET_CDCL_LIT_PTR_ROOT(l)) <= GET_CDCL_LIT_PTR_OBS(impLitOpp)){
			failedLit = l;
			while (GET_CDCL_LIT_PTR_DSC(failedLit) > GET_CDCL_LIT_PTR_OBS(impLitOpp)){
				failedLit = GET_CDCL_LIT_PTR_PRT(failedLit);
			}
			//We check if we can add the opposite of the failed literal to the UP queue.
			failedLit = GET_CDCL_LIT_PTR_OPPOSITE(failedLit);
			if (CDCL_UP_IS_OPPLIT_ON_S_PTR(failedLit)){
				//This constitutes a global conflict as we found a failed literal for which the opposite is also failed.
				#ifdef VERBOSE_CDCL
				printf("UNH-EQ-CONFf ");
				#endif
				*conflict = c;
				return 0;
			} else if (!CDCL_UP_IS_LIT_ON_S_PTR(failedLit)){
				//We can add the literal since it is not already enqueued.
				v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(failedLit));
				CDCL_UP_ADD_DEC_PTR(failedLit,v);
			}//Else the literal is already enqueued.
			if (GET_CDCL_LIT_PTR_DSC(impLitOpp) != 0 && GET_CDCL_LIT_PTR_FIN(impLitOpp) == 0) continue;
		}
		//Next, we perform the basic stamping operations.
		if (GET_CDCL_LIT_PTR_DSC(impLit) == 0){
			SET_CDCL_LIT_PTR_PRT(impLit, l);
			SET_CDCL_LIT_PTR_ROOT(impLit, GET_CDCL_LIT_PTR_ROOT(l));
			//Recursive stamping for the implied literal.
			stamp = cdcl_inprocessingRules_intern_unhiding_stamp(impLit, stamp, conflict);
			if (*conflict != NULL) return 0;
		}
		//Next, we check for literal equivalences for equivalence literal substitution.
		if (GET_CDCL_LIT_PTR_FIN(impLit) == 0 && GET_CDCL_LIT_PTR_DSC(impLit) < GET_CDCL_LIT_PTR_DSC(l)){
			//Literal l is equivalent to literal impLit.
			SET_CDCL_LIT_PTR_DSC(l, GET_CDCL_LIT_PTR_DSC(impLit));
			flagSCC = 0;
		}
		SET_CDCL_LIT_PTR_OBS(impLit, stamp);
	}
	//We now check if l represents an SCC for equivalent literal substitution.
	if (flagSCC){
		//Yes, l represents an SCC. All literals in the stack will receive the same stamp.
		++stamp;
		//Allocate new equivalence class if necessary.
		CDCL_IR_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--cdcl_inprocessingRules_unhiding_lStack_current);
			SET_CDCL_LIT_PTR_DSC(impLit, GET_CDCL_LIT_PTR_DSC(l));
			SET_CDCL_LIT_PTR_FIN(impLit, stamp);
			if (GET_CDCL_LIT_PTR_STAMP(impLit) != cdcl_litStamp){
				CDCL_IR_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_CDCL_LIT_PTR_STAMP(impLit, cdcl_litStamp);
				SET_CDCL_LIT_PTR_STAMP(GET_CDCL_LIT_PTR_OPPOSITE(impLit), cdcl_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++cdcl_inprocessingRules_els_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used] = 0;
		}
	}

	return stamp;
}

cdclClause* cdcl_inprocessingRules_intern_unhiding(uint32_t roundsToPerform){
	cdclClause *conflict = NULL, *c;
	cdclLiteral *l, *lOpp, *classRep;
	cdclVariable *v;
	uint32_t stamp, random, oldsize, numToPermute, rounds=0;
	int32_t i,j, oIgnore, lIgnore, toRID, reWID, same, opp;

	#ifdef VERBOSE_CDCL
	printf("UNH%d", roundsToPerform);fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	do {
		//We prepare our decision stack by back-jumping to DL0 in APPEND mode. The advanced stamping and the units learned by
		//UHLE can then be put to the end of the DL0.
		cdcl_inprocessingRules_backJump(0,0);
		cdcl_inprocessingRules_unhiding_lStack_current = cdcl_inprocessingRules_unhiding_lStack + 1;


		//We initialize the literals for the advanced stamping.
		for (i = 0; i < cdcl_vars_e_used; ++i){
			v = cdcl_variables + GET_CDCL_VAR_PTR_ID(cdcl_vars_e[i]);
			l = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
			SET_CDCL_LIT_PTR_DSC(l, 0);
			SET_CDCL_LIT_PTR_FIN(l, 0);
			SET_CDCL_LIT_PTR_OBS(l, 0);
			SET_CDCL_LIT_PTR_PRT(l, l);
			SET_CDCL_LIT_PTR_ROOT(l, l);
			l = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
			SET_CDCL_LIT_PTR_DSC(l, 0);
			SET_CDCL_LIT_PTR_FIN(l, 0);
			SET_CDCL_LIT_PTR_OBS(l, 0);
			SET_CDCL_LIT_PTR_PRT(l, l);
			SET_CDCL_LIT_PTR_ROOT(l, l);
		}

		//First, we initialize a new permutation of all the literals that remain.
		numToPermute = 2*cdcl_vars_e_used;
		perm_LFSR_init(numToPermute);
		stamp = 0;

		//We prepare the literal stamp to present classes only once.
		++cdcl_litStamp;

		//Initialize the classes information.
		cdcl_inprocessingRules_els_classes_used = 0;
		for (i = 0; i < cdcl_inprocessingRules_els_classes_avail; ++i){
			cdcl_inprocessingRules_els_classes_memUsed[i] = 0;
		}

		for (i = 0; i < perm_lfsr_perLen; ++i){
			random = perm_LFSR_nextState();
			if (random >= numToPermute) continue;
			if (random < cdcl_vars_e_used){
				l = cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_vars_e[random]);
			} else {
				l = cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_vars_e[random - cdcl_vars_e_used]);
			}
			cdcl_inprocessingRules_litPerm[stamp++] = l;
		}
		stamp = 0;

		//We then walk through the permutation to stamp all roots.
		for (i = 0; i < numToPermute; ++i){
			l = cdcl_inprocessingRules_litPerm[i];
			lOpp = GET_CDCL_LIT_PTR_OPPOSITE(l);
			//Literal l is now the literal we want to stamp. If the opposite has no implications, l is a root.
			if (GET_CDCL_LIT_PTR_NUMIMPS_USED(lOpp) == 0U){
				//Since the opposite of l has no implications, l must be a root. We stamp it.
				stamp = cdcl_inprocessingRules_intern_unhiding_stamp(l, stamp, &conflict);
				if (conflict != NULL){
					#ifdef VERBOSE_CDCL
					printf(" RS");
					#endif
					return conflict;
				}
			}
		}

		//We stamp the remaining literals if they have not be discovered yet.
		for (i = 0; i < numToPermute; ++i){
			l = cdcl_inprocessingRules_litPerm[i];
			//If l was not yet discovered, we can stamp it.
			if (GET_CDCL_LIT_PTR_DSC(l) == 0U){
				stamp = cdcl_inprocessingRules_intern_unhiding_stamp(l, stamp, &conflict);
				if (conflict != NULL){
					#ifdef VERBOSE_CDCL
					printf(" LS");
					#endif
					return conflict;
				}
			}
		}

		//The advanced stamping of all the literals in BIG is now over. It is now time to iterate through all the clauses of
		//the formula and perform UHTE or UHLE. The following is the implementation of the SIMPLIFY algorithm from the paper.
		//We first iterate through all the original clauses of the formula, and then through all the learned clauses.
		oIgnore = 0;
		lIgnore = 0;
		for (i = 0; i < cdcl_cls_o_used + cdcl_cls_l_used; ++i){
			if (i < cdcl_cls_o_used - oIgnore){
				c = cdcl_cls_o[i];
			} else if (i < cdcl_cls_o_used) {
				i = cdcl_cls_o_used-1;
				continue;
			} else if (i - cdcl_cls_o_used < cdcl_cls_l_used - lIgnore){
				c = cdcl_cls_l[i - cdcl_cls_o_used];
			} else {
				break;
			}

			//Temporarily disable the clause.
			cdcl_inprocessingRules_intern_disableClause(c);
			//Check for UHTE -- Unhiding Hidden Tautology Elimination.
			if (GET_CDCL_CLS_SIZE(c) > 2 && IS_CDCL_CLS_LEARNED(c) && cdcl_inprocessingRules_intern_unhiding_uhte(c)){
				//It is a hidden tautology. We get rid of the clause for good.
				cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
				cdcl_inprocessingRules_intern_deleteClause(c);
			} else {
				//Otherwise, it is no hidden tautology, so we cannot get rid of the clause. We will first perform UHLE and
				//check how far it was reduced. If it is no unit clause, we re-enable it. If it is unit, we try to put its
				//literal in the UP. If this fails, we have a global conflict. If this does not fail, we can abandon the
				//clause because we do not need units.
				oldsize = GET_CDCL_CLS_SIZE(c);
				cdcl_inprocessingRules_intern_unhiding_uhle(c);
				if (GET_CDCL_CLS_SIZE(c) == 1){
					//We have reduced it to a unit clause and must now check if we can put its literal in the UP.
					v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c, 0));
					l = cdcl_literals + GET_CDCL_CLS_LITNUM(c, 0);
					if (CDCL_UP_IS_OPPLIT_ON_S_PTR(l)){
						//This constitutes a global conflict as we found a unit for which the opposite is enforced.
						#ifdef VERBOSE_CDCL
						printf(" LE");
						#endif
						return c;
					} else if (!CDCL_UP_IS_LIT_ON_S_PTR(l)){
						//We can add the literal since it is not already enqueued.
						CDCL_UP_ADD_DEC_PTR(l, v);
					}
					//Anyway, we can abandon the clause.
					cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
					cdcl_inprocessingRules_intern_deleteClause(c);
				} else {
					//We did not reduce it to a unit clause. However it went with the reduction for c, it is at least binary
					//and we must keep it. We therefore enable it again.
					cdcl_inprocessingRules_intern_enableClause(c);
					if (oldsize != GET_CDCL_CLS_SIZE(c)){
						#ifdef VERBOSE_CDCL
						remLits += (oldsize - GET_CDCL_CLS_SIZE(c));
						#endif
						cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(c);

					}
					if (IS_CDCL_CLS_LEARNED(c)){
						++lIgnore;
					} else {
						++oIgnore;
					}
				}
			}
		}

		//We walk through all the found equivalence classes and replace the literals or propagate them, if possible.
		for (i = 0; i < cdcl_inprocessingRules_els_classes_used; ++i){
			//First, we check if any literal in the current class is already on the stack.
			same = 0;
			opp = 0;
			for (j = 1; j < cdcl_inprocessingRules_els_classes_memUsed[i]; ++j){
				//Grab the literal and variable to replace.
				classRep = cdcl_inprocessingRules_els_classes[i][j];
				if (CDCL_UP_IS_LIT_ON_S_PTR(classRep)){
					same = 1;
				} else if (CDCL_UP_IS_OPPLIT_ON_S_PTR(classRep)){
					opp = 1;
				}
			}
			//Check, if any of the literals is marked for propagation.
			if (same && opp){
				//From the current class, there is at least one literal marked for propagation with same sign and one marked
				//for propagation with opposite sign. This constitutes a global conflict as all literals must be assigned
				//the same way since they are all equivalent.
				#ifdef VERBOSE_CDCL
				printf(" EQ ");
				#endif
				return (cdclClause*) 1;
			}
			if (same){
				//There is at least one literal that is marked for propagation with the same sign. We add all the other
				//literals with the same sign into the UP as well.
				for (j = 0; j < cdcl_inprocessingRules_els_classes_memUsed[i]; ++j){
					classRep = cdcl_inprocessingRules_els_classes[i][j];
					if (!CDCL_UP_IS_LIT_ON_S_PTR(classRep)){
						v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(classRep));
						CDCL_UP_ADD_DEC_PTR(classRep, v);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			} else if (opp){
				//There is at least one literal that is marked for propagation with the opposite sign. We add all the other
				//literals with the opposite sign into the UP as well.
				for (j = 0; j < cdcl_inprocessingRules_els_classes_memUsed[i]; ++j){
					classRep = cdcl_inprocessingRules_els_classes[i][j];
					if (!CDCL_UP_IS_OPPLIT_ON_S_PTR(classRep)){
						v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(classRep));
						classRep = GET_CDCL_LIT_PTR_OPPOSITE(classRep);
						CDCL_UP_ADD_DEC_PTR(classRep, v);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			}
			//If we get this far, we must make a replacement. For class i, we pick the class representative (first literal).
			classRep = cdcl_inprocessingRules_els_classes[i][0];
			reWID = GET_CDCL_LIT_ID_FROM_PTR(classRep);
			//For all the remaining literals in the class, we replace their occurrence with the representative.
			for (j = 1; j < cdcl_inprocessingRules_els_classes_memUsed[i]; ++j){
				//Grab the literal and variable to replace.
				l = cdcl_inprocessingRules_els_classes[i][j];
				toRID = GET_CDCL_LIT_ID_FROM_PTR(l);
				v = cdcl_variables + abs(toRID);

				//Replace the occurrence of l with classRep.
				conflict = cdcl_inprocessingRules_intern_els_replaceLit(l, classRep);
				if (conflict != NULL){
					#ifdef VERBOSE_CDCL
					printf(" RC ");
					#endif
					return conflict;
				}

				//Replace the occurrence of -l with -classRep.
				conflict = cdcl_inprocessingRules_intern_els_replaceLit(GET_CDCL_LIT_PTR_OPPOSITE(l),GET_CDCL_LIT_PTR_OPPOSITE(classRep));
				if (conflict != NULL){
					#ifdef VERBOSE_CDCL
					printf(" RC ");
					#endif
					return conflict;
				}

				//Mark the replaced variable with its representative.
				if ((toRID < 0 && reWID < 0) || (toRID > 0 && reWID > 0)){
					SET_CDCL_VAR_PTR_ISEQUIVTO(v, abs(reWID));
				} else {
					SET_CDCL_VAR_PTR_ISEQUIVTO(v, -abs(reWID));
				}

				//Disable the replaced variable.
				cdcl_inprocessingRules_intern_disableVariable(v);
			}
		}

		//We propagate every unit we found. If we did not find any unit, then this will again terminate DL0.
		conflict = cdcl_inprocessingRules_upPropagate();
		if (conflict != NULL ){
			#ifdef VERBOSE_CDCL
			printf(" CONFLICT ");
			#endif
			break;
		}

	} while (++rounds < roundsToPerform);
	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif

	return conflict;
}

void cdcl_inprocessingRules_backJump(uint32_t toLevel, uint32_t clearTargetLevel){
	//This method back-jumps to the given level. It will reset the UP to append to this level and removes all assignments
	//and reasons above this level up to the current end-pointer of the UP.
	cdclLiteral **clearTo = cdcl_up_end, **clearFrom;	//We clear the decision stack between clearFrom and clearTo.
	cdclLiteral *l;
	cdclVariable *v;

	//We check if the target level is to be cleared as well. If 0, we will just append to the target level in next UP.
	if (clearTargetLevel){
		CDCL_UP_SET_DECLEVEL(toLevel);//We reset the UP pointers and the decision stack to override the level completely.
	} else {
		CDCL_UP_SET_DECLEVEL_APPEND(toLevel);//We reset the UP pointers and the decision stack to append to the target level.
	}
	//Depending on the above, the cdcl_up_current pointer now points to the beginning of a level or its end. No matter what,
	//everything from this position onwards must be cleared -- remove assignments, reasons and so on.
	clearFrom = cdcl_up_current;

	while (clearFrom < clearTo){
		l = *clearFrom;
		//We grabbed the element from the stack. We skip NULL values in between the decision levels we jump over.
		if (l != NULL){
			//If the literal had a reason (was no decision), we tell the clause that it is no reason anymore.
			v = cdcl_variables + GET_CDCL_VAR_ID_FROM_LIT_PTR(l);
			SET_CDCL_VAR_PTR_ASS_2(v);
		}
		++clearFrom;
	}
	#ifdef COLLECTSTATS
	++stats_cdcl_numBackjumps;
	#endif

}

cdclClause* cdcl_inprocessingRules_intern_nhbr_detect(cdclLiteral *rootLit){
	//Performs unit propagation prioritizing binary clauses. It assumes that we are in DL1 and that DL1 is empty.
	//This method performs a test-wise unit propagation of literal l. We assume that the current DL is empty and use it for
	//the test propagations.
	cdclVariable *v;
	cdclClause *c;
	cdclLiteral *l;
	cdclLiteral **checkFrom = cdcl_up_current;
	int32_t lid, lit, i, offset, litPos, learnBinary, numBinOccsForLearnBinary;

	++cdcl_litStamp;

	//Add our test-literal to the stack.
	v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(rootLit));
	CDCL_UP_ADD_DEC_PTR(rootLit,v);
	SET_CDCL_LIT_PTR_STAMP(rootLit, cdcl_litStamp);

	do {
		while (cdcl_up_current < cdcl_up_end){
			//We grab the next literal from the stack and assign it accordingly.
			l = *(cdcl_up_current++);
			lid = GET_CDCL_LIT_ID_FROM_PTR(l);
			if (lid < 0){
				//The literal is negative. The variable must be assigned to 0.
				v = cdcl_variables - lid;
				SET_CDCL_VAR_PTR_ASS_0(v);
			} else {
				//The literal is positive. The variable must be assigned to 1.
				v = cdcl_variables + lid;
				SET_CDCL_VAR_PTR_ASS_1(v);
			}

			//We check all the implications of this literal.
			for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i+=2){
				lit = GET_CDCL_LIT_PTR_IMPNUM(l, i);
				v = cdcl_variables + abs(lit);
				if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
					//First, we check if the opposite literal has been stamped. If so, then there is at least one large clause
					//that needs the opposite assignment. This results in a conflict right away.
					if (GET_CDCL_LIT_STAMP(-lit) == cdcl_litStamp){
						//Yes, this is a conflict.
						c = GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i);
						return c;
					}

					if (CDCL_UP_IS_OPPLIT_ON_S(lit)){
						//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
						c = GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i);
						return c;
					} else if (!CDCL_UP_IS_LIT_ON_S(lit)){
						//The element is not yet on the stack. We put it there and stamp it.
						v = cdcl_variables + abs(lit);
						CDCL_UP_ADD_DEC(lit,v);
						SET_CDCL_LIT_STAMP(lit, cdcl_litStamp);
					} //Else, the literal is already enqueued.
				}
			}
		}

		learnBinary = 0;
		numBinOccsForLearnBinary = -1;
		while (checkFrom < cdcl_up_current){
			l = *(checkFrom++);
			lid = GET_CDCL_LIT_ID_FROM_PTR(l);
			//We check the watcher lists of the falsified literal.
			l = cdcl_literals - lid;

			//WL1
			offset = 0;
			while (GET_CDCL_LIT_PTR_NUMW1_USED(l) > offset){
				c = GET_CDCL_LIT_PTR_W1_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CDCL_CLS_REPLIT(c);
				if (IS_CDCL_LIT_SAT(lit) || GET_CDCL_LIT_STAMP(lit) == cdcl_litStamp){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW1(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit) || GET_CDCL_LIT_STAMP(lit) == cdcl_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the second watcher.
					if (lit == GET_CDCL_CLS_W2(c)) continue;
					//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
					if (IS_CDCL_LIT_SAT(lit) || (!IS_CDCL_LIT_ASSIGNED(lit) && GET_CDCL_LIT_STAMP(-lit) != cdcl_litStamp)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W1(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W1(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W1LIST(-lid, litPos);
					ADD_CDCL_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CDCL_CLS_W2(c);
					if (IS_CDCL_LIT_UNSAT(lit) || GET_CDCL_LIT_STAMP(-lit) == cdcl_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						//Clean-up.
						return c;
					} else {
						//The element is not yet on the stack. We stamp it.
						SET_CDCL_LIT_STAMP(lit, cdcl_litStamp);
						//We can use this literal to learn a new binary clause.
						if ((int32_t)(GET_CDCL_LIT_NUMIMPS_USED(lit)) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_CDCL_LIT_NUMIMPS_USED(lit);
						}

					}
				}
			}

			//WL2
			offset = 0;
			while (GET_CDCL_LIT_PTR_NUMW2_USED(l) > offset){
				c = GET_CDCL_LIT_PTR_W2_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CDCL_CLS_REPLIT(c);
				if (IS_CDCL_LIT_SAT(lit) || GET_CDCL_LIT_STAMP(lit) == cdcl_litStamp){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW2(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit) || GET_CDCL_LIT_STAMP(lit) == cdcl_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the representative
						//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the first watcher.
					if (lit == GET_CDCL_CLS_W1(c)) continue;
					//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
					if (IS_CDCL_LIT_SAT(lit) || (!IS_CDCL_LIT_ASSIGNED(lit) && GET_CDCL_LIT_STAMP(-lit) != cdcl_litStamp)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W2(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W2(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W2LIST(-lid, litPos);
					ADD_CDCL_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CDCL_CLS_W1(c);
					if (IS_CDCL_LIT_UNSAT(lit) || GET_CDCL_LIT_STAMP(-lit) == cdcl_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						//Clean-up.
						return c;
					} else {
						//The element is not yet on the stack. We stamp it.
						SET_CDCL_LIT_STAMP(lit, cdcl_litStamp);
						//We can use this literal to learn a new binary clause.
						if ((int32_t)(GET_CDCL_LIT_NUMIMPS_USED(lit)) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_CDCL_LIT_NUMIMPS_USED(lit);
						}
					}
				}
			}
		}

		if (!learnBinary) break;

		//We can now learn the new binary (-rootLit OR learnBinary).
		cdcl_inprocessingRules_intern_addLearnedBinaryClause(-GET_CDCL_LIT_ID_FROM_PTR(rootLit), learnBinary);
		cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(cdcl_cls_l[cdcl_cls_l_used-1]);
		//The implied binary must now be added to the UP queue.
		v = cdcl_variables + abs(learnBinary);
		CDCL_UP_ADD_DEC(learnBinary, v);
		SET_CDCL_LIT_STAMP(learnBinary, cdcl_litStamp);
	} while (1);
	CDCL_UP_TERMINATE_CURRENT();
	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_nhbr(){
	//Performs non-transitive hyper binary resolution using unit propagation.
	cdclLiteral *l;
	cdclVariable *v;
	cdclClause *conflict = NULL;
	int32_t litID;

	#ifdef VERBOSE_CDCL
	printf("NHB");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	//Then, we walk through the literal permutation and satisfy each of the roots (if the literal is still a root).
	while ((l = cdcl_literals_Q_dequeueBest()) !=NULL){
		litID = GET_CDCL_LIT_ID_FROM_PTR(l);
		//We ignore literals of variables that are already assigned.
		if (IS_CDCL_LIT_ASSIGNED(litID)) continue;
		if (GET_CDCL_LIT_PTR_NUMIMPS_USED(l) == 0) continue;
		conflict = cdcl_inprocessingRules_intern_nhbr_detect(l);
		if (conflict != NULL){
			//Literal l is a failed literal, we must propagate the opposite.
			cdcl_inprocessingRules_backJump(0,0);
			v = cdcl_variables + abs(litID);
			CDCL_UP_ADD_DEC(-litID, v);
			conflict = cdcl_inprocessingRules_upPropagate();
			if (conflict != NULL){
				//Both sides are failed. This means the instance is unsatisfiable.
				break;
			}
		} else {
			cdcl_inprocessingRules_backJump(1,1);
		}

	}

	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif

	return conflict;
}

void cdcl_inprocessingRules_intern_treelook_enqueue(cdclLiteral *l){
	//First, we enqueue literal l.
	cdclLiteral *lOpp = GET_CDCL_LIT_PTR_OPPOSITE(l), *lDash;
	uint32_t i;
	*(cdcl_inprocessingRules_trl_lQ_e++) = l;
	SET_CDCL_LIT_PTR_TRL_ISINQ(l, 1U);
	//We now want all literal l' for which we have clauses (l v -l'). We pick all the implications of -l, and invert their
	//sign. In the above example, the implication list of -l contains -l'. Inverting the sign gives us l'.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(lOpp); i+=2){
		//Grab l' by inverting the sign of the implication of -l.
		lDash = cdcl_literals - GET_CDCL_LIT_PTR_IMPNUM(lOpp, i);
		//Check if lDash is already enqueued. If not, enqueue it.
		if (!GET_CDCL_LIT_PTR_TRL_ISINQ(lDash)){
			cdcl_inprocessingRules_intern_treelook_enqueue(lDash);
		}
	}
	//Finally, we put the special element NULL into the queue.
	*(cdcl_inprocessingRules_trl_lQ_e++) = NULL;
}

uint32_t cdcl_inprocessingRules_intern_treelook_checkUnsat(){
	//Check if any of the clauses that lost a literal is now false under the tree-look assignment.
	cdclClause *c;
	int32_t lit;
	uint32_t i, numUnsatLits;

	//Pop all the clauses from the queue and check if any of them is empty under the tree-look assignment.
	while ((c = cdcl_clauses_Q_dequeueBest()) != NULL){
		//Check if the clause is satisfied.
		if (IS_CDCL_LIT_SAT(GET_CDCL_CLS_REPLIT(c))){
			//If the clause is satisfied we do not need to look at it again.
			continue;
		}
		//If not, we must count the number of unsatisfied literals it has.
		numUnsatLits = 0;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			lit = GET_CDCL_CLS_LITNUM(c,i);
			if (IS_CDCL_LIT_SAT(lit)){
				SET_CDCL_CLS_REPLIT(c,lit);
				numUnsatLits = 0;
				break;
			} else if (IS_CDCL_LIT_UNSAT(lit)) {
				++numUnsatLits;
			} else {
				//The clause has at least one not yet assigned literal. We do not need it.
				numUnsatLits = 0;
				break;
			}
		}
		if (numUnsatLits == GET_CDCL_CLS_SIZE(c)){
			//Yes, the number of false literals in this clause matches the number of literals in it. The clause is false. We
			//put it back into the queue for later re-checking and return.
			cdcl_clauses_Q_touchClause(c);
			return 1;
		}
	}
	return 0;
}

cdclClause* cdcl_inprocessingRules_intern_treelook_detect(cdclLiteral *rootLit){
	//This method propagates rootLit and all implied assignments.

	cdclLiteral **checkStart = cdcl_up_current, **checkFrom, *aLit;
	cdclVariable *v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(rootLit));
	cdclClause *c;
	int32_t aLitID = 0, lit = 0;
	uint32_t count, litPos, offset;
	int32_t i, learnBinary;

	CDCL_UP_ADD_DEC_PTR(rootLit, v)

	do {
		//We retain the information from what position in the queue the new assignments start.
		checkFrom = checkStart;

		do {
			//We grab the next literal to assign and assign it.
			aLit = *(cdcl_up_current++);
			lit = GET_CDCL_LIT_ID_FROM_PTR(aLit);
			if (!IS_CDCL_LIT_ASSIGNED(lit)){
				v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(aLit));
				if (lit < 0){
					SET_CDCL_VAR_PTR_ASS_0(v);
				} else {
					SET_CDCL_VAR_PTR_ASS_1(v);
				}
			}

			//From the just assigned literal, we must put all the implied on the stack as well and assign them accordingly.
			for (count = 0; count < GET_CDCL_LIT_PTR_NUMIMPS_USED(aLit); count += 2){
				lit = GET_CDCL_LIT_PTR_IMPNUM(aLit, count);
				if (IS_CDCL_LIT_SAT(lit)){
					//No further action must be performed, but we can update the representative for this clause.
					SET_CDCL_CLS_REPLIT(GET_CDCL_LIT_PTR_IMPNUM_REASON(aLit, count), lit);
				} else if (IS_CDCL_LIT_UNSAT(lit)){
					//A conflict was discovered. We can return.
					cdcl_up_current = cdcl_up_end;
					return (cdclClause*)1;
				} else {
					//The literal is neither satisfied nor unsatisfied. This implies that it is not assigned.
					v = cdcl_variables + abs(lit);
					CDCL_UP_ADD_DEC(lit, v);
					if (lit < 0){
						SET_CDCL_VAR_PTR_ASS_0(v);
					} else {
						SET_CDCL_VAR_PTR_ASS_1(v);
					}
					//We can update the representative for this clause.
					SET_CDCL_CLS_REPLIT(GET_CDCL_LIT_PTR_IMPNUM_REASON(aLit, count), lit);
				}
			}
		} while (cdcl_up_current < cdcl_up_end);

		//Now all binary implications have been propagated. It is time to find out if any of the large clauses that lost
		//a literal during these action is either empty, or unit. In case it is empty we can return right away as well. If it
		//is unit, we can put the last literal in the clause (which MUST be unassigned now) on the stack as well and
		//propagate its assignment and implications.
		learnBinary = 0;
		while (checkFrom < cdcl_up_current){
			aLit = *(checkFrom++);
			aLitID = -GET_CDCL_LIT_ID_FROM_PTR(aLit);
			//Literal aLitID is now a literal that is false. We check all the clauses where this literal is a watcher.

			//WL1
			offset = 0;
			while (offset < GET_CDCL_LIT_NUMW1_USED(aLitID)){
				c = GET_CDCL_LIT_W1_WATCHED_CLS(aLitID, offset);
				//Check if the clause is satisfied by its representative.
				if (IS_CDCL_LIT_SAT(GET_CDCL_CLS_REPLIT(c)) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W2(c)) ){
					//Yes, no need to look at it again.
					++offset;
					continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW1(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the second watcher.
					if (lit == GET_CDCL_CLS_W2(c)) continue;
					//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
					if (!IS_CDCL_LIT_ASSIGNED(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W1(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W1(c) != aLitID){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W1LIST(aLitID, litPos);
					ADD_CDCL_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					lit = GET_CDCL_CLS_W2(c);
					if (IS_CDCL_LIT_UNSAT(lit)){
						//A conflict has been discovered.
						return c;
					} else if (!IS_CDCL_LIT_SAT(lit)){
						//The second watcher is not assigned. Since we are about to assign it such that it is true, we can
						//make it the representative of this clause to faster ignore it in future checks.
						SET_CDCL_CLS_REPLIT(c,lit);
						learnBinary = lit;
						break;
					} else {
						++offset;
					}
				}
			}
			//After checking the clauses, we must check if we can learn a new binary clause from that and can put its
			//its assignment into the current decision level.
			if (learnBinary != 0){
				//We can now learn the new binary (-rootLit OR learnBinary).
				cdcl_inprocessingRules_intern_addLearnedBinaryClause(-GET_CDCL_LIT_ID_FROM_PTR(rootLit), learnBinary);
				//The implied binary must now be added to the UP queue.
				v = cdcl_variables + abs(learnBinary);
				CDCL_UP_ADD_DEC(learnBinary, v);
				//We stop checking large clauses as there is new stuff to be propagated (and possibly new binaries that must
				//be prioritized).
				break;
			}
			//If we were unable to learn a new binary clause, we continue with checking the second watcher.
			//WL2
			offset = 0;
			while (offset < GET_CDCL_LIT_NUMW2_USED(aLitID)){
				c = GET_CDCL_LIT_W2_WATCHED_CLS(aLitID, offset);
				//Check if the clause is satisfied by its representative.
				if (IS_CDCL_LIT_SAT(GET_CDCL_CLS_REPLIT(c)) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W1(c))  ){
					//Yes, no need to look at it again.
					++offset;
					continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW2(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the first watcher.
					if (lit == GET_CDCL_CLS_W1(c)) continue;
					//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
					if (!IS_CDCL_LIT_ASSIGNED(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W2(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W2(c) != aLitID){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W2LIST(aLitID, litPos);
					ADD_CDCL_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					lit = GET_CDCL_CLS_W1(c);
					if (IS_CDCL_LIT_UNSAT(lit)){
						//A conflict has been discovered.
						return c;
					} else if (!IS_CDCL_LIT_SAT(lit)){
						//The first watcher is not assigned. Since we are about to assign it such that it is true, we can
						//make it the representative of this clause to faster ignore it in future checks.
						SET_CDCL_CLS_REPLIT(c,lit);
						learnBinary = lit;
						break;
					} else {
						++offset;
					}
				}
			}
			//After checking the clauses, we must check if we can learn a new binary clause from that and can put its
			//its assignment into the current decision level.
			if (learnBinary != 0){
				//We can now learn the new binary (-rootLit OR learnBinary).
				cdcl_inprocessingRules_intern_addLearnedBinaryClause(-GET_CDCL_LIT_ID_FROM_PTR(rootLit), learnBinary);
				//The implied binary must now be added to the UP queue.
				v = cdcl_variables + abs(learnBinary);
				CDCL_UP_ADD_DEC(learnBinary, v);
				//We stop checking large clauses as there is new stuff to be propagated (and possibly new binaries that must
				//be prioritized).
				break;
			}
		}
	} while (cdcl_up_current < cdcl_up_end);

	//In case we reach this position the UP finished without a conflict, but it might have learned new binaries.
	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_treelook(){
	//This method performs ELS until saturation. Afterwards it creates an out-tree queue. Then it learns NHBR clauses.
	#ifdef VERBOSE_CDCL
	printf("TRL: ");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	int32_t ignoredOutTrees = 0;
	#endif

	cdclClause *conflict = NULL, *c;
	cdclLiteral *aLit, **currentDL;
	cdclVariable *v;
	int32_t aLitID;
	uint32_t i, offset, numFailed = 0;


	//We start to perform ELS until saturation.
	do {
		i = cdcl_vars_e_used;
		conflict = cdcl_inprocessingRules_intern_els();
		if (conflict != NULL) {
			#ifdef VERBOSE_CDCL
			printf("ELS-C");
			#endif
			return conflict;
		}
	} while (i > cdcl_vars_e_used);

	//We can now initialize the out-tree queue. We do not have to reset the CDCL_LIT_TRL_ISINQ(l) value, because that is
	//the ISINS value from the ELS. Since ELS must pop all elements from the stack, all these values will be zero here.
	cdcl_inprocessingRules_trl_lQ_c = cdcl_inprocessingRules_trl_lQ;
	cdcl_inprocessingRules_trl_lQ_e = cdcl_inprocessingRules_trl_lQ;

	cdcl_literals_Q_flush();
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_inprocessing_treelook;

	for (i = 0; i < cdcl_vars_e_used; ++i){
		aLit = cdcl_literals + GET_CDCL_VAR_PTR_ID(cdcl_vars_e[i]);
		cdcl_literals_Q_touchLiteral(aLit);
		aLit = cdcl_literals - GET_CDCL_VAR_PTR_ID(cdcl_vars_e[i]);
		cdcl_literals_Q_touchLiteral(aLit);
	}

	while ((aLit = cdcl_literals_Q_dequeueBest()) != NULL){
		if (GET_CDCL_LIT_PTR_NUMIMPS_USED(aLit) == 0){
			cdcl_inprocessingRules_intern_treelook_enqueue(aLit);
		}
	}
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;

	//Prepare the clause priority queue for efficient test if any large clause became false under reduction.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_inprocessing_treelook;

	//At this point we have enqueued all the root literals along with their recursive implications. We continue by performing
	//the tree-look as long as the queue is not empty.

	++cdcl_litStamp;

	//We back-jump in preparation for the tree-look stacking.
	cdcl_inprocessingRules_backJump(0,0);

	while (cdcl_inprocessingRules_trl_lQ_c < cdcl_inprocessingRules_trl_lQ_e){
		//Grab the next element from the literal queue.
		aLit = *(cdcl_inprocessingRules_trl_lQ_c++);

		//Optimization: Out-trees of size one can be ignored if the root-literal for this out-tree has no implications.
		if (cdcl_decLevel == 0 		//We would now start a new assignment trail on the stack (checking a new out-tree).
				&& aLit != NULL 	//The literal could actually be propagated.
				&& *cdcl_inprocessingRules_trl_lQ_c == NULL //However, the size of the tree is one.
				&& GET_CDCL_LIT_PTR_NUMIMPS_USED(aLit) == 0		//And it does not receive any new implications.
				){
			//The literal aLit is the top of an out-tree of depth one and has no implications. So propagating it would result
			//in no further assignments. Since the out-tree is then removed from the decision stack right afterwards, there
			//is no need to propagate at all -- nothing can be gained by this. We simply continue with the next out-tree.
			cdcl_inprocessingRules_trl_lQ_c++;
			#ifdef VERBOSE_CDCL
			++ignoredOutTrees;
			#endif
			continue;
		}

		if (aLit != NULL){
			//Grab the new element from the out tree.
			aLitID = GET_CDCL_LIT_ID_FROM_PTR(aLit);

			//Open up a new decision level.
			CDCL_UP_TERMINATE_CURRENT();

			//We first check if the literal that is supposed to open up the new level is a failed literal.
			if (IS_CDCL_LIT_UNSAT(aLitID) || cdcl_inprocessingRules_intern_treelook_checkUnsat()){
				//It is a failed literal. We must store this information if this does not already end in a global conflict.
				if (GET_CDCL_LIT_PTR_STAMP(GET_CDCL_LIT_PTR_OPPOSITE(aLit)) == cdcl_litStamp){
					//It ends in a global conflict. We can stop.
					#ifdef VERBOSE_CDCL
					printf("TRL-Ca");
					#endif
					conflict = (cdclClause*)1;
				} else if (GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp){
					//It does not end in a global conflict. We can still store this literal.
					cdcl_inprocessingRules_trl_faileds[numFailed++] = aLit;
					SET_CDCL_LIT_PTR_STAMP(aLit, cdcl_litStamp);
				}//Else the literal is already marked for later propagation.
			} else if (!IS_CDCL_LIT_SAT(aLitID)){
				//The literal is neither failed nor already satisfied. We can perform a detection on in in this new level.
				conflict = cdcl_inprocessingRules_intern_treelook_detect(aLit);

				//Check if this resulted in a conflict.
				if (conflict != NULL){
					//It is a failed literal. We must store this information if this does not already end in a global conflict.
					conflict = NULL;
					if (GET_CDCL_LIT_PTR_STAMP(GET_CDCL_LIT_PTR_OPPOSITE(aLit)) == cdcl_litStamp){
						//It ends in a global conflict. We can stop.
						#ifdef VERBOSE_CDCL
						printf("TRL-Cb");
						#endif
						conflict = (cdclClause*)1;
					} else if (GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp){
						//It does not end in a global conflict. We can still store this literal.
						cdcl_inprocessingRules_trl_faileds[numFailed++] = aLit;
						SET_CDCL_LIT_PTR_STAMP(aLit, cdcl_litStamp);
					}//Else the literal is already marked for later propagation.
					if (conflict == NULL){
						//We must now enqueue all clauses that lost a literal in the last decision level. All these clauses
						//can be false (and stay false) now until we remove the level in the out-tree. It suffices to look
						//at the remaining watched clauses for all these literals.
						currentDL = cdcl_decStackPtrs[cdcl_decLevel];
						while (currentDL < cdcl_up_end){
							aLit = *(currentDL++);
							aLit = GET_CDCL_LIT_PTR_OPPOSITE(aLit);
							//The aLit is now one of the literals that have been falsified in the last decision level. We
							//take a look at all the remaining clauses this literal has. If the clause has a satisfied
							//representative, we ignore it. If not, we add it to the clause queue for later checks.
							offset = 0;
							while (offset < GET_CDCL_LIT_PTR_NUMOCCS_USED(aLit)){
								c = GET_CDCL_LIT_PTR_OCCNUM(aLit, offset++);
								if (!IS_CDCL_LIT_SAT(GET_CDCL_CLS_REPLIT(c))){
									cdcl_clauses_Q_touchClause(c);
								}
							}
							//Now all clauses that that contained the falsified aLit are enqueued if not already satisfied.
						}
						//Now all clauses that lost a literal are enqueued.
					}
				}
			}
		} else {
			cdcl_inprocessingRules_backJump(cdcl_decLevel-1,0);
		}
		if (conflict != NULL) break;
	}

	//Finalize the tree-look by again moving into DL1.
	CDCL_UP_TERMINATE_CURRENT();

	if (numFailed > 0 && conflict == NULL){
		//We must propagate all the failed literals with opposite sign.
		cdcl_inprocessingRules_backJump(0,0);
		for (i = 0; i < numFailed; ++i){
			aLit = GET_CDCL_LIT_PTR_OPPOSITE(cdcl_inprocessingRules_trl_faileds[i]);
			v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(aLit));
			CDCL_UP_ADD_DEC_PTR(aLit, v);
		}
		conflict = cdcl_inprocessingRules_upPropagate();
	} else if (conflict != NULL){
		//We must back-jump to DL0, as this is where the conflict happened.
		cdcl_inprocessingRules_backJump(0,0);
	}

	//Reset clauses stack and queue.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;

	#ifdef VERBOSE_CDCL
	printf("<%d>[%d,%d] ", ignoredOutTrees, initVars - cdcl_vars_e_used, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}

void cdcl_inprocessingRules_intern_ternary(){
	#ifdef VERBOSE_CDCL
	printf("TER");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	cdclClause *c, *d;
	cdclLiteral *l, *lOpp, *lNew;
	uint64_t sig;
	uint32_t lOccNum, lOppOccNum, sameCount, oppCount, numLearnt, numLearntTotal = 0;
	int32_t lit1, lit2, lit3, imp;

	//We pick a literal that we want to do ternary resolution with.
	while ((l = cdcl_literals_S_pop()) != NULL){
		numLearnt = 0;
		lOpp = GET_CDCL_LIT_PTR_OPPOSITE(l);
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) == 0 || GET_CDCL_LIT_PTR_NUMOCCS_USED(lOpp) == 0) continue;
		//We walk through all the occurrences of l and search for a 3-clause.
		for (lOccNum = 0; lOccNum < GET_CDCL_LIT_PTR_NUMOCCS_USED(l); ++lOccNum){
			c = GET_CDCL_LIT_PTR_OCCNUM(l, lOccNum);
			if (GET_CDCL_CLS_SIZE(c) != 3) continue;
			//We have now found a 3-clause in c that we can try ternary resolution with. We need to locate a clause d that
			//contains lOpp as a possible resolution partner.
			for (lOppOccNum = 0; lOppOccNum < GET_CDCL_LIT_PTR_NUMOCCS_USED(lOpp); ++lOppOccNum){
				d = GET_CDCL_LIT_PTR_OCCNUM(lOpp, lOppOccNum);
				if (GET_CDCL_CLS_SIZE(d) != 3) continue;
				//We have now found a 3-clause in d that we can resolve with c.
				//When checking the signature, a zero means no clashing literal classes -> no common literal.
				if ((GET_CDCL_CLS_SIG(c) & GET_CDCL_CLS_SIG(d)) == 0ULL) continue;
				//It seems that we can find a common literal between c and d. We check how many that are the hard way.
				++cdcl_litStamp;
				sameCount = 0;
				oppCount = 0;
				SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0), cdcl_litStamp);
				SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1), cdcl_litStamp);
				SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2), cdcl_litStamp);
				if (GET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(d,0)) == cdcl_litStamp) ++sameCount;
				if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,0)) == cdcl_litStamp) ++oppCount;
				if (GET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(d,1)) == cdcl_litStamp) ++sameCount;
				if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,1)) == cdcl_litStamp) ++oppCount;
				if (GET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(d,2)) == cdcl_litStamp) ++sameCount;
				if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,2)) == cdcl_litStamp) ++oppCount;

				//Now, if sameCount is not equal 1 or oppCount is larger 1, we do not resolve. Either they have no
				//clashing literal as initially assumed, or they have more than the resolvent literal with opposite sign in
				//common. In any case, the result would be no ternary clause.
				if (sameCount != 1 || oppCount > 1) continue;

				//If we reach this position, we can resolve. We grab from clause d the literal that has the same stamp and
				//the literal that is not stamped at all. From clause c we grab the literal that is not literal 1 or 2 and
				//not the literal we resolve on.
				if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,0)) == cdcl_litStamp){
					lit1 = GET_CDCL_CLS_LITNUM(d,0);
				} else if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,1)) == cdcl_litStamp){
					lit1 = GET_CDCL_CLS_LITNUM(d,1);
				} else {
					lit1 = GET_CDCL_CLS_LITNUM(d,2);
				}

				if (       GET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(d,0)) != cdcl_litStamp
						&& GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,0)) != cdcl_litStamp){
					lit2 = GET_CDCL_CLS_LITNUM(d,0);
				} else if (GET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(d,1)) != cdcl_litStamp
						&& GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,1)) != cdcl_litStamp){
					lit2 = GET_CDCL_CLS_LITNUM(d,1);
				} else {
					lit2 = GET_CDCL_CLS_LITNUM(d,2);
				}

				if (GET_CDCL_CLS_LITNUM(c,0) != lit1 && GET_CDCL_CLS_LITNUM(c,0) != lit2 && cdcl_literals + GET_CDCL_CLS_LITNUM(c,0) != l){
					lit3 = GET_CDCL_CLS_LITNUM(c,0);
				} else if (GET_CDCL_CLS_LITNUM(c,1) != lit1 && GET_CDCL_CLS_LITNUM(c,1) != lit2 && cdcl_literals + GET_CDCL_CLS_LITNUM(c,1) != l){
					lit3 = GET_CDCL_CLS_LITNUM(c,1);
				} else {
					lit3 = GET_CDCL_CLS_LITNUM(c,2);
				}

				//Now li1, lit2, and lit3 can form a 3-clause. We must find out if there is any other 3-clause equivalent
				//to the one we could now learn. If so, this would result in a subsumption and nothing would be gained. The
				//subsumption test will work via the signature of the new clause and the literal it contains that has the
				//smallest occurrence list.
				++cdcl_litStamp;
				SET_CDCL_LIT_STAMP(lit1, cdcl_litStamp);
				SET_CDCL_LIT_STAMP(lit2, cdcl_litStamp);
				SET_CDCL_LIT_STAMP(lit3, cdcl_litStamp);
				sig = 0ULL;
				sig |= GET_CDCL_LIT_SIG(lit1);
				lNew = cdcl_literals + lit1;
				sig |= GET_CDCL_LIT_SIG(lit2);
				if (GET_CDCL_LIT_NUMOCCS_USED(lit2) < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNew)) lNew = cdcl_literals + lit2;
				sig |= GET_CDCL_LIT_SIG(lit3);
				if (GET_CDCL_LIT_NUMOCCS_USED(lit3) < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNew)) lNew = cdcl_literals + lit3;
				//Literal lNew is now the literal that has the smallest occurrence list. We check all the clauses in here if
				//any of them matches the signature.
				for (sameCount = 0; sameCount < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNew); ++sameCount){
					d = GET_CDCL_LIT_PTR_OCCNUM(lNew, sameCount);
					if (GET_CDCL_CLS_SIZE(d) != 3 || sig != GET_CDCL_CLS_SIG(d)) continue;
					//Here, it might be that clause d is equal to what we can learn. We check if d contains lit1, lit2
					//and lit3 via looking at the stamps.
					if (       GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,0)) == cdcl_litStamp
							&& GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,1)) == cdcl_litStamp
							&& GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,2)) == cdcl_litStamp){
						//All three literals match. We cannot learn the new clause as it is equal to d.
						break;
					}
				}
				if (sameCount < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNew)) {
					continue;
				}

				//We must now find out if the new clause can be subsumed by any two clause, if so, this would result in a
				//subsumption and nothing would be gained. It suffices to check the implication lists of two of the opposite
				//literals in the clause. If any of them contains one of our literals, we can stop.
				lNew = cdcl_literals - lit1;
				for (sameCount = 0; sameCount < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNew); sameCount += 2){
					imp = GET_CDCL_LIT_PTR_IMPNUM(lNew, sameCount);
					if (GET_CDCL_LIT_STAMP(imp) == cdcl_litStamp){
						break;
					}
				}
				if (sameCount < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNew)){
					continue;
				}
				lNew = cdcl_literals - lit2;
				for (sameCount = 0; sameCount < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNew); sameCount += 2){
					imp = GET_CDCL_LIT_PTR_IMPNUM(lNew, sameCount);
					if (GET_CDCL_LIT_STAMP(imp) == cdcl_litStamp){
						break;
					}
				}
				if (sameCount < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNew)){
					continue;
				}
				//Here at this point, the clause we can learn passed all the tests. We add it.
				cdcl_inprocessingRules_intern_addLearnedTernaryClause(lit1, lit2, lit3);
				cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(cdcl_cls_l[cdcl_cls_l_used-1]);

				if (++numLearnt == param_cdclIRTERNARYLitMax) break;
			}
			if (numLearnt == param_cdclIRTERNARYLitMax) break;
		}
		numLearntTotal += numLearnt;
		if (numLearntTotal >= param_cdclIRTERNARYMax) break;
	}
	cdcl_literals_S_flush();

	#ifdef VERBOSE_CDCL
	printf("[%d,%d,%d] ", initVars - cdcl_vars_e_used, remLits, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
}

cdclClause* cdcl_inprocessingRules_intern_els_replaceLit(cdclLiteral* toReplace, cdclLiteral* replaceWith){
	cdclClause *c;
	cdclVariable *v;
	uint64_t sig;
	uint32_t i, occLitPosition, found, taut;
	int32_t toRID = GET_CDCL_LIT_ID_FROM_PTR(toReplace);
	int32_t reWID = GET_CDCL_LIT_ID_FROM_PTR(replaceWith);

	while (GET_CDCL_LIT_PTR_NUMOCCS_USED(toReplace) > 0){
		//Grab the next clause that contains toReplace.
		c = GET_CDCL_LIT_PTR_OCCNUM(toReplace, 0);
		occLitPosition = GET_CDCL_LIT_PTR_OCCLITPOSNUM(toReplace,0);

		sig = 0ULL;
		found = 0;
		taut = 0;

		//Several things are performed in the following loop: 1. Look at all the literals and grab the signature bit of
		//those not equal to toReplace. 2. See if replaceWith is already in the clause.
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			//Skip the literal we want to replace.
			if (GET_CDCL_CLS_LITNUM(c, i) == toRID){
				continue;
			}
			//Retain if the literal that we want to replace with is already found.
			if (GET_CDCL_CLS_LITNUM(c, i) == reWID){
				found = 1;
			}

			//Check if the opposite of the replacement is in the clause.
			if (GET_CDCL_CLS_LITNUM(c, i) == -reWID){
				taut = 1;
				break;
			}

			//Update the signature with all the literals we encounter not equal to the one we replace.
			sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, i));
		}

		//If the clause becomes a tautology, we just drop it.
		if (taut){
			cdcl_inprocessingRules_intern_disableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
			cdcl_inprocessingRules_intern_deleteClause(c);
			continue;
		}

		//Disable the clause.
		cdcl_inprocessingRules_intern_disableClause(c);

		//If we have not found the literal we replace with, we must add its signature bit manually and write the replace
		//literal to the position where the old literal was.
		if (!found){
			//We did not find the literal we replace with. We manually add the signature bit. We override the literal.
			sig |= GET_CDCL_LIT_SIG(reWID);
			SET_CDCL_CLS_LITNUM(c, occLitPosition, reWID);
		} else {
			//We did find the replacement literal. This means that we effectively remove a literal form this clause.
			//We override the position of the literal to replace with the last literal in the clause.
			SET_CDCL_CLS_SIZE_DEC(c);
			SET_CDCL_CLS_LITNUM(c, occLitPosition, GET_CDCL_CLS_LITNUM(c, GET_CDCL_CLS_SIZE(c)));
			SET_CDCL_CLS_OCCPOSNUM(c, occLitPosition, GET_CDCL_CLS_OCCPOSNUM(c,GET_CDCL_CLS_SIZE(c)));
		}

		//If the clause became unit, we do not have to re-enable it. If not, we can re-enable it normally.
		if (GET_CDCL_CLS_SIZE(c) == 1){
			//The clause became unit because of the replacement. We can now learn that -reWID is a failed literal and put
			//reWID on the stack. If this fails it constitutes a global conflict.
			if (CDCL_UP_IS_OPPLIT_ON_S(reWID)){
				//This constitutes a global conflict.
				return (cdclClause*) 1;
			} else if (!CDCL_UP_IS_LIT_ON_S(reWID)){
				//We can add the literal to the UP queue.
				v = cdcl_variables + abs(reWID);
				CDCL_UP_ADD_DEC(reWID, v);
			}//Else the literal is already enqueued.
			//This unit is now taken care of.
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
			cdcl_inprocessingRules_intern_deleteClause(c);
		} else if (GET_CDCL_CLS_SIZE(c) == 2){
			//Update the clause signature.
			SET_CDCL_CLS_SIG(c, sig);
			//Set the representative.
			SET_CDCL_CLS_REPLIT(c, reWID);
			//Reset the watchers.
			SET_CDCL_CLS_W1(c,0);
			SET_CDCL_CLS_WHEREW1(c, 0);
			SET_CDCL_CLS_W2(c,0);
			SET_CDCL_CLS_WHEREW2(c, 0);
			//The clause is at least binary.
			cdcl_inprocessingRules_intern_enableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(c);
		} else {
			//Update the clause signature.
			SET_CDCL_CLS_SIG(c, sig);
			if (GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_LBD(c)){
				SET_CDCL_CLS_LBD(c, GET_CDCL_CLS_SIZE(c));
			}
			//We update the clause representative literal, watcher 1 and watcher 2.
			if (GET_CDCL_CLS_REPLIT(c) == toRID){
				//We replaced the representative.
				SET_CDCL_CLS_REPLIT(c, reWID);
			}
			if (GET_CDCL_CLS_W1(c) == toRID){
				//We replaced the first watcher.
				if (GET_CDCL_CLS_W2(c) != reWID){
					SET_CDCL_CLS_W1(c, reWID);
				} else {
					for (occLitPosition = 0; occLitPosition < GET_CDCL_CLS_SIZE(c); ++occLitPosition){
						if (GET_CDCL_CLS_LITNUM(c, occLitPosition) != reWID){
							SET_CDCL_CLS_W1(c, GET_CDCL_CLS_LITNUM(c,occLitPosition));
							break;
						}
					}
				}
			} else if (GET_CDCL_CLS_W2(c) == toRID){
				//We replaced the second watcher.
				if (GET_CDCL_CLS_W1(c) != reWID){
					SET_CDCL_CLS_W2(c, reWID);
				}  else {
					for (occLitPosition = 0; occLitPosition < GET_CDCL_CLS_SIZE(c); ++occLitPosition){
						if (GET_CDCL_CLS_LITNUM(c, occLitPosition) != reWID){
							SET_CDCL_CLS_W2(c, GET_CDCL_CLS_LITNUM(c,occLitPosition));
							break;
						}
					}
				}
			}
			cdcl_inprocessingRules_intern_enableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(c);

		}
		//The literal has now been removed from the clause successfully.
	}

	return NULL;
}

uint32_t cdcl_inprocessingRules_intern_els_strongConnect(cdclLiteral* l, uint32_t index){
	cdclLiteral *impLit;
	uint32_t i;

	SET_CDCL_LIT_PTR_ELS_INDEX(l,index);
	SET_CDCL_LIT_PTR_ELS_LOWLINK(l, index);
	++index;
	*(cdcl_inprocessingRules_els_lStack_current++) = l;
	SET_CDCL_LIT_PTR_ELS_ISINS(l, 1U);

	for (i= 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i+=2){
		impLit = cdcl_literals + GET_CDCL_LIT_PTR_IMPNUM(l,i);
		if (GET_CDCL_LIT_PTR_ELS_INDEX(impLit) == 0U){
			//Not yet marked. Recurse on implied literal.
			index = cdcl_inprocessingRules_intern_els_strongConnect(impLit, index);
			if (GET_CDCL_LIT_PTR_ELS_LOWLINK(impLit) < GET_CDCL_LIT_PTR_ELS_LOWLINK(l)){
				SET_CDCL_LIT_PTR_ELS_LOWLINK(l, GET_CDCL_LIT_PTR_ELS_LOWLINK(impLit));
			}
		} else if (GET_CDCL_LIT_PTR_ELS_ISINS(impLit)){
			//Already in stack -- implied literal is in the current SCC.
			if (GET_CDCL_LIT_PTR_ELS_INDEX(impLit) < GET_CDCL_LIT_PTR_ELS_LOWLINK(l)){
				SET_CDCL_LIT_PTR_ELS_LOWLINK(l, GET_CDCL_LIT_PTR_ELS_INDEX(impLit));
			}
		}
	}

	//If l is a root, then pop the stack and generate an SCC class.
	if (GET_CDCL_LIT_PTR_ELS_LOWLINK(l) == GET_CDCL_LIT_PTR_ELS_INDEX(l)){
		//Allocate new equivalence class if necessary.
		CDCL_IR_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--cdcl_inprocessingRules_els_lStack_current);
			SET_CDCL_LIT_PTR_ELS_ISINS(impLit, 0U);

			if (GET_CDCL_LIT_PTR_STAMP(impLit) != cdcl_litStamp){
				CDCL_IR_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_CDCL_LIT_PTR_STAMP(impLit, cdcl_litStamp);
				SET_CDCL_LIT_PTR_STAMP(GET_CDCL_LIT_PTR_OPPOSITE(impLit), cdcl_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++cdcl_inprocessingRules_els_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			cdcl_inprocessingRules_els_classes_memUsed[cdcl_inprocessingRules_els_classes_used] = 0;
		}
	}

	return index;
}

cdclClause* cdcl_inprocessingRules_intern_els(){
	#ifdef VERBOSE_CDCL
	printf("ELS");
	int32_t initVars = cdcl_vars_e_used;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif
	cdclClause *conflict = NULL;
	cdclLiteral *l, *classRep;
	cdclVariable *v;
	uint32_t i,j, index = 1;
	int32_t toRID, reWID;

	//We back-jump in preparation of unit propagation.
	cdcl_inprocessingRules_backJump(0,0);

	//Initialize the ELS literal stack to be empty.
	cdcl_inprocessingRules_els_lStack_current = cdcl_inprocessingRules_els_lStack + 1;

	//We prepare the literal stamp to present classes only once.
	++cdcl_litStamp;

	//Initialize the classes information.
	cdcl_inprocessingRules_els_classes_used = 0;
	for (i = 0; i < cdcl_inprocessingRules_els_classes_avail; ++i){
		cdcl_inprocessingRules_els_classes_memUsed[i] = 0;
	}

	//Prepare all remaining literals.
	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		l = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		SET_CDCL_LIT_PTR_ELS_INDEX(l, 0);
		SET_CDCL_LIT_PTR_ELS_LOWLINK(l, 0);
		SET_CDCL_LIT_PTR_ELS_ISINS(l, 0);
		l = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		SET_CDCL_LIT_PTR_ELS_INDEX(l, 0);
		SET_CDCL_LIT_PTR_ELS_LOWLINK(l, 0);
		SET_CDCL_LIT_PTR_ELS_ISINS(l, 0);
	}

	for (i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		l = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		//Check if the literal has been checked by strong-connect.
		if (GET_CDCL_LIT_PTR_ELS_INDEX(l) == 0){
			index = cdcl_inprocessingRules_intern_els_strongConnect(l, index);
		}
		l = GET_CDCL_LIT_PTR_OPPOSITE(l);
		if (GET_CDCL_LIT_PTR_ELS_INDEX(l) == 0){
			index = cdcl_inprocessingRules_intern_els_strongConnect(l, index);
		}
	}

	//We walk through all the found equivalence classes and replace the literals.
	for (i = 0; i < cdcl_inprocessingRules_els_classes_used; ++i){
		//For class i, we pick the class representative (the first literal in the class).
		classRep = cdcl_inprocessingRules_els_classes[i][0];
		reWID = GET_CDCL_LIT_ID_FROM_PTR(classRep);
		//For all the remaining literals in the class, we replace their occurrence with the representative.
		for (j = 1; j < cdcl_inprocessingRules_els_classes_memUsed[i]; ++j){
			//Grab the literal and variable to replace.
			l = cdcl_inprocessingRules_els_classes[i][j];
			toRID = GET_CDCL_LIT_ID_FROM_PTR(l);
			v = cdcl_variables + abs(toRID);

			//Replace the occurrence of l with classRep.
			conflict = cdcl_inprocessingRules_intern_els_replaceLit(l, classRep);
			if (conflict != NULL){
				#ifdef VERBOSE_CDCL
				printf(" representative ");
				#endif
				return conflict;
			}

			//Replace the occurrence of -l with -classRep.
			conflict = cdcl_inprocessingRules_intern_els_replaceLit(GET_CDCL_LIT_PTR_OPPOSITE(l),GET_CDCL_LIT_PTR_OPPOSITE(classRep));
			if (conflict != NULL){
				#ifdef VERBOSE_CDCL
				printf(" representative ");
				#endif
				return conflict;
			}

			//Mark the replaced variable with its representative.
			if ((toRID < 0 && reWID < 0) || (toRID > 0 && reWID > 0)){
				SET_CDCL_VAR_PTR_ISEQUIVTO(v, abs(reWID));
			} else {
				SET_CDCL_VAR_PTR_ISEQUIVTO(v, -abs(reWID));
			}

			//Disable the replaced variable.
			cdcl_inprocessingRules_intern_disableVariable(v);
		}
	}

	//Finally, we perform unit propagation.
	conflict = cdcl_inprocessingRules_upPropagate();
	#ifdef VERBOSE_CDCL
	printf("[%d,%d] ", initVars - cdcl_vars_e_used, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}


cdclClause* cdcl_inprocessingRules_intern_niver(){
	//This method implements NIVER for the CDCL in-processing.
	cdclClause *conflict = NULL;
	#ifdef VERBOSE_CDCL
	printf("NIV");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif

	cdclLiteral *lOuter, *lInner;
	cdclClause *c, *d;
	cdclVariable *v;
	uint64_t litSig, cSigSimp;
	uint32_t totalClauses, temp,i,j,k;
	int32_t lit;


	while ((lOuter = cdcl_literals_Q_dequeueBest()) != NULL){
		//We pick the best outer variables and its inner correspondent.
		lit = GET_CDCL_LIT_ID_FROM_PTR(lOuter);
		lInner = GET_CDCL_LIT_PTR_OPPOSITE(lOuter);
		cdcl_literals_Q_delete(lInner);
		totalClauses = GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) + GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner);

		//We ignore the variable if it is assigned.
		if (!IS_CDCL_VAR_UNASSIGNED(abs(lit))){
			continue;
		}
		//We check if the literals are pure.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) == 0 || GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner) == 0){
			//Yes, that is the case. We will now perform the unit propagation in DL0.
			cdcl_inprocessingRules_backJump(0,0);
			v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(lOuter));
			if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) == 0){
				CDCL_UP_ADD_DEC_PTR(lInner, v);
			} else {
				CDCL_UP_ADD_DEC_PTR(lOuter, v);
			}
			conflict = cdcl_inprocessingRules_upPropagate();
			if (conflict != NULL) break;
			continue;
		}

		//After this check, we will perform NIVER, if and only if the number of resolvents added is not exceeding the clauses
		//that will be removed by this operation.
		temp = 0;
		//We now walk through the occurrence list of lOuter, for the picked clause c we kill the bit for lit if possible.
		//If this is not possible, we can go on with the next clause right away (all signature tests would fail anyway).
		litSig = GET_CDCL_LIT_SIG(lit);
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter); ++i){
			c = GET_CDCL_LIT_PTR_OCCNUM(lOuter, i);
			cSigSimp = GET_CDCL_CLS_SIG(c);
			if (GET_CDCL_CLS_SIZE(c) == POPCOUNT_UINT64(cSigSimp)){
				//Since each literal has its own bit in the signature, we can kill the bit of lit.
				cSigSimp &= ~litSig;
			} else {
				//We must find out the hard way if there is a literal in c different from lit that has the same signature.
				for (j = 0; j < GET_CDCL_CLS_SIZE(c); ++j){
					if (GET_CDCL_CLS_LITNUM(c,j) != lit && GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c,j)) == litSig){
						break;
					}
				}
				if (j == GET_CDCL_CLS_SIZE(c)){
					//We have not found another literal that causes the same signature as lit. We can kill the bit.
					cSigSimp &= ~litSig;
				}
			}
			//Here we check if we successfully killed the bit. If not, we must go on with the next clause c, or abort
			//the tests (if we could not get above the border of totalClauses anyway).
			if ((cSigSimp & litSig) != 0ULL){
				//We were unable to kill the bit. We must go on with the next clause.
				continue;
			}
			//Otherwise, we can rotate the signature and check all occurrences in lInner.
			cSigSimp = ROTATE_32RIGHT_UINT64(cSigSimp);
			//Now we use this signature to check against all occurrences of lInner. Those, were no tautology would be
			//possible increase temp.
			for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner); ++j){
				d = GET_CDCL_LIT_PTR_OCCNUM(lInner, j);
				if ((cSigSimp & GET_CDCL_CLS_SIG(d)) == 0ULL) ++temp;
			}

			//We can stop if it is already too much.
			if (temp > totalClauses) {
				break;
			}
		}
		//As soon as temp > totalClauses, we know that this cannot work for NIVER, we then go on.
		if (temp > totalClauses) {
			continue;
		}
		//It seems that we are going to perform the resolution. The number of resolvents must be set to a worst case of
		//posClauses*negClauses, because all the resolvents could survive the resolution.
		CDCL_IR_RESET_ALL_RESOLVENTS();
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter); ++i){
			c = GET_CDCL_LIT_PTR_OCCNUM(lOuter, i);
			for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner); ++j){
				d = GET_CDCL_LIT_PTR_OCCNUM(lInner, j);
				//We now prepare the copying operation. The literal we resolve on is lit in c and -lit in d.
				CDCL_IR_RESET_RESOLVENT(cdcl_inprocessingRules_resUsed);
				++cdcl_litStamp;
				//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
				for (k = 0; k < GET_CDCL_CLS_SIZE(c); ++k){
					if (GET_CDCL_CLS_LITNUM(c,k) != lit){
						CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(c,k));
						SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,k), cdcl_litStamp);
					}
				}
				//We now copy all the literals from d, we invert the sign of lit for this, as this is what we ignore in d.
				lit = -lit;
				for (k = 0; k < GET_CDCL_CLS_SIZE(d); ++k){
					//We ignore the resolution literal and any literal already in the resolvent.
					if (GET_CDCL_CLS_LITNUM(d,k) != lit && GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k)) != cdcl_litStamp){
						//We check if adding this literal will result in a tautology. If not, we add it. If so, we break.
						if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,k)) == cdcl_litStamp){
							//We break early since this will end in a tautology.
							break;
						}
						//No tautology yet.
						CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(d,k));
						SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k), cdcl_litStamp);
					}
				}
				//We revert the temporary sign inversion.
				lit = -lit;
				if (k < GET_CDCL_CLS_SIZE(d)){
					//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
					continue;
				}
				//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
				CDCL_IR_INCREASE_NUM_RESOLVENTS( temp );
				//If we are now above the border of totalClauses, then we can abort the NIVER operations on this variable, as
				//NIVER will not be performed.
				if (cdcl_inprocessingRules_resMemUsed[cdcl_inprocessingRules_resUsed-1] == 1){
					//The created resolvent is a unit clause. Independent of everything we did so far, we will propagate
					//this finally.
					cdcl_inprocessingRules_backJump(0,0);
					v = cdcl_variables + abs(cdcl_inprocessingRules_res[cdcl_inprocessingRules_resUsed-1][0]);
					CDCL_UP_ADD_DEC(cdcl_inprocessingRules_res[cdcl_inprocessingRules_resUsed-1][0], v);
					conflict = cdcl_inprocessingRules_upPropagate();
					cdcl_inprocessingRules_resUsed = INT32_MAX;
				}
				if (cdcl_inprocessingRules_resUsed > totalClauses){
					break;
				}
			}
			if (cdcl_inprocessingRules_resUsed > totalClauses){
				break;
			}
		}
		if (conflict != NULL) break;
		//This concludes the learning operations. The following check determines if we are going to perform the variable
		//elimination by removing the old clauses and adding the new resolvents in stead.
		if (cdcl_inprocessingRules_resUsed > totalClauses){
			//Nope, we drop everything and go on with the next variable.
			continue;
		}
		//We perform the elimination. We transfer the larger set to the removed clauses array.
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner) > 0){
			d = GET_CDCL_LIT_PTR_OCCNUM(lInner, 0);
			cdcl_inprocessingRules_intern_disableClause(d);
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(d);
			cdcl_inprocessingRules_intern_makeClauseRemoved(d, lit, CDCL_IR_REMCLS_NIVER);
			cdcl_inprocessingRules_intern_deleteClause(d);
		}
		//We then transfer the smaller set to the removed clauses array.
		while (GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) > 0){
			c = GET_CDCL_LIT_PTR_OCCNUM(lOuter, 0);
			cdcl_inprocessingRules_intern_disableClause(c);
			cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
			cdcl_inprocessingRules_intern_makeClauseRemoved(c, lit, CDCL_IR_REMCLS_NIVER);
			cdcl_inprocessingRules_intern_deleteClause(c);
		}
		//We finally disable the variable.
		v = cdcl_variables + abs(lit);
		cdcl_inprocessingRules_intern_disableVariable(v);

		//We now add all the resolvents as new original clauses to the formula.
		for (i = 0; i < cdcl_inprocessingRules_resUsed; ++i){
			cdcl_inprocessingRules_intern_addOriginalClause(cdcl_inprocessingRules_res[i],cdcl_inprocessingRules_resMemUsed[i]);
			c = cdcl_cls_o[cdcl_cls_o_used-1];
			cdcl_inprocessingRules_intern_QSUpdateClauseEnabling(c);
		}
		//This concludes NIVER for variable v.
	}

	#ifdef VERBOSE_CDCL
	printf("[%d,%d] ", initVars - cdcl_vars_e_used, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}

cdclClause* cdcl_inprocessingRules_intern_ge_extract(cdclVariable *v){
	int32_t i, j, k, ignoreLit, hadUnits = 0;
	cdclClause *c, *d;
	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
	//In case variable v has a definition provided in the sets G_-v and G_v, we can remove the variable from the formula by
	//performing the following steps.
	//		1. Disable the clauses defining the gate.
	//		2. Creating the resolvents of set R_v and G_-v, as well as R_-v and G_v (resolve the remaining clauses).
	//		3. Making all the clauses participating in the gate definition removed (v/-v being the litOfInterest).
	//		4. Disabling all remaining clauses that contain the defined variable in R_v and R_-v.
	//		5. Disable the variable v.
	//		6. Add the resolvents to the formula.

	//1. We have identified a gate, and will first make the clauses defining it disabled. This way they do not appear in any
	//occurrence or watcher lists or binary implication lists anymore, but their literal arrays are still available.
	for (i = 0; i < cdcl_inprocessingRules_G_negVUsed; ++i){
		cdcl_inprocessingRules_intern_disableClause(cdcl_inprocessingRules_G_negV[i]);
	}
	for (i = 0; i < cdcl_inprocessingRules_G_posVUsed; ++i){
		cdcl_inprocessingRules_intern_disableClause(cdcl_inprocessingRules_G_posV[i]);
	}
	//2. We must now create all the resolvents, one set is G_-v and R_v (the remaining other clauses containing v), the other
	//set is G_v and R_-v (the remaining other clauses containing -v). Since we already disabled the clauses defining the
	//gate, all the remaining occurrences belong to either R_v or R_-v. We will pick one element from G_negV/G_posV after
	//another and resolve them with all remaining occurrences.

	CDCL_IR_RESET_ALL_RESOLVENTS();

	//Resolve G_-v with R_v.
	for (i = 0; i < cdcl_inprocessingRules_G_negVUsed; ++i){
		c = cdcl_inprocessingRules_G_negV[i];
		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			d = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			//We now prepare the copying operation. The literal we resolve on is -ignoreLit in c and ignoreLit in d.
			CDCL_IR_RESET_RESOLVENT(cdcl_inprocessingRules_resUsed);
			++cdcl_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			ignoreLit = -GET_CDCL_VAR_PTR_ID(v);
			for (k = 0; k < GET_CDCL_CLS_SIZE(c); ++k){
				if (GET_CDCL_CLS_LITNUM(c, k) != ignoreLit){
					CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(c, k));
					SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c, k), cdcl_litStamp);
				}
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			for (k = 0; k < GET_CDCL_CLS_SIZE(d); ++k){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (GET_CDCL_CLS_LITNUM(d,k) != ignoreLit && GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k)) != cdcl_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it.
					if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,k)) == cdcl_litStamp){
						//We break early since the resolvent would be a tautology.
						break;
					}
					//No tautology yet.
					CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(d,k));
					SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k), cdcl_litStamp);
				}
			}
			if (k < GET_CDCL_CLS_SIZE(d)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			CDCL_IR_INCREASE_NUM_RESOLVENTS( k );
			if (cdcl_inprocessingRules_resMemUsed[cdcl_inprocessingRules_resUsed-1] == 1){
				hadUnits = 1;
			}
		}
	}

	//Resolve G_v with R_-v.
	for (i = 0; i < cdcl_inprocessingRules_G_posVUsed; ++i){
		c = cdcl_inprocessingRules_G_posV[i];
		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			d = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			//We now prepare the copying operation. The literal we resolve on is ignoreLit in c and -ignoreLit in d.
			CDCL_IR_RESET_RESOLVENT(cdcl_inprocessingRules_resUsed);
			++cdcl_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			ignoreLit = GET_CDCL_VAR_PTR_ID(v);
			for (k=0; k < GET_CDCL_CLS_SIZE(c); ++k){
				if (GET_CDCL_CLS_LITNUM(c,k) != ignoreLit){
					CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(c,k));
					SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,k), cdcl_litStamp);
				}
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			for (k=0; k < GET_CDCL_CLS_SIZE(d); ++k){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (GET_CDCL_CLS_LITNUM(d,k) != ignoreLit && GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k)) != cdcl_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it.
					if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(d,k)) == cdcl_litStamp){
						//We break early since the resolvent would be a tautology.
						break;
					}
					//No tautology yet.
					CDCL_IR_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CDCL_CLS_LITNUM(d,k));
					SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,k), cdcl_litStamp);
				}
			}

			if (k < GET_CDCL_CLS_SIZE(d)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			CDCL_IR_INCREASE_NUM_RESOLVENTS( k );
			if (cdcl_inprocessingRules_resMemUsed[cdcl_inprocessingRules_resUsed-1] == 1){
				hadUnits = 1;
			}
		}
	}
	//We have now created all the resolvents.

	//3. Make the clauses defining the gate removed using v->id and -v->id as the literal of interest.
	for (i = 0; i < cdcl_inprocessingRules_G_negVUsed; ++i){
		cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(cdcl_inprocessingRules_G_negV[i]);
		cdcl_inprocessingRules_intern_makeClauseRemoved(cdcl_inprocessingRules_G_negV[i], -GET_CDCL_VAR_PTR_ID(v), CDCL_IR_REMCLS_GE);
		cdcl_inprocessingRules_intern_deleteClause(cdcl_inprocessingRules_G_negV[i]);
	}
	for (i = 0; i < cdcl_inprocessingRules_G_posVUsed; ++i){
		cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(cdcl_inprocessingRules_G_posV[i]);
		cdcl_inprocessingRules_intern_makeClauseRemoved(cdcl_inprocessingRules_G_posV[i],  GET_CDCL_VAR_PTR_ID(v), CDCL_IR_REMCLS_GE);
		cdcl_inprocessingRules_intern_deleteClause(cdcl_inprocessingRules_G_posV[i]);
	}

	//4. Disable all remaining clauses that contain the defined variable in R_v and R_-v.
	while (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > 0){
		c = GET_CDCL_LIT_PTR_OCCNUM(lPos, 0);
		cdcl_inprocessingRules_intern_disableClause(c);
		cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
		cdcl_inprocessingRules_intern_deleteClause(c);
	}
	while (GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg) > 0){
		c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, 0);
		cdcl_inprocessingRules_intern_disableClause(c);
		cdcl_inprocessingRules_intern_QSUpdateClauseDisabling(c);
		cdcl_inprocessingRules_intern_deleteClause(c);
	}

	//5. Disable variable v.
	cdcl_inprocessingRules_intern_disableVariable(v);

	//6. Add the created resolvents.
	if (hadUnits){
		cdcl_inprocessingRules_backJump(0,0);
	}
	for (i = 0; i < cdcl_inprocessingRules_resUsed; ++i){
		if (cdcl_inprocessingRules_resMemUsed[i] == 1){
			if (CDCL_UP_IS_OPPLIT_ON_S(cdcl_inprocessingRules_res[i][0])){
				return (cdclClause*)1;
			} else if (!CDCL_UP_IS_LIT_ON_S(cdcl_inprocessingRules_res[i][0])){
				v = cdcl_variables + abs(cdcl_inprocessingRules_res[i][0]);
				CDCL_UP_ADD_DEC(cdcl_inprocessingRules_res[i][0], v);
			}
		} else {
			cdcl_inprocessingRules_intern_addOriginalClause(cdcl_inprocessingRules_res[i], cdcl_inprocessingRules_resMemUsed[i]);
		}
	}
	if (hadUnits){
		#ifdef VERBOSE_CDCL
		printf("<U>");
		#endif
		if (cdcl_inprocessingRules_upPropagate() != NULL){
			return (cdclClause*)1;
		}
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge_AND(cdclVariable *v){
	//This method will check if the given variable has an AND gate definition.
	//For three variables this looks like this for v:
	//(v or -a or -b or -c)
	//(-v or a)
	//...
	//(-v or c)
	//v <=> a = b = c = 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i,j, theSize = INT32_MAX;
	cdclClause *c = NULL;
	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);

	//We reset the gate definition arrays, we are about to need them.
	CDCL_IR_RESET_GDEFARRAYS();

	//In order to detect such an AND definition, we will first go through all the implications of the positive v, stamp
	//each of the implied literals. After that, we walk through all the occurrences of the positive v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an AND gate.
	//First, we stamp all implications of the positive v (by checking the implications of -v).
	++cdcl_litStamp;
	SET_CDCL_LIT_PTR_STAMP(lNeg, cdcl_litStamp);
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos); i+=2){
		SET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lPos,i), cdcl_litStamp);
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (-v or a) are now stamped. We now
	//check if any of the clauses that contains +v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an AND gate.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++i){
		c = GET_CDCL_LIT_PTR_OCCNUM(lPos, i);
		if (GET_CDCL_CLS_SIZE(c) < 3 || GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) < GET_CDCL_CLS_SIZE(c) - 1) continue;
		//Check if all opposite literals in c are stamped.
		for (j=0; j < GET_CDCL_CLS_SIZE(c); ++j){
			if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,j)) != cdcl_litStamp){
				//This opposite literal was not stamped. Clause cannot be used.
				break;
			}
		}
		if (j == GET_CDCL_CLS_SIZE(c)){
			//In case we did not break early, we have found a clause c used in the definition of an AND gate. We store the size
			//of the large clause (the smaller, the better).
			if (GET_CDCL_CLS_SIZE(c) < theSize){
				//Yes, the new found clause used in the AND definition is smaller than the old one. Since it can be only
				//one clause containing the variable v positively in the AND gate definition, we just override what we had.
				theSize = GET_CDCL_CLS_SIZE(c);
				cdcl_inprocessingRules_G_posV[0] = c;
				cdcl_inprocessingRules_G_posVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an AND gate using clause G_posV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++cdcl_litStamp;
		for (j = 0; j < GET_CDCL_CLS_SIZE(cdcl_inprocessingRules_G_posV[0]); ++j){
			SET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(cdcl_inprocessingRules_G_posV[0],j), cdcl_litStamp);
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos); i+=2){
			if (GET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lPos, i)) == cdcl_litStamp){
				SET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lPos, i), cdcl_litStamp -1);
				//Put this clause into the G_negV array.
				CDCL_IR_ADD_CLS_TO_GNEGV(GET_CDCL_LIT_PTR_IMPNUM_REASON(lPos, i));
			}
		}

		//We have now collected all the defining clauses for the AND gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (cdcl_inprocessingRules_G_negVUsed*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos)-cdcl_inprocessingRules_G_posVUsed)
				+cdcl_inprocessingRules_G_posVUsed*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)-cdcl_inprocessingRules_G_negVUsed)
			< param_cdclIRGEMaxAONN) {
			//It is time to extract the gate.
			c = cdcl_inprocessingRules_intern_ge_extract(v);
			if (c != NULL){
				return c;
			}
		}
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge_OR(cdclVariable *v){
	//This method will check if the given variable has an OR gate definition.
	//For three variables this looks like this for v:
	//(-v or a or b or c)
	//(v or -a)
	//...
	//(v or -c)
	//v <=> a + b + c >= 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i,j, theSize = INT32_MAX;
	cdclClause *c = NULL;
	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);

	//We reset the gate definition arrays, we are about to need them.
	CDCL_IR_RESET_GDEFARRAYS();

	//In order to detect such an OR definition, we will first go through all the implications of the negative v and stamp
	//each of the implied literals. After that, we walk through all the occurrences of the negative v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an OR gate.
	//First we stamp all implications of the negative v (by checking the implications of +v).
	++cdcl_litStamp;
	SET_CDCL_LIT_PTR_STAMP(lPos, cdcl_litStamp);
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg); i+=2){
		SET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lNeg,i), cdcl_litStamp);
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (v or -a) are now stamped. We now
	//check if any of the clauses that contains -v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an OR gate.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++i){
		c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, i);
		if (GET_CDCL_CLS_SIZE(c) < 3 || GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg) < GET_CDCL_CLS_SIZE(c) - 1) continue;
		//Check if all opposite literals in c are stamped.
		for (j=0; j < GET_CDCL_CLS_SIZE(c); ++j){
			if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,j)) != cdcl_litStamp){
				//This opposite literal was not stamped. Clause cannot be used.
				break;
			}
		}
		if (j == GET_CDCL_CLS_SIZE(c)){
			//In case we did not break early, we have found a clause c used in the definition of an AND gate. We store the size
			//of the large clause (the smaller, the better).
			if (GET_CDCL_CLS_SIZE(c) < theSize){
				//Yes, the new found clause used in the AND definition is smaller than the old one. Since it can be only
				//one clause containing the variable v positively in the AND gate definition, we just override what we had.
				theSize = GET_CDCL_CLS_SIZE(c);
				cdcl_inprocessingRules_G_negV[0] = c;
				cdcl_inprocessingRules_G_negVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an OR gate using clause G_negV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++cdcl_litStamp;
		for (j = 0; j < GET_CDCL_CLS_SIZE(cdcl_inprocessingRules_G_negV[0]); ++j){
			SET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(cdcl_inprocessingRules_G_negV[0],j), cdcl_litStamp);
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg); i+=2){
			if (GET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lNeg, i)) == cdcl_litStamp){
				SET_CDCL_LIT_STAMP(GET_CDCL_LIT_PTR_IMPNUM(lNeg, i), cdcl_litStamp -1);
				//Put this clause into the G_negV array.
				CDCL_IR_ADD_CLS_TO_GPOSV(GET_CDCL_LIT_PTR_IMPNUM_REASON(lNeg, i));
			}
		}
		//We have now collected all the defining clauses for the OR gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (cdcl_inprocessingRules_G_negVUsed*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos)-cdcl_inprocessingRules_G_posVUsed)
				+cdcl_inprocessingRules_G_posVUsed*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)-cdcl_inprocessingRules_G_negVUsed)
			< param_cdclIRGEMaxAONN) {
			//It is time to extract the gate.
			c = cdcl_inprocessingRules_intern_ge_extract(v);
			if (c != NULL){
				return c;
			}
		}
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge_SIM3(cdclVariable* v){
	//This method extracts similarity gates for exactly three variables. This looks as follows. Given clauses
	//Positive side:   Negative side:
	//X1  (a -b -c)      Y1 (-a  b  c)
	//X2  (a -b  c)      Y2 (-a -b  c)
	//X3  (a  b -c)      Y3 (-a  b -c)
	//This requires a = b = c.
	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
	cdclClause *c, *conflict = NULL;
	cdclVariable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++i){
		c = GET_CDCL_LIT_PTR_OCCNUM(lPos,i);
		if (GET_CDCL_CLS_SIZE(c) != 3) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++cdcl_litStamp;
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0), cdcl_litStamp);
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1), cdcl_litStamp);
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2), cdcl_litStamp);
		CDCL_IR_RESET_GDEFARRAYS();
		CDCL_IR_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 and X3 that have 2 stamped literals and 1 opposite stamped literal.
		for (j = i+1; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos,j);
			if (GET_CDCL_CLS_SIZE(c) != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if (sCountP != 2 || sCountN != 1) continue;
			//It has. We must now check if it actually new for the positive set.
			for (k = 0; k < cdcl_inprocessingRules_G_posVUsed; ++k){
				if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_posV[k]) == GET_CDCL_CLS_SIG(c)) break;
			}
			if (k < cdcl_inprocessingRules_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of c. This in turn means that the clauses are the same.
				continue;
			}
			//Yes, the clause is new. We add it to the positive side.
			CDCL_IR_ADD_CLS_TO_GPOSV(c);
			if (cdcl_inprocessingRules_G_posVUsed == 3){
				//We have collected three clauses for the X side. We can continue with the clauses of the Y side.
				break;
			}
		}
		if (cdcl_inprocessingRules_G_posVUsed != 3) continue;
		//Three clauses on the positive side were collected. We check if we find three clauses on the negative side.
		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg,j);
			if (GET_CDCL_CLS_SIZE(c) != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 0 && sCountN == 3) || (sCountP == 1 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < cdcl_inprocessingRules_G_negVUsed; ++k){
					if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_negV[k]) == GET_CDCL_CLS_SIG(c)) break;
				}
				if (k < cdcl_inprocessingRules_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				CDCL_IR_ADD_CLS_TO_GNEGV(c);
				if (cdcl_inprocessingRules_G_negVUsed == 3){
					//We have collected three clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (cdcl_inprocessingRules_G_negVUsed != 3) continue;

		//Here, we have collected the three X and three Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = cdcl_inprocessingRules_G_posV[0];
		reWID = GET_CDCL_VAR_PTR_ID(v);
		toRIDa = 0;
		toRIDb = 0;
		if (GET_CDCL_CLS_LITNUM(c,0) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,0);
			} else {
				toRIDb = -GET_CDCL_CLS_LITNUM(c,0);
			}
		}
		if (GET_CDCL_CLS_LITNUM(c,1) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,1);
			} else {
				toRIDb = -GET_CDCL_CLS_LITNUM(c,1);
			}
		}
		if (GET_CDCL_CLS_LITNUM(c,2) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,2);
			} else {
				toRIDb = -GET_CDCL_CLS_LITNUM(c,2);
			}
		}

		//We back-jump in preparation of unit propagation.
		cdcl_inprocessingRules_backJump(0,0);

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals + toRIDa, cdcl_literals + reWID);
		if (conflict != NULL){
			return conflict;
		}

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals - toRIDa, cdcl_literals - reWID);
		if (conflict != NULL){
			return conflict;
		}

		//Mark the replaced variable with its representative.
		repVar = cdcl_variables + abs(toRIDa);
		if ((toRIDa < 0 && reWID < 0) || (toRIDa > 0 && reWID > 0)){
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		cdcl_inprocessingRules_intern_disableVariable(cdcl_variables + abs(toRIDa));

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals + toRIDb, cdcl_literals + reWID);
		if (conflict != NULL){
			return conflict;
		}

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals - toRIDb, cdcl_literals - reWID);
		if (conflict != NULL){
			return conflict;
		}

		//Mark the replaced variable with its representative.
		repVar = cdcl_variables + abs(toRIDb);
		if ((toRIDb < 0 && reWID < 0) || (toRIDb > 0 && reWID > 0)){
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		cdcl_inprocessingRules_intern_disableVariable(cdcl_variables + abs(toRIDb));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		conflict = cdcl_inprocessingRules_upPropagate();
		if (conflict != NULL){
			return conflict;
		}

		//We can restart working on v.
		i = 0;
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge_SIM4(cdclVariable* v){
	//This method extracts similarity gates for exactly four variables. This looks as follows. Given clauses
	//Positive side:   Negative side:
	//X1  (a -b -c -d)      Y1 (-a  b  c  d)
	//X2  (a -b  c  d)      Y2 (-a -b  c  d)
	//X3  (a  b -c  d)      Y3 (-a  b -c  d)
	//X4  (a  b  c -d)      Y4 (-a  b  c -d)
	//X5  (a -b -c  d)      Y5 (-a -b -c  d)
	//X6  (a -b  c -d)      Y6 (-a -b  c -d)
	//X7  (a  b -c -d)      Y7 (-a  b -c -d)
	//This requires a = b = c = d.
	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
	cdclClause *c, *conflict = NULL;
	cdclVariable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, toRIDc, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++i){
		c = GET_CDCL_LIT_PTR_OCCNUM(lPos,i);
		if (GET_CDCL_CLS_SIZE(c) != 4) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++cdcl_litStamp;
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0), cdcl_litStamp);
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1), cdcl_litStamp);
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2), cdcl_litStamp);
		SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,3), cdcl_litStamp);
		CDCL_IR_RESET_GDEFARRAYS();
		CDCL_IR_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 to X7.
		for (j = i+1; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos,j);
			if (GET_CDCL_CLS_SIZE(c) != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,3)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,3)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 2 && sCountN == 2) || (sCountP == 3 && sCountN == 1)){
				//It has. We must now check if it actually new for the positive set.
				for (k = 0; k < cdcl_inprocessingRules_G_posVUsed; ++k){
					if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_posV[k]) == GET_CDCL_CLS_SIG(c)) break;
				}
				if (k < cdcl_inprocessingRules_G_posVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the positive side.
				CDCL_IR_ADD_CLS_TO_GPOSV(c);
				if (cdcl_inprocessingRules_G_posVUsed == 7){
					//We have collected three clauses for the X side. We can continue with the clauses of the Y side.
					break;
				}
			}
		}
		if (cdcl_inprocessingRules_G_posVUsed != 7) continue;
		//Three clauses on the positive side were collected. We check if we find three clauses on the negative side.
		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg,j);
			if (GET_CDCL_CLS_SIZE(c) != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,0)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,1)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,2)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c,3)) == cdcl_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,3)) == cdcl_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountN == 4 && sCountP == 0) || (sCountP == 1 && sCountN == 3) || (sCountP == 2 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < cdcl_inprocessingRules_G_negVUsed; ++k){
					if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_negV[k]) == GET_CDCL_CLS_SIG(c)) break;
				}
				if (k < cdcl_inprocessingRules_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				CDCL_IR_ADD_CLS_TO_GNEGV(c);
				if (cdcl_inprocessingRules_G_negVUsed == 7){
					//We have collected three clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (cdcl_inprocessingRules_G_negVUsed != 7) continue;

		//Here, we have collected the three X and three Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = cdcl_inprocessingRules_G_posV[0];
		reWID = GET_CDCL_VAR_PTR_ID(v);
		toRIDa = 0;
		toRIDb = 0;
		toRIDc = 0;
		if (GET_CDCL_CLS_LITNUM(c,0) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,0);
			} else if (toRIDb == 0){
				toRIDb = -GET_CDCL_CLS_LITNUM(c,0);
			} else {
				toRIDc = -GET_CDCL_CLS_LITNUM(c,0);
			}
		}
		if (GET_CDCL_CLS_LITNUM(c,1) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,1);
			} else if (toRIDb == 0){
				toRIDb = -GET_CDCL_CLS_LITNUM(c,1);
			} else {
				toRIDc = -GET_CDCL_CLS_LITNUM(c,1);
			}
		}
		if (GET_CDCL_CLS_LITNUM(c,2) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,2);
			} else if (toRIDb == 0){
				toRIDb = -GET_CDCL_CLS_LITNUM(c,2);
			} else {
				toRIDc = -GET_CDCL_CLS_LITNUM(c,2);
			}
		}
		if (GET_CDCL_CLS_LITNUM(c,3) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CDCL_CLS_LITNUM(c,3);
			} else if (toRIDb == 0){
				toRIDb = -GET_CDCL_CLS_LITNUM(c,3);
			} else {
				toRIDc = -GET_CDCL_CLS_LITNUM(c,3);
			}
		}

		//We back-jump in preparation of unit propagation.
		cdcl_inprocessingRules_backJump(0,0);

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals + toRIDa, cdcl_literals + reWID);
		if (conflict != NULL){
			return conflict;
		}

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals - toRIDa, cdcl_literals - reWID);
		if (conflict != NULL){
			return conflict;
		}

		//Mark the replaced variable with its representative.
		repVar = cdcl_variables + abs(toRIDa);
		if ((toRIDa < 0 && reWID < 0) || (toRIDa > 0 && reWID > 0)){
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		cdcl_inprocessingRules_intern_disableVariable(cdcl_variables + abs(toRIDa));

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals + toRIDb, cdcl_literals + reWID);
		if (conflict != NULL){
			return conflict;
		}

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals - toRIDb, cdcl_literals - reWID);
		if (conflict != NULL){
			return conflict;
		}

		//Mark the replaced variable with its representative.
		repVar = cdcl_variables + abs(toRIDb);
		if ((toRIDb < 0 && reWID < 0) || (toRIDb > 0 && reWID > 0)){
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		cdcl_inprocessingRules_intern_disableVariable(cdcl_variables + abs(toRIDb));

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals + toRIDc, cdcl_literals + reWID);
		if (conflict != NULL){
			return conflict;
		}

		conflict = cdcl_inprocessingRules_intern_els_replaceLit(cdcl_literals - toRIDc, cdcl_literals - reWID);
		if (conflict != NULL){
			return conflict;
		}

		//Mark the replaced variable with its representative.
		repVar = cdcl_variables + abs(toRIDc);
		if ((toRIDc < 0 && reWID < 0) || (toRIDc > 0 && reWID > 0)){
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_CDCL_VAR_PTR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		cdcl_inprocessingRules_intern_disableVariable(cdcl_variables + abs(toRIDc));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		conflict = cdcl_inprocessingRules_upPropagate();
		if (conflict != NULL){
			return conflict;
		}

		//We can restart working on v.
		i = 0;
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge_XOR(cdclVariable *v){
	//This method will check if the given variable v has an exclusive or gate definition called an XOR gate.
	//If v <=>  = x_1 XOR ... XOR x_w, it must hold that we have at least
	//#N = sum_{k=0}^{floor(w/2)} (\binomial{w,2*k})
	//clauses containing -v (all with an even number of positive literals x_1, ..., x_w). Furthermore, we need at least
	//#P = sum_{k=0}^{floor((w-1)/2)} (\binomial{w,2*k+1})
	//clauses containing +v (all with odd number of positive literals x_1, ..., x_w). All these #N + #P clauses must
	//contain -v or +v and the remainder must contain only +/-x_i. We need at least 2^w many clauses for such a definition.

	cdclLiteral *lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v), *lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
	cdclClause *c, *d;
	int32_t i,j,z,k,numNeeded,
		largePosAvail = (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) - GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos)),
		largeNegAvail = (GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg) - GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg));

	//We walk through the set of clauses containing -v.
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++i){
		//If the clause is too short, we will ignore it. If v does not have 2^(c->size-1) many occurrences (not binary!),
		//it cannot be in an XOR gate as well. Half of the clauses must be in the positive side, half in the negative side.
		c = GET_CDCL_LIT_PTR_OCCNUM(lNeg,i);
		numNeeded = pow(2,GET_CDCL_CLS_SIZE(c)-2);

		if (GET_CDCL_CLS_SIZE(c) < 3 || largeNegAvail - i < numNeeded){
			//Size is wrong or we simply do not have enough clauses left containing v to create a XOR gate out of it.
			continue;
		}

		//Before we go on, we check if the theoretical number of resolvents created would be too large. If so, we continue.
		if (numNeeded*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos)-numNeeded) + numNeeded*(GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)-numNeeded)
				> param_cdclIRGEMaxXOR) continue;

		//Otherwise, c might be defining in an XOR gate. We will now stamp all the literals in the clause, and stamp them such
		//that it is clear if they  are contained negatively or positively.
		CDCL_IR_RESET_GDEFARRAYS();
		cdcl_litStamp+=2;
		for (j=0; j < GET_CDCL_CLS_SIZE(c); ++j){
			//In case the literal is positive in the clause, we stamp it with litStamp, otherwise we stamp it with litStamp-1.
			if (GET_CDCL_CLS_LITNUM(c,j) > 0){
				SET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(c,j), cdcl_litStamp);
				SET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,j), cdcl_litStamp);
			} else {
				SET_CDCL_LIT_STAMP( GET_CDCL_CLS_LITNUM(c,j), cdcl_litStamp-1);
				SET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c,j), cdcl_litStamp-1);
			}
		}

		//We add this clause as the first candidate.
		CDCL_IR_ADD_CLS_TO_GNEGV(c);

		//Now we walk through the remaining negative clauses, and for everyone that has an even number of stamped negated
		//literals and the right size, we check if this clause is new via the signature. If so, we add it to the G_negV array.
		for (j = i+1; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			d = GET_CDCL_LIT_PTR_OCCNUM(lNeg,j);
			//If the clause does not have the right size we cannot use it. We need an even number of them to be negated!
			if (GET_CDCL_CLS_SIZE(d) != GET_CDCL_CLS_SIZE(c)) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largeNegAvail - j < numNeeded - cdcl_inprocessingRules_G_negVUsed) break;

			//We count the number of negated literals in this clause.
			k = 0;
			for (z = 0; z < GET_CDCL_CLS_SIZE(d); ++z){
				//We ignore the literal for v.
				if (GET_CDCL_CLS_LITNUM(d,z) == -GET_CDCL_VAR_PTR_ID(v)){
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an even number of such literals.
				if (GET_CDCL_CLS_LITNUM(d,z) > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) == cdcl_litStamp-1){
						++k;
					} else if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) != cdcl_litStamp){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) == cdcl_litStamp){
						++k;
					} else if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) != cdcl_litStamp-1){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}
			}

			//We now check if the number of counted literals is even. If not, this clause cannot be part of an XOR definition.
			if (z < GET_CDCL_CLS_SIZE(d) || k%2 != 0) continue;

			//Otherwise we must check if this clause is new for the set of G_negV clauses.
			for (k = 0; k < cdcl_inprocessingRules_G_negVUsed; ++k){
				if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_negV[k]) == GET_CDCL_CLS_SIG(d)) break;
			}
			if (k < cdcl_inprocessingRules_G_negVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v negatively.
			CDCL_IR_ADD_CLS_TO_GNEGV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (cdcl_inprocessingRules_G_negVUsed == numNeeded) break;
		}

		//We can continue with the next candidate clause c if we failed to collect enough clauses with -v for the XOR.
		if (cdcl_inprocessingRules_G_negVUsed != numNeeded) continue;

		//After having checked the collection of negative clauses, we can go on with the positive ones.
		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			d = GET_CDCL_LIT_PTR_OCCNUM(lPos,j);
			//If the clause does not have the right size we cannot use it.
			if (GET_CDCL_CLS_SIZE(d) != GET_CDCL_CLS_SIZE(c)) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largePosAvail - j < numNeeded - cdcl_inprocessingRules_G_posVUsed) break;

			//We count the number of negated literals in this clause. We need an odd number of them to be negated.
			k = 0;
			for (z=0; z < GET_CDCL_CLS_SIZE(d); ++z){
				//We ignore the literal for v.
				if (GET_CDCL_CLS_LITNUM(d,z) == GET_CDCL_VAR_PTR_ID(v)){
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an odd number of such literals.
				if (GET_CDCL_CLS_LITNUM(d,z) > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) == cdcl_litStamp-1){
						++k;
					} else if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) != cdcl_litStamp){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) == cdcl_litStamp){
						++k;
					} else if (GET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(d,z)) != cdcl_litStamp-1){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}
			}
			//We now check if the number of counted literals is odd. If not, this clause cannot be part of an XOR definition.
			if (z < GET_CDCL_CLS_SIZE(d) || k%2 != 1) continue;

			//Otherwise we must check if this clause is new for the set of G_posV clauses.
			for (k = 0; k < cdcl_inprocessingRules_G_posVUsed; ++k){
				if (GET_CDCL_CLS_SIG(cdcl_inprocessingRules_G_posV[k]) == GET_CDCL_CLS_SIG(d)) break;
			}
			if (k < cdcl_inprocessingRules_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v positively.
			CDCL_IR_ADD_CLS_TO_GPOSV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (cdcl_inprocessingRules_G_posVUsed == numNeeded) break;
		}

		//In case we have collected the right number of clauses we can extract the gate.
		if (cdcl_inprocessingRules_G_posVUsed == numNeeded && cdcl_inprocessingRules_G_negVUsed == numNeeded){
			//Time to extract the gate.
			c = cdcl_inprocessingRules_intern_ge_extract(v);
			if (c != NULL){
				return c;
			}
			break;
		}
	}

	return NULL;
}

cdclClause* cdcl_inprocessingRules_intern_ge(){
	//This method implements gate extraction for the CDCL in-processor.
	#ifdef VERBOSE_CDCL
	printf("GE");fflush(stdout);
	int32_t initVars = cdcl_vars_e_used;
	int32_t initCls = cdcl_cls_o_used + cdcl_cls_l_used;
	#endif
	cdclClause *conflict = NULL;
	cdclLiteral *lOuter, *lInner;
	cdclVariable *v;

	while ((lOuter = cdcl_literals_Q_dequeueBest()) != NULL){
		//We pick the best outer variables and its inner correspondent.
		lInner = GET_CDCL_LIT_PTR_OPPOSITE(lOuter);
		cdcl_literals_Q_delete(lInner);

		//Variable v is what we want to extract.
		v = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(lOuter));

		//We ignore the variable if its number of occurrences is larger than what we allow or if the variable is assigned.
		if (!IS_CDCL_VAR_PTR_UNASSIGNED(v)){
			continue;
		}

		//We check if the literals are pure.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) == 0 || GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner) == 0){
			//Yes, that is the case. We will now perform the unit propagation in DL0.
			cdcl_inprocessingRules_backJump(0,0);
			if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter) == 0){
				CDCL_UP_ADD_DEC_PTR(lInner, v);
			} else {
				CDCL_UP_ADD_DEC_PTR(lOuter, v);
			}
			conflict = cdcl_inprocessingRules_upPropagate();
			if (conflict != NULL) break;
			continue;
		}

		//We check that the variable is still enabled, and if so, perform GE on it.
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v) && GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter)>0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner)>0){
			//We perform SIM3 gate extraction with it.
			conflict = cdcl_inprocessingRules_intern_ge_SIM3(v);
			if (conflict != NULL){
				break;
			}
		}
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v) && GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter)>0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner)>0){
			//We perform SIM4 gate extraction with it.
			conflict = cdcl_inprocessingRules_intern_ge_SIM4(v);
			if (conflict != NULL){
				break;
			}
		}
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v) && GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter)>0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner)>0){
			//We perform AND gate extraction with it.
			conflict = cdcl_inprocessingRules_intern_ge_AND(v);
			if (conflict != NULL){
				break;
			}
		}
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v) && GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter)>0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner)>0){
			//We perform OR gate extraction with it.
			conflict = cdcl_inprocessingRules_intern_ge_OR(v);
			if (conflict != NULL){
				break;
			}
		}
		if (IS_CDCL_VAR_PTR_UNASSIGNED(v) && GET_CDCL_LIT_PTR_NUMOCCS_USED(lOuter)>0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lInner)>0){
			//We perform XOR gate extraction with it.
			conflict = cdcl_inprocessingRules_intern_ge_XOR(v);
			if (conflict != NULL){
				break;
			}
		}
	}


	#ifdef VERBOSE_CDCL
	printf("[%d,%d] ", initVars - cdcl_vars_e_used, initCls - (int32_t)(cdcl_cls_o_used + cdcl_cls_l_used) );
	#endif
	return conflict;
}
