/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "pre.h"

int32_t pre_intern_ACCE_cla(){
	literal *lsame, *lopp;
	clause *d;
	uint64_t sig;
	int32_t *start, *check, restart, litToIgnore = 0, *theLit, lit, i, resolveableFound, *litD;

	//This method performs covered literal addition with all the literals found on the UP queue. This method returns 0 if
	//the addition completed normally, and it returns a literal number if this literal blocks the clause. The start value
	//marks the position from which we start to add CLA literals to the upQueue. If start == upQueue_current, then nothing
	//was added. Whenever something was added, we must again check all the other literals of the clause.

	do {
		restart = 0;
		theLit = main_simpleUPQueue;
		//As long as we have more literals to check, and do not hit litToIgnore, we go on. If we hit litToIgnore we completed
		//a round-trip of all the literals in the clause, checking if any of them adds covered literals but failed for all of
		//them. This means that there is nothing else possible we can do here.
		while (theLit < main_simpleUPQueue_current){
			//We ignore the literal that we worked on the last time, as a second pass that reaches this position means no
			//changes and no changes means we have done all we can for this literal in the last pass.
			if (*theLit == litToIgnore) {
				++theLit;
				continue;
			}
			lsame = main_litData + *theLit;
			lopp = main_litData - *theLit;

			start = main_simpleUPQueue_current;
			++main_litStamp;
			//We want to first locate a valid resolvent (that resolution would result in no tautology). We then add all
			//literals of this clause to the UP queue, that are actually new. If no new literal is available, we can early
			//abort, since the resolution intersection is empty right away.

			//We assume we cannot find a clause to resolve with.
			resolveableFound = 0;
			//First we check for a literal we can add from the implications of lsame. If some literal is found, we can stop
			//the search for a valid resolution partner.
			for (i = 0; i < lsame->numImpUsed; i += 2){
				lit = ((int32_t) (addr_t) lsame->implies[i]);
				if (IS_LIT_IN_SIMPLEUPQUEUE(lit)) {
					//The implication is already in the clause. We could resolve into something non-tautological, but we will
					//not do it here.
					resolveableFound = 1;
					break;
				}
				if (IS_LIT_IN_SIMPLEUPQUEUE(-lit)) {
					//The opposite of the implication is already in the clause. This would result in a tautology.
					continue;
				}
				//Neither the implication nor its opposite are in the clause. We have found something.
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				SET_LIT_STAMP((main_litData + lit), main_litStamp);
				i += 2;
				//Obviously, there is a binary clause that we can resolve with. Therefore, the literal *theLit is not blocking.
				resolveableFound = 1;
				break;
			}

			//If we found something to resolve with but did not add anything to the queue, then the one thing we could have
			//added using a binary clause was already present in the queue. This means we can early abort with the literal.
			if (start == main_simpleUPQueue_current && resolveableFound){
				//In case we found a binary clause resolvable with the current clause, but did not add the implied literal,
				//the literal is already in the clause and the resolution intersection is empty.
				++theLit;
				continue;
			}

			//We can now start to intersect with the literals we have found. As long as the implications found from this point
			//onwards are the added literal, we can continue (the resolution intersection is the one literal). But as soon as
			//we find something else, we can stop.
			for (; i < lsame->numImpUsed; i += 2){
				lit = ((int32_t) (addr_t) lsame->implies[i]);
				if (IS_LIT_IN_SIMPLEUPQUEUE(lit) &&	GET_LIT_STAMP((main_litData + lit)) == main_litStamp) {
					continue;
				}
				//Here, there is a literal that could also be added that is not the one we already have. This means that the
				//resolution intersection is empty. We remove the added literal and go on with the next theLit from the buffer.
				lit = *(--main_simpleUPQueue_current);
				--(main_litData + lit)->upStamp;
				--(main_litData + lit)->stamp;
				break;
			}
			//If the resolution intersection is empty even though we did had a resolution partner, then there was another
			//binary clause that cut out the one literal we were able to add. This means we can stop.
			if (start == main_simpleUPQueue_current && resolveableFound) {
				//We were able to remove the single added literal found in the valid resolvent. Resolution intersection empty.
				++theLit;
				continue;
			}

			//Here we have two possibilities.
			//First, we did not add anything because there was no valid binary partner available. This means that we must use
			//the large clauses to find a valid resolution partner to create the resolution intersection. After this was
			//successful, we must check with the remaining large clauses if we empty the set again.
			//Second, we added something using a binary clause but did not empty the set again.
			//This means that we must use the larger clauses to remove the added literal (can be only one here). We can ignore
			//binary clauses as they have all been checked at this point.

			i = 0;
			if (start == main_simpleUPQueue_current){
				//Case 1, we did not add anything. We go through all the occurrences and ignore clauses of size two. Anything
				//larger is checked if it can be resolved with our current clause.
				for (; i < lopp->numOccsUsed; ++i){
					d = lopp->occs[i];
					if (d->size == 2) {
						continue;
					}
					//Check if a resolution is possible.
					litD = d->lits;
					while (HAS_CLS_MORE_LITS(d,litD)){
						//When checking for opposite literals, we must ignore the literal we resolve on.
						if (*litD != -(*theLit) && IS_LIT_IN_SIMPLEUPQUEUE(-(*litD))){
							//Early abort.
							break;
						}
						++litD;
					}
					//Check.
					if (HAS_CLS_MORE_LITS(d,litD)) {
						//In an early abort, we did not check all the literals. That means the result would be a tautology.
						continue;
					}
					//No early abort. We can copy anything not yet in the clause.
					resolveableFound = 1;
					litD = d->lits;
					while (HAS_CLS_MORE_LITS(d,litD)){
						//If we already have this literal, we do not need to add it again.
						if (*litD == -(*theLit) || IS_LIT_IN_SIMPLEUPQUEUE(*litD)){
							//Yes, old stuff or the literal we resolve on and that we ignore.
							++litD;
							continue;
						}
						//Nope, the literal is new.
						ADD_LIT_TO_SIMPLEUPQUEUE(*litD);
						SET_LIT_STAMP((main_litData + *litD), main_litStamp);
						++litD;
					}

					//We can stop, independent of whether we added something or not.
					++i;
					break;
				}
			}

			if (start == main_simpleUPQueue_current && resolveableFound) {
				//In case we found a valid resolution partner, but did not add any of its literals, the resolution
				//intersection must be empty. We can stop working on this literal.
				++theLit;
				continue;
			}

			//Independent of whether we added stuff with implications or large clauses, we must now try to empty the set, if
			//indeed something was added -- thats why we have the start < upQueue_current check in the for loop header.
			for (; i < lopp->numOccsUsed && start < main_simpleUPQueue_current && resolveableFound; ++i){
				d = lopp->occs[i];
				//Ignore the clause if it is binary. We cannot use binary clauses to cut stuff, the binary clauses had their
				//chance in the beginning.
				if (d->size == 2) {
					continue;
				}
				//Check if a resolution is possible.
				litD = d->lits;
				while (HAS_CLS_MORE_LITS(d,litD)){
					//When checking for opposite literals, we must ignore the literal we resolve on.
					if (*litD != -(*theLit) && IS_LIT_IN_SIMPLEUPQUEUE(-(*litD))
							&& GET_LIT_STAMP((main_litData - *litD)) != main_litStamp
							){
						//Early abort.
						break;
					}
					++litD;
				}
				//Check.
				if (HAS_CLS_MORE_LITS(d,litD)) {
					//In an early abort, we did not check all the literals. That means the result would be a tautology.
					continue;
				}
				//No early abort. We can use the clauses literals to remove the formerly added ones.
				check = start;
				//We grab one added literal after another, and see if they are found in d. If not, we remove them.
				while (check < main_simpleUPQueue_current){
					sig = GET_LIT_SIG(*check);
					//First check if the signature bit for *check is set in d.
					if ((sig & d->sig) == 0ULL){
						//The literal *check cannot be part of clause d, so it can be removed. We override it with the last
						//element in the upQueue, which must be an added literal too.
						--(main_litData + *check)->upStamp;
						--(main_litData + *check)->stamp;
						*check = *(--main_simpleUPQueue_current);
						//We do not increment check, since a new literal took this specific position.
						continue;
					}
					//Yes the bit is set, so it could be that the literal is found in d.
					litD = d->lits;
					while (HAS_CLS_MORE_LITS(d,litD)){
						if (*litD == *check){
							//Break early.
							break;
						}
						++litD;
					}
					//An early break means we have found the literal.
					if (!HAS_CLS_MORE_LITS(d,litD)){
						//Nope we did not break early, the literal was not found. We can remove it.
						--(main_litData + *check)->upStamp;
						--(main_litData + *check)->stamp;
						*check = *(--main_simpleUPQueue_current);
						//We do not increment check, since a new literal took this specific position.
						continue;
					}
					++check;
				}
			}

			//We must check the result of the resolution intersection computation. Check if a possible resolution partner
			//was found (even if it did not add anything to the clause).
			if (!resolveableFound){
				//This is not the case, the clause is blocked via *theLit, as all possible resolutions are tautologies.
				return *theLit;
			}

			//It seems that resolutions were possible. So the clause is not yet blocked by *theLit. We must check if anything
			//was added and survived the removals. If so, we add those literals to the covLits array as covered literals.
			check = start;
			while (check < main_simpleUPQueue_current){
				pre_covLits[pre_covLitsUsed].lit = *(check++);
				pre_covLits[pre_covLitsUsed++].coveredBy = *theLit;
			}

			if (start < main_simpleUPQueue_current){
				//We were able to add something to the clause using theLit. We perform another pass of the CLA, hoping that
				//previous literals might add something now (more literals mean more possible tautologies, and they rule out
				//clauses that might have been used for literal removal earlier on).
				litToIgnore = *theLit;
				restart = 1;
			}
			++theLit;
		}
	} while (restart);
	//We return 0 if the clause is not yet blocked.
	return 0;
}

void pre_intern_GE_extract(variable *v){
	int32_t i,j,k, *litC, *litD, ignoreLit;
	clause *c, *d;
	literal *lpos = main_litData + v->id, *lneg = main_litData - v->id;
	//In case variable v has a definition provided in the sets G_-v and G_v, we can remove the variable from the formula by
	//performing the following steps.
	//		1. Disable the clauses defining the gate.
	//		2. Creating the resolvents of set R_v and G_-v, as well as R_-v and G_v (resolve the remaining clauses).
	//		3. Making all the clauses participating in the gate definition removed (v/-v being the litOfInterest).
	//		4. Disabling all remaining clauses that contain the defined variable in R_v and R_-v.
	//		5. Disable the variable v.
	//		6. Add the resolvents to the formula.

	#ifdef VERBOSE_PRE
	int32_t count = 0 - ((int32_t)lpos->numOccsUsed) - ((int32_t)lneg->numOccsUsed);
	printf("%dN%dP%dA", v->id, (int32_t)lpos->numOccsUsed, (int32_t)lneg->numOccsUsed);fflush(stdout);
	#endif

	//1. We have identified a gate, and will first make the clauses defining it disabled. This way they do not appear in any
	//occurrence or watcher lists or binary implication lists anymore, but their literal arrays are still available.
	for (i = 0; i < pre_G_negVUsed; ++i){
		clauses_disable(pre_G_negV[i]);
	}
	for (i = 0; i < pre_G_posVUsed; ++i){
		clauses_disable(pre_G_posV[i]);
	}
	//2. We must now create all the resolvents, one set is G_-v and R_v (the remaining other clauses containing v), the other
	//set is G_v and R_-v (the remaining other clauses containing -v). Since we already disabled the clauses defining the
	//gate, all the remaining occurrences belong to either R_v or R_-v. We will pick one element from G_negV/G_posV after
	//another and resolve them with all remaining occurrences.

	PRE_RESET_ALL_RESOLVENTS();

	//Resolve G_-v with R_v.
	for (i = 0; i < pre_G_negVUsed; ++i){
		c = pre_G_negV[i];
		for (j = 0; j < lpos->numOccsUsed; ++j){
			d = lpos->occs[j];
			//We now prepare the copying operation. The literal we resolve on is -ignoreLit in c and ignoreLit in d.
			PRE_RESET_RESOLVENT(pre_resUsed);
			++main_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			litC = c->lits;
			ignoreLit = -v->id;
			while (HAS_CLS_MORE_LITS(c, litC)){
				if (*litC != ignoreLit){
					//We add the literal.
					PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litC);
					//We stamp the literal.
					(main_litData + *litC)->stamp = main_litStamp;
				}
				++litC;
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			litD = d->lits;
			while (HAS_CLS_MORE_LITS(d, litD)){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (*litD != ignoreLit && (main_litData + *litD)->stamp != main_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it. If so, we break.
					if ((main_litData - *litD)->stamp == main_litStamp){
						//We break early.
						break;
					}
					//No tautology yet.
					PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litD);
					(main_litData + *litD)->stamp = main_litStamp;
				}
				++litD;
			}
			if (HAS_CLS_MORE_LITS(d, litD)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			PRE_INCREASE_NUM_RESOLVENTS( k );
		}
	}

	//Resolve G_v with R_-v.
	for (i = 0; i < pre_G_posVUsed; ++i){
		c = pre_G_posV[i];
		for (j = 0; j < lneg->numOccsUsed; ++j){
			d = lneg->occs[j];
			//We now prepare the copying operation. The literal we resolve on is ignoreLit in c and -ignoreLit in d.
			PRE_RESET_RESOLVENT(pre_resUsed);
			++main_litStamp;
			//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
			litC = c->lits;
			ignoreLit = v->id;
			while (HAS_CLS_MORE_LITS(c, litC)){
				if (*litC != ignoreLit){
					//We add the literal.
					PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litC);
					//We stamp the literal.
					(main_litData + *litC)->stamp = main_litStamp;
				}
				++litC;
			}
			//We now copy all the literals from d, we invert the sign of ignoreLit for this, as this is what we ignore in d.
			ignoreLit = -ignoreLit;
			litD = d->lits;
			while (HAS_CLS_MORE_LITS(d, litD)){
				//We ignore the resolution literal and any literal already in the resolvent.
				if (*litD != ignoreLit && (main_litData + *litD)->stamp != main_litStamp){
					//We check if adding this literal will result in a tautology. If not, we add it. If so, we break.
					if ((main_litData - *litD)->stamp == main_litStamp){
						//We break early.
						break;
					}
					//No tautology yet.
					PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litD);
					(main_litData + *litD)->stamp = main_litStamp;
				}
				++litD;
			}
			if (HAS_CLS_MORE_LITS(d, litD)){
				//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
				continue;
			}
			//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
			PRE_INCREASE_NUM_RESOLVENTS( k );
		}
	}
	//We have now created all the resolvents.

	//3. Make the clauses defining the gate removed using v->id and -v->id as the literal of interest.
	for (i = 0; i < pre_G_negVUsed; ++i){
		prep_intern_makeClauseRemoved(pre_G_negV[i], -v->id, PREP_REMCLS_GE, 0, NULL);
	}
	for (i = 0; i < pre_G_posVUsed; ++i){
		prep_intern_makeClauseRemoved(pre_G_posV[i],  v->id, PREP_REMCLS_GE, 0, NULL);
	}

	//4. Disable all remaining clauses that contain the defined variable in R_v and R_-v.
	while (lpos->numOccsUsed > 0){
		prep_intern_abandonClause(lpos->occs[0]);
	}
	while (lneg->numOccsUsed > 0){
		prep_intern_abandonClause(lneg->occs[0]);
	}

	//5. Disable variable v.
	prep_intern_disableVariable(v);

	//6. Add the created resolvents.
	for (i = 0; i < pre_resUsed; ++i){
		if (pre_resMemUsed[i] == 1){
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(pre_res[i][0])){
				pre_returnCode = PRE_UNSAT;
				return;
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE(pre_res[i][0])){
				ADD_LIT_TO_SIMPLEUPQUEUE(pre_res[i][0])
			}
		} else {
			prep_intern_addOriginalClause(pre_res[i], pre_resMemUsed[i]);
		}
	}

	if (!IS_SIMPLEUPQUEUE_EMPTY()){
		#ifdef VERBOSE_PRE
		printf("<U>");
		#endif
		c = main_simpleUP_propagate_prep();
		if (c != NULL){
			pre_returnCode = PRE_UNSAT;
		}
	}

	#ifdef VERBOSE_PRE
	count += pre_resUsed;
	printf("%dC%d ", pre_resUsed, count);
	#endif
}

void pre_intern_GE_AND(variable *v){
	//This method will check if the given variable has an AND gate definition.
	//For three variables this looks like this for v:
	//(v or -a or -b or -c)
	//(-v or a)
	//...
	//(-v or c)
	//v <=> a = b = c = 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i, *litC, theSize = INT32_MAX;
	clause *c = NULL;
	literal *lpos = main_litData + v->id;

	//We reset the gate definition arrays, we are about to need them.
	PRE_RESET_GDEFARRAYS();

	//In order to detect such an AND definition, we will first go through all the implications of the positive v, stamp
	//each of the implied literals. After that, we walk through all the occurrences of the positive v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an AND gate.
	//First, we stamp all implications of the positive v (by checking the implications of -v).
	++main_litStamp;
	(main_litData - v->id)->stamp = main_litStamp;
	for (i = 0; i < lpos->numImpUsed; i+=2){
		(main_litData + ((int32_t) (addr_t) lpos->implies[i]))->stamp = main_litStamp;
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (-v or a) are now stamped. We now
	//check if any of the clauses that contains +v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an AND gate.
	for (i = 0; i < lpos->numOccsUsed; ++i){
		c = lpos->occs[i];
		if (c->size < 3 || lpos->numImpUsed < c->size - 1) continue;
		litC = c->lits;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if ((main_litData - *litC)->stamp != main_litStamp){
				//Break early. Clause c cannot be part of an AND definition, because this literal was not stamped opposite.
				break;
			}
			++litC;
		}
		if (!HAS_CLS_MORE_LITS(c, litC)){
			//In case we did not break early, we have found a clause c used in the definition of an AND gate. We store the size
			//of the large clause (the smaller, the better).
			if (c->size < theSize){
				//Yes, the new found clause used in the AND definition is smaller than the old one. Since it can be only
				//one clause containing the variable v positively in the AND gate definition, we just override what we had.
				theSize = c->size;
				pre_G_posV[0] = c;
				pre_G_posVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an AND gate using clause G_posV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++main_litStamp;
		litC = pre_G_posV[0]->lits;
		while (HAS_CLS_MORE_LITS(pre_G_posV[0], litC)){
			(main_litData - *litC)->stamp = main_litStamp;
			++litC;
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < lpos->numImpUsed; i+=2){
			if ((main_litData + ((int32_t) (addr_t) (lpos->implies[i])))->stamp == main_litStamp){
				--(main_litData + ((int32_t) (addr_t) (lpos->implies[i])))->stamp;
				//Put this clause into the G_negV array.
				PRE_ADD_CLS_TO_GNEGV(lpos->implies[i+1]);
			}
		}

		//We have now collected all the defining clauses for the AND gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (pre_G_negVUsed * (lpos->numOccsUsed - pre_G_posVUsed)
				+   pre_G_posVUsed*((main_litData - v->id)->numOccsUsed - pre_G_negVUsed)
			< param_prepGEMaxAONN) {
			//It is time to extract the gate.
			#ifdef VERBOSE_PRE
			printf("AND:");
			#endif
			pre_intern_GE_extract(v);
			if (pre_returnCode != PRE_UNKNOWN){
				return;
			}
		}
	}

}

void pre_intern_GE_OR(variable *v){
	//This method will check if the given variable has an OR gate definition.
	//For three variables this looks like this for v:
	//(-v or a or b or c)
	//(v or -a)
	//...
	//(v or -c)
	//v <=> a + b + c >= 1. If so, we will will remove the variable by calling prepGE_extract(v).

	int32_t i, *litC, theSize = INT32_MAX;
	clause *c = NULL;
	literal *lneg = main_litData - v->id;

	//We reset the gate definition arrays, we are about to need them.
	PRE_RESET_GDEFARRAYS();

	//In order to detect such an OR definition, we will first go through all the implications of the negative v and stamp
	//each of the implied literals. After that, we walk through all the occurrences of the negative v, ignoring binaries.
	//For each of these clauses, we walk through the literals and check if all of them are stamped. If so, we have found a
	//large clause and a set of binaries that are defining an OR gate.
	//First we stamp all implications of the negative v (by checking the implications of +v).
	++main_litStamp;
	(main_litData + v->id)->stamp = main_litStamp;
	for (i = 0; i < lneg->numImpUsed; i+=2){
		(main_litData + ((int32_t) (addr_t) lneg->implies[i]))->stamp = main_litStamp;
	}
	//Now all the literals implied by -v are stamped, that is all literals a in clauses like (v or -a) are now stamped. We now
	//check if any of the clauses that contains -v has all its opposite literals stamped. If so, this clause and the binaries
	//that had the literal stamped are defining an OR gate.
	for (i = 0; i < lneg->numOccsUsed; ++i){
		c = lneg->occs[i];
		if (c->size < 3 || lneg->numImpUsed < c->size - 1) continue;
		litC = c->lits;
		while (HAS_CLS_MORE_LITS(c, litC)){
			if ((main_litData - *litC)->stamp != main_litStamp){
				//Break early. Clause c cannot be part of an OR definition, because this literal was not stamped opposite.
				break;
			}
			++litC;
		}
		if (!HAS_CLS_MORE_LITS(c, litC)){
			//In case we did not break early, we have found a clause c used in the definition of an OR gate. We store the size
			//of the large clause (the smaller, the better).
			if (c->size < theSize){
				//Yes, the new found clause used in the OR definition is smaller than the old one. Since it can be only
				//one clause containing the variable v negatively in the OR gate definition, we just override what we had.
				theSize = c->size;
				pre_G_negV[0] = c;
				pre_G_negVUsed = 1;
			}
		}
	}

	if (theSize != INT32_MAX){
		//Yes, we identified a variable defined in an OR gate using clause G_negV[0] as large clause. Remember, do not use
		//c here, as this was just used earlier on to read through all the clauses. We must now collect all the necessary
		//implications. First, we stamp all the literals in the clause again.
		++main_litStamp;
		litC = pre_G_negV[0]->lits;
		while (HAS_CLS_MORE_LITS(pre_G_negV[0], litC)){
			(main_litData - *litC)->stamp = main_litStamp;
			++litC;
		}
		//Then we go through the implications and collect each clause once (stamp must be reduced!).
		for (i = 0; i < lneg->numImpUsed; i+=2){
			if ((main_litData + ((int32_t) (addr_t) (lneg->implies[i])))->stamp == main_litStamp){
				--(main_litData + ((int32_t) (addr_t) (lneg->implies[i])))->stamp;
				//Put this clause into the G_posV array.
				PRE_ADD_CLS_TO_GPOSV(lneg->implies[i+1]);
			}
		}

		//We have now collected all the defining clauses for the OR gate of variable v. Check the resolvents. We want
		//the gate to be extracted if the number of resolvents is small enough.
		if (pre_G_negVUsed * ((main_litData + v->id)->numOccsUsed - pre_G_posVUsed)
				+ pre_G_posVUsed * (lneg->numOccsUsed - pre_G_negVUsed)
			< param_prepGEMaxAONN){
			#ifdef VERBOSE_PRE
			printf("OR:");
			#endif
			pre_intern_GE_extract(v);
			if (pre_returnCode != PRE_UNKNOWN){
				return;
			}
		}
	}
}

void pre_intern_GE_XOR(variable *v){
	//This method will check if the given variable v has an exclusive or gate definition called an XOR gate.
	//If v <=>  = x_1 XOR ... XOR x_w, it must hold that we have at least
	//#N = sum_{k=0}^{floor(w/2)} (\binomial{w,2*k})
	//clauses containing -v (all with an even number of positive literals x_1, ..., x_w). Furthermore, we need at least
	//#P = sum_{k=0}^{floor((w-1)/2)} (\binomial{w,2*k+1})
	//clauses containing +v (all with odd number of positive literals x_1, ..., x_w). All these #N + #P clauses must
	//contain -v or +v and the remainder must contain only +/-x_i. We need at least 2^w many clauses for such a definition.

	literal *lpos = main_litData + v->id, *lneg = main_litData - v->id;
	clause *c, *d;
	int32_t i,j,k, *litC, numNeeded,
		largePosAvail = (lpos->numOccsUsed - lpos->numImpUsed),
		largeNegAvail = (lneg->numOccsUsed - lneg->numImpUsed);

	//We walk through the set of clauses containing -v.
	for (i = 0; i < lneg->numOccsUsed; ++i){
		//If the clause is too short, we will ignore it. If v does not have 2^(c->size-1) many occurrences (not binary!),
		//it cannot be in an XOR gate as well. Half of the clauses must be in the positive side, half in the negative side.
		c = lneg->occs[i];
		numNeeded = pow(2,c->size-2);

		if (c->size < 3 || largeNegAvail - i < numNeeded){
			//Size is wrong or we simply do not have enough clauses left containing v to create a XOR gate out of it.
			continue;
		}

		//Before we go on, we check if the theoretical number of resolvents created would be too large. If so, we continue.
		if (numNeeded*((lpos->numOccsUsed)-numNeeded) + numNeeded*((lneg->numOccsUsed)-numNeeded)
				> param_prepGEMaxXOR) continue;

		//Otherwise, c might be defining in an XOR gate. We will now stamp all the literals in the clause, and stamp them such
		//that it is clear if they  are contained negatively or positively.
		PRE_RESET_GDEFARRAYS();
		main_litStamp+=2;
		litC = c->lits;
		while (HAS_CLS_MORE_LITS(c, litC)){
			//In case the literal is positive in the clause, we stamp it with litStamp, otherwise we stamp it with litStamp-1.
			if (*litC > 0){
				(main_litData + *litC)->stamp = main_litStamp;
				(main_litData - *litC)->stamp = main_litStamp;
			} else {
				(main_litData + *litC)->stamp = main_litStamp-1;
				(main_litData - *litC)->stamp = main_litStamp-1;
			}
			++litC;
		}
		//We add this clause as the first candidate.
		PRE_ADD_CLS_TO_GNEGV(c);

		//Now we walk through the remaining negative clauses, and for everyone that has an even number of stamped negated
		//literals and the right size, we check if this clause is new via the signature. If so, we add it to the G_negV array.
		for (j = i+1; j < lneg->numOccsUsed; ++j){
			d = lneg->occs[j];
			//If the clause does not have the right size we cannot use it. We need an even number of them to be negated!
			if (d->size != c->size) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largeNegAvail - j < numNeeded - pre_G_negVUsed) break;

			//We count the number of negated literals in this clause.
			litC = d->lits;
			k = 0;
			while (HAS_CLS_MORE_LITS(d, litC)){
				//We ignore the literal for v.
				if (*litC == -v->id) {
					++litC;
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an even number of such literals.
				if (*litC > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if ((main_litData + *litC)->stamp == main_litStamp-1){
						++k;
					} else if ((main_litData + *litC)->stamp != main_litStamp) {
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if ((main_litData + *litC)->stamp == main_litStamp){
						++k;
					} else if ((main_litData + *litC)->stamp != main_litStamp-1) {
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}

				++litC;
			}
			//We now check if the number of counted literals is even. If not, this clause cannot be part of an XOR definition.
			if (HAS_CLS_MORE_LITS(d,litC) || k%2 != 0) continue;

			//Otherwise we must check if this clause is new for the set of G_negV clauses.
			for (k = 0; k < pre_G_negVUsed; ++k){
				if (pre_G_negV[k]->sig == d->sig) break;
			}
			if (k < pre_G_negVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v negatively.
			PRE_ADD_CLS_TO_GNEGV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (pre_G_negVUsed == numNeeded) break;
		}

		//We can continue with the next candidate clause c if we failed to collect enough clauses with -v for the XOR.
		if (pre_G_negVUsed != numNeeded) continue;

		//After having checked the collection of negative clauses, we can go on with the positive ones.
		for (j = 0; j < lpos->numOccsUsed; ++j){
			d = lpos->occs[j];
			//If the clause does not have the right size we cannot use it.
			if (d->size != c->size) continue;

			//If what we have plus what we could get is not enough to collect enough clauses for an XOR gate with +v, we
			//can stop.
			if (largePosAvail - j < numNeeded - pre_G_posVUsed) break;

			//We count the number of negated literals in this clause. We need an odd number of them to be negated.
			litC = d->lits;
			k = 0;
			while (HAS_CLS_MORE_LITS(d, litC)){
				//We ignore the literal for v.
				if (*litC == v->id) {
					++litC;
					continue;
				}
				//Now we check if the literals are stamped, and we count the number of those that are stamped with the
				//opposite sign. There must be an odd number of such literals.
				if (*litC > 0){
					//If the literal is positive in d it must be negative in c for it to get counted.
					if ((main_litData + *litC)->stamp == main_litStamp-1){
						++k;
					} else if ((main_litData + *litC)->stamp != main_litStamp) {
						//The literal in d is not stamped at all. Early break.
						break;
					}
				} else {
					//If the literal is negative in d it must be positive in c for it to get counted.
					if ((main_litData + *litC)->stamp == main_litStamp){
						++k;
					} else if ((main_litData + *litC)->stamp != main_litStamp-1) {
						//The literal in d is not stamped at all. Early break.
						break;
					}
				}

				++litC;
			}
			//We now check if the number of counted literals is odd. If not, this clause cannot be part of an XOR definition.
			if (HAS_CLS_MORE_LITS(d,litC) || k%2 != 1) continue;

			//Otherwise we must check if this clause is new for the set of G_posV clauses.
			for (k = 0; k < pre_G_posVUsed; ++k){
				if (pre_G_posV[k]->sig == d->sig) break;
			}
			if (k < pre_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of d. This in turn means that the clauses are the same.
				continue;
			}
			//Otherwise we can add the new clause containing v positively.
			PRE_ADD_CLS_TO_GPOSV(d);
			//We can stop if we have collected the necessary number of clauses.
			if (pre_G_posVUsed == numNeeded) break;
		}

		//In case we have collected the right number of clauses we can extract the gate.
		if (pre_G_posVUsed == numNeeded && pre_G_negVUsed == numNeeded){
			//Time to extract the gate.
			#ifdef VERBOSE_PRE
			printf("XOR:");
			#endif
			pre_intern_GE_extract(v);
			if (pre_returnCode != PRE_UNKNOWN){
				return;
			}
		}
	}
}

void pre_intern_GE_SIM3(variable *v){
	//This method extracts similarity gates for exactly three variables. This looks as follows. Given clauses
	//Positive side:   Negative side:
	//X1  (a -b -c)      Y1 (-a  b  c)
	//X2  (a -b  c)      Y2 (-a -b  c)
	//X3  (a  b -c)      Y3 (-a  b -c)
	//This requires a = b = c.
	literal *lPos = main_litData + v->id, *lNeg = main_litData - v->id;
	clause *c;
	variable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < lPos->numOccsUsed; ++i){
		c = lPos->occs[i];
		if (c->size != 3) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++main_litStamp;
		(main_litData + c->lits[0])->stamp = main_litStamp;
		(main_litData + c->lits[1])->stamp = main_litStamp;
		(main_litData + c->lits[2])->stamp = main_litStamp;
		PRE_RESET_GDEFARRAYS();
		PRE_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 and X3 that have 2 stamped literals and 1 opposite stamped literal.
		for (j = i+1; j < lPos->numOccsUsed; ++j){
			c = lPos->occs[j];
			if (c->size != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if ((main_litData + c->lits[0])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[0])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[1])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[1])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[2])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[2])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if (sCountP != 2 || sCountN != 1) continue;
			//It has. We must now check if it actually new for the positive set.
			for (k = 0; k < pre_G_posVUsed; ++k){
				if (pre_G_posV[k]->sig == c->sig) break;
			}
			if (k < pre_G_posVUsed){
				//We did not check against all the clauses we already stored. This means that for at least one of them, the
				//signature was equal to the signature of c. This in turn means that the clauses are the same.
				continue;
			}
			//Yes, the clause is new. We add it to the positive side.
			PRE_ADD_CLS_TO_GPOSV(c);
			if (pre_G_posVUsed == 3){
				//We have collected three clauses for the X side. We can continue with the clauses of the Y side.
				break;
			}
		}
		if (pre_G_posVUsed != 3) continue;
		//Three clauses on the positive side were collected. We check if we find three clauses on the negative side.
		for (j = 0; j < lNeg->numOccsUsed; ++j){
			c = lNeg->occs[j];
			if (c->size != 3) continue;
			//Here, we have found another 3 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if ((main_litData + c->lits[0])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[0])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[1])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[1])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[2])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[2])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 0 && sCountN == 3) || (sCountP == 1 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < pre_G_negVUsed; ++k){
					if (pre_G_negV[k]->sig == c->sig) break;
				}
				if (k < pre_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				PRE_ADD_CLS_TO_GNEGV(c);
				if (pre_G_negVUsed == 3){
					//We have collected three clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (pre_G_negVUsed != 3) continue;

		//Here, we have collected the three X and three Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = pre_G_posV[0];
		reWID = v->id;
		toRIDa = 0;
		toRIDb = 0;
		if (c->lits[0] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[0];
			} else {
				toRIDb = -c->lits[0];
			}
		}
		if (c->lits[1] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[1];
			} else {
				toRIDb = -c->lits[1];
			}
		}
		if (c->lits[2] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[2];
			} else {
				toRIDb = -c->lits[2];
			}
		}

		#ifdef VERBOSE_PRE
		printf("SIM3:%d ",reWID);
		#endif

		pre_intern_ELS_replaceLit(main_litData + toRIDa, main_litData + reWID);
		if (pre_returnCode == PRE_UNSAT){
			return;
		}

		pre_intern_ELS_replaceLit(main_litData - toRIDa, main_litData - reWID);
		if (pre_returnCode == PRE_UNSAT){
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
		prep_intern_disableVariable(main_varData + abs(toRIDa));

		pre_intern_ELS_replaceLit(main_litData + toRIDb, main_litData + reWID);
		if (pre_returnCode == PRE_UNSAT){
			return;
		}

		pre_intern_ELS_replaceLit(main_litData - toRIDb, main_litData - reWID);
		if (pre_returnCode == PRE_UNSAT){
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
		prep_intern_disableVariable(main_varData + abs(toRIDb));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		if (!IS_SIMPLEUPQUEUE_EMPTY()) return;

		//We can restart working on v.
		i = 0;
	}
}

void pre_intern_GE_SIM4(variable *v){
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
	literal *lPos = main_litData + v->id, *lNeg = main_litData - v->id;
	clause *c;
	variable *repVar;
	uint32_t sCountP, sCountN, i, j, k;
	int32_t toRIDa, toRIDb, toRIDc, reWID;

	//We now take a look at all clauses of size 3 containing lPos.
	for (i = 0; i < lPos->numOccsUsed; ++i){
		c = lPos->occs[i];
		if (c->size != 4) continue;
		//We have now found a clause c that will stand for X1. We now stamp all literals in c.
		++main_litStamp;
		(main_litData + c->lits[0])->stamp = main_litStamp;
		(main_litData + c->lits[1])->stamp = main_litStamp;
		(main_litData + c->lits[2])->stamp = main_litStamp;
		(main_litData + c->lits[3])->stamp = main_litStamp;
		PRE_RESET_GDEFARRAYS();
		PRE_ADD_CLS_TO_GPOSV(c);

		//We must now find clauses X2 ... X7.
		for (j = i+1; j < lPos->numOccsUsed; ++j){
			c = lPos->occs[j];
			if (c->size != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if ((main_litData + c->lits[0])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[0])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[1])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[1])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[2])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[2])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[3])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[3])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountP == 2 && sCountN == 2) || (sCountP == 3 && sCountN == 1)){
				//It has. We must now check if it actually new for the positive set.
				for (k = 0; k < pre_G_posVUsed; ++k){
					if (pre_G_posV[k]->sig == c->sig) break;
				}
				if (k < pre_G_posVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the positive side.
				PRE_ADD_CLS_TO_GPOSV(c);
				if (pre_G_posVUsed == 7){
					//We have collected the clauses for the X side. We can continue with the clauses of the Y side.
					break;
				}
			}
		}
		if (pre_G_posVUsed != 7) continue;
		//Three clauses on the positive side were collected. We check if we find the clauses on the negative side.
		for (j = 0; j < lNeg->numOccsUsed; ++j){
			c = lNeg->occs[j];
			if (c->size != 4) continue;
			//Here, we have found another 4 clause that might serve. We check its stamps.
			sCountP = 0;
			sCountN = 0;
			if ((main_litData + c->lits[0])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[0])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[1])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[1])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[2])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[2])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			if ((main_litData + c->lits[3])->stamp == main_litStamp){
				//The literal itself is stamped.
				++sCountP;
			} else if ((main_litData - c->lits[3])->stamp == main_litStamp){
				//The opposite literal is stamped.
				++sCountN;
			} else {
				//None of the literals is stamped. We can continue.
				continue;
			}
			//We now check if the clause has the correct number of stamped literals.
			if ((sCountN == 4 && sCountP == 0) || (sCountP == 1 && sCountN == 3) || (sCountP == 2 && sCountN == 2)) {
				//In this case, we have found a clause from the Y side. We check if the clause is actually new.
				for (k = 0; k < pre_G_negVUsed; ++k){
					if (pre_G_negV[k]->sig == c->sig) break;
				}
				if (k < pre_G_negVUsed){
					//We did not check against all the clauses we already stored. This means that for at least one of them, the
					//signature was equal to the signature of c. This in turn means that the clauses are the same.
					continue;
				}
				//Yes, the clause is new. We add it to the negative side.
				PRE_ADD_CLS_TO_GNEGV(c);
				if (pre_G_negVUsed == 7){
					//We have collected the clauses for the Y side. We are done.
					break;
				}
			}
		}
		if (pre_G_negVUsed != 7) continue;

		//Here, we have collected the the X and the Y clauses, that define literals to be equivalent. We walk through the
		//first positive clause and replace all the literals that are not v->id with v->id, but invert their sign.
		c = pre_G_posV[0];
		reWID = v->id;
		toRIDa = 0;
		toRIDb = 0;
		toRIDc = 0;
		if (c->lits[0] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[0];
			} else if (toRIDb == 0){
				toRIDb = -c->lits[0];
			} else {
				toRIDc = -c->lits[0];
			}
		}
		if (c->lits[1] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[1];
			} else if (toRIDb == 0){
				toRIDb = -c->lits[1];
			} else {
				toRIDc = -c->lits[1];
			}
		}
		if (c->lits[2] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[2];
			} else if (toRIDb == 0){
				toRIDb = -c->lits[2];
			} else {
				toRIDc = -c->lits[2];
			}
		}
		if (c->lits[3] != reWID) {
			if (toRIDa == 0){
				toRIDa = -c->lits[3];
			} else if (toRIDb == 0){
				toRIDb = -c->lits[3];
			} else {
				toRIDc = -c->lits[3];
			}
		}
		#ifdef VERBOSE_PRE
		printf("SIM4:%d ",reWID);
		#endif

		pre_intern_ELS_replaceLit(main_litData + toRIDa, main_litData + reWID);
		if (pre_returnCode == PRE_UNSAT){
			return;
		}

		pre_intern_ELS_replaceLit(main_litData - toRIDa, main_litData - reWID);
		if (pre_returnCode == PRE_UNSAT){
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
		prep_intern_disableVariable(main_varData + abs(toRIDa));

		pre_intern_ELS_replaceLit(main_litData + toRIDb, main_litData + reWID);
		if (pre_returnCode == PRE_UNSAT){
			return;
		}

		pre_intern_ELS_replaceLit(main_litData - toRIDb, main_litData - reWID);
		if (pre_returnCode == PRE_UNSAT){
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
		prep_intern_disableVariable(main_varData + abs(toRIDb));

		pre_intern_ELS_replaceLit(main_litData + toRIDc, main_litData + reWID);
		if (pre_returnCode == PRE_UNSAT){
			return;
		}

		pre_intern_ELS_replaceLit(main_litData - toRIDc, main_litData - reWID);
		if (pre_returnCode == PRE_UNSAT){
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
		prep_intern_disableVariable(main_varData + abs(toRIDc));

		//We now check if any unit propagation resulted from this. If so, we return right away.
		if (!IS_SIMPLEUPQUEUE_EMPTY()) return;

		//We can restart working on v.
		i = 0;
	}
}

void pre_extern_printVersion(){
	printf("[Preprocessing   ] [Preprocessor                   ] :: %3d.%-4d :: %s",
			PRE_VERSION_MA, PRE_VERSION_MI, PRE_VERSION_NAME);
}

void pre_extern_SE(){
	//Subsumption elimination.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	int32_t i, found, *litC, *litD, numOccs;
	clause *c, *d, **occs;

	#ifdef VERBOSE_PRE
	int32_t numSubsumed = 0;
	printf("c     PRE: SE[%d]:\nc     PRE:\t", cQ_leaf - 1);
	#endif

	//Now we pick dequeue the best elements in the priority queue, as long as there are more to dequeue.
	while ((c = cQ_dequeueBest()) != NULL){
		//We first check which of the literals in c has the smallest occurrence list.
		numOccs = INT32_MAX;
		occs = main_litData[c->lits[0]].occs;
		for (i = 0; i < c->size; ++i){
			if (main_litData[c->lits[i]].numOccsUsed < numOccs){
				occs = main_litData[c->lits[i]].occs;				//The occurrences pointer now points to the list.
				numOccs = main_litData[c->lits[i]].numOccsUsed;		//The number of occurrences has been stored.
			}
		}
		//Here lit now is the one for which we walk through the occurrence list and check if any of the clauses can be
		//subsumed. If this is the case, we abandon the specific clause right away. For each of these checks, we will use the
		//signatures of the clauses to identify those where a subsumption is certainly NOT possible.
		for (i = 0; i < numOccs; ++i){
			d = occs[i];
			//We ignore the clause itself.
			if (c == d) continue;
			//If c contains something not found in d we can continue, since no strengthening is possible.
			if ((c->size > d->size) || (c->sig & ~d->sig) != 0) continue;

			//Here at this point, it COULD be that a subsumption is possible. All we need to do here is to walk through the
			//literals of c and check if they are also found in d. If so, we abandon d.
			litC = c->lits;
			found = 0;
			while (HAS_CLS_MORE_LITS(c,litC)){
				litD = d->lits;
				found = 0;
				while (HAS_CLS_MORE_LITS(d,litD)){
					if (*litC == *litD){
						++found;
						break;
					}
					++litD;
				}
				//If the literal is missing, we can stop.
				if (!found) break;
				++litC;
			}

			//If a literal is missing, no subsumption is possible. We can continue with the next occurrence.
			if (!found) continue;

			//At this point, we have found all the literals from c in d. We can get rid of d right now. This drops the
			//current occurrence in the occurrence list occs. We therefore must reduce the counter by 1.
			#ifdef VERBOSE_PRE
			++numSubsumed;
			printf("%p ", (void*)d);
			if (numSubsumed % 10 == 0){
				printf("\nc     PRE:\t");
			}
			#endif
			//We also remove the clause from the priority queue if it is in there.
			prep_intern_abandonClause(d);
			--numOccs;
			--i;
		}
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n", numSubsumed);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_SE += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_SE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_PLE(){
	//Pure literal elimination.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	clause* c;
	literal *l;
	variable* v;
	int32_t lit;

	#ifdef VERBOSE_PRE
	int32_t numAssigned = 0;
	printf("c     PRE: PLE[%d]: ", vS_leaf - 1);
	#endif

	//Now we pick a variable from the stack, as long as we have some more.
	while ((v = vS_pop()) != NULL){
		//We check, if the any literal of the variable has has exactly zero occurrences. If so, we assign the variable the
		//other way and delete all clauses from its occurrence list.
		if (!IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)) continue;
		l = main_litData + v->id;
		if(l->numOccsUsed == 0U){
			lit = -v->id;	//Satisfied literal number.
			#ifdef VERBOSE_PRE
			printf("%d ",lit);
			if (++numAssigned % 7 == 0){
				printf("\nc     PRE: PLE[%d]: ", vS_leaf - 1);
			}
			#endif
			if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					printf("conflict.\n");
					pre_returnCode = PRE_UNSAT;
					return;
				}
			}

			continue;
		}

		l = main_litData - v->id;
		if(l->numOccsUsed == 0U){
			lit = v->id;	//Satisfied literal number.
			#ifdef VERBOSE_PRE
			printf("%d ",lit);
			if (++numAssigned % 7 == 0){
				printf("\nc     PRE: PLE[%d]: ", vS_leaf - 1);
			}
			#endif
			if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					printf("conflict.\n");
					pre_returnCode = PRE_UNSAT;
					return;
				}
			}
		}
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n", numAssigned);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_PLE += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_PLE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_FLD(){
	//Failed literal detection.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	variable *v;
	literal *l, *l2;
	clause* c;
	int32_t litToSatisfy, litToSatisfy2, numPropagated = 0, i, lastWorked = 0, *upQ;

	#ifdef VERBOSE_PRE
	int32_t numActive = f.n_vars_e_used;
	printf("c     PRE: FLD[%d]: ", lQ_leaf - 1);
	uint32_t numMsg = 0;
	#endif

	//Now we pick a literal from the queue, as long as we have some more.
	while ((l = lQ_dequeueBest()) != NULL){
		//We grab the corresponding variable.
		litToSatisfy = GET_LIT_NUM_FROM_PTR(l);
		v = main_varData + abs(litToSatisfy);
		if (IS_VAR_UNASSIGNED(v) && !IS_VAR_DISABLED(v)){
			//The variable is not yet assigned and has implications available.
			ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy);
			//We unit propagate temporarily, and unassign if and only if we run into a conflict.
			c = main_simpleUP_propagate_temporary(&numPropagated, 0, 0, 0, NULL);
			if (c != NULL){
				//The unit propagation ran into a conflict. We must propagate the opposite.
				#ifdef VERBOSE_PRE
				printf("F%d ", litToSatisfy);
				if (++numMsg % 8 == 0){
					printf("\nc     PRE: FLD[%d]: ", lQ_leaf - 1);
				}
				#endif
				litToSatisfy = -litToSatisfy;
				ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy);
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					#ifdef VERBOSE_PRE
					printf(" F%d conflict.\n", litToSatisfy);
					#endif
					pre_returnCode = PRE_UNSAT;
					#ifdef COLLECTSTATS
					stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
					#endif
					return;
				}
			} else {
				//If we reach this position and did not run into a conflict, all the propagations were in vain, but we at
				//least know that all the propagated literals are not failed.
				//The unit propagation went through without running into conflicts. All the literals that have been
				//propagated can be removed from the literal priority queue.
				RESTORE_SIMPLEUPQUEUE(numPropagated);
				for (i = 1; i < numPropagated; ++i){
					if ((main_litData + main_simpleUPQueue[i])->lQPos > 0 ) lQ_delete(main_litData + main_simpleUPQueue[i]);
				}

				//We now pick the next literal from the queue, and propagate it.
				if (lQ_leaf > 1){
					l2 = lQ[1];
					litToSatisfy2 = GET_LIT_NUM_FROM_PTR(l2);
				} else {
					//There is no more literal in the literal priority queue. We unassign everything that is on the up queue.
					UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
					RESET_SIMPLEUPQUEUE();
					continue;
				}

				//We can additionally propagate litToSatisfy2 on top of what was done during the propagation of litToSatisfy.
				lastWorked = numPropagated;
				ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy2);
				//We unit propagate temporarily, and unassign if and only if we run into a conflict up to the
				//position we were before such that we can propagate the opposite.
				c = main_simpleUP_propagate_temporary(&numPropagated, 0, lastWorked, 0, NULL);
				if (c != NULL){
					//We ran into a problem and must propagate the opposite of litToSatisfy2.
					RESTORE_SIMPLEUPQUEUE(lastWorked);
					litToSatisfy2 = -litToSatisfy2;
					ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy2);
					c = main_simpleUP_propagate_temporary(&numPropagated, 0, lastWorked, 0, NULL);
					if (c != NULL){
						//We also ran into a problem the other way around. We must clean the whole queue and propagate
						//-litToSatisfy. If this leads into a conflict, the formula is UNSAT.
						//We unassign everything that is on the up queue.
						RESTORE_SIMPLEUPQUEUE(lastWorked);
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
						litToSatisfy = -litToSatisfy;
						#ifdef VERBOSE_PRE
						printf("D%d ", litToSatisfy);
						if (++numMsg % 8 == 0){
							printf("\nc     PRE: FLD[%d]: ", lQ_leaf - 1);
						}
						#endif
						ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy);
						c = main_simpleUP_propagate_prep();
						if (c != NULL){
							#ifdef VERBOSE_PRE
							printf(" F%d conflict.\n", litToSatisfy);
							#endif
							pre_returnCode = PRE_UNSAT;
							#ifdef COLLECTSTATS
							stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
							#endif
							return;
						}
					} else {
						//It did work out with the satisfying assignment of -litToSatisfy. We can remove everything on
						//the stack and dequeue all the literals involved because they all cannot be failed.
						RESTORE_SIMPLEUPQUEUE(numPropagated);
						for (i = lastWorked; i < numPropagated; ++i){
							if ((main_litData + main_simpleUPQueue[i])->lQPos > 0 ) lQ_delete(main_litData + main_simpleUPQueue[i]);
						}
						//We unassign everything that is on the up queue.
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
					}
				} else {
					//The second propagation in addition to the first went through smoothly as well. All the literals that
					//where additionally propagated can be removed from the queue as well.
					RESTORE_SIMPLEUPQUEUE(numPropagated);
					for (i = lastWorked; i < numPropagated; ++i){
						if ((main_litData + main_simpleUPQueue[i])->lQPos > 0 ) lQ_delete(main_litData + main_simpleUPQueue[i]);
					}
					//We unassign everything that is on the up queue.
					UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
					RESET_SIMPLEUPQUEUE();
				}
			}
		}
	}

	#ifdef VERBOSE_PRE
	printf("\nc     PRE:\tFLD removed %d variables.\n", numActive - f.n_vars_e_used);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_FLD += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_FLD += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_SLFLD(){
	//Failed literal detection.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	variable *v;
	literal *l;
	clause* c;
	int32_t litToSatisfy, numPropagated = 0, i, lastWorked = 0, *upQ;

	#ifdef VERBOSE_PRE
	int32_t numActive = f.n_vars_e_used;
	printf("c     PRE: SLFLD[%d]: ", lQ_leaf - 1);
	uint32_t numMsg = 0;
	#endif

	//Now we pick a literal from the queue, as long as we have some more.
	while ((l = lQ_dequeueBest()) != NULL){
		//We grab the corresponding variable.
		litToSatisfy = GET_LIT_NUM_FROM_PTR(l);
		v = main_varData + abs(litToSatisfy);
		if (IS_VAR_UNASSIGNED(v) && !IS_VAR_DISABLED(v)){
			//The variable is not yet assigned and has implications available.
			ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy);
			//We unit propagate temporarily, and unassign if and only if we run into a conflict.
			c = main_simpleUP_propagate_temporary(&numPropagated, 0, 0, 0, NULL);
			if (c != NULL){
				//The unit propagation ran into a conflict. We must propagate the opposite.
				#ifdef VERBOSE_PRE
				printf("F%d ", litToSatisfy);
				if (++numMsg % 8 == 0){
					printf("\nc     PRE: SLFLD[%d]: ", lQ_leaf - 1);
				}
				#endif
				litToSatisfy = -litToSatisfy;
				ADD_LIT_TO_SIMPLEUPQUEUE(litToSatisfy);
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					#ifdef VERBOSE_PRE
					printf(" F%d conflict.\n", litToSatisfy);
					#endif
					pre_returnCode = PRE_UNSAT;
					#ifdef COLLECTSTATS
					stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
					#endif
					return;
				}
			} else {
				//The propagation went through smoothly. All the literals that
				//where propagated can be removed from the queue as well.
				RESTORE_SIMPLEUPQUEUE(numPropagated);
				for (i = lastWorked; i < numPropagated; ++i){
					if ((main_litData + main_simpleUPQueue[i])->lQPos > 0 ) lQ_delete(main_litData + main_simpleUPQueue[i]);
				}
				//We unassign everything that is on the up queue.
				UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
				RESET_SIMPLEUPQUEUE();
			}
		}
	}

	#ifdef VERBOSE_PRE
	printf("\nc     PRE:\tSLFLD removed %d variables.\n", numActive - f.n_vars_e_used);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_FLD += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_FLD += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_STR(){
	//Strengthening.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	clause *c, *d;
	literal *oppOccLit;
	int32_t i,j, found, resLit = 0, resLitPos = 0, *litC, *litD;
	uint64_t cSigSimp, litSig, wasunit;

	#ifdef VERBOSE_PRE
	int32_t numStrengthened = 0;
	printf("c     PRE: STR[%d]:\nc     PRE:\t", cS_leaf - 1);
	#endif

	//We grab clauses from the stack as long as there are some left.
	while ((c = cS_pop()) != NULL){
		//We now check if this clause is able to strengthen another. We check all literals in c and check if any of the
		//clauses we can resolve c with can be strengthened.
		wasunit = 0;
		for (i = 0; i < c->size && !IS_CLS_DISABLED(c); ++i){
			resLit = c->lits[i];							//The literal in c we want to resolve with.
			//In order to speed things up, we will use the signature of c, but kill the bit for the literal we resolve on, if
			//there is no other literal in the same class. We then ask the question: what is found in this signature, that
			//is not found in the signature of d. If there is anything found, then a strengthening is not possible.
			cSigSimp = c->sig;								//Grab the signature.
			litSig = GET_LIT_SIG(resLit);
			if (c->size == POPCOUNT_UINT64(cSigSimp)){
				//Since each literal has its own bit in the signature, we can kill the bit of resLit.
				cSigSimp &= ~litSig;
			} else {
				//We must find out the hard way if there is a literal in c different from resLit that has the same signature.
				litC = c->lits;
				found = 0;
				while (HAS_CLS_MORE_LITS(c,litC)){
					if (*litC != resLit && GET_LIT_SIG(*litC) == litSig){
						++found;
						break;
					}
					++litC;
				}
				if (!found){
					//We have not found another literal that causes the same signature as resLit. We can kill the bit.
					cSigSimp &= ~litSig;
				}
			}
			//Now, cSigSimp is the signature of c ignoring resLit.
			resLit = -resLit;								//The literal we want to resolve out of d using c.
			oppOccLit = main_litData + resLit;					//The occurrence list containing this specific literal.
			for (j = 0; j < oppOccLit->numOccsUsed; ++j){
				d = oppOccLit->occs[j];						//The clause d containing the resLit.

				//First, we check if the signature test is passed and that c is not larger than d.
				if (c->size > d->size || (cSigSimp & ~d->sig) != 0ULL) continue;
				//Passed. We must now make sure that all literals but one from c are also found in d. And that the one
				//missing is the one we resolve on.
				litC = c->lits;
				while (HAS_CLS_MORE_LITS(c,litC)){
					if (*litC == -resLit) {
						++litC;
						continue;							//We ignore the literal we resolve on.
					}
					litD = d->lits;
					found = 0;								//We assume we cannot find *litC in d.
					while (HAS_CLS_MORE_LITS(d,litD)){
						if (*litC == -*litD){
							//Literal litC has been found in d with opposite sign. This ends in a tautology.
							found = 0;
							break;
						} else if (*litC == *litD){
							++found;						//We found it. We can stop searching for it.
							break;
						}
						++litD;
					}
					if (!found){
						//We were unable to locate a literal from c in d that is not the one we resolve on. No strengthening.
						resLit = 0;
						break;
					}
					++litC;
				}
				//We check if we were able to locate all literals.
				if (resLit == 0) continue;
				//We were able to. We must now remove literal resLit from clause d.

				#ifdef VERBOSE_PRE
				++numStrengthened;
				printf("%p<%p:", (void*)c, (void*)d);
				#endif

				resLitPos = oppOccLit->occLitPos[j];		//At what position is resLit in d.
				//We must distinct two cases here. The general case, where clause d is at least of size 3, or the more
				//rare case, where d is of size 2. In the latter case, we are about to learn a unit clause now.
				if (d->size > 2){
					//The standard case. We remove the literal from the clause. This will remove the occurrence at position
					//j in the current for loop. So we must reduce j by 1.
					#ifdef VERBOSE_PRE
					printf("%d:%d ", resLit, d->size-1);
					if (numStrengthened % 4 == 0){
						printf("\nc     PRE:\t");
					}
					#endif
					prep_intern_deleteLitInClause(d, resLit, resLitPos);
				} else {
					//The rare case where we can learn a unit. What we do now is, we get the one literal from the clause, that
					//is not resLit, and put it on the UP queue. Propagating will result in clauses c and d being removed from
					//the formula.
					#ifdef VERBOSE_PRE
					printf("UNIT\n");
					#endif
					wasunit = 1;
					if (d->lits[0] == resLit){
						//The other one must be the one to propagate. The resLit now becomes the result literal to propagate.
						resLit = d->lits[1];
					} else {
						//The current one must be the one to propagate. The resLit now becomes the result literal to propagate.
						resLit = d->lits[0];
					}
					//Now resLit can be added to the UP queue as our decision an be propagated.
					ADD_LIT_TO_SIMPLEUPQUEUE(resLit);
					d = main_simpleUP_propagate_prep();
					if (d != NULL){
						//We tried to propagate, but this ended in a conflict. The formula is unsatisfiable.
						pre_returnCode = PRE_UNSAT;
						#ifdef COLLECTSTATS
						stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
						#endif
						return;
					}
					//We propagated successfully. We can stop working on c. Breaking here will leave the for loop of the
					//occurrences and end in the for loop for the literals in c. Since c is disabled, this for loop will
					//stop as well.
					#ifdef VERBOSE_PRE
					printf("c     PRE:\t");
					#endif
					break;
				}
			}
			if (wasunit) break;
		}
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n", numStrengthened);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_STR += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_STR += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_NIVER(){
	//Non increasing variable elimination by resolution.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	variable *v;
	literal *lp, *ln, *louter, *linner;
	clause *c, *d;
	int32_t totalClauses, posClauses, negClauses, temp, lit, i, j, *litC, *litD;
	uint64_t cSigSimp, litSig;

	#ifdef VERBOSE_PRE
	int32_t numVarRemoved = 0, numRemoved = 0, numAdded = 0;
	printf("c     PRE: NIVER[%d]:\nc     PRE:\t", vQ_leaf - 1);fflush(stdout);
	#endif

	while ((v = vQ_dequeueBest()) != NULL){

		if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v)) continue;

		lp = main_litData + v->id; posClauses = lp->numOccsUsed;
		ln = main_litData - v->id; negClauses = ln->numOccsUsed;

		totalClauses = posClauses + negClauses;

		//We do not care about variables that are pure. We unit propagate them right away.
		if (posClauses == 0 || negClauses == 0) {
			#ifdef VERBOSE_PRE
			printf(" [RV%d:RC%d:AC%d] Calling UP for pure...\n", numVarRemoved, numRemoved, numAdded);
			#endif
			if (posClauses == 0){
				//We satisfy all negative clauses.
				lit = -v->id;
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				c = main_simpleUP_propagate_prep();
				//This cannot result in a conflict, so no check is necessary.
			} else {
				//We satisfy all positive clauses.
				lit = v->id;
				ADD_LIT_TO_SIMPLEUPQUEUE(lit);
				c = main_simpleUP_propagate_prep();
				//This cannot result in a conflict, so no check is necessary.
			}
			#ifdef VERBOSE_PRE
			printf("c     PRE:\tContinued NIVER[%d]:\nc     PRE:\t", vQ_leaf - 1);
			#endif
			if (f.m_eo_used + f.m_el_used == 0) {
				//There are no more clauses after this, we must return.
				#ifdef VERBOSE_PRE
				printf("No clauses left after NIVER pure propagation.\n");
				#endif
				vQ_flush();
				#ifdef COLLECTSTATS
				stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
				#endif
				return;
			}
			//We go on with the next variable.*/
			continue;
		}

		//Using the clause signatures, we will first check what the minimum number of resolvents would be if we resolve all
		//positive occurrences of v with all negative occurrences of v. We only use the signatures here, and get a lower
		//border. This might, in fact, be much larger if the resolution is in deed performed. We count this lower border
		//number in temp.
		//We also set the outer for loop to be the one with less occurrences.
		if (posClauses > negClauses){
			louter = ln; linner = lp; lit = -v->id;
		} else {
			louter = lp; linner = ln; lit = v->id;
		}
		temp = 0;
		if(posClauses > 1 && negClauses > 1){
			//We pick the one occurrence list that is shorter, and for each clause found there, we compute a simplified
			//signature that does not contain the signature bit for v, if not set by a different literal. With this signature
			//we then check all clauses of the other occurrence list if there is one that cannot be resolved into a tautology.
			//If so, we increase temp.
			//We now walk through the occurrence list of louter, for the picked clause c we kill the bit for lit if possible.
			//If this is not possible, we can go on with the next clause right away (all signature tests would fail anyway).
			litSig = GET_LIT_SIG(lit);
			for (i = 0; i < louter->numOccsUsed; ++i){
				c = louter->occs[i];
				cSigSimp = c->sig;
				if (c->size == POPCOUNT_UINT64(cSigSimp)){
					//Since each literal has its own bit in the signature, we can kill the bit of lit.
					cSigSimp &= ~litSig;
				} else {
					//We must find out the hard way if there is a literal in c different from lit that has the same signature.
					litC = c->lits;
					j = 0;
					while (HAS_CLS_MORE_LITS(c,litC)){
						if (*litC != lit && GET_LIT_SIG(*litC) == litSig){
							++j;
							break;
						}
						++litC;
					}
					if (!j){
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
				//Otherwise, we can rotate the signature and check all occurrences in linner.
				cSigSimp = ROTATE_32RIGHT_UINT64(cSigSimp);
				//Now we use this signature to check against all occurrences of linner. Those, were no tautology would be
				//possible increase temp.
				for (j = 0; j < linner->numOccsUsed; ++j){
					d = linner->occs[j];
					if ((cSigSimp & d->sig) == 0ULL) ++temp;
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
		}
		//It seems that we are going to perform the resolution. The number of resolvents must be set to a worst case of
		//posClauses*negClauses, because all the resolvents could survive the resolution.
		PRE_RESET_ALL_RESOLVENTS();
		for (i = 0; i < louter->numOccsUsed; ++i){
			c = louter->occs[i];
			for (j = 0; j < linner->numOccsUsed; ++j){
				d = linner->occs[j];
				//We now prepare the copying operation. The literal we resolve on is lit in c and -lit in d.
				PRE_RESET_RESOLVENT(pre_resUsed);
				++main_litStamp;
				//We can grab all the literals from c, ignoring the one we resolve on. It can be no tautology yet.
				litC = c->lits;
				while (HAS_CLS_MORE_LITS(c, litC)){
					if (*litC != lit){
						//We add the literal.
						PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litC);
						//We stamp the literal.
						(main_litData + *litC)->stamp = main_litStamp;
					}
					++litC;
				}
				//We now copy all the literals from d, we invert the sign of lit for this, as this is what we ignore in d.
				lit = -lit;
				litD = d->lits;
				while (HAS_CLS_MORE_LITS(d, litD)){
					//We ignore the resolution literal and any literal already in the resolvent.
					if (*litD != lit && (main_litData + *litD)->stamp != main_litStamp){
						//We check if adding this literal will result in a tautology. If not, we add it. If so, we break.
						if ((main_litData - *litD)->stamp == main_litStamp){
							//We break early since the resolvent would be a tautology.
							break;
						}
						//No tautology yet.
						PRE_ADD_LIT_TO_CURRENT_RESOLVENT(*litD);
						(main_litData + *litD)->stamp = main_litStamp;
					}
					++litD;
				}
				//We revert the temporary sign inversion.
				lit = -lit;
				if (HAS_CLS_MORE_LITS(d, litD)){
					//The resolvent has only been generated partially and was identified to be a tautology. We can ignore it.
					continue;
				}
				//If we get this far, then the resolvent is not a tautology. We want to keep it for now.
				PRE_INCREASE_NUM_RESOLVENTS( temp );
				//If we are now above the border of totalClauses, then we can abort the NIVER operations on this variable, as
				//NIVER will not be performed.
				if (pre_resMemUsed[pre_resUsed-1] == 1){
					//The created resolvent is a unit clause. Independent of everything we did so far, we will propagate
					//this finally.
					#ifdef VERBOSE_PRE
					printf(" [RV%d:RC%d:AC%d] Calling UP for UNIT...\n", numVarRemoved, numRemoved, numAdded);fflush(stdout);
					#endif
					ADD_LIT_TO_SIMPLEUPQUEUE(pre_res[pre_resUsed-1][0]);
					d = main_simpleUP_propagate_prep();
					#ifdef VERBOSE_PRE
					printf("c     PRE:\tContinued NIVER[%d]:\nc     PRE:\t", vQ_leaf - 1);
					#endif
					if (d != NULL){
						#ifdef VERBOSE_PRE
						printf("Conflict during NIVER unit resolvent propagation.\n");
						#endif
						pre_returnCode = PRE_UNSAT;
						#ifdef COLLECTSTATS
						stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
						#endif
						return;
					}
					if (f.m_eo_used + f.m_el_used == 0) {
						//There are no more clauses after this, we must return.
						#ifdef VERBOSE_PRE
						printf("No clauses left after NIVER pure propagation.\n");
						#endif
						vQ_flush();
						#ifdef COLLECTSTATS
						stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
						#endif
						return;
					}
					pre_resUsed = INT32_MAX;
				}
				if (pre_resUsed > totalClauses){
					break;
				}
			}
			if (pre_resUsed > totalClauses){
				break;
			}
		}
		//This concludes the learning operations. The following check determines if we are going to perform the variable
		//elimination by removing the old clauses and adding the new resolvents in stead.
		if (pre_resUsed > totalClauses){
			//Nope, we drop everything and go on with the next variable.
			continue;
		}
		//We perform the elimination. We first delete the larger set, and then make the clauses of the smaller set removed.
		while (linner->numOccsUsed > 0){
			d = linner->occs[0];
			prep_intern_deleteClause(d);
		}
		//We then transfer the smaller set to the removed clauses array. The literal lit we used for the resolution is the
		//one literal of interest for NIVER.
		while (louter->numOccsUsed > 0){
			c = louter->occs[0];
			prep_intern_makeClauseRemoved(c, lit, PREP_REMCLS_NIVER, 0, NULL);//We make the clause removed.
		}
		//We finally assign the variable behind lit in such a way that it does NOT satisfy the clauses we just removed. So
		//the literal lit will become FALSE because of this assignment. We then disable the variable.
		if (lit > 0){
			//The literal is positive. Assign it with 0.
			SET_VAR_ASS_0(v);
		} else {
			//The literal is negative. Assign it with 1.
			SET_VAR_ASS_1(v);
		}
		prep_intern_disableVariable(v);

		//We now add all the resolvents as new original clauses to the formula.
		for (i = 0; i < pre_resUsed; ++i){
			prep_intern_addOriginalClause(pre_res[i], pre_resMemUsed[i]);
		}
		//This concludes NIVER for variable v.
		#ifdef VERBOSE_PRE
		numRemoved += totalClauses; numAdded += pre_resUsed; ++numVarRemoved;
		printf("%dP%dN%dA%d ", v->id, posClauses, negClauses, pre_resUsed);
		if (numVarRemoved % 7 == 0){
			printf("\nc     PRE:\t");
		}
		#endif
	}

	#ifdef VERBOSE_PRE
	printf("[RV%d:RC%d:AC%d]\n", numVarRemoved, numRemoved, numAdded);
	#endif

	#ifdef COLLECTSTATS
	stats_pre_varRemoved_NIVER += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_NIVER += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_ATE(){
	//Asymmetric tautology elimination.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	int32_t i;
	clause *c, *d;

	#ifdef VERBOSE_PRE
	int32_t numRemoved = 0;
	printf("c     PRE: ATE[%d]:\nc     PRE:\t", cS_leaf - 1);
	#endif


	//While we have clauses for checking the ATE, we continue.
	while ((c = cS_pop()) != NULL){
		//We add all the literals of the clause with inverted sign in the UP queue.
		for (i = 0; i < c->size; i++){
			ADD_LIT_TO_SIMPLEUPQUEUE(-c->lits[i]);
		}
		//We perform the unit propagation and check if it fails. If so, the clause can be abandoned.
		d = main_simpleUP_propagate_ignore(c);
		if (d != NULL){
			//Clause c is an asymmetric tautology. We remove it.
			#ifdef VERBOSE_PRE
			printf("A%p ", (void*)c);
			++numRemoved;
			if (numRemoved % 10 == 0){
				printf("\nc     PRE:\t");
			}
			#endif
			prep_intern_makeClauseRemoved(c, 0, PREP_REMCLS_ATE, 0, NULL);
		}
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n", numRemoved);
	#endif

	#ifdef COLLECTSTATS
	stats_pre_varRemoved_ATE += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_ATE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_ACCE(){
	//Asymmetric covered clause elimination.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	literal *l;
	clause *c, *alaResult;
	variable *v;
	int32_t bLit, offset, claEndSize, upStartSize, *litPtr;

	#ifdef VERBOSE_PRE
	int32_t numRemoved = 0;
	printf("c     PRE: ACCE[%d]:\nc     PRE:\t", lQ_leaf - 1);
	#endif

	if (param_prepACCESaturate == 0){
		//In case the parameter is set accordingly, we will not re-add literals to check their clauses anew. This is a simple
		//way of massively decreasing the cost for ACCE.
		lQ_touchLiteral = &lQ_touchLiteral_null;
	}

	//We perform asymmetric covered clause elimination.
	while ((l = lQ_dequeueBest()) != NULL){
		//This literal has the least opposite clause occurrences. We walk through its occurrence list, and for each of the
		//contained clauses, we check if it is blocked.
		v = main_varData + abs(GET_LIT_NUM_FROM_PTR(l));
		if (!IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)) continue;

		offset = 0; //Will be increased for each clause that is not removed.

		while (offset < l->numOccsUsed){
			c = l->occs[offset];
			//We check if the clause is too small to be checked by ACCE.
			if (c->size < param_prepACCEMinClsSize){
				//It is too small, we skip it.
				++offset;
				continue;
			}
			//We store the clause we want to check in the unit propagation queue.
			litPtr = c->lits;
			while (HAS_CLS_MORE_LITS(c, litPtr)){
				ADD_LIT_TO_SIMPLEUPQUEUE(*litPtr);
				++litPtr;
			}
			//Now, the UP queue contains all the literals of the clause. We assume it not blocked or covered.
			bLit = 0;
			alaResult = NULL;
			//We retain the information where to start the unit propagation. Since this is a fresh clause, we start at zero.
			upStartSize = 0;
			do {
				//First, we perform covered literal addition, CLA. This is done by computing the resolution intersection
				//of all the literals in the clause, including those that are added.
				bLit = pre_intern_ACCE_cla();
				if (bLit != 0){
					//The clause is blocked via bLit.
					break;
				}
				//The claEndSize tells us how many elements are found in the UP queue after CLA is done. If this does not
				//increase by ALA, we can stop. Another loop is not necessary because CLA itself is already done until
				//saturation.
				claEndSize = (int32_t)(addr_t)(main_simpleUPQueue_current - main_simpleUPQueue);

				//Second, we perform ALA, which means we propagate everything we have in the UP queue. This will not reset the
				//UPqueue as long as there was no conflict. It also does no cleanup (unassigning of variables).
				alaResult = main_simpleUP_propagate_ala(upStartSize, c);
				if (alaResult != NULL){
					//The check failed.
					break;
				}
				//Next time, the unit propagation must start at the point we are currently at.
				upStartSize = (int32_t)(addr_t)(main_simpleUPQueue_current - main_simpleUPQueue);
				//The check whether more changes are possible is done by comparing the claEndSize and upStartSize. If the
				//upStartSize is beyond the claEndSize, then the last UP added new stuff in the UP queue. This new stuff might
				//enable new covered literals to be found or the clause being blocked.
			} while (claEndSize < upStartSize);
			//Because ALA UP does not unassign nor reset, we must do this now. It does not matter anyway because all the
			//information on the clause c is still available (covered literals, the blocking literal, if any and the clause
			//itself as original. We do not need the information given in the UP queue.
			UNASSIGN_SIMPLEUPQUEUE(v, litPtr, 0);
			RESET_SIMPLEUPQUEUE();
			if (bLit != 0){
				//The clause is blocked via bLit. We make the clause removed.
				if (pre_covLitsUsed == 0) {
					#ifdef VERBOSE_PRE
					++numRemoved;
					printf("B%p ", (void*)c);
					if (numRemoved % 10 == 0){
						printf("\nc     PRE:\t");
					}
					#endif
					prep_intern_makeClauseRemoved(c,bLit,PREP_REMCLS_BLOCKED,0,NULL);
				} else {
					#ifdef VERBOSE_PRE
					++numRemoved;
					printf("C%p ", (void*)c);
					if (numRemoved % 10 == 0){
						printf("\nc     PRE:\t");
					}
					#endif
					//Waring: the next call consumes the pre_covLits array. We must allocate a new one in its stead.
					prep_intern_makeClauseRemoved(c,bLit,PREP_REMCLS_COVERED,pre_covLitsUsed,pre_covLits);
					//We must now allocate a new array for the covered literals.
					pre_covLits = malloc(sizeof(coveredLit)*(f.n_vars_e_used + 1));
				}
			} else if (alaResult != NULL) {
				//The clause is an asymmetric tautology.
				if (pre_covLitsUsed == 0){
					#ifdef VERBOSE_PRE
					++numRemoved;
					printf("A%p ", (void*)c);
					if (numRemoved % 10 == 0){
						printf("\nc     PRE:\t");
					}
					#endif
					prep_intern_makeClauseRemoved(c, 0, PREP_REMCLS_ATE, 0, NULL);
				} else {
					++offset;
				}
			} else {
				++offset;
			}

			//We reset the found covered literals, if they were needed, they have been used by now. The same is true for the
			//asymmetrically added literals.
			pre_covLitsUsed = 0;
		}
	}

	if (param_prepACCESaturate == 0){
		//In case the parameter is set accordingly, we must now re-enable the touching of literals for ACCE, since other
		//methods might now re-add something to the literal queue.
		lQ_touchLiteral = &lQ_touchLiteral_acce;
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n", numRemoved);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_ACCE += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_ACCE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_GE(){
	//Gate extraction.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	int32_t numVarsActive = f.n_vars_e_used;
	int32_t numClsActive = f.m_el_used + f.m_eo_used;
	#endif
	variable *v;
	clause *c;

	#ifdef VERBOSE_PRE
	int32_t numRemoved = 0;
	printf("c     PRE: GE[%d]:\nc     PRE:\t", vQ_leaf - 1);fflush(stdout);
	#endif

	//This method will perform gate extraction. For every variable in the variable priority queue, it will check if it can
	//detect any definitions. Later, we could add further besides the ones of ANDs, and ORs, and XORs.

	while ((v = vQ_dequeueBest()) != NULL){
		//For the given variable, we first check if it is pure. If so, we call the UP right away.
		if (IS_VAR_DISABLED(v) || !IS_VAR_UNASSIGNED(v)) continue;
		if ((main_litData + v->id)->numOccsUsed == 0){
			//The literal is pure negatively. Enforce using UP.
			#ifdef VERBOSE_PRE
			printf("Calling UP for pure...\n");
			#endif
			ADD_LIT_TO_SIMPLEUPQUEUE(-v->id);
			c = main_simpleUP_propagate_prep();
			//This cannot result in a conflict, but it is safer to check anyway.
			if (c != NULL){
				printf("c ERROR. Conflict encountered in pure propagation.\n");
				pre_returnCode = PRE_ERROR;
				return;
			}
			#ifdef VERBOSE_PRE
			printf("c     PRE:\tContinued GE[%d]:\nc     PRE:\t", vQ_leaf - 1);
			#endif
			if (f.m_eo_used + f.m_el_used == 0) {
				//There are no more clauses after this, we must return.
				#ifdef VERBOSE_PRE
				printf("No clauses left after GE pure propagation.\n");
				#endif
				vQ_flush();
				#ifdef COLLECTSTATS
				stats_pre_varRemoved_GE += (numVarsActive - (int32_t)f.n_vars_e_used);
				stats_pre_clsRemoved_GE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
				stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
				stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
				stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
				#endif
				return;
			}
			continue;
		}
		if ((main_litData - v->id)->numOccsUsed == 0){
			//The literal is pure negatively. Enforce using UP.
			#ifdef VERBOSE_PRE
			printf("Calling UP for pure...\n");
			#endif
			ADD_LIT_TO_SIMPLEUPQUEUE(v->id);
			c = main_simpleUP_propagate_prep();
			//This cannot result in a conflict, but it is safer to check anyway.
			if (c != NULL){
				printf("c ERROR. Conflict encountered in pure propagation.\n");
				pre_returnCode = PRE_ERROR;
				break;
			}
			#ifdef VERBOSE_PRE
			printf("c     PRE:\tContinued GE[%d]:\nc     PRE:\t", vQ_leaf - 1);
			#endif
			if (f.m_eo_used + f.m_el_used == 0) {
				//There are no more clauses after this, we must return.
				#ifdef VERBOSE_PRE
				printf("No clauses left after GE pure propagation.\n");
				#endif
				break;
			}
			continue;
		}
		//In case the variable is not pure in one of the directions, we might be able to find a gate.
		if (!IS_VAR_DISABLED(v)){
			pre_intern_GE_SIM3(v);
			if (pre_returnCode != PRE_UNKNOWN) break;
			//We now check if any unit propagation resulted from this.
			if (!IS_SIMPLEUPQUEUE_EMPTY()){
				#ifdef VERBOSE_PRE
				printf("Calling UP for unit...\n");
				#endif
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					printf("c ERROR. Conflict encountered in GE unit propagation.\n");
					pre_returnCode = PRE_ERROR;
					break;
				}
				#ifdef VERBOSE_PRE
				printf("c     PRE:\tContinued GE[%d]:\nc     PRE:\t", vQ_leaf - 1);
				#endif
				if (f.m_eo_used + f.m_el_used == 0) {
					//There are no more clauses after this, we must return.
					#ifdef VERBOSE_PRE
					printf("No clauses left after GE unit propagation.\n");
					#endif
					break;
				}
			}
			if ((main_litData + v->id)->numOccsUsed == 0 && (main_litData - v->id)->numOccsUsed == 0) {
				prep_intern_disableVariable(v);
			}
		}
		if (!IS_VAR_DISABLED(v)){
			pre_intern_GE_SIM4(v);
			if (pre_returnCode != PRE_UNKNOWN) break;
			//We now check if any unit propagation resulted from this.
			if (!IS_SIMPLEUPQUEUE_EMPTY()){
				#ifdef VERBOSE_PRE
				printf("Calling UP for unit...\n");
				#endif
				c = main_simpleUP_propagate_prep();
				if (c != NULL){
					printf("c ERROR. Conflict encountered in GE unit propagation.\n");
					pre_returnCode = PRE_ERROR;
					return;
				}
				#ifdef VERBOSE_PRE
				printf("c     PRE:\tContinued GE[%d]:\nc     PRE:\t", vQ_leaf - 1);
				#endif
				if (f.m_eo_used + f.m_el_used == 0) {
					//There are no more clauses after this, we must return.
					#ifdef VERBOSE_PRE
					printf("No clauses left after GE unit propagation.\n");
					#endif
					vQ_flush();
					#ifdef COLLECTSTATS
					stats_pre_varRemoved_GE += (numVarsActive - (int32_t)f.n_vars_e_used);
					stats_pre_clsRemoved_GE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
					stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
					stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
					stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
					#endif
					return;
				}
			}
			if ((main_litData + v->id)->numOccsUsed == 0 && (main_litData - v->id)->numOccsUsed == 0) {
				prep_intern_disableVariable(v);
			}
		}
		if (!IS_VAR_DISABLED(v)){
			pre_intern_GE_AND(v);
			if (pre_returnCode != PRE_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			pre_intern_GE_OR(v);
			if (pre_returnCode != PRE_UNKNOWN){
				break;
			}
		}
		if (!IS_VAR_DISABLED(v)){
			pre_intern_GE_XOR(v);
			if (pre_returnCode != PRE_UNKNOWN){
				break;
			}
		}
		#ifdef VERBOSE_PRE
		if (IS_VAR_DISABLED(v)){
			//The variable was disabled during gate extraction.
			++numRemoved;
			if (numRemoved % 5 == 0){
				printf("\nc     PRE:\t");
			}
		}
		#endif
	}

	#ifdef VERBOSE_PRE
	printf("[%d]\n",numRemoved);
	#endif

	#ifdef COLLECTSTATS
	stats_pre_varRemoved_GE += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_GE += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

int32_t pre_intern_UNHIDING_SSet_incDSC(const void* a, const void* b){
	//Sorts the literals with increasing discovery time. We swap if a has a larger discovery time.
	if ( GET_LIT_DSC((*(literal**)a)) > GET_LIT_DSC((*(literal**)b)) ) return 1;
	return -1;
}

int32_t pre_intern_UNHIDING_SSet_decDSC(const void* a, const void* b){
	//Sorts the literals with decreasing discovery time. We swap if a has a smaller discovery time.
	if ( GET_LIT_DSC((*(literal**)a)) < GET_LIT_DSC((*(literal**)b)) ) return 1;
	return -1;
}

void pre_intern_UNHIDING_uhle(clause* c){
	//This performs an UHLE check on the given clause (which must be disabled). We begin by creating the SPos array in
	//reversed order.
	literal *l;
	uint64_t sig = 0ULL;
	uint32_t finished, i, oldSize = GET_CLS_SIZE(c), maxSize = GET_CLS_SIZE(c), writePos;
	int32_t litID;
	for (i = 0; i < maxSize; ++i){
		l = main_litData + GET_CLS_LITNUM(c, i);
		pre_UNHIDING_SPlus[i] = l;
	}
	//We now sort the SPlus array to hold the literals with decreasing discovery time.
	qsort(pre_UNHIDING_SPlus, maxSize, sizeof(literal*), pre_intern_UNHIDING_SSet_decDSC);
	finished = GET_LIT_FIN(pre_UNHIDING_SPlus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = pre_UNHIDING_SPlus[i];
		if (GET_LIT_FIN(l) > finished){
			//We do not keep the element in SPlus at position i. We just go on with the next literal in SPos.
		} else {
			//We want to keep l. We write it at the current writePos and go on.
			pre_UNHIDING_SPlus[writePos++] = l;
			finished = GET_LIT_FIN(l);
		}
	}
	//We update the maxSize with the elements written (number of retained elements).
	maxSize = writePos;
	//We now create SMinus with all the retained elements, but invert their sign. We sort with increasing discovery time.
	for(i = 0; i < maxSize; ++i){
		pre_UNHIDING_SMinus[i] = GET_LIT_OPPOSITE(pre_UNHIDING_SPlus[i]);
	}
	qsort(pre_UNHIDING_SMinus, maxSize, sizeof(literal*), pre_intern_UNHIDING_SSet_incDSC);
	finished = GET_LIT_FIN(pre_UNHIDING_SMinus[0]);
	writePos = 1;
	for (i = 1; i < maxSize; ++i){
		l = pre_UNHIDING_SMinus[i];
		if (GET_LIT_FIN(l) < finished){
			//We do not keep the element in SMinus at position i. We just go on with the next literal in SMinus.
		} else {
			//We want to keep l. Write it at the current writePos and go on.
			pre_UNHIDING_SMinus[writePos++] = l;
			finished = GET_LIT_FIN(l);
		}
	}
	//The amounts of literals written is now the new size of the clause. We check if we did indeed remove something from c.
	maxSize = writePos;
	if (oldSize > maxSize){
		//Yes, a removal happened. We will now copy all the retained elements from SMinus with opposite sign to the clause.
		//We update the signature of the clause and its size and watcher information.
		for(i = 0; i < maxSize; ++i){
			l = GET_LIT_OPPOSITE(pre_UNHIDING_SMinus[i]);
			litID = GET_LIT_NUM_FROM_PTR(l);
			sig |= GET_LIT_SIG(litID);
			SET_CLS_LITNUM(c, i, litID);
		}
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

uint32_t pre_intern_UNHIDING_uhte(clause* c){
	//This performs an UHTE check on the given clause (which must be disabled). This method returns 1 if the clause is a HT.
	//We first create the SPlus and SMinus sets such that they hold the respective literals or opposite literals in increasing
	//discovery time order.
	literal *lPos, *lNeg;
	uint32_t i, lastEltPos = GET_CLS_SIZE(c) - 1, currentEltPosPlus, currentEltPosMinus;
	//We create SPlus first by copying all the literals from c with the same sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		lPos = main_litData + GET_CLS_LITNUM(c, i);
		pre_UNHIDING_SPlus[i] = lPos;
	}
	qsort(pre_UNHIDING_SPlus, GET_CLS_SIZE(c), sizeof(literal*), pre_intern_UNHIDING_SSet_incDSC);
	lPos = pre_UNHIDING_SPlus[0];
	//We then create SMinus by copying all the literals form c with the opposite sign and then sorting it (increasing DSC).
	for (i = 0; i < GET_CLS_SIZE(c); ++i){
		lNeg = main_litData - GET_CLS_LITNUM(c, i);
		pre_UNHIDING_SMinus[i] = lNeg;
	}
	qsort(pre_UNHIDING_SMinus, GET_CLS_SIZE(c), sizeof(literal*), pre_intern_UNHIDING_SSet_incDSC);
	lNeg = pre_UNHIDING_SMinus[0];

	currentEltPosPlus = 0;
	currentEltPosMinus = 0;
	while(1){
		if (GET_LIT_DSC(lNeg) > GET_LIT_DSC(lPos)){
			if (currentEltPosPlus == lastEltPos) return 0;
			lPos = pre_UNHIDING_SPlus[++currentEltPosPlus];
		} else if ( (GET_LIT_FIN(lNeg) < GET_LIT_FIN(lPos))
				//|| ( GET_CLS_SIZE(c) == 2 && (lPos == GET_LIT_OPPOSITE(lNeg) || GET_LIT_PRT(lPos) == lNeg) )
				//The above check on clauses with size two is unnecessary, as transitive edges are removed on the fly
				//by the advanced stamping scheme.
					){
			if (currentEltPosMinus == lastEltPos) return 0;
			lNeg = pre_UNHIDING_SMinus[++currentEltPosMinus];
		} else {
			return 1;
		}
	}

	return 0;
}

uint32_t pre_intern_UNHIDING_stamp(literal* l, uint32_t stamp){
	//This method implements the advanced stamping procedure.
	literal *impLit, *impLitOpp, *failedLit;
	clause *c;
	uint32_t flagSCC = 1, impNum,i;	//The flag captures if l represents a strongly connected component (SCC).

	++stamp;
	SET_LIT_DSC(l, stamp);
	SET_LIT_OBS(l, stamp);
	//Push the literal to the literal stack.
	*(pre_UNHIDING_lStack_current++) = l;
	//We now check all binary clauses in which -l occurs. This is already captured in the implication list of l.
	for (impNum = 0; impNum < GET_LIT_NUMIMP_USED(l); impNum += 2){
		impLit = GET_IMPLIED_LIT_AS_PTR(l, impNum);
		c = GET_IMPLIED_LIT_REASON_AS_PTR(l, impNum);
		//First, we check if we can remove this implication.
		if (GET_LIT_DSC(l) < GET_LIT_OBS(impLit)) {
			//The binary clause can be removed (transitive reduction). This will also remove the implication from the list
			//of l, we therefore must reduce the impNum counter by 2 to stay at this specific position in the list.
			prep_intern_abandonClause(c);
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
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE_PTR(failedLit)){
				//This constitutes a global conflict as we found a failed literal for which the opposite is also failed.
				#ifdef VERBOSE_PREP
				printf("UNHIDING-EQ-CONFf ");
				#endif
				pre_returnCode = PRE_UNSAT;
				return 0;
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE_PTR(failedLit)){
				//We can add the literal since it is not already enqueued.
				ADD_LIT_TO_SIMPLEUPQUEUE_PTR(failedLit);
			}//Else the literal is already enqueued.
			if (GET_LIT_DSC(impLitOpp) != 0 && GET_LIT_FIN(impLitOpp) == 0) continue;
		}
		//Next, we perform the basic stamping operations.
		if (GET_LIT_DSC(impLit) == 0){
			SET_LIT_PRT(impLit, l);
			SET_LIT_ROOT(impLit, GET_LIT_ROOT(l));
			//Recursive stamping for the implied literal.
			stamp = pre_intern_UNHIDING_stamp(impLit, stamp);
			if (pre_returnCode == PRE_UNSAT) return 0;
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
		PRE_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--pre_UNHIDING_lStack_current);
			SET_LIT_DSC(impLit, GET_LIT_DSC(l));
			SET_LIT_FIN(impLit, stamp);

			if (GET_LIT_STAMP(impLit) != main_litStamp){
				PRE_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_LIT_STAMP(impLit, main_litStamp);
				SET_LIT_STAMP(GET_LIT_OPPOSITE(impLit), main_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (pre_ELS_classes_memUsed[pre_ELS_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++pre_ELS_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			pre_ELS_classes_memUsed[pre_ELS_classes_used] = 0;
		}
	}

	return stamp;
}

void pre_extern_UNHIDING(uint32_t numRountsToPerform){
	//This performs UNHIDING simplifications based on the BIG and the advanced stamping procedure of Armin, Marijn and Matti.
	//See the paper "Efficient CNF Simplifications based on Binary Implication Graphs".
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	uint32_t numClsActive = f.m_eo_used + f.m_el_used;
	uint32_t numVarsActive = f.n_vars_e_used;
	#endif
	literal *l, *lOpp, *classRep;
	variable *v;
	clause *c;
	int32_t lit, i,j, same, opp, reWID, toRID;
	uint32_t stamp, random, numToPermute, oIgnore, lIgnore, numRounds = 0;

	#ifdef VERBOSE_PRE
	uint32_t oldNumCls = f.m_eo_used + f.m_el_used;
	uint32_t oldNumVars = f.n_vars_e_used;
	uint32_t oldSize;
	uint32_t numStr = 0;
	printf("c     PRE: UNHIDING: ");fflush(stdout);
	#endif

	do {
		pre_UNHIDING_lStack_current = pre_UNHIDING_lStack+1;
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
		for (i = 0; i < perm_lfsr_perLen; ++i){
			random = perm_LFSR_nextState();
			if (random >= numToPermute) continue;
			if (random < f.n_vars_e_used){
				l = main_litData + GET_VAR_ID(f.vars_e[random]);
			} else {
				l = main_litData - GET_VAR_ID(f.vars_e[random - f.n_vars_e_used]);
			}
			pre_litPerm[stamp++] = l;
		}
		stamp = 0;

		//We prepare the literal stamp to present classes only once.
		++main_litStamp;

		//Initialize the classes information.
		pre_ELS_classes_used = 0;
		for (i = 0; i < pre_ELS_classes_avail; ++i){
			pre_ELS_classes_memUsed[i] = 0;
		}

		//We then walk through the permutation to stamp all the roots.
		for (i = 0; i < numToPermute; ++i){
			l = pre_litPerm[i];
			lOpp = GET_LIT_OPPOSITE(l);
			//Literal l is now the literal we want to stamp. Check if the literal l is a root or if it is already removed.
			if (GET_LIT_NUMIMP_USED(lOpp) == 0U){
				//Since lOpp has no implications, l cannot be in any binary clause, and is therefore a root.
				stamp = pre_intern_UNHIDING_stamp(l, stamp);
				if (pre_returnCode == PRE_UNSAT){
					#ifdef VERBOSE_PRE
					printf("R-CONFLICT.\n");fflush(stdout);
					#endif
					return;
				}
				//We can remove the literal from the permutation as it has been stamped.
			}
		}

		//We stamp the remaining literals if they have not be discovered yet.
		for (i = 0; i < numToPermute; ++i){
			l = pre_litPerm[i];
			if (GET_LIT_DSC(l) == 0U){
				//Literal l was not yet discovered.
				stamp = pre_intern_UNHIDING_stamp(l, stamp);
				if (pre_returnCode == PRE_UNSAT){
					#ifdef VERBOSE_PRE
					printf("T-CONFLICT.\n");fflush(stdout);
					#endif
					return;
				}
			}
		}

		//The advanced stamping of all the literals in BIG is now over. It is now time to iterate through all the clauses of
		//the formula and perform UHTE or UHLE. The following is the implementation of the SIMPLIFY algorithm from the paper.
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
			prep_intern_disableClause(c);
			//Check for UHTE -- Unhiding Hidden Tautology Elimination.
			if (GET_CLS_SIZE(c) > 2 && pre_intern_UNHIDING_uhte(c)){
				//It is a hidden tautology. We get rid of the clause for good.
				prep_intern_abandonClause(c);
			} else {
				//Otherwise, it is no hidden tautology, so we cannot get rid of the clause. We will first perform UHLE and
				//check how far it was reduced. If it is no unit clause, we re-enable it. If it is unit, we try to put its
				//literals in the UP. If this fails, we have a global conflict. If this does not fail, we can abandon the
				//clauses because we do not need units.
				#ifdef VERBOSE_PRE
				oldSize = GET_CLS_SIZE(c);
				#endif
				pre_intern_UNHIDING_uhle(c);
				#ifdef VERBOSE_PRE
				if (oldSize > GET_CLS_SIZE(c)) ++numStr;
				#endif
				if (GET_CLS_SIZE(c) == 1){
					//We have reduced it to a unit clause and must now check if we can put its literal in the UP.
					lit = GET_CLS_LITNUM(c, 0);
					if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
						//This constitutes a global conflict as we found a unit for which the opposite is enforced.
						pre_returnCode = PRE_UNSAT;
						#ifdef VERBOSE_PRE
						printf("UHLE-CONFLICT.\n");fflush(stdout);
						#endif
						return;
					} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
						//We can add the literal since it is not already enqueued.
						ADD_LIT_TO_SIMPLEUPQUEUE(lit);
					}
					//Anyway, we can abandon the clause.
					prep_intern_abandonClause(c);
				} else {
					//We did not reduce it to a unit clause. However it went with the reduction for c, it is at least binary
					//and we must keep it. We therefore enable it again. However, this operation also adds it to the clause
					//stack again which is of course something we do not want. We therefore drop it right away.
					prep_intern_enableClause(c);
					if (IS_CLS_LEARNED(c)){
						++lIgnore;
					} else {
						++oIgnore;
					}
				}
			}
		}

		//Finally, after we performed all the operations, we can propagate any unit clauses we found.
		//We now check the ELS classes and either replace all elements with the class representative or propagate all elements
		//in a class if one of them is marked for propagation. This might lead into a conflict.
		for (i = 0; i < pre_ELS_classes_used; ++i){
			//First, we check if any literal in the current class is already on the stack.
			same = 0;
			opp = 0;
			for (j = 0; j < pre_ELS_classes_memUsed[i]; ++j){
				classRep = pre_ELS_classes[i][j];
				if (IS_LIT_IN_SIMPLEUPQUEUE_PTR(classRep)){
					same = 1;
				} else if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE_PTR(classRep)){
					opp = 1;
				}
			}
			//Check, if any of the literals is marked for propagation.
			if (same && opp){
				//From the current class, there is at least one literal marked for propagation with same sign and one marked
				//for propagation with opposite sign. This constitutes a global conflict as all literals must be assigned
				//the same way since they are all equivalent.
				#ifdef VERBOSE_PRE
				printf(" EQ ");
				#endif
				pre_returnCode = PRE_UNSAT;
				return;
			}
			if (same){
				//There is at least one literal that is marked for propagation with the same sign. We add all the other
				//literals with the same sign into the UP as well.
				for (j = 0; j < pre_ELS_classes_memUsed[i]; ++j){
					classRep = pre_ELS_classes[i][j];
					if (!IS_LIT_IN_SIMPLEUPQUEUE_PTR(classRep)){
						ADD_LIT_TO_SIMPLEUPQUEUE_PTR(classRep);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			} else if (opp){
				//There is at least one literal that is marked for propagation with the opposite sign. We add all the other
				//literals with the opposite sign into the UP as well.
				for (j = 0; j < pre_ELS_classes_memUsed[i]; ++j){
					classRep = pre_ELS_classes[i][j];
					if (!IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE_PTR(classRep)){
						classRep = GET_LIT_OPPOSITE(classRep);
						ADD_LIT_TO_SIMPLEUPQUEUE_PTR(classRep);
					}
				}
				//Since all the elements in this class are now marked for propagation, there is no need to do any replacement.
				continue;
			}
			//If we get this far, we must make a replacement. For class i, we pick the class representative (first literal).
			classRep = pre_ELS_classes[i][0];
			reWID = GET_LIT_NUM_FROM_PTR(classRep);
			//For all the remaining literals in the class, we replace their occurrence with the representative.
			for (j = 1; j < pre_ELS_classes_memUsed[i]; ++j){
				//Grab the literal and variable to replace.
				l = pre_ELS_classes[i][j];
				toRID = GET_LIT_NUM_FROM_PTR(l);
				v = main_varData + abs(toRID);

				//Replace the occurrence of l with classRep.
				pre_intern_ELS_replaceLit(l, classRep);
				if (pre_returnCode != PRE_UNKNOWN){
					#ifdef VERBOSE_PRE
					printf(" CR ");
					#endif
					return;
				}

				//Replace the occurrence of -l with -classRep.
				pre_intern_ELS_replaceLit(GET_LIT_OPPOSITE(l),GET_LIT_OPPOSITE(classRep));
				if (pre_returnCode != PRE_UNKNOWN){
					#ifdef VERBOSE_PRE
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
				prep_intern_disableVariable(v);
			}
		}

		if (!IS_SIMPLEUPQUEUE_EMPTY()){
			c = main_simpleUP_propagate_prep();
			if (c != NULL){
				#ifdef VERBOSE_PRE
				printf("CONFLICT.\n");
				#endif
				pre_returnCode = PRE_UNSAT;
				return;
			} else if (f.n_vars_e_used == 0 || f.m_eo_used + f.m_el_used == 0){
				return;
			}
		}
	} while (++numRounds < numRountsToPerform);

	#ifdef VERBOSE_PRE
	printf("[%d, %d, %d] \n", oldNumVars - f.n_vars_e_used, numStr,	oldNumCls-(f.m_eo_used+f.m_el_used));
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_UNHIDING += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_UNHIDING += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

clause* pre_intern_NHBR_detect(int32_t rootLit){
	//This method propagates rootLit first. In each propagation, binary clauses are preferred.
	literal *lPtr;
	variable *v;
	clause *c;
	int32_t *upQ = main_simpleUPQueue, *checkFrom = main_simpleUPQueue,
			upLit, i, lit, offset=0, litPos, learnBinary, numBinOccsForLearnBinary;

	++main_litStamp;
	ADD_LIT_TO_SIMPLEUPQUEUE(rootLit);
	SET_LIT_STAMP((main_litData + rootLit), main_litStamp);

	do {
		//This do-while loop does the following. 1. Propagate all binaries as long as there are more. 2. Check all the newly
		//propagated false literal occurrences if any of them became empty. 3. Find a large clause that has but one not yet
		//propagated literal left. Use this to learn a new binary. 4. Stop if no new binary can be learned. 5. Learn a new
		//binary.

		//Step 1. Propagate binaries.
		while (upQ < main_simpleUPQueue_current){
			upLit =*(upQ++);				//The literal number of the literal that is to be satisfied.
			lPtr = main_litData + upLit;	//Satisfied literal.
			v = main_varData + abs(upLit);
			if (upLit > 0){
				SET_VAR_ASS_1( v );
			} else {
				SET_VAR_ASS_0( v );
			}

			//Walk through the implication list of lPtr. All the implications must be put on the UP queue. If any of them
			//fails, we can return with a conflict.
			for (i = 0; i < GET_LIT_NUMIMP_USED(lPtr); i+=2){
				lit = (int32_t) (addr_t) GET_LIT_IMPNUM(lPtr, i);
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//First, we check if the opposite literal has been stamped. If so, then there is at least one large clause
					//that needs the opposite assignment. This results in a conflict right away.
					if (GET_LIT_STAMP((main_litData - lit)) == main_litStamp){
						//Yes, this is a conflict.
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
						return GET_LIT_IMPNUM_REASON(lPtr, i);
					}
					//Otherwise, we can also propagate this buddy if the opposite is not already enqueued.
					if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(lit)){
						//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
						return GET_LIT_IMPNUM_REASON(lPtr, i);
					} else if (!IS_LIT_IN_SIMPLEUPQUEUE(lit)){
						//The element is not yet in the UP queue. We put it there and stamp it.
						ADD_LIT_TO_SIMPLEUPQUEUE(lit);
						SET_LIT_STAMP((main_litData + lit), main_litStamp);
					} //Else, the literal is already enqueued.
				}
			}

			//We have now added all the direct implications.
		}

		//Step 2 and 3. Check for conflicts and a large clause with only one unassigned literal.
		//All binary clauses that were available so far have been propagated. We must now check the watcher lists of ALL the
		//propagated opposite literals. We are looking for 1. conflicts, 2. unit clauses derived by non-binaries.
		learnBinary = 0;
		numBinOccsForLearnBinary = -1;
		while (checkFrom < main_simpleUPQueue_current){
			upLit =*(checkFrom++);
			lPtr = main_litData - upLit; //A literal that became false.

			//We now take a closer look at the watcher lists for each lPtr, and thereby take a look at large clauses. If any
			//of them became empty, we ran into a conflict and hence rootLit is a failed literal. If no conflicts arise, we
			//pick a large clause we find that is not yet satisfied and remains a unit under the current literal stamps.

			//WL1
			offset = 0;
			while (lPtr->numw1Used > offset){
				c = lPtr->w1[offset];
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = c->repLit;
				if (GET_LIT_STAMP((main_litData + lit)) == main_litStamp){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				litPos = c->whereW1;
				for (i = 0; i < c->size; ++i){
					lit = c->lits[i];
					if (GET_LIT_STAMP((main_litData + lit)) == main_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the representative
						//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						c->repLit = lit;
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					if (lit == c->w2) continue;
					if (GET_LIT_STAMP((main_litData + lit)) != main_litStamp && GET_LIT_STAMP((main_litData - lit)) != main_litStamp){
						//We have found an unassigned literal that can serve as watcher.
						c->w1 = lit;
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (c->w1 != -upLit){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W1LIST(-upLit, litPos);
					ADD_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = c->w2;
					if (GET_LIT_STAMP((main_litData - lit)) == main_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
						return c;
					} else {
						//The element is not yet in the UP queue. We can stamp it and signal that it has been used now.
						SET_LIT_STAMP((main_litData + lit), main_litStamp);
						//We can now use the literal to do some learning.
						if ((int32_t)(GET_LIT_NUMIMP_USED((main_litData + lit))) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_LIT_NUMIMP_USED((main_litData + lit));
						}
					}
				}
			}

			//WL2
			offset = 0;
			while (lPtr->numw2Used > offset){
				c = lPtr->w2[offset];
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = c->repLit;
				if (GET_LIT_STAMP((main_litData + lit)) == main_litStamp){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				litPos = c->whereW2;
				for (i = 0; i < c->size; ++i){
					lit = c->lits[i];
					if (GET_LIT_STAMP((main_litData + lit)) == main_litStamp){
						//We have found a literal in the clause that satisfies the clause. This should have been the representative
						//literal. We make it so. Since the clause is satisfied, we do not need to change the watcher.
						c->repLit = lit;
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					if (lit == c->w1) continue;
					if (GET_LIT_STAMP((main_litData + lit)) != main_litStamp && GET_LIT_STAMP((main_litData - lit)) != main_litStamp){
						//We have found an unassigned literal that can serve as watcher.
						c->w2 = lit;
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (c->w2 != -upLit){
					//Yes, we can perform the replacement.
					REM_CLS_FROM_LIT_W2LIST(-upLit, litPos);
					ADD_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = c->w1;
					if (GET_LIT_STAMP((main_litData - lit)) == main_litStamp){
						//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
						UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
						RESET_SIMPLEUPQUEUE();
						return c;
					} else {
						//The element is not yet in the UP queue. We can stamp it and signal that it has been used now.
						SET_LIT_STAMP((main_litData + lit), main_litStamp);
						//We can now use the literal to do some learning.
						if ((int32_t)(GET_LIT_NUMIMP_USED((main_litData + lit))) > numBinOccsForLearnBinary){
							learnBinary = lit;
							numBinOccsForLearnBinary = GET_LIT_NUMIMP_USED((main_litData + lit));
						}
					} //Else, the literal is already enqueued.
				}
			}
		}

		//Step 4. Stop if no new binary can be learned.
		if (!learnBinary) break;

		//Step 5. Learn a new binary and add it.
		//We can now learn the new binary (-rootLit OR learnBinary).
		prep_intern_addLearnedBinaryClause(-rootLit, learnBinary);

		//The implied binary must now be added to the UP queue.
		ADD_LIT_TO_SIMPLEUPQUEUE(learnBinary);
		SET_LIT_STAMP((main_litData + learnBinary), main_litStamp);
	} while (1);

	UNASSIGN_SIMPLEUPQUEUE(v, upQ, 0);
	RESET_SIMPLEUPQUEUE();
	return NULL;
}

void pre_extern_NHBR(){
	//Non-transitive hyper binary resolution.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	uint32_t numClsActive = f.m_eo_used + f.m_el_used;
	uint32_t numVarsActive = f.n_vars_e_used;
	#endif
	literal *l;
	clause* conflict;
	int32_t litID;

	#ifdef VERBOSE_PRE
	int32_t oldNumCls = f.m_eo_used + f.m_el_used;
	int32_t oldNumVars = f.n_vars_e_used;
	int32_t numMsg = 0;
	printf("c     PRE: NHBR[%u]: ", lQ_leaf-1);fflush(stdout);
	#endif

	//Then, we walk through the literal permutation and satisfy each of the roots (if the literal is still a root).
	while ((l = lQ_dequeueBest()) != NULL){
		litID = GET_LIT_NUM_FROM_PTR(l);
		//We ignore literals of variables that are already assigned.
		if (IS_LIT_ASSIGNED(litID) || IS_VAR_DISABLED((main_varData + abs(litID)))) continue;
		//Literal l is now the literal we want to satisfy. Check if the literal l is a root or if it is already removed.
		//Since lOpp has no implications l must be a root. We try and detect with it.
		conflict = pre_intern_NHBR_detect(litID);
		if (conflict != NULL){
			//Literal l is a failed literal, we must propagate the opposite.
			ADD_LIT_TO_SIMPLEUPQUEUE(-litID);
			#ifdef VERBOSE_PRE
			printf("F%d ", litID);
			if (++numMsg % 10 == 0){
				printf("\nc     PRE: NHBR: ");fflush(stdout);
			}
			#endif
			conflict = main_simpleUP_propagate_prep();
			if (conflict != NULL){
				//Both sides are failed. This means the instance is unsatisfiable.
				pre_returnCode = PRE_UNSAT;
				#ifdef VERBOSE_PRE
				printf(" global conflict.\n");
				#endif
				return;
			}
		}//Else no conflict was detected and (maybe) new binary clauses have been added by the detect function.
	}

	#ifdef VERBOSE_PRE
	printf("\nc     PRE:\tDone, removed %d clauses, removed %u variables.\n",
			oldNumCls-(int32_t)(f.m_eo_used+f.m_el_used), oldNumVars - (int32_t)f.n_vars_e_used);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_NHBR += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_NHBR += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_extern_TERNARY(){
	//Ternary resolution.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	uint32_t numClsActive = f.m_eo_used + f.m_el_used;
	uint32_t numVarsActive = f.n_vars_e_used;
	#endif
	#ifdef VERBOSE_PRE
	int32_t oldNumCls = f.m_eo_used + f.m_el_used, numLearntWith = 0;
	printf("c     PRE: TERNARY[%u]: ", lS_leaf-1);fflush(stdout);
	#endif

	clause *c, *d;
	literal *l, *lOpp, *lNew;
	uint64_t sig;
	uint32_t lOccNum, lOppOccNum, sameCount, oppCount, numLearnt, numLearntTotal = 0;
	int32_t lit1, lit2, lit3, imp;


	//We pick a literal that we want to do ternary resolution with.
	while ((l = lS_pop()) != NULL){
		numLearnt = 0;
		lOpp = GET_LIT_OPPOSITE(l);
		if (GET_LIT_NUMOCCS_USED(l) == 0 || GET_LIT_NUMOCCS_USED(lOpp) == 0) continue;
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
				SET_LIT_STAMP((main_litData + GET_CLS_LITNUM(c,0)), main_litStamp);
				SET_LIT_STAMP((main_litData + GET_CLS_LITNUM(c,1)), main_litStamp);
				SET_LIT_STAMP((main_litData + GET_CLS_LITNUM(c,2)), main_litStamp);
				if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,0))) == main_litStamp) ++sameCount;
				if (GET_LIT_STAMP((main_litData - GET_CLS_LITNUM(d,0))) == main_litStamp) ++oppCount;
				if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,1))) == main_litStamp) ++sameCount;
				if (GET_LIT_STAMP((main_litData - GET_CLS_LITNUM(d,1))) == main_litStamp) ++oppCount;
				if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,2))) == main_litStamp) ++sameCount;
				if (GET_LIT_STAMP((main_litData - GET_CLS_LITNUM(d,2))) == main_litStamp) ++oppCount;

				//Now, if sameCount is not equal 1 or oppCount is larger 1, we do not resolve. Either they have no
				//clashing literal as initially assumed, or they have more than the resolvent literal with opposite sign in
				//common. In any case, the result would be no ternary clause.
				if (sameCount != 1 || oppCount > 1) continue;

				//If we reach this position, we can resolve. We grab from clause d the literal that has the same stamp and
				//the literal that is not stamped at all. From clause c we grab the literal that is not literal 1 or 2 and
				//not the literal we resolve on.
				if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,0))) == main_litStamp){
					lit1 = GET_CLS_LITNUM(d,0);
				} else if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,1))) == main_litStamp){
					lit1 = GET_CLS_LITNUM(d,1);
				} else {
					lit1 = GET_CLS_LITNUM(d,2);
				}

				if (       GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,0))) != main_litStamp
						&& GET_LIT_STAMP((main_litData - GET_CLS_LITNUM(d,0))) != main_litStamp){
					lit2 = GET_CLS_LITNUM(d,0);
				} else if (GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,1))) != main_litStamp
						&& GET_LIT_STAMP((main_litData - GET_CLS_LITNUM(d,1))) != main_litStamp){
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
				SET_LIT_STAMP((main_litData + lit1), main_litStamp);
				SET_LIT_STAMP((main_litData + lit2), main_litStamp);
				SET_LIT_STAMP((main_litData + lit3), main_litStamp);
				sig = 0ULL;
				sig |= GET_LIT_SIG(lit1);
				lNew = main_litData + lit1;
				sig |= GET_LIT_SIG(lit2);
				if (GET_LIT_NUMOCCS_USED((main_litData + lit2)) < GET_LIT_NUMOCCS_USED(lNew)) lNew = main_litData + lit2;
				sig |= GET_LIT_SIG(lit3);
				if (GET_LIT_NUMOCCS_USED((main_litData + lit3)) < GET_LIT_NUMOCCS_USED(lNew)) lNew = main_litData + lit3;
				//Literal lNew is now the literal that has the smallest occurrence list. We check all the clauses in here if
				//any of them matches the signature.
				for (sameCount = 0; sameCount < GET_LIT_NUMOCCS_USED(lNew); ++sameCount){
					d = GET_LIT_OCCNUM(lNew, sameCount);
					if (GET_CLS_SIZE(d) != 3 || sig != GET_CLS_SIG(d)) continue;
					//Here, it might be that clause d is equal to what we can learn. We check if d contains lit1, lit2
					//and lit3 via looking at the stamps.
					if (       GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,0))) == main_litStamp
							&& GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,1))) == main_litStamp
							&& GET_LIT_STAMP((main_litData + GET_CLS_LITNUM(d,2))) == main_litStamp){
						//All three literals match. We cannot learn the new binary as it is equal to d.
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
					if (GET_LIT_STAMP((main_litData + imp)) == main_litStamp){
						break;
					}
				}
				if (sameCount < GET_LIT_NUMIMP_USED(lNew)){
					continue;
				}
				lNew = main_litData - lit2;
				for (sameCount = 0; sameCount < GET_LIT_NUMIMP_USED(lNew); sameCount += 2){
					imp = GET_LIT_IMPNUM(lNew, sameCount);
					if (GET_LIT_STAMP((main_litData + imp)) == main_litStamp){
						break;
					}
				}
				if (sameCount < GET_LIT_NUMIMP_USED(lNew)){
					continue;
				}
				//Here at this point, the clause we can learn passed all the tests. We add it.
				prep_intern_addLearnedTernaryClause(lit1, lit2, lit3);
				if (++numLearnt == param_prepTERNARYLitMax) break;
			}
			if (numLearnt == param_prepTERNARYLitMax) break;
		}
		#ifdef VERBOSE_PRE
		++numLearntWith;
		#endif
		numLearntTotal += numLearnt;
		if (numLearntTotal >= param_prepTERNARYMax) break;
	}
	lS_flush();


	#ifdef VERBOSE_PRE
	printf("Learned %d ternary clauses resolving on %d resolution variables.\n",
			abs(oldNumCls-(int32_t)(f.m_eo_used+f.m_el_used)), numLearntWith);
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_TERNARY += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_TERNARY += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_intern_ELS_replaceLit(literal *toReplace, literal *replaceWith){
	clause *c;
	uint64_t sig;
	uint32_t occLitPosition, found, taut, i;
	int32_t toRID = GET_LIT_NUM_FROM_PTR(toReplace);
	int32_t reWID = GET_LIT_NUM_FROM_PTR(replaceWith);

	while (GET_LIT_NUMOCCS_USED(toReplace) > 0){
		//Grab the next clause that contains toReplace.
		c = GET_LIT_OCCNUM(toReplace, 0);
		occLitPosition = GET_LIT_OCCLITPOSNUM(toReplace, 0);

		sig = 0ULL;
		found = 0;
		taut = 0;

		//Several things are performed in the following loop: 1. Look at all the literals and grab the signature bit of those
		//not equal to toReplace. 2. See if replaceWith is already in the clause.
		for (i = 0; i < GET_CLS_SIZE(c); ++i){
			//Skip the literal we want to replace.
			if (GET_CLS_LITNUM(c,i) == toRID){
				continue;
			}
			//Retain if the literal we want to replace with is already found.
			if (GET_CLS_LITNUM(c,i) == reWID){
				found = 1;
			}
			//Check if the opposite of the replacement is in the clause.
			if (GET_CLS_LITNUM(c,i) == -reWID){
				taut = 1;
				break;
			}

			//Update the signature with all the literals we encounter not equal to the one we replace.
			sig |= GET_LIT_SIG(GET_CLS_LITNUM(c,i));
		}

		//If the clause becomes a tautology, we just drop it.
		if (taut){
			prep_intern_abandonClause(c);
			continue;
		}

		//Disable the clause.
		prep_intern_disableClause(c);

		//If we have not found the literal we replace with, we must add its signature bit manually and write the replacement
		//literal to the position where the old literal was.
		if (!found){
			//We did not find the literal we replace with. We manually add the signature bit. We override literal.
			sig |= GET_LIT_SIG(reWID);
			SET_CLS_LITNUM(c, occLitPosition, reWID);
		} else {
			//We did find the replacement literal. This means that we effectively remove a literal from this clause.
			//We override the position of the literal to replace with the last literal in the clause.
			SET_CLS_SIZE_DEC(c);
			SET_CLS_LITNUM(c, occLitPosition, GET_CLS_LITNUM(c, GET_CLS_SIZE(c)));
			SET_CLS_OCCPOSNUM(c, occLitPosition, GET_CLS_OCCPOSNUM(c, GET_CLS_SIZE(c)));
			SET_CLS_MSGNUM(c, occLitPosition, GET_CLS_MSGNUM(c, GET_CLS_SIZE(c)));
		}
		//If the clause became unit, we do not have to re-enable it. If not, we can re-enable it normally.
		if (GET_CLS_SIZE(c) == 1){
			//The clause became unit because of the replacement. We can now learn that -reWID is a failed literal and put
			//reWID on the stack. If this fails it constitutes a global conflict.
			if (IS_OPPOSITE_LIT_IN_SIMPLEUPQUEUE(reWID)){
				//This constitutes a global conflict.
				pre_returnCode = PRE_UNSAT;
				return;
			} else if (!IS_LIT_IN_SIMPLEUPQUEUE(reWID)){
				//We can add the literal to the UP queue.
				ADD_LIT_TO_SIMPLEUPQUEUE(reWID);
			}//Else the literal is already enqueued.
			//This unit is now taken care of.
			prep_intern_abandonClause(c);
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
			prep_intern_enableClause(c);
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
			prep_intern_enableClause(c);
		}
		//The literal has now been removed from the clause successfully.
	}
}

uint32_t pre_intern_ELS_strongConnect(literal* l, uint32_t index){
	literal *impLit;
	uint32_t i;

	SET_LIT_ELS_INDEX(l,index);
	SET_LIT_ELS_LOWLINK(l, index);
	++index;
	*(pre_ELS_lStack_current++) = l;
	SET_LIT_ELS_ISINS(l, 1U);

	for (i= 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
		impLit = main_litData + GET_LIT_IMPNUM(l,i);
		if (GET_LIT_ELS_INDEX(impLit) == 0U){
			//Not yet marked. Recurse on implied literal.
			index = pre_intern_ELS_strongConnect(impLit, index);
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
		PRE_ENLARGE_ELS_CLASSES_IF_NECESSARY(i);
		do {
			impLit = *(--pre_ELS_lStack_current);
			SET_LIT_ELS_ISINS(impLit, 0U);

			if (GET_LIT_STAMP(impLit) != main_litStamp){
				PRE_ADD_LIT_TO_CURRENT_ELS_CLASS(impLit);
				SET_LIT_STAMP(impLit, main_litStamp);
				SET_LIT_STAMP(GET_LIT_OPPOSITE(impLit), main_litStamp);
			}
		} while (impLit != l);
		//If the class now contains more than one element it contains useful information. If not, we can reset the class
		//as it contains only one element.
		if (pre_ELS_classes_memUsed[pre_ELS_classes_used] > 1U){
			//Yes, some literals in this class are defined to be equivalent.
			++pre_ELS_classes_used;
		} else {
			//No, we can revert the class as it would contain only one element.
			pre_ELS_classes_memUsed[pre_ELS_classes_used] = 0;
		}
	}

	return index;
}

void pre_extern_ELS(){
	//Performs equivalent literals substitution using Tarjan's algorithm.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	uint32_t numClsActive = f.m_eo_used + f.m_el_used;
	uint32_t numVarsActive = f.n_vars_e_used;
	#endif
	#ifdef VERBOSE_PRE
	int32_t oldNumVars = f.n_vars_e_used;
	int32_t oldNumCls = f.m_eo_used + f.m_el_used;
	int32_t numFoundClasses = 0;
	printf("c     PRE: ELS[%u]: ", 2*f.n_vars_e_used);fflush(stdout);
	#endif
	literal *l, *classRep;
	variable *v;
	uint32_t i,j, index = 1;
	int32_t toRID, reWID;


	//Initialize the ELS literal stack to be empty.
	pre_ELS_lStack_current = pre_ELS_lStack + 1;

	//We prepare the literal stamp to present classes only once.
	++main_litStamp;

	//Initialize the classes information.
	pre_ELS_classes_used = 0;
	for (i = 0; i < pre_ELS_classes_avail; ++i){
		pre_ELS_classes_memUsed[i] = 0;
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
			index = pre_intern_ELS_strongConnect(l, index);
		}
		l = GET_LIT_OPPOSITE(l);
		if (GET_LIT_ELS_INDEX(l) == 0){
			index = pre_intern_ELS_strongConnect(l, index);
		}
	}

	//We walk through all the found equivalence classes and replace the literals.
	#ifdef VERBOSE_PRE
	numFoundClasses = pre_ELS_classes_used;
	#endif
	for (i = 0; i < pre_ELS_classes_used; ++i){
		//For class i, we pick the class representative (the first literal in the class).
		classRep = pre_ELS_classes[i][0];
		reWID = GET_LIT_NUM_FROM_PTR(classRep);
		//For all the remaining literals in the class, we replace their occurrence with the representative.
		for (j = 1; j < pre_ELS_classes_memUsed[i]; ++j){
			//Grab the literal and variable to replace.
			l = pre_ELS_classes[i][j];
			toRID = GET_LIT_NUM_FROM_PTR(l);
			v = main_varData + abs(toRID);

			//Replace the occurrence of l with classRep.
			pre_intern_ELS_replaceLit(l, classRep);
			if (pre_returnCode == PRE_UNSAT){
				#ifdef VERBOSE_PRE
				printf("representative conflict.\n");
				#endif
				return;
			}

			//Replace the occurrence of -l with -classRep.
			pre_intern_ELS_replaceLit(GET_LIT_OPPOSITE(l), GET_LIT_OPPOSITE(classRep));
			if (pre_returnCode == PRE_UNSAT){
				#ifdef VERBOSE_PRE
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
			prep_intern_disableVariable(v);
		}
	}

	//Finally, we perform unit propagation.
	if (main_simpleUP_propagate_prep() != NULL){
		pre_returnCode = PRE_UNSAT;
		#ifdef VERBOSE_PRE
		printf("UP conflict.\n");
		#endif
		return;
	}

	#ifdef VERBOSE_PRE
	printf("%d[%d,%d]\n", numFoundClasses, oldNumVars - (int32_t)f.n_vars_e_used,
			oldNumCls-(int32_t)(f.m_eo_used+f.m_el_used));
	#endif
	#ifdef COLLECTSTATS
	stats_pre_varRemoved_ELS += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved_ELS += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_varRemoved += (numVarsActive - (int32_t)f.n_vars_e_used);
	stats_pre_clsRemoved += (numClsActive - (int32_t)f.m_el_used - (int32_t)f.m_eo_used);
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	#endif
	#ifdef VERBOSE_PRE
	printf("c     PRE: Component reset...\n");
	#endif
	pre_returnCode = PRE_UNKNOWN;
	pre_res = NULL;
	pre_resAvail = 0;
	pre_resUsed = 0;
	pre_resMemAvail = NULL;
	pre_resMemUsed = NULL;
	pre_covLits = NULL;
	pre_covLitsUsed = 0;
	pre_G_negV = NULL;
	pre_G_negVAvail = 0;
	pre_G_negVUsed = 0;
	pre_G_posV = NULL;
	pre_G_posVAvail = 0;
	pre_G_posVUsed = 0;
	pre_UNHIDING_lStack = NULL;
	pre_UNHIDING_lStack_current = NULL;
	pre_UNHIDING_SPlus = NULL;
	pre_UNHIDING_SMinus = NULL;
	pre_litPerm = NULL;
	pre_ELS_classes = NULL;
	pre_ELS_classes_avail = 0;
	pre_ELS_classes_used = 0;
	pre_ELS_classes_memUsed = NULL;
	pre_ELS_lStack = NULL;
	pre_ELS_lStack_current = NULL;

	#ifdef COLLECTSTATS
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_initModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	#endif
	#ifdef VERBOSE_PRE
	printf("c     PRE: Component init...\n");
	#endif
	int32_t i;
	pre_returnCode = PRE_UNKNOWN;
	//We initialize the resolvents and the corresponding data-structures.
	pre_resUsed = 0;
	pre_resAvail = 2*BLOCKSIZE;
	pre_res = malloc(sizeof(int32_t*) * pre_resAvail);
	if (pre_res == NULL){
		printf("c ERROR. Failed to initialize the resolvents array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_resMemAvail = malloc(sizeof(int32_t) * pre_resAvail);
	if (pre_resMemAvail == NULL){
		printf("c ERROR. Failed to initialize the resolvents memory availability array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_resMemUsed = malloc(sizeof(int32_t) * pre_resAvail);
	if (pre_resMemAvail == NULL){
		printf("c ERROR. Failed to initialize the resolvents memory usage array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	if (pre_returnCode == PRE_ERROR) {
		if (pre_res != NULL){
			for (i = 0; i < pre_resAvail; ++i){	pre_res[i] = NULL; }
		}
		return;
	}
	for (i = 0; i < pre_resAvail; ++i){
		if (pre_returnCode == PRE_UNKNOWN){
			pre_res[i] = NULL;
			pre_res[i] = malloc(sizeof(int32_t)*BLOCKSIZE);
			if (pre_res[i] == NULL) {
				pre_returnCode = PRE_ERROR;
				pre_resMemAvail[i] = 0;
				pre_resMemUsed[i] = 0;
			} else {
				pre_resMemAvail[i] = BLOCKSIZE;
				pre_resMemUsed[i] = 0;
			}
		} else {
			pre_res[i] = NULL;
			pre_resMemAvail[i] = 0;
			pre_resMemUsed[i] = 0;
		}
	}

	//Then we initialize the covered literals array.
	pre_covLits = malloc(sizeof(coveredLit)*f.n_initial);
	if (pre_covLits == NULL){
		printf("c ERROR. Failed to initialize the covered literals array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_covLitsUsed = 0;

	//Then we initialize the gate extraction related arrays.
	pre_G_negVAvail = BLOCKSIZE;
	pre_G_negVUsed = 0;
	pre_G_negV = malloc(sizeof(clause*)*pre_G_negVAvail);
	if (pre_G_negV == NULL){
		printf("c ERROR. Failed to initialize the gate extraction array G_negV. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_G_posVAvail = BLOCKSIZE;
	pre_G_posVUsed = 0;
	pre_G_posV = malloc(sizeof(clause*)*pre_G_posVAvail);
	if (pre_G_posV == NULL){
		printf("c ERROR. Failed to initialize the gate extraction array G_posV. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_UNHIDING_lStack = malloc(sizeof(literal*)*(2*f.n_initial+2));
	if (pre_UNHIDING_lStack == NULL){
		printf("c ERROR. Failed to initialize the gate UNHIDING literal stack. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_UNHIDING_lStack[0] = NULL;
	pre_UNHIDING_lStack_current = pre_UNHIDING_lStack+1;

	pre_UNHIDING_SPlus = malloc(sizeof(literal*)*f.n_initial);
	if (pre_UNHIDING_SPlus == NULL){
		printf("c ERROR. Failed to initialize the Unhiding SPlus array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_UNHIDING_SMinus = malloc(sizeof(literal*)*f.n_initial);
	if (pre_UNHIDING_SMinus == NULL){
		printf("c ERROR. Failed to initialize the Unhiding SMinus array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_litPerm = malloc(sizeof(literal*)*(2*f.n_initial+1));
	if (pre_litPerm == NULL){
		printf("c ERROR. Failed to initialize the litPerm array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_ELS_classes_avail = BLOCKSIZE;
	pre_ELS_classes_used = 0;

	pre_ELS_classes = malloc(sizeof(literal**) * pre_ELS_classes_avail);
	if (pre_ELS_classes == NULL){
		printf("c ERROR. Failed to initialize the ELS classes array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_ELS_classes_memUsed = malloc(sizeof(uint32_t) * pre_ELS_classes_avail);
	if (pre_ELS_classes_memUsed == NULL){
		printf("c ERROR. Failed to initialize the ELS memory usage array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	pre_ELS_classes_memAvail = malloc(sizeof(uint32_t) * pre_ELS_classes_avail);
	if (pre_ELS_classes_memAvail == NULL){
		printf("c ERROR. Failed to initialize the ELS memory availability array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}

	for (i = 0; i < pre_ELS_classes_avail; ++i){
		if (pre_returnCode == PRE_UNKNOWN){
			pre_ELS_classes[i] = NULL;
			pre_ELS_classes[i] = malloc(sizeof(literal*) * BLOCKSIZE);
			if (pre_ELS_classes[i] == NULL){
				pre_returnCode = PRE_ERROR;
				pre_ELS_classes_memUsed[i] = 0;
				pre_ELS_classes_memAvail[i] = 0;
			} else {
				pre_ELS_classes_memUsed[i] = 0;
				pre_ELS_classes_memAvail[i] = BLOCKSIZE;
			}
		} else {
			pre_ELS_classes[i] = NULL;
			pre_ELS_classes_memUsed[i] = 0;
			pre_ELS_classes_memAvail[i] = 0;
		}
	}

	pre_ELS_lStack = malloc(sizeof(literal*)*(2*f.n_initial + 2));
	if (pre_ELS_lStack == NULL){
		printf("c ERROR. Failed to initialize the ELS literal stack array. Out of memory?\n");
		pre_returnCode = PRE_ERROR;
		return;
	}
	pre_ELS_lStack[0] = NULL;
	pre_ELS_lStack_current = pre_ELS_lStack + 1;

	#ifdef COLLECTSTATS
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void pre_disposeModule(){
	//Dispose the PRE component.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_pre_component_totalCalls;
	#endif
	#ifdef VERBOSE_PRE
	printf("c     PRE: Component dispose...\n");
	#endif
	int32_t i;
	if (pre_res != NULL){
		for (i = 0; i < pre_resAvail; ++i){
			if (pre_res[i] != NULL){
				free(pre_res[i]);
				pre_res[i] = NULL;
			}
		}
		free(pre_res);
		pre_res = NULL;
	}
	pre_resAvail = 0;
	pre_resUsed = 0;

	if (pre_resMemAvail != NULL){
		free(pre_resMemAvail);
		pre_resMemAvail = NULL;
	}

	if (pre_resMemUsed != NULL){
		free(pre_resMemUsed);
		pre_resMemUsed = NULL;
	}

	if (pre_covLits != NULL){
		free(pre_covLits);
		pre_covLits = NULL;
	}

	if (pre_G_negV != NULL){
		free(pre_G_negV);
		pre_G_negV = NULL;
	}
	if (pre_G_posV != NULL){
		free(pre_G_posV);
		pre_G_posV = NULL;
	}

	if (pre_UNHIDING_lStack != NULL){
		free(pre_UNHIDING_lStack);
		pre_UNHIDING_lStack = NULL;
		pre_UNHIDING_lStack_current = NULL;
	}

	if (pre_UNHIDING_SPlus != NULL){
		free(pre_UNHIDING_SPlus);
		pre_UNHIDING_SPlus = NULL;
	}

	if (pre_UNHIDING_SMinus != NULL){
		free(pre_UNHIDING_SMinus);
		pre_UNHIDING_SMinus = NULL;
	}

	if (pre_litPerm != NULL){
		free(pre_litPerm);
		pre_litPerm = NULL;
	}

	if (pre_ELS_classes != NULL){
		for (i = 0; i < pre_ELS_classes_avail; ++i){
			if (pre_ELS_classes[i] != NULL){
				free(pre_ELS_classes[i]);
				pre_ELS_classes[i] = NULL;
			}
		}
		free(pre_ELS_classes);
		pre_ELS_classes = NULL;
	}
	pre_ELS_classes_avail = 0;
	pre_ELS_classes_used = 0;

	if (pre_ELS_classes_memUsed != NULL){
		free(pre_ELS_classes_memUsed);
		pre_ELS_classes_memUsed = NULL;
	}
	if (pre_ELS_classes_memAvail != NULL){
		free(pre_ELS_classes_memAvail);
		pre_ELS_classes_memAvail = NULL;
	}

	if (pre_ELS_lStack != NULL){
		free(pre_ELS_lStack);
		pre_ELS_lStack = NULL;
		pre_ELS_lStack_current = NULL;
	}

	pre_returnCode = PRE_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_pre_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
