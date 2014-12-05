/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../mp.h"

void mp_literals_prepareCall(){
	uint32_t i;
	#ifdef VERBOSE_MP
	printf("c     MP:   T and F values reset...\n");
	#endif
	mp_varNum = 0;
	for (i = 0; i < f.n_vars_e_used; ++i){
		//We ignore all variables already assigned. They do not make it into the clauses anyway.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		++mp_varNum;

		//We reset the positive and negative occurrence of the variables. We also initialize the T/F values and assume a
		//multiplicative scheme is used (so we set the products to 1.0).
		SET_MP_LIT_TORF(f.vars_e[i]->id, 1.0f);
		SET_MP_LIT_NUMOCCS( f.vars_e[i]->id , 0 );
		SET_MP_LIT_NUMZEROS( f.vars_e[i]->id , 0 );

		SET_MP_LIT_TORF(-f.vars_e[i]->id, 1.0f);
		SET_MP_LIT_NUMOCCS( -f.vars_e[i]->id , 0 );
		SET_MP_LIT_NUMZEROS( -f.vars_e[i]->id , 0 );
	}
}

void mp_literals_reset(){
	mp_literals = NULL;
	mp_varNum = 0;
}

void mp_literals_init(){
	mp_literals = malloc(sizeof(mpLiteral)*(2*f.n_initial+1U));
	if (mp_literals == NULL){
		printf("c ERROR. MP was unable to allocate memory for the literals. Out of memory?\n");
		mp_returnCode = MP_ERROR;
		#ifdef COLLECTSTATS
		++stats_mp_return_ERROR;
		#endif
		return;
	}
	//Shift the pointer.
	mp_literals += f.n_initial;
}

void mp_literals_dispose(){
	mp_varNum = 0;
	if (mp_literals != NULL){
		mp_literals -= f.n_initial;
		free(mp_literals);
		mp_literals = NULL;
	}
}
