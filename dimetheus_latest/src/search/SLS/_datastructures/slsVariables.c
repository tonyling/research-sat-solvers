/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../sls.h"

void sls_variables_prepareCall(){
	int32_t i;
	sls_varNum = 0;
	for (i = 0; i < f.n_vars_e_used; ++i){
		//We ignore all variables already assigned. They do not make it into the clauses anyway.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		++sls_varNum;
		SET_SLS_VAR_BRAKE(f.vars_e[i]->id, 0);
	}
}

void sls_variables_prepareTry(){
	//This method updates all the score information.
	int32_t i;
	slsClause *c;
	//We first reset all the literal scores.
	for (i = 0; i < f.n_vars_e_used; ++i){
		//We ignore all variables already assigned. They do not make it into the clauses anyway.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		SET_SLS_VAR_BRAKE(f.vars_e[i]->id, 0);
	}
	for (i = 0; i < sls_clsUsed; ++i){
		c = sls_clauses + i;
		if (GET_SLS_CLS_NUMTRUE(c) == 1U){
			//The true literal in this clause is the only one to make it.
			SET_SLS_VAR_BRAKE_INC(GET_SLS_CLS_TRUELIT(c));
		}
	}
}

void sls_variables_reset(){
	sls_assignments = NULL;
	sls_variables = NULL;
	sls_varNum = 0;
}

void sls_variables_init(){
	uint32_t i;
	sls_assignments = malloc(sizeof(uint64_t)*( (f.n_initial / 64U) +1U ));
	if (sls_assignments == NULL){
		printf("c ERROR. SLS was unable to allocate memory for the variable assignments. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}
	//We reset the assignment to 64 variables in one go.
	for (i = 0; i <= f.n_initial / 64U; ++i){
		sls_assignments[i] = 0ULL;
	}

	sls_variables = malloc(sizeof(slsVariable) * (f.n_initial+1));
	if (sls_variables == NULL){
		printf("c ERROR. SLS was unable to allocate memory for the variables. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}

	for (i = 0; i < f.n_initial+1; ++i){
		sls_variables[i].brake = 0;
	}
}

void sls_variables_dispose(){
	if (sls_assignments != NULL){
		free(sls_assignments);
		sls_assignments = NULL;
	}
	if (sls_variables != NULL){
		free(sls_variables);
		sls_variables = NULL;
	}
	sls_varNum = 0;
}

#ifdef VERBOSE_SLS
void sls_variables_printAll(){
	uint32_t i;
	printf("c     SLS: VARIABLES: \n");
	for (i=1; i < f.n_initial+1; ++i){
		printf("c       VAR:%d\tBRAKE:%d\tASS:%llu\n", i, GET_SLS_VAR_BRAKE(i), GET_SLS_VAR_ASSIGNMENT(i));
	}
}

void sls_variables_printAssignment(){
	uint32_t i;
	printf("c ASSIGNMENTS (SLS)[ ");
	for (i = 0; i <= f.n_initial / 64U; ++i){
		PRINT_BITS_UINT64(sls_assignments[i]);printf(" ");
	}
	printf(" ]\n");
}
#endif
