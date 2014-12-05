/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "vQueue.h"

void vQ_reset(){
	//Reset the variable queue.
	vQ_touchVariable = &vQ_touchVariable_null;	//We reset the touch variable method to do nothing.
	vQ = NULL;									//Priority queue does not yet exist.
	vQ_leaf = 1;									//The leaf is at first position if the queue is empty.
}

uint32_t vQ_init(){
	//For initializing once f.n_initial is known.
	vQ = malloc(sizeof(variable*)*(f.n_initial + 1)); //+1 for the initial field that is never used.
	if (vQ == NULL){
		printf("c ERROR. The allocation of the variable priority queue failed. Out of memory?\n");
		return 0;
	}
	vQ_leaf = 1;					//The queue is initialized and empty. So the leaf is at position 1.
	//vQ_update = &zQ_update_se;//The first update rule we use is the one for subsumption elimination.
	return 1;
}

void vQ_dispose(){
	//For freeing all related memory.
	vQ_touchVariable = &vQ_touchVariable_null;	//We reset the touch variable method to do nothing.
	if (vQ != NULL){
		free(vQ);
		vQ = NULL;
		vQ_leaf = 1;
	}
}

//Methods for handling the variable queue vQ.
void vQ_moveUp(variable* v){
	variable* parent;
	int32_t vPos, vScore, parentPos, parentScore;

	//We first get the current position of the variable and its score.
	vPos = GET_VAR_Q_POS(v);
	vScore = GET_VAR_Q_SCORE(v);

	while(1){
		//We can stop right away if the variable is at the topmost position.
		if (vPos == 1) break;

		//We move the variable upwards by exchanging it with its parent as long as the parent has more score.
		//Division by two is shift right by one.
		parentPos = vPos >> 1;
		parent = vQ[parentPos];
		parentScore = GET_VAR_Q_SCORE(parent);

		//If the parent has less score than the variable, we can return.
		if (parentScore <= vScore) break;

		//Otherwise, we must exchange the variable and its parent. Put the parent at the current variable position.
		vQ[vPos] = parent;
		SET_VAR_Q_POS(parent, vPos);

		//Then we put the variable in the position of the parent. We do not yet update the position. See the end.
		vQ[parentPos] = v;
		vPos = parentPos;
	}

	//Finally we update the variable position in the priority queue.
	SET_VAR_Q_POS(v, vPos);
}

void vQ_moveDown(variable* v){
	variable *leftChild, *rightChild;
	int32_t vPos, vScore, leftChildPos, leftScore, rightChildPos, rightScore;
	vPos = GET_VAR_Q_POS(v);
	vScore = GET_VAR_Q_SCORE(v);

	while(1){
		leftChildPos = vPos << 1;
		rightChildPos = (vPos << 1)+1;

		//No left child means no right child either. Variable already at bottom of the heap. We cannot push further down.
		if (leftChildPos >= vQ_leaf)	break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos >= vQ_leaf) {
			leftChild = vQ[leftChildPos];
			leftScore = GET_VAR_Q_SCORE(leftChild);

			//If the left child has more score, we must move down the variable to this position. Otherwise we are done.
			if (leftScore >= vScore) break;

			//We are not done yet. We first lift the child and then push down the variable we want to push down.
			vQ[vPos] = leftChild;
			SET_VAR_Q_POS(leftChild, vPos);

			//Then we push down the variable to the child position. We do not update its position yet, we do that at the end.
			vQ[leftChildPos] = v;
			vPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a better score.
			leftChild = vQ[leftChildPos];
			leftScore = GET_VAR_Q_SCORE(leftChild);

			rightChild = vQ[rightChildPos];
			rightScore = GET_VAR_Q_SCORE(rightChild);

			if (leftScore > rightScore){
				//We compare against the right child. We override the left child with it.
				leftChild = rightChild;
				leftScore = rightScore;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//We are done if the left child does not have a better score than the variable itself.
			if (leftScore >= vScore) break;

			//We are not done yet. We first lift the child and then push down the variable.
			vQ[vPos] = leftChild;
			SET_VAR_Q_POS(leftChild, vPos);

			//Then we push down the variable to the child position. We do not update its position yet, we do that at the end.
			vQ[leftChildPos] = v;
			vPos = leftChildPos;
		}
	}
	//Finally we update the variable position in the priority queue.
	SET_VAR_Q_POS(v, vPos);
}

void vQ_replaceRoot(variable* newRoot){
	//If there is no stuff at the root element, we must not remove anything.
	if (vQ_leaf > 1U) {
		//There is a root, we must override it. First, we must tell the root element, that it is not in the queue anymore.
		SET_VAR_Q_POS(vQ[1], 0);
		vQ[1] = newRoot;
		//Then we add the new root, tell it it is in the queue, but then make it move down (to keep the heap property).
		SET_VAR_Q_POS(vQ[1], 1U);
		vQ_moveDown(newRoot);
	} else {
		//There is no root. We just add the new element to the first position.
		//First we put the variable on the next priority queue leaf.
		vQ[1] = newRoot;
		//Then we update its position for being in the queue.
		SET_VAR_Q_POS(newRoot,vQ_leaf++);
	}
}


variable* vQ_dequeueBest(){
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (vQ_leaf == 1) return NULL;

	//If there is only one element left we pick it but do not shuffle around a replacement.
	if (vQ_leaf == 2){
		SET_VAR_Q_POS(vQ[1],0);
		--vQ_leaf;
		return vQ[1];
	}

	//If there is more than one element left we pick the topmost element and signal that it is not in the queue anymore.
	variable* best = vQ[1];
	SET_VAR_Q_POS(best,0);

	//We override the first position with the last element in the queue and push it down afterwards.
	vQ[1] = vQ[--vQ_leaf];
	SET_VAR_Q_POS(vQ[1],1);

	vQ_moveDown(vQ[1]);
	//The queue is again up to date and we return the best one we have dequeued.
	return best;
}

void vQ_delete(variable* v){
	variable *replacementV, *parent;
	int32_t vPos, replacementScore, parentPos, parentScore;
	//We first pick the current position of this variable in the queue.
	vPos = GET_VAR_Q_POS(v);

	//If the given variable is not in the queue we are already done.
	if (vPos == 0) return;

	//We set the element to be not in the queue anymore.
	SET_VAR_Q_POS(v,0);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (vPos == vQ_leaf-1){
		--vQ_leaf;
		return;
	}

	//We pick the replacement.
	replacementV = vQ[--vQ_leaf];

	//We override the position of the variable to be deleted by the one we picked as replacement.
	vQ[vPos] = replacementV;

	//We update the position of the replacement variable in the queue.
	SET_VAR_Q_POS(replacementV,vPos);
	replacementScore = GET_VAR_Q_SCORE(replacementV);

	//We then check if we have to move the element upwards.
	parentPos = vPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = vQ[parentPos];
		parentScore = GET_VAR_Q_SCORE(parent);

		//Check if the replacement must be moved up or down in the queue.
		if (parentScore >  replacementScore){
			//We must make the upwards move.
			vQ_moveUp(replacementV);
		} else {
			//We must make the downwards move.
			vQ_moveDown(replacementV);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		vQ_moveDown(replacementV);
	}
}

void vQ_enqueue(variable* v){
	//First we put the variable on the next priority queue leaf.
	vQ[vQ_leaf] = v;
	//Then we update its position for being in the queue.
	SET_VAR_Q_POS(v,vQ_leaf++);
	//Then we move the variable up in the queue until it is at the right position.
	vQ_moveUp(v);
}

void vQ_touchVariable_null(variable* v){
	//This method does not do anything.
}

void vQ_touchVariable_niver(variable* v){
	//Touch a variable in case non increasing variable elimination by resolution is performed.
	int32_t oldScore = GET_VAR_Q_SCORE(v), newScore = INT32_MAX,
			noccP = (main_litData + v->id)->numOccsUsed, noccN = (main_litData - v->id)->numOccsUsed;

	//We can return right away if the variable is already satisfied.
	if (! IS_VAR_UNASSIGNED(v) ) return;

	//We can ignore variables that are pure, PLE will take care of them.
	if (noccP == 0 || noccN == 0) return;

	//The score of the variable is the product of the numbers of positive and negative occurrences minus the sum of those
	//occurrences plus 1. The perfect value is zero. The idea is: on average, take what is added and subtract what is removed.
	newScore = noccP * noccN - (noccP + noccN) + 1;

	SET_VAR_Q_SCORE(v,newScore);
	if (GET_VAR_Q_POS(v) != 0){
		//The variable is already in the queue, but receives an update in its position in the queue.
		if ((noccN != 1 && noccP != 1) && (noccP > param_prepNIVERMax || noccN > param_prepNIVERMax)) {
			vQ_delete(v);
			return;
		}
		if (newScore < oldScore){
			//Yes, something changed and the variable now has a better score.
			vQ_moveUp(v);
		} else {
			//Yes, something changed and the variable now has a worse score than before.
			vQ_moveDown(v);
		}
	} else {
		//The variable is not in the queue yet.
		if ((noccN != 1 && noccP != 1) && (noccP > param_prepNIVERMax || noccN > param_prepNIVERMax)) return;
		vQ_enqueue(v);
	}
}

void vQ_touchVariable_ge(variable* v){
	//Touch a variable in case gate extraction is performed.
	int32_t oldScore = GET_VAR_Q_SCORE(v), newScore = INT32_MAX;

	//We can return right away if the variable is already satisfied.
	if (! IS_VAR_UNASSIGNED(v) ) return;

	//The score of the variable is the given by the number of implications it has. The more, the better.
	newScore = 2*f.n_initial - (main_litData + v->id)->numImpUsed - (main_litData - v->id)->numImpUsed;

	SET_VAR_Q_SCORE(v,newScore);
	if (GET_VAR_Q_POS(v) != 0){
		//The variable is already in the queue, but receives an update in its position in the queue.
		if (newScore < oldScore){
			//Yes, something changed and the variable now has a better score.
			vQ_moveUp(v);
		} else {
			//Yes, something changed and the variable now has a worse score than before.
			vQ_moveDown(v);
		}
	} else {
		//The variable is not in the queue yet.
		vQ_enqueue(v);
	}
}

void vQ_touchVariable_inp_ge(variable* v){
	//Touch a variable in case gate extraction is performed.
	int32_t oldScore = GET_VAR_Q_SCORE(v), newScore = INT32_MAX;

	//We can return right away if the variable is already satisfied.
	if (! IS_VAR_UNASSIGNED(v) ) return;

	//The score of the variable is the given by the number of implications it has. The more, the better.
	newScore = 2*f.n_initial - (main_litData + v->id)->numImpUsed - (main_litData - v->id)->numImpUsed;

	SET_VAR_Q_SCORE(v,newScore);
	if (GET_VAR_Q_POS(v) != 0){
		//The variable is already in the queue, but receives an update in its position in the queue.
		if (newScore < oldScore){
			//Yes, something changed and the variable now has a better score.
			vQ_moveUp(v);
		} else {
			//Yes, something changed and the variable now has a worse score than before.
			vQ_moveDown(v);
		}
	} else {
		//The variable is not in the queue yet.
		vQ_enqueue(v);
	}
}

void vQ_flush(){
	//This will flush out all the elements from the priority queue.
	int32_t i;
	for (i = 1; i < vQ_leaf; ++i){
		vQ[i]->vQPos = 0;
	}
	vQ_leaf = 1;
}

//Methods for handling the variable stack.
void vS_reset(){
	//Reset the variable stack.
	vS = NULL;									//Stack does not yet exist.
	vS_touchVariable = &vS_touchVariable_null;	//No touching operation defined yet.
	vS_leaf = 1;									//The leaf is at first position if the stack is empty.
}

uint32_t vS_init(){
	//For initializing once f.n_initial is known.
	vS = malloc(sizeof(clause*)*(f.n_initial + 1)); //+1 for the initial field that is never used.
	if (vS == NULL){
		printf("c ERROR. The allocation of the variable stack failed. Out of memory?\n");
		return 0;
	}
	vS_leaf = 1;			//The stack is initialized and empty. So the leaf is at position 1.
	return 1;
}

void vS_dispose(){
	//For freeing all related memory.
	if (vS != NULL){
		free(vS);
		vS = NULL;
		vS_touchVariable = &vS_touchVariable_null;
		vS_leaf = 1;
	}
}

variable* vS_pop(){
	//To pop the topmost element in the stack.
	if (vS_leaf == 1) return NULL;

	--vS_leaf;
	SET_VAR_S_POS(vS[vS_leaf],0);
	return vS[vS_leaf];
}

void vS_delete(variable* v){
	//To delete a variable from the variable stack.
	int32_t pos = GET_VAR_S_POS(v);
	if (pos == 0) return;

	SET_VAR_S_POS(v, 0);

	if (pos == --vS_leaf) {
		//Since the removed variable was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last variable that is now at vSLeaf position.
	vS[pos] = vS[vS_leaf];
	vS[pos]->vSPos = pos;
}

void vS_touchVariable_null(variable* v){
	//To do absolutely nothing.
}

void vS_touchVariable_pl(variable* v){
	//To touch a variable for the variable stack.
	//If the variable is already in the stack, we can return.
	if (GET_VAR_S_POS(v) > 0U) return;
	//We can return right away if the variable is already assigned.
	if (! IS_VAR_UNASSIGNED(v) ) return;
	//We can return if the variable is not pure in either direction.
	vS[vS_leaf] = v;
	v->vSPos = vS_leaf++;
}

void vS_touchVariable_inp_pl(variable* v){
	//To touch a variable for the variable stack.
	//If the variable is already in the stack, we can return.
	if (GET_VAR_S_POS(v) > 0U) return;
	//We can return right away if the variable is already assigned.
	if (! IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)) return;
	//We can return if the variable is not pure in either direction.
	vS[vS_leaf] = v;
	v->vSPos = vS_leaf++;
}

void vS_touchVariable_inp_ternary(variable* v){
	//To touch a variable for the variable stack.
	//If the variable is already in the stack, we can return.
	if (GET_VAR_S_POS(v) > 0U) return;
	//We can return right away if the variable is already assigned.
	if (! IS_VAR_UNASSIGNED(v) || IS_VAR_DISABLED(v)) return;
	//We can return if the variable is not pure in either direction.
	vS[vS_leaf] = v;
	v->vSPos = vS_leaf++;
}

void vS_push(variable* v){
	//To just push a variable to the stack if it is not already in there.
	if (GET_VAR_S_POS(v) == 0){
		vS[vS_leaf] = v;
		SET_VAR_S_POS(v, vS_leaf);
		vS_leaf++;
	}
}

void vS_flush(){
	//To remove all elements from the stack.
	int32_t i;
	for (i = 1; i < vS_leaf; ++i){
		vS[i]->vSPos = 0;
	}
	vS_leaf = 1;
}
