/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../mp.h"

void mp_variables_prepareCall(){

}

void mp_variables_retainBiases(){
	//This method will retain the biases computed for all the variables that are still active in the global formula.
	uint32_t i, vid;
	variable *v;

	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		if (!IS_VAR_UNASSIGNED(v)) continue;
		vid = v->id;
		SET_VAR_ACTIVITY(v, CONVERT_MP_VAR_BIAS_TO_ACTIVITY( vid ));
		if (GET_MP_VAR_BIAS(vid) > ZERO) {
			SET_VAR_PHASE_1(v);
		} else {
			SET_VAR_PHASE_0(v);
		}
	}
}

void mp_variables_reset(){
	mp_variables = NULL;
}

void mp_variables_init(){
	uint32_t i;
	mp_variables = malloc(sizeof(mpVariable)*(f.n_initial+1U));
	if (mp_variables == NULL){
		printf("c ERROR. Unable to allocate the memory for MP variable data. Out of memory?\n");
		mp_returnCode = MP_ERROR;
		#ifdef COLLECTSTATS
		++stats_mp_return_ERROR;
		#endif
		return;
	}
	for (i = 1; i <= f.n_initial; ++i){
		SET_MP_VAR_BIAS(i, 0.0f);
	}
}

void mp_variables_dispose(){
	if (mp_variables != NULL){
		free(mp_variables);
		mp_variables = NULL;
	}
}

#ifdef VERBOSE_MP
void mp_variables_printVariable(mpVariable* v){
	printf("c MP_VAR %p: BIAS: %f\n", (void*) v, v->bias);
}
#endif
