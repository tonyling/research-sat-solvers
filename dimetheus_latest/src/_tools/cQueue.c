/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "cQueue.h"

void cQ_reset(){
	//Reset the clause queue. SMALLER SCORE VALUES MEAN HIGHER PRIORITY.
	cQ_touchClause = &cQ_touchClause_null;	//We reset the type of priority queue we have.
	cQ = NULL;								//Priority queue does not yet exist.
	cQ_leaf = 1;							//The leaf is at first position if the queue is empty.
	cQ_avail = 0;							//No memory available yet.
}

uint32_t cQ_init(){
	//For initializing once f.m_initial is known.
	cQ_avail = (f.m_initial + 1);
	cQ = malloc(sizeof(clause*)*cQ_avail); //+1 for the initial field that is never used.
	if (cQ == NULL){
		printf("c ERROR. The allocation of the clause priority queue failed. Out of memory?\n");
		return 0;
	}
	cQ_leaf = 1;							//The queue is initialized and empty. So the leaf is at position 1.
	cQ_touchClause = &cQ_touchClause_se;	//We will use subsumption elimination as a default.
	return 1;
}

void cQ_dispose(){
	//For freeing all related memory.
	cQ_touchClause = &cQ_touchClause_null;
	if (cQ != NULL){
		free(cQ);
		cQ = NULL;
		cQ_leaf = 1;
		cQ_avail = 0;
	}
}

//Methods for handling the clause priority queue cQ.
void cQ_moveUp(clause* c){
	clause* parent;
	int32_t cPos, cScore, parentPos, parentScore;

	//We first get the current position of the clause and its score.
	cPos = GET_CLS_Q_POS(c);
	cScore = GET_CLS_Q_SCORE(c);

	while(1){
		//We can stop right away if the clause is at the topmost position.
		if (cPos == 1) break;

		//We move the clause upwards by exchanging it with its parent as long as the parent has more less score.
		//Division by two is shift right by one.
		parentPos = cPos >> 1;
		parent = cQ[parentPos];
		parentScore = GET_CLS_Q_SCORE(parent);

		//If the parent has less score than the clause, we can return.
		if (parentScore <= cScore) break;

		//Otherwise, we must exchange the clause and its parent. Put the parent at the current clause position.
		cQ[cPos] = parent;
		SET_CLS_Q_POS(parent, cPos);

		//Then we put the clause in the position of the parent. We do not yet update the position of the clause. See the end.
		cQ[parentPos] = c;
		cPos = parentPos;
	}

	//Finally we update the clauses position in the priority queue.
	SET_CLS_Q_POS(c, cPos);
}

void cQ_moveDown(clause* c){
	clause *leftChild, *rightChild;
	int32_t cPos, cScore, leftChildPos, leftScore, rightChildPos, rightScore;
	cPos = GET_CLS_Q_POS(c);
	cScore = GET_CLS_Q_SCORE(c);

	while(1){
		leftChildPos = cPos << 1;
		rightChildPos = (cPos << 1)+1;

		//No left child means no right child either. Clause already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= cQ_leaf)	break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= cQ_leaf) {
			leftChild = cQ[leftChildPos];
			leftScore = GET_CLS_Q_SCORE(leftChild);

			//If the left child more score, we must move down the clause to this position. Otherwise we are done.
			if (leftScore >= cScore) break;

			//We are not done yet. We first lift the child and then push down the clause we want to push down.
			cQ[cPos] = leftChild;
			SET_CLS_Q_POS(leftChild, cPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			cQ[leftChildPos] = c;
			cPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a better score.
			leftChild = cQ[leftChildPos];
			leftScore = GET_CLS_Q_SCORE(leftChild);

			rightChild = cQ[rightChildPos];
			rightScore = GET_CLS_Q_SCORE(rightChild);

			if (leftScore > rightScore){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftScore = rightScore;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child does not have a better score than the clause itself.
			if (leftScore >= cScore) break;

			//We are not done yet. We first lift the child and then push down the clause.
			cQ[cPos] = leftChild;
			SET_CLS_Q_POS(leftChild, cPos);

			//Then we push down the clause to the child position. We do not update its position yet, we do that at the end.
			cQ[leftChildPos] = c;
			cPos = leftChildPos;
		}
	}
	//Finally we update the clause position position in the priority queue.
	SET_CLS_Q_POS(c, cPos);
}

clause* cQ_dequeueBest(){
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (cQ_leaf == 1) return NULL;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (cQ_leaf == 2){
		SET_CLS_Q_POS(cQ[1],0);
		--cQ_leaf;
		return cQ[1];
	}

	//If there is more than one element left we pick the topmost element and signal that it is not in the queue anymore.
	clause* best = cQ[1];
	SET_CLS_Q_POS(best,0);

	//We override the first position with the last element in the queue and push it down afterwards.
	cQ[1] = cQ[--cQ_leaf];
	SET_CLS_Q_POS(cQ[1],1);

	cQ_moveDown(cQ[1]);
	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void cQ_delete(clause* c){
	clause *replacementC, *parent;
	int32_t cPos, replacementScore, parentPos, parentScore;
	//We first pick the current position of this clause in the queue.
	cPos = GET_CLS_Q_POS(c);

	//If the given clause is not in the queue we are already done.
	if (cPos == 0) return;

	//We set the element to be not in the queue anymore.
	SET_CLS_Q_POS(c,0);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (cPos == cQ_leaf-1){
		--cQ_leaf;
		return;
	}

	//We pick the replacement.
	replacementC = cQ[--cQ_leaf];

	//We override the position of the clause to be deleted by the one we picked as replacement.
	cQ[cPos] = replacementC;

	//We update the position of the replacement clause in the queue.
	SET_CLS_Q_POS(replacementC,cPos);
	replacementScore = GET_CLS_Q_SCORE(replacementC);

	//We then check if we have to move the element upwards.
	parentPos = cPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = cQ[parentPos];
		parentScore = GET_CLS_Q_SCORE(parent);

		//Check if the replacement must be moved up or down in the queue.
		if (parentScore >  replacementScore){
			//We must make the upwards move.
			cQ_moveUp(replacementC);
		} else {
			//We must make the downwards move.
			cQ_moveDown(replacementC);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		cQ_moveDown(replacementC);
	}
}

void cQ_enqueue(clause* c){
	//First we put the clause on the next priority queue leaf.
	cQ[cQ_leaf] = c;
	//Then we update its position for being in the queue.
	SET_CLS_Q_POS(c,cQ_leaf++);
	//Increase the size of the queue if necessary.
	if (cQ_leaf == cQ_avail){
		cQ_avail += 8*BLOCKSIZE;
		cQ = realloc(cQ, sizeof(clause*)*cQ_avail);
	}
	//Then we move the clause up in the queue until it is at the right position.
	cQ_moveUp(c);
}


void cQ_touchClause_null(clause* c){
	//This null method does not do anything.
}

void cQ_touchClause_se(clause* c){
	//This is the touch-clause method for subsumption elimination. It is called every time a clause becomes shorter or is
	//learned such that we can again check it for subsumption operations.
	int32_t oldScore = GET_CLS_Q_SCORE(c), newScore = INT32_MAX;

	//The new score for the clause is its current size. The shorter the clause, the better it is for subsuming others with it.
	newScore = c->size;
	SET_CLS_Q_SCORE(c,newScore);

	if (GET_CLS_Q_POS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the clause up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the clause now has a better score.
			cQ_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cQ_moveDown(c);
		}
	} else {
		cQ_enqueue(c);
	}
}

void cQ_touchClause_inp_se(clause* c){
	//This is the touch-clause method for subsumption elimination.
	int32_t oldScore = GET_CLS_Q_SCORE(c), newScore = INT32_MAX;

	//The new score for the clause is its current size. The shorter the clause, the better it is for subsuming others with it.
	newScore = c->size;
	SET_CLS_Q_SCORE(c,newScore);

	if (GET_CLS_Q_POS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the clause up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the clause now has a better score.
			cQ_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cQ_moveDown(c);
		}
	} else {
		cQ_enqueue(c);
	}
}

void cQ_touchClause_inp_str(clause* c){
	//This is the touch-clause method for strengthening.
	int32_t oldScore = GET_CLS_Q_SCORE(c), newScore = INT32_MAX;

	if (GET_CLS_SIZE(c) == 2 || GET_CLS_SIZE(c) > param_inpStrMaxSize) return;

	//The new score for the clause is its current size. The shorter the clause, the better it is for STR others with it.
	newScore = c->size;
	SET_CLS_Q_SCORE(c,newScore);

	if (GET_CLS_Q_POS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause has
		//changed. We store its old score, compute the new score, and if the new score is smaller than the old score, we
		//move the clause up, if the score stayed we do nothing, if the score became larger (literal loss), we move it
		//downwards.
		if (newScore < oldScore){
			//Yes, something changed and the clause now has a better score.
			cQ_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cQ_moveDown(c);
		}
	} else {
		cQ_enqueue(c);
	}
}

void cQ_touchClause_inp_treelook(clause* c){
	//To do touch a clause when doing in-processing TREELOOK - shortest clauses first.
	float_ty oldScore = GET_CLS_Q_SCORE(c), newScore = UINT32_MAX;

	//The new score for the clause is its current size. The shorter the clause, the better it is for becoming false.
	newScore = GET_CLS_SIZE(c);
	SET_CLS_Q_SCORE(c,newScore);

	if (GET_CLS_Q_POS(c) != 0){
		//The clause is already in the queue, but receives an update in its position since something for the clause changed.
		if (newScore < oldScore){
			//Yes, something changed and the clause now has a better score.
			cQ_moveUp(c);
		} else {
			//Yes, something changed and the clause now has a worse score than before.
			cQ_moveDown(c);
		}
	} else {
		cQ_enqueue(c);
	}
}

void cQ_flush(){
	//This will flush out all the elements from the priority queue.
	int32_t i;
	for (i = 1; i < cQ_leaf; ++i){
		cQ[i]->cQPos = 0;
	}
	cQ_leaf = 1;
}

//Methods for handling the clause stack.
void cS_reset(){
	//Reset the clause stack.
	cS = NULL;								//Stack does not yet exist.
	cS_touchClause = &cS_touchClause_null;	//No touch operation defined yet.
	cS_leaf = 1;								//The leaf is at first position if the stack is empty.
	cS_avail = 0;							//No memory yet available.
}

uint32_t cS_init(){
	//For initializing once f.m_initial is known.
	cS_avail = (f.m_initial + 1);
	cS = malloc(sizeof(clause*)*cS_avail); //+1 for the initial field that is never used.
	if (cS == NULL){
		printf("c ERROR. The allocation of the clause stack failed. Out of memory?\n");
		return 0;
	}
	cS_leaf = 1;			//The stack is initialized and empty. So the leaf is at position 1.
	return 1;
}

void cS_dispose(){
	//For freeing all related memory.
	if (cS != NULL){
		free(cS);
		cS = NULL;
		cS_touchClause = &cS_touchClause_null;
		cS_leaf = 1;
		cS_avail = 0;
	}
}

clause* cS_pop(){
	//To pop the topmost element in the stack.
	if (cS_leaf == 1) return NULL;

	--cS_leaf;
	SET_CLS_S_POS(cS[cS_leaf],0);
	return cS[cS_leaf];
}

clause* cS_read(){
	//To read the topmost element.
	if (cS_leaf == 1) return NULL;
	return cS[cS_leaf-1];
}

void cS_drop(){
	if (cS_leaf > 1) {
		--cS_leaf;
		SET_CLS_S_POS(cS[cS_leaf],0);
	}
}

void cS_push(clause* c){
	//To push a clause to the stack.
	if (GET_CLS_S_POS(c) > 0) return;

	cS[cS_leaf] = c;
	c->cSPos = cS_leaf++;
	if (cS_leaf == cS_avail){
		cS_avail += 8*BLOCKSIZE;
		cS = realloc(cS, sizeof(clause*)*cS_avail);
	}
}

void cS_delete(clause* c){
	//To delete a clause from the clause stack.
	int32_t pos = GET_CLS_S_POS(c);
	if (pos == 0) return;

	SET_CLS_S_POS(c, 0);

	if (pos == --cS_leaf) {
		//Since the removed clause was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last clause that is now at cSLeaf position.
	cS[pos] = cS[cS_leaf];
	cS[pos]->cSPos = pos;
}

void cS_touchClause_null(clause* c){
	//To do absolutely nothing.
}

void cS_touchClause_str(clause* c){
	//To touch a clause for the clause stack. If the clause is already in the stack, we can return.
	if (GET_CLS_S_POS(c) > 0) return;

	cS[cS_leaf] = c;
	c->cSPos = cS_leaf++;
	if (cS_leaf == cS_avail){
		cS_avail += 8*BLOCKSIZE;
		cS = realloc(cS, sizeof(clause*)*cS_avail);
	}
}

void cS_touchClause_ate(clause* c){
	//This is the touch clause method for asymmetric tautology elimination.
	//We do not enqueue clauses that are too small or those we already have.
	if (c->size < param_prepATEMin || GET_CLS_S_POS(c) > 0) return;

	cS[cS_leaf] = c;
	c->cSPos = cS_leaf++;
	if (cS_leaf == cS_avail){
		cS_avail += 8*BLOCKSIZE;
		cS = realloc(cS, sizeof(clause*)*cS_avail);
	}
}

void cS_flush(){
	//To remove all elements from the stack.
	int32_t i;
	for (i = 1; i < cS_leaf; ++i){
		cS[i]->cSPos = 0;
	}
	cS_leaf = 1;
}

void cSS0_reset(){
	//Reset the clause stack.
	cSS0 = NULL;								//Stack does not yet exist.
	cSS0_touchClause = &cSS0_touchClause_null;	//No touch operation defined yet.
	cSS0_leaf = 1;								//The leaf is at first position if the stack is empty.
	cSS0_avail = 0;								//No memory yet available.
}

uint32_t cSS0_init(){
	//For initializing once f.m_initial is known.
	cSS0_avail = (8*BLOCKSIZE);
	cSS0 = malloc(sizeof(clause*)*cSS0_avail); //+1 for the initial field that is never used.
	if (cSS0 == NULL){
		printf("c ERROR. The allocation of the clause stack failed. Out of memory?\n");
		return 0;
	}
	cSS0_leaf = 1;			//The stack is initialized and empty. So the leaf is at position 1.
	return 1;
}

void cSS0_dispose(){
	//For freeing all related memory.
	if (cSS0 != NULL){
		free(cSS0);
		cSS0 = NULL;
		cSS0_touchClause = &cSS0_touchClause_null;
		cSS0_leaf = 1;
		cSS0_avail = 0;
	}
}

clause* cSS0_pop(){
	//To pop the topmost element in the stack.
	if (cSS0_leaf == 1) return NULL;

	--cSS0_leaf;
	SET_CLS_SS0_POS(cSS0[cSS0_leaf],0);
	return cSS0[cSS1_leaf];
}

clause* cSS0_read(){
	//To read the topmost element.
	if (cSS0_leaf == 1) return NULL;
	return cSS0[cSS1_leaf-1];
}

void cSS0_drop(){
	if (cSS0_leaf > 1) {
		--cSS0_leaf;
		SET_CLS_SS0_POS(cSS0[cSS0_leaf],0);
	}
}

void cSS0_push(clause* c){
	//To push a clause to the stack.
	if (GET_CLS_SS0_POS(c) > 0) return;

	cSS0[cSS0_leaf] = c;
	c->cSS0Pos = cSS0_leaf++;
	if (cSS0_leaf == cSS0_avail){
		cSS0_avail += 8*BLOCKSIZE;
		cSS0 = realloc(cSS0, sizeof(clause*)*cSS0_avail);
	}
}

void cSS0_delete(clause* c){
	//To delete a clause from the clause stack.
	int32_t pos = GET_CLS_SS0_POS(c);
	if (pos == 0) return;

	SET_CLS_SS0_POS(c, 0);

	if (pos == --cSS0_leaf) {
		//Since the removed clause was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last clause that is now at cSLeaf position.
	cSS0[pos] = cSS0[cSS0_leaf];
	cSS0[pos]->cSS0Pos = pos;
}

void cSS0_touchClause_null(clause* c){
	//To do absolutely nothing.
}

void cSS0_touchClause_satelite(clause* c){
	//To touch a clause for the clause stack in SATELITE. If the clause is already in the stack, we can return.
	if (GET_CLS_SS0_POS(c) > 0) return;

	cSS0[cSS0_leaf] = c;
	c->cSS0Pos = cSS0_leaf++;
	if (cSS0_leaf == cSS0_avail){
		cSS0_avail += 8*BLOCKSIZE;
		cSS0 = realloc(cSS0, sizeof(clause*)*cSS0_avail);
	}
}


void cSS0_flush(){
	//To remove all elements from the stack.
	int32_t i;
	for (i = 1; i < cSS0_leaf; ++i){
		cSS0[i]->cSS0Pos = 0;
	}
	cSS0_leaf = 1;
}

void cSS1_reset(){
	//Reset the clause stack.
	cSS1 = NULL;								//Stack does not yet exist.
	cSS1_touchClause = &cSS1_touchClause_null;	//No touch operation defined yet.
	cSS1_leaf = 1;								//The leaf is at first position if the stack is empty.
	cSS1_avail = 0;								//No memory yet available.
}

uint32_t cSS1_init(){
	//For initializing once f.m_initial is known.
	cSS1_avail = (8*BLOCKSIZE);
	cSS1 = malloc(sizeof(clause*)*cSS1_avail); //+1 for the initial field that is never used.
	if (cSS1 == NULL){
		printf("c ERROR. The allocation of the clause stack failed. Out of memory?\n");
		return 0;
	}
	cSS1_leaf = 1;			//The stack is initialized and empty. So the leaf is at position 1.
	return 1;
}

void cSS1_dispose(){
	//For freeing all related memory.
	if (cSS1 != NULL){
		free(cSS1);
		cSS1 = NULL;
		cSS1_touchClause = &cSS1_touchClause_null;
		cSS1_leaf = 1;
		cSS1_avail = 0;
	}
}

clause* cSS1_pop(){
	//To pop the topmost element in the stack.
	if (cSS1_leaf == 1) return NULL;

	--cSS1_leaf;
	SET_CLS_SS1_POS(cSS1[cSS1_leaf],0);
	return cSS1[cSS1_leaf];
}

clause* cSS1_read(){
	//To read the topmost element.
	if (cSS1_leaf == 1) return NULL;
	return cSS1[cSS1_leaf-1];
}

void cSS1_drop(){
	if (cSS1_leaf > 1) {
		--cSS1_leaf;
		SET_CLS_SS1_POS(cSS1[cSS1_leaf],0);
	}
}

void cSS1_push(clause* c){
	//To push a clause to the stack.
	if (GET_CLS_SS1_POS(c) > 0) return;

	cSS1[cSS1_leaf] = c;
	c->cSS1Pos = cSS1_leaf++;
	if (cSS1_leaf == cSS1_avail){
		cSS1_avail += 8*BLOCKSIZE;
		cSS1 = realloc(cSS1, sizeof(clause*)*cSS1_avail);
	}
}

void cSS1_delete(clause* c){
	//To delete a clause from the clause stack.
	int32_t pos = GET_CLS_SS1_POS(c);
	if (pos == 0) return;

	SET_CLS_SS1_POS(c, 0);

	if (pos == --cSS1_leaf) {
		//Since the removed clause was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last clause that is now at cSLeaf position.
	cSS1[pos] = cSS1[cSS1_leaf];
	cSS1[pos]->cSS1Pos = pos;
}

void cSS1_touchClause_null(clause* c){
	//To do absolutely nothing.
}

void cSS1_touchClause_satelite(clause* c){
	//To touch a clause for the clause stack in SATELITE. If the clause is already in the stack, we can return.
	if (GET_CLS_SS1_POS(c) > 0) return;

	cSS1[cSS1_leaf] = c;
	c->cSS1Pos = cSS1_leaf++;
	if (cSS1_leaf == cSS1_avail){
		cSS1_avail += 8*BLOCKSIZE;
		cSS1 = realloc(cSS1, sizeof(clause*)*cSS1_avail);
	}
}


void cSS1_flush(){
	//To remove all elements from the stack.
	int32_t i;
	for (i = 1; i < cSS1_leaf; ++i){
		cSS1[i]->cSS1Pos = 0;
	}
	cSS1_leaf = 1;
}
