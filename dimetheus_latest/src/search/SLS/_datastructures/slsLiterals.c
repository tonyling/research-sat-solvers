/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../sls.h"

void sls_literals_prepareCall(){
	int32_t i;
	sls_litMaxOcc = 0;
	for (i = 0; i < f.n_vars_e_used; ++i){
		//We ignore all variables already assigned. They do not make it into the clauses anyway.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		SET_SLS_LIT_NUMOCCS_USED(f.vars_e[i]->id, 0);
		sls_literals[f.vars_e[i]->id].occs[0] = NULL;
		SET_SLS_LIT_NUMOCCS_USED(-f.vars_e[i]->id, 0);
		sls_literals[-f.vars_e[i]->id].occs[0] = NULL;
	}
}

void sls_literals_prepareTry(){

}

void sls_literals_reset(){
	sls_literals 	= NULL;
	sls_litMaxOcc	= 0;
}

void sls_literals_init(){
	uint32_t i;
	sls_litMaxOcc	= 0;

	sls_literals = malloc(sizeof(slsLiteral)*(2*f.n_initial+1U));
	if (sls_literals == NULL){
		printf("c ERROR. SLS was unable to allocate memory for the literals. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}
	//We make sure that all the literals have occurrence lists and reset values.
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_SLS_LIT_OCCS(i, NULL);
	}

	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_SLS_LIT_NUMOCCS_AVAIL(i,BLOCKSIZE);
		sls_literals[i].occs = malloc(sizeof(slsClause*)*BLOCKSIZE);
		if (GET_SLS_LIT_OCCS(i) == NULL){
			printf("c ERROR. SLS was unable to allocate memory for the literals occurrence list. Out of memory?\n");
			sls_returnCode = SLS_ERROR;
			#ifdef COLLECTSTATS
			++stats_sls_return_ERROR;
			#endif
			return;
		}
		SET_SLS_LIT_NUMOCCS_USED(i,0);
		sls_literals[i].occs[0] = NULL;
	}

	//Shift the pointer.
	sls_literals += f.n_initial;
}

void sls_literals_dispose(){
	uint32_t i;

	if (sls_literals != NULL){
		sls_literals -= f.n_initial;

		for (i = 0; i <= 2*f.n_initial; ++i){
			if (GET_SLS_LIT_OCCS(i) != NULL){
				free(sls_literals[i].occs);
				sls_literals[i].occs = NULL;
			}
		}

		free(sls_literals);
		sls_literals = NULL;
	}

	sls_litMaxOcc	= 0;
}

