/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectVarRules.h"

void cdcl_selectVarRules_intern_varActInit_provided(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity initialization: PROVIDED.\n");
	#endif
	uint32_t i;
	for(i = 0; i < cdcl_vars_e_used; ++i){
		if (GET_CDCL_VAR_PTR_ACTIVITY(cdcl_vars_e[i]) > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = GET_CDCL_VAR_PTR_ACTIVITY(cdcl_vars_e[i]);
		}
	}
}

void cdcl_selectVarRules_intern_varActUpdate_provided(){
	//This method re-organizes the variable priority queue using the provided activities.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity update: PROVIDED.\n");
	#endif
	cdclVariable *cdclVar;
	variable *mainVar;
	float_ty oldAct;
	uint32_t i;
	for(i = 0; i < cdcl_vars_e_used; ++i){
		cdclVar = cdcl_vars_e[i];
		oldAct = GET_CDCL_VAR_PTR_ACTIVITY(cdclVar);
		mainVar = main_varData + abs(GET_CDCL_VAR_PTR_ID(cdclVar));
		SET_CDCL_VAR_PTR_ACTIVITY(cdclVar, GET_VAR_ACTIVITY(mainVar));
		if (oldAct < GET_CDCL_VAR_PTR_ACTIVITY(cdclVar)){
			cdcl_variables_Q_moveUp(cdclVar);
		} else {
			cdcl_variables_Q_moveDown(cdclVar);
		}
		if (GET_CDCL_VAR_PTR_ACTIVITY(cdclVar) > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = GET_CDCL_VAR_PTR_ACTIVITY(cdclVar);
		}
	}
}

void cdcl_selectVarRules_intern_varActInit_occnum(){
	//Sets the variables such that those with more occurrences have a higher activity.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity initialization: OCCNUM.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	float_ty act, numOccs, maxAct = cdcl_cls_o_used + cdcl_cls_l_used;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		//Update the activity of variable v using the number of occurrences in both literal directions.
		numOccs = GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) + GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg);
		act = (numOccs / maxAct);
		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}
		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}

}

void cdcl_selectVarRules_intern_varActUpdate_occnum(){
	//Sets the variables such that those with more occurrences have a higher activity.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity update: OCCNUM.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	float_ty act, numOccs, maxAct = cdcl_cls_o_used + cdcl_cls_l_used;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		//Update the activity of variable v using the number of occurrences in both literal directions.
		numOccs = GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) + GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg);
		act = (numOccs / maxAct);
		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}
		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}

}

void cdcl_selectVarRules_intern_varActInit_jereslowwang(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity initialization: JERESLOWWANG.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty act;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		act = ZERO;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				act += pow(2.0, -10);
			} else {
				act += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				act += pow(2.0, -10);
			} else {
				act += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}

		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}
}

void cdcl_selectVarRules_intern_varActUpdate_jereslowwang(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity update: JERESLOWWANG.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty act;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		act = ZERO;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				act += pow(2.0, -10);
			} else {
				act += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				act += pow(2.0, -10);
			} else {
				act += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}

		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}
}

void cdcl_selectVarRules_intern_varActInit_binocc(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity initialization: BINOCC.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	float_ty act, numBins, maxAct = cdcl_cls_o_used;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		//Update the activity of variable v using the number of implications in both literal directions.
		numBins = GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) * GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg)
				+ GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) + GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg);
		act = (numBins / maxAct);
		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}
		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}

}

void cdcl_selectVarRules_intern_varActUpdate_binocc(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Activity update: BINOCC.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	float_ty act, numBins, maxAct = cdcl_cls_o_used;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		//Update the activity of variable v using the number of implications in both literal directions.
		numBins = GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) * GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg)
				+ GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) + GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg);
		act = (numBins / maxAct);
		if (act > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveUp(v);
		} else {
			SET_CDCL_VAR_PTR_ACTIVITY(v, act);
			cdcl_variables_Q_moveDown(v);
		}
		if (act > cdcl_selectVarRules_maxActivity){
			cdcl_selectVarRules_maxActivity = act;
		}
	}

}

void cdcl_selectVarRules_intern_varActInit(){
	if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_PROVIDED){
		cdcl_selectVarRules_intern_varActInit_provided();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_OCCNUM){
		cdcl_selectVarRules_intern_varActInit_occnum();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG){
		cdcl_selectVarRules_intern_varActInit_jereslowwang();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_BINOCC){
		cdcl_selectVarRules_intern_varActInit_binocc();
	}
}

void cdcl_selectVarRules_extern_varActUpdate(){
	if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_PROVIDED){
		cdcl_selectVarRules_intern_varActUpdate_provided();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_OCCNUM){
		cdcl_selectVarRules_intern_varActUpdate_occnum();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_JERESLOWWANG){
		cdcl_selectVarRules_intern_varActUpdate_jereslowwang();
	} else if (param_cdclSelectVarActInit == CDCL_SELECTVARRULE_VARACTINIT_BINOCC){
		cdcl_selectVarRules_intern_varActUpdate_binocc();
	}
}

void cdcl_extern_selectVarRules_switch(int32_t switchTo){
	//Use to switch the select rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_selectVarRules_disposePlugin();
	//We override the parameter to use the new select rule indicated.
	param_cdclSelectVarRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_selectVarRules_initPlugin();
}

void cdcl_selectVarRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting select variable rule (SVR) plug-in...\n");
	#endif
	cdcl_selectVarRule_select  = &cdcl_selectVarRule_random_select;
	cdcl_selectVarRule_prepare = &cdcl_selectVarRule_random_prepare;
	cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_random_incAct;
	cdcl_selectVarRule_reward  = &cdcl_selectVarRule_random_reward;
	cdcl_selectVarRule_rescale = &cdcl_selectVarRule_random_rescale;
	cdcl_selectVarRule_bump    = &cdcl_selectVarRule_random_bump;

	//Reset SVR data-structures here.
	cdcl_selectVarRules_currentActInc = ONE;
	cdcl_selectVarRules_maxActivity = ONE;
}

void cdcl_selectVarRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the select variable rule... ");
	#endif
	//Pick the select variable rule.
	if (param_cdclSelectVarRule == CDCL_SELECTVARRULE_RANDOM){
		#ifdef VERBOSE_CDCL
		printf("RANDOM");
		#endif
		cdcl_selectVarRule_select  = &cdcl_selectVarRule_random_select;
		cdcl_selectVarRule_prepare = &cdcl_selectVarRule_random_prepare;
		cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_random_incAct;
		cdcl_selectVarRule_reward  = &cdcl_selectVarRule_random_reward;
		cdcl_selectVarRule_rescale = &cdcl_selectVarRule_random_rescale;
		cdcl_selectVarRule_bump    = &cdcl_selectVarRule_random_bump;
	} else 	if (param_cdclSelectVarRule == CDCL_SELECTVARRULE_VSIDS){
		#ifdef VERBOSE_CDCL
		printf("VSIDS");
		#endif
		cdcl_selectVarRule_select  = &cdcl_selectVarRule_vsids_select;
		cdcl_selectVarRule_prepare = &cdcl_selectVarRule_vsids_prepare;
		cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_vsids_incAct;
		cdcl_selectVarRule_reward  = &cdcl_selectVarRule_vsids_reward;
		cdcl_selectVarRule_rescale = &cdcl_selectVarRule_vsids_rescale;
		cdcl_selectVarRule_bump    = &cdcl_selectVarRule_vsids_bump;
	} else if (param_cdclSelectVarRule == CDCL_SELECTVARRULE_RVSIDS){
		#ifdef VERBOSE_CDCL
		printf("RVSIDS");
		#endif
		cdcl_selectVarRule_select  = &cdcl_selectVarRule_rvsids_select;
		cdcl_selectVarRule_prepare = &cdcl_selectVarRule_rvsids_prepare;
		cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_rvsids_incAct;
		cdcl_selectVarRule_reward  = &cdcl_selectVarRule_rvsids_reward;
		cdcl_selectVarRule_rescale = &cdcl_selectVarRule_rvsids_rescale;
		cdcl_selectVarRule_bump    = &cdcl_selectVarRule_rvsids_bump;
	} else if (param_cdclSelectVarRule == CDCL_SELECTVARRULE_RRVSIDS){
		#ifdef VERBOSE_CDCL
		printf("RRVSIDS");
		#endif
		cdcl_selectVarRule_select  = &cdcl_selectVarRule_rrvsids_select;
		cdcl_selectVarRule_prepare = &cdcl_selectVarRule_rrvsids_prepare;
		cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_rrvsids_incAct;
		cdcl_selectVarRule_reward  = &cdcl_selectVarRule_rrvsids_reward;
		cdcl_selectVarRule_rescale = &cdcl_selectVarRule_rrvsids_rescale;
		cdcl_selectVarRule_bump    = &cdcl_selectVarRule_rrvsids_bump;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize SVR data-structures here.
	cdcl_selectVarRules_currentActInc = ONE;
	cdcl_selectVarRules_maxActivity = ONE;

}

void cdcl_selectVarRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing select variable rule (SVR) plug-in...\n");
	#endif
	cdcl_selectVarRule_select  = &cdcl_selectVarRule_random_select;
	cdcl_selectVarRule_prepare = &cdcl_selectVarRule_random_prepare;
	cdcl_selectVarRule_incAct  = &cdcl_selectVarRule_random_incAct;
	cdcl_selectVarRule_reward  = &cdcl_selectVarRule_random_reward;
	cdcl_selectVarRule_rescale = &cdcl_selectVarRule_random_rescale;
	cdcl_selectVarRule_bump    = &cdcl_selectVarRule_random_bump;

	//Dispose SVR data-structures here.
	cdcl_selectVarRules_currentActInc = ONE;
	cdcl_selectVarRules_maxActivity = ONE;
}
