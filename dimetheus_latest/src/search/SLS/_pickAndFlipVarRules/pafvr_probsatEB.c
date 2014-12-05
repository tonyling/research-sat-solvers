/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "pickAndFlipVarRules.h"

void sls_pickAndFlipVarRule_probsatEB_printHelp(){
	printf("c      %-3d: PROBSAT-EB:\n", SLS_PICKANDFLIPVARRULE_PROBSATEB);
    printf("c           Behavior: Using ProbSAT exponential (brake only) probability scores to pick variables to flip.\n");
}

void sls_pickAndFlipVarRule_probsatEB_prepare(){
	#ifdef VERBOSE_SLS
	printf("c     SLS:     PAFVR-Plug-in [%-3d: PROBSAT-EB]:\n", SLS_PICKANDFLIPVARRULE_PROBSATEB);
	printf("c     SLS:       Maximum literal occurrence encountered: %d.\n", sls_litMaxOcc);
	printf("c     SLS:       -slsProbsatCB %f.\n", param_slsProbsatCB);
	#endif
	int32_t numOcc;
	//ProbSAT needs to assign scores for each literal according to its brake value. Since we do not want to re-compute
	//a score every time (and because we know the maximum value for the brake possible), we pre-compute all the
	//score values and later use this table to look them up on the fly.
	sls_probsat_scoreLookupB = realloc(sls_probsat_scoreLookupB, sizeof(float_ty)*(sls_litMaxOcc+1));
	sls_probsat_scoreLookupB[0] = 1.0f;
	for (numOcc = 1; numOcc < sls_litMaxOcc+1; ++numOcc){
		sls_probsat_scoreLookupB[numOcc] = (float_ty) pow(param_slsProbsatCB, -numOcc);
	}

	#ifdef VERBOSE_SLS
	printf("c     SLS:       Score table of size %d computed. The values are:", sls_litMaxOcc+1U);
	for (numOcc = 0; numOcc < sls_litMaxOcc+1; ++numOcc){
		if (numOcc % 10u == 0u) printf("\nc     SLS:       ");
		printf("%f ", sls_probsat_scoreLookupB[numOcc]);
	}
	printf("\n");
	#endif
}

void sls_pickAndFlipVarRule_probsatEB_paf(){
	//Performs a try using a ProbSAT exponential probability distribution using only brake score.
	uint64_t numFlips = 0;
	slsClause **occs;
	slsClause *c;
	int32_t *litC, toEnforce = 0,nf=0;
	float_ty totalScore;

	//We flip as long as we are allowed to. The search_maxNumSLSFlips is zero if this is unlimited.
	do {
		//We grab one of the unsatisfied clauses at random, if available. If not, we are done.
		if (sls_numUnsat == 0U){
			sls_returnCode = SLS_SAT;
			break;
		}
		//Nope, there are more clauses still unsatisfied.
		c = sls_unsatClauses[numFlips % sls_numUnsat];

		//We must now select a variable to flip from this clause using a probability distribution. We use the brake score
		//lookup table to determine the score of a variable given its brake value.
		totalScore = 0.0f;
		litC = GET_SLS_CLS_LITS(c);
		do {
			//Given the brake, we can set the raw score for the literal (not normalized), and add this score mass
			//to the total mass computed so far.
			totalScore += sls_probsat_scoreLookupB[GET_SLS_VAR_BRAKE(*litC)];
			++litC;
		} while (*litC != 0);

		//We now collected all the scores of the literals. We will subtract a random amount from this value, and then walk
		//through the clause again, subtracting what the literal has in score. As soon as the remaining score drops below zero,
		//we will use this literal for flipping.
		totalScore = totalScore * rand_realOne() - 0.00000001f; //just to make sure.
		litC = GET_SLS_CLS_LITS(c) - 1;
		do {
			//Given the brake, we can remove the raw score for the literal (not normalized) from the total score mass.
			++litC;
			totalScore -= sls_probsat_scoreLookupB[GET_SLS_VAR_BRAKE(*litC)];
		} while (totalScore > 0.0f);
		toEnforce = *litC;

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
