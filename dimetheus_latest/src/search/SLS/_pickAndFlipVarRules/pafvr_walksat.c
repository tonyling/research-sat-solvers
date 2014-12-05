/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "pickAndFlipVarRules.h"

void sls_pickAndFlipVarRule_walksat_printHelp(){
	printf("c      %-3d: WALKSAT:\n", SLS_PICKANDFLIPVARRULE_WALKSAT);
    printf("c           Behavior: In an unsatisfied clause, pick a variable to flip using brake scores via WalkSAT.\n");
}

void sls_pickAndFlipVarRule_walksat_prepare(){
	//Nothing is to be done to initialize a call to WalkSAT.
	#ifdef VERBOSE_SLS
	printf("c     SLS:     PAFVR-Plug-in [%-3d: WALKSAT]:\n", SLS_PICKANDFLIPVARRULE_WALKSAT);
	printf("c     SLS:       -slsNoise %f.\n", param_slsNoise);
	#endif
}

void sls_pickAndFlipVarRule_walksat_paf(){
	//Performs a single WalkSAT try.
	//This method implements the flipping in the SLS solver, according to the WalkSAT heuristic. Its idea is to pick one of
	//the unsatisfied clauses at random, and in that clause, if there is a literal that can be flipped without braking other
	//clauses, pick one of those (brake score zero). If not, select the set of literals with least brake. Then, with
	//probability p, flip one of those at random, or with probability 1-p, pick any of the valid literals at random.
	//The current implementation computes brake values on the fly for the selected clause. This way we do not have to store
	//or update this value in the clauses.
	uint64_t numFlips = 0;
	slsClause **occs;
	slsClause *c;
	int32_t *litC, toEnforce = 0, best = 0,nf=0;
	uint32_t bestBrake, numBest;

	//We flip as long as we are allowed to. The search_maxNumSLSFlips is zero if this is unlimited.
	do {
		//We grab one of the unsatisfied clauses at random, if available. If not, we are done.
		if (sls_numUnsat == 0U){
			sls_returnCode = SLS_SAT;
			break;
		}
		//Nope, there are more clauses to still unsatisfied. Always remember that we fill the clause stack from position 1.
		c = sls_unsatClauses[numFlips % sls_numUnsat];

		//We now determine the number of best scored literals in the clause.
		bestBrake = UINT32_MAX;
		numBest = 0U;
		litC = GET_SLS_CLS_LITS(c);
		while ((toEnforce = -*litC) != 0){
			if (GET_SLS_VAR_BRAKE(toEnforce) < bestBrake){
				numBest = 1U;
				bestBrake = GET_SLS_VAR_BRAKE(toEnforce);
				best = -toEnforce;
			} else if (GET_SLS_VAR_BRAKE(toEnforce) == bestBrake){
				++numBest;
			}
			++litC;
		}

		//We now perform the WalkSAT selection of the literal to flip. In WalkSAT, we check if the best literal is to be flipped
		//which is the case if there is a literal with brake zero, or, if the random number generator says so.
		if (bestBrake == 0U || rand_realOne() > param_slsNoise){
			if (numBest > 1U){
				numBest = rand_mod(numBest);
				litC = GET_SLS_CLS_LITS(c);
				while ((toEnforce = -*litC) != 0){
					//We now pick the literal number numBest from all the literals in the clause having the best brake.
					if (GET_SLS_VAR_BRAKE(toEnforce) == bestBrake){
						if (numBest-- == 0U) {
							toEnforce = *litC;
							break;
						}
					}
					++litC;
				}
			} else {
				//If there is only one best literal, we can pick it right away as we retained it in best.
				toEnforce = best;
			}
		} else {
			//We just want any of the available literals. Since litC has not been modified since it was last used on the
			//clause, the difference in pointers of litC and the literals pointer of c is the size of the clause.
			numBest = rand_mod((uint32_t) (litC - GET_SLS_CLS_LITS(c)));
			litC = GET_SLS_CLS_LITS(c);
			while ((toEnforce = *litC) != 0){
				//We respect only literals not yet assigned by the search strategy.
				if (numBest-- == 0U) {
					toEnforce = *litC;
					break;
				}
				++litC;
			}
		}

		//From now on, toEnforce is the one literal we want to flip. We now perform the Flip. This includes:
		//	1. Changing the SLS assignment for the variable.
		//	2. In all clauses that the SLS must take care of (non satisfied repLit) where this literal occurs satisfied:
		//			Increase numTrue. If numTrue == 1, remove it from the clause stack holding the unsatisfied clauses.
		//							                   and set trueLit to toEnforce (it now satisfies the clause).
		//	3. In all clauses that the SLS must take care of (non satisfied repLit) where this literal occurs unsatisfied:
		//			Decrease numTrue. If numTrue == 0, add this clause to clause stack holding the unsatisfied clauses.
		//							  If numTrue == 1, update the trueLit of the clause (find one!).
		//							  If numTrue >  1, do nothing, there are plenty more literals available.

		//Step 1.
		SET_SLS_LIT_ASSIGNMENT_FLIP(toEnforce);

		//Step 2. Check the occurrence list of the literal becoming true.
		occs = GET_SLS_LIT_OCCS(toEnforce);
		while ((c = *occs) != NULL){
			//We first increase its numTrue and see to the sub-cases of 2.
			if (GET_SLS_CLS_NUMTRUE(c) == 0U){
				//We must remove this clause from the set of unsatisfied clauses. It was unsatisfied but will be satisfied now.
				SET_SLS_CLS_WHEREUNSAT(sls_unsatClauses[--sls_numUnsat], GET_SLS_CLS_WHEREUNSAT(c));
				sls_unsatClauses[GET_SLS_CLS_WHEREUNSAT(c)] = sls_unsatClauses[sls_numUnsat];
				SET_SLS_CLS_TRUELIT(c, toEnforce);
				SET_SLS_VAR_BRAKE_INC(toEnforce);
			} else if (GET_SLS_CLS_NUMTRUE(c) == 1U){
				//This clause will have more than one literal that satisfies it. We reduce the count for single satisfying
				//literals for this clause. The current true literal must be up to date, as it is updated every time the
				//number of true literals falls (or reaches) 1. The tLwU value is a literal in this branch of the if statement.
				SET_SLS_VAR_BRAKE_DEC(GET_SLS_CLS_TRUELIT(c));
			}
			SET_SLS_CLS_NUMTRUE_INC(c);
			++occs;
		}

		//Switch to the unsatisfied literal.
		toEnforce = -toEnforce;

		//Step 3. Check the occurrence list of the literal becoming false.
		occs = GET_SLS_LIT_OCCS(toEnforce);
		while ((c = *occs) != NULL){
			//We do need to check the clause. We first decrease its numTrue and see to the sub-cases of 3.
			SET_SLS_CLS_NUMTRUE_DEC(c);
			if (GET_SLS_CLS_NUMTRUE(c) == 0U){
				//The clause became unsatisfied by this action. We add the clause to the clause stack holding all unsatisfied.
				SET_SLS_VAR_BRAKE_DEC(GET_SLS_CLS_TRUELIT(c));
				SET_SLS_CLS_WHEREUNSAT(c, sls_numUnsat);
				sls_unsatClauses[sls_numUnsat++] = c;
			} else if (GET_SLS_CLS_NUMTRUE(c) == 1U){
				//In this branch of the if statement, the clause lost one literal making it true, leaving it with only one. We
				//update the true literal if and only if the true literal is not satisfied.
				if (IS_SLS_LIT_UNSAT(GET_SLS_CLS_TRUELIT(c))) {
					//It is the one making it true. We must find a replacement in the literals of the clause.
					litC = GET_SLS_CLS_LITS(c);
					while (*litC != 0){
						if (IS_SLS_LIT_SAT(*litC)){
							//The literal lit is the replacement for the true literal in the clause.
							SET_SLS_CLS_TRUELIT(c, *litC);
							break;
						}
						++litC;
					}
				}
				//This literal is now solely responsible for making the clause. We increase its brake value.
				SET_SLS_VAR_BRAKE_INC(GET_SLS_CLS_TRUELIT(c));
			}//else, the clause has at least two satisfying literals. Nothing to update.
			++occs;
		}
		//We periodically re-initialize the linear congruence generator to pick new random numbers.
		if (++nf > 3*f.n_initial){
			nf = 0;
			rand_init();
		}
		//We have now updated all the clauses numTrue and trueLit after the flip. We go on with the next flip.
	} while (++numFlips != sls_maxNumFlips);

	sls_numFlips += numFlips;

	#ifdef COLLECTSTATS
	stats_sls_ls_totalFlips += numFlips;
	#endif
}
