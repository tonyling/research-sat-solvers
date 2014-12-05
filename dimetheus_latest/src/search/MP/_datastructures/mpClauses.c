/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../mp.h"

void mp_clauses_prepareCall(){
	uint32_t i, num, cSize;
	int32_t *litC;
	clause *c;
	float_ty *msgsC;
	uint8_t *noiseArr = NULL;
	mp_clsUsed = 0;
	mp_clsMaxSize = 0;
	//Creates a restriction of the main formula into the clauses of the MP. First we make sure, that all the clauses have
	//been allocated in a minimal form.
	if (mp_clsAvail < (f.m_eo_used + f.m_el_used)){
		mp_clauses = realloc(mp_clauses, sizeof(mpClause) * (f.m_eo_used + f.m_el_used));
		for (i = mp_clsAvail; i < (f.m_eo_used + f.m_el_used); ++i){
			mp_clauses[i].lits = malloc(sizeof(int32_t)*1U);
			mp_clauses[i].lits[0] = 0;
			mp_clauses[i].omegas = malloc(sizeof(float_ty)*1U);
			mp_clauses[i].omegas[0] = 0.5f;
			mp_clauses[i].weights = malloc(sizeof(float_ty)*1U);
			mp_clauses[i].weights[0] = ONE;
		}
		mp_clsAvail = f.m_eo_used + f.m_el_used;
	}
	//Second, we will, if applicable, randomly select a noise direction for every variable.
	if (param_mpWeightNoise > ZERO){
		//There is some noise given. We will now initialize an array that tells us (for every variable) whether to add or
		//subtract noise. If the value is 1, we add noise on the positive literals and subtract noise from the
		//negative literals. If the value is 0, we do it the other way around.
		noiseArr = malloc(sizeof(uint8_t)*(f.n_initial+1));
		for (i = 1; i <= f.n_initial; ++i){
			noiseArr[i] = (uint8_t) (rand_random() % 2);
		}
	}

	//We will now walk through all the clauses in the original formula and reset a clause for the MP. If the clause in the
	//original formula is satisfied, we ignore it. We also update the literals T/F values and assume a multiplicative scheme.
	//Any UP not using multiplicative update rules must re-initialize the values for the literals T/F values in its prepare
	//method. Since we have practically only multiplicative versions, this is most likely not to happen.
	//First the enabled original clauses.
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(c->repLit)) continue;
		litC = c->lits;
		num = 0; cSize = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_LIT_SAT(*litC)){
				c->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the MP clause.
				++num;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(c, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		cSize = num;
		mp_clauses[mp_clsUsed].lits = realloc(mp_clauses[mp_clsUsed].lits, sizeof(int32_t)*(cSize+1U));
		mp_clauses[mp_clsUsed].omegas = realloc(mp_clauses[mp_clsUsed].omegas, sizeof(float_ty)*(cSize+1U));
		mp_clauses[mp_clsUsed].weights = realloc(mp_clauses[mp_clsUsed].weights, sizeof(float_ty)*(cSize+1U));
		litC = c->lits;
		msgsC = c->msgs;
		num = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We use it in the MP clause.
				mp_clauses[mp_clsUsed].lits[num] = *litC;
				mp_clauses[mp_clsUsed].omegas[num] = *msgsC;
				mp_clauses[mp_clsUsed].weights[num] = ONE;
				//We now modify the neutral weight in case the noisy weight modification is enabled.
				if (param_mpWeightNoise > ZERO){
					//First, we check the sign of the literal, and then we check the value of the noise array.
					if (*litC > 0) {
						//The literal sign is positive.
						if (noiseArr[*litC] > 0) {
							//The noise indicator is positive as well. We add some noise here.
							mp_clauses[mp_clsUsed].weights[num] += param_mpWeightNoise;
						} else {
							//The noise indicator is not positive. We subtract some noise here.
							mp_clauses[mp_clsUsed].weights[num] -= param_mpWeightNoise;
							if (mp_clauses[mp_clsUsed].weights[num] < param_mpEpsilon){
								mp_clauses[mp_clsUsed].weights[num] = param_mpEpsilon;
							}
						}
					} else {
						//The literal sign is negative.
						if (noiseArr[-*litC] > 0) {
							//The noise indicator is positive. We subtract some noise here.
							mp_clauses[mp_clsUsed].weights[num] -= param_mpWeightNoise;
							if (mp_clauses[mp_clsUsed].weights[num] < param_mpEpsilon){
								mp_clauses[mp_clsUsed].weights[num] = param_mpEpsilon;
							}
						} else {
							//The noise indicator is not positive. We add some noise here.
							mp_clauses[mp_clsUsed].weights[num] += param_mpWeightNoise;
						}
					}
				}
				if (ONE - mp_clauses[mp_clsUsed].omegas[num] > param_mpEpsilon){
					SET_MP_LIT_APPLYWARN_MULT(*litC, ONE-mp_clauses[mp_clsUsed].omegas[num]);
				} else {
					SET_MP_LIT_NUMZEROSINC(*litC);
				}
				SET_MP_LIT_NUMOCCSINC(*litC);
				++num;
			}
			++litC;
			++msgsC;
		}
		//Terminate the clause.
		mp_clauses[mp_clsUsed++].lits[num] = 0;
		if (num > mp_clsMaxSize) mp_clsMaxSize = num;
	}
	//Second the enabled learned clauses.
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(c->repLit)) continue;
		litC = c->lits;
		num = 0; cSize = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_LIT_SAT(*litC)){
				c->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the MP clause.
				++num;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(c, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		cSize = num;
		mp_clauses[mp_clsUsed].lits = realloc(mp_clauses[mp_clsUsed].lits, sizeof(int32_t)*(cSize+1U));
		mp_clauses[mp_clsUsed].omegas = realloc(mp_clauses[mp_clsUsed].omegas, sizeof(float_ty)*(cSize+1U));
		mp_clauses[mp_clsUsed].weights = realloc(mp_clauses[mp_clsUsed].weights, sizeof(float_ty)*(cSize+1U));
		litC = c->lits;
		msgsC = c->msgs;
		num = 0;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We use it in the MP clause.
				mp_clauses[mp_clsUsed].lits[num] = *litC;
				mp_clauses[mp_clsUsed].omegas[num] = *msgsC;
				mp_clauses[mp_clsUsed].weights[num] = ONE;
				if (ONE - mp_clauses[mp_clsUsed].omegas[num] > param_mpEpsilon){
					SET_MP_LIT_APPLYWARN_MULT(*litC, 1.0f-mp_clauses[mp_clsUsed].omegas[num]);
				} else {
					SET_MP_LIT_NUMZEROSINC(*litC);
				}
				SET_MP_LIT_NUMOCCSINC(*litC);
				++num;
			}
			++litC;
			++msgsC;
		}
		//Terminate the clause.
		mp_clauses[mp_clsUsed++].lits[num] = 0;
		if (num > mp_clsMaxSize) mp_clsMaxSize = num;
	}

	if (noiseArr != NULL){
		free(noiseArr);
		param_mpWeightNoise = ZERO;
	}
}

void mp_clauses_retainMsgs(){
	int32_t *litC,i;
	clause *c;
	mpClause *mpC;
	float_ty *msgsC, *msgsMPC;
	//We walk through all the clauses of the original formula, and for each of them that seems to be respected we just
	//copy over all the messages we have. Here, we just check the representative literal of the clause, as this must be
	//up to date.

	#ifdef COLLECTSTATS
	uint32_t sumHW, sumNW, sumLW;
	float_ty *wgtC;
	for (i=0; i < STATS_MP_COUNTHWLITS_MAX+2; ++i){
		stats_mp_hwLits[i] = 0;
	}
	for (i=0; i < STATS_MP_COUNTNWLITS_MAX+2; ++i){
		stats_mp_nwLits[i] = 0;
	}
	for (i=0; i < STATS_MP_COUNTLWLITS_MAX+2; ++i){
		stats_mp_lwLits[i] = 0;
	}
	stats_mp_countedClauses = 0;
	#endif

	//We retain the number of the clause we have seen.
	mpC = mp_clauses;
	//We check the enabled original clauses.
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		//Check if the clause is satisfied in the original formula.
		if (IS_LIT_SAT(c->repLit)) continue;

		//For all the literals in this clause, we check if they are assigned. If not, we copy the message from the mpClause
		//into the message array position of the global clause.
		litC = c->lits;
		msgsC = c->msgs;
		msgsMPC = mpC->omegas;

		#ifdef COLLECTSTATS
		sumHW = 0;
		sumNW = 0;
		sumLW = 0;
		wgtC = mpC->weights;
		++stats_mp_countedClauses;
		#endif
		while (HAS_CLS_MORE_LITS(c, litC)){
			//Check if this literal was used in the MP clause. It can only be used if it was unassigned.
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				*msgsC = *msgsMPC;	//Copy the message.
				++msgsMPC;			//Go to the next message position.
				#ifdef COLLECTSTATS
				if (*wgtC > ONE){
					++sumHW;
				} else if (*wgtC == ONE){
					++sumNW;
				} else if (*wgtC < ONE){
					++sumLW;
				}
				++wgtC;
				#endif
			}
			++litC;
			++msgsC;
		}
		#ifdef COLLECTSTATS
		if (sumHW > STATS_MP_COUNTHWLITS_MAX){
			++stats_mp_hwLits[STATS_MP_COUNTHWLITS_MAX];
		} else {
			++stats_mp_hwLits[sumHW];
		}
		if (sumNW > STATS_MP_COUNTNWLITS_MAX){
			++stats_mp_nwLits[STATS_MP_COUNTNWLITS_MAX];
		} else {
			++stats_mp_nwLits[sumNW];
		}
		if (sumLW > STATS_MP_COUNTLWLITS_MAX){
			++stats_mp_lwLits[STATS_MP_COUNTLWLITS_MAX];
		} else {
			++stats_mp_lwLits[sumLW];
		}
		#endif
		//Here at this point, we have copied all the messages from the MP clause to the global clause, ignoring the positions
		//of already globally assigned variables. We can go on to the next MP clause.
		++mpC;
	}
	//We check the enabled learned clauses.
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		//Check if the clause is satisfied in the original formula.
		if (IS_LIT_SAT(c->repLit)) continue;

		//For all the literals in this clause, we check if they are assigned. If not, we copy the message from the mpClause
		//into the message array position of the global clause.
		litC = c->lits;
		msgsC = c->msgs;
		msgsMPC = mpC->omegas;

		#ifdef COLLECTSTATS
		sumHW = 0;
		sumNW = 0;
		sumLW = 0;
		wgtC = mpC->weights;
		++stats_mp_countedClauses;
		#endif
		while (HAS_CLS_MORE_LITS(c, litC)){
			//Check if this literal was used in the MP clause. It can only be used if it was unassigned.
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				*msgsC = *msgsMPC;	//Copy the message.
				++msgsMPC;			//Go to the next message position.
				#ifdef COLLECTSTATS
				if (*wgtC > ONE){
					++sumHW;
				} else if (*wgtC == ONE){
					++sumNW;
				} else if (*wgtC < ONE){
					++sumLW;
				}
				++wgtC;
				#endif
			}
			++litC;
			++msgsC;
		}
		#ifdef COLLECTSTATS
		if (sumHW > STATS_MP_COUNTHWLITS_MAX){
			++stats_mp_hwLits[STATS_MP_COUNTHWLITS_MAX];
		} else {
			++stats_mp_hwLits[sumHW];
		}
		if (sumNW > STATS_MP_COUNTNWLITS_MAX){
			++stats_mp_nwLits[STATS_MP_COUNTNWLITS_MAX];
		} else {
			++stats_mp_nwLits[sumNW];
		}
		if (sumLW > STATS_MP_COUNTLWLITS_MAX){
			++stats_mp_lwLits[STATS_MP_COUNTLWLITS_MAX];
		} else {
			++stats_mp_lwLits[sumLW];
		}
		#endif
		//Here at this point, we have copied all the messages from the MP clause to the global clause, ignoring the positions
		//of already globally assigned variables. We can go on to the next MP clause.
		++mpC;
	}
}

void mp_clauses_reset(){
	mp_clauses 		= NULL;
	mp_clsUsed 		= 0;
	mp_clsAvail 	= 0;
	mp_clsMaxSize 	= 0;
}

void mp_clauses_init(){
	mpClause *c;
	uint32_t i;

	mp_clsAvail = BLOCKSIZE;
	mp_clauses = malloc(sizeof(mpClause)*mp_clsAvail);
	if (mp_clauses == NULL){
		printf("c ERROR. MP was unable to allocate memory for the clauses. Out of memory?\n");
		mp_returnCode = MP_ERROR;
		#ifdef COLLECTSTATS
		++stats_mp_return_ERROR;
		#endif
		return;
	}
	mp_clsUsed		= 0;
	mp_clsMaxSize 	= 0;

	for (i = 0; i < mp_clsAvail; ++i){
		c = mp_clauses + i;
		SET_MP_CLS_LITS(c,NULL);
		SET_MP_CLS_OMEGAS(c,NULL);
		SET_MP_CLS_WEIGHTS(c,NULL);
	}

	for (i = 0; i < mp_clsAvail; ++i){
		c = mp_clauses + i;
		c->lits = malloc(sizeof(int32_t)*1U);
		c->lits[0] = 0;
		c->omegas = malloc(sizeof(float_ty)*1U);
		c->omegas[0] = 0.5f;
		c->weights = malloc(sizeof(float_ty)*1U);
		c->weights[0] = ONE;
	}
}

void mp_clauses_dispose(){
	uint32_t i;
	mpClause *c;
	if (mp_clauses != NULL){
		for (i = 0; i < mp_clsAvail; ++i){
			c = mp_clauses + i;
			if (GET_MP_CLS_LITS(c) != NULL){
				free(GET_MP_CLS_LITS(c));
				SET_MP_CLS_LITS(c,NULL);
			}
			if (GET_MP_CLS_OMEGAS(c) != NULL){
				free(GET_MP_CLS_OMEGAS(c));
				SET_MP_CLS_OMEGAS(c,NULL);
			}
			if (GET_MP_CLS_WEIGHTS(c) != NULL){
				free(GET_MP_CLS_WEIGHTS(c));
				SET_MP_CLS_WEIGHTS(c,NULL);
			}
		}
		free(mp_clauses);
		mp_clauses = NULL;
		mp_clsUsed = 0;
		mp_clsAvail = 0;
		mp_clsMaxSize = 0;
	}
}

#ifdef VERBOSE_MP
void mp_clauses_printClause(mpClause* c){
	int32_t *litC;
	float_ty *msgsC,*wghtC;
	printf("c   MP: Clause %p:\tLITS(OMEGAS,WEIGHTS): ", (void*)c);
	litC = GET_MP_CLS_LITS(c);
	msgsC = GET_MP_CLS_OMEGAS(c);
	wghtC = GET_MP_CLS_WEIGHTS(c);
	while (*litC != 0){
		printf("%7d(%f,%f) ", *litC, *msgsC, *wghtC);
		++litC;
		++msgsC;
		++wghtC;
	}
	printf("\n");
}

void mp_clauses_printAll(){
	uint32_t cNum;
	printf("c MP CLAUSES:\n");
	for (cNum = 0; cNum < mp_clsUsed; ++cNum){
		mp_clauses_printClause(mp_clauses+cNum);
	}
}
#endif
