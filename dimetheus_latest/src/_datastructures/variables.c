/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../dimetheus.h"

void variables_reset(){
	f.vars = NULL;
	f.vars_e = NULL;
	f.vars_d = NULL;

	f.n_initial = 0;
	f.n_afterPrep = 0;
	f.n_vars_e_used = 0;
	f.n_vars_d_used = 0;
}

uint32_t variables_init(){
	int i;

	f.vars_e = malloc(sizeof(variable*)*(f.n_initial+1));
	if (f.vars_e == NULL){
		printf("c ERROR. Unable to allocate the enabled variables address array. Out of Memory?\n");
		return 0;
	}

	f.vars_d = malloc(sizeof(variable*)*(f.n_initial+1));
	if (f.vars_d == NULL){
		printf("c ERROR. Unable to allocate the disabled variables address array. Out of Memory?\n");
		return 0;
	}

	f.vars = malloc(sizeof(variable)*(f.n_initial+1));
	if (f.vars == NULL){
		printf("c ERROR. Unable to allocate the variable data array. Out of Memory?\n");
		return 0;
	}

	main_varData = f.vars;					//This will help us access the variable data a bit faster.

	for (i=0; i<f.n_initial+1; ++i){
		main_varData[i].reason = NULL;		//The variable has no reason yet.
		main_varData[i].activity = ONE;		//The variable activity is set to 1.0.
		main_varData[i].baseData = 0;		//Reset the base data.
		main_varData[i].id =	i;			//Set the variables id.
		main_varData[i].decLevel = -1;		//The variable is not yet assigned.
		main_varData[i].isEquivTo = 0;		//Is not equivalent to anything but itself.
		main_varData[i].vQPos = 0;			//Not in the variable priority queue.
		main_varData[i].vQScore = 0;		//No score yet.
		main_varData[i].vSPos = 0;			//Not in the variable stack.
		if (i == 0) continue;
		//We now reset the base data to 1000 0000 0000 0000 0000 0000 0000 0000, and then add the offset.
		main_varData[i].baseData = VAR_UNASSIGNED_MASK + ((uint32_t)(i-1));	//Reset the base data and add the offset
		f.vars_e[i-1] = main_varData + i;									//Make the variable enabled.
		//Now the base data indicates this variable to be unassigned, enabled, SLS assignment is 0 and it has the offset i-1.
	}
	f.n_vars_e_used = f.n_initial;

	return 1;
}

void variables_resetActivitiesAndPhases(){
	variable *v;
	float_ty act;
	int32_t i;
	for (i=1; i < f.n_initial+1; ++i){
		v = main_varData + i;
		if (IS_VAR_DISABLED(v)) continue;
		//Set the phase of the variable according to the maximum occurrence.
		if (GET_LIT_NUM_NUMOCCS_USED(i) > GET_LIT_NUM_NUMOCCS_USED(-i)){
			SET_VAR_PHASE_1(v);
		} else {
			SET_VAR_PHASE_0(v);
		}
		//Set the initial activity based on the total occurrence of the variable.
		act = (GET_LIT_NUM_NUMOCCS_USED(i) + GET_LIT_NUM_NUMOCCS_USED(-i)) / (f.m_el_used + f.m_eo_used);
		SET_VAR_ACTIVITY(v, act);
	}
}


void variables_dispose(){
	if (f.vars != NULL){
		free(f.vars);
		f.vars = NULL;
	}

	if (f.vars_e != NULL){
		free(f.vars_e);
		f.vars_e = NULL;
	}

	if (f.vars_d != NULL){
		free(f.vars_d);
		f.vars_d = NULL;
	}
}

void variables_disable(variable* v){
	uint32_t pos;
	//To disable a single variable. The variable MUST be enabled. Since we do not allow the addition of new variables, the
	//address arrays f.vars_e and f.vars_d have sufficient size. We just have to move the pointer (append it), update the
	//offset, and mark the variable as disabled.

	//We remove the variable from the vars_e array.
	pos = GET_VAR_OFFSET(v);
	REM_VAR_FROM_VARS_LIST(f.vars_e, pos, f.n_vars_e_used);

	//We append the variable to the list of disabled variables.
	SWITCH_VAR_ENABLED_DISABLED_LIST(f.vars_d, v, f.n_vars_d_used);
}

void variables_enable(variable* v){
	uint32_t pos;
	//To re-enable a single variable. The variable MUST be disabled. Since we do not allow the addition of new variables, the
	//address arrays f.vars_e and f.vars_d have sufficient size. We just have to move the pointer (append it), update the
	//offset, and mark the variable as enabled.

	//We remove the variable from the vars_d array.
	pos = GET_VAR_OFFSET(v);
	REM_VAR_FROM_VARS_LIST(f.vars_d, pos, f.n_vars_d_used);

	//We append the variable to the list of enabled variables.
	SWITCH_VAR_ENABLED_DISABLED_LIST(f.vars_e, v, f.n_vars_e_used);
}

void variables_assignEquivVars_assign(variable *v){
	//This method tries to find the equivalence ancestor for v. If this variable is unassigned and has no ancestor
	//it will stay unassigned.
	variable *anc;
	int32_t ancID = GET_VAR_ISEQUIVTO(v);

	//Check if the variable is already assigned or if it has an equivalence ancestor.
	if (!IS_VAR_UNASSIGNED(v) || ancID == 0) return;

	//The variable is unassigned and has an equivalence ancestor. We ensure that the ancestor is assigned.
	anc = main_varData + abs(ancID);
	variables_assignEquivVars_assign(anc);
	if (IS_VAR_UNASSIGNED(anc)) return;
	//The ancestor of variable v has now been assigned to its correct value. We will now assign v. If the ancID is negative,
	//we will assign it to the opposite assignment, otherwise we will assign it to the same assignment.
	if (ancID > 0){
		//Variable v must have the same assignment as its ancestor.
		if (IS_VAR_TRUE(anc)){
			SET_VAR_ASS_1(v);
		} else {
			SET_VAR_ASS_0(v);
		}
	} else {
		//Variable v must have the opposite assignment as its ancestor.
		if (IS_VAR_TRUE(anc)){
			SET_VAR_ASS_0(v);
		} else {
			SET_VAR_ASS_1(v);
		}
	}
}

#ifdef VERBOSE
void variables_printVariable(variable* v){
	printf("c VAR %p ID:%6d ISEQUIVTO:%6d DECLEVEL:%6d REASON %p ACT: %f PHA: %d HEAD:",
			(void*) v, v->id, v->isEquivTo, v->decLevel, (void*)v->reason, v->activity, GET_VAR_PHASE(v));
	PRINT_BITS_UINT32(v->baseData);
	printf("[%u|%u] ", GET_VAR_ASS_UINT(v), GET_VAR_DIS_UINT(v));
	printf(" QPOS:%5d\tQSCORE:%5d\t", v->vQPos, v->vQScore);
	printf("\n");
}

void variables_printEnabledVariables(){
	uint32_t i;
	printf("c Pointers:");
	for (i = 0; i < f.n_vars_e_used; ++i){
			printf("%p ", (void*)f.vars_e[i]);
	}
	printf("\n");
}

void variables_printDisabledVariables(){
	uint32_t i;
	printf("c Pointers:");
	for (i = 0; i < f.n_vars_d_used; ++i){
			printf("%p ", (void*)f.vars_d[i]);
	}
	printf("\n");
}

void variables_printVariableAssignments(){
	uint32_t counter;
	printf("c ASSIGNMENTS (global)[ ");
	for (counter = 1; counter <= f.n_initial; ++counter){
		if (IS_VAR_TRUE((main_varData + counter))) {
			printf("+%d", counter);
		} else if (IS_VAR_FALSE((main_varData + counter))){
			printf("-%d", counter);
		} else {
			printf("?%d", counter);
		}
	}
	printf("] \n");
}

void variables_printEnabledVariablesData(){
	uint32_t i;
	for (i = 0; i < f.n_vars_e_used; ++i){
		variables_printVariable(f.vars_e[i]);
	}
}

uint32_t variables_verifyVariable(variable* v){
	if (IS_VAR_DISABLED(v)) return 1;

	if (GET_VAR_ISEQUIVTO(v) != 0){
		printf("\nThe variable is enabled but has an equivalence representative.\n");
		variables_printVariable(v);
		return 0;
	}

	return 1;
}

uint32_t variables_verifyAll(){
	int32_t i;

	for (i = 1; i < f.n_initial+1; ++i){
		if(!variables_verifyVariable(main_varData+i)){
			return 0;
		}
	}

	return 1;
}
#endif
