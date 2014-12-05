/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../sls.h"

void sls_clauses_prepareCall(){
	uint32_t i, numLits;
	int32_t *litC;
	clause *c;
	sls_clsUsed = 0;
	sls_clsMaxSize = 0;
	sls_clsMinSize = UINT32_MAX;
	//Creates a restriction of the main formula into the clauses of the SLS. This will also append clauses into the occurrence
	//lists of the literals. First we make sure, that all the clauses have been allocated in a minimal form.
	if (sls_clsAvail < (f.m_eo_used + f.m_el_used)){
		sls_clauses = realloc(sls_clauses, sizeof(slsClause) * (f.m_eo_used + f.m_el_used));
		for (i = sls_clsAvail; i < (f.m_eo_used + f.m_el_used); ++i){
			sls_clauses[i].lits = malloc(sizeof(int32_t)*1U);
			sls_clauses[i].lits[0] = 0;
		}

		sls_unsatClauses = realloc(sls_unsatClauses, sizeof(slsClause*) * (f.m_eo_used + f.m_el_used));
		sls_clsAvail = f.m_eo_used + f.m_el_used;
	}
	//We will now walk through all the clauses in the original formula and reset a clause for the SLS. If the clause in the
	//original formula is satisfied, we ignore it.
	//First the enabled original clauses.
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(c->repLit)) continue;
		litC = c->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_LIT_SAT(*litC)){
				c->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				++numLits;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(c, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		sls_clauses[sls_clsUsed].lits = realloc(sls_clauses[sls_clsUsed].lits, sizeof(int32_t)*(numLits+1U));
		litC = c->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				sls_clauses[sls_clsUsed].lits[numLits++] = *litC;
				ADD_SLS_LIT_OCC(*litC, (sls_clauses + sls_clsUsed));
			}
			++litC;
		}
		//Terminate the clause.
		sls_clauses[sls_clsUsed++].lits[numLits] = 0;
		if (numLits > sls_clsMaxSize) sls_clsMaxSize = numLits;
		if (numLits < sls_clsMinSize) sls_clsMinSize = numLits;
	}
	//Second the enabled learned clauses.
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(c->repLit)) continue;
		litC = c->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_LIT_SAT(*litC)){
				c->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				++numLits;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(c, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		sls_clauses[sls_clsUsed].lits = realloc(sls_clauses[sls_clsUsed].lits, sizeof(int32_t)*(numLits+1U));
		litC = c->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				sls_clauses[sls_clsUsed].lits[numLits++] = *litC;
				ADD_SLS_LIT_OCC(*litC, (sls_clauses + sls_clsUsed));
			}
			++litC;
		}
		//Terminate the clause.
		sls_clauses[sls_clsUsed++].lits[numLits] = 0;
		if (numLits > sls_clsMaxSize) sls_clsMaxSize = numLits;
		if (numLits < sls_clsMinSize) sls_clsMinSize = numLits;
	}
}

void sls_clauses_prepareTry(){
	//This method basically initializes all the clause data for the current SLS assignment.
	uint32_t i, numTrue;
	int32_t *litC;
	slsClause *c;
	sls_numUnsat = 0;
	for (i = 0; i < sls_clsUsed; ++i){
		c = sls_clauses + i;
		litC = GET_SLS_CLS_LITS(c);
		numTrue = 0;
		while (*litC != 0){
			if (IS_SLS_LIT_SAT(*litC)){
				++numTrue;
				SET_SLS_CLS_TRUELIT(c,*litC);
			}
			++litC;
		}
		//Update the number of true literals for this clause.
		SET_SLS_CLS_NUMTRUE(c, numTrue);
		//Check if the clause is unsatisfied. If so, add it to the unsatisfied clauses.
		if (numTrue == 0U){
			sls_unsatClauses[sls_numUnsat] = c;
			SET_SLS_CLS_WHEREUNSAT(c,sls_numUnsat++);
		}
	}
}

void sls_clauses_reset(){
	sls_clauses 			= NULL;
	sls_unsatClauses 		= NULL;
	sls_numUnsat			= 0;
	sls_clsUsed				= 0;
	sls_clsAvail 			= 0;
	sls_clsMaxSize			= 0;
	sls_clsMinSize 			= 0;
}

void sls_clauses_init(){
	slsClause *c;
	uint32_t i;

	sls_clsAvail = BLOCKSIZE;
	sls_clauses = malloc(sizeof(slsClause)*sls_clsAvail);
	if (sls_clauses == NULL){
		printf("c ERROR. SLS was unable to allocate memory for the clauses. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}
	sls_clsUsed		= 0;

	sls_unsatClauses = malloc(sizeof(slsClause*)*sls_clsAvail);
	if (sls_unsatClauses == NULL){
		printf("c ERROR. SLS was unable to allocate memory for the unsatisfied clauses array. Out of memory?\n");
		sls_returnCode = SLS_ERROR;
		#ifdef COLLECTSTATS
		++stats_sls_return_ERROR;
		#endif
		return;
	}
	sls_numUnsat = 0;

	for (i = 0; i < sls_clsAvail; ++i){
		c = sls_clauses + i;
		SET_SLS_CLS_LITS(c,NULL);
	}

	for (i = 0; i < sls_clsAvail; ++i){
		c = sls_clauses + i;
		c->lits = malloc(sizeof(int32_t)*1U);
		c->lits[0] = 0;
	}
}

void sls_clauses_dispose(){
	slsClause *c;
	uint32_t i;
	if (sls_clauses != NULL){
		for (i = 0; i < sls_clsAvail; ++i){
			c = sls_clauses + i;
			if (GET_SLS_CLS_LITS(c) != NULL){
				free(GET_SLS_CLS_LITS(c));
				SET_SLS_CLS_LITS(c,NULL);
			}
		}
		free(sls_clauses);
		sls_clauses = NULL;
	}

	if (sls_unsatClauses != NULL){
		free (sls_unsatClauses);
		sls_unsatClauses = NULL;
	}

	sls_clsUsed		= 0;
	sls_clsAvail 	= 0;
	sls_clsMaxSize	= 0;
	sls_clsMinSize 	= 0;
}

#ifdef VERBOSE_SLS
void sls_clauses_printAll(){
	printf("c     SLS: CLAUSES: \n");
	int32_t i;
	for (i=0; i<sls_clsUsed;++i){
		sls_clauses_printClause(sls_clauses+i);
	}
}
void sls_clauses_printClause(slsClause *c){
	int32_t *litC;
	printf("c       SLS: Clause %p: NUMTRUE:%7d\tTLWU:%7d\tLITS: ", (void*)c, c->numTrue, c->tLwU);
	litC = c->lits;
	while (*litC != 0){
		printf("%7d[%d|%d] ", *litC, (int32_t)IS_SLS_LIT_SAT(*litC), (int32_t)GET_SLS_VAR_ASSIGNMENT(abs(*litC)));
		++litC;
	}
	printf("\n");
}
#endif
