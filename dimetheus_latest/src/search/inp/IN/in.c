/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "in.h"

void in_extern_printVersion(){
	printf("[Inprocessing    ] [Inprocessor                    ] :: %3d.%-4d :: %s",
			IN_VERSION_MA, IN_VERSION_MI, IN_VERSION_NAME);
}

void in_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	#ifdef VERBOSE_IN
	printf("c       IN: Component reset...\n");
	#endif
	in_returnCode = IN_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_initModule(){
	#ifdef VERBOSE_IN
	printf("c       IN: Component init...\n");
	#endif
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	in_returnCode = IN_UNKNOWN;
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif

}

void in_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	#ifdef VERBOSE_IN
	printf("c       IN: Component dispose...\n");
	#endif
	in_returnCode = IN_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_extern_SE(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//Performs SE with all the clauses in the clause queue.
	clause *c, *d;
	literal *l = NULL;
	uint32_t num, counted, numD, occSize = UINT32_MAX;

	#ifdef VERBOSE_IN
	printf("SE");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_el_used + f.m_eo_used;
	#endif

	while ((c = cQ_dequeueBest()) != NULL){
		//Search for the smallest occurrence list in clause c.
		++main_litStamp;
		for (num = 0; num < GET_CLS_SIZE(c); ++num){
			SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,num), main_litStamp);
			if (GET_LIT_NUM_NUMOCCS_USED(GET_CLS_LITNUM(c,num)) < occSize){
				l = main_litData + GET_CLS_LITNUM(c,num);
				occSize = GET_LIT_NUMOCCS_USED(l);
			}
		}
		//Literal l is now the literal with smallest occurrence list. We now walk through this list and disable/delete all
		//clauses that clause c subsumes (ignoring c itself of course).
		for (num = 0; num < GET_LIT_NUMOCCS_USED(l); ++num){
			d = GET_LIT_OCCNUM(l, num);
			//In order to fast rule out some clauses that cannot be subsumed with c, we check the size and signature of d.
			if (c == d || (GET_CLS_SIZE(c) > GET_CLS_SIZE(d)) || (GET_CLS_SIG(c) & ~GET_CLS_SIG(d)) != 0ULL ){
				//Either c is larger than d or the signature of c contains something not found in d. Either case: no subsumption.
				continue;
			}
			//So far, none of the checks ruled out that c could subsume d. We now count the literals in d that are found in c.
			//If this count equals the number of literals we have in c we have found all literals in c in clause d.
			counted = 0;
			for (numD = 0; numD < GET_CLS_SIZE(d); ++numD){
				if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d, numD)) == main_litStamp){
					if (++counted == GET_CLS_SIZE(c)) break;
				}
			}

			//Check if the count equals the size of c.
			if (counted != GET_CLS_SIZE(c)){
				//No, at least one literal was not found.
				continue;
			}

			//We did not abort early which means that all the literals in c where found in d as well. Subsumption.
			//We must now distinct between two different cases.
			if ((IS_CLS_LEARNED(c) && !IS_CLS_LEARNED(d))){
				#ifdef VERBOSE_IN
				remLits += (GET_CLS_SIZE(d) - GET_CLS_SIZE(c));
				#endif
				//A learned clause is able to subsume an original clause. We align the original clause to be equal to learned
				//clause that is able to subsume it. Afterwards, we drop the learned clause c, add d to the clause stack for
				//further investigations and return. We disable the original clause temporarily, remove all the literals that
				//are not stamped, and enable the clause again.
				inp_intern_disableClause(d);
				//Remove literals not stamped.
				for (numD = 0; numD < GET_CLS_SIZE(d); ++numD){
					//If the variable is not stamped, we do not want to keep it in this clause. We override it with the last.
					if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d, numD)) != main_litStamp){
						SET_CLS_SIZE_DEC(d);
						SET_CLS_LITNUM(d, numD, GET_CLS_LITNUM(d, GET_CLS_SIZE(d)));
						--numD;
					}
				}
				//We update the clause data: representative and watchers.
				SET_CLS_SIG(d, GET_CLS_SIG(c));
				SET_CLS_W1(d, GET_CLS_W1(c));
				SET_CLS_W2(d, GET_CLS_W2(c));
				SET_CLS_REPLIT(d, GET_CLS_REPLIT(c));
				inp_intern_enableClause(d);
				inp_intern_abandonClause(c);
				break;
			} else {
				inp_intern_abandonClause(d);
			}
			//Since we removed the occurrence of literal l, we must decrease the counter.
			--num;
		}
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_el_used + f.m_eo_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_extern_STR(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//Performs STR with all the clauses in the clause queue.
	clause *c, *d, *conflict = NULL;
	literal *lOpp;
	variable *v;
	uint64_t sig, sigNew, litSig;
	int32_t lit, litID, impLitA, impLitB;
	uint32_t num, occNum, numC, numD, occLitPos, litPos, found, unitFound;

	#ifdef VERBOSE_IN
	printf("STR");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_el_used + f.m_eo_used;
	#endif

	while ((c = cQ_dequeueBest()) != NULL){
		//For each literal in clause c.
		unitFound = 0;
		for (num = 0; num < GET_CLS_SIZE(c); ++num){
			//Grab the opposite literal.
			lit = GET_CLS_LITNUM(c, num);
			litSig = GET_LIT_SIG(lit);
			lOpp = main_litData - lit;
			//We create a signature that does not contain the literal we resolve on.
			sig = GET_CLS_SIG(c);
			if (GET_CLS_SIZE(c) == POPCOUNT_UINT64(sig)){
				//Since each literal has its own bit in the signature, we can kill the bit of resLit.
				sig &= ~litSig;
			} else {
				//We must find out the hard way if there is a literal in c different from resLit that has the same signature.
				found = 0;
				for (numC = 0; numC < GET_CLS_SIZE(c); ++numC){
					if (GET_CLS_LITNUM(c,numC) != lit && GET_LIT_SIG(GET_CLS_LITNUM(c,numC)) == litSig){
						++found;
						break;
					}
				}
				if (!found){
					//We have not found another literal that causes the same signature as lit. We can kill the bit.
					sig &= ~litSig;
				}
			}
			//For all occurrences of the opposite literal.
			for (occNum = 0; occNum < GET_LIT_NUMOCCS_USED(lOpp); ++occNum){
				//Grab the occurrence of the opposite literal to get a possible resolution partner.
				d = GET_LIT_OCCNUM(lOpp, occNum);
				//Check that c is not too large and that the signature of c does not contain anything not in d.
				if (GET_CLS_SIZE(c) > GET_CLS_SIZE(d) ||(sig & ~GET_CLS_SIG(d))!=0ULL){
					continue;
				}
				//The simple checks did not rule out clause d. We must check if all the literals from c if they are found in d.
				found = 0;
				for (numC = 0; numC < GET_CLS_SIZE(c); ++numC){
					if (GET_CLS_LITNUM(c,numC) == lit) {
						continue;//We ignore the literal we resolve on.
					}
					found = 0;
					for (numD = 0; numD < GET_CLS_SIZE(d); ++numD){
						if (GET_CLS_LITNUM(c,numC) == -GET_CLS_LITNUM(d,numD)){
							//The literal from c, which is not the literal to resolve on, has also been found in d with
							//opposite sign. This strengthening would end in a tautology.
							found = 0; break;
						} else if (GET_CLS_LITNUM(c,numC) == GET_CLS_LITNUM(d,numD)){
							//We have found the literal.
							++found;
							break;
						}
					}
					if (!found){
						break;
					}
				}
				if (!found) continue;
				//Here a strengthening operation is possible by removing -lit from d.
				//We check what the result will be -- especially if d is of size two, we get a unit from it.
				if (GET_CLS_SIZE(d) == 2){
					//Yes, the result will be a unit. We first pick the variable to propagate (being not -lit).
					if (GET_CLS_LITNUM(d,0) == -lit){
						litID = GET_CLS_LITNUM(d,1);
					} else {
						litID = GET_CLS_LITNUM(d,0);
					}
					v = main_varData + abs(litID);
					inp_intern_backJump(0,0);
					MAIN_COMPLEXUP_ADD_DEC(litID,v);
					conflict = inp_intern_upPropagate();
					unitFound = 1;
					break;
				} else {
					//The result will not be unit.
					//First, we remove the clause from the occurrence list of -lit.
					occLitPos = GET_LIT_OCCLITPOSNUM(lOpp, occNum);
					REM_CLS_FROM_LIT_OCCS(-lit,occNum);
					//We must now remove -lit at position occLitPos in clause c.
					REM_CLS_LIT_NUM_AT_POS(d, occLitPos);
					//If the literal was the representative, we must replace it.
					if (GET_CLS_REPLIT(d) == -lit){
						SET_CLS_REPLIT(d, GET_CLS_LITNUM(d,0));
					}

					#ifdef VERBOSE_IN
					++remLits;
					#endif
					//The clause just lost its literal.
				}

				//Handle the clause according to its new size.
				if (GET_CLS_SIZE(d) == 2){
					//Yes, the clause is now binary.
					litID = GET_CLS_W1(d);
					litPos = GET_CLS_WHEREW1(d);
					REM_CLS_FROM_LIT_W1LIST(litID, litPos);
					litID = GET_CLS_W2(d);
					litPos = GET_CLS_WHEREW2(d);
					REM_CLS_FROM_LIT_W2LIST(litID, litPos);
					ADD_LIT_IMPLICATIONS(d, impLitA , impLitB );
				} else {
					//The clause is larger than binary. We check if one of the watchers is the removed literal. If so, we
					//need to find a replacement.
					if (GET_CLS_W1(d) == -lit){
						//Yes, we killed the first watcher and must replace it with something not the second watcher.
						litID = GET_CLS_W1(d);
						litPos = GET_CLS_WHEREW1(d);
						REM_CLS_FROM_LIT_W1LIST(litID, litPos);
						//Search for a literal in the clause not yet assigned and not the second watcher.
						for (litPos = 0; litPos < GET_CLS_SIZE(d); ++litPos){
							litID = GET_CLS_LITNUM(d,litPos);
							if (litID == GET_CLS_W2(d) || IS_LIT_ASSIGNED(litID)) continue;
							//The litID literal is now a replacement for the old first watcher.
							SET_CLS_W1(d, litID);
							ADD_CLS_TO_LIT_W1LIST(d, litID);
							break;
						}
					} else if (GET_CLS_W2(d) == -lit){
						//Yes, we killed the second watcher and must replace it with something not the first watcher.
						litID = GET_CLS_W2(d);
						litPos = GET_CLS_WHEREW2(d);
						REM_CLS_FROM_LIT_W2LIST(litID, litPos);
						//Search for a literal in the clause not yet assigned and not the second watcher.
						for (litPos = 0; litPos < GET_CLS_SIZE(d); ++litPos){
							litID = GET_CLS_LITNUM(d,litPos);
							if (litID == GET_CLS_W1(d) || IS_LIT_ASSIGNED(litID)) continue;
							//The litID literal is now a replacement for the old second watcher.
							SET_CLS_W2(d, litID);
							ADD_CLS_TO_LIT_W2LIST(d, litID);
							break;
						}
					}
				}
				//We have removed the literal occurrence of -lit. We must now update the signature of the reduced clause.
				if (POPCOUNT_UINT64(GET_CLS_SIG(d)) == GET_CLS_SIZE(d)+1){
					sigNew = GET_CLS_SIG(d) & ~GET_LIT_SIG(-lit);
				} else {
					sigNew = 0ULL;
					for (litPos = 0; litPos < GET_CLS_SIZE(d); ++litPos){
						sigNew |= GET_LIT_SIG(GET_CLS_LITNUM(d,litPos));
					}
				}
				SET_CLS_SIG(d, sigNew);
				inp_intern_QSUpdateLiteralDeletion(d, -lit);
				--occNum;
			}
			if (unitFound) break;
		}
		if (conflict != NULL) break;
	}

	if (conflict != NULL){
		in_returnCode = IN_UNSAT;
	} else if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_el_used + f.m_eo_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_extern_PLE(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//This method performs pure literal elimination. First, we back-jump to decision level 0 in APPEND mode. All assignments
	//that we will now identify will be propagated at the end of this method. This can, of course, not end in a conflict.
	variable *v;
	clause *conflict;
	literal *lPos, *lNeg;

	#ifdef VERBOSE_IN
	printf("PLE");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_el_used + f.m_eo_used;
	#endif

	//First, we back-jump to decision level 0. This also updates the UP stamp.
	inp_intern_backJump(0,0);

	//We now check for all the variables that are still enabled (not assigned), if they are pure. If so, we will put them on
	//the stack. Since PLE cannot end in a conflict, we do not have to worry about this.
	while ((v = vS_pop()) != NULL){
		//Grab the next variable. If this variable has no occurrences to the positive or negative side, then propagate the
		//respectively other side.
		if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v)) continue;
		lPos = main_litData + GET_VAR_ID(v);
		lNeg = main_litData - GET_VAR_ID(v);
		//We first check, if the positive occurrences are empty.
		if (GET_LIT_NUMOCCS_USED(lPos) == 0U){
			//The positive side has no occurrences, we propagate the negative side.
			MAIN_COMPLEXUP_ADD_DEC_PTR(lNeg, v);
		} else if (GET_LIT_NUMOCCS_USED(lNeg) == 0U){
			//The negative side has no occurrences, we propagate the positive side.
			MAIN_COMPLEXUP_ADD_DEC_PTR(lPos, v);
		}
	}

	//After collecting variables (or not), we call for UP to assign the variables and terminate DL0 again.
	conflict = inp_intern_upPropagate();
	if (conflict != NULL){
		in_returnCode = IN_UNSAT;
	} else if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_el_used + f.m_eo_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

clause* in_intern_fld_testPropagate(literal *l){
	//This method performs a test-wise unit propagation of literal l. We assume that the current DL is empty and use it for
	//the test propagations.
	variable *v;
	clause *c;
	literal **tempUPcurrent = main_complexUP_current;
	literal **tempUPend = main_complexUP_end;
	int32_t lid, lit, i, offset, litPos;

	++main_upStamp;

	//Add our test-literal to the stack.
	*(tempUPend++) = l;
	SET_LIT_UPSTAMP(l, main_upStamp);

	while (tempUPcurrent < tempUPend){
		//We grab the next literal from the stack and assign it accordingly.
		l = *(tempUPcurrent++);
		lid = GET_LIT_NUM_FROM_PTR(l);
		if (lid < 0){
			//The literal is negative. The variable must be assigned to 0.
			v = main_varData - lid;
			SET_VAR_ASS_0(v);
		} else {
			//The literal is positive. The variable must be assigned to 1.
			v = main_varData + lid;
			SET_VAR_ASS_1(v);
		}

		//We check all the implications of this literal.
		for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
			lit = GET_LIT_IMPNUM(l, i);
			v = main_varData + abs(lit);
			if (IS_VAR_UNASSIGNED(v)){
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					c = GET_LIT_IMPNUM_REASON(l, i);
					//Clean-up.
					while (tempUPcurrent > main_complexUP_current){
						l = *(--tempUPcurrent);
						lid = GET_LIT_NUM_FROM_PTR(l);
						if (lid < 0){
							v = main_varData - lid;
						} else {
							v = main_varData + lid;
						}
						SET_VAR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++main_upStamp;
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_LIT_NUM_UPSTAMP(lit, main_upStamp);
					*(tempUPend++) = ( main_litData + lit );
					//No need to propagate a literal more than once.
					lQ_delete(main_litData + lit);
				} //Else, the literal is already enqueued.
			}
		}

		//We check the watcher lists of the falsified literal.
		l = main_litData - lid;

		//WL1
		offset = 0;
		while (GET_LIT_NUMW1_USED(l) > offset){
			c = GET_LIT_W1_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit) || IS_LIT_SAT(GET_CLS_W2(c))){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CLS_WHEREW1(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CLS_SIZE(c); ++i){
				lit = GET_CLS_LITNUM(c,i);
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the second watcher.
				if (lit == GET_CLS_W2(c)) continue;
				//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
				v = main_varData + abs(lit);
				if (IS_VAR_UNASSIGNED(v)){
					//We have found an unassigned literal that can serve as watcher.
					SET_CLS_W1(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CLS_W1(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W1LIST(-lid, litPos);
				ADD_CLS_TO_LIT_W1LIST(c, lit);
			} else {
				//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CLS_W2(c);
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					//Clean-up.
					while (tempUPcurrent > main_complexUP_current){
						l = *(--tempUPcurrent);
						lid = GET_LIT_NUM_FROM_PTR(l);
						if (lid < 0){
							v = main_varData - lid;
						} else {
							v = main_varData + lid;
						}
						SET_VAR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++main_upStamp;
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_LIT_NUM_UPSTAMP(lit, main_upStamp);
					*(tempUPend++) = ( main_litData + lit );
				} //Else, the literal is already on the stack.
			}
		}

		//WL2
		offset = 0;
		while (GET_LIT_NUMW2_USED(l) > offset){
			c = GET_LIT_W2_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit) || IS_LIT_SAT(GET_CLS_W1(c))){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
			}
			//We retain the information at what position the watcher is found.
			litPos = GET_CLS_WHEREW2(c);
			//We now try to find out if we can replace the representative by some satisfied literal in the clause.
			for (i = 0; i < GET_CLS_SIZE(c); ++i){
				lit = GET_CLS_LITNUM(c,i);
				if (IS_LIT_SAT(lit)){
					//We have found a literal in the clause that satisfies the clause. This should have been the representative
					//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
					SET_CLS_REPLIT(c, lit);
					++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
					break;
				}
				//We ignore the literal if it is the first watcher.
				if (lit == GET_CLS_W1(c)) continue;
				//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
				v = main_varData + abs(lit);
				if (IS_VAR_UNASSIGNED(v)){
					//We have found an unassigned literal that can serve as watcher.
					SET_CLS_W2(c, lit);
					break;
				}
			}
			//Check if the representative literal replacement was successful. If so, we are done with this clause.
			if (i<0) continue;
			//Check if we found a new watcher.
			if (GET_CLS_W2(c) != -lid){
				//Yes, we can perform the replacement.
				REM_CLS_FROM_LIT_W2LIST(-lid, litPos);
				ADD_CLS_TO_LIT_W2LIST(c, lit);
			} else {
				//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
				++offset;
				lit = GET_CLS_W1(c);
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					//Clean-up.
					while (tempUPcurrent > main_complexUP_current){
						l = *(--tempUPcurrent);
						lid = GET_LIT_NUM_FROM_PTR(l);
						if (lid < 0){
							v = main_varData - lid;
						} else {
							v = main_varData + lid;
						}
						SET_VAR_ASS_2(v);
					}
					*tempUPcurrent = NULL;
					*(tempUPcurrent+1) = NULL;
					++main_upStamp;
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					SET_LIT_NUM_UPSTAMP(lit, main_upStamp);
					*(tempUPend++) = ( main_litData + lit );
				} //Else, the literal is already on the stack.
			}
		}

	}

	//We must undo all assignments in this test-propagation.
	//Clean-up.
	while (tempUPcurrent > main_complexUP_current){
		l = *(--tempUPcurrent);
		lid = GET_LIT_NUM_FROM_PTR(l);
		if (lid < 0){
			v = main_varData - lid;
		} else {
			v = main_varData + lid;
		}
		SET_VAR_ASS_2(v);
	}
	*tempUPcurrent = NULL;
	*(tempUPcurrent+1) = NULL;
	++main_upStamp;
	return NULL;
}

void in_extern_FLD(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//This method performs failed literal detection. For each of the enabled variables we check the literal if it has
	//implications. If so, we assign such that the implications are triggered in a test propagation. If this results in a
	//conflict, we must finally propagate the opposite. This can run into a conflict and if that happens, we must return
	//the conflict clause.

	variable *v;
	literal *l;
	clause *conflict = NULL;

	#ifdef VERBOSE_IN
	printf("FLD");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_el_used + f.m_eo_used;
	#endif

	while ((l = lQ_dequeueBest()) != NULL){
		//Yes, we can test propagate this literal.
		if (GET_LIT_NUMIMP_USED(l) == 0 || IS_VAR_DISABLED((main_varData+abs(GET_LIT_NUM_FROM_PTR(l))))) continue;
		conflict = in_intern_fld_testPropagate(l);
		if (conflict != NULL){
			//Yes, the opposite MUST be enforced. We back-jump to DL0 in APPEND mode, append the literal and propagate.
			inp_intern_backJump(0,0);
			l = GET_LIT_OPPOSITE(l);
			v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
			MAIN_COMPLEXUP_ADD_DEC_PTR(l, v);
			conflict = inp_intern_upPropagate();
			//Check if this resulted in a conflict.
			if (conflict != NULL){
				//Yes, the enforced assignment resulted in a conflict. We can stop.
				break;
			}
		}

	}
	if (conflict != NULL){
		in_returnCode = IN_UNSAT;
	} else if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_el_used + f.m_eo_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_extern_TERNARY(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	#ifdef VERBOSE_IN
	printf("TER");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_el_used + f.m_eo_used;
	#endif

	clause *c, *d;
	literal *l, *lOpp, *lNew;
	variable *v;
	uint64_t sig;
	uint32_t lOccNum, lOppOccNum, sameCount, oppCount, numLearnt, numLearntTotal = 0;
	int32_t lit1, lit2, lit3, imp;

	//We pick a literal that we want to do ternary resolution with.
	while ((v = vS_pop()) != NULL){
		//We pick the outer literal from the variable such that it has a larger occurrence list.
		if (GET_LIT_NUM_NUMOCCS_USED(GET_VAR_ID(v)) > GET_LIT_NUM_NUMOCCS_USED(-GET_VAR_ID(v))){
			l = main_litData + GET_VAR_ID(v);
		} else {
			l = main_litData - GET_VAR_ID(v);
		}
		numLearnt = 0;
		lOpp = GET_LIT_OPPOSITE(l);
		//We ignore pure literals.
		if (IS_VAR_DISABLED(v) || GET_LIT_NUMOCCS_USED(l) == 0 || GET_LIT_NUMOCCS_USED(lOpp) == 0) continue;

		//We walk through all the occurrences of l and search for a 3-clause.
		for (lOccNum = 0; lOccNum < GET_LIT_NUMOCCS_USED(l); ++lOccNum){
			c = GET_LIT_OCCNUM(l, lOccNum);
			if (GET_CLS_SIZE(c) != 3) continue;
			//We have now found a 3-clause in c that we can try ternary resolution with. We need to locate a clause d that
			//contains lOpp as a possible resolution partner.
			for (lOppOccNum = 0; lOppOccNum < GET_LIT_NUMOCCS_USED(lOpp); ++lOppOccNum){
				d = GET_LIT_OCCNUM(lOpp, lOppOccNum);
				if (GET_CLS_SIZE(d) != 3) continue;
				//We have now found a 3-clause in d that we can resolve with c.
				//When checking the signature, a zero means no clashing literal classes -> no common literal.
				if ((GET_CLS_SIG(c) & GET_CLS_SIG(d)) == 0ULL) continue;
				//It seems that we can find a common literal between c and d. We check how many that are the hard way.
				++main_litStamp;
				sameCount = 0;
				oppCount = 0;
				SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0), main_litStamp);
				SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1), main_litStamp);
				SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2), main_litStamp);
				if (GET_LIT_NUM_STAMP( GET_CLS_LITNUM(d,0)) == main_litStamp) ++sameCount;
				if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,0)) == main_litStamp) ++oppCount;
				if (GET_LIT_NUM_STAMP( GET_CLS_LITNUM(d,1)) == main_litStamp) ++sameCount;
				if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,1)) == main_litStamp) ++oppCount;
				if (GET_LIT_NUM_STAMP( GET_CLS_LITNUM(d,2)) == main_litStamp) ++sameCount;
				if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,2)) == main_litStamp) ++oppCount;

				//Now, if sameCount is not equal 1 or oppCount is larger 1, we do not resolve. Either they have no
				//clashing literal as initially assumed, or they have more than the resolvent literal with opposite sign in
				//common. In any case, the result would be no ternary clause.
				if (sameCount != 1 || oppCount > 1) continue;

				//If we reach this position, we can resolve. We grab from clause d the literal that has the same stamp and
				//the literal that is not stamped at all. From clause c we grab the literal that is not literal 1 or 2 and
				//not the literal we resolve on.
				if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,0)) == main_litStamp){
					lit1 = GET_CLS_LITNUM(d,0);
				} else if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,1)) == main_litStamp){
					lit1 = GET_CLS_LITNUM(d,1);
				} else {
					lit1 = GET_CLS_LITNUM(d,2);
				}

				if (       GET_LIT_NUM_STAMP( GET_CLS_LITNUM(d,0)) != main_litStamp
						&& GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,0)) != main_litStamp){
					lit2 = GET_CLS_LITNUM(d,0);
				} else if (GET_LIT_NUM_STAMP( GET_CLS_LITNUM(d,1)) != main_litStamp
						&& GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,1)) != main_litStamp){
					lit2 = GET_CLS_LITNUM(d,1);
				} else {
					lit2 = GET_CLS_LITNUM(d,2);
				}

				if (GET_CLS_LITNUM(c,0) != lit1 && GET_CLS_LITNUM(c,0) != lit2 && main_litData + GET_CLS_LITNUM(c,0) != l){
					lit3 = GET_CLS_LITNUM(c,0);
				} else if (GET_CLS_LITNUM(c,1) != lit1 && GET_CLS_LITNUM(c,1) != lit2 && main_litData + GET_CLS_LITNUM(c,1) != l){
					lit3 = GET_CLS_LITNUM(c,1);
				} else {
					lit3 = GET_CLS_LITNUM(c,2);
				}

				//Now li1, lit2, and lit3 can form a 3-clause. We must find out if there is any other 3-clause equivalent
				//to the one we could now learn. If so, this would result in a subsumption and nothing would be gained. The
				//subsumption test will work via the signature of the new clause and the literal it contains that has the
				//smallest occurrence list.
				++main_litStamp;
				SET_LIT_NUM_STAMP(lit1, main_litStamp);
				SET_LIT_NUM_STAMP(lit2, main_litStamp);
				SET_LIT_NUM_STAMP(lit3, main_litStamp);
				sig = 0ULL;
				sig |= GET_LIT_SIG(lit1);
				lNew = main_litData + lit1;
				sig |= GET_LIT_SIG(lit2);
				if (GET_LIT_NUM_NUMOCCS_USED(lit2) < GET_LIT_NUMOCCS_USED(lNew)) lNew = main_litData + lit2;
				sig |= GET_LIT_SIG(lit3);
				if (GET_LIT_NUM_NUMOCCS_USED(lit3) < GET_LIT_NUMOCCS_USED(lNew)) lNew = main_litData + lit3;
				//Literal lNew is now the literal that has the smallest occurrence list. We check all the clauses in here if
				//any of them matches the signature.
				for (sameCount = 0; sameCount < GET_LIT_NUMOCCS_USED(lNew); ++sameCount){
					d = GET_LIT_OCCNUM(lNew, sameCount);
					if (GET_CLS_SIZE(d) != 3 || sig != GET_CLS_SIG(d)) continue;
					//Here, it might be that clause d is equal to what we can learn. We check if d contains lit1, lit2
					//and lit3 via looking at the stamps.
					if (       GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,0)) == main_litStamp
							&& GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,1)) == main_litStamp
							&& GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,2)) == main_litStamp){
						//All three literals match. We cannot learn the new clause as it is equal to d.
						break;
					}
				}
				if (sameCount < GET_LIT_NUMOCCS_USED(lNew)) {
					continue;
				}

				//We must now find out if the new clause can be subsumed by any two clause, if so, this would result in a
				//subsumption and nothing would be gained. It suffices to check the implication lists of two of the opposite
				//literals in the clause. If any of them contains one of our literals, we can stop.
				lNew = main_litData - lit1;
				for (sameCount = 0; sameCount < GET_LIT_NUMIMP_USED(lNew); sameCount += 2){
					imp = GET_LIT_IMPNUM(lNew, sameCount);
					if (GET_LIT_NUM_STAMP(imp) == main_litStamp){
						break;
					}
				}
				if (sameCount < GET_LIT_NUMIMP_USED(lNew)){
					continue;
				}
				lNew = main_litData - lit2;
				for (sameCount = 0; sameCount < GET_LIT_NUMIMP_USED(lNew); sameCount += 2){
					imp = GET_LIT_IMPNUM(lNew, sameCount);
					if (GET_LIT_NUM_STAMP(imp) == main_litStamp){
						break;
					}
				}
				if (sameCount < GET_LIT_NUMIMP_USED(lNew)){
					continue;
				}
				//Here at this point, the clause we can learn passed all the tests. We add it.
				inp_intern_addLearnedTernaryClause(lit1, lit2, lit3);

				if (++numLearnt == param_inpTernaryLitMax) break;
			}
			if (numLearnt == param_inpTernaryLitMax) break;
		}
		numLearntTotal += numLearnt;
		if (numLearntTotal >= param_inpTernaryMax) break;
	}
	vS_flush();

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_el_used + f.m_eo_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

int32_t in_intern_unhiding_SSet_incDSC(const void* a, const void* b){
	//Sorts the literals with increasing discovery time. We swap if a has a larger discovery time.
	if ( GET_LIT_DSC((*(literal**)a)) > GET_LIT_DSC((*(literal**)b)) ) return 1;
	return -1;
}

int32_t in_intern_unhiding_SSet_decDSC(const void* a, const void* b){
	//Sorts the literals with decreasing discovery time. We swap if a has a smaller discovery time.
	if ( GET_LIT_DSC((*(literal**)a)) < GET_LIT_DSC((*(literal**)b)) ) return 1;
	return -1;
}

void in_intern_unhiding_uhle(clause *c){
	//This performs an UHLE check on the given clause (which must be disabled). We begin by creating the SPos array in
	//reversed order.
	literal *l;
	uint64_t sig = 0ULL;
	uint32_t finished, i, oldSize = GET_CLS_SIZE(c), maxSize = GET_CLS_SIZE(c), writePos;
	int32_t litID;
	for (i = 0; i < maxSize; ++i){
		l = main_litData + GET_CLS_LITNUM(c, i);
		inp_unhiding_SPlus[i] = l;
	}
	//We now sort the SPlus array to hold the literals with decreasing discovery time.
	qsort(inp_unhiding_SPlus, maxSize, sizeof(literal*), in_intern_unhiding_SSet_decDSC);
	finished = GET_LIT_FIN(inp_unhiding_SPlus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = inp_unhiding_SPlus[i];
		if (GET_LIT_FIN(l) > finished){
			//We do not keep the element in SPlus at position i. We just go on with the next literal in SPos.
		} else {
			//We want to keep l. We write it at the current writePos and go on.
			inp_unhiding_SPlus[writePos++] = l;
			finished = GET_LIT_FIN(l);
		}
	}
	//We update the maxSize with the elements written (number of retained elements).
	maxSize = writePos;
	//We now create SMinus with all the retained elements, but invert their sign. We sort with increasing discovery time.
	for(i = 0; i < maxSize; ++i){
		inp_unhiding_SMinus[i] = GET_LIT_OPPOSITE(inp_unhiding_SPlus[i]);
	}
	qsort(inp_unhiding_SMinus, maxSize, sizeof(literal*), in_intern_unhiding_SSet_incDSC);
	finished = GET_LIT_FIN(inp_unhiding_SMinus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = inp_unhiding_SMinus[i];
		if (GET_LIT_FIN(l) < finished){
			//We do not keep the element in SMinus at position i. We just go on with the next literal in SMinus.
		} else {
			//We want to keep l. Write it at the current writePos and go on.
			inp_unhiding_SMinus[writePos++] = l;
			finished = GET_LIT_FIN(l);
		}
	}
	//The amounts of literals written is now the new size of the clause. We check if we did indeed remove something from c.
	maxSize = writePos;
	if (oldSize > maxSize){
		//Yes, a removal happened. We will now copy all the retained elements from SMinus with opposite sign to the clause.
		//We update the signature of the clause and its size and watcher information.
		for(i = 0; i < maxSize; ++i){
			l = GET_LIT_OPPOSITE(inp_unhiding_SMinus[i]);
			litID = GET_LIT_NUM_FROM_PTR(l);
			sig |= GET_LIT_SIG(litID);
			SET_CLS_LITNUM(c, i, litID);
		}
		SET_CLS_LITNUM(c, i, 0);
		SET_CLS_SIZE(c, i);
		SET_CLS_SIG(c, sig);
		SET_CLS_W1(c, GET_CLS_LITNUM(c, 0));
		SET_CLS_WHEREW1(c, 0);
		SET_CLS_W2(c, GET_CLS_LITNUM(c, 1));
		SET_CLS_WHEREW2(c, 0);
		SET_CLS_REPLIT(c, GET_CLS_LITNUM(c, 0));
	}
	//We have now reduced the clause as much as possible.
}

uint32_t in_intern_unhiding_uhte(clause *c){
	//This performs an UHTE check on the given clause (which must be disabled). This method returns 1 if the clause is a HT.
	//We first create the SPlus and SMinus sets such that they hold the respective literals or opposite literals in increasing
	//discovery time order.
	literal *lPos, *lNeg;
	uint32_t i, lastEltPos = GET_CLS_SIZE(c) - 1, currentEltPosPlus, currentEltPosMinus;
	//We create SPlus first by copying all the literals from c with the same sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		lPos = main_litData + GET_CLS_LITNUM(c, i);
		inp_unhiding_SPlus[i] = lPos;
	}
	qsort(inp_unhiding_SPlus, GET_CLS_SIZE(c), sizeof(literal*), in_intern_unhiding_SSet_incDSC);
	lPos = inp_unhiding_SPlus[0];
	//We then create SMinus by copying all the literals form c with the opposite sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		lNeg = main_litData - GET_CLS_LITNUM(c, i);
		inp_unhiding_SMinus[i] = lNeg;
	}
	qsort(inp_unhiding_SMinus, GET_CLS_SIZE(c), sizeof(literal*), in_intern_unhiding_SSet_incDSC);
	lNeg = inp_unhiding_SMinus[0];

	currentEltPosPlus = 0;
	currentEltPosMinus = 0;
	while(1){
		if (GET_LIT_DSC(lNeg) > GET_LIT_DSC(lPos)){
			if (currentEltPosPlus == lastEltPos) return 0;
			lPos = inp_unhiding_SPlus[++currentEltPosPlus];
		} else if ( (GET_LIT_FIN(lNeg) < GET_LIT_FIN(lPos))
				//|| ( GET_CLS_SIZE(c) == 2 && (lPos == GET_LIT_OPPOSITE(lNeg) || GET_LIT_PRT(lPos) == lNeg) )
				//The above check on clauses with size two is unnecessary, as transitive edges are removed on the fly
				//by the advanced stamping scheme.
					){
			if (currentEltPosMinus == lastEltPos) return 0;
			lNeg = inp_unhiding_SMinus[++currentEltPosMinus];
		} else {
			return 1;
		}
	}

	return 0;
}

uint32_t in_intern_unhiding_stamp(literal *l, uint32_t stamp){
	//This method implements the advanced stamping procedure.
	literal *impLit, *impLitOpp, *failedLit;
	clause *c;
	variable *v;
	uint32_t flagSCC = 1, impNum, i;	//The flag captures if l represents a strongly connected component (SCC).

	++stamp;
	SET_LIT_DSC(l, stamp);
	SET_LIT_OBS(l, stamp);
	//Push the literal to the literal stack.
	*(inp_unhiding_lStack_current++) = l;
	//We now check all binary clauses in which -l occurs. This is already captured in the implication list of l.
	for (impNum = 0; impNum < GET_LIT_NUMIMP_USED(l); impNum += 2){
		impLit = main_litData + GET_LIT_IMPNUM(l, impNum);
		c = GET_LIT_IMPNUM_REASON(l, impNum);
		//First, we check if we can remove this implication.
		if (GET_LIT_DSC(l) < GET_LIT_OBS(impLit)) {
			//The binary clause can be removed (transitive reduction). This will also remove the implication from the list
			//of l, we therefore must reduce the impNum counter by 2 to stay at this specific position in the list.
			inp_intern_deleteClause(c);
			impNum -= 2;
			continue;
		}
		//Next, we perform a failed literal detection.
		impLitOpp = GET_LIT_OPPOSITE(impLit);
		if (GET_LIT_DSC(GET_LIT_ROOT(l)) <= GET_LIT_OBS(impLitOpp)){
			failedLit = l;
			while (GET_LIT_DSC(failedLit) > GET_LIT_OBS(impLitOpp)){
				failedLit = GET_LIT_PRT(failedLit);
			}
			//We check if we can add the opposite of the failed literal to the UP queue.
			failedLit = GET_LIT_OPPOSITE(failedLit);
			if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S_PTR(failedLit)){
				//This constitutes a global conflict as we found a failed literal for which the opposite is also failed.
				#ifdef VERBOSE_IN
				printf("UNH-EQ-CONFf ");
				#endif
				in_returnCode = IN_UNSAT;
				return 0;
			} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S_PTR(failedLit)){
				//We can add the literal since it is not already enqueued.
				v = main_varData + abs(GET_LIT_NUM_FROM_PTR(failedLit));
				MAIN_COMPLEXUP_ADD_DEC_PTR(failedLit,v);
			}//Else the literal is already enqueued.
			if (GET_LIT_DSC(impLitOpp) != 0 && GET_LIT_FIN(impLitOpp) == 0) continue;
		}
		//Next, we perform the basic stamping operations.
		if (GET_LIT_DSC(impLit) == 0){
			SET_LIT_PRT(impLit, l);
			SET_LIT_ROOT(impLit, GET_LIT_ROOT(l));
			//Recursive stamping for the implied literal.
			stamp = in_intern_unhiding_stamp(impLit, stamp);
			if (in_returnCode != IN_UNKNOWN){
				return 0;
			}
		}
		//Next, we check for literal equivalences for equivalence literal substitution.
		if (GET_LIT_FIN(impLit) == 0 && GET_LIT_DSC(impLit) < GET_LIT_DSC(l)){
			//Literal l is equivalent to literal impLit.
			SET_LIT_DSC(l, GET_LIT_DSC(impLit));
			flagSCC = 0;
		}
		SET_LIT_OBS(impLit, stamp);
	}
	//We now check if l represents an SCC for equivalent literal substitution.
	if (flagSCC){
		//Yes, l represents an SCC. All literals in the stack will receive the same stamp.
		++stamp;
		//Allocate new equivalence class if necessary.
		INP_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--inp_unhiding_lStack_current);
			SET_LIT_DSC(impLit, GET_LIT_DSC(l));
			SET_LIT_FIN(impLit, stamp);

			if (GET_LIT_STAMP(impLit) != main_litStamp){
				INP_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_LIT_STAMP(impLit, main_litStamp);
				SET_LIT_STAMP(GET_LIT_OPPOSITE(impLit), main_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (inp_els_classes_memUsed[inp_els_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++inp_els_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			inp_els_classes_memUsed[inp_els_classes_used] = 0;
		}
	}

	return stamp;
}

void in_extern_UNHIDING(uint32_t roundsToPerform){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	clause *c;
	literal *l, *lOpp, *classRep;
	variable *v;
	uint32_t stamp, random, numToPermute, rounds=0;
	int32_t i,j, oIgnore, lIgnore, reWID, toRID, same, opp;

	#ifdef VERBOSE_IN
	printf("UNH%d", roundsToPerform);fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	uint32_t oldsize;
	#endif

	do {
		//We prepare our decision stack by back-jumping to DL0 in APPEND mode. The advanced stamping and the units learned by
		//UHLE can then be put to the end of the DL0.
		inp_intern_backJump(0,0);
		inp_unhiding_lStack_current = inp_unhiding_lStack + 1;


		//We initialize the literals for the advanced stamping.
		for (i = 0; i < f.n_vars_e_used; ++i){
			v = main_varData + GET_VAR_ID(f.vars_e[i]);
			l = main_litData + GET_VAR_ID(v);
			SET_LIT_DSC(l, 0);
			SET_LIT_FIN(l, 0);
			SET_LIT_OBS(l, 0);
			SET_LIT_PRT(l, l);
			SET_LIT_ROOT(l, l);
			l = main_litData - GET_VAR_ID(v);
			SET_LIT_DSC(l, 0);
			SET_LIT_FIN(l, 0);
			SET_LIT_OBS(l, 0);
			SET_LIT_PRT(l, l);
			SET_LIT_ROOT(l, l);
		}

		//First, we initialize a new permutation of all the literals that remain.
		numToPermute = 2*f.n_vars_e_used;
		perm_LFSR_init(numToPermute);
		stamp = 0;

		//We prepare the literal stamp to present classes only once.
		++main_litStamp;
		//Initialize the classes information.
		inp_els_classes_used = 0;
		for (i = 0; i < inp_els_classes_avail; ++i){
			inp_els_classes_memUsed[i] = 0;
		}

		for (i = 0; i < perm_lfsr_perLen; ++i){
			random = perm_LFSR_nextState();
			if (random >= numToPermute) continue;
			if (random < f.n_vars_e_used){
				l = main_litData + GET_VAR_ID(f.vars_e[random]);
			} else {
				l = main_litData - GET_VAR_ID(f.vars_e[random - f.n_vars_e_used]);
			}
			inp_litPerm[stamp++] = l;
		}
		stamp = 0;

		//We then walk through the permutation to stamp all roots.
		for (i = 0; i < numToPermute; ++i){
			l = inp_litPerm[i];
			lOpp = GET_LIT_OPPOSITE(l);
			//Literal l is now the literal we want to stamp. If the opposite has no implications, l is a root.
			if (GET_LIT_NUMIMP_USED(lOpp) == 0U){
				//Since the opposite of l has no implications, l must be a root. We stamp it.
				stamp = in_intern_unhiding_stamp(l, stamp);
				if (in_returnCode != IN_UNKNOWN){
					#ifdef VERBOSE_IN
					printf(" RS ");
					#endif
					return;
				}
			}
		}

		//We stamp the remaining literals if they have not be discovered yet.
		for (i = 0; i < numToPermute; ++i){
			l = inp_litPerm[i];
			//If l was not yet discovered, we can stamp it.
			if (GET_LIT_DSC(l) == 0U){
				stamp = in_intern_unhiding_stamp(l, stamp);
				if (in_returnCode != IN_UNKNOWN){
					#ifdef VERBOSE_IN
					printf(" LS ");
					#endif
					return;
				}
			}
		}

		//The advanced stamping of all the literals in BIG is now over. It is now time to iterate through all the clauses of
		//the formula and perform UHTE or UHLE. The following is the implementation of the SIMPLIFY algorithm from the paper.
		//We first iterate through all the original clauses of the formula, and then through all the learned clauses.
		oIgnore = 0;
		lIgnore = 0;
		for (i = 0; i < f.m_eo_used + f.m_el_used; ++i){
			if (i < f.m_eo_used - oIgnore){
				c = f.clauses_eo[i];
			} else if (i < f.m_eo_used) {
				i = f.m_eo_used-1;
				continue;
			} else if (i - f.m_eo_used < f.m_el_used - lIgnore){
				c = f.clauses_el[i - f.m_eo_used];
			} else {
				break;
			}

			//Temporarily disable the clause.
			inp_intern_disableClause(c);
			//Check for UHTE -- Unhiding Hidden Tautology Elimination.
			if (GET_CLS_SIZE(c) > 2 && IS_CLS_LEARNED(c) && in_intern_unhiding_uhte(c)){
				//It is a hidden tautology. We get rid of the clause for good.
				inp_intern_abandonClause(c);
			} else {
				//Otherwise, it is no hidden tautology, so we cannot get rid of the clause. We will first perform UHLE and
				//check how far it was reduced. If it is no unit clause, we re-enable it. If it is unit, we try to put its
				//literal in the UP. If this fails, we have a global conflict. If this does not fail, we can abandon the
				//clause because we do not need units.
				#ifdef VERBOSE_IN
				oldsize = GET_CLS_SIZE(c);
				#endif
				in_intern_unhiding_uhle(c);
				if (GET_CLS_SIZE(c) == 1){
					//We have reduced it to a unit clause and must now check if we can put its literal in the UP.
					v = main_varData + abs(GET_CLS_LITNUM(c, 0));
					l = main_litData + GET_CLS_LITNUM(c, 0);
					if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S_PTR(l)){
						//This constitutes a global conflict as we found a unit for which the opposite is enforced.
						#ifdef VERBOSE_IN
						printf(" LE ");
						#endif
						in_returnCode = IN_UNSAT;
						return;
					} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S_PTR(l)){
						//We can add the literal since it is not already enqueued.
						MAIN_COMPLEXUP_ADD_DEC_PTR(l, v);
					}
					//Anyway, we can abandon the clause.
					inp_intern_abandonClause(c);
				} else {
					//We did not reduce it to a unit clause. However it went with the reduction for c, it is at least binary
					//and we must keep it. We therefore enable it again.
					inp_intern_enableClause(c);
					#ifdef VERBOSE_IN
					if (oldsize != GET_CLS_SIZE(c)){
						remLits += (oldsize - GET_CLS_SIZE(c));
					}
					#endif
					if (IS_CLS_LEARNED(c)){
						++lIgnore;
					} else {
						++oIgnore;
					}
				}
			}
		}

		//We now check the ELS classes and either replace all elements with the class representative or propagate all elements
		//in a class if one of them is marked for propagation. This might lead into a conflict.
		for (i = 0; i < inp_els_classes_used; ++i){
			//First, we check if any literal in the current class is already on the stack.
			same = 0;
			opp = 0;
			for (j = 0; j < inp_els_classes_memUsed[i]; ++j){
				classRep = inp_els_classes[i][j];
				if (MAIN_COMPLEXUP_IS_LIT_ON_S_PTR(classRep)){
					same = 1;
				} else if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S_PTR(classRep)){
					opp = 1;
				}
			}
			//Check, if any of the literals is marked for propagation.
			if (same && opp){
				//From the current class, there is at least one literal marked for propagation with same sign and one marked
				//for propagation with opposite sign. This constitutes a global conflict as all literals must be assigned
				//the same way since they are all equivalent.
				#ifdef VERBOSE_IN
				printf(" EQ ");
				#endif
				in_returnCode = IN_UNSAT;
				return;
			}
			if (same){
				//There is at least one literal that is marked for propagation with the same sign. We add all the other
				//literals with the same sign into the UP as well.
				for (j = 0; j < inp_els_classes_memUsed[i]; ++j){
					classRep = inp_els_classes[i][j];
					if (!MAIN_COMPLEXUP_IS_LIT_ON_S_PTR(classRep)){
						v = main_varData + abs(GET_LIT_NUM_FROM_PTR(classRep));
						MAIN_COMPLEXUP_ADD_DEC_PTR(classRep, v);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			} else if (opp){
				//There is at least one literal that is marked for propagation with the opposite sign. We add all the other
				//literals with the opposite sign into the UP as well.
				for (j = 0; j < inp_els_classes_memUsed[i]; ++j){
					classRep = inp_els_classes[i][j];
					if (!MAIN_COMPLEXUP_IS_OPPLIT_ON_S_PTR(classRep)){
						v = main_varData + abs(GET_LIT_NUM_FROM_PTR(classRep));
						classRep = GET_LIT_OPPOSITE(classRep);
						MAIN_COMPLEXUP_ADD_DEC_PTR(classRep, v);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			}
			//If we get this far, we must make a replacement. For class i, we pick the class representative (first literal).
			classRep = inp_els_classes[i][0];
			reWID = GET_LIT_NUM_FROM_PTR(classRep);
			//For all the remaining literals in the class, we replace their occurrence with the representative.
			for (j = 1; j < inp_els_classes_memUsed[i]; ++j){
				//Grab the literal and variable to replace.
				l = inp_els_classes[i][j];
				toRID = GET_LIT_NUM_FROM_PTR(l);
				v = main_varData + abs(toRID);

				//Replace the occurrence of l with classRep.
				in_intern_els_replaceLit(l, classRep);
				if (in_returnCode != IN_UNKNOWN){
					#ifdef VERBOSE_IN
					printf(" CR ");
					#endif
					return;
				}

				//Replace the occurrence of -l with -classRep.
				in_intern_els_replaceLit(GET_LIT_OPPOSITE(l),GET_LIT_OPPOSITE(classRep));
				if (in_returnCode != IN_UNKNOWN){
					#ifdef VERBOSE_IN
					printf(" CR ");
					#endif
					return;
				}

				//Mark the replaced variable with its representative.
				if ((toRID < 0 && reWID < 0) || (toRID > 0 && reWID > 0)){
					SET_VAR_ISEQUIVTO(v, abs(reWID));
				} else {
					SET_VAR_ISEQUIVTO(v, -abs(reWID));
				}

				//Disable the replaced variable.
				inp_intern_disableVariable(v);
			}
		}

		//We propagate every unit we found. If we did not find any unit, then this will again terminate DL0.
		c = inp_intern_upPropagate();
		if (c != NULL){
			#ifdef VERBOSE_IN
			printf(" CONFLICT ");
			#endif
			in_returnCode = IN_UNSAT;
			return;
		}
		if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
			in_returnCode = IN_SAT;
			break;
		}

	} while (++rounds < roundsToPerform);

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

clause* in_intern_nhbr_detect(literal *rootLit){
	//Performs unit propagation prioritizing binary clauses. It assumes that we are in DL1 and that DL1 is empty.
	//This method performs a test-wise unit propagation of literal l. We assume that the current DL is empty and use it for
	//the test propagations.
	variable *v;
	clause *c;
	literal *l;
	literal **checkFrom = main_complexUP_current;
	int32_t lid, lit, i, offset, litPos, learnBinary, numBinOccsForLearnBinary;

	++main_litStamp;

	//Add our test-literal to the stack.
	v = main_varData + abs(GET_LIT_NUM_FROM_PTR(rootLit));
	MAIN_COMPLEXUP_ADD_DEC_PTR(rootLit,v);
	SET_LIT_STAMP(rootLit, main_litStamp);

	do {
		while (main_complexUP_current < main_complexUP_end){
			//We grab the next literal from the stack and assign it accordingly.
			l = *(main_complexUP_current++);
			lid = GET_LIT_NUM_FROM_PTR(l);
			if (lid < 0){
				//The literal is negative. The variable must be assigned to 0.
				v = main_varData - lid;
				SET_VAR_ASS_0(v);
			} else {
				//The literal is positive. The variable must be assigned to 1.
				v = main_varData + lid;
				SET_VAR_ASS_1(v);
			}

			//We check all the implications of this literal.
			for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
				lit = GET_LIT_IMPNUM(l, i);
				v = main_varData + abs(lit);
				if (IS_VAR_UNASSIGNED(v)){
					//First, we check if the opposite literal has been stamped. If so, then there is at least one large clause
					//that needs the opposite assignment. This results in a conflict right away.
					if (GET_LIT_NUM_STAMP(-lit) == main_litStamp){
						//Yes, this is a conflict.
						return GET_LIT_IMPNUM_REASON(l, i);
					}
					if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
						//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
						return GET_LIT_IMPNUM_REASON(l, i);
					} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
						//The element is not yet on the stack. We put it there and stamp it.
						v = main_varData + abs(lit);
						MAIN_COMPLEXUP_ADD_DEC(lit,v);
						SET_LIT_NUM_STAMP(lit, main_litStamp);
					} //Else, the literal is already enqueued.
				}
			}
		}

		learnBinary = 0;
		numBinOccsForLearnBinary = -1;
		while (checkFrom < main_complexUP_current){
			l = *(checkFrom++);
			lid = GET_LIT_NUM_FROM_PTR(l);
			//We check the watcher lists of the falsified literal.
			l = main_litData - lid;

			//WL1
			offset = 0;
			while (GET_LIT_NUMW1_USED(l) > offset){
				c = GET_LIT_W1_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CLS_REPLIT(c);
				if (IS_LIT_SAT(lit) || GET_LIT_NUM_STAMP(lit) == main_litStamp){
					//The representative literal is satisfied. We do not have to look at the clause any further.
					++offset;
					continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CLS_WHEREW1(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CLS_SIZE(c); ++i){
					lit = GET_CLS_LITNUM(c,i);
					if (IS_LIT_SAT(lit) || GET_LIT_NUM_STAMP(lit) == main_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the second watcher.
					if (lit == GET_CLS_W2(c)) continue;
					//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
					if (IS_LIT_SAT(lit) || (!IS_LIT_ASSIGNED(lit) && GET_LIT_NUM_STAMP(-lit) != main_litStamp)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CLS_W1(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CLS_W1(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W1LIST(-lid, litPos);
					ADD_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CLS_W2(c);
					if (IS_LIT_UNSAT(lit) || GET_LIT_NUM_STAMP(-lit) == main_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						return c;
					} else {
						//The element is not yet on the stack. We stamp it.
						SET_LIT_NUM_STAMP(lit, main_litStamp);
						//We can use this literal to learn a new binary clause.
						if ((int32_t)(GET_LIT_NUM_NUMIMP_USED(lit)) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_LIT_NUM_NUMIMP_USED(lit);
						}

					}
				}
			}

			//WL2
			offset = 0;
			while (GET_LIT_NUMW2_USED(l) > offset){
				c = GET_LIT_W2_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CLS_REPLIT(c);
				if (IS_LIT_SAT(lit) || GET_LIT_NUM_STAMP(lit) == main_litStamp){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CLS_WHEREW2(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CLS_SIZE(c); ++i){
					lit = GET_CLS_LITNUM(c,i);
					if (IS_LIT_SAT(lit) || GET_LIT_NUM_STAMP(lit) == main_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the representative
						//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the first watcher.
					if (lit == GET_CLS_W1(c)) continue;
					//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
					if (IS_LIT_SAT(lit) || (!IS_LIT_ASSIGNED(lit) && GET_LIT_NUM_STAMP(-lit) != main_litStamp)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CLS_W2(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CLS_W2(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W2LIST(-lid, litPos);
					ADD_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CLS_W1(c);
					if (IS_LIT_UNSAT(lit) || GET_LIT_NUM_STAMP(-lit) == main_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						return c;
					} else {
						//The element is not yet on the stack. We stamp it.
						SET_LIT_NUM_STAMP(lit, main_litStamp);
						//We can use this literal to learn a new binary clause.
						if ((int32_t)(GET_LIT_NUM_NUMIMP_USED(lit)) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_LIT_NUM_NUMIMP_USED(lit);
						}
					}
				}
			}
		}

		if (!learnBinary) break;

		//We can now learn the new binary (-rootLit OR learnBinary).
		inp_intern_addLearnedBinaryClause(-GET_LIT_NUM_FROM_PTR(rootLit), learnBinary);
		//The implied binary must now be added to the UP queue.
		v = main_varData + abs(learnBinary);
		MAIN_COMPLEXUP_ADD_DEC(learnBinary, v);
		SET_LIT_NUM_STAMP(learnBinary, main_litStamp);
	} while (1);
	MAIN_COMPLEXUP_TERMINATE_CURRENT();
	return NULL;
}

void in_extern_NHBR(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//Performs non-transitive hyper binary resolution using unit propagation.
	literal *l;
	variable *v;
	clause *conflict = NULL;
	int32_t litID;

	#ifdef VERBOSE_IN
	printf("NHB");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t remLits = 0;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	#endif

	//Then, we walk through the literal permutation and satisfy each of the roots (if the literal is still a root).
	while ((l = lQ_dequeueBest()) !=NULL){
		litID = GET_LIT_NUM_FROM_PTR(l);
		//We ignore literals of variables that are already assigned.
		if (IS_LIT_ASSIGNED(litID) || IS_VAR_DISABLED((main_varData+abs(litID)))) continue;
		if (GET_LIT_NUMIMP_USED(l) == 0) continue;
		conflict = in_intern_nhbr_detect(l);
		if (conflict != NULL){
			//Literal l is a failed literal, we must propagate the opposite.
			inp_intern_backJump(0,0);
			v = main_varData + abs(litID);
			MAIN_COMPLEXUP_ADD_DEC(-litID, v);
			conflict = inp_intern_upPropagate();
			if (conflict != NULL){
				in_returnCode = IN_UNSAT;
				break;
			}
		} else {
			inp_intern_backJump(1,1);
		}

	}

	if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d,%d] ", initVars - f.n_vars_e_used, remLits, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_intern_els_replaceLit(literal* toReplace, literal* replaceWith){
	clause *c;
	variable *v;
	uint64_t sig;
	uint32_t i, occLitPosition, found, taut;
	int32_t toRID = GET_LIT_NUM_FROM_PTR(toReplace);
	int32_t reWID = GET_LIT_NUM_FROM_PTR(replaceWith);

	while (GET_LIT_NUMOCCS_USED(toReplace) > 0){
		//Grab the next clause that contains toReplace.
		c = GET_LIT_OCCNUM(toReplace, 0);
		occLitPosition = GET_LIT_OCCLITPOSNUM(toReplace,0);

		sig = 0ULL;
		found = 0;
		taut = 0;

		//Several things are performed in the following loop: 1. Look at all the literals and grab the signature bit of
		//those not equal to toReplace. 2. See if replaceWith is already in the clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			//Skip the literal we want to replace.
			if (GET_CLS_LITNUM(c, i) == toRID){
				continue;
			}
			//Retain if the literal that we want to replace with is already found.
			if (GET_CLS_LITNUM(c, i) == reWID){
				found = 1;
			}

			//Check if the opposite of the replacement is in the clause.
			if (GET_CLS_LITNUM(c, i) == -reWID){
				taut = 1;
				break;
			}

			//Update the signature with all the literals we encounter not equal to the one we replace.
			sig |= GET_LIT_SIG(GET_CLS_LITNUM(c, i));
		}

		//If the clause becomes a tautology, we just drop it.
		if (taut){
			inp_intern_abandonClause(c);
			continue;
		}

		//Disable the clause.
		inp_intern_disableClause(c);

		//If we have not found the literal we replace with, we must add its signature bit manually and write the replace
		//literal to the position where the old literal was.
		if (!found){
			//We did not find the literal we replace with. We manually add the signature bit. We override the literal.
			sig |= GET_LIT_SIG(reWID);
			SET_CLS_LITNUM(c, occLitPosition, reWID);
		} else {
			//We did find the replacement literal. This means that we effectively remove a literal form this clause.
			//We override the position of the literal to replace with the last literal in the clause.
			SET_CLS_SIZE_DEC(c);
			SET_CLS_LITNUM(c, occLitPosition, GET_CLS_LITNUM(c, GET_CLS_SIZE(c)));
			SET_CLS_OCCPOSNUM(c, occLitPosition, GET_CLS_OCCPOSNUM(c,GET_CLS_SIZE(c)));
		}

		//If the clause became unit, we do not have to re-enable it. If not, we can re-enable it normally.
		if (GET_CLS_SIZE(c) == 1){
			//The clause became unit because of the replacement. We can now learn that -reWID is a failed literal and put
			//reWID on the stack. If this fails it constitutes a global conflict.
			if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(reWID)){
				//This constitutes a global conflict.
				in_returnCode = IN_UNSAT;
				return;
			} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(reWID)){
				//We can add the literal to the UP queue.
				v = main_varData + abs(reWID);
				MAIN_COMPLEXUP_ADD_DEC(reWID, v);
			}//Else the literal is already enqueued.
			//This unit is now taken care of.
			inp_intern_abandonClause(c);
		} else if (GET_CLS_SIZE(c) == 2){
			//Update the clause signature.
			SET_CLS_SIG(c, sig);
			//Set the representative.
			SET_CLS_REPLIT(c, reWID);
			//Reset the watchers.
			SET_CLS_W1(c,0);
			SET_CLS_WHEREW1(c, 0);
			SET_CLS_W2(c,0);
			SET_CLS_WHEREW2(c, 0);
			//The clause is at least binary.
			inp_intern_enableClause(c);
		} else {
			//Update the clause signature.
			SET_CLS_SIG(c, sig);
			//We update the clause representative literal, watcher 1 and watcher 2.
			if (GET_CLS_REPLIT(c) == toRID){
				//We replaced the representative.
				SET_CLS_REPLIT(c, reWID);
			}
			if (GET_CLS_W1(c) == toRID){
				//We replaced the first watcher.
				if (GET_CLS_W2(c) != reWID){
					SET_CLS_W1(c, reWID);
				} else {
					for (occLitPosition = 0; occLitPosition < GET_CLS_SIZE(c); ++occLitPosition){
						if (GET_CLS_LITNUM(c, occLitPosition) != reWID){
							SET_CLS_W1(c, GET_CLS_LITNUM(c,occLitPosition));
							break;
						}
					}
				}
			} else if (GET_CLS_W2(c) == toRID){
				//We replaced the second watcher.
				if (GET_CLS_W1(c) != reWID){
					SET_CLS_W2(c, reWID);
				}  else {
					for (occLitPosition = 0; occLitPosition < GET_CLS_SIZE(c); ++occLitPosition){
						if (GET_CLS_LITNUM(c, occLitPosition) != reWID){
							SET_CLS_W2(c, GET_CLS_LITNUM(c,occLitPosition));
							break;
						}
					}
				}
			}
			inp_intern_enableClause(c);
		}
		//The literal has now been removed from the clause successfully.
	}
}

uint32_t in_intern_els_strongConnect(literal* l, uint32_t index){
	literal *impLit;
	uint32_t i;

	SET_LIT_ELS_INDEX(l,index);
	SET_LIT_ELS_LOWLINK(l, index);
	++index;
	*(inp_els_lStack_current++) = l;
	SET_LIT_ELS_ISINS(l, 1U);

	for (i= 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
		impLit = main_litData + GET_LIT_IMPNUM(l,i);
		if (GET_LIT_ELS_INDEX(impLit) == 0U){
			//Not yet marked. Recurse on implied literal.
			index = in_intern_els_strongConnect(impLit, index);
			if (GET_LIT_ELS_LOWLINK(impLit) < GET_LIT_ELS_LOWLINK(l)){
				SET_LIT_ELS_LOWLINK(l, GET_LIT_ELS_LOWLINK(impLit));
			}
		} else if (GET_LIT_ELS_ISINS(impLit)){
			//Already in stack -- implied literal is in the current SCC.
			if (GET_LIT_ELS_INDEX(impLit) < GET_LIT_ELS_LOWLINK(l)){
				SET_LIT_ELS_LOWLINK(l, GET_LIT_ELS_INDEX(impLit));
			}
		}
	}

	//If l is a root, then pop the stack and generate an SCC class.
	if (GET_LIT_ELS_LOWLINK(l) == GET_LIT_ELS_INDEX(l)){
		//Allocate new equivalence class if necessary.
		INP_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--inp_els_lStack_current);
			SET_LIT_ELS_ISINS(impLit, 0U);

			if (GET_LIT_STAMP(impLit) != main_litStamp){
				INP_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_LIT_STAMP(impLit, main_litStamp);
				SET_LIT_STAMP(GET_LIT_OPPOSITE(impLit), main_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (inp_els_classes_memUsed[inp_els_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++inp_els_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			inp_els_classes_memUsed[inp_els_classes_used] = 0;
		}
	}

	return index;
}

void in_extern_ELS(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	#ifdef VERBOSE_IN
	printf("ELS");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	#endif
	literal *l, *classRep;
	variable *v;
	uint32_t i,j, index = 1;
	int32_t toRID, reWID;

	//We back-jump in preparation of unit propagation.
	inp_intern_backJump(0,0);

	//Initialize the ELS literal stack to be empty.
	inp_els_lStack_current = inp_els_lStack + 1;

	//We prepare the literal stamp to present classes only once.
	++main_litStamp;

	//Initialize the classes information.
	inp_els_classes_used = 0;
	for (i = 0; i < inp_els_classes_avail; ++i){
		inp_els_classes_memUsed[i] = 0;
	}

	//Prepare all remaining literals.
	for(i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		l = main_litData + GET_VAR_ID(v);
		SET_LIT_ELS_INDEX(l, 0);
		SET_LIT_ELS_LOWLINK(l, 0);
		SET_LIT_ELS_ISINS(l, 0);
		l = main_litData - GET_VAR_ID(v);
		SET_LIT_ELS_INDEX(l, 0);
		SET_LIT_ELS_LOWLINK(l, 0);
		SET_LIT_ELS_ISINS(l, 0);
	}

	for (i = 0; i < f.n_vars_e_used; ++i){
		v = f.vars_e[i];
		l = main_litData + GET_VAR_ID(v);
		//Check if the literal has been checked by strong-connect.
		if (GET_LIT_ELS_INDEX(l) == 0){
			index = in_intern_els_strongConnect(l, index);
		}
		l = GET_LIT_OPPOSITE(l);
		if (GET_LIT_ELS_INDEX(l) == 0){
			index = in_intern_els_strongConnect(l, index);
		}
	}

	//We walk through all the found equivalence classes and replace the literals.
	for (i = 0; i < inp_els_classes_used; ++i){
		//For class i, we pick the class representative (the first literal in the class).
		classRep = inp_els_classes[i][0];
		reWID = GET_LIT_NUM_FROM_PTR(classRep);
		//For all the remaining literals in the class, we replace their occurrence with the representative.
		for (j = 1; j < inp_els_classes_memUsed[i]; ++j){
			//Grab the literal and variable to replace.
			l = inp_els_classes[i][j];
			toRID = GET_LIT_NUM_FROM_PTR(l);
			v = main_varData + abs(toRID);

			//Replace the occurrence of l with classRep.
			in_intern_els_replaceLit(l, classRep);
			if (in_returnCode != IN_UNKNOWN){
				#ifdef VERBOSE_IN
				printf("representative conflict.\n");
				#endif
				return;
			}

			//Replace the occurrence of -l with -classRep.
			in_intern_els_replaceLit(GET_LIT_OPPOSITE(l),GET_LIT_OPPOSITE(classRep));
			if (in_returnCode != IN_UNKNOWN){
				#ifdef VERBOSE_IN
				printf("representative conflict.\n");
				#endif
				return;
			}

			//Mark the replaced variable with its representative.
			if ((toRID < 0 && reWID < 0) || (toRID > 0 && reWID > 0)){
				SET_VAR_ISEQUIVTO(v, abs(reWID));
			} else {
				SET_VAR_ISEQUIVTO(v, -abs(reWID));
			}

			//Disable the replaced variable.
			inp_intern_disableVariable(v);
		}
	}

	//Finally, we perform unit propagation.
	if (inp_intern_upPropagate() != NULL){
		#ifdef VERBOSE_IN
		printf(" propagation conflict ");
		#endif
		in_returnCode = IN_UNSAT;
	}
	if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d] ", initVars - f.n_vars_e_used, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_intern_treelook_enqueue(literal *l){
	//First, we enqueue literal l.
	literal *lOpp = GET_LIT_OPPOSITE(l), *lDash;
	uint32_t i;
	*(inp_trl_lQ_e++) = l;
	SET_LIT_TRL_ISINQ(l, 1U);
	//We now want all literal l' for which we have clauses (l v -l'). We pick all the implications of -l, and invert their
	//sign. In the above example, the implication list of -l contains -l'. Inverting the sign gives us l'.
	for (i = 0; i < GET_LIT_NUMIMP_USED(lOpp); i+=2){
		//Grab l' by inverting the sign of the implication of -l.
		lDash = main_litData - GET_LIT_IMPNUM(lOpp, i);
		//Check if lDash is already enqueued. If not, enqueue it.
		if (!GET_LIT_TRL_ISINQ(lDash)){
			in_intern_treelook_enqueue(lDash);
		}
	}
	//Finally, we put the special element NULL into the queue.
	*(inp_trl_lQ_e++) = NULL;
}

uint32_t in_intern_treelook_checkUnsat(){
	//Check if any of the clauses that lost a literal is now false under the tree-look assignment.
	clause *c;
	int32_t lit;
	uint32_t i, numUnsatLits;

	//Pop all the clauses from the stack and check if any of them is empty under the tree-look assignment.
	while ((c = cQ_dequeueBest()) != NULL){
		//Check if the clause is satisfied.
		if (IS_LIT_SAT(GET_CLS_REPLIT(c))){
			//If the clause is satisfied we do not need to look at it again.
			continue;
		}
		//If not, we must count the number of unsatisfied literals it has.
		numUnsatLits = 0;
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			lit = GET_CLS_LITNUM(c,i);
			if (IS_LIT_SAT(lit)){
				SET_CLS_REPLIT(c,lit);
				numUnsatLits = 0;
				break;
			} else if (IS_LIT_UNSAT(lit)) {
				++numUnsatLits;
			} else {
				//The clause has at least one not yet assigned literal. We do not need it.
				numUnsatLits = 0;
				break;
			}
		}
		if (numUnsatLits == GET_CLS_SIZE(c)){
			//Yes, the number of false literals in this clause matches the number of literals in it. The clause is false. We
			//put it back into the queue for later re-checking and return.
			cQ_touchClause(c);
			return 1;
		}
	}
	return 0;
}

clause* in_intern_treelook_detect(literal *rootLit){
	//This method propagates rootLit and all implied assignments.

	literal **checkStart = main_complexUP_current, **checkFrom, *aLit;
	variable *v = main_varData + abs(GET_LIT_NUM_FROM_PTR(rootLit));
	clause *c;
	int32_t aLitID = 0, lit = 0;
	uint32_t count, litPos, offset;
	int32_t i, learnBinary;

	MAIN_COMPLEXUP_ADD_DEC_PTR(rootLit, v)

	do {
		//We retain the information from what position in the queue the new assignments start.
		checkFrom = checkStart;

		do {
			//We grab the next literal to assign and assign it.
			aLit = *(main_complexUP_current++);
			lit = GET_LIT_NUM_FROM_PTR(aLit);
			if (!IS_LIT_ASSIGNED(lit)){
				v = main_varData + abs(GET_LIT_NUM_FROM_PTR(aLit));
				if (lit < 0){
					SET_VAR_ASS_0(v);
				} else {
					SET_VAR_ASS_1(v);
				}
			}

			//From the just assigned literal, we must put all the implied on the stack as well and assign them accordingly.
			for (count = 0; count < GET_LIT_NUMIMP_USED(aLit); count += 2){
				lit = GET_LIT_IMPNUM(aLit, count);
				if (IS_LIT_SAT(lit)){
					//No further action must be performed, but we can update the representative for this clause.
					SET_CLS_REPLIT(GET_LIT_IMPNUM_REASON(aLit, count), lit);
				} else if (IS_LIT_UNSAT(lit)){
					//A conflict was discovered. We can return.
					main_complexUP_current = main_complexUP_end;
					return (clause*)1;
				} else {
					//The literal is neither satisfied nor unsatisfied. This implies that it is not assigned.
					v = main_varData + abs(lit);
					MAIN_COMPLEXUP_ADD_DEC(lit, v);
					if (lit < 0){
						SET_VAR_ASS_0(v);
					} else {
						SET_VAR_ASS_1(v);
					}
					//We can update the representative for this clause.
					SET_CLS_REPLIT(GET_LIT_IMPNUM_REASON(aLit, count), lit);
				}
			}
		} while (main_complexUP_current < main_complexUP_end);

		//Now all binary implications have been propagated. It is time to find out if any of the large clauses that lost
		//a literal during these action is either empty, or unit. In case it is empty we can return right away as well. If it
		//is unit, we can put the last literal in the clause (which MUST be unassigned now) on the stack as well and
		//propagate its assignment and implications.
		learnBinary = 0;
		while (checkFrom < main_complexUP_current){
			aLit = *(checkFrom++);
			aLitID = -GET_LIT_NUM_FROM_PTR(aLit);
			//Literal aLitID is now a literal that is false. We check all the clauses where this literal is a watcher.

			//WL1
			offset = 0;
			while (offset < GET_LIT_NUM_NUMW1_USED(aLitID)){
				c = GET_LIT_NUM_W1_WATCHED_CLS(aLitID, offset);
				//Check if the clause is satisfied by its representative.
				if (IS_LIT_SAT(GET_CLS_REPLIT(c)) || IS_LIT_SAT(GET_CLS_W2(c)) ){
					//Yes, no need to look at it again.
					++offset;
					continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CLS_WHEREW1(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CLS_SIZE(c); ++i){
					lit = GET_CLS_LITNUM(c,i);
					if (IS_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the second watcher.
					if (lit == GET_CLS_W2(c)) continue;
					//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
					if (!IS_LIT_ASSIGNED(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CLS_W1(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CLS_W1(c) != aLitID){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W1LIST(aLitID, litPos);
					ADD_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					lit = GET_CLS_W2(c);
					if (IS_LIT_UNSAT(lit)){
						//A conflict has been discovered.
						return c;
					} else if (!IS_LIT_SAT(lit)){
						//The second watcher is not assigned. Since we are about to assign it such that it is true, we can
						//make it the representative of this clause to faster ignore it in future checks.
						SET_CLS_REPLIT(c,lit);
						learnBinary = lit;
						break;
					} else {
						++offset;
					}
				}
			}
			//After checking the clauses, we must check if we can learn a new binary clause from that and can put its
			//its assignment into the current decision level.
			if (learnBinary != 0){
				//We can now learn the new binary (-rootLit OR learnBinary).
				inp_intern_addLearnedBinaryClause(-GET_LIT_NUM_FROM_PTR(rootLit), learnBinary);
				//The implied binary must now be added to the UP queue.
				v = main_varData + abs(learnBinary);
				MAIN_COMPLEXUP_ADD_DEC(learnBinary, v);
				//We stop checking large clauses as there is new stuff to be propagated (and possibly new binaries that must
				//be prioritized).
				break;
			}
			//If we were unable to learn a new binary clause, we continue with checking the second watcher.
			//WL2
			offset = 0;
			while (offset < GET_LIT_NUM_NUMW2_USED(aLitID)){
				c = GET_LIT_NUM_W2_WATCHED_CLS(aLitID, offset);
				//Check if the clause is satisfied by its representative.
				if (IS_LIT_SAT(GET_CLS_REPLIT(c)) || IS_LIT_SAT(GET_CLS_W1(c))  ){
					//Yes, no need to look at it again.
					++offset;
					continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CLS_WHEREW2(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CLS_SIZE(c); ++i){
					lit = GET_CLS_LITNUM(c,i);
					if (IS_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						SET_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the first watcher.
					if (lit == GET_CLS_W1(c)) continue;
					//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
					if (!IS_LIT_ASSIGNED(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CLS_W2(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CLS_W2(c) != aLitID){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W2LIST(aLitID, litPos);
					ADD_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					lit = GET_CLS_W1(c);
					if (IS_LIT_UNSAT(lit)){
						//A conflict has been discovered.
						return c;
					} else if (!IS_LIT_SAT(lit)){
						//The first watcher is not assigned. Since we are about to assign it such that it is true, we can
						//make it the representative of this clause to faster ignore it in future checks.
						SET_CLS_REPLIT(c,lit);
						learnBinary = lit;
						break;
					} else {
						++offset;
					}
				}
			}
			//After checking the clauses, we must check if we can learn a new binary clause from that and can put its
			//its assignment into the current decision level.
			if (learnBinary != 0){
				//We can now learn the new binary (-rootLit OR learnBinary).
				inp_intern_addLearnedBinaryClause(-GET_LIT_NUM_FROM_PTR(rootLit), learnBinary);
				//The implied binary must now be added to the UP queue.
				v = main_varData + abs(learnBinary);
				MAIN_COMPLEXUP_ADD_DEC(learnBinary, v);
				//We stop checking large clauses as there is new stuff to be propagated (and possibly new binaries that must
				//be prioritized).
				break;
			}
		}
	} while (main_complexUP_current < main_complexUP_end);

	//In case we reach this position the UP finished without a conflict, but it might have learned new binaries.
	return NULL;
}

void in_extern_TREELOOK(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//This method performs ELS until saturation. Afterwards it creates an out-tree queue. Then it learns NHBR clauses.
	#ifdef VERBOSE_IN
	printf("TRL: ");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	int32_t ignoredOutTrees = 0;
	#endif

	clause *conflict = NULL, *c;
	literal *aLit, **currentDL;
	variable *v;
	int32_t aLitID;
	uint32_t i, offset, numFailed = 0;


	//We start to perform ELS until saturation.
	do {
		i = f.n_vars_e_used;
		in_extern_ELS();
		if (in_returnCode != IN_UNKNOWN) {
			#ifdef VERBOSE_IN
			printf("ELS-C");
			#endif
			return;
		}
	} while (i > f.n_vars_e_used);

	//We can now initialize the out-tree queue. We do not have to reset the LIT_TRL_ISINQ(l) value, because that is
	//the ISINS value from the ELS. Since ELS must pop all elements from the stack, all these values will be zero here.
	inp_trl_lQ_c = inp_trl_lQ;
	inp_trl_lQ_e = inp_trl_lQ;

	lQ_flush();
	lQ_touchLiteral = &lQ_touchLiteral_inp_treelook;

	for (i = 0; i < f.n_vars_e_used; ++i){
		aLit = main_litData + GET_VAR_ID(f.vars_e[i]);
		lQ_touchLiteral(aLit);
		aLit = main_litData - GET_VAR_ID(f.vars_e[i]);
		lQ_touchLiteral(aLit);
	}

	while ((aLit = lQ_dequeueBest()) != NULL){
		if (GET_LIT_NUMIMP_USED(aLit) == 0){
			in_intern_treelook_enqueue(aLit);
		}
	}
	lQ_touchLiteral = &lQ_touchLiteral_null;

	//Prepare the clause priority queue for efficient test if any large clause became false under reduction.
	cQ_flush();
	cQ_touchClause = &cQ_touchClause_inp_treelook;

	//At this point we have enqueued all the root literals along with their recursive implications. We continue by performing
	//the tree-look as long as the queue is not empty.

	++main_litStamp;

	//We back-jump in preparation for the tree-look stacking.
	inp_intern_backJump(0,0);

	while (inp_trl_lQ_c < inp_trl_lQ_e){
		//Grab the next element from the literal queue.
		aLit = *(inp_trl_lQ_c++);
		//Optimization: Out-trees of size one can be ignored if the root-literal for this out-tree has no implications.
		if (main_decLevel == 0 		//We would now start a new assignment trail on the stack (checking a new out-tree).
				&& aLit != NULL 	//The literal could actually be propagated.
				&& *inp_trl_lQ_c == NULL //However, the size of the tree is one.
				&& GET_LIT_NUMIMP_USED(aLit) == 0	//And it does not receive any new implications.
				){
			//The literal aLit is the top of an out-tree of depth one and has no implications. So propagating it would result
			//in no further assignments. Since the out-tree is then removed from the decision stack right afterwards, there
			//is no need to propagate at all -- nothing can be gained by this. We simply continue with the next out-tree.
			inp_trl_lQ_c++;
			#ifdef VERBOSE_IN
			++ignoredOutTrees;
			#endif
			continue;
		}

		if (aLit != NULL){
			//Grab the new element from the out tree.
			aLitID = GET_LIT_NUM_FROM_PTR(aLit);

			//Open up a new decision level.
			MAIN_COMPLEXUP_TERMINATE_CURRENT();

			//We first check if the literal that is supposed to open up the new level is a failed literal.
			if (IS_LIT_UNSAT(aLitID) || in_intern_treelook_checkUnsat()){
				//It is a failed literal. We must store this information if this does not already end in a global conflict.
				if (GET_LIT_STAMP(GET_LIT_OPPOSITE(aLit)) == main_litStamp){
					//It ends in a global conflict. We can stop.
					#ifdef VERBOSE_IN
					printf("TRL-Ca");fflush(stdout);
					#endif
					conflict = (clause*)1;
				} else if (GET_LIT_STAMP(aLit) != main_litStamp){
					//It does not end in a global conflict. We can still store this literal.
					inp_trl_faileds[numFailed++] = aLit;
					SET_LIT_STAMP(aLit, main_litStamp);
				}//Else the literal is already marked for later propagation.
			} else if (!IS_LIT_SAT(aLitID)){
				//The literal is neither failed nor already satisfied. We can perform a detection on in in this new level.
				conflict = in_intern_treelook_detect(aLit);

				//Check if this resulted in a conflict.
				if (conflict != NULL){
					//It is a failed literal. We must store this information if this does not already end in a global conflict.
					conflict = NULL;
					if (GET_LIT_STAMP(GET_LIT_OPPOSITE(aLit)) == main_litStamp){
						//It ends in a global conflict. We can stop.
						#ifdef VERBOSE_IN
						printf("TRL-Cb");fflush(stdout);
						#endif
						conflict = (clause*)1;
					} else if (GET_LIT_STAMP(aLit) != main_litStamp){
						//It does not end in a global conflict. We can still store this literal.
						inp_trl_faileds[numFailed++] = aLit;
						SET_LIT_STAMP(aLit, main_litStamp);
					}//Else the literal is already marked for later propagation.
					if (conflict == NULL){
						//We must now enqueue all clauses that lost a literal in the last decision level. All these clauses
						//can be false (and stay false) now until we remove the level in the out-tree. It suffices to look
						//at the remaining watched clauses for all these literals.
						currentDL = main_decStackPtrs[main_decLevel];
						while (currentDL < main_complexUP_end){
							aLit = *(currentDL++);
							aLit = GET_LIT_OPPOSITE(aLit);
							//The aLit is now one of the literals that have been falsified in the last decision level. We
							//take a look at all the remaining clauses this literal has. If the clause has a satisfied
							//representative, we ignore it. If not, we add it to the clause queue for later checks.
							offset = 0;
							while (offset < GET_LIT_NUMOCCS_USED(aLit)){
								c = GET_LIT_OCCNUM(aLit, offset++);
								if (!IS_LIT_SAT(GET_CLS_REPLIT(c))){
									cQ_touchClause(c);
								}
							}
							//Now all clauses that that contained the falsified aLit are enqueued if not already satisfied.
						}
						//Now all clauses that lost a literal are enqueued.
					}
				}
			}
		} else {
			inp_intern_backJump(main_decLevel-1,0);
		}
		if (conflict != NULL) break;
	}

	//Finalize the tree-look by again moving into DL1.
	MAIN_COMPLEXUP_TERMINATE_CURRENT();

	if (numFailed > 0 && conflict == NULL){
		//We must propagate all the failed literals with opposite sign.
		inp_intern_backJump(0,0);
		for (i = 0; i < numFailed; ++i){
			aLit = GET_LIT_OPPOSITE(inp_trl_faileds[i]);
			v = main_varData + abs(GET_LIT_NUM_FROM_PTR(aLit));
			MAIN_COMPLEXUP_ADD_DEC_PTR(aLit, v);
		}
		conflict = inp_intern_upPropagate();
	}

	if (conflict != NULL){
		//We must back-jump to DL0, as this is where the conflict happened.
		if (main_decLevel > 0) inp_intern_backJump(0,0);
		in_returnCode = IN_UNSAT;
	} if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	//Reset clauses stack and queue.
	cQ_flush();
	cQ_touchClause = &cQ_touchClause_null;

	#ifdef VERBOSE_IN
	printf("<%d>[%d,%d] ", ignoredOutTrees, initVars - f.n_vars_e_used, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_extern_NIVER(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	//This method implements NIVER for the global in-processing.
	#ifdef VERBOSE_IN
	printf("NIV");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	#endif

	literal *lOuter, *lInner;
	clause *c, *d, *conflict = NULL;
	variable *v;
	uint64_t litSig, cSigSimp;
	uint32_t totalClauses, temp,i,j,k;
	int32_t lit;


	while ((lOuter = lQ_dequeueBest()) != NULL){
		//We pick the best outer variables and its inner correspondent.
		lit = GET_LIT_NUM_FROM_PTR(lOuter);
		lInner = GET_LIT_OPPOSITE(lOuter);
		lQ_delete(lInner);
		totalClauses = GET_LIT_NUMOCCS_USED(lOuter) + GET_LIT_NUMOCCS_USED(lInner);

		//We ignore the variable if it is assigned.
		v = main_varData + abs(lit);
		if (!IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)){
			continue;
		}
		//We check if the literals are pure.
		if (GET_LIT_NUMOCCS_USED(lOuter) == 0 || GET_LIT_NUMOCCS_USED(lInner) == 0){
			//Yes, that is the case. We will now perform the unit propagation in DL0.
			inp_intern_backJump(0,0);
			v = main_varData + abs(GET_LIT_NUM_FROM_PTR(lOuter));
			if (GET_LIT_NUMOCCS_USED(lOuter) == 0){
				MAIN_COMPLEXUP_ADD_DEC_PTR(lInner, v);
			} else {
				MAIN_COMPLEXUP_ADD_DEC_PTR(lOuter, v);
			}
			conflict = inp_intern_upPropagate();
			if (conflict != NULL) break;
			continue;
		}

		//After this check, we will perform NIVER, if and only if the number of resolvents added is not exceeding the clauses
		//that will be removed by this operation.
		temp = 0;
		//We now walk through the occurrence list of lOuter, for the picked clause c we kill the bit for lit if possible.
		//If this is not possible, we can go on with the next clause right away (all signature tests would fail anyway).
		litSig = GET_LIT_SIG(lit);
		for (i = 0; i < GET_LIT_NUMOCCS_USED(lOuter); ++i){
			c = GET_LIT_OCCNUM(lOuter, i);
			cSigSimp = GET_CLS_SIG(c);
			if (GET_CLS_SIZE(c) == POPCOUNT_UINT64(cSigSimp)){
				//Since each literal has its own bit in the signature, we can kill the bit of lit.
				cSigSimp &= ~litSig;
			} else {
				//We must find out the hard way if there is a literal in c different from lit that has the same signature.
				for (j = 0; j < GET_CLS_SIZE(c); ++j){
					if (GET_CLS_LITNUM(c,j) != lit && GET_LIT_SIG(GET_CLS_LITNUM(c,j)) == litSig){
						break;
					}
				}
				if (j == GET_CLS_SIZE(c)){
					//We have not found another literal that causes the same signature as lit. We can kill the bit.
					cSigSimp &= ~litSig;
				}
			}
			//Here we check if we successfully killed the bit. If not, we must go on with the next clause c, or abort
			//the tests (if we could not get above the border of totalClauses anyway).
			if ((cSigSimp & litSig) != 0ULL){
				//We were unable to kill the bit. We must go on with the next clause.
				continue;
			}
			//Otherwise, we can rotate the signature and check all occurrences in lInner.
			cSigSimp = ROTATE_32RIGHT_UINT64(cSigSimp);
			//Now we use this signature to check against all occurrences of lInner. Those, were no tautology would be
			//possible increase temp.
			for (j = 0; j < GET_LIT_NUMOCCS_USED(lInner); ++j){
				d = GET_LIT_OCCNUM(lInner, j);
				if ((cSigSimp & GET_CLS_SIG(d)) == 0ULL) ++temp;
			}

			//We can stop if it is already too much.
			if (temp > totalClauses) {
				break;
			}
		}
		//As soon as temp > totalClauses, we know that this cannot work for NIVER, we then go on.
		if (temp > totalClauses) {
			continue;
		}
		//It seems that we are going to perform the resolution. The number of resolvents must be set to a worst case of
		//posClauses*negClauses, because all the resolvents could survive the resolution.
		INP_RESET_ALL_RESOLVENTS();
		for (i = 0; i < GET_LIT_NUMOCCS_USED(lOuter); ++i){
			c = GET_LIT_OCCNUM(lOuter, i);
			for (j = 0; j < GET_LIT_NUMOCCS_USED(lInner); ++j){
				d = GET_LIT_OCCNUM(lInner, j);
				//We now prepare the copying operation. The literal we resolve on is lit in c and -lit in d.
				INP_RESET_RESOLVENT(inp_resUsed);
				++main_litStamp;
				//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
				for (k = 0; k < GET_CLS_SIZE(c); ++k){
					if (GET_CLS_LITNUM(c,k) != lit){
						INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(c,k));
						SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,k), main_litStamp);
					}
				}
				//We now copy all the literals from d, we invert the sign of lit for this, as this is what we ignore in d.
				lit = -lit;
				for (k = 0; k < GET_CLS_SIZE(d); ++k){
					//We ignore the resolution literal and any literal already in the resolvent.
					if (GET_CLS_LITNUM(d,k) != lit && GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k)) != main_litStamp){
						//We check if adding this literal will result in a tautology. If not, we add it. If so, we break.
						if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,k)) == main_litStamp){
							//We break early since this will end in a tautology.
							break;
						}
						//No tautology yet.
						INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(d,k));
						SET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k), main_litStamp);
					}
				}
				//We revert the temporary sign inversion.
				lit = -lit;
				if (k < GET_CLS_SIZE(d)){
					//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
					continue;
				}
				//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
				INP_INCREASE_NUM_RESOLVENTS( temp );
				//If we are now above the border of totalClauses, then we can abort the NIVER operations on this variable, as
				//NIVER will not be performed.
				if (inp_resMemUsed[inp_resUsed-1] == 1){
					//The created resolvent is a unit clause. Independent of everything we did so far, we will propagate
					//this finally.
					inp_intern_backJump(0,0);
					v = main_varData + abs(inp_res[inp_resUsed-1][0]);
					MAIN_COMPLEXUP_ADD_DEC(inp_res[inp_resUsed-1][0], v);
					conflict = inp_intern_upPropagate();
					inp_resUsed = INT32_MAX;
				}
				if (inp_resUsed > totalClauses){
					break;
				}
			}
			if (inp_resUsed > totalClauses){
				break;
			}
		}
		if (conflict != NULL) break;
		//This concludes the learning operations. The following check determines if we are going to perform the variable
		//elimination by removing the old clauses and adding the new resolvents in stead.
		if (inp_resUsed > totalClauses){
			//Nope, we drop everything and go on with the next variable.
			continue;
		}

		//We perform the elimination. We first delete the larger set, and then make the clauses of the smaller set removed.
		while (GET_LIT_NUMOCCS_USED(lInner) > 0){
			d = GET_LIT_OCCNUM(lInner, 0);
			inp_intern_abandonClause(d);
		}
		//We then transfer the smaller set to the removed clauses array. The literal lit we used for the resolution is the
		//one literal of interest for NIVER.
		while (GET_LIT_NUMOCCS_USED(lOuter) > 0){
			c = GET_LIT_OCCNUM(lOuter, 0);
			inp_intern_makeClauseRemoved(c, lit, PREP_REMCLS_NIVER);
		}
		//We finally assign the variable behind lit in such a way that it does NOT satisfy the clauses we just removed. So
		//the literal lit will become FALSE because of this assignment. We then disable the variable.
		v = main_varData + abs(lit);
		if (lit > 0){
			//The literal is positive. Assign it with 0.
			SET_VAR_ASS_0(v);
		} else {
			//The literal is negative. Assign it with 1.
			SET_VAR_ASS_1(v);
		}
		inp_intern_disableVariable(v);

		//We now add all the resolvents as new original clauses to the formula.
		for (i = 0; i < inp_resUsed; ++i){
			inp_intern_addOriginalClause(inp_res[i],inp_resMemUsed[i]);
		}
		//This concludes NIVER for variable v.
	}

	if (conflict != NULL){
		#ifdef VERBOSE_IN
		printf(" propagation conflict ");
		#endif
		in_returnCode = IN_UNSAT;
	} else if (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d] ", initVars - f.n_vars_e_used, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void in_intern_ge_extract(variable* v){
	//To extract the given variable along with the current GDEF.
	int32_t i, j, k, ignoreLit, hadUnits = 0;
	clause *c, *d;
	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);
	//In case variable v has a definition provided in the sets G_-v and G_v, we can remove the variable from the formula by
	//performing the following steps.
	//		1. Disable the clauses defining the gate.
	//		2. Creating the resolvents of set R_v and G_-v, as well as R_-v and G_v (resolve the remaining clauses).
	//		3. Making all the clauses participating in the gate definition removed (v/-v being the litOfInterest).
	//		4. Disabling all remaining clauses that contain the defined variable in R_v and R_-v.
	//		5. Disable the variable v.
	//		6. Add the resolvents to the formula.

	//1. We have identified a gate, and will first make the clauses defining it disabled. This way they do not appear in any
	//occurrence or watcher lists or binary implication lists anymore, but their literal arrays are still available.
	for (i = 0; i < inp_G_negVUsed; ++i){
		inp_intern_disableClause(inp_G_negV[i]);
	}
	for (i = 0; i < inp_G_posVUsed; ++i){
		inp_intern_disableClause(inp_G_posV[i]);
	}
	//2. We must now create all the resolvents, one set is G_-v and R_v (the remaining other clauses containing v), the other
	//set is G_v and R_-v (the remaining other clauses containing -v). Since we already disabled the clauses defining the
	//gate, all the remaining occurrences belong to either R_v or R_-v. We will pick one element from G_negV/G_posV after
	//another and resolve them with all remaining occurrences.

	INP_RESET_ALL_RESOLVENTS();

	//Resolve G_-v with R_v.
	for (i = 0; i < inp_G_negVUsed; ++i){
		c = inp_G_negV[i];
		for (j = 0; j < GET_LIT_NUMOCCS_USED(lPos); ++j){
			d = GET_LIT_OCCNUM(lPos, j);
			//We now prepare the copying operation. The literal we resolve on is -ignoreLit in c and ignoreLit in d.
			INP_RESET_RESOLVENT(inp_resUsed);
			++main_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			ignoreLit = -GET_VAR_ID(v);
			for (k = 0; k < GET_CLS_SIZE(c); ++k){
				if (GET_CLS_LITNUM(c, k) != ignoreLit){
					INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(c, k));
					SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c, k), main_litStamp);
				}
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			for (k = 0; k < GET_CLS_SIZE(d); ++k){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (GET_CLS_LITNUM(d,k) != ignoreLit && GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k)) != main_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it.
					if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,k)) == main_litStamp){
						//We break early since the resolvent would be a tautology.
						break;
					}
					//No tautology yet.
					INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(d,k));
					SET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k), main_litStamp);
				}
			}
			if (k < GET_CLS_SIZE(d)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			INP_INCREASE_NUM_RESOLVENTS( k );
			if (inp_resMemUsed[inp_resUsed-1] == 1){
				hadUnits = 1;
			}
		}
	}

	//Resolve G_v with R_-v.
	for (i = 0; i < inp_G_posVUsed; ++i){
		c = inp_G_posV[i];
		for (j = 0; j < GET_LIT_NUMOCCS_USED(lNeg); ++j){
			d = GET_LIT_OCCNUM(lNeg, j);
			//We now prepare the copying operation. The literal we resolve on is ignoreLit in c and -ignoreLit in d.
			INP_RESET_RESOLVENT(inp_resUsed);
			++main_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			ignoreLit = GET_VAR_ID(v);
			for (k=0; k < GET_CLS_SIZE(c); ++k){
				if (GET_CLS_LITNUM(c,k) != ignoreLit){
					INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(c,k));
					SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,k), main_litStamp);
				}
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			for (k=0; k < GET_CLS_SIZE(d); ++k){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (GET_CLS_LITNUM(d,k) != ignoreLit && GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k)) != main_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it.
					if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(d,k)) == main_litStamp){
						//We break early since the resolvent would be a tautology.
						break;
					}
					//No tautology yet.
					INP_ADD_LIT_TO_CURRENT_RESOLVENT(GET_CLS_LITNUM(d,k));
					SET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,k), main_litStamp);
				}
			}

			if (k < GET_CLS_SIZE(d)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			INP_INCREASE_NUM_RESOLVENTS( k );
			if (inp_resMemUsed[inp_resUsed-1] == 1){
				hadUnits = 1;
			}
		}
	}
	//We have now created all the resolvents.

	//3. Make the clauses defining the gate removed using v->id and -v->id as the literal of interest.
	for (i = 0; i < inp_G_negVUsed; ++i){
		inp_intern_makeClauseRemoved(inp_G_negV[i], -GET_VAR_ID(v), PREP_REMCLS_GE);
	}
	for (i = 0; i < inp_G_posVUsed; ++i){
		inp_intern_makeClauseRemoved(inp_G_posV[i],  GET_VAR_ID(v), PREP_REMCLS_GE);
	}

	//4. Disable all remaining clauses that contain the defined variable in R_v and R_-v.
	while (GET_LIT_NUMOCCS_USED(lPos) > 0){
		c = GET_LIT_OCCNUM(lPos, 0);
		inp_intern_abandonClause(c);
	}
	while (GET_LIT_NUMOCCS_USED(lNeg) > 0){
		c = GET_LIT_OCCNUM(lNeg, 0);
		inp_intern_abandonClause(c);
	}

	//5. Disable variable v.
	inp_intern_disableVariable(v);

	//6. Add the created resolvents.
	if (hadUnits){
		inp_intern_backJump(0,0);
	}
	for (i = 0; i < inp_resUsed; ++i){
		if (inp_resMemUsed[i] == 1){
			if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(inp_res[i][0])){
				in_returnCode = IN_UNSAT;
				return;
			} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(inp_res[i][0])){
				v = main_varData + abs(inp_res[i][0]);
				MAIN_COMPLEXUP_ADD_DEC(inp_res[i][0], v);
			}
		} else {
			inp_intern_addOriginalClause(inp_res[i], inp_resMemUsed[i]);
		}
	}
	if (hadUnits){
		#ifdef VERBOSE_IN
		printf("<U>");
		#endif
		if (inp_intern_upPropagate() != NULL){
			in_returnCode = IN_UNSAT;
		}
	}
}

void in_intern_ge_AND(variable* v){
	//This method will check if the given variable has an AND gate definition.
	//For three variables this looks like this for v:
	//(v or -a or -b or -c)
	//(-v or a)
	//...
	//(-v or c)
	//v <=> a = b = c = 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i,j, theSize = INT32_MAX;
	clause *c = NULL;
	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);

	//We reset the gate definition arrays, we are about to need them.
	INP_RESET_GDEFARRAYS();

	//In order to detect such an AND definition, we will first go through all the implications of the positive v, stamp
	//each of the implied literals. After that, we walk through all the occurrences of the positive v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an AND gate.
	//First, we stamp all implications of the positive v (by checking the implications of -v).
	++main_litStamp;
	SET_LIT_STAMP(lNeg, main_litStamp);
	for (i = 0; i < GET_LIT_NUMIMP_USED(lPos); i+=2){
		SET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lPos,i), main_litStamp);
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (-v or a) are now stamped. We now
	//check if any of the clauses that contains +v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an AND gate.
	for (i = 0; i < GET_LIT_NUMOCCS_USED(lPos); ++i){
		c = GET_LIT_OCCNUM(lPos, i);
		if (GET_CLS_SIZE(c) < 3 || GET_LIT_NUMIMP_USED(lPos) < GET_CLS_SIZE(c) - 1) continue;
		//Check if all opposite literals in c are stamped.
		for (j=0; j < GET_CLS_SIZE(c); ++j){
			if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,j)) != main_litStamp){
				//This opposite literal was not stamped. Clause cannot be used.
				break;
			}
		}
		if (j == GET_CLS_SIZE(c)){
			//In case we did not break early, we have found a clause c used in the definition of an AND gate. We store the size
			//of the large clause (the smaller, the better).
			if (GET_CLS_SIZE(c) < theSize){
				//Yes, the new found clause used in the AND definition is smaller than the old one. Since it can be only
				//one clause containing the variable v positively in the AND gate definition, we just override what we had.
				theSize = GET_CLS_SIZE(c);
				inp_G_posV[0] = c;
				inp_G_posVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an AND gate using clause G_posV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++main_litStamp;
		for (j = 0; j < GET_CLS_SIZE(inp_G_posV[0]); ++j){
			SET_LIT_NUM_STAMP(-GET_CLS_LITNUM(inp_G_posV[0],j), main_litStamp);
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < GET_LIT_NUMIMP_USED(lPos); i+=2){
			if (GET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lPos, i)) == main_litStamp){
				SET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lPos, i), main_litStamp -1);
				//Put this clause into the G_negV array.
				INP_ADD_CLS_TO_GNEGV(GET_LIT_IMPNUM_REASON(lPos, i));
			}
		}

		//We have now collected all the defining clauses for the AND gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (inp_G_negVUsed*(GET_LIT_NUMOCCS_USED(lPos)-inp_G_posVUsed)
				+inp_G_posVUsed*(GET_LIT_NUMOCCS_USED(lNeg)-inp_G_negVUsed)	< param_inpGEMaxAONN) {
			//It is time to extract the gate.
			in_intern_ge_extract(v);
		}
	}
}

void in_intern_ge_OR(variable* v){
	//This method will check if the given variable has an OR gate definition.
	//For three variables this looks like this for v:
	//(-v or a or b or c)
	//(v or -a)
	//...
	//(v or -c)
	//v <=> a + b + c >= 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i,j, theSize = INT32_MAX;
	clause *c = NULL;
	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);

	//We reset the gate definition arrays, we are about to need them.
	INP_RESET_GDEFARRAYS();

	//In order to detect such an OR definition, we will first go through all the implications of the negative v and stamp
	//each of the implied literals. After that, we walk through all the occurrences of the negative v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an OR gate.
	//First we stamp all implications of the negative v (by checking the implications of +v).
	++main_litStamp;
	SET_LIT_STAMP(lPos, main_litStamp);
	for (i = 0; i < GET_LIT_NUMIMP_USED(lNeg); i+=2){
		SET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lNeg,i), main_litStamp);
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (v or -a) are now stamped. We now
	//check if any of the clauses that contains -v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an OR gate.
	for (i = 0; i < GET_LIT_NUMOCCS_USED(lNeg); ++i){
		c = GET_LIT_OCCNUM(lNeg, i);
		if (GET_CLS_SIZE(c) < 3 || GET_LIT_NUMIMP_USED(lNeg) < GET_CLS_SIZE(c) - 1) continue;
		//Check if all opposite literals in c are stamped.
		for (j=0; j < GET_CLS_SIZE(c); ++j){
			if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,j)) != main_litStamp){
				//This opposite literal was not stamped. Clause cannot be used.
				break;
			}
		}
		if (j == GET_CLS_SIZE(c)){
			//In case we did not break early, we have found a clause c used in the definition of an AND gate. We store the size
			//of the large clause (the smaller, the better).
			if (GET_CLS_SIZE(c) < theSize){
				//Yes, the new found clause used in the AND definition is smaller than the old one. Since it can be only
				//one clause containing the variable v positively in the AND gate definition, we just override what we had.
				theSize = GET_CLS_SIZE(c);
				inp_G_negV[0] = c;
				inp_G_negVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an OR gate using clause G_negV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++main_litStamp;
		for (j = 0; j < GET_CLS_SIZE(inp_G_negV[0]); ++j){
			SET_LIT_NUM_STAMP(-GET_CLS_LITNUM(inp_G_negV[0],j), main_litStamp);
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < GET_LIT_NUMIMP_USED(lNeg); i+=2){
			if (GET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lNeg, i)) == main_litStamp){
				SET_LIT_NUM_STAMP(GET_LIT_IMPNUM(lNeg, i), main_litStamp -1);
				//Put this clause into the G_negV array.
				INP_ADD_CLS_TO_GPOSV(GET_LIT_IMPNUM_REASON(lNeg, i));
			}
		}
		//We have now collected all the defining clauses for the OR gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (inp_G_negVUsed*(GET_LIT_NUMOCCS_USED(lPos)-inp_G_posVUsed)
				+inp_G_posVUsed*(GET_LIT_NUMOCCS_USED(lNeg)-inp_G_negVUsed)	< param_inpGEMaxAONN) {
			//It is time to extract the gate.
			in_intern_ge_extract(v);
		}
	}
}

void in_intern_ge_SIM3(variable* v){
	//This method extracts similarity gates for exactly three variables. This looks as follows. Given clauses
	//Positive side:   Negative side:
	//X1  (a -b -c)      Y1 (-a  b  c)
	//X2  (a -b  c)      Y2 (-a -b  c)
	//X3  (a  b -c)      Y3 (-a  b -c)
	//This requires a = b = c.
	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);
	clause *c, *conflict = NULL;
	variable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < GET_LIT_NUMOCCS_USED(lPos); ++i){
		c = GET_LIT_OCCNUM(lPos,i);
		if (GET_CLS_SIZE(c) != 3) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++main_litStamp;
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0), main_litStamp);
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1), main_litStamp);
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2), main_litStamp);
		INP_RESET_GDEFARRAYS();
		INP_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 and X3 that have 2 stamped literals and 1 opposite stamped literal.
		for (j = i+1; j < GET_LIT_NUMOCCS_USED(lPos); ++j){
			c = GET_LIT_OCCNUM(lPos,j);
			if (GET_CLS_SIZE(c) != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if (sCountP != 2 || sCountN != 1) continue;
			//It has. We must now check if it actually new for the positive set.
			for (k = 0; k < inp_G_posVUsed; ++k){
				if (GET_CLS_SIG(inp_G_posV[k]) == GET_CLS_SIG(c)) break;
			}
			if (k < inp_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of c. This in turn means that the clauses are the same.
				continue;
			}
			//Yes, the clause is new. We add it to the positive side.
			INP_ADD_CLS_TO_GPOSV(c);
			if (inp_G_posVUsed == 3){
				//We have collected three clauses for the X side. We can continue with the clauses of the Y side.
				break;
			}
		}
		if (inp_G_posVUsed != 3) continue;
		//Three clauses on the positive side were collected. We check if we find three clauses on the negative side.
		for (j = 0; j < GET_LIT_NUMOCCS_USED(lNeg); ++j){
			c = GET_LIT_OCCNUM(lNeg,j);
			if (GET_CLS_SIZE(c) != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 0 && sCountN == 3) || (sCountP == 1 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < inp_G_negVUsed; ++k){
					if (GET_CLS_SIG(inp_G_negV[k]) == GET_CLS_SIG(c)) break;
				}
				if (k < inp_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				INP_ADD_CLS_TO_GNEGV(c);
				if (inp_G_negVUsed == 3){
					//We have collected three clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (inp_G_negVUsed != 3) continue;

		//Here, we have collected the three X and three Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = inp_G_posV[0];
		reWID = GET_VAR_ID(v);
		toRIDa = 0;
		toRIDb = 0;
		if (GET_CLS_LITNUM(c,0) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,0);
			} else {
				toRIDb = -GET_CLS_LITNUM(c,0);
			}
		}
		if (GET_CLS_LITNUM(c,1) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,1);
			} else {
				toRIDb = -GET_CLS_LITNUM(c,1);
			}
		}
		if (GET_CLS_LITNUM(c,2) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,2);
			} else {
				toRIDb = -GET_CLS_LITNUM(c,2);
			}
		}

		//We back-jump in preparation of unit propagation.
		inp_intern_backJump(0,0);

		in_intern_els_replaceLit(main_litData + toRIDa, main_litData + reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		in_intern_els_replaceLit(main_litData - toRIDa, main_litData - reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		//Mark the replaced variable with its representative.
		repVar = main_varData + abs(toRIDa);
		if ((toRIDa < 0 && reWID < 0) || (toRIDa > 0 && reWID > 0)){
			SET_VAR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_VAR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		inp_intern_disableVariable(main_varData + abs(toRIDa));

		in_intern_els_replaceLit(main_litData + toRIDb, main_litData + reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		in_intern_els_replaceLit(main_litData - toRIDb, main_litData - reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		//Mark the replaced variable with its representative.
		repVar = main_varData + abs(toRIDb);
		if ((toRIDb < 0 && reWID < 0) || (toRIDb > 0 && reWID > 0)){
			SET_VAR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_VAR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		inp_intern_disableVariable(main_varData + abs(toRIDb));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		conflict = inp_intern_upPropagate();
		if (conflict != NULL){
			in_returnCode = IN_UNSAT;
			return;
		} else if (f.n_vars_e_used == 0 || (f.m_el_used + f.m_eo_used == 0)){
			in_returnCode = IN_SAT;
			return;
		}

		//We can restart working on v.
		i = 0;
	}
}

void in_intern_ge_SIM4(variable* v){
	//This method extracts similarity gates for exactly four variables. This looks as follows. Given clauses
	//Positive side:   Negative side:
	//X1  (a -b -c -d)      Y1 (-a  b  c  d)
	//X2  (a -b  c  d)      Y2 (-a -b  c  d)
	//X3  (a  b -c  d)      Y3 (-a  b -c  d)
	//X4  (a  b  c -d)      Y4 (-a  b  c -d)
	//X5  (a -b -c  d)      Y5 (-a -b -c  d)
	//X6  (a -b  c -d)      Y6 (-a -b  c -d)
	//X7  (a  b -c -d)      Y7 (-a  b -c -d)
	//This requires a = b = c = d.
	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);
	clause *c, *conflict = NULL;
	variable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, toRIDc, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < GET_LIT_NUMOCCS_USED(lPos); ++i){
		c = GET_LIT_OCCNUM(lPos,i);
		if (GET_CLS_SIZE(c) != 4) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++main_litStamp;
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0), main_litStamp);
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1), main_litStamp);
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2), main_litStamp);
		SET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,3), main_litStamp);
		INP_RESET_GDEFARRAYS();
		INP_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 to X7.
		for (j = i+1; j < GET_LIT_NUMOCCS_USED(lPos); ++j){
			c = GET_LIT_OCCNUM(lPos,j);
			if (GET_CLS_SIZE(c) != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,3)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,3)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 2 && sCountN == 2) || (sCountP == 3 && sCountN == 1)){
				//It has. We must now check if it actually new for the positive set.
				for (k = 0; k < inp_G_posVUsed; ++k){
					if (GET_CLS_SIG(inp_G_posV[k]) == GET_CLS_SIG(c)) break;
				}
				if (k < inp_G_posVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the positive side.
				INP_ADD_CLS_TO_GPOSV(c);
				if (inp_G_posVUsed == 7){
					//We have collected three clauses for the X side. We can continue with the clauses of the Y side.
					break;
				}
			}
		}
		if (inp_G_posVUsed != 7) continue;
		//Three clauses on the positive side were collected. We check if we find three clauses on the negative side.
		for (j = 0; j < GET_LIT_NUMOCCS_USED(lNeg); ++j){
			c = GET_LIT_OCCNUM(lNeg,j);
			if (GET_CLS_SIZE(c) != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,0)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,1)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,2)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(c,3)) == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if (GET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,3)) == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountN == 4 && sCountP == 0) || (sCountP == 1 && sCountN == 3) || (sCountP == 2 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < inp_G_negVUsed; ++k){
					if (GET_CLS_SIG(inp_G_negV[k]) == GET_CLS_SIG(c)) break;
				}
				if (k < inp_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				INP_ADD_CLS_TO_GNEGV(c);
				if (inp_G_negVUsed == 7){
					//We have collected three clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (inp_G_negVUsed != 7) continue;

		//Here, we have collected the three X and three Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = inp_G_posV[0];
		reWID = GET_VAR_ID(v);
		toRIDa = 0;
		toRIDb = 0;
		toRIDc = 0;
		if (GET_CLS_LITNUM(c,0) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,0);
			} else if (toRIDb == 0){
				toRIDb = -GET_CLS_LITNUM(c,0);
			} else {
				toRIDc = -GET_CLS_LITNUM(c,0);
			}
		}
		if (GET_CLS_LITNUM(c,1) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,1);
			} else if (toRIDb == 0){
				toRIDb = -GET_CLS_LITNUM(c,1);
			} else {
				toRIDc = -GET_CLS_LITNUM(c,1);
			}
		}
		if (GET_CLS_LITNUM(c,2) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,2);
			} else if (toRIDb == 0){
				toRIDb = -GET_CLS_LITNUM(c,2);
			} else {
				toRIDc = -GET_CLS_LITNUM(c,2);
			}
		}
		if (GET_CLS_LITNUM(c,3) != reWID) {
			if (toRIDa == 0){
				toRIDa = -GET_CLS_LITNUM(c,3);
			} else if (toRIDb == 0){
				toRIDb = -GET_CLS_LITNUM(c,3);
			} else {
				toRIDc = -GET_CLS_LITNUM(c,3);
			}
		}

		//We back-jump in preparation of unit propagation.
		inp_intern_backJump(0,0);

		in_intern_els_replaceLit(main_litData + toRIDa, main_litData + reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		in_intern_els_replaceLit(main_litData - toRIDa, main_litData - reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		//Mark the replaced variable with its representative.
		repVar = main_varData + abs(toRIDa);
		if ((toRIDa < 0 && reWID < 0) || (toRIDa > 0 && reWID > 0)){
			SET_VAR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_VAR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		inp_intern_disableVariable(main_varData + abs(toRIDa));

		in_intern_els_replaceLit(main_litData + toRIDb, main_litData + reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		in_intern_els_replaceLit(main_litData - toRIDb, main_litData - reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		//Mark the replaced variable with its representative.
		repVar = main_varData + abs(toRIDb);
		if ((toRIDb < 0 && reWID < 0) || (toRIDb > 0 && reWID > 0)){
			SET_VAR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_VAR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		inp_intern_disableVariable(main_varData + abs(toRIDb));

		in_intern_els_replaceLit(main_litData + toRIDc, main_litData + reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		in_intern_els_replaceLit(main_litData - toRIDc, main_litData - reWID);
		if (in_returnCode != IN_UNKNOWN){
			return;
		}

		//Mark the replaced variable with its representative.
		repVar = main_varData + abs(toRIDc);
		if ((toRIDc < 0 && reWID < 0) || (toRIDc > 0 && reWID > 0)){
			SET_VAR_ISEQUIVTO(repVar, abs(reWID));
		} else {
			SET_VAR_ISEQUIVTO(repVar, -abs(reWID));
		}
		//Disable the replaced variable.
		inp_intern_disableVariable(main_varData + abs(toRIDc));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		conflict = inp_intern_upPropagate();
		if (conflict != NULL){
			in_returnCode = IN_UNSAT;
			return;
		} else if (f.n_vars_e_used == 0 || (f.m_el_used + f.m_eo_used == 0)){
			in_returnCode = IN_SAT;
			return;
		}

		//We can restart working on v.
		i = 0;
	}
}

void in_intern_ge_XOR(variable* v){
	//This method will check if the given variable v has an exclusive or gate definition called an XOR gate.
	//If v <=>  = x_1 XOR ... XOR x_w, it must hold that we have at least
	//#N = sum_{k=0}^{floor(w/2)} (\binomial{w,2*k})
	//clauses containing -v (all with an even number of positive literals x_1, ..., x_w). Furthermore, we need at least
	//#P = sum_{k=0}^{floor((w-1)/2)} (\binomial{w,2*k+1})
	//clauses containing +v (all with odd number of positive literals x_1, ..., x_w). All these #N + #P clauses must
	//contain -v or +v and the remainder must contain only +/-x_i. We need at least 2^w many clauses for such a definition.

	literal *lPos = main_litData + GET_VAR_ID(v), *lNeg = main_litData - GET_VAR_ID(v);
	clause *c, *d;
	int32_t i,j,z,k,numNeeded,
		largePosAvail = (GET_LIT_NUMOCCS_USED(lPos) - GET_LIT_NUMIMP_USED(lPos)),
		largeNegAvail = (GET_LIT_NUMOCCS_USED(lNeg) - GET_LIT_NUMIMP_USED(lNeg));

	//We walk through the set of clauses containing -v.
	for (i = 0; i < GET_LIT_NUMOCCS_USED(lNeg); ++i){
		//If the clause is too short, we will ignore it. If v does not have 2^(c->size-1) many occurrences (not binary!),
		//it cannot be in an XOR gate as well. Half of the clauses must be in the positive side, half in the negative side.
		c = GET_LIT_OCCNUM(lNeg,i);
		numNeeded = pow(2,GET_CLS_SIZE(c)-2);

		if (GET_CLS_SIZE(c) < 3 || largeNegAvail - i < numNeeded){
			//Size is wrong or we simply do not have enough clauses left containing v to create a XOR gate out of it.
			continue;
		}

		//Before we go on, we check if the theoretical number of resolvents created would be too large. If so, we continue.
		if (numNeeded*(GET_LIT_NUMOCCS_USED(lPos)-numNeeded) + numNeeded*(GET_LIT_NUMOCCS_USED(lNeg)-numNeeded)
				> param_inpGEMaxXOR) continue;

		//Otherwise, c might be defining in an XOR gate. We will now stamp all the literals in the clause, and stamp them such
		//that it is clear if they  are contained negatively or positively.
		INP_RESET_GDEFARRAYS();
		main_litStamp+=2;
		for (j=0; j < GET_CLS_SIZE(c); ++j){
			//In case the literal is positive in the clause, we stamp it with litStamp, otherwise we stamp it with litStamp-1.
			if (GET_CLS_LITNUM(c,j) > 0){
				SET_LIT_NUM_STAMP( GET_CLS_LITNUM(c,j), main_litStamp);
				SET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,j), main_litStamp);
			} else {
				SET_LIT_NUM_STAMP( GET_CLS_LITNUM(c,j), main_litStamp-1);
				SET_LIT_NUM_STAMP(-GET_CLS_LITNUM(c,j), main_litStamp-1);
			}
		}

		//We add this clause as the first candidate.
		INP_ADD_CLS_TO_GNEGV(c);

		//Now we walk through the remaining negative clauses, and for everyone that has an even number of stamped negated
		//literals and the right size, we check if this clause is new via the signature. If so, we add it to the G_negV array.
		for (j = i+1; j < GET_LIT_NUMOCCS_USED(lNeg); ++j){
			d = GET_LIT_OCCNUM(lNeg,j);
			//If the clause does not have the right size we cannot use it. We need an even number of them to be negated!
			if (GET_CLS_SIZE(d) != GET_CLS_SIZE(c)) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largeNegAvail - j < numNeeded - inp_G_negVUsed) break;

			//We count the number of negated literals in this clause.
			k = 0;
			for (z = 0; z < GET_CLS_SIZE(d); ++z){
				//We ignore the literal for v.
				if (GET_CLS_LITNUM(d,z) == -GET_VAR_ID(v)){
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an even number of such literals.
				if (GET_CLS_LITNUM(d,z) > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) == main_litStamp-1){
						++k;
					} else if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) != main_litStamp){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) == main_litStamp){
						++k;
					} else if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) != main_litStamp-1){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}
			}

			//We now check if the number of counted literals is even. If not, this clause cannot be part of an XOR definition.
			if (z < GET_CLS_SIZE(d) || k%2 != 0) continue;

			//Otherwise we must check if this clause is new for the set of G_negV clauses.
			for (k = 0; k < inp_G_negVUsed; ++k){
				if (GET_CLS_SIG(inp_G_negV[k]) == GET_CLS_SIG(d)) break;
			}
			if (k < inp_G_negVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v negatively.
			INP_ADD_CLS_TO_GNEGV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (inp_G_negVUsed == numNeeded) break;
		}

		//We can continue with the next candidate clause c if we failed to collect enough clauses with -v for the XOR.
		if (inp_G_negVUsed != numNeeded) continue;

		//After having checked the collection of negative clauses, we can go on with the positive ones.
		for (j = 0; j < GET_LIT_NUMOCCS_USED(lPos); ++j){
			d = GET_LIT_OCCNUM(lPos,j);
			//If the clause does not have the right size we cannot use it.
			if (GET_CLS_SIZE(d) != GET_CLS_SIZE(c)) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largePosAvail - j < numNeeded - inp_G_posVUsed) break;

			//We count the number of negated literals in this clause. We need an odd number of them to be negated.
			k = 0;
			for (z=0; z < GET_CLS_SIZE(d); ++z){
				//We ignore the literal for v.
				if (GET_CLS_LITNUM(d,z) == GET_VAR_ID(v)){
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an odd number of such literals.
				if (GET_CLS_LITNUM(d,z) > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) == main_litStamp-1){
						++k;
					} else if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) != main_litStamp){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) == main_litStamp){
						++k;
					} else if (GET_LIT_NUM_STAMP(GET_CLS_LITNUM(d,z)) != main_litStamp-1){
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}
			}
			//We now check if the number of counted literals is odd. If not, this clause cannot be part of an XOR definition.
			if (z < GET_CLS_SIZE(d) || k%2 != 1) continue;

			//Otherwise we must check if this clause is new for the set of G_posV clauses.
			for (k = 0; k < inp_G_posVUsed; ++k){
				if (GET_CLS_SIG(inp_G_posV[k]) == GET_CLS_SIG(d)) break;
			}
			if (k < inp_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v positively.
			INP_ADD_CLS_TO_GPOSV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (inp_G_posVUsed == numNeeded) break;
		}

		//In case we have collected the right number of clauses we can extract the gate.
		if (inp_G_posVUsed == numNeeded && inp_G_negVUsed == numNeeded){
			//Time to extract the gate.
			in_intern_ge_extract(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
	}
}

void in_extern_GE(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_in_component_totalCalls;
	#endif
	#ifdef VERBOSE_IN
	printf("GE");fflush(stdout);
	int32_t initVars = f.n_vars_e_used;
	int32_t initCls = f.m_eo_used + f.m_el_used;
	#endif
	clause *conflict = NULL;
	literal *lPos, *lNeg;
	variable *v;

	while ((v = vQ_dequeueBest()) != NULL){
		//We pick the best outer variables and its inner correspondent.
		lPos = main_litData + GET_VAR_ID(v);
		lNeg = main_litData - GET_VAR_ID(v);

		//We ignore the variable if its number of occurrences is larger than what we allow or if the variable is assigned.
		if (!IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)){
			continue;
		}

		//We check if the literals are pure.
		if (GET_LIT_NUMOCCS_USED(lPos) == 0 || GET_LIT_NUMOCCS_USED(lNeg) == 0){
			//Yes, that is the case. We will now perform the unit propagation in DL0.
			inp_intern_backJump(0,0);
			if (GET_LIT_NUMOCCS_USED(lPos) == 0){
				MAIN_COMPLEXUP_ADD_DEC_PTR(lNeg, v);
			} else {
				MAIN_COMPLEXUP_ADD_DEC_PTR(lPos, v);
			}
			conflict = inp_intern_upPropagate();
			if (conflict != NULL) break;
			continue;
		}

		//We check that the variable is still enabled, and if so, perform GE on it.
		if (!IS_VAR_DISABLED(v)){
			//We perform SIM3 gate extraction with it.
			in_intern_ge_SIM3(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			//We perform SIM4 gate extraction with it.
			in_intern_ge_SIM4(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			//We perform AND gate extraction with it.
			in_intern_ge_AND(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			//We perform OR gate extraction with it.
			in_intern_ge_OR(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			//We perform XOR gate extraction with it.
			in_intern_ge_XOR(v);
			if (in_returnCode != IN_UNKNOWN){
				break;
			}
		}
	}

	if (in_returnCode == IN_UNKNOWN && (f.n_vars_e_used == 0 || f.m_el_used + f.m_eo_used == 0)){
		in_returnCode = IN_SAT;
	}

	#ifdef VERBOSE_IN
	printf("[%d,%d] ", initVars - f.n_vars_e_used, initCls - (int32_t)(f.m_eo_used + f.m_el_used) );
	#endif
	#ifdef COLLECTSTATS
	stats_in_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
