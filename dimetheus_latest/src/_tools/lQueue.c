/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "lQueue.h"

void lQ_reset(){
	//Reset the literal queue.
	lQ_touchLiteral = &lQ_touchLiteral_null;	//We reset the current touch method to be NULL.
	lQ = NULL;				//Priority queue does not yet exist.
	lQ_leaf = 1;				//The leaf is at first position if the queue is empty.
}

uint32_t lQ_init(){
	//For initializing once f.n_initial is known.
	lQ = malloc(sizeof(literal*)*(2*f.n_initial + 1)); //+1 for the initial field that is never used.
	if (lQ == NULL){
		printf("c ERROR. The allocation of the literal priority queue failed. Out of memory?\n");
		return 0;
	}
	lQ_leaf = 1;								//The queue is initialized and empty. So the leaf is at position 1.
	lQ_touchLiteral = &lQ_touchLiteral_fld;	//The first literal touch method we use is the one for failed literal detection.
	return 1;
}

void lQ_dispose(){
	//For freeing all related memory.
	lQ_touchLiteral = &lQ_touchLiteral_null;
	if (lQ != NULL){
		free(lQ);
		lQ = NULL;
		lQ_leaf = 1;
	}
}

//Methods for handling the literal queue lQ.
void lQ_moveUp(literal* l){
	literal* parent;
	int32_t lPos, lScore, parentPos, parentScore;

	//We first get the current position of the literal and its score.
	lPos = GET_LIT_Q_POS(l);
	lScore = GET_LIT_Q_SCORE(l);

	while(1){
		//We can stop right away if the literal is at the topmost position.
		if (lPos == 1) break;

		//We move the literal upwards by exchanging it with its parent as long as the parent has more score.
		//Division by two is shift right by one.
		parentPos = lPos >> 1;
		parent = lQ[parentPos];
		parentScore = GET_LIT_Q_SCORE(parent);

		//If the parent has less score than the literal, we can return.
		if (parentScore <= lScore) break;

		//Otherwise, we must exchange the clause and its parent. Put the parent at the current clause position.
		lQ[lPos] = parent;
		SET_LIT_Q_POS(parent, lPos);

		//Then we put the literal in the position of the parent. We do not yet update the position of the clause. See the end.
		lQ[parentPos] = l;
		lPos = parentPos;
	}

	//Finally we update the literal position in the priority queue.
	SET_LIT_Q_POS(l, lPos);
}

void lQ_moveDown(literal* l){
	literal *leftChild, *rightChild;
	int32_t lPos, lScore, leftChildPos, leftScore, rightChildPos, rightScore;
	lPos = GET_LIT_Q_POS(l);
	lScore = GET_LIT_Q_SCORE(l);

	while(1){
		leftChildPos = lPos << 1;
		rightChildPos = (lPos << 1)+1;

		//No left child means no right child either. Literal already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= lQ_leaf)	break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= lQ_leaf) {
			leftChild = lQ[leftChildPos];
			leftScore = GET_LIT_Q_SCORE(leftChild);

			//If the left child has more score, we must move down the literal to this position. Otherwise we are done.
			if (leftScore >= lScore) break;

			//We are not done yet. We first lift the child and then push down the literal we want to push down.
			lQ[lPos] = leftChild;
			SET_LIT_Q_POS(leftChild, lPos);

			//Then we push down the literal to the child position. We do not update its position yet, we do that at the end.
			lQ[leftChildPos] = l;
			lPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a better score.
			leftChild = lQ[leftChildPos];
			leftScore = GET_LIT_Q_SCORE(leftChild);

			rightChild = lQ[rightChildPos];
			rightScore = GET_LIT_Q_SCORE(rightChild);

			if (leftScore > rightScore){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftScore = rightScore;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child does not have a better score than the clause itself.
			if (leftScore >= lScore) break;

			//We are not done yet. We first lift the child and then push down the literal.
			lQ[lPos] = leftChild;
			SET_LIT_Q_POS(leftChild, lPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			lQ[leftChildPos] = l;
			lPos = leftChildPos;
		}
	}
	//Finally we update the literal position position in the priority queue.
	SET_LIT_Q_POS(l, lPos);
}

literal* lQ_dequeueBest(){
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (lQ_leaf == 1) return NULL;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (lQ_leaf == 2){
		SET_LIT_Q_POS(lQ[1],0);
		--lQ_leaf;
		return lQ[1];
	}

	//If there is more than one element left we pick the topmost element and signal that it is not in the queue anymore.
	literal* best = lQ[1];
	SET_LIT_Q_POS(best,0);

	//We override the first position with the last element in the queue and push it down afterwards.
	lQ[1] = lQ[--lQ_leaf];
	SET_LIT_Q_POS(lQ[1],1);

	lQ_moveDown(lQ[1]);
	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void lQ_delete(literal* l){
	literal *replacementL, *parent;
	int32_t lPos, replacementScore, parentPos, parentScore;
	//We first pick the current position of this literal in the queue.
	lPos = GET_LIT_Q_POS(l);

	//If the given literal is not in the queue we are already done.
	if (lPos == 0) return;

	//We set the element to be not in the queue anymore.
	SET_LIT_Q_POS(l,0);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (lPos == lQ_leaf-1){
		--lQ_leaf;
		return;
	}

	//We pick the replacement.
	replacementL = lQ[--lQ_leaf];

	//We override the position of the literal to be deleted by the one we picked as replacement.
	lQ[lPos] = replacementL;

	//We update the position of the replacement literal in the queue.
	SET_LIT_Q_POS(replacementL,lPos);
	replacementScore = GET_LIT_Q_SCORE(replacementL);

	//We then check if we have to move the element upwards.
	parentPos = lPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = lQ[parentPos];
		parentScore = GET_LIT_Q_SCORE(parent);

		//Check if the replacement must be moved up or down in the queue.
		if (parentScore >  replacementScore){
			//We must make the upwards move.
			lQ_moveUp(replacementL);
		} else {
			//We must make the downwards move.
			lQ_moveDown(replacementL);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		lQ_moveDown(replacementL);
	}
}

void lQ_enqueue(literal* l){
	//First we put the literal on the next priority queue leaf.
	lQ[lQ_leaf] = l;
	//Then we update its position for being in the queue.
	SET_LIT_Q_POS(l,lQ_leaf++);
	//Then we move the literal up in the queue until it is at the right position.
	lQ_moveUp(l);
}

void lQ_touchLiteral_null(literal* l){
	//This method does not do anything at all.
}

void lQ_touchLiteral_fld(literal* l){
	//This method is used whenever a literal is touched in failed literal detection mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = INT32_MAX;

	//We can return right away if the literal is already satisfied.
	if (! IS_VAR_UNASSIGNED( (main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//The score of the literal is the number of variables minus its number of implications. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial - l->numImpUsed;
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_slfld(literal* l){
	//This method is used whenever a literal is touched in single-look failed literal detection mode. We are looking for
	//literals that have at least one binary implication.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = INT32_MAX;

	//We can return right away if the literal is already satisfied.
	if (! IS_VAR_UNASSIGNED( (main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//The score of the literal is the number of variables minus its number of implications. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial - l->numImpUsed;
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_acce(literal* l){
	//This method is used whenever a literal is touched in ACCE mode. We are looking for literals that
	//have a small amount of occurrences in the opposite literal.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = INT32_MAX;

	//We can return right away if the variable is already assigned.
	if (! IS_VAR_UNASSIGNED( (main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//The score of the literal is the number of occurrences the opposite literal has. The more, the worse. We aim to minimize
	//the number of possible resolutions for this literal. The less, the easier it may be to have this clause being blocked
	//or covered.
	newScore = (main_litData - GET_LIT_NUM_FROM_PTR(l))->numOccsUsed;
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update.
		if (newScore > param_prepACCEMax) {
			//The literal has too many occurrences, we do not want to check it with ACCE anymore.
			lQ_delete(l);
			return;
		}
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		if (newScore > param_prepACCEMax) return;
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_nhbr(literal* l){
	//This method is used whenever a literal is touched in NHBR mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = INT32_MAX;

	//We can return right away if the literal is already satisfied.
	if (! IS_VAR_UNASSIGNED( (main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//We can return right away if the literal has too few implications.
	if (GET_LIT_NUMIMP_USED(l) < param_prepNHBRMin) return;

	//The score of the literal is the number of variables minus its number of implications. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial - GET_LIT_NUMIMP_USED(l);
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_inp_fld(literal* l){
	//This method is used whenever a literal is touched in failed literal detection mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = INT32_MAX;

	//We can return right away if the literal is already satisfied.
	if (! IS_VAR_UNASSIGNED( (main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//The score of the literal is the number of variables minus its number of implications. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial -  GET_LIT_NUMIMP_USED(l);
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_inp_nhbr(literal* l){
	//This method is used whenever a literal is touched in NHBR mode. We are looking for literals that
	//have at least one binary implication.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = 0;

	if (GET_LIT_NUMIMP_USED(l) < param_inpNHBRMin) return;

	//We can return right away if the literal is already satisfied.
	if (!IS_VAR_UNASSIGNED((main_varData + abs(GET_LIT_NUM_FROM_PTR(l)))) ) return;

	//The score of the literal is the number of variables minus its number of implications. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial - GET_LIT_NUMIMP_USED(l);
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_inp_treelook(literal* l){
	//This method is used whenever a literal is touched in tree-look mode.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = 0;

	//The score of the literal is the number of implications of the opposite. Since the queuing system prefers
	//small scores, the literals priority will be the higher, the more implications it has.
	newScore = f.n_initial - GET_LIT_NUMIMP_USED(GET_LIT_OPPOSITE(l));
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the literal up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		lQ_enqueue(l);
	}
}

void lQ_touchLiteral_inp_niver(literal* l){
	//This method is used whenever a literal is touched in NIVER mode.
	int32_t oldScore = GET_LIT_Q_SCORE(l), newScore = GET_LIT_NUMOCCS_USED(l);

	//We give the literal its score and update the priority queue with it.
	SET_LIT_Q_SCORE(l,newScore);
	if (GET_LIT_Q_POS(l) != 0){
		//The literal is already in the queue, but receives an update in its position since something for the literal changed.
		if (GET_LIT_NUMOCCS_USED(l)+GET_LIT_NUMOCCS_USED(GET_LIT_OPPOSITE(l))>param_inpNiverMax){
			//The literal has too many occurrences. We remove it from the queue.
			lQ_delete(l);
			lQ_delete(GET_LIT_OPPOSITE(l));
			return;
		}
		if (newScore < oldScore){
			//Yes, something changed and the literal now has a better score.
			lQ_moveUp(l);
		} else {
			//Yes, something changed and the literal now has a worse score than before.
			lQ_moveDown(l);
		}
	} else {
		//The literal is not in the queue yet.
		if (GET_LIT_NUMOCCS_USED(l)+GET_LIT_NUMOCCS_USED(GET_LIT_OPPOSITE(l))<=param_inpNiverMax){
			//The literal does not have too many occurrences.
			lQ_enqueue(l);
		}
	}
}

void lQ_flush(){
	//This will flush out all the elements from the priority queue.
	int32_t i;
	for (i = 1; i < lQ_leaf; ++i){
		lQ[i]->lQPos = 0;
	}
	lQ_leaf = 1;
}

//Methods for handling the literal stack.
void lS_reset(){
	//Reset the variable stack.
	lS = NULL;									//Stack does not yet exist.
	lS_touchLiteral = &lS_touchLiteral_null;	//No touching operation defined yet.
	lS_leaf = 1;									//The leaf is at first position if the stack is empty.
}

uint32_t lS_init(){
	//For initializing once f.n_initial is known.
	lS = malloc(sizeof(literal*)*(2*f.n_initial + 1)); //+1 for the initial field that is never used.
	if (lS == NULL){
		printf("c ERROR. The allocation of the literal stack failed. Out of memory?\n");
		return 0;
	}
	lS_leaf = 1;			//The stack is initialized and empty. So the leaf is at position 1.
	return 1;
}

void lS_dispose(){
	//For freeing all related memory.
	if (lS != NULL){
		free(lS);
		lS = NULL;
		lS_touchLiteral = &lS_touchLiteral_null;
		lS_leaf = 1;
	}
}

literal* lS_pop(){
	//To pop the topmost element in the stack.
	if (lS_leaf == 1) return NULL;

	--lS_leaf;
	SET_LIT_S_POS(lS[lS_leaf],0);
	return lS[lS_leaf];
}

void lS_delete(literal* l){
	//To delete a variable from the variable stack.
	int32_t pos = GET_LIT_S_POS(l);
	if (pos == 0) return;

	SET_LIT_S_POS(l, 0);

	if (pos == --lS_leaf) {
		//Since the removed literal was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last variable that is now at vSLeaf position.
	lS[pos] = lS[lS_leaf];
	lS[pos]->lSPos = pos;
}

void lS_touchLiteral_null(literal* l){
	//To do absolutely nothing.
}

void lS_touchLiteral_ternary(literal* l){
	//This method is used whenever a literal is touched in TERNARY mode.

	if (GET_LIT_NUMOCCS_USED(l) == 0 || GET_LIT_NUMOCCS_USED(GET_LIT_OPPOSITE(l)) == 0) return;
	if (GET_LIT_S_POS(l) > 0 || GET_LIT_S_POS(GET_LIT_OPPOSITE(l)) > 0) return;

	lS[lS_leaf] = l;
	SET_LIT_S_POS(l, lS_leaf);
	++lS_leaf;
}

void lS_push(literal *l){
	if (GET_LIT_S_POS(l) == 0){
		lS[lS_leaf] = l;
		SET_LIT_S_POS(l, lS_leaf);
		++lS_leaf;
	}
}

literal* lS_readAt(uint32_t pos){
	//To read the element in the stack found at the given position.
	if (pos >= lS_leaf || pos < 1) return NULL;
	return lS[pos];
}


void lS_flush(){
	//To remove all elements from the stack.
	int32_t i;
	for (i = 1; i < lS_leaf; ++i){
		lS[i]->lSPos = 0;
	}
	lS_leaf = 1;
}
