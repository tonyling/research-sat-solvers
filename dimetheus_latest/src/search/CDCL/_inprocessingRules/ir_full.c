/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "inprocessingRules.h"

void cdcl_inprocessingRule_full_printHelp(){
	printf("c      %-3d: FULL:\n", CDCL_INPROCESSINGRULE_FULL);
    printf("c           Behavior: Reduces the formula and does TER GE STR UNH TRL ELS NIV and SE.\n");
}

void cdcl_inprocessingRule_full_prepare(){
	cdcl_inprocessingRules_lastConflict = 0;
	cdcl_inprocessingRules_reduceFrom = cdcl_decStackPtrs[0];
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     IR-Plug-in [%-3d: FULL]:\n", CDCL_INPROCESSINGRULE_FULL);
	printf("c     CDCL:       param_cdclIRMinConfDistance         = %f\n", param_cdclIRMinConfDistance);
	printf("c     CDCL:       param_cdclIRTERNARYLitMax           = %d\n", param_cdclIRTERNARYLitMax);
	printf("c     CDCL:       param_cdclIRTERNARYMax              = %d\n", param_cdclIRTERNARYMax);
	printf("c     CDCL:       param_cdclIRGEMaxAONN               = %d\n", param_cdclIRGEMaxAONN);
	printf("c     CDCL:       param_cdclIRGEMaxXOR                = %d\n", param_cdclIRGEMaxXOR);
	printf("c     CDCL:       param_cdclIRStrMaxSize              = %d\n", param_cdclIRStrMaxSize);
	printf("c     CDCL:       param_cdclIRUNHRounds               = %d\n", param_cdclIRUNHRounds);
	printf("c     CDCL:       param_cdclIRNIVERMax                = %d\n", param_cdclIRNIVERMax);
	printf("c     CDCL:       cdcl_inprocessingRules_lastConflict = %d\n", cdcl_inprocessingRules_lastConflict);
	#endif
}

cdclClause* cdcl_inprocessingRule_full_inprocess(){
	//This method assumes that we are currently in DL1 and that DL1 is empty (decision state). It will
	//back-jump to DL0 in append mode and add all the assignments it identifies. After that, it will perform the UP. If this
	//results in a conflict the method returns the conflicting clause, otherwise NULL.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_cdcl_numInprocessing;
	#endif
	#ifdef VERBOSE_CDCL
	int32_t initNumV = cdcl_vars_e_used;
	int32_t initNumC = cdcl_cls_l_used + cdcl_cls_o_used;
	printf("c     CDCL:   In-Processing [FULL]: Starting to in-process (%d conflicts distance to last in-processing)...\n",
			cdcl_numConflicts-cdcl_inprocessingRules_lastConflict);
	printf("c     CDCL:   In-Processing [FULL]:   Remaining variables: %d\n", initNumV);
	printf("c     CDCL:   In-Processing [FULL]:   Remaining clauses:   %d\n", initNumC);
	printf("c     CDCL:   In-Processing [FULL]:   ");fflush(stdout);
	#endif

	cdclClause* conflictClause = NULL;
	cdclLiteral *lPos, *lNeg;
	cdclVariable *v;
	int32_t i;
	uint32_t random, numToPermute, currentNumVars, unhSuccess;

	//Reset queues and stacks.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;
	cdcl_clauses_S_flush();
	cdcl_clauses_S_touchClause = &cdcl_clauses_S_touchClause_null;
	cdcl_literals_Q_flush();
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;
	cdcl_literals_S_flush();
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_null;
	cdcl_variables_S_flush();
	cdcl_variables_S_touchVariable = &cdcl_variables_S_touchVariable_null;

	//Reduce the clause database such that the formula has again minimal size.
	cdcl_inprocessingRules_intern_reduceDB();
	//If the last in-processing was too close by we do not do anything else.
	if (cdcl_numConflicts-cdcl_inprocessingRules_lastConflict < cdcl_vars_e_used*param_cdclIRMinConfDistance){
		#ifdef VERBOSE_CDCL
		printf("Minimum conflict distance not reached.\n");fflush(stdout);
		#endif
		cdcl_inprocessingRules_lastConflict = cdcl_numConflicts;
		return NULL;
	}
	cdcl_inprocessingRules_lastConflict = cdcl_numConflicts;

	//TERNARY
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_inprocessing_ternary;
	numToPermute = cdcl_vars_e_used;
	perm_LFSR_init(numToPermute);
	for (i = 0; i < perm_lfsr_perLen; ++i){
		random = perm_LFSR_nextState();
		if (random >= numToPermute) continue;
		v = cdcl_vars_e[random];		//The variable.
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, positive.
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, negative.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > 0 && GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg) > 0){
			if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
				cdcl_literals_S_touchLiteral(lPos);
			} else {
				cdcl_literals_S_touchLiteral(lNeg);
			}
		}
	}
	cdcl_inprocessingRules_intern_ternary();
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_null;

	//GE
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_inprocessing_ge;
	for (i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, positive.
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, negative.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
			cdcl_literals_Q_touchLiteral(lPos);
		} else {
			cdcl_literals_Q_touchLiteral(lNeg);
		}
	}
	conflictClause = cdcl_inprocessingRules_intern_ge();
	if (conflictClause != NULL){
		#ifdef VERBOSE_CDCL
		printf(" conflict.\n");
		#endif
		return conflictClause;
	}
	if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
		#ifdef VERBOSE_CDCL
		printf(" nothing remains.\n");
		#endif
		return NULL;
	}
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;

	#ifdef VERBOSE_CDCL
	printf("\nc     CDCL:   In-Processing [FULL]:   ");
	#endif

	do {
		//STR
		cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_inprocessing_str;
		for (i = 0; i < cdcl_cls_l_used; ++i){
			cdcl_clauses_Q_touchClause(cdcl_cls_l[i]);
		}
		for (i = 0; i < cdcl_cls_o_used; ++i){
			cdcl_clauses_Q_touchClause(cdcl_cls_o[i]);
		}
		conflictClause = cdcl_inprocessingRules_intern_str();
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}
		cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;

		//UNHIDING
		currentNumVars = cdcl_vars_e_used;
		conflictClause = cdcl_inprocessingRules_intern_unhiding(param_cdclIRUNHRounds);
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}
		unhSuccess = 0;
		if (currentNumVars > cdcl_vars_e_used){
			unhSuccess = 1;
		}

		//TREELOOK
		conflictClause = cdcl_inprocessingRules_intern_treelook();
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}

		//ELS
		conflictClause = cdcl_inprocessingRules_intern_els();
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}

		//GE
		cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_inprocessing_ge;
		for (i = 0; i < cdcl_vars_e_used; ++i){
			v = cdcl_vars_e[i];
			lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, positive.
			lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, negative.
			if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
				cdcl_literals_Q_touchLiteral(lPos);
			} else {
				cdcl_literals_Q_touchLiteral(lNeg);
			}
		}
		conflictClause = cdcl_inprocessingRules_intern_ge();
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}
		cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;

		//NIVER
		cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_inprocessing_niver;
		for (i = 0; i < cdcl_vars_e_used; ++i){
			v = cdcl_vars_e[i];
			lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, positive.
			lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);	//The corresponding literal, negative.
			if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
				cdcl_literals_Q_touchLiteral(lPos);
			} else {
				cdcl_literals_Q_touchLiteral(lNeg);
			}
		}
		conflictClause = cdcl_inprocessingRules_intern_niver();
		if (conflictClause != NULL){
			#ifdef VERBOSE_CDCL
			printf(" conflict.\n");
			#endif
			return conflictClause;
		}
		if (cdcl_vars_e_used == 0 || cdcl_cls_l_used + cdcl_cls_o_used == 0){
			#ifdef VERBOSE_CDCL
			printf(" nothing remains.\n");
			#endif
			return NULL;
		}
		cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_inprocessing_niver;

		#ifdef VERBOSE_CDCL
		printf("\nc     CDCL:   In-Processing [FULL]:   ");
		#endif
	} while (unhSuccess);

	//SE
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_inprocessing_se;
	for (i = 0; i < cdcl_cls_l_used; ++i){
		cdcl_clauses_Q_touchClause(cdcl_cls_l[i]);
	}
	for (i = 0; i < cdcl_cls_o_used; ++i){
		cdcl_clauses_Q_touchClause(cdcl_cls_o[i]);
	}
	cdcl_inprocessingRules_intern_se();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;

	#ifdef VERBOSE_CDCL
	printf("\n");
	printf("c     CDCL:   In-Processing [FULL]:   Removed variables: %d (%d remaining)\n",
			initNumV - cdcl_vars_e_used, cdcl_vars_e_used);
	printf("c     CDCL:   In-Processing [FULL]:   Removed clauses: %d (%d remaining)\n",
			initNumC - (int32_t)(cdcl_cls_l_used + cdcl_cls_o_used), cdcl_cls_l_used + cdcl_cls_o_used);
	#endif

	//Reset queues and stacks.
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;
	cdcl_clauses_S_flush();
	cdcl_clauses_S_touchClause = &cdcl_clauses_S_touchClause_null;
	cdcl_literals_Q_flush();
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;
	cdcl_literals_S_flush();
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_null;
	cdcl_variables_S_flush();
	cdcl_variables_S_touchVariable = &cdcl_variables_S_touchVariable_null;

	#ifdef COLLECTSTATS
	stats_cdcl_time_inp += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
	//We return that we were unable to find additional variable assignments.
	return conflictClause;
}


