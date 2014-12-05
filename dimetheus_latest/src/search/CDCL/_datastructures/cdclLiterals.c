/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../cdcl.h"

void cdcl_literals_Q_moveUp(cdclLiteral* l){
	//To move an element up in the literal priority queue. Larger score means higher priority.
	cdclLiteral* parent;
	int32_t lPos, lScore, parentPos, parentScore;

	//We first get the current position of the literal and its score.
	lPos = GET_CDCL_LIT_PTR_QPOS(l);
	lScore = GET_CDCL_LIT_PTR_QSCORE(l);

	while(1){
		//We can stop right away if the literal is at the topmost position.
		if (lPos == 1) break;

		//We move the literal upwards by exchanging it with its parent as long as the parent has less score.
		//Division by two is shift right by one.
		parentPos = lPos >> 1;
		parent = cdcl_literals_Q[parentPos];
		parentScore = GET_CDCL_LIT_PTR_QSCORE(parent);

		//If the parent has less score than the literal, we can return.
		if (parentScore >= lScore) break;

		//Otherwise, we must exchange the clause and its parent. Put the parent at the current clause position.
		cdcl_literals_Q[lPos] = parent;
		SET_CDCL_LIT_PTR_QPOS(parent, lPos);

		//Then we put the literal in the position of the parent. We do not yet update the position of the clause. See the end.
		cdcl_literals_Q[parentPos] = l;
		lPos = parentPos;
	}

	//Finally we update the literal position in the priority queue.
	SET_CDCL_LIT_PTR_QPOS(l, lPos);
}

void cdcl_literals_Q_moveDown(cdclLiteral* l){
	//To move an element down in the literal priority queue. Larger score means higher priority.
	cdclLiteral *leftChild, *rightChild;
	int32_t lPos, lScore, leftChildPos, leftScore, rightChildPos, rightScore;
	lPos = GET_CDCL_LIT_PTR_QPOS(l);
	lScore = GET_CDCL_LIT_PTR_QSCORE(l);

	while(1){
		leftChildPos = lPos << 1;
		rightChildPos = (lPos << 1)+1;

		//No left child means no right child either. Literal already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= cdcl_literals_Q_leaf)	break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= cdcl_literals_Q_leaf) {
			leftChild = cdcl_literals_Q[leftChildPos];
			leftScore = GET_CDCL_LIT_PTR_QSCORE(leftChild);

			//If the left child has more score, we must move down the literal to this position. Otherwise we are done.
			if (leftScore <= lScore) break;

			//We are not done yet. We first lift the child and then push down the literal we want to push down.
			cdcl_literals_Q[lPos] = leftChild;
			SET_CDCL_LIT_PTR_QPOS(leftChild, lPos);

			//Then we push down the literal to the child position. We do not update its position yet, we do that at the end.
			cdcl_literals_Q[leftChildPos] = l;
			lPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a better score.
			leftChild = cdcl_literals_Q[leftChildPos];
			leftScore = GET_CDCL_LIT_PTR_QSCORE(leftChild);

			rightChild = cdcl_literals_Q[rightChildPos];
			rightScore = GET_CDCL_LIT_PTR_QSCORE(rightChild);

			if (leftScore < rightScore){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftScore = rightScore;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child does not have a better score than the clause itself.
			if (leftScore <= lScore) break;

			//We are not done yet. We first lift the child and then push down the literal.
			cdcl_literals_Q[lPos] = leftChild;
			SET_CDCL_LIT_PTR_QPOS(leftChild, lPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			cdcl_literals_Q[leftChildPos] = l;
			lPos = leftChildPos;
		}
	}
	//Finally we update the literal position position in the priority queue.
	SET_CDCL_LIT_PTR_QPOS(l, lPos);
}

cdclLiteral* cdcl_literals_Q_dequeueBest(){
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (cdcl_literals_Q_leaf == 1) return NULL;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (cdcl_literals_Q_leaf == 2){
		SET_CDCL_LIT_PTR_QPOS(cdcl_literals_Q[1],0);
		--cdcl_literals_Q_leaf;
		return cdcl_literals_Q[1];
	}

	//If there is more than one element left we pick the topmost element and signal that it is not in the queue anymore.
	cdclLiteral* best = cdcl_literals_Q[1];
	SET_CDCL_LIT_PTR_QPOS(best,0);

	//We override the first position with the last element in the queue and push it down afterwards.
	cdcl_literals_Q[1] = cdcl_literals_Q[--cdcl_literals_Q_leaf];
	SET_CDCL_LIT_PTR_QPOS(cdcl_literals_Q[1],1);

	cdcl_literals_Q_moveDown(cdcl_literals_Q[1]);
	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void cdcl_literals_Q_delete(cdclLiteral* l){
	//To delete an element from the literal priority queue.
	cdclLiteral *replacementL, *parent;
	int32_t lPos, replacementScore, parentPos, parentScore;
	//We first pick the current position of this literal in the queue.
	lPos = GET_CDCL_LIT_PTR_QPOS(l);

	//If the given literal is not in the queue we are already done.
	if (lPos == 0) return;

	//We set the element to be not in the queue anymore.
	SET_CDCL_LIT_PTR_QPOS(l,0);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (lPos == cdcl_literals_Q_leaf-1){
		--cdcl_literals_Q_leaf;
		return;
	}

	//We pick the replacement.
	replacementL = cdcl_literals_Q[--cdcl_literals_Q_leaf];

	//We override the position of the literal to be deleted by the one we picked as replacement.
	cdcl_literals_Q[lPos] = replacementL;

	//We update the position of the replacement literal in the queue.
	SET_CDCL_LIT_PTR_QPOS(replacementL,lPos);
	replacementScore = GET_CDCL_LIT_PTR_QSCORE(replacementL);

	//We then check if we have to move the element upwards.
	parentPos = lPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = cdcl_literals_Q[parentPos];
		parentScore = GET_CDCL_LIT_PTR_QSCORE(parent);

		//Check if the replacement must be moved up or down in the queue.
		if (parentScore >  replacementScore){
			//We must make the upwards move.
			cdcl_literals_Q_moveUp(replacementL);
		} else {
			//We must make the downwards move.
			cdcl_literals_Q_moveDown(replacementL);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		cdcl_literals_Q_moveDown(replacementL);
	}
}
void cdcl_literals_Q_enqueue(cdclLiteral* l){
	//To enqueue a literal.
	if (GET_CDCL_LIT_PTR_QPOS(l) > 0U) return;
	//First we put the literal on the next priority queue leaf.
	cdcl_literals_Q[cdcl_literals_Q_leaf] = l;
	//Then we update its position for being in the queue.
	SET_CDCL_LIT_PTR_QPOS(l,cdcl_literals_Q_leaf++);
	//Then we move the literal up in the queue until it is at the right position.
	cdcl_literals_Q_moveUp(l);
}

void cdcl_literals_Q_touchLiteral_inprocessing_fld(cdclLiteral* l){
	//This method is used whenever a literal is touched in failed literal detection mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_CDCL_LIT_PTR_QSCORE(l), newScore = 0;

	//We can return right away if the literal is already satisfied.
	if (! IS_CDCL_VAR_UNASSIGNED( abs(GET_CDCL_LIT_ID_FROM_PTR(l))) ) return;

	//The score of the literal is the number of variables minus its number of implications.
	newScore = GET_CDCL_LIT_PTR_NUMIMPS_USED(l);
	SET_CDCL_LIT_PTR_QSCORE(l,newScore);
	if (GET_CDCL_LIT_PTR_QPOS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore > oldScore){
			//Yes, something changed and the literal now has a better score.
			cdcl_literals_Q_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			cdcl_literals_Q_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		cdcl_literals_Q_enqueue(l);
	}
}

void cdcl_literals_Q_touchLiteral_inprocessing_nhbr(cdclLiteral* l){
	//This method is used whenever a literal is touched in NHBR mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_CDCL_LIT_PTR_QSCORE(l), newScore = 0;

	if (GET_CDCL_LIT_PTR_NUMIMPS_USED(l) < param_cdclIRNHBRMin) return;

	//We can return right away if the literal is already satisfied.
	if (! IS_CDCL_VAR_UNASSIGNED( abs(GET_CDCL_LIT_ID_FROM_PTR(l))) ) return;

	//The score of the literal is the number of variables minus its number of implications.
	newScore = GET_CDCL_LIT_PTR_NUMIMPS_USED(l);
	SET_CDCL_LIT_PTR_QSCORE(l,newScore);
	if (GET_CDCL_LIT_PTR_QPOS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore > oldScore){
			//Yes, something changed and the literal now has a better score.
			cdcl_literals_Q_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			cdcl_literals_Q_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		cdcl_literals_Q_enqueue(l);
	}
}

void cdcl_literals_Q_touchLiteral_inprocessing_treelook(cdclLiteral* l){
	//This method is used whenever a literal is touched in tree-look mode.
	int32_t oldScore = GET_CDCL_LIT_PTR_QSCORE(l), newScore = 0;

	//The score of the literal is the number of implications of the opposite.
	newScore = GET_CDCL_LIT_PTR_NUMIMPS_USED(GET_CDCL_LIT_PTR_OPPOSITE(l));
	SET_CDCL_LIT_PTR_QSCORE(l,newScore);
	if (GET_CDCL_LIT_PTR_QPOS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore > oldScore){
			//Yes, something changed and the literal now has a better score.
			cdcl_literals_Q_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			cdcl_literals_Q_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		cdcl_literals_Q_enqueue(l);
	}
}

void cdcl_literals_Q_touchLiteral_inprocessing_niver(cdclLiteral* l){
	//This method is used whenever a literal is touched in NIVER mode.
	int32_t oldScore = GET_CDCL_LIT_PTR_QSCORE(l), newScore = UINT32_MAX - GET_CDCL_LIT_PTR_NUMOCCS_USED(l);

	//We give the literal its score and update the priority queue with it.
	SET_CDCL_LIT_PTR_QSCORE(l,newScore);
	if (GET_CDCL_LIT_PTR_QPOS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal changed.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(l)+GET_CDCL_LIT_PTR_NUMOCCS_USED(GET_CDCL_LIT_PTR_OPPOSITE(l))>param_cdclIRNIVERMax){
			//The literal has too many occurrences. We remove it from the queue.
			cdcl_literals_Q_delete(l);
			cdcl_literals_Q_delete(GET_CDCL_LIT_PTR_OPPOSITE(l));
			return;
		}
		if (newScore > oldScore){
			//Yes, something changed and the literal now has a better score.
			cdcl_literals_Q_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			cdcl_literals_Q_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		if (GET_CDCL_LIT_PTR_NUMOCCS_USED(l)+GET_CDCL_LIT_PTR_NUMOCCS_USED(GET_CDCL_LIT_PTR_OPPOSITE(l))<=param_cdclIRNIVERMax){
			//The literal does not have too many occurrences.
			cdcl_literals_Q_enqueue(l);
		}
	}
}

void cdcl_literals_Q_touchLiteral_inprocessing_ge(cdclLiteral* l){
	//This method is used whenever a literal is touched in GE mode.
	int32_t oldScore = GET_CDCL_LIT_PTR_QSCORE(l), newScore = UINT32_MAX - GET_CDCL_LIT_PTR_NUMOCCS_USED(l);

	//Otherwise, we give the literal its score and update the priority queue with it.
	SET_CDCL_LIT_PTR_QSCORE(l,newScore);
	if (GET_CDCL_LIT_PTR_QPOS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal changed.
		if (newScore > oldScore){
			//Yes, something changed and the literal now has a better score.
			cdcl_literals_Q_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			cdcl_literals_Q_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		cdcl_literals_Q_enqueue(l);
	}
}

void cdcl_literals_Q_touchLiteral_null(cdclLiteral* l){

}

void cdcl_literals_Q_flush(){
	//To remove all elements in the literal queue.
	//This will flush out all the elements from the priority queue.
	int32_t i;
	for (i = 1; i < cdcl_literals_Q_leaf; ++i){
		cdcl_literals_Q[i]->lQPos = 0;
	}
	cdcl_literals_Q_leaf = 1;
}

void cdcl_literals_S_delete(cdclLiteral* l){
	//To delete a literal from the literal stack.
	int32_t pos = GET_CDCL_LIT_PTR_SPOS(l);
	if (pos == 0) return;

	SET_CDCL_LIT_PTR_SPOS(l, 0);

	if (pos == --cdcl_literals_S_leaf) {
		//Since the removed literal was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last variable that is now at vSLeaf position.
	cdcl_literals_S[pos] = cdcl_literals_S[cdcl_literals_S_leaf];
	SET_CDCL_LIT_PTR_SPOS(cdcl_literals_S[pos], pos);
}

void cdcl_literals_S_push(cdclLiteral* l){
	//To push something on the literal stack.
	if (GET_CDCL_LIT_PTR_SPOS(l) > 0) return;

	cdcl_literals_S[cdcl_literals_S_leaf] = l;
	SET_CDCL_LIT_PTR_SPOS(l, cdcl_literals_S_leaf);
	++cdcl_literals_S_leaf;
}

void cdcl_literals_S_touchLiteral_null(cdclLiteral* l){
	//To do nothing.
}

void cdcl_literals_S_touchLiteral_inprocessing_ternary(cdclLiteral* l){
	//This method is used whenever a literal is touched in TERNARY mode.

	if (GET_CDCL_LIT_PTR_NUMOCCS_USED(l) == 0 || GET_CDCL_LIT_PTR_NUMOCCS_USED(GET_CDCL_LIT_PTR_OPPOSITE(l)) == 0) return;
	if (GET_CDCL_LIT_PTR_SPOS(l) > 0 || GET_CDCL_LIT_PTR_SPOS(GET_CDCL_LIT_PTR_OPPOSITE(l)) > 0) return;

	cdcl_literals_S[cdcl_literals_S_leaf] = l;
	SET_CDCL_LIT_PTR_SPOS(l, cdcl_literals_S_leaf);
	++cdcl_literals_S_leaf;
}

cdclLiteral* cdcl_literals_S_pop(){
	//To pop the topmost element from the stack.
	if (cdcl_literals_S_leaf == 1) return NULL;

	--cdcl_literals_S_leaf;
	SET_CDCL_LIT_PTR_SPOS(cdcl_literals_S[cdcl_literals_S_leaf],0);
	return cdcl_literals_S[cdcl_literals_S_leaf];
}

void cdcl_literals_S_flush(){
	//To remove all elements from the literal stack.
	uint32_t i;
	for (i = 1; i < cdcl_literals_S_leaf; ++i){
		SET_CDCL_LIT_PTR_SPOS(cdcl_literals_S[i], 0);
	}
	cdcl_literals_S_leaf = 1;
}

void cdcl_literals_prepareCall(){
	//This method resets all literals for the snapshot formula.
	int32_t i,vid;

	for (i = 0; i < f.n_vars_e_used; ++i){
		vid = f.vars_e[i]->id;

		//Initialize the positive literal.
		SET_CDCL_LIT_STAMP(vid, 0);
		SET_CDCL_LIT_UPSTAMP(vid, 0);
		SET_CDCL_LIT_NUMOCCS_USED(vid, 0);
		SET_CDCL_LIT_NUMIMPS_USED(vid, 0);
		SET_CDCL_LIT_NUMW1_USED(vid, 0);
		SET_CDCL_LIT_NUMW2_USED(vid, 0);
		SET_CDCL_LIT_SPOS(vid, 0);
		SET_CDCL_LIT_QPOS(vid, 0);
		SET_CDCL_LIT_QSCORE(vid, 0);

		//Initialize the negative literal.
		SET_CDCL_LIT_STAMP(-vid, 0);
		SET_CDCL_LIT_UPSTAMP(-vid, 0);
		SET_CDCL_LIT_NUMOCCS_USED(-vid, 0);
		SET_CDCL_LIT_NUMIMPS_USED(-vid, 0);
		SET_CDCL_LIT_NUMW1_USED(-vid, 0);
		SET_CDCL_LIT_NUMW2_USED(-vid, 0);
		SET_CDCL_LIT_SPOS(-vid, 0);
		SET_CDCL_LIT_QPOS(-vid, 0);
		SET_CDCL_LIT_QSCORE(-vid, 0);
	}

	cdcl_literals_Q_flush();
	cdcl_literals_Q_touchLiteral = &cdcl_literals_Q_touchLiteral_null;
	cdcl_literals_S_flush();
	cdcl_literals_S_touchLiteral = &cdcl_literals_S_touchLiteral_null;

}

void cdcl_literals_reset(){
	cdcl_literals = NULL;
	cdcl_literals_S = NULL;
	cdcl_literals_S_leaf = 0;
	cdcl_literals_Q = NULL;
	cdcl_literals_Q_leaf = 0;
}

void cdcl_literals_init(){
	uint32_t i;

	//Reset values.
	cdcl_litStamp = 1U;

	//We allocate the literals array.
	cdcl_literals = malloc(sizeof(cdclLiteral)*(2*f.n_initial+1U));
	if (cdcl_literals == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the literals. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}

	//We make sure that all the literals have occurrence lists and reset values.
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_OCCS(i, NULL);
		SET_CDCL_LIT_STAMP(i, 0);
		SET_CDCL_LIT_UPSTAMP(i, 0);
	}
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_NUMOCCS_AVAIL(i,BLOCKSIZE);
		cdcl_literals[i].occs = malloc(sizeof(cdclClause*)*GET_CDCL_LIT_NUMOCCS_AVAIL(i));
		if (GET_CDCL_LIT_OCCS(i) == NULL){
			printf("c ERROR. CDCL was unable to allocate memory for the literals occurrence list. Out of memory?\n");
			cdcl_returnCode = CDCL_ERROR;
			#ifdef COLLECTSTATS
			++stats_cdcl_return_ERROR;
			#endif
			return;
		}
		SET_CDCL_LIT_NUMOCCS_USED(i,0);
		cdcl_literals[i].occs[0] = NULL;
	}

	//We make sure that all the literals have literal occurrence position lists.
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_OCCLITPOS(i, NULL);
	}
	for (i = 0; i <= 2*f.n_initial; ++i){
		//The occLitPos array has the exact same size as the occurrence array.
		cdcl_literals[i].occLitPos = malloc(sizeof(uint32_t*)*GET_CDCL_LIT_NUMOCCS_AVAIL(i));
		if (GET_CDCL_LIT_OCCLITPOS(i) == NULL){
			printf("c ERROR. CDCL was unable to allocate memory for the literals occurrence position list. Out of memory?\n");
			cdcl_returnCode = CDCL_ERROR;
			#ifdef COLLECTSTATS
			++stats_cdcl_return_ERROR;
			#endif
			return;
		}
		cdcl_literals[i].occLitPos[0] = 0;
	}

	//We make sure that all the literals have implication lists.
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_IMPLIES(i, NULL);
	}
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_NUMIMPS_AVAIL(i,2*BLOCKSIZE);
		cdcl_literals[i].implies = malloc(sizeof(cdclClause*)*GET_CDCL_LIT_NUMIMPS_AVAIL(i));
		if (GET_CDCL_LIT_IMPLIES(i) == NULL){
			printf("c ERROR. CDCL was unable to allocate memory for the literals implication list. Out of memory?\n");
			cdcl_returnCode = CDCL_ERROR;
			#ifdef COLLECTSTATS
			++stats_cdcl_return_ERROR;
			#endif
			return;
		}
		SET_CDCL_LIT_NUMIMPS_USED(i,0);
	}

	//We make sure that all the literals have watcher lists.
	for (i = 0; i <= 2*f.n_initial; ++i){
		SET_CDCL_LIT_W1(i, NULL);
		SET_CDCL_LIT_W2(i, NULL);
	}
	for (i = 0; i <= 2*f.n_initial; ++i){
		//Watcher 1
		SET_CDCL_LIT_NUMW1_AVAIL(i,BLOCKSIZE);
		cdcl_literals[i].w1 = malloc(sizeof(cdclClause*)*BLOCKSIZE);
		if (GET_CDCL_LIT_W1(i) == NULL){
			printf("c ERROR. CDCL was unable to allocate memory for the literals implication list. Out of memory?\n");
			cdcl_returnCode = CDCL_ERROR;
			#ifdef COLLECTSTATS
			++stats_cdcl_return_ERROR;
			#endif
			return;
		}
		SET_CDCL_LIT_NUMW1_USED(i,0);
		//Watcher 2
		SET_CDCL_LIT_NUMW2_AVAIL(i,BLOCKSIZE);
		cdcl_literals[i].w2 = malloc(sizeof(cdclClause*)*BLOCKSIZE);
		if (GET_CDCL_LIT_W2(i) == NULL){
			printf("c ERROR. CDCL was unable to allocate memory for the literals implication list. Out of memory?\n");
			cdcl_returnCode = CDCL_ERROR;
			#ifdef COLLECTSTATS
			++stats_cdcl_return_ERROR;
			#endif
			return;
		}
		SET_CDCL_LIT_NUMW2_USED(i,0);
	}


	//Shift the pointer.
	cdcl_literals += f.n_initial;

	//We make sure we have a literal stack that can hold all the literals in the worst case.
	cdcl_literals_S = malloc(sizeof(cdclLiteral*)*(2*f.n_initial+2));
	if (cdcl_literals_S == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the literal stack. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_literals_S_leaf = 1;

	//We make sure we have a literal stack that can hold all the literals in the worst case.
	cdcl_literals_Q = malloc(sizeof(cdclLiteral*)*(2*f.n_initial+2));
	if (cdcl_literals_Q == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the literal priority queue. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_literals_Q_leaf = 1;

}

void cdcl_literals_dispose(){
	uint32_t i;
	if (cdcl_literals != NULL){
		cdcl_literals -= f.n_initial;

		for (i = 0; i <= 2*f.n_initial; ++i){
			if (GET_CDCL_LIT_OCCS(i) != NULL){
				free(cdcl_literals[i].occs);
				cdcl_literals[i].occs = NULL;
			}
			if (GET_CDCL_LIT_OCCLITPOS(i) != NULL){
				free(cdcl_literals[i].occLitPos);
				cdcl_literals[i].occLitPos = NULL;
			}
			if (GET_CDCL_LIT_IMPLIES(i) != NULL){
				free(cdcl_literals[i].implies);
				cdcl_literals[i].implies = NULL;
			}
			if (GET_CDCL_LIT_W1(i) != NULL){
				free(cdcl_literals[i].w1);
				cdcl_literals[i].w1 = NULL;
			}
			if (GET_CDCL_LIT_W2(i) != NULL){
				free(cdcl_literals[i].w2);
				cdcl_literals[i].w2 = NULL;
			}
		}
		free(cdcl_literals);
		cdcl_literals = NULL;
	}

	if (cdcl_literals_S != NULL){
		free(cdcl_literals_S);
		cdcl_literals_S = NULL;
	}
	cdcl_literals_S_leaf = 0;

	if (cdcl_literals_Q != NULL){
		free(cdcl_literals_Q);
		cdcl_literals_Q = NULL;
	}
	cdcl_literals_Q_leaf = 0;
}

#ifdef VERBOSE_CDCL
void cdcl_literals_printLiteralImplications(cdclLiteral* l){
	uint32_t i;
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i += 2){
		printf("%d %p ", (int32_t)(addr_t)(l->implies[ i ]), (void *)(l->implies[ i +1]));
	}
}

void cdcl_literals_printLiteralOccs(cdclLiteral *l){
	uint32_t i;
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(l); ++i){
		printf("%p ", (void*) l->occs[ i ]);
	}
}

void cdcl_literals_printLiteralOccLitPos(cdclLiteral *l){
	uint32_t i;
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMOCCS_USED(l); ++i){
		printf("%u ", l->occLitPos[ i ]);
	}
}

void cdcl_literals_printLiteralW1List(cdclLiteral *l){
	uint32_t i;
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMW1_USED(l); ++i){
		printf("%p ", (void*) l->w1[ i ]);
	}
}

void cdcl_literals_printLiteralW2List(cdclLiteral *l){
	uint32_t i;
	for (i = 0; i < GET_CDCL_LIT_PTR_NUMW2_USED(l); ++i){
		printf("%p ", (void*) l->w2[ i ]);
	}
}

void cdcl_literals_printLiteral(cdclLiteral* l){
	printf("c     CDCL:   LIT: %6d ", GET_CDCL_LIT_ID_FROM_PTR(l));
	printf("STAMP:%10d\tUPSTAMP:%10d\tIMP(U/A/LIST):%d/%d/",
			GET_CDCL_LIT_PTR_STAMP(l),GET_CDCL_LIT_PTR_UPSTAMP(l),GET_CDCL_LIT_PTR_NUMIMPS_USED(l),
			GET_CDCL_LIT_PTR_NUMIMPS_AVAIL(l));
	cdcl_literals_printLiteralImplications(l);
	printf("\tOCC(U/A/LIST):%d/%d/", GET_CDCL_LIT_PTR_NUMOCCS_USED(l), GET_CDCL_LIT_PTR_NUMOCCS_AVAIL(l));
	cdcl_literals_printLiteralOccs(l);
	printf("\tOCCLITPOS(U/A/LIST):%d/%d/", GET_CDCL_LIT_PTR_NUMOCCS_USED(l), GET_CDCL_LIT_PTR_NUMOCCS_AVAIL(l));
	cdcl_literals_printLiteralOccLitPos(l);
	printf("\tW1LIST(U/A/LIST):%d/%d/", GET_CDCL_LIT_PTR_NUMW1_USED(l), GET_CDCL_LIT_PTR_NUMW1_AVAIL(l));
	cdcl_literals_printLiteralW1List(l);
	printf("\tW2LIST(U/A/LIST):%d/%d/", GET_CDCL_LIT_PTR_NUMW2_USED(l), GET_CDCL_LIT_PTR_NUMW2_AVAIL(l));
	cdcl_literals_printLiteralW2List(l);
	printf("\n");
}

void cdcl_literals_printAll(){
	int32_t i;
	cdclLiteral *l;
	printf("c     CDCL:   LITERAL DATA:\n");
	for (i = 1; i < f.n_initial+1; ++i){
		l = cdcl_literals + i;
		cdcl_literals_printLiteral(l);
		l = cdcl_literals - i;
		cdcl_literals_printLiteral(l);
	}
	printf("c     CDCL:\n");
}
#endif
