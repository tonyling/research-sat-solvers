/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../cdcl.h"

void cdcl_clauses_Q_moveUp(cdclClause* c){
	//To move an element up in the CDCL clause priority queue. A larger score value means better score.
	cdclClause* parent;
	float_ty cScore, parentScore;
	int32_t cPos, parentPos;

	//We first get the current position of the clause and its score.
	cPos = GET_CDCL_CLS_QPOS(c);
	cScore = GET_CDCL_CLS_QSCORE(c);

	while(1){
		//We can stop right away if the clause is at the topmost position.
		if (cPos == 1) break;

		//We move the clause upwards by exchanging it with its parent as long as the parent has less score.
		//Division by two is shift right by one.
		parentPos = cPos >> 1;
		parent = cdcl_clauses_Q[parentPos];
		parentScore = GET_CDCL_CLS_QSCORE(parent);

		//If the parent has more score than the clause, we can return.
		if (parentScore > cScore) break;

		//Otherwise, we must exchange the clause and its parent. Put the parent at the current clause position.
		cdcl_clauses_Q[cPos] = parent;
		SET_CDCL_CLS_QPOS(parent, cPos);

		//Then we put the clause in the position of the parent. We do not yet update the position of the clause. See the end.
		cdcl_clauses_Q[parentPos] = c;
		cPos = parentPos;
	}

	//Finally we update the clauses position in the priority queue.
	SET_CDCL_CLS_QPOS(c, cPos);
}

void cdcl_clauses_Q_moveDown(cdclClause* c){
	//To move an element down in the CDCL clause priority queue. A larger score value means better score.
	cdclClause *leftChild, *rightChild;
	float_ty cScore, leftScore, rightScore;
	uint32_t cPos, leftChildPos, rightChildPos;
	cPos = GET_CDCL_CLS_QPOS(c);
	cScore = GET_CDCL_CLS_QSCORE(c);

	while(1){
		leftChildPos = cPos << 1;
		rightChildPos = (cPos << 1)+1;

		//No left child means no right child either. Clause already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= cdcl_clauses_Q_leaf)	break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= cdcl_clauses_Q_leaf) {
			leftChild = cdcl_clauses_Q[leftChildPos];
			leftScore = GET_CDCL_CLS_QSCORE(leftChild);

			//If the left child has more score, we must move down the clause to this position. Otherwise we are done.
			if (leftScore <= cScore) break;

			//We are not done yet. We first lift the child and then push down the clause we want to push down.
			cdcl_clauses_Q[cPos] = leftChild;
			SET_CDCL_CLS_QPOS(leftChild, cPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			cdcl_clauses_Q[leftChildPos] = c;
			cPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a better score.
			leftChild = cdcl_clauses_Q[leftChildPos];
			leftScore = GET_CDCL_CLS_QSCORE(leftChild);

			rightChild = cdcl_clauses_Q[rightChildPos];
			rightScore = GET_CDCL_CLS_QSCORE(rightChild);

			if (leftScore < rightScore){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftScore = rightScore;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child does not have a better score than the clause itself.
			if (leftScore <= cScore) break;

			//We are not done yet. We first lift the child and then push down the clause.
			cdcl_clauses_Q[cPos] = leftChild;
			SET_CDCL_CLS_QPOS(leftChild, cPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			cdcl_clauses_Q[leftChildPos] = c;
			cPos = leftChildPos;
		}
	}
	//Finally we update the clause position position in the priority queue.
	SET_CDCL_CLS_QPOS(c, cPos);
}

cdclClause* cdcl_clauses_Q_dequeueBest(){
	//To dequeue the best element in the CDCL clause priority queue.
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (cdcl_clauses_Q_leaf == 1) return NULL;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (cdcl_clauses_Q_leaf == 2){
		SET_CDCL_CLS_QPOS(cdcl_clauses_Q[1],0);
		--cdcl_clauses_Q_leaf;
		return cdcl_clauses_Q[1];
	}

	//If there is more than one element left we pick the topmost element and signal that it is not in the queue anymore.
	cdclClause* best = cdcl_clauses_Q[1];
	SET_CDCL_CLS_QPOS(best,0);

	//We override the first position with the last element in the queue and push it down afterwards.
	cdcl_clauses_Q[1] = cdcl_clauses_Q[--cdcl_clauses_Q_leaf];
	SET_CDCL_CLS_QPOS(cdcl_clauses_Q[1],1);

	cdcl_clauses_Q_moveDown(cdcl_clauses_Q[1]);
	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void cdcl_clauses_Q_delete(cdclClause* c){
	//To delete an element from the CDCL clause priority queue.
	cdclClause *replacementC, *parent;
	float_ty replacementScore, parentScore;
	uint32_t cPos, parentPos;

	//We first pick the current position of this clause in the queue.
	cPos = GET_CDCL_CLS_QPOS(c);

	//If the given clause is not in the queue we are already done.
	if (cPos == 0) return;

	//We set the element to be not in the queue anymore.
	SET_CDCL_CLS_QPOS(c,0);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (cPos == cdcl_clauses_Q_leaf-1){
		--cdcl_clauses_Q_leaf;
		return;
	}

	//We pick the replacement.
	replacementC = cdcl_clauses_Q[--cdcl_clauses_Q_leaf];

	//We override the position of the clause to be deleted by the one we picked as replacement.
	cdcl_clauses_Q[cPos] = replacementC;

	//We update the position of the replacement clause in the queue.
	SET_CDCL_CLS_QPOS(replacementC,cPos);
	replacementScore = GET_CDCL_CLS_QSCORE(replacementC);

	//We then check if we have to move the element upwards.
	parentPos = cPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = cdcl_clauses_Q[parentPos];
		parentScore = GET_CDCL_CLS_QSCORE(parent);

		//Check if the replacement must be moved up or down in the queue.
		if (parentScore >  replacementScore){
			//We must make the upwards move.
			cdcl_clauses_Q_moveUp(replacementC);
		} else {
			//We must make the downwards move.
			cdcl_clauses_Q_moveDown(replacementC);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		cdcl_clauses_Q_moveDown(replacementC);
	}
}

void cdcl_clauses_Q_enqueue(cdclClause* c){
	//First we put the clause on the next priority queue leaf.
	cdcl_clauses_Q[cdcl_clauses_Q_leaf] = c;
	//Then we update its position for being in the queue.
	SET_CDCL_CLS_QPOS(c,cdcl_clauses_Q_leaf++);
	//Increase the size of the queue if necessary.
	if (cdcl_clauses_Q_leaf >= cdcl_clauses_Q_avail-2){
		cdcl_clauses_Q_avail += 8*BLOCKSIZE;
		cdcl_clauses_Q = realloc(cdcl_clauses_Q, sizeof(clause*)*(cdcl_clauses_Q_avail+2));
	}
	//Then we move the clause up in the queue until it is at the right position.
	cdcl_clauses_Q_moveUp(c);

}

void cdcl_clauses_Q_touchClause_maintenance_inactivity(cdclClause* c){
	//To touch a clause when doing clause database maintenance.
	float_ty oldScore = GET_CDCL_CLS_QSCORE(c), newScore = ZERO;

	//The new score for the clause is its current activity. The larger the activity, the farther at the end it will be.
	newScore = CLS_MAX_ACTIVITY_VALUE - GET_CDCL_CLS_ACTIVITY(c);
	SET_CDCL_CLS_QSCORE(c,newScore);

	if (GET_CDCL_CLS_QPOS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore > oldScore){
			//Yes, something changed and the clause now has a better score.
			cdcl_clauses_Q_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cdcl_clauses_Q_moveDown(c);
		}
	} else {
		cdcl_clauses_Q_enqueue(c);
	}
}

void cdcl_clauses_Q_touchClause_maintenance_lbd(cdclClause* c){
	//To touch a clause when doing clause database maintenance.
	float_ty oldScore = GET_CDCL_CLS_QSCORE(c), newScore = UINT32_MAX;

	//The new score for the clause is its LBD value. The smaller the LBD, the farther at the end the clause will be.
	newScore = GET_CDCL_CLS_LBD(c);
	SET_CDCL_CLS_QSCORE(c,newScore);

	if (GET_CDCL_CLS_QPOS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore > oldScore){
			//Yes, something changed and the clause now has a better score.
			cdcl_clauses_Q_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cdcl_clauses_Q_moveDown(c);
		}
	} else {
		cdcl_clauses_Q_enqueue(c);
	}
}

void cdcl_clauses_Q_touchClause_inprocessing_se(cdclClause* c){
	//To touch a clause when doing subsumption elimination -- shortest clauses first.
	float_ty oldScore = GET_CDCL_CLS_QSCORE(c), newScore = UINT32_MAX;

	//The new score for the clause is its current size. The shorter the clause, the better it is for subsuming others with it.
	newScore = UINT32_MAX - GET_CDCL_CLS_SIZE(c);
	SET_CDCL_CLS_QSCORE(c,newScore);

	if (GET_CDCL_CLS_QPOS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore > oldScore){
			//Yes, something changed and the clause now has a better score.
			cdcl_clauses_Q_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cdcl_clauses_Q_moveDown(c);
		}
	} else {
		cdcl_clauses_Q_enqueue(c);
	}
}

void cdcl_clauses_Q_touchClause_inprocessing_str(cdclClause* c){
	//To touch a clause when doing strengthening - shortest clauses first.
	float_ty oldScore = GET_CDCL_CLS_QSCORE(c), newScore = UINT32_MAX;

	if (GET_CDCL_CLS_SIZE(c) == 2 || GET_CDCL_CLS_SIZE(c) > param_cdclIRStrMaxSize) return;

	//The new score for the clause is its current size. The shorter the clause, the better it is for strengthening others.
	newScore = UINT32_MAX - GET_CDCL_CLS_SIZE(c);
	SET_CDCL_CLS_QSCORE(c,newScore);

	if (GET_CDCL_CLS_QPOS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore > oldScore){
			//Yes, something changed and the clause now has a better score.
			cdcl_clauses_Q_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cdcl_clauses_Q_moveDown(c);
		}
	} else {
		cdcl_clauses_Q_enqueue(c);
	}
}

void cdcl_clauses_Q_touchClause_inprocessing_treelook(cdclClause* c){
	//To do touch a clause when doing in-processing TREELOOK - shortest clauses first.
	float_ty oldScore = GET_CDCL_CLS_QSCORE(c), newScore = UINT32_MAX;

	//The new score for the clause is its current size. The shorter the clause, the better it is for becoming false.
	newScore = UINT32_MAX - GET_CDCL_CLS_SIZE(c);
	SET_CDCL_CLS_QSCORE(c,newScore);

	if (GET_CDCL_CLS_QPOS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore > oldScore){
			//Yes, something changed and the clause now has a better score.
			cdcl_clauses_Q_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cdcl_clauses_Q_moveDown(c);
		}
	} else {
		cdcl_clauses_Q_enqueue(c);
	}
}


void cdcl_clauses_Q_touchClause_null(cdclClause* c){
	//To do nothing when a clause is touched.
}

void cdcl_clauses_Q_flush(){
	//To remove all elements in the CDCL clause priority queue.
	uint32_t i;
	for (i = 1; i < cdcl_clauses_Q_leaf; ++i){
		cdcl_clauses_Q[i]->cQPos = 0;
	}
	cdcl_clauses_Q_leaf = 1;
}

void cdcl_clauses_Q_ensureSize(uint32_t size){
	//To make sure that the queue can hold a specific amount of clauses.
	if (cdcl_clauses_Q_avail < size+3){
		cdcl_clauses_Q_avail = size + 3 + BLOCKSIZE;
		cdcl_clauses_Q = realloc(cdcl_clauses_Q, sizeof(cdclClause*)*cdcl_clauses_Q_avail);
	}
}

void cdcl_clauses_S_delete(cdclClause* c){
	//To delete a literal from the clause stack.
	int32_t pos = GET_CDCL_CLS_SPOS(c);
	if (pos == 0) return;

	SET_CDCL_CLS_SPOS(c, 0);

	if (pos == --cdcl_clauses_S_leaf) {
		//Since the removed literal was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last variable that is now at vSLeaf position.
	cdcl_clauses_S[pos] = cdcl_clauses_S[cdcl_clauses_S_leaf];
	SET_CDCL_CLS_SPOS(cdcl_clauses_S[pos], pos);
}

void cdcl_clauses_S_push(cdclClause* c){
	//To push something on the clause stack.
	if (GET_CDCL_CLS_SPOS(c) > 0) return;

	cdcl_clauses_S[cdcl_clauses_S_leaf] = c;
	SET_CDCL_CLS_SPOS(c, cdcl_clauses_S_leaf);
	if (++cdcl_clauses_S_leaf >= cdcl_clauses_S_avail-2){
		cdcl_clauses_S_avail += 8*BLOCKSIZE;
		cdcl_clauses_S = realloc(cdcl_clauses_S, sizeof(cdclClause*)*(cdcl_clauses_S_avail+2));
	}
}

void cdcl_clauses_S_touchClause_null(cdclClause* c){
	//To do nothing when a clause is touched.
}

cdclClause* cdcl_clauses_S_pop(){
	//To pop the topmost element from the stack.
	if (cdcl_clauses_S_leaf == 1) return NULL;

	--cdcl_clauses_S_leaf;
	SET_CDCL_CLS_SPOS(cdcl_clauses_S[cdcl_clauses_S_leaf],0);
	return cdcl_clauses_S[cdcl_clauses_S_leaf];
}

void cdcl_clauses_S_flush(){
	//To remove all elements from the literal stack.
	uint32_t i;
	for (i = 1; i < cdcl_clauses_S_leaf; ++i){
		SET_CDCL_CLS_SPOS(cdcl_clauses_S[i], 0);
	}
	cdcl_clauses_S_leaf = 1;
}

void cdcl_clauses_S_ensureSize(uint32_t size){
	if (cdcl_clauses_S_avail < size+3){
		cdcl_clauses_S_avail = size + 3 + BLOCKSIZE;
		cdcl_clauses_S = realloc(cdcl_clauses_S, sizeof(cdclClause*)*cdcl_clauses_S_avail);
	}
}

void cdcl_clauses_prepareCall(){
	int32_t *litC,lit1, lit2;
	clause *origC;
	cdclClause *cdclC;
	uint32_t i,numLits;
	//Creates a restriction of the main formula into the clauses of the CDCL.
	//Initialize clauses for the call here. All clauses (original and learned in the formula f) are going to be
	//original clauses in the CDCL. Its learned clauses array is empty after preparing the call.
	//First we make sure, that all the clauses have been allocated in a minimal form.
	if (cdcl_cls_o_avail < (f.m_eo_used + f.m_el_used)){
		cdcl_cls_o = realloc(cdcl_cls_o, sizeof(cdclClause*) * (f.m_eo_used + f.m_el_used));
		for (i = cdcl_cls_o_avail; i < (f.m_eo_used + f.m_el_used); ++i){
			ALLOCATE_ORIGINAL_CDCL_CLS_MINIMAL(cdclC,i);
		}
		cdcl_cls_o_avail = f.m_eo_used + f.m_el_used;
	}

	//We now completely load the formula given in f into the original array. If the clause in the original formula is
	//satisfied, we ignore it.
	cdcl_cls_o_used = 0;
	cdcl_cls_l_used = 0;
	cdcl_cls_r_used = 0;
	//First the enabled original clauses.
	for (i = 0; i < f.m_eo_used; ++i){
		origC = f.clauses_eo[i];

		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(origC->repLit)) continue;
		litC = origC->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(origC, litC)){
			if (IS_LIT_SAT(*litC)){
				origC->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				++numLits;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(origC, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		cdclC = cdcl_cls_o[cdcl_cls_o_used];
		cdclC->lits = realloc(cdclC->lits, sizeof(int32_t)*(numLits+1U));
		cdclC->occPos = realloc(cdclC->occPos, sizeof(int32_t)*(numLits));
		litC = origC->lits;
		cdclC->sig = 0ULL;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(origC, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We use it in the CDCL clause.
				ADD_CDCL_CLS_TO_LIT_OCCS(cdclC, *litC, numLits);
				cdclC->lits[numLits++] = *litC;
				cdclC->sig |= GET_CDCL_LIT_SIG(*litC);
			}
			++litC;
		}
		//Terminate the clause and update repLit, size and watcher/implications.
		cdclC->lits[numLits] = 0;
		cdclC->size = numLits;
		cdclC->activity = ONE;
		cdclC->lbd = numLits;
		cdclC->repLit = cdclC->lits[0];
		cdclC->baseData = cdcl_cls_o_used;

		//We update the watchers of a clause if and only if it is not binary. If it is, we just add the binary
		//partners to the binary array of each literal, such that we know what is implied.
		if (cdclC->size > 2){
			//Update the watchers of the clause. By default, the first two literals are the watchers.
			cdclC->w1 = cdclC->lits[0];
			cdclC->w2 = cdclC->lits[1];
			ADD_CDCL_CLS_TO_LIT_W1LIST(cdclC, cdclC->lits[0]);
			ADD_CDCL_CLS_TO_LIT_W2LIST(cdclC, cdclC->lits[1]);
		} else {
			//Update the implications for the literals. For each of the two literal, put the other literal in the
			//implies array of the literal with opposite sign. In such a way, whenever we unit propagate, we must
			//only copy all the literals found in the array.
			cdclC->w1 = 0;
			cdclC->whereW1 = 0;
			cdclC->w2 = 0;
			cdclC->whereW2 = 0;
			ADD_CDCL_LIT_IMPLICATIONS(cdclC, lit1, lit2);
		}

		if (cdclC->size == 1){
			printf("UNIT");
			exit(0);
		}

		//We stored one more clause.
		++cdcl_cls_o_used;
	}

	//Second the enabled learned clauses.
	for (i = 0; i < f.m_el_used; ++i){
		origC = f.clauses_el[i];
		//Check if the clause is satisfied in the original formula. Count its number of unassigned literals.
		if (IS_LIT_SAT(origC->repLit)) continue;
		litC = origC->lits;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(origC, litC)){
			if (IS_LIT_SAT(*litC)){
				origC->repLit = *litC;
				break;
			}
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We could use it in the SLS clause.
				++numLits;
			}
			++litC;
		}
		//If we did not look at all the literals, we can continue.
		if (HAS_CLS_MORE_LITS(origC, litC)) continue;
		//If we get this far, we checked all the literals in the clause, and found that it is not satisfied. We will allocate
		//memory for it and copy all the literals unassigned in the clause.
		cdclC = cdcl_cls_o[cdcl_cls_o_used];
		cdclC->lits = realloc(cdclC->lits, sizeof(int32_t)*(numLits+1U));
		cdclC->occPos = realloc(cdclC->occPos, sizeof(int32_t)*(numLits));
		litC = origC->lits;
		cdclC->sig = 0ULL;
		numLits = 0;
		while (HAS_CLS_MORE_LITS(origC, litC)){
			if (IS_VAR_UNASSIGNED((main_varData + abs(*litC)))){
				//The literal in the clause is not assigned. We use it in the CDCL clause.
				ADD_CDCL_CLS_TO_LIT_OCCS(cdclC, *litC, numLits);
				cdclC->lits[numLits++] = *litC;
				cdclC->sig |= GET_CDCL_LIT_SIG(*litC);
			}
			++litC;
		}
		//Terminate the clause and update repLit, size and watcher/implications.
		cdclC->lits[numLits] = 0;
		cdclC->size = numLits;
		cdclC->activity = ONE;
		cdclC->lbd = numLits;
		cdclC->repLit = cdclC->lits[0];
		cdclC->baseData = cdcl_cls_o_used;

		//We update the watchers of a clause if and only if it is not binary. If it is, we just add the binary
		//partners to the binary array of each literal, such that we know what is implied.
		if (cdclC->size > 2){
			//Update the watchers of the clause. By default, the first two literals are the watchers.
			cdclC->w1 = cdclC->lits[0];
			cdclC->w2 = cdclC->lits[1];
			ADD_CDCL_CLS_TO_LIT_W1LIST(cdclC, cdclC->lits[0]);
			ADD_CDCL_CLS_TO_LIT_W2LIST(cdclC, cdclC->lits[1]);
		} else {
			//Update the implications for the literals. For each of the two literal, put the other literal in the
			//implies array of the literal with opposite sign. In such a way, whenever we unit propagate, we must
			//only copy all the literals found in the array.
			cdclC->w1 = 0;
			cdclC->whereW1 = 0;
			cdclC->w2 = 0;
			cdclC->whereW2 = 0;
			ADD_CDCL_LIT_IMPLICATIONS(cdclC, lit1, lit2);
		}

		//We stored one more clause.
		++cdcl_cls_o_used;
	}
	cdcl_clauses_Q_flush();
	cdcl_clauses_Q_touchClause = &cdcl_clauses_Q_touchClause_null;
	cdcl_clauses_S_flush();
	cdcl_clauses_S_touchClause = &cdcl_clauses_S_touchClause_null;
}

void cdcl_clauses_reset(){
	cdcl_cls_o 			 = NULL;
	cdcl_cls_o_used 	 = 0;
	cdcl_cls_o_avail 	 = 0;

	cdcl_cls_l 			 = NULL;
	cdcl_cls_l_used 	 = 0;
	cdcl_cls_l_avail 	 = 0;

	cdcl_clauses_Q       = NULL;
	cdcl_clauses_Q_leaf  = 0;
	cdcl_clauses_Q_avail = 0;

	cdcl_clauses_S       = NULL;
	cdcl_clauses_S_leaf  = 0;
	cdcl_clauses_S_avail = 0;

	cdcl_cls_r 			 = NULL;
	cdcl_cls_r_used 	 = 0;
	cdcl_cls_r_avail 	 = 0;
}

void cdcl_clauses_init(){
	uint32_t i;
	cdclClause *cdclC;
	remCdclClause *remCdclC;

	//Create clause and initialize data-structures here for a small set BLOCKSIZE.

	cdcl_cls_o_avail = BLOCKSIZE;
	cdcl_cls_o = malloc(sizeof(cdclClause*)*cdcl_cls_o_avail);
	if (cdcl_cls_o == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the original clauses. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 0; i < cdcl_cls_o_avail; ++i){
		ALLOCATE_ORIGINAL_CDCL_CLS_MINIMAL(cdclC, i);
		cdcl_cls_o[i] = cdclC;
	}
	cdcl_cls_o_used = 0;

	cdcl_cls_l_avail = BLOCKSIZE;
	cdcl_cls_l = malloc(sizeof(cdclClause*)*cdcl_cls_l_avail);
	if (cdcl_cls_l == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the learned clauses. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 0; i < cdcl_cls_l_avail; ++i){
		ALLOCATE_LEARNED_CDCL_CLS_MINIMAL(cdclC, i);
		cdcl_cls_l[i] = cdclC;
	}
	cdcl_cls_l_used = 0;

	cdcl_clauses_Q_leaf = 1;
	cdcl_clauses_Q_avail = 8*BLOCKSIZE;
	cdcl_clauses_Q = malloc(sizeof(cdclClause*)*cdcl_clauses_Q_avail);
	if (cdcl_clauses_Q == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the clauses priority queue. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_clauses_S_leaf = 1;
	cdcl_clauses_S_avail = 8*BLOCKSIZE;
	cdcl_clauses_S = malloc(sizeof(cdclClause*)*cdcl_clauses_S_avail);
	if (cdcl_clauses_S == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the clauses stack. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	cdcl_cls_r_avail = BLOCKSIZE;
	cdcl_cls_r = malloc(sizeof(remCdclClause*)*cdcl_cls_r_avail);
	if (cdcl_cls_r == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the removed clauses. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 0; i < cdcl_cls_r_avail; ++i){
		ALLOCATE_REMOVED_CDCL_CLS_MINIMAL(remCdclC, i);
		cdcl_cls_r[i] = remCdclC;
	}
	cdcl_cls_r_used = 0;
}

void cdcl_clauses_dispose(){
	uint32_t i;
	cdclClause *cdclC;
	remCdclClause *remCdclC;

	if (cdcl_cls_o != NULL){
		for (i = 0; i < cdcl_cls_o_avail; ++i){
			//Grab next clause.
			cdclC = cdcl_cls_o[i];

			if (cdclC != NULL){
				//Free the memory allocated in c.
				if (GET_CDCL_CLS_LITS(cdclC) != NULL){
					free(cdclC->lits);
					cdclC->lits = NULL;
				}
				if (GET_CDCL_CLS_OCCPOS(cdclC) != NULL){
					free(cdclC->occPos);
					cdclC->occPos = NULL;
				}
				//Free clause number i itself.
				free(cdcl_cls_o[i]);
				cdcl_cls_o[i] = NULL;
			}
		}

		//Free the clauses array.
		free(cdcl_cls_o);
		cdcl_cls_o = NULL;
	}

	if (cdcl_cls_l != NULL){
		for (i = 0; i < cdcl_cls_l_avail; ++i){
			//Grab next clause.
			cdclC = cdcl_cls_l[i];

			if (cdclC != NULL){
				//Free the memory allocated in c.
				if (GET_CDCL_CLS_LITS(cdclC) != NULL){
					free(cdclC->lits);
					cdclC->lits = NULL;
				}
				if (GET_CDCL_CLS_OCCPOS(cdclC) != NULL){
					free(cdclC->occPos);
					cdclC->occPos = NULL;
				}
				//Free clause number i itself.
				free(cdcl_cls_l[i]);
				cdcl_cls_l[i] = NULL;
			}
		}

		//Free the clauses array.
		free(cdcl_cls_l);
		cdcl_cls_l = NULL;
	}

	cdcl_cls_o_used = 0;
	cdcl_cls_o_avail = 0;

	if (cdcl_clauses_Q != NULL){
		free(cdcl_clauses_Q);
		cdcl_clauses_Q = NULL;
	}
	cdcl_clauses_Q_leaf = 0;
	cdcl_clauses_Q_avail = 0;

	if (cdcl_clauses_S != NULL){
		free(cdcl_clauses_S);
		cdcl_clauses_S = NULL;
	}
	cdcl_clauses_S_leaf = 0;
	cdcl_clauses_S_avail = 0;

	if (cdcl_cls_r != NULL){
		for (i = 0; i < cdcl_cls_r_avail; ++i){
			//Grab next clause.
			remCdclC = cdcl_cls_r[i];

			if (remCdclC != NULL){
				//Free the memory allocated in c.
				if (remCdclC->lits != NULL){
					free(remCdclC->lits);
					remCdclC->lits = NULL;
				}
				//Free clause number i itself.
				free(cdcl_cls_r[i]);
				cdcl_cls_r[i] = NULL;
			}
		}

		//Free the clauses array.
		free(cdcl_cls_r);
		cdcl_cls_r = NULL;
	}
}

void cdcl_clauses_deleteClause(cdclClause* c){
	uint32_t pos, i;
	int32_t lit;
	cdclClause *tmp;
	//For deleting a clause from the formula. We first disable the clause by removing it from all the lists.
	//We then shrink the memory of this clause back to a minimally initialized clause. Then, we exchange the pointers
	//for this clause with the last used clause and update the offsets.

	//We will remove the clause from all the watcher lists, if the clause is indeed not binary.
	if (c->size > 2){
		lit = c->w1;
		pos = c->whereW1;
		REM_CDCL_CLS_FROM_LIT_W1LIST(lit, pos);
		lit = c->w2;
		pos = c->whereW2;
		REM_CDCL_CLS_FROM_LIT_W2LIST(lit, pos);
	}  else {
		//It is a binary clause and we must remove the implications it imposes.
		REM_CDCL_LIT_IMPLICATIONS(c, lit, pos);
	}

	//Then, we will remove the clause from the occurrence lists.
	for (i = 0; i < c->size; ++i){
		lit = c->lits[i];
		pos = c->occPos[i];
		REM_CDCL_LIT_OCC(lit, pos);
	}

	pos = GET_CDCL_CLS_OFFSET(c);

	//First, we determine if the clause is learned or original. This tells us in which arrays we perform the operations.
	if (IS_CDCL_CLS_LEARNED(c)){
		//It is a learned clause, we work in the cdcl_cls_l array. We first shrikn the clause. Then we must move the
		//pointer at cdcl_cls_l[used] to cdcl_cls_l[pos], and cdcl_cls_l[used] to cdcl_cls_l[pos]. After that, both
		//offsets must be updated to reflect the new position the clauses have in the list.
		--cdcl_cls_l_used;
		tmp = cdcl_cls_l[cdcl_cls_l_used];
		SHRINK_LEARNED_CDCL_CLS(c, cdcl_cls_l_used);
		cdcl_cls_l[pos] = tmp;
		SET_CDCL_CLS_OFFSET(tmp, pos);
	} else {
		//It is an original clause, we work in the cdcl_cls_o array. We first shrikn the clause. Then we must move the
		//pointer at cdcl_cls_l[used] to cdcl_cls_l[pos], and cdcl_cls_l[used] to cdcl_cls_l[pos]. After that, both
		//offsets must be updated to reflect the new position the clauses have in the list.
		--cdcl_cls_o_used;
		tmp = cdcl_cls_o[cdcl_cls_o_used];
		SHRINK_ORIGINAL_CDCL_CLS(c, cdcl_cls_o_used);
		cdcl_cls_o[pos] = tmp;
		SET_CDCL_CLS_OFFSET(tmp, pos);
	}

}

#ifdef VERBOSE_CDCL
void cdcl_clauses_printClauseLiterals_inline(cdclClause *c){
	uint32_t i;
	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		printf("%d ", c->lits[i]);
	}
}

void cdcl_clauses_printClauseOccPos_inline(cdclClause *c){
	uint32_t i;
	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		printf("%d ", c->occPos[ i ]);
	}
}

void cdcl_clauses_printClause(cdclClause* c){
	printf("c     CDCL:   CLS %p", (void*)c);
	if (c == NULL) {
		printf("\n");
		return;
	}
	printf(" BASE:");
	PRINT_BITS_UINT32(GET_CDCL_CLS_BASEDATA(c));
	printf(" REAS:%d LEARNED:%d BLOCKED:%d OFFSET:%6d LBD: %6d ", IS_CDCL_CLS_REASON(c), IS_CDCL_CLS_LEARNED(c),
			IS_CDCL_CLS_BLOCKED(c),	GET_CDCL_CLS_OFFSET(c), GET_CDCL_CLS_LBD(c));
	printf(" SIG:");
	PRINT_BITS_UINT64(GET_CDCL_CLS_SIG(c));
	printf("\tREPLIT:%7d\tW1:%7d(%d)\tW2:%7d(%d)\tSIZE:%7d\tLITS:",
			GET_CDCL_CLS_REPLIT(c),GET_CDCL_CLS_W1(c),GET_CDCL_CLS_WHEREW1(c),GET_CDCL_CLS_W2(c),
			GET_CDCL_CLS_WHEREW2(c),GET_CDCL_CLS_SIZE(c));
	if (GET_CDCL_CLS_LITS(c) == NULL){
		printf("NULL ");
	} else {
		cdcl_clauses_printClauseLiterals_inline(c);
	}
	printf("\tOCCPOS:");
	if (GET_CDCL_CLS_OCCPOS(c) == NULL){
		printf("NULL ");
	} else {
		cdcl_clauses_printClauseOccPos_inline(c);
	}
	printf("\n");
}

void cdcl_clauses_printClauseInline(cdclClause* c){
	printf("CLS ");
	if (GET_CDCL_CLS_LITS(c) == NULL){
		printf("NULL ");
	} else {
		cdcl_clauses_printClauseLiterals_inline(c);
	}
}

void cdcl_clauses_printOriginal(){
	uint32_t i;
	printf("c     CDCL:   Original Clauses (%u):\n", cdcl_cls_o_used);
	for (i=0; i < cdcl_cls_o_used; ++i){
		cdcl_clauses_printClause(cdcl_cls_o[i]);
	}
}

void cdcl_clauses_printLearned(){
	uint32_t i;
	printf("c     CDCL:   Learned Clauses (%u):\n", cdcl_cls_l_used);
	for (i=0; i < cdcl_cls_l_used; ++i){
		cdcl_clauses_printClause(cdcl_cls_l[i]);
	}
}

void cdcl_clauses_printLastNLearned(uint32_t numToPrint){
	uint32_t i;
	printf("c     CDCL:   Last %u learned Clauses:\n", numToPrint);
	for (i=cdcl_cls_l_used - numToPrint; i < cdcl_cls_l_used; ++i){
		cdcl_clauses_printClause(cdcl_cls_l[i]);
	}
}

void cdcl_clauses_printAll(){
	printf("c     CDCL:   CLAUSE DATA:\n");
	cdcl_clauses_printOriginal();
	cdcl_clauses_printLearned();
	printf("c     CDCL:\n");
}

uint32_t cdcl_clauses_verifyClause(cdclClause* c){
	cdclLiteral *litA, *litB;
	cdclVariable *v;
	variable *mainVar;
	uint64_t aSig = 0ULL;
	int32_t litNum, lit, occPos, repLit, w1Lit, w2Lit, repLitFound, w1LitFound, w2LitFound, whereW1, whereW2, numFalse, numTrue;

	//Make a small check to see if this clause exists.
	if (GET_CDCL_CLS_SIZE(c) == 0){
		printf("\nThe clause has size 0.\n");
		cdcl_clauses_printClause(c);
		return 0;
	}

	//First, we verify that the representative literal and the watchers are part of this clause, and that the watchers are
	//two different literals.
	repLit = GET_CDCL_CLS_REPLIT(c);
	w1Lit = GET_CDCL_CLS_W1(c);
	whereW1 = GET_CDCL_CLS_WHEREW1(c);
	w2Lit = GET_CDCL_CLS_W2(c);
	whereW2 = GET_CDCL_CLS_WHEREW2(c);
	repLitFound = 0;
	w1LitFound = 0;
	w2LitFound = 0;
	numFalse = 0;
	numTrue = 0;
	for (litNum = 0; litNum < GET_CDCL_CLS_SIZE(c); ++litNum){
		lit = GET_CDCL_CLS_LITNUM(c, litNum);
		v = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,litNum));
		mainVar = main_varData + abs(GET_CDCL_CLS_LITNUM(c,litNum));
		if (IS_VAR_DISABLED(mainVar) || !IS_VAR_UNASSIGNED(mainVar) || GET_VAR_ISEQUIVTO(mainVar) != 0){
			printf("\nThe variable is either disabled, already assigned, or has an equivalence in the outer formula.\n");
			return 0;
		}
		aSig |= GET_CDCL_LIT_SIG(lit);
		occPos = GET_CDCL_CLS_OCCPOSNUM(c, litNum);

		if (GET_CDCL_VAR_PTR_ISEQUIVTO(v) != 0){
			printf("\nThe clause contains a variable that is equivalent to some other. This cannot be.");
			cdcl_variables_printVariable(v);
			cdcl_clauses_printClause(c);
			return 0;
		}

		if (GET_CDCL_LIT_NUMOCCS_USED(lit) <= occPos){
			printf("\nThe occurrence position indicated in the clause for the literal is too large.\n");
			cdcl_literals_printLiteral(cdcl_literals + lit);
			cdcl_clauses_printClause(c);
			return 0;
		}

		if (GET_CDCL_LIT_OCCNUM(lit, occPos) != c){
			printf("\nThe occurrence information of the literal in the clause is wrong.");
			cdcl_literals_printLiteral(cdcl_literals + lit);
			cdcl_clauses_printClause(c);
			return 0;
		}
		if (lit == repLit) {repLitFound = 1;}
		if (lit == w1Lit) {w1LitFound = 1;}
		if (lit == w2Lit) {w2LitFound = 1;}
		if (IS_CDCL_LIT_UNSAT(lit)) ++numFalse;
		if (IS_CDCL_LIT_SAT(lit)) ++numTrue;
	}

	if (numFalse == GET_CDCL_CLS_SIZE(c)){
		printf("\nThe clause is false.\n");
		cdcl_clauses_printClause(c);
		return 0;
	}

	if (numFalse == GET_CDCL_CLS_SIZE(c)-1 && numTrue == 0){
		printf("\nThe clause is unit.\n");
		cdcl_clauses_printClause(c);
		return 0;
	}

	if (aSig != GET_CDCL_CLS_SIG(c)){
		printf("\nThe signature is wrong.\n");
		cdcl_clauses_printClause(c);
		return 0;
	}

	if (GET_CDCL_CLS_SIZE(c) == 2){
		if (!repLitFound){
			printf("\nReplit not correct.\n");
			cdcl_clauses_printClause(c);
			return 0;
		}
	} else {
		if (!repLitFound || !w1LitFound || !w2LitFound){
			printf("\nReplit or Watcher not correct.\n");
			cdcl_clauses_printClause(c);
			return 0;
		}
		if (w1Lit == w2Lit){
			printf("\nThe clause has two equivalent watchers.\n");
			cdcl_clauses_printClause(c);
			return 0;
		}
		litA = cdcl_literals + w1Lit;
		litB = cdcl_literals + w2Lit;

		if (GET_CDCL_LIT_PTR_NUMW1_USED(litA) <= whereW1){
			printf("\nThe whereW1 information of the clause is wrong (too large for list).\n");
			cdcl_literals_printLiteral(litA);
			cdcl_clauses_printClause(c);
			return 0;
		}

		if (GET_CDCL_LIT_PTR_NUMW2_USED(litB) <= whereW2){
			printf("\nThe whereW2 information of the clause is wrong (too large for list).\n");
			cdcl_literals_printLiteral(litB);
			cdcl_clauses_printClause(c);
			return 0;
		}

		if (GET_CDCL_LIT_PTR_W1_WATCHED_CLS(litA, whereW1) != c){
			printf("\nThe occurrence information for watcher 1 is wrong.\n");
			cdcl_literals_printLiteral(litA);
			cdcl_clauses_printClause(c);
			return 0;
		}
		if (GET_CDCL_LIT_PTR_W2_WATCHED_CLS(litB, whereW2) != c){
			printf("\nThe occurrence information for watcher 2 is wrong.\n");
			cdcl_literals_printLiteral(litB);
			cdcl_clauses_printClause(c);
			return 0;
		}
	}

	return 1;
}

uint32_t cdcl_clauses_verifyAll(){
	//This method performs a costly consistency check on all the clauses of the CDCL formula.
	cdclClause *c;
	uint32_t cNum;

	for (cNum = 0; cNum < cdcl_cls_o_used; ++cNum){
		c = cdcl_cls_o[cNum];
		if (!cdcl_clauses_verifyClause(c)){
			return 0;
		}
	}

	for (cNum = 0; cNum < cdcl_cls_l_used; ++cNum){
		c = cdcl_cls_l[cNum];
		if (!cdcl_clauses_verifyClause(c)){
			return 0;
		}
	}

	return 1;
}

#endif
