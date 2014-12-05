/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../dimetheus.h"

void clauses_reset(){

}

void clauses_dispose(){

}

void clauses_addOrig(int32_t* literals, int32_t size){
	//This will add the literals array as a new clause to the formula f.
	clause* c;
	int32_t j,lit;
	float_ty msg = pow(0.5f, size-1);
	if (msg < param_mpEpsilon){
		//The message would be too small. We make it big enough.
		msg = param_mpEpsilon + 0.01f;
	}
	//We allocate the memory for the clause.
	ALLOCATE_ORIGINAL_CLS(c, size, literals[0]);

	for (j=0; j < size; ++j) {
		//We grab the literal we want in our clause.
		lit = literals[j];

		//We store the literal into the clause and update the clauses signature.
		c->lits[j] = lit;
		c->msgs[j] = msg;
		c->sig |= GET_LIT_SIG(lit);

		//Add the clause into the occurrence lists of the literal.
		ADD_CLS_TO_LIT_OCCS(c, lit, j);
	}

	//We update the watchers of a clause if and only if it is not binary. If it is, we just add the binary
	//partners to the binary array of each literal, such that we know what is implied.
	if (c->size > 2){
		//Update the watchers of the clause. By default, the first two literals are the watchers.
		c->w1 = c->lits[0];
		c->w2 = c->lits[1];
		ADD_CLS_TO_LIT_W1LIST(c, c->lits[0]);
		ADD_CLS_TO_LIT_W2LIST(c, c->lits[1]);
	} else {
		//Update the binary partners for the literals. For each of the two literal, put the other literal in the
		//implies array of the literal with opposite sign. In such a way, whenever we unit propagate, we must
		//only copy all the literals found in the array.
		ADD_LIT_IMPLICATIONS(c, lit, j);
	}

	//We increase the number of clauses we have.
	++f.m_eo_used;

	//We finally check if the clause memory is used up.
	if (f.m_eo_avail == f.m_eo_used){
		//We need to increase the amount of memory usable for clause addresses.
		f.m_eo_avail *= 2;
		f.clauses_eo = realloc(f.clauses_eo, sizeof(clause**)*f.m_eo_avail);
	}
}

void clauses_addLearned(int32_t* literals, int32_t size){
	//This will add the literals array as a new learned clause to the formula f.
	clause* c; int32_t j,lit;
	float_ty msg = pow(0.5f, size-1);
	if (msg < param_mpEpsilon){
		//The message would be too small. We make it big enough.
		msg = param_mpEpsilon + 0.01f;
	}

	//We allocate the memory for the clause.
	ALLOCATE_LEARNED_CLS(c, size, literals[0]);

	for (j=0; j < size; ++j) {
		//We grab the literal we want in our clause.
		lit = literals[j];

		//We store the literal into the clause and update the clauses signature.
		c->lits[j] = lit;
		c->msgs[j] = msg;
		c->sig |= GET_LIT_SIG(lit);

		//Add the clause into the occurrence lists of the literal.
		ADD_CLS_TO_LIT_OCCS(c, lit, j);
	}

	//We update the watchers of a clause if and only if it is not binary. If it is, we just add the binary
	//partners to the binary array of each literal, such that we know what is implied.
	if (c->size > 2){
		//Update the watchers of the clause. By default, the first two literals are the watchers.
		c->w1 = c->lits[0];
		c->w2 = c->lits[1];
		ADD_CLS_TO_LIT_W1LIST(c, c->w1);
		ADD_CLS_TO_LIT_W2LIST(c, c->w2);
	} else {
		//Update the binary partners for the literals. For each of the two literal, put the other literal in the
		//implies array of the literal with opposite sign. In such a way, whenever we unit propagate, we must
		//only copy all the literals found in the array.
		ADD_LIT_IMPLICATIONS(c, lit, j);
	}

	//We increase the number of clauses we have.
	++f.m_el_used;

	//We finally check if the clause memory is used up.
	if (f.m_el_avail == f.m_el_used){
		//We need to increase the amount of memory usable for clause addresses.
		f.m_el_avail *= 2;
		f.clauses_el = realloc(f.clauses_el, sizeof(clause**)*f.m_el_avail);
	}
}

void clauses_addLearnedBinary(int32_t lit1, int32_t lit2){
	//For adding a single learned clause provided the two literals.
	clause *c;

	//Allocate the memory for our new binary clause.
	ALLOCATE_LEARNED_CLS(c, 2, lit1);

	//Add the two literals to the clause.
	c->lits[0] = lit1;
	c->lits[1] = lit2;

	//Set the clause messages.
	c->msgs[0] = 0.5;
	c->msgs[1] = 0.5;

	//Set the signature of the clause.
	c->sig |= GET_LIT_SIG(lit1);
	c->sig |= GET_LIT_SIG(lit2);

	//Add the clause to the occurrence lists.
	ADD_CLS_TO_LIT_OCCS(c, lit1, 0);
	ADD_CLS_TO_LIT_OCCS(c, lit2, 1);

	//Add the clauses implications.
	ADD_LIT_IMPLICATIONS(c, lit1, lit2);

	//We increase the number of clauses we have.
	++f.m_el_used;

	//We finally check if the clause memory is used up.
	if (f.m_el_avail == f.m_el_used){
		//We need to increase the amount of memory usable for clause addresses.
		f.m_el_avail *= 2;
		f.clauses_el = realloc(f.clauses_el, sizeof(clause**)*f.m_el_avail);
	}
}

void clauses_addLearnedTernary(int32_t lit1, int32_t lit2, int32_t lit3){
	//For adding a single learned clause provided the two literals.
	clause *c;

	//Allocate the memory for our new binary clause.
	ALLOCATE_LEARNED_CLS(c, 3, lit1);

	//Add the two literals to the clause.
	c->lits[0] = lit1;
	c->lits[1] = lit2;
	c->lits[2] = lit3;

	//Set the clause messages.
	c->msgs[0] = 0.25;
	c->msgs[1] = 0.25;
	c->msgs[2] = 0.25;

	//Set the signature of the clause.
	c->sig |= GET_LIT_SIG(lit1);
	c->sig |= GET_LIT_SIG(lit2);
	c->sig |= GET_LIT_SIG(lit3);

	//Add the clause to the occurrence lists.
	ADD_CLS_TO_LIT_OCCS(c, lit1, 0);
	ADD_CLS_TO_LIT_OCCS(c, lit2, 1);
	ADD_CLS_TO_LIT_OCCS(c, lit3, 2);

	//Set the watchers.
	c->w1 = c->lits[0];
	c->w2 = c->lits[1];
	ADD_CLS_TO_LIT_W1LIST(c, c->w1);
	ADD_CLS_TO_LIT_W2LIST(c, c->w2);

	//We increase the number of clauses we have.
	++f.m_el_used;

	//We finally check if the clause memory is used up.
	if (f.m_el_avail == f.m_el_used){
		//We need to increase the amount of memory usable for clause addresses.
		f.m_el_avail *= 2;
		f.clauses_el = realloc(f.clauses_el, sizeof(clause**)*f.m_el_avail);
	}
}


void clauses_deleteCls(clause* c){
	int32_t i, lit, pos;
	//For completely deleting clause c from the formula f. If it is yet enabled, we remove it from the lists. If it is not
	//enabled, we just remove its memory and address from the respective array.

	//First, we check if the clause is still enabled.
	if (IS_CLS_DISABLED(c)){
		//The clause is disabled. We just have to remove it from the address list.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//The clause is a learned clause. We work with f.clauses_dl.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_dl , pos , f.m_dl_used);
		} else {
			//The clause is an original clause. We work with f.clauses_do.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_do , pos , f.m_do_used);
		}
	} else {
		//The clause is enabled.
		if (c->size > 2){
			//We will remove the clause from all the watcher lists, if the clause is indeed not binary.
			lit = c->w1;
			pos = c->whereW1;
			REM_CLS_FROM_LIT_W1LIST(lit, pos);
			lit = c->w2;
			pos = c->whereW2;
			REM_CLS_FROM_LIT_W2LIST(lit, pos);
		} else {
			//It is a binary clause and we must remove the implications it imposes.
			REM_LIT_IMPLICATIONS(c, lit, pos);
		}

		//Then, we will remove the clause from the occurrence lists.
		for (i = 0; i < c->size; ++i){
			lit = c->lits[i];
			pos = c->occPos[i];
			REM_CLS_FROM_LIT_OCCS(lit, pos);
		}

		//Then, we will update the address lists.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//The clause is a learned clause. We work with f.clauses_el.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_el , pos , f.m_el_used);
		} else {
			//The clause is an original clause. We work with f.clauses_eo.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_eo , pos , f.m_eo_used);
		}
	}

	//Then, we will free all the memory of the arrays in the clause, and finally, the clause itself.
	free(c->lits);
	free(c->msgs);
	free(c->occPos);
	free(c);
}

void clauses_deleteLastNClauses(clause** list, int32_t delCount, int32_t* useCount){
	clause* c;
	int32_t i, j, lit, pos;
	//For deleting the last delCount clauses from list (beginning with element *useCount-delCount). The counter for how many
	//elements are in the list, *useCount, is updated automatically. Thats why it is a pointer.
	for (i = *useCount - delCount; i < *useCount; ++i){
		c = *(list + i);
		if (!IS_CLS_DISABLED(c)){
			//The clause is enabled. We will remove the clause from all the watcher lists, if the clause is indeed not binary.
			if (c->size > 2){
				lit = c->w1;
				pos = c->whereW1;
				REM_CLS_FROM_LIT_W1LIST(lit, pos);
				lit = c->w2;
				pos = c->whereW2;
				REM_CLS_FROM_LIT_W2LIST(lit, pos);
			} else {
				//It is a binary clause and we must remove the implications it imposes.
				REM_LIT_IMPLICATIONS(c, lit, pos);
			}

			//Then, we will remove the clause from the occurrence lists.
			for (j = 0; j < c->size; ++j){
				lit = c->lits[j];
				pos = c->occPos[j];
				REM_CLS_FROM_LIT_OCCS(lit, pos);
			}
		}
		//The clause is now removed from all lists.
		//We will free all the memory of the arrays in the clause, and finally, the clause itself.
		free(c->lits);
		free(c->msgs);
		free(c->occPos);
		free(c);
	}

	//After having removed all last delCount occurrences, we update the counter.
	*useCount = *useCount - delCount;
}

void clauses_deleteLit(clause* c, int32_t lit, int32_t litPos){
	int32_t temp = c->occPos[litPos], remLit = lit;
	//This method will remove the literal lit at position litPos from clause c. The clause must NOT be binary.

	//We remove the clause from the literals occurrence list.
	REM_CLS_FROM_LIT_OCCS(lit, temp);

	//If the literal we remove is not at the last position in the clause, we must override this specific position with
	//the last literal in the clause. For this literal, we must copy the lit, message, occPos. We also update the
	//information where this literal is found in the clause for its occurrence list.
	if (litPos < --c->size){
		//We must override position litPos in c.
		c->lits[litPos] = c->lits[c->size];
		c->msgs[litPos] = c->msgs[c->size];
		c->occPos[litPos] = c->occPos[c->size];
		//We must set the litOccPos in the occurrence list anew for the literal we just copied. We now override lit.
		temp = c->lits[litPos];
		main_litData[temp].occLitPos[ c->occPos[litPos] ] = litPos;
	}

	//We now take care of the clauses watched literals. If the clause is still large enough to need them and our literal is
	//one, we remove the watcher. If a replacement can be found, we use it. Otherwise, we just leave the zero in place to
	//signal that the watcher is not up to date.
	if (c->size > 2){
		if (lit == c->w1){
			c->w1 = 0;
			temp = c->whereW1;
			REM_CLS_FROM_LIT_W1LIST(lit, temp);
			for (temp = 0; temp < c->size; ++temp){
				lit = c->lits[temp];
				if (lit == c->w2) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w1 = lit;
					ADD_CLS_TO_LIT_W1LIST(c, lit);
					break;
				}
			}
		} else if (lit == c->w2){
			c->w2 = 0;
			temp = c->whereW2;
			REM_CLS_FROM_LIT_W2LIST(lit, temp);
			for (temp = 0; temp < c->size; ++temp){
				lit = c->lits[temp];
				if (lit == c->w1) continue;
				if (IS_VAR_UNASSIGNED((main_varData + abs(lit)))){
					//We have found an unassigned literal that can serve as watcher.
					c->w2 = lit;
					ADD_CLS_TO_LIT_W2LIST(c, lit);
					break;
				}
			}
		}
	} else {
		//The clause just became binary. We must remove it from all the watcher lists.
		lit = c->w1;
		litPos = c->whereW1;
		REM_CLS_FROM_LIT_W1LIST(lit, litPos);
		lit = c->w2;
		litPos = c->whereW2;
		REM_CLS_FROM_LIT_W2LIST(lit, litPos);

		ADD_LIT_IMPLICATIONS(c, lit, temp);
	}
	//From this point onwards, we have overridden lit, and lost all information on it.

	//After overriding the literal position, we must update the clauses signature, and representative literal.
	c->repLit = c->lits[0];
	//In order to update the signature, we count the number of set bits. If this count equals the size of the clause,
	//we know that every literal in the clause has its own bit. Updating then works by killing this bit. Otherwise,
	//we must re-create the signature from scratch.
	if (POPCOUNT_UINT64(c->sig) == c->size+1){
		//The signature update works by just removing the bit for the specific literal.
		c->sig &= ~GET_LIT_SIG(remLit);
	} else {
		//From scratch.
		c->sig = 0;
		for (temp = 0; temp < c->size; ++temp){
			lit = c->lits[temp];
			c->sig |= GET_LIT_SIG(lit);
		}
	}
}

void clauses_disable(clause* c){
	int i, lit, pos;
	//For disabling an enabled clause. This will remove the clause from all the watcher and occurrence lists, but leaves it
	//allocated. It transfers the clause address from the learned/original array to the disabled version of the array.
	//The clause MUST be enabled for this to work.

	//We will remove the clause from all the watcher lists, if the clause is indeed not binary.
	if (c->size > 2){
		lit = c->w1;
		pos = c->whereW1;
		REM_CLS_FROM_LIT_W1LIST(lit, pos);
		lit = c->w2;
		pos = c->whereW2;
		REM_CLS_FROM_LIT_W2LIST(lit, pos);
	}  else {
		//It is a binary clause and we must remove the implications it imposes.
		REM_LIT_IMPLICATIONS(c, lit, pos);
	}

	//Then, we will remove the clause from the occurrence lists.
	for (i = 0; i < c->size; ++i){
		lit = c->lits[i];
		pos = c->occPos[i];
		REM_CLS_FROM_LIT_OCCS(lit, pos);
	}

	//We now remove the clause from the address list it is in, and append it to the respective disabled array.
	pos = GET_CLS_OFFSET(c);
	if (IS_CLS_LEARNED(c)){
		//It is a learned clause, so it must be found in the f.clauses_el array.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_el , pos , f.m_el_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_dl, CLS_ID_DL, c, f.m_dl_used, f.m_dl_avail);
	} else {
		//It is an original clause, so it must be found in the f.clauses_eo array.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_eo , pos , f.m_eo_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_do, CLS_ID_DO, c, f.m_do_used, f.m_do_avail);
	}
}

void clauses_enable(clause* c){
	uint32_t i, pos;
	int32_t lit, lit2;
	//For re-enabling one of the disabled clauses (adding it back to the lists).
	//The clause MUST be disabled for this to work.

	//We re-add the clause to the occurrences of the contained variables.
	for (i = 0; i < c->size; ++i){
		lit = c->lits[i];
		ADD_CLS_TO_LIT_OCCS(c, lit, i);
	}

	//We update the watchers of a clause if and only if it is not binary. If it is, we just add the binary
	//partners to the binary array of each literal, such that we know what is implied.
	if (c->size > 2){
		//Update the watchers of the clause. By default, the first two literals are the watchers.
		c->repLit = c->lits[0];
		c->w1 = c->lits[0];
		c->w2 = c->lits[1];
		ADD_CLS_TO_LIT_W1LIST(c, c->w1);
		ADD_CLS_TO_LIT_W2LIST(c, c->w2);
	} else {
		ADD_LIT_IMPLICATIONS(c, lit, lit2);
	}

	//We now remove the clauses address from the respective disabled array and add it back to the enabled array.
	pos = GET_CLS_OFFSET(c);
	if (IS_CLS_LEARNED(c)){
		//It is a learned clause, so it must be found in the f.clauses_dl array.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_dl , pos , f.m_dl_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_el, CLS_ID_EL, c, f.m_el_used, f.m_el_avail);
	} else {
		//It is an original clause, so it must be found in the f.clauses_do array.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_do , pos , f.m_do_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_eo, CLS_ID_EO, c, f.m_eo_used, f.m_eo_avail);
	}
}

void clauses_makeLearned(clause* c){
	uint32_t pos;
	//This method will transfer a clause, that was formerly an original clause, over to the respective (enabled or disabled,
	//depending on what the clause currently is) learned array. This will make the clause look like a learned clause.
	//The clause MUST be an original clause for this to work.

	//We determine if the clause is enabled or disabled.
	pos = GET_CLS_OFFSET(c);
	if (IS_CLS_DISABLED(c)){
		//The clause is currently disabled. We must move it from f.clauses_do to f.clauses_dl.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_do , pos , f.m_do_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_dl, CLS_ID_DL, c, f.m_dl_used, f.m_dl_avail);
	} else {
		//The clause is currently enabled. We must move it from f.clauses_eo to f.clauses_el.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_eo , pos , f.m_eo_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_el, CLS_ID_EL, c, f.m_el_used, f.m_el_avail);
	}
}

void clauses_makeOriginal(clause* c){
	uint32_t pos;
	//This method will transfer a clause, that was formerly a learned clause, over to the respective (enabled or disabled,
	//depending on what the clause currently is) original array. This will make the clause look like an orginal clause.
	//The clause MUST be a learned clause for this to work.

	//We determine if the clause is enabled or disabled.
	pos = GET_CLS_OFFSET(c);
	if (IS_CLS_DISABLED(c)){
		//The clause is currently disabled. We must move it from f.clauses_dl to f.clauses_do.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_dl , pos , f.m_dl_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_do, CLS_ID_DO, c, f.m_do_used, f.m_do_avail);
	} else {
		//The clause is currently enabled. We must move it from f.clauses_el to f.clauses_eo.
		REM_CLS_FROM_CLAUSES_LIST(f.clauses_el , pos , f.m_el_used);
		ADD_CLS_TO_CLAUSES_LIST(f.clauses_eo, CLS_ID_EO, c, f.m_eo_used, f.m_eo_avail);
	}
}

void clauses_remove(clause* c, int32_t litOfInterest){
	//This method will make the given clause a removed one. Removed clauses are like disabled clauses, except that they cannot
	//be deleted, and they host different information. The litOfInterest is either a blocking literal, or a literal that was
	//resolved on during variable elimination by resolution.
	int32_t i, lit, pos;

	//If the clause is not yet disabled, we will remove it from the respective lists now.
	if (!IS_CLS_DISABLED(c)){
		//We will remove the clause from all the watcher lists, if the clause is indeed not binary.
		if (c->size > 2){
			lit = c->w1;
			pos = c->whereW1;
			REM_CLS_FROM_LIT_W1LIST(lit, pos);
			lit = c->w2;
			pos = c->whereW2;
			REM_CLS_FROM_LIT_W2LIST(lit, pos);
		} else {
			//It is a binary clause and we must remove the implications it imposes.
			REM_LIT_IMPLICATIONS(c, lit, pos);
		}

		//Then, we will remove the clause from the occurrence lists. We do not remove implications we once found.
		for (i = 0; i < c->size; ++i){
			lit = c->lits[i];
			pos = c->occPos[i];
			REM_CLS_FROM_LIT_OCCS(lit, pos);
		}

		//We now remove the clause from the address list it is in, and append it to the respective disabled array.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//It is a learned clause, so it must be found in the f.clauses_el array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_el , pos , f.m_el_used);
		} else {
			//It is an original clause, so it must be found in the f.clauses_eo array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_eo , pos , f.m_eo_used);
		}
	} else {
		//We do not have to remove the clause from any lists, as it is already disabled.
		//We now remove the clause from the address list it is in, and append it to the respective disabled array.
		pos = GET_CLS_OFFSET(c);
		if (IS_CLS_LEARNED(c)){
			//It is a learned clause, so it must be found in the f.clauses_dl array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_dl , pos , f.m_dl_used);
		} else {
			//It is an original clause, so it must be found in the f.clauses_do array.
			REM_CLS_FROM_CLAUSES_LIST(f.clauses_do , pos , f.m_do_used);
		}
	}
	//We must now create a new removed clause and transfer all the necessary information of the old clause to the new clause.
	//Afterwards, we delete the clause completely to free the memory.
	remClause* rC = malloc(sizeof(remClause));
	rC->size = c->size;
	rC->lits = c->lits;
	rC->litOfInterest = litOfInterest;
	rC->numCovered = 0;
	rC->coveredLits = NULL;
	ADD_REMCLS_TO_REMCLAUSES_LIST(rC);

	//After the transfer of the clause into the removed clauses set, we can destroy the rest of c. The c->lits must, however,
	//survive, because we address them from the removed clause as well.
	free(c->msgs);
	free(c->occPos);
	free(c);
}

#ifdef VERBOSE
void clauses_printClauseLiterals_inline(clause *c){
	uint32_t i;
	for (i = 0; i < c->size; ++i){
		printf("%d ", c->lits[i]);
	}
}

void clauses_printRemClauseLiterals_inline(remClause *rc){
	uint32_t i;
	for (i = 0; i < rc->size; ++i){
		printf("%d ", rc->lits[i]);
	}
}

void clauses_printClauseMessages_inline(clause *c){
	uint32_t i;
	for (i = 0; i < c->size; ++i){
		printf("%1.8f ", c->msgs[i]);
	}
}

void clauses_printClauseOccPos_inline(clause *c){
	uint32_t i;
	for (i = 0; i < c->size; ++i){
		printf("%d ", c->occPos[ i ]);
	}
}

void clauses_printClause(clause* c){
	printf("c CLS %p", (void*)c);
	if (c == NULL) {
		printf("\n");
		return;
	}
	printf(" BASE:");
	PRINT_BITS_UINT32(c->baseData);
	printf(" QPOS:%5d\tQSCORE:%5d\t",c->cQPos, c->cQScore);
	printf(" SPOS:%5d\t",c->cSPos);
	printf(" SS0POS:%5d\t",c->cSS0Pos);
	printf(" SS1POS:%5d\t",c->cSS1Pos);
	printf(" SIG:");
	PRINT_BITS_UINT64(c->sig);
	printf("\tREPLIT:%7d\tW1:%7d(%d)\tW2:%7d(%d)\tSIZE:%7d\tLITS:",
			c->repLit,c->w1,c->whereW1,c->w2,c->whereW2,c->size);
	if (c->lits == NULL){
		printf("NULL ");
	} else {
		clauses_printClauseLiterals_inline(c);
	}
	printf("\tMSGS:");
	if (c->msgs == NULL){
		printf("NULL ");
	} else {
		clauses_printClauseMessages_inline(c);
	}
	printf("\tOCCPOS:");
	if (c->occPos == NULL){
		printf("NULL ");
	} else {
		clauses_printClauseOccPos_inline(c);
	}
	printf("\n");
}

void clauses_printClauseInline(clause* c){
	printf("CLS ");
	if (c->lits == NULL){
		printf("NULL ");
	} else {
		clauses_printClauseLiterals_inline(c);
	}
}

void clauses_printRemClause(remClause* rc){
	int32_t i;
	printf("c RCLS %p BYACTION:%d", (void*) rc, rc->byAction);
	printf(" LITOFINTEREST:%6d\tNUMCOVERED:%5d\tLITS: ", rc->litOfInterest, rc->numCovered);
	if (rc->lits == NULL){
		printf("NULL ");
	} else {
		clauses_printRemClauseLiterals_inline(rc);
	}
	printf("\t COVEREDLITS: ");
	for (i = 0; i < rc->numCovered; ++i){
		printf("%d(%d) ", rc->coveredLits[i].lit, rc->coveredLits[i].coveredBy);
	}
	printf("\n");
}

void clauses_printAll(){
	int32_t i;
	clause *c;
	remClause *rc;
	printf("c Original clauses:\n");
	for (i = 0; i < f.m_eo_used; ++i){
		c = f.clauses_eo[i];
		clauses_printClause(c);
	}

	printf("c Learned clauses:\n");
	for (i = 0; i < f.m_el_used; ++i){
		c = f.clauses_el[i];
		clauses_printClause(c);
	}

	printf("c Disabled Original clauses:\n");
	for (i = 0; i < f.m_do_used; ++i){
		c = f.clauses_do[i];
		clauses_printClause(c);
	}

	printf("c Disabled Learned clauses:\n");
	for (i = 0; i < f.m_dl_used; ++i){
		c = f.clauses_dl[i];
		clauses_printClause(c);
	}

	printf("c Removed clauses:\n");
	for (i = 0; i < f.m_rem_used; ++i){
		rc = f.clauses_rem[i];
		clauses_printRemClause(rc);
	}
}

uint32_t clauses_verifyClause(clause* c){
	literal *l, *litA, *litB;
	variable *v;
	uint64_t aSig = 0ULL;
	int32_t litNum, lit, occPos, repLit, w1Lit, w2Lit, repLitFound, w1LitFound, w2LitFound, whereW1, whereW2, numTrue, numFalse;

	//First, we verify that the representative literal and the watchers are part of this clause, and that the watchers are
	//two different literals.
	repLit = GET_CLS_REPLIT(c);
	w1Lit = GET_CLS_W1(c);
	whereW1 = GET_CLS_WHEREW1(c);
	w2Lit = GET_CLS_W2(c);
	whereW2 = GET_CLS_WHEREW2(c);
	repLitFound = 0;
	w1LitFound = 0;
	w2LitFound = 0;
	numFalse = 0;
	numTrue = 0;

	for (litNum = 0; litNum < GET_CLS_SIZE(c); ++litNum){
		lit = GET_CLS_LITNUM(c, litNum);
		l = main_litData + lit;
		v = main_varData + abs(lit);

		if (GET_VAR_ISEQUIVTO(v) != 0){
			printf("\nThe variable has an equivalence representative. This should not be.\n");
			variables_printVariable(v);
			return 0;
		}

		aSig |= GET_LIT_SIG(lit);
		occPos = GET_CLS_OCCPOSNUM(c, litNum);

		if (GET_VAR_ISEQUIVTO(v) != 0){
			printf("\nThe clause contains a variable that is equivalent to some other. This cannot be.");
			variables_printVariable(v);
			clauses_printClause(c);
			return 0;
		}

		if (GET_LIT_NUMOCCS_USED(l) <= occPos){
			printf("\nThe occurrence position indicated in the clause for the literal is too large.\n");
			literals_printLiteral(l);
			clauses_printClause(c);
			return 0;
		}

		if (GET_LIT_OCCNUM(l, occPos) != c){
			printf("\nThe occurrence information of the literal in the clause is wrong.");
			literals_printLiteral(l);
			clauses_printClause(c);
			return 0;
		}
		if (lit == repLit) {repLitFound = 1;}
		if (lit == w1Lit) {w1LitFound = 1;}
		if (lit == w2Lit) {w2LitFound = 1;}
		if (IS_LIT_UNSAT(lit)) ++numFalse;
		if (IS_LIT_SAT(lit)) ++numTrue;
	}

	if (numFalse == GET_CLS_SIZE(c)){
		printf("\nThe clause is false.\n");
		clauses_printClause(c);
		return 0;
	}

	if (numFalse == GET_CLS_SIZE(c)-1 && numTrue == 0){
		printf("\nThe clause is unit.\n");
		clauses_printClause(c);
		return 0;
	}

	if (aSig != GET_CLS_SIG(c)){
		printf("\nThe signature is wrong.\n");
		clauses_printClause(c);
		return 0;
	}

	if (GET_CLS_SIZE(c) == 2){
		if (!repLitFound){
			printf("\nReplit not correct.\n");
			clauses_printClause(c);
			return 0;
		}
	} else {
		if (!repLitFound || !w1LitFound || !w2LitFound){
			printf("\nReplit or Watcher not correct.\n");
			clauses_printClause(c);
			return 0;
		}
		if (w1Lit == w2Lit){
			printf("\nThe clause has two equivalent watchers.\n");
			clauses_printClause(c);
			return 0;
		}
		litA = main_litData + w1Lit;
		litB = main_litData + w2Lit;

		if (GET_LIT_NUMW1_USED(litA) <= whereW1){
			printf("\nThe whereW1 information of the clause is wrong (too large for list).\n");
			literals_printLiteral(litA);
			clauses_printClause(c);
			return 0;
		}

		if (GET_LIT_NUMW2_USED(litB) <= whereW2){
			printf("\nThe whereW2 information of the clause is wrong (too large for list).\n");
			literals_printLiteral(litB);
			clauses_printClause(c);
			return 0;
		}

		if (GET_LIT_W1_WATCHED_CLS(litA, whereW1) != c){
			printf("\nThe occurrence information for watcher 1 is wrong.\n");
			literals_printLiteral(litA);
			clauses_printClause(c);
			return 0;
		}
		if (GET_LIT_W2_WATCHED_CLS(litB, whereW2) != c){
			printf("\nThe occurrence information for watcher 2 is wrong.\n");
			literals_printLiteral(litB);
			clauses_printClause(c);
			return 0;
		}
	}

	return 1;
}

uint32_t clauses_verifyAll(){
	//This method performs a costly consistency check on all the clauses of the CDCL formula.
	clause *c;
	uint32_t cNum;

	for (cNum = 0; cNum < f.m_eo_used; ++cNum){
		c = f.clauses_eo[cNum];
		if (!clauses_verifyClause(c)) {
			return 0;
		}
	}

	for (cNum = 0; cNum < f.m_el_used; ++cNum){
		c = f.clauses_el[cNum];
		if (!clauses_verifyClause(c)) {
			return 0;
		}
	}

	return 1;
}
#endif
