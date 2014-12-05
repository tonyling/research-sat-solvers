/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "updateRules.h"

void mp_updateRule_L0_SP_printHelp(){
	printf("c      %-3d: L0-SP:\n", MP_UPDATERULE_L0_SP);
    printf("c           Behavior: Performs Survey Propagation.\n");
}

void mp_updateRule_L0_SP_prepare(){
	//In this method, we see to it that the mp_messageFactors array has sufficient size.
	#ifdef VERBOSE_MP
	printf("c     MP:     UR-Plug-in [%-3d: L0-SP]:\n", MP_UPDATERULE_L0_SP);
	printf("c     MP:       Maximum clause size encountered: %d.\n", mp_clsMaxSize);
	#endif
	mp_clauseDeltas = realloc(mp_clauseDeltas, sizeof(float_ty)*(mp_clsMaxSize+1U));
	#ifdef VERBOSE_MP
	printf("c     MP:       mp_messageFactors resized.\n");
	#endif
}

void mp_updateRule_L0_SP_msgUpdate(){
	//This is the level 0 heuristics implementation of SP.
	mpClause *c;		//The clause we work on.
	int32_t *litC;		//The literals in the clause.
	float_ty *omegasC;	//The warning messages send by this clause.
	float_ty *deltas;	//The disrespect messages send towards this clause, computed on the fly.
	uint32_t i, zeros;	//The number of zero values in the disrespect messages.
	float_ty allProd;	//The product of all disrespect messages.
	float_ty newOmega = ZERO, U = ZERO, S = ZERO, denominator;
	for (i = 0; i < perm_lfsr_perLen; ++i){
		//First, we grab a clause number from the permutation and ignore it if it is beyond the clauses we have. We abuse the
		//memory position for zeros here, as we will not need it right now.
		zeros = perm_LFSR_nextState();
		if (zeros >= mp_clsUsed) continue;
		c = mp_clauses + zeros;

		//We picked clause c. We now walk through the clause and create deltas from all the literal S/U values in the clause.
		zeros = 0;
		allProd = ONE;
		litC = GET_MP_CLS_LITS(c);
		omegasC = GET_MP_CLS_OMEGAS(c);
		deltas = mp_clauseDeltas;
		while (*litC != 0){
			//Grab the S value by checking on the opposite literals T/F value. If this value is big, then the literal can be
			//assigned in such a way that it satisfies the clause.
			S = ((!GET_MP_LIT_NUMZEROS(-*litC)) * GET_MP_LIT_TORF(-*litC));
			#ifdef COLLECTSTATS
			++stats_mp_computation_numMult;
			#endif
			//Grab the U_l value by checking on the same literals T/F value. The value is small if the literal can be assigned
			//in such a way that it satisfies the clause.
			if (GET_MP_LIT_NUMZEROS(*litC) == 0U){
				//If there is no zero factor, then we get U_l by dividing out the 1-message for this literal in clause c.
				U = GET_MP_LIT_TORF(*litC) / (ONE - *omegasC);
				#ifdef COLLECTSTATS
				++stats_mp_computation_numDiv;
				#endif
			} else if (GET_MP_LIT_NUMZEROS(*litC) == 1U && (ONE - *omegasC) < param_mpEpsilon){
				//If there is exactly one zero factor and it is the one in the current clause c, then we can simply use
				//what we have computed, since this method ignored zero factors.
				U = GET_MP_LIT_TORF(*litC);
			} else {
				//If there is more than one zero factor, then the product must be zero too.
				U = 0;
			}
			//We have now updated the S and U values of l and are ready to update its disrespect message towards c.
			//Check the paper to see where the following formula comes from. This is the simplified equation.
			denominator = U*(1-S) + S;

			//We must distinct some cases here. In case the denominator is larger zero, we can compute the delta as usual.
			if (denominator > ZERO){
				//The "good" case happens. The machine precision is good enough to compute the delta.
				*deltas = (U*(1-S)) / denominator;
				#ifdef COLLECTSTATS
				++stats_mp_computation_numDiv;
				stats_mp_computation_numMult += 2;
				#endif
			} else {
				//The "bad" case happens. The denominator is zero, which indicates that the literal can neither be SAT nor
				//UNSAT. This means that the corresponding variable receives strong warnings from both sides -- it is pure
				//luck whether it will make the clause or not. Therefore, we set the delta to 0.5. The disrespect is half.
				*deltas = HALF;
				#ifdef COLLECTSTATS
				++stats_mp_computation_numMult;
				#endif
			}


			if (*deltas < param_mpEpsilon){
				//If more than two factors in clause c are zero, then all messages send by c will be zero.
				if (++zeros == 2U) break;
			} else{
				//The factor we just computed is not too small. We multiply it on top of all the other multiplications so far.
				allProd *= *deltas;
				#ifdef COLLECTSTATS
				++stats_mp_computation_numMult;
				#endif
			}
			++litC;
			++omegasC;
			++deltas;
		}
		//What follows is the update of all the omegas values of the clause and an update of the T/F values.
		litC = GET_MP_CLS_LITS(c);
		omegasC = GET_MP_CLS_OMEGAS(c);
		deltas = mp_clauseDeltas;
		while (*litC != 0){
			if (!zeros){
				newOmega = allProd / *deltas;
				#ifdef COLLECTSTATS
				++stats_mp_computation_numDiv;
				#endif
			} else if (zeros == 1U && *deltas < param_mpEpsilon){
				newOmega = allProd;
			} else {
				newOmega = ZERO;
			}

			//It now is: newOmega = \omega(c,v) (product of all deltas excluding the one for this literal).
			//We update the T/F values now.

			if (ONE - *omegasC > param_mpEpsilon){
				//The old factor in this clause was indeed used as a factor.
				if (ONE - newOmega > param_mpEpsilon){
					//The new factor is big enough as well. We divide out the old one and add the new one.
					SET_MP_LIT_REMOVEWARN_MULT(*litC, ONE - *omegasC);
					SET_MP_LIT_APPLYWARN_MULT(*litC, ONE - newOmega);
					#ifdef COLLECTSTATS
					++stats_mp_computation_numMult;
					++stats_mp_computation_numDiv;
					#endif
				} else {
					//The new factor is not big enough. We divide out the old, but just increase the number of zeros.
					SET_MP_LIT_REMOVEWARN_MULT(*litC, ONE - *omegasC);
					SET_MP_LIT_NUMZEROSINC(*litC);
					#ifdef COLLECTSTATS
					++stats_mp_computation_numDiv;
					#endif
				}
			} else {
				//The old factor was not big enough to be used as a factor.
				if (ONE - newOmega > param_mpEpsilon){
					//However, the new factor is. We multiply in the new factor and decrease the number of zeros.
					SET_MP_LIT_APPLYWARN_MULT(*litC, ONE - newOmega);
					SET_MP_LIT_NUMZEROSDEC(*litC);
					#ifdef COLLECTSTATS
					++stats_mp_computation_numMult;
					#endif
				}//else, there is nothing that changes because the old and the new factor are both zero.
			}

			//We now check how much the omega(c,v) did change. If this is the biggest change we have seen so far,
			//we will remember it. This is later used to see if message passing converged.
			if (newOmega > *omegasC){
				if (mp_maxMessageDiff < newOmega - *omegasC) {
					//Yes, the difference is bigger than what we have seen so far.
					mp_maxMessageDiff = newOmega - *omegasC;
				}
			} else {
				if (mp_maxMessageDiff < *omegasC - newOmega ) {
					//Yes, the difference is bigger than what we have seen so far.
					mp_maxMessageDiff = *omegasC - newOmega;
				}
			}

			//We update the omega(c,v) message for the current variable.
			*omegasC = newOmega;

			++litC;
			++omegasC;
			++deltas;
			#ifdef COLLECTSTATS
			++stats_mp_passing_totalMsgUpdates;
			#endif
		}
	}
}

void mp_updateRule_L0_SP_computeBias(){
	float_ty F,T,muPlus,muMinus,muPM,mu;
	mpVariable* var;
	uint32_t i, usedVars = 0;

	mp_globalMagnetization = 0;
	for (i = 0; i < f.n_vars_e_used; ++i){
		//We again ignore already assigned variables.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		var = mp_variables + f.vars_e[i]->id;
		++usedVars;

		//Grab the variables freedom to be assigned positively and negatively: T and F.
		//Check the paper on the computation of magnetization and biases in SP.
		F = GET_MP_VAR_BIGF(f.vars_e[i]->id);
		T = GET_MP_VAR_BIGT(f.vars_e[i]->id);

		if (F > ZERO || T > ZERO){
			//If not both products are zero, the variable will have a real bias.
			muPM 	= T*F;						//Compute \mu^+-
			muPlus 	= T*(ONE-F);	            //Compute \mu^+
			muMinus = F*(ONE-T);	            //Compute \mu^-

			mu      = muPlus + muMinus + muPM;  //Compute \mu

			//Check if the total magnetization is larger zero. If not, the bias will be zero.
			if (mu == ZERO){
				muPlus = ZERO;
				muMinus = ZERO;
			} else {
				muPlus  = muPlus  / mu;											//Compute \beta^+
				muMinus = muMinus / mu;											//Compute \beta^-
			}

			var->bias = muPlus - muMinus;		//Compute \beta

			if (muPlus > muMinus){
				mp_globalMagnetization += muPlus;
			} else {
				mp_globalMagnetization += muMinus;
			}

		} else {
			//We set the bias of a variable to zero if both of the products are zero.
			var->bias 	= ZERO;
		}
	}
	if (usedVars > 0U) {
		mp_globalMagnetization = mp_globalMagnetization/((float_ty)usedVars);
	}
}
