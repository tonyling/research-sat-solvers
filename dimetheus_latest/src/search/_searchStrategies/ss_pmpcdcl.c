/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "searchStrategies.h"

void search_strategy_pmpcdcl_printHelp(){
	printf("c      %-3d: PMPCDCL\n", SEARCH_STRATEGY_PMPCDCL);
    printf("c           Behavior: Performs rho-sigma-PMP-i and uses the biases to guide CDCL search [MP, CDCL].\n");
}

void search_strategy_pmpcdcl_reset(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy reset (PMPCDCL)...\n");
	#endif
	inp_resetPhase();
	mp_resetModule();
	cdcl_resetModule();

	search_strategy_pmpcdcl_midBlockSize = 1;
	search_strategy_pmpcdcl_assignmentMax = 1;
	search_strategy_pmpcdcl_maxMPImpact = ZERO;
}

void search_strategy_pmpcdcl_init(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy init (PMPCDCL)...\n");
	#endif
	inp_initPhase(); if (inp_returnCode != INP_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
	mp_initModule(); if (mp_returnCode != MP_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }
	cdcl_initModule(); if (cdcl_returnCode != CDCL_UNKNOWN){ search_returnCode = SEARCH_ERROR; return; }

	//Search strategy settings.
	search_strategy_pmpcdcl_midBlockSize = param_searchMIDBlockSize * f.n_vars_e_used;
	if (search_strategy_pmpcdcl_midBlockSize == 0U) ++search_strategy_pmpcdcl_midBlockSize;
	search_strategy_pmpcdcl_assignmentMax = search_strategy_pmpcdcl_midBlockSize;
	search_strategy_pmpcdcl_maxMPImpact = f.n_vars_e_used * param_searchImpactMaxFactor;

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH:   param_searchImpactMaxFactor              = %f\n", param_searchImpactMaxFactor);
	printf("c   SEARCH:   param_searchMinClsImpact                 = %f\n", param_searchMinClsImpact);
	printf("c   SEARCH:   param_searchMIDBlockSize                 = %f\n", param_searchMIDBlockSize);
	printf("c   SEARCH:   search_strategy_pmpcdcl_midBlockSize     = %u\n", search_strategy_pmpcdcl_midBlockSize);
	printf("c   SEARCH:   search_strategy_pmpcdcl_assignmentMax    = %u\n", search_strategy_pmpcdcl_assignmentMax);
	printf("c   SEARCH:   search_strategy_pmpcdcl_maxMPImpact      = %f\n", search_strategy_pmpcdcl_maxMPImpact);
	#endif
}

void search_strategy_pmpcdcl_dispose(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy dispose (PMPCDCL)...\n");
	#endif
	inp_disposePhase();
	mp_disposeModule();
	cdcl_disposeModule();

	search_strategy_pmpcdcl_midBlockSize = 1;
	search_strategy_pmpcdcl_assignmentMax = 1;
	search_strategy_pmpcdcl_maxMPImpact = ZERO;
}

void search_strategy_pmpcdcl_execute(){
	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Strategy execute (PMPCDCL)...\n");
	#endif

	uint32_t restart = 1;
	float_ty retainedImpact = ZERO, impact;

	do {
		//Check how to proceed. We have three possibilities.
		//If restart == 1: we want everything to be completely restarted. The CDCL will forget all its learned clauses.
		//If restart == 0: we want the CDCL to proceed its search from where it left off.
		if (restart == 1){
			#ifdef COLLECTSTATS
			++stats_search_restarts;
			#endif
			//We want to completely restart the search. For MP, we perform on the global formula. For CDCL, we initialize
			//everything -- including the learned clauses and the variable and value selection heuristics based on the result
			//computed by MP.
			//We reset the assignment target for the CDCL.
			search_strategy_pmpcdcl_assignmentMax = search_strategy_pmpcdcl_midBlockSize;
			search_strategy_pmpcdcl_maxMPImpact = f.n_vars_e_used * param_searchImpactMaxFactor;
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: Restarting. VARS: %6d CLS: %6d MAXASSIGN: %6d MAXIMPACT: %f\n",
					f.n_vars_e_used, f.m_eo_used + f.m_el_used,	search_strategy_pmpcdcl_assignmentMax,
					search_strategy_pmpcdcl_maxMPImpact);
			#endif
			//We perform MP on the global formula and check on its result.
			mp_extern_iterate(1);
			//If MP did converge, we can compute biases, otherwise we stick to the current ones.
			if (mp_returnCode == MP_ERROR){
				printf("c ERROR. PMPCDCL asked for MP but this resulted in an error (A). PMPCDCL failed.\n");
				search_returnCode = SEARCH_ERROR;break;
			} else if (mp_returnCode == MP_CONVERGED) {
				//Yes, it did converge. Computing new biases is useful.
				mp_extern_computeBiases();
			}
			//Independent of whether we did compute new biases or stick to the old ones, we call CDCL with full re-preparation.
			//This will, in the current search strategy, also respect new biases for initializing the variable and value
			//selection heuristics.
			cdcl_extern_learn(1);
		} else {
			//We do not restart. In order to better guide the CDCL on the remaining formula, we will perform MP on the
			//restriction based on the CDCL assignment. We do this, however, if and only if the impact of learned clauses is
			//not too big (this would indicate that MP is not helpful anyway).
			if (retainedImpact < search_strategy_pmpcdcl_maxMPImpact){
				//It seems that doing MP gives good guidance as the impact of learned things is low.
				search_strategy_pmpcdcl_assumeAndUpdate(); if (search_returnCode != SEARCH_UNKNOWN){break;}
				//Check on how to best use the result of MP.
				if (mp_returnCode == MP_CONVERGED_PARAMAG){
					//MP successfully converged, but ran into a paramagnetic state. This implies that the remaining sub-formula
					//is easy to solve. Therefore, we have the CDCL try to assign all remaining variables.
					search_strategy_pmpcdcl_assignmentMax = f.n_initial +2;
					#ifdef VERBOSE_SEARCH
					printf("c   SEARCH: Low  retained impact. PARAMAGNETIC. VARS: %6d CLS: %6d MAXASSIGN: %6d\n",
							f.n_vars_e_used, f.m_eo_used + f.m_el_used,	search_strategy_pmpcdcl_assignmentMax);
					#endif
				} else {
					//MP did either not converge or converged into a non-paramagnetic state. If it did not converge, it also
					//did not update any biases. If it successfully converged into a non-paramagnetic state it updated new
					//biases. The CDCL already updated it heuristics. In both cases, we want the CDCL to assign a new MID block.
					search_strategy_pmpcdcl_assignmentMax = CDCL_DECSTACK_GET_NUM_ASSIGNMENTS() + search_strategy_pmpcdcl_midBlockSize;
					if (search_strategy_pmpcdcl_assignmentMax >= f.n_vars_e_used){
						//Just to make sure that the CDCL cannot get stuck.
						search_strategy_pmpcdcl_assignmentMax = f.n_initial +2;
					}
					#ifdef VERBOSE_SEARCH
					if (mp_returnCode == MP_UNCONVERGED){
						printf("c   SEARCH: Low retained impact. UNCONVERGED. VARS: %6d CLS: %6d MAXASSIGN: %6d\n",
								f.n_vars_e_used, f.m_eo_used + f.m_el_used,	search_strategy_pmpcdcl_assignmentMax);
					} else {
						printf("c   SEARCH: Low retained impact. Non-paramagnetic. VARS: %6d CLS: %6d MAXASSIGN: %6d\n",
								f.n_vars_e_used, f.m_eo_used + f.m_el_used,	search_strategy_pmpcdcl_assignmentMax);
					}
					#endif
				}
			} else {
				//It seems that MP gave us bad guidance as the CDCL had to learn a lot of impacting things in order to reach
				//the assignment target. We will now disable MP until the CDCL learned a new unit.
				search_strategy_pmpcdcl_assignmentMax = f.n_initial +2;
				#ifdef VERBOSE_SEARCH
				printf("c   SEARCH: HIGH RETAINED IMPACT. VARS: %6d CLS: %6d MAXASSIGN: %6d\n",
						f.n_vars_e_used, f.m_eo_used + f.m_el_used,	search_strategy_pmpcdcl_assignmentMax);
				#endif
			}
			cdcl_extern_learn(0);
		}

		//Check the result of the CDCL computations.
		if (cdcl_returnCode == CDCL_SAT){
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: CDCL found a satisfying assignment. PMPCDCL succeeded.\n");
			#endif
			search_intern_transferCDCLAss();
			search_returnCode = SEARCH_SAT;
		} else if (cdcl_returnCode == CDCL_UNSAT){
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH: CDCL found a global conflict. PMPCDCL succeeded.\n");
			#endif
			search_returnCode = SEARCH_UNSAT;
		} else if (cdcl_returnCode == CDCL_UNKNOWN){
			//We investigate the information gathered by the CDCL. We assume, however, that this does not require us to
			//completely restart the search.
			restart = 0;
			impact = ZERO;

			//We check if the CDCL did find new DL0 assignments. If so, it is time for in-processing.
			if (CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() > 0){
				//We retain equivalences found by the CDCL.
				impact += search_strategy_pmpcdcl_retainNewEquivalences();		if (search_returnCode != SEARCH_UNKNOWN) break;
				//We retain highly impacting clauses that have been learned by the CDCL.
				impact += search_strategy_pmpcdcl_convertImpactingClauses();	if (search_returnCode != SEARCH_UNKNOWN) break;
				//We retain unit clauses that have been found.
				impact += search_strategy_pmpcdcl_retainNewUnits();				if (search_returnCode != SEARCH_UNKNOWN) break;
				retainedImpact += impact;
				#ifdef VERBOSE_SEARCH
				printf("c   SEARCH: New impact: %f. Current retained impact (will be reset): %f.\n", impact, retainedImpact);
				#endif
				//We then perform in-processing with all the new knowledge.
				inp_extern_inprocessing();
				if (inp_returnCode == INP_UNSAT){
					#ifdef VERBOSE_SEARCH
					printf("c   SEARCH: INP returned unsatisfiable. PMPCDCL succeeded.\n");
					#endif
					search_returnCode = SEARCH_UNSAT;
				} else if (inp_returnCode == INP_SAT){
					#ifdef VERBOSE_SEARCH
					printf("c   SEARCH: INP returned satisfiable. PMPCDCL succeeded.\n");
					#endif
					search_returnCode = SEARCH_SAT;
				} else if (inp_returnCode == INP_ERROR){
					#ifdef VERBOSE_SEARCH
					printf("c   SEARCH: INP returned an error. PMPCDCL failed.\n");
					#endif
					search_returnCode = SEARCH_ERROR;
				}
				//Since the formula did undergo some strong changes, we will completely restart the search. We reset the
				//retained impact of learned clauses -- they are all treated as "not new" now. The CDCL will, from now on,
				//treat these clauses as "original clauses".
				restart = 1;
				retainedImpact = ZERO;
			} else {
				//The CDCL did not detect new units. We retain highly impacting clauses that have been learned by the CDCL.
				impact += search_strategy_pmpcdcl_convertImpactingClauses();	if (search_returnCode != SEARCH_UNKNOWN) break;
				retainedImpact += impact;
				#ifdef VERBOSE_SEARCH
				printf("c   SEARCH: New impact: %f. Current retained impact (will be kept): %f.\n", impact, retainedImpact);
				#endif
			}
		} else {
			#ifdef VERBOSE_SEARCH
			printf("c   SEARCH:   CDCL returned an unexpected return code. PMPCDCL failed.\n");
			#endif
			search_returnCode = SEARCH_ERROR;
		}
	} while (search_returnCode == SEARCH_UNKNOWN);
}

uint32_t search_strategy_pmpcdcl_isCDCLClauseSubsumed(cdclClause *cc){
	//This method determines if the CDCL clause is actually subsumed by any clause in the current main formula. It returns 1
	//if this is the case.
	//We first check if the clause is subsumed by any known binary. In order to do that, we stamp all the literals and then
	//walk through the implication lists of the literals. If we find a stamped literal in there we can stop and return 1.

	clause *c;
	uint32_t i,j;
	int32_t lit, impLit;
	++main_litStamp;
	for (i = 0; i < GET_CDCL_CLS_SIZE(cc); ++i){
		//Stamp the literal at the current position.
		SET_LIT_NUM_STAMP(GET_CDCL_CLS_LITNUM(cc,i), main_litStamp);
	}

	//Check the binary clauses.
	for (i = 0; i < GET_CDCL_CLS_SIZE(cc); ++i){
		//Get the opposite literal in order to check its implications.
		lit = -GET_CDCL_CLS_LITNUM(cc,i);
		for (j = 0; j < GET_LIT_NUM_NUMIMP_USED(lit); j+=2){
			impLit = GET_LIT_NUM_IMPNUM(lit,j);
			//Check if the implied literal is stamped.
			if (GET_LIT_NUM_STAMP(impLit) == main_litStamp) {
				//Yes, the clause is subsumed by the binary behind the implication with lit and impLit.
				return 1;
			}
		}
	}

	//If we reach this point, then the clause is not subsumed by any binary clause in the main formula. We must also check the
	//large clauses. Here, we will grab the occurrences of all the literals in the clause (ignoring binaries) and check with
	//the signature of the clauses if a subsumption is possible. If this is the case, we walk through the occurrence clause
	//and check if all the literals in it are stamped. If and only if this is the case, the clause is subsumed and we can
	//return one.
	for (i = 0; i < GET_CDCL_CLS_SIZE(cc); ++i){
		//Get the literal in order to check its occurrence list.
		lit = GET_CDCL_CLS_LITNUM(cc,i);
		for (j = 0; j < GET_LIT_NUM_NUMOCCS_USED(lit); ++j){
			c = GET_LIT_NUM_OCCNUM(lit,j);
			//Check if the clause is binary or if its size is too large or if the signature already indicates no subsumption.
			if (GET_CLS_SIZE(c) == 2 || GET_CLS_SIZE(c) > GET_CLS_SIZE(cc) || (GET_CLS_SIG(c) & ~GET_CDCL_CLS_SIG(cc)) != 0ULL){
				//The clause is either binary, too large, or its signature contains stuff not in the clause. No subsumption.
				continue;
			}
			//The simple checks did not rule out a possible subsumption of cc with c. We must take a look at all literals of
			//clause c and check if they are stamped. If they are all stamped, then cc is subsumed by c.
			for (impLit = 0; impLit < GET_CLS_SIZE(c); ++impLit){
				lit = GET_CLS_LITNUM(c, impLit);
				if (GET_LIT_NUM_STAMP(lit) != main_litStamp){
					//This literal is not stamped. So c cannot be subsuming here.
					break;
				}
			}
			//Check if we have seen all literals in c. If not, there was at least one that was not stamped.
			if (impLit == GET_CLS_SIZE(c)){
				//Yes, all literals are stamped. Clause cc is subsumed by c.
				return 1;
			}
		}
	}
	return 0;
}

float_ty search_strategy_pmpcdcl_updateMessages(clause *c){
	//This method computes the warning messages send by this clause based on the current MP biases of the variables. The idea
	//is to use the abstract of the biases as an indication how strongly confined a variable already is. The more, the better.
	//The warning message send by the clause to a variables is the product of the bias abstracts of all other variables.

	//We initialize the impact to be one.
	float_ty impact = ONE, biasAbs, newOmega;
	uint32_t i, zeros = 0;

	//We first compute the global impact of this clause, and using this value, we re-construct the omega for each literal.
	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		biasAbs = GET_VAR_ACTIVITY((main_varData + abs(GET_CLS_LITNUM(c, i))));
		if (biasAbs < param_mpEpsilon){
			if (++zeros > 1) break;
		} else {
			impact *= biasAbs;
		}
	}

	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		biasAbs = GET_VAR_ACTIVITY((main_varData + abs(GET_CLS_LITNUM(c, i))));
		if (!zeros){
			//There were no zero factors. We get the warning send to the variable by dividing out the value that the variable
			//added to the impact itself.
			newOmega = impact / biasAbs;
		} else if (zeros == 1U && biasAbs < param_mpEpsilon){
			//The was exactly one zero factor and the zero factor belongs to the literal. The warning send to the variable is
			//the current impact value.
			newOmega = impact;
		} else {
			//There were at least two literals that are almost certainly satisfied. All the warnings will be zero.
			newOmega = ZERO;
		}
		//Write the warning -- the probability that no other literal will make this clause satisfied.
		SET_CLS_MSGNUM(c,i,newOmega);
	}

	//Finally, we check if there was a zero factor. If so, the impact of the clause will be zero too.
	if (zeros) impact = ZERO;

	return impact;
}

float_ty search_strategy_pmpcdcl_retainNewEquivalences(){
	//This method checks on all the remaining variables and tries to find out if they have been detected as equivalences by
	//the CDCL. If so, we will introduce two new binary clauses in order to explicitly state this knowledge (the
	//in-processor can then use ELS to extract that knowledge again).
	//In case the CDCL found X <->  Y, we add X <->  Y = X-> Y AND  Y->X = (-X v  Y) AND (-Y v X).
	float_ty clsImpact = ZERO, cumulativeImpact = ZERO;
	int32_t X, Y;
	uint32_t i, j;

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Retaining CDCL equivalences... ");
	uint32_t equivRetained = 0;
	#endif

	for (i=0; i < f.n_vars_e_used; ++i){
		//We only need to look at variables that are not yet assigned.
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;
		//We check if the corresponding CDCL variable has been marked as being equivalent to some other variable.
		Y = GET_CDCL_VAR_ISEQUIVTO(GET_VAR_ID(f.vars_e[i]));
		if (Y != 0){
			//Yes, we have the equivalence X <-> Y. We add the binaries.
			X = GET_VAR_ID(f.vars_e[i]);

			//We first check of the clause (-X v Y can be added). If so, we handle the addition of (-X v Y).
			for (j = 0; j < GET_LIT_NUM_NUMIMP_USED(X); j+=2){
				//If we find the literal in the implication list of X we do not have to add the binary.
				if (GET_LIT_NUM_IMPNUM(X, j) == Y) break;
			}
			//Check if we had to look through all the implications. If not, we have already found it.
			if (j == GET_LIT_NUM_NUMIMP_USED(X)){
				//Yes, we can add the new implication as it was not found.
				#ifdef VERBOSE_SEARCH
				++equivRetained;
				#endif
				clauses_addLearnedBinary(-X, Y);
				clsImpact = search_strategy_pmpcdcl_updateMessages(f.clauses_el[f.m_el_used-1]);
				cumulativeImpact += clsImpact;
				#ifdef COLLECTSTATS
				++stats_search_childClausesRetainedB;
				if (floor(100.0*clsImpact) < 100.0) ++stats_search_impactsB[((uint32_t)floor(100.0*clsImpact))];
				#endif
			}
			//We first check of the clause (X v -Y can be added). We handle the addition of (X v -Y).
			for (j = 0; j < GET_LIT_NUM_NUMIMP_USED(-X); j+=2){
				//If we find the literal in the implication list of X we do not have to add the binary.
				if (GET_LIT_NUM_IMPNUM(-X, j) == -Y) break;
			}
			//Check if we had to look through all the implications. If not, we have already found it.
			if (j == GET_LIT_NUM_NUMIMP_USED(-X)){
				//Yes, we can add the new implication as it was not found.
				clauses_addLearnedBinary(X, -Y);
				clsImpact = search_strategy_pmpcdcl_updateMessages(f.clauses_el[f.m_el_used-1]);
				cumulativeImpact += clsImpact;
				#ifdef COLLECTSTATS
				++stats_search_childClausesRetainedB;
				if (floor(100.0*clsImpact) < 100.0) ++stats_search_impactsB[((uint32_t)floor(100.0*clsImpact))];
				#endif
			}
		}
	}

	#ifdef VERBOSE_SEARCH
	printf("%d retained with a cumulative impact of %f.\n", equivRetained, cumulativeImpact);
	#endif
	return cumulativeImpact;
}

float_ty search_strategy_pmpcdcl_convertImpactingClauses(){
	//This method will check all the learned clauses of the CDCL. If they have a strong impact on MP or are binary, they will
	//be converted into a learned clause for the main formula.
	cdclClause *cc;
	float_ty cumulativeImpact = ZERO, impact, biasAbs, clsImpact;
	uint32_t i,j,zeros;
	int32_t lit1, lit2, aLit, impLit;

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Retaining CDCL clauses... ");
	uint32_t bretained = 0, lretained = 0;
	#endif

	for (i = 0; i < CDCL_GET_NUM_LEARNED_CLAUSES(); ++i){
		//Grab the next learned clause that is larger than two.
		cc = GET_CDCL_LEARNED_CLS_NUM(i);

		//If the clause is blocked we know that we have learned with it already. No need to do this again.
		if (IS_CDCL_CLS_BLOCKED(cc)) continue;

		//Check if it is binary or large size.
		if (GET_CDCL_CLS_SIZE(cc) == 2){
			//Here, cc is a clause that is binary. We must now find out if it is a NEW binary that we can learn.
			lit1 = GET_CDCL_CLS_LITNUM(cc,0);
			lit2 = GET_CDCL_CLS_LITNUM(cc,1);
			//We check the implication list that is smaller.
			if (GET_LIT_NUM_NUMIMP_USED(-lit1) < GET_LIT_NUM_NUMIMP_USED(-lit2)){
				aLit = -lit1;
				impLit = lit2;
			} else {
				aLit = -lit2;
				impLit = lit1;
			}
			//We check the implication list of aLit trying to find impLit.
			for (j = 0; j < GET_LIT_NUM_NUMIMP_USED(aLit); j+=2){
				if (GET_LIT_NUM_IMPNUM(aLit,j) == impLit){
					//We have found the implied literal.
					break;
				}
			}
			if (j < GET_LIT_NUM_NUMIMP_USED(aLit)){
				//We did not check all the implications of aLit. This means we have found the implied literal.
				continue;
			}
			//If we get this far, the implication is new and we can convert it.
			clauses_addLearnedBinary(lit1, lit2);
			//We tell the CDCL that it cannot remove this clause anymore.
			SET_CDCL_CLS_BLOCKED_1(cc);
			//We now set the correct messages send by this clause.
			clsImpact = search_strategy_pmpcdcl_updateMessages(f.clauses_el[f.m_el_used-1]);
			cumulativeImpact += clsImpact;
			#ifdef COLLECTSTATS
			++stats_search_childClausesRetainedB;
			if (floor(100.0*clsImpact) < 100.0) ++stats_search_impactsB[((uint32_t)floor(100.0*clsImpact))];
			#endif
			#ifdef VERBOSE_SEARCH
			++bretained;
			#endif
		} else {
			//We will now first compute the impact of this large clause on MP, if the impact is large, we convert it into a
			//learned clause for the main formula.
			impact = ONE; zeros = 0;
			for (j = 0; j < GET_CDCL_CLS_SIZE(cc); ++j){
				biasAbs = GET_VAR_ACTIVITY((main_varData + abs(GET_CLS_LITNUM(cc,j))));
				if (biasAbs < param_mpEpsilon){
					zeros = 1; break;
				} else {
					impact *= biasAbs;
				}
			}
			//For all clauses that are currently acting as reasons, we convert them. For all clauses that are no reasons, we
			//check on the impact. If there was a zero factor we ignore this clause right away. It would have no impact. We
			//also ignore the clause if its impact is too small. We also ignore it if it gets subsumed by anything we know.
			if (!IS_CDCL_CLS_REASON(cc)
					&& (zeros || impact < param_searchMinClsImpact || search_strategy_pmpcdcl_isCDCLClauseSubsumed(cc))) {
				continue;
			}
			clauses_addLearned(GET_CDCL_CLS_LITS(cc), GET_CDCL_CLS_SIZE(cc));
			//We tell the CDCL that it cannot remove this clause anymore.
			SET_CDCL_CLS_BLOCKED_1(cc);
			clsImpact = search_strategy_pmpcdcl_updateMessages(f.clauses_el[f.m_el_used-1]);
			cumulativeImpact += clsImpact;
			#ifdef COLLECTSTATS
			++stats_search_childClausesRetainedL;
			if (floor(100.0*clsImpact) < 100.0) ++stats_search_impactsL[((uint32_t)floor(100.0*clsImpact))];
			#endif
			#ifdef VERBOSE_SEARCH
			++lretained;
			#endif
		}
	}

	#ifdef VERBOSE_SEARCH
	printf("%d binary and %d large clauses retained with a combined impact of %f.\n", bretained, lretained, cumulativeImpact);
	#endif

	return cumulativeImpact;
}

float_ty search_strategy_pmpcdcl_retainNewUnits(){
	//This method assigns all the new unit assignments found by the CDCL.
	cdclLiteral *unitLit;
	literal *l;
	variable *v;
	float_ty unitImpact = ZERO;
	int32_t i = 0;

	#ifdef VERBOSE_SEARCH
	printf("c   SEARCH: Retaining CDCL units... ");
	int32_t numOldUnits = MAIN_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO();
	#endif

	//We back-jump to DL0 in the main decision stack.
	main_decStack_backJump(0,0,0);

	while (i < CDCL_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO()){
		//We grab the next DL0 assignment of the CDCL and add it to the main decision stack in DL0.
		unitLit = cdcl_decStackPtrs[0][i];
		l = main_litData + GET_CDCL_LIT_ID_FROM_PTR(unitLit);
		v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
		if (!IS_VAR_UNASSIGNED(v)) {
			printf("c ERROR. PMPCDCL tried to transfer units and found a variable that is already assigned.\n");
			search_returnCode = SEARCH_ERROR;
			return ZERO;
		}
		MAIN_COMPLEXUP_ADD_DEC_PTR(l,v);
		++i;
		unitImpact += ((float_ty)GET_LIT_NUMOCCS_USED(GET_LIT_OPPOSITE(l)) + ONE) / ((float_ty)GET_LIT_NUMOCCS_USED(l) + ONE);
	}

	//After collecting the DL0 assignments, we propagate.
	if (main_complexup_propagate(0) != NULL){
		//The propagation ended in a global conflict, we can stop.
		#ifdef VERBOSE_SEARCH
		printf("global conflict. PMPCDCL succeeded... ");
		#endif
		search_returnCode = SEARCH_UNSAT;
	} else if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		//The propagation removed all remaining clauses, we can stop.
		#ifdef VERBOSE_SEARCH
		printf("cleared the remaining formula. PMPCDCL succeeded... ");
		#endif
		search_returnCode = SEARCH_SAT;
	}
	#ifdef VERBOSE_SEARCH
	printf("%d retained (%d total) with an impact of %f.\n", MAIN_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO() - numOldUnits,
			MAIN_DECSTACK_GET_NUM_ASSIGNMENTS_IN_LEVEL_ZERO(), unitImpact);
	#endif

	return unitImpact;
}

void search_strategy_pmpcdcl_assumeAndUpdate(){
	//This method will first assign all the variables according to the CDCL assignment. It will then perform MP again, which
	//leads to biases under the assumptions made by the CDCL. We implement this by first assigning the variables according to
	//CDCL. We then iterate, compute the biases, and then again undo all the assignments.

	variable *v;
	uint32_t dl;
	int32_t decID;

	//First, we assign the variables according to the CDCL. We follow its decisions. Recall, that the CDCL returned with
	//result CDCL_UNKNOWN. This implies that the current assignment made by the CDCL cannot be conflicting. We simply
	//propagate all the assignments it made temporarily and then call for MP again.
	for (dl = 1; dl < CDCL_GET_DECLEVEL(); ++dl){
		//We get the CDCL decision in the specific level.
		decID = CDCL_GET_DECISION_ID_IN_LEVEL(dl);
		//We add this assignment to the main decision stack and propagate.
		v = main_varData + abs(decID);
		MAIN_COMPLEXUP_ADD_DEC(decID, v);
		if (main_complexup_propagate(0) != NULL){
			printf("c ERROR. PMPCDCL tried to follow the CDCL decisions and ran into a conflict (U). PMPCDCL failed.\n");
			search_returnCode = SEARCH_ERROR;
			return;
		}
	}

	//We then iterate on the remaining sub-formula.
	mp_extern_iterate(1);
	//If MP did converge, we can compute biases. Otherwise, we assume the information is useless and stick to the old biases.
	if (mp_returnCode == MP_ERROR){
		printf("c ERROR. PMPCDCL asked for MP but this resulted in an error. PMPCDCL failed.\n");
		search_returnCode = SEARCH_ERROR;return;
	} else if (mp_returnCode == MP_CONVERGED) {
		//Yes, it did converge. Computing new biases is useful.
		mp_extern_computeBiases();
		//If the biases are indeed helpful, we make the CDCL re-organize its variable and value selection heuristics.
		if (mp_returnCode == MP_CONVERGED_NONPARA){
			//Yes, valuable biases have been computed. Tell CDCL to update.
			cdcl_extern_reOrganizeRemainingVars();
		}
	}

	//We must now undo all the temporary assignments made. We back-jump to DL1 in erase mode. The CDCL will keep its updated
	//variable and value selection heuristic and can use the biases created for the sub-formula it works on.
	main_decStack_backJump(1,1,0);
}
