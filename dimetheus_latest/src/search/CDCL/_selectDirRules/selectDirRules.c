/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "selectDirRules.h"

void cdcl_selectDirRules_intern_dirInit_provided(){
	//This method does not touch the variable directions - they have been initialized to what the main component suggests.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: PROVIDED.\n");
	#endif
}

void cdcl_selectDirRules_intern_dirUpdate_provided(){
	//This method updates the phase of the variables according to the main phase of the variable.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: PROVIDED.\n");
	#endif

	variable *mainVar;
	cdclVariable *cdclVar;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		cdclVar = cdcl_vars_e[i];
		mainVar = main_varData + abs(GET_CDCL_VAR_PTR_ID(cdclVar));
		if (GET_VAR_PHASE(mainVar)){
			SET_CDCL_VAR_PTR_PHASE(cdclVar,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(cdclVar,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit_zerofirst(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: ZEROFIRST.\n");
	#endif

	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		SET_CDCL_VAR_PTR_PHASE(cdcl_vars_e[i],0);
	}
}

void cdcl_selectDirRules_intern_dirUpdate_zerofirst(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: ZEROFIRST.\n");
	#endif

	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		SET_CDCL_VAR_PTR_PHASE(cdcl_vars_e[i],0);
	}
}

void cdcl_selectDirRules_intern_dirInit_onefirst(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: ONEFIRST.\n");
	#endif

	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		SET_CDCL_VAR_PTR_PHASE(cdcl_vars_e[i],1);
	}
}

void cdcl_selectDirRules_intern_dirUpdate_onefirst(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: ONEFIRST.\n");
	#endif

	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		SET_CDCL_VAR_PTR_PHASE(cdcl_vars_e[i],1);
	}
}

void cdcl_selectDirRules_intern_dirInit_maxocc(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: MAXOCC.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirUpdate_maxocc(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: MAXOCC.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit_jereslowwang(){
	//Picks the direction that has a higher score in the Jereslow-Wang heuristic.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: JERESLOWWANG.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty sumPos, sumNeg;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		sumPos = 0;
		sumNeg = 0;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumPos += pow(2.0, -10);
			} else {
				sumPos += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumNeg += pow(2.0, -10);
			} else {
				sumNeg += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}


		if (sumPos > sumNeg){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirUpdate_jereslowwang(){
	//Picks the direction that has a higher score in the Jereslow-Wang heuristic.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: JERESLOWWANG.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty sumPos, sumNeg;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		sumPos = 0;
		sumNeg = 0;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumPos += pow(2.0, -10);
			} else {
				sumPos += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumNeg += pow(2.0, -10);
			} else {
				sumNeg += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		if (sumPos > sumNeg){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit_maxoccfail(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: MAXOCCFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,0);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,1);
		}
	}
}

void cdcl_selectDirRules_intern_dirUpdate_maxoccfail(){
	//Picks the direction that has more occurrences.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: MAXOCCFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos) > GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,0);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,1);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit_jereslowwangfail(){
	//Picks the direction that has a higher score in the Jereslow-Wang heuristic.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: JERESLOWWANGFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty sumPos, sumNeg;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		sumPos = 0;
		sumNeg = 0;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumPos += pow(2.0, -10);
			} else {
				sumPos += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumNeg += pow(2.0, -10);
			} else {
				sumNeg += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}


		if (sumPos > sumNeg){
			SET_CDCL_VAR_PTR_PHASE(v,0);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,1);
		}
	}
}

void cdcl_selectDirRules_intern_dirUpdate_jereslowwangfail(){
	//Picks the direction that has a higher score in the Jereslow-Wang heuristic.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: JERESLOWWANGFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	cdclClause *c;
	float_ty sumPos, sumNeg;
	uint32_t i, j;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		sumPos = 0;
		sumNeg = 0;

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lPos); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lPos, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumPos += pow(2.0, -10);
			} else {
				sumPos += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}

		for (j = 0; j < GET_CDCL_LIT_PTR_NUMOCCS_USED(lNeg); ++j){
			c = GET_CDCL_LIT_PTR_OCCNUM(lNeg, j);
			if (GET_CDCL_CLS_SIZE(c) > 10){
				sumNeg += pow(2.0, -10);
			} else {
				sumNeg += pow(2.0, -GET_CDCL_CLS_SIZE(c));
			}
		}


		if (sumPos > sumNeg){
			SET_CDCL_VAR_PTR_PHASE(v,0);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,1);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit_binfail(){
	//Picks the direction that has more implications.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction initialization: BINFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) > GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirUpdate_binfail(){
	//Picks the direction that has more implications.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:       Direction update: BINFAIL.\n");
	#endif

	cdclVariable *v;
	cdclLiteral *lPos, *lNeg;
	uint32_t i;

	for(i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_e[i];
		lPos = cdcl_literals + GET_CDCL_VAR_PTR_ID(v);
		lNeg = cdcl_literals - GET_CDCL_VAR_PTR_ID(v);
		if (GET_CDCL_LIT_PTR_NUMIMPS_USED(lPos) > GET_CDCL_LIT_PTR_NUMIMPS_USED(lNeg)){
			SET_CDCL_VAR_PTR_PHASE(v,1);
		} else {
			SET_CDCL_VAR_PTR_PHASE(v,0);
		}
	}
}

void cdcl_selectDirRules_intern_dirInit(){
	if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_PROVIDED){
		cdcl_selectDirRules_intern_dirInit_provided();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST){
		cdcl_selectDirRules_intern_dirInit_zerofirst();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST){
		cdcl_selectDirRules_intern_dirInit_onefirst();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_MAXOCC){
		cdcl_selectDirRules_intern_dirInit_maxocc();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG){
		cdcl_selectDirRules_intern_dirInit_jereslowwang();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL){
		cdcl_selectDirRules_intern_dirInit_maxoccfail();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL){
		cdcl_selectDirRules_intern_dirInit_jereslowwangfail();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_BINFAIL){
		cdcl_selectDirRules_intern_dirInit_binfail();
	}
}

void cdcl_selectDirRules_extern_dirUpdate(){
	if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_PROVIDED){
		cdcl_selectDirRules_intern_dirUpdate_provided();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_ZEROFIRST){
		cdcl_selectDirRules_intern_dirUpdate_zerofirst();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_ONEFIRST){
		cdcl_selectDirRules_intern_dirUpdate_onefirst();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_MAXOCC){
		cdcl_selectDirRules_intern_dirUpdate_maxocc();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANG){
		cdcl_selectDirRules_intern_dirUpdate_jereslowwang();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_MAXOCCFAIL){
		cdcl_selectDirRules_intern_dirUpdate_maxoccfail();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_JERESLOWWANGFAIL){
		cdcl_selectDirRules_intern_dirUpdate_jereslowwangfail();
	} else if (param_cdclSelectDirDirInit == CDCL_SELECTDIRRULE_DIRINIT_BINFAIL){
		cdcl_selectDirRules_intern_dirUpdate_binfail();
	}
}

void cdcl_extern_selectDirRules_switch(int32_t switchTo){
	//Use to switch the selection rule without re-initializing the whole CDCL module. We first dispose the old plug-in.
	cdcl_selectDirRules_disposePlugin();
	//We override the parameter to use the new select rule indicated.
	param_cdclSelectDirRule = switchTo;
	//We re-initialize the plug-in.
	cdcl_selectDirRules_initPlugin();
}

void cdcl_selectDirRules_resetPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Resetting select direction rule (SDR) plug-in...\n");
	#endif
	cdcl_selectDirRule_select  = &cdcl_selectDirRule_random_select;
	cdcl_selectDirRule_save	   = &cdcl_selectDirRule_random_save;
	cdcl_selectDirRule_prepare = &cdcl_selectDirRule_random_prepare;

	//Reset SDR data-structures here.
}

void cdcl_selectDirRules_initPlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Pick the select direction rule... ");
	#endif
	//Pick the select variable rule.
	if (param_cdclSelectDirRule == CDCL_SELECTDIRRULE_RANDOM){
		#ifdef VERBOSE_CDCL
		printf("RANDOM");
		#endif
		cdcl_selectDirRule_select  = &cdcl_selectDirRule_random_select;
		cdcl_selectDirRule_save	   = &cdcl_selectDirRule_random_save;
		cdcl_selectDirRule_prepare = &cdcl_selectDirRule_random_prepare;
	} else if (param_cdclSelectDirRule == CDCL_SELECTDIRRULE_FAILBINARY){
		#ifdef VERBOSE_CDCL
		printf("FAILBINARY");
		#endif
		cdcl_selectDirRule_select  = &cdcl_selectDirRule_failbinary_select;
		cdcl_selectDirRule_save	   = &cdcl_selectDirRule_failbinary_save;
		cdcl_selectDirRule_prepare = &cdcl_selectDirRule_failbinary_prepare;
	} else if (param_cdclSelectDirRule == CDCL_SELECTDIRRULE_PHASESAVING){
		#ifdef VERBOSE_CDCL
		printf("PHASESAVING");
		#endif
		cdcl_selectDirRule_select  = &cdcl_selectDirRule_phasesaving_select;
		cdcl_selectDirRule_save	   = &cdcl_selectDirRule_phasesaving_save;
		cdcl_selectDirRule_prepare = &cdcl_selectDirRule_phasesaving_prepare;
	} else if (param_cdclSelectDirRule == CDCL_SELECTDIRRULE_PHASEFLIP){
		#ifdef VERBOSE_CDCL
		printf("PHASEFLIP");
		#endif
		cdcl_selectDirRule_select  = &cdcl_selectDirRule_phaseflip_select;
		cdcl_selectDirRule_save	   = &cdcl_selectDirRule_phaseflip_save;
		cdcl_selectDirRule_prepare = &cdcl_selectDirRule_phaseflip_prepare;
	}
	#ifdef VERBOSE_CDCL
	printf("\n");
	#endif
	//Initialize SDR data-structures here.
}

void cdcl_selectDirRules_disposePlugin(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Disposing select direction rule (SDR) plug-in...\n");
	#endif
	cdcl_selectDirRule_select  = &cdcl_selectDirRule_random_select;
	cdcl_selectDirRule_save	   = &cdcl_selectDirRule_random_save;
	cdcl_selectDirRule_prepare = &cdcl_selectDirRule_random_prepare;

	//Dispose SDR data-structures here.
}

