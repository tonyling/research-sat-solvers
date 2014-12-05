/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../cdcl.h"

//Methods for handling the CDCL variable priority queue.
void cdcl_variables_Q_moveUp(cdclVariable* v){
	//To move an element up in the CDCL variable priority queue.
	cdclVariable* parent;
	uint32_t vPos, parentPos;
	float_ty vActivity, parentActivity;

	//We first get the current position of the variable and its activity.
	vPos = GET_CDCL_VAR_PTR_OFFSET(v);
	vActivity = GET_CDCL_VAR_PTR_ACTIVITY(v);

	while(1){
		//We can stop right away if the variable is at the topmost position.
		if (vPos == 1) break;

		//We move the variable upwards by exchanging it with its parent as long as the parent has less activity.
		//Division by two is shift right by one.
		parentPos = vPos >> 1;
		parent = cdcl_vars_q[parentPos];
		parentActivity = GET_CDCL_VAR_PTR_ACTIVITY(parent);

		//If the parent has more activity than the variable, we can return.
		if (parentActivity >= vActivity) break;

		//Otherwise, we must exchange the variable and its parent. Put the parent at the current variable position.
		cdcl_vars_q[vPos] = parent;
		SET_CDCL_VAR_PTR_OFFSET(parent, vPos);

		//Then we put the variable in the position of the parent. We do not yet update the position. See the end.
		cdcl_vars_q[parentPos] = v;
		vPos = parentPos;
	}

	//Finally we update the variable position in the priority queue.
	SET_CDCL_VAR_PTR_OFFSET(v, vPos);
}
void cdcl_variables_Q_moveDown(cdclVariable* v){
	//To move an element down in the CDCL variable priority queue.
	cdclVariable *leftChild, *rightChild;
	uint32_t vPos, leftChildPos, rightChildPos;
	float_ty vActivity, rightActivity, leftActivity;
	vPos = GET_CDCL_VAR_PTR_OFFSET(v);
	vActivity = GET_CDCL_VAR_PTR_ACTIVITY(v);

	while(1){
		leftChildPos = vPos << 1;
		rightChildPos = (vPos << 1)+1U;

		//No left child means no right child either. Variable already at bottom of the heap. We cannot push further down.
		if (leftChildPos > cdcl_vars_e_used) break;

		//No right child means we have to check for a down move with the lift child only.
		if (rightChildPos > cdcl_vars_e_used) {
			leftChild = cdcl_vars_q[leftChildPos];
			leftActivity = GET_CDCL_VAR_PTR_ACTIVITY(leftChild);

			//If the left child has more activity, we must move down the variable to this position. Otherwise we are done.
			if (leftActivity <= vActivity) break;

			//We are not done yet. We first lift the child and then push down the variable.
			cdcl_vars_q[vPos] = leftChild;
			SET_CDCL_VAR_PTR_OFFSET(leftChild, vPos);

			//Then we push down the variable to the child position. We do not update its position yet, we do that at the end.
			cdcl_vars_q[leftChildPos] = v;
			vPos = leftChildPos;
		} else {
			//We have both a left and a right child and must first determine which one has a higher activity.
			leftChild = cdcl_vars_q[leftChildPos];
			leftActivity = GET_CDCL_VAR_PTR_ACTIVITY(leftChild);

			rightChild = cdcl_vars_q[rightChildPos];
			rightActivity = GET_CDCL_VAR_PTR_ACTIVITY(rightChild);

			if (leftActivity < rightActivity){
				//We compare against the right child, since it has a higher activity. We override the left child with it.
				leftChild = rightChild;
				leftActivity = rightActivity;
				leftChildPos = rightChildPos;
			}//Otherwise we leave it as it is and compare against the original left child. Do not get confused here, below,
			//leftChild could be the right child. We just use the name here to spare an additional else statement.

			//If the left child has more activity, we must move down the variable to this position. Otherwise we are done.
			if (leftActivity <= vActivity) break;

			//We are not done yet. We first lift the child and then push down the variable.
			cdcl_vars_q[vPos] = leftChild;
			SET_CDCL_VAR_PTR_OFFSET(leftChild, vPos);

			//Then we push down the variable to the child position. We do not update its position yet, we do that at the end.
			cdcl_vars_q[leftChildPos] = v;
			vPos = leftChildPos;
		}
	}
	//Finally we update the variable position in the priority queue.
	SET_CDCL_VAR_PTR_OFFSET(v, vPos);
}

cdclVariable* cdcl_variables_Q_readBest(){
	//To return the topmost element without deleting it from the queue.
	//If there is no more stuff available in the priority queue we return the terminating symbol.
	if (cdcl_vars_e_used == 0U) return NULL;

	//Otherwise we return the element at the top.
	return cdcl_vars_q[1];
}

void cdcl_variables_Q_delete(cdclVariable* v){
	//To delete an element from the CDCL variable priority queue.
	cdclVariable *replacementV, *parent;
	uint32_t vPos, parentPos;
	float_ty replacementActivity, parentActivity;
	//We first pick the current position of this variable in the queue.
	vPos = GET_CDCL_VAR_PTR_OFFSET(v);

	if (vPos == 0) return;

	//We signal that the element is not in the queue anymore.
	SET_CDCL_VAR_PTR_OFFSET(v, 0U);

	//If c is the element at the leaf of the tree then we do not have to do anything but move the leaf.
	if (vPos == cdcl_vars_e_used){
		--cdcl_vars_e_used;
		return;
	}

	//We pick the replacement.
	replacementV = cdcl_vars_q[cdcl_vars_e_used--];

	//We override the position of the variable to be deleted by the one we picked as replacement.
	cdcl_vars_q[vPos] = replacementV;

	//We update the position of the replacement variable in the queue.
	SET_CDCL_VAR_PTR_OFFSET(replacementV,vPos);
	replacementActivity = GET_CDCL_VAR_PTR_ACTIVITY(replacementV);

	//We then check if we have to move the element upwards.
	parentPos = vPos >> 1;
	//We check if the element has a parent.
	if (parentPos > 0){
		//The element has a parent.
		parent = cdcl_vars_q[parentPos];
		parentActivity = GET_CDCL_VAR_PTR_ACTIVITY(parent);

		//Check if the replacement must be moved up or down in the queue. A larger activity is better.
		if (parentActivity <  replacementActivity){
			//We must make the upwards move.
			cdcl_variables_Q_moveUp(replacementV);
		} else {
			//We must make the downwards move.
			cdcl_variables_Q_moveDown(replacementV);
		}
	} else {
		//The element has no parent. We must make the downwards move.
		cdcl_variables_Q_moveDown(replacementV);
	}
}
void cdcl_variables_Q_enqueue(cdclVariable* v){
	//To enqueue an element in the CDCL variable priority queue. This MUST ONLY be called on variables that are not already
	//in the queue.
	//First we put the variable on the next priority queue leaf.
	cdcl_vars_q[++cdcl_vars_e_used] = v;
	//Then we update its position for being in the queue.
	SET_CDCL_VAR_PTR_OFFSET(v,cdcl_vars_e_used);
	//Then we move the variable up in the queue until it is at the right position.
	cdcl_variables_Q_moveUp(v);
}
void cdcl_variables_Q_flush(){
	//To remove all elements in the CDCL variable queue. This method just resets the counter for the enabled variables.
	cdcl_vars_e_used = 0;
}

//Methods for handling the variable stack.
void cdcl_variables_S_delete(cdclVariable* v){
	//To delete a variable from the variable stack.
	int32_t pos = GET_CDCL_VAR_PTR_SPOS(v);
	if (pos == 0) return;

	SET_CDCL_VAR_PTR_SPOS(v, 0);

	if (pos == --cdcl_variables_S_leaf) {
		//Since the removed variable was the last in the stack, we do not have to shuffle anything around.
		return;
	}

	//We must shuffle around a replacement to the position. We take the last variable that is now at vSLeaf position.
	cdcl_variables_S[pos] = cdcl_variables_S[cdcl_variables_S_leaf];
	SET_CDCL_VAR_PTR_SPOS(cdcl_variables_S[pos], pos);
}

void cdcl_variables_S_touchVariable_null(cdclVariable* v){
	//To do nothing when touching a variable.
}

void cdcl_variables_S_touchVariable_inprocessing_pl(cdclVariable* v){
	//To touch a variable for the variable stack.
	//If the variable is already in the stack, we can return.
	if (GET_CDCL_VAR_PTR_SPOS(v) > 0U) return;
	//We can return right away if the variable is already assigned.
	if (! IS_CDCL_VAR_PTR_UNASSIGNED(v) ) return;
	//We can return if the variable is not pure in either direction.
	cdcl_variables_S[cdcl_variables_S_leaf] = v;
	SET_CDCL_VAR_PTR_SPOS(v, cdcl_variables_S_leaf++);
}

cdclVariable* cdcl_variables_S_pop(){
	//To pop the topmost element from the stack.
	if (cdcl_variables_S_leaf == 1) return NULL;

	--cdcl_variables_S_leaf;
	SET_VAR_S_POS(cdcl_variables_S[cdcl_variables_S_leaf],0);
	return cdcl_variables_S[cdcl_variables_S_leaf];
}

void cdcl_variables_S_flush(){
	//To remove all elements from the variable stack.
	int32_t i;
	for (i = 1; i < cdcl_variables_S_leaf; ++i){
		SET_CDCL_VAR_PTR_SPOS(cdcl_variables_S[i],0);
	}
	cdcl_variables_S_leaf = 1;
}

void cdcl_variables_prepareCall(){
	//This method creates the snapshot of the variables in the current call to the CDCL module.
	uint32_t i;
	cdclVariable *cdclV;

	//Then, we clear the variable stack and queue.
	cdcl_variables_S_flush();
	cdcl_variables_S_touchVariable = &cdcl_variables_S_touchVariable_null;
	cdcl_variables_Q_flush();

	//Then, we copy all the variables still enabled in the global formula, and add their pointers to the CDCL variable queue.
	for (i = 0; i < f.n_vars_e_used; ++i){
		cdclV = cdcl_variables + f.vars_e[i]->id;
		SET_CDCL_VAR_PTR_STAMP(cdclV, 0U);
		SET_CDCL_VAR_PTR_BASEDATA(cdclV, 0U);
		SET_CDCL_VAR_PTR_ACTIVITY(cdclV, GET_VAR_ACTIVITY(f.vars_e[i]));
		SET_CDCL_VAR_PTR_PHASE(cdclV, GET_VAR_PHASE(f.vars_e[i]));
		SET_CDCL_VAR_PTR_SPOS(cdclV, 0);
		SET_CDCL_VAR_PTR_ISEQUIVTO(cdclV,0);
		if (!IS_VAR_UNASSIGNED(f.vars_e[i])) continue;

		//Add the variable to the enabled list -- the variable priority queue (this implicitly makes the variable unassigned).
		cdcl_variables_Q_enqueue(cdclV);
	}

	for (i = 0; i < f.n_vars_d_used; ++i){
		cdclV = cdcl_variables + f.vars_d[i]->id;
		SET_CDCL_VAR_PTR_STAMP(cdclV, 0U);
		SET_CDCL_VAR_PTR_BASEDATA(cdclV, 0U);
		SET_CDCL_VAR_PTR_ACTIVITY(cdclV, ZERO);
		SET_CDCL_VAR_PTR_PHASE(cdclV, 0);
		SET_CDCL_VAR_PTR_SPOS(cdclV, 0);
		SET_CDCL_VAR_PTR_ISEQUIVTO(cdclV,0);
	}
}

void cdcl_variables_flipVarWithEquivs(cdclVariable *v){
	//Flips the variable v along with all the equivalent variables.
	uint32_t numToFlip, i;
	cdclVariable **listToFlip;
	//First, we grab the equivalence class of v that must now be completely flipped.
	if (cdcl_eqVars[GET_CDCL_VAR_PTR_ID(v)].representedBy == NULL){
		//The variable represents its own equivalence class. We flip all the variables in this class.
		numToFlip = cdcl_eqVars[GET_CDCL_VAR_PTR_ID(v)].numRepresents;
		listToFlip = cdcl_eqVars[GET_CDCL_VAR_PTR_ID(v)].equiv;
	} else {
		//The variable is part of a different equivalence class. We pick this one instead for flipping.
		numToFlip = cdcl_eqVars[GET_CDCL_VAR_PTR_ID(v)].representedBy->numRepresents;
		listToFlip = cdcl_eqVars[GET_CDCL_VAR_PTR_ID(v)].representedBy->equiv;
	}

	//Flip all the literals in the class.
	for (i = 0; i < numToFlip; ++i){
		SET_CDCL_VAR_PTR_ASS_FLIP(listToFlip[i]);
		#ifdef VERBOSE_CDCL
		printf("%d ", IS_CDCL_VAR_PTR_TRUE(listToFlip[i]) ? GET_CDCL_VAR_PTR_ID(listToFlip[i]) : -GET_CDCL_VAR_PTR_ID(listToFlip[i]));
		#endif
	}
}

cdclVariable* cdcl_variables_getRootRepresentative(cdclVariable *v){
	if (GET_CDCL_VAR_PTR_ISEQUIVTO(v) == 0) return v;
	return cdcl_variables_getRootRepresentative(cdcl_variables + abs(GET_CDCL_VAR_PTR_ISEQUIVTO(v)));
}

void cdcl_variables_assignEquivVars_assign(cdclVariable *v){
	//This method tries to find the equivalence ancestor for v. If this variable is unassigned and has no ancestor
	//it will stay unassigned.
	cdclVariable *anc;
	int32_t ancID = GET_CDCL_VAR_PTR_ISEQUIVTO(v);

	//Check if the variable is already assigned or if it has an equivalence ancestor.
	if (!IS_CDCL_VAR_PTR_UNASSIGNED(v) || ancID == 0) return;

	//The variable is unassigned and has an equivalence ancestor. We ensure that the ancestor is assigned.
	anc = cdcl_variables + abs(ancID);
	cdcl_variables_assignEquivVars_assign(anc);
	if (IS_CDCL_VAR_PTR_UNASSIGNED(anc)) return;
	//The ancestor of variable v has now been assigned to its correct value. We will now assign v. If the ancID is negative,
	//we will assign it to the opposite assignment, otherwise we will assign it to the same assignment.
	if (ancID > 0){
		//Variable v must have the same assignment as its ancestor.
		if (IS_CDCL_VAR_PTR_TRUE(anc)){
			SET_CDCL_VAR_PTR_ASS_1(v);
		} else {
			SET_CDCL_VAR_PTR_ASS_0(v);
		}
	} else {
		//Variable v must have the opposite assignment as its ancestor.
		if (IS_CDCL_VAR_PTR_TRUE(anc)){
			SET_CDCL_VAR_PTR_ASS_0(v);
		} else {
			SET_CDCL_VAR_PTR_ASS_1(v);
		}
	}
}

void cdcl_variables_assignRemainingVars(){
	//This method ensures that all variables are assigned correctly.
	cdclVariable *v;
	uint32_t i;

	#ifdef VERBOSE_CDCL
	printf("c     CDCL: Assigning remaining non-equivalent variables: ");
	#endif

	//In this sweep, we identify variables that are unassigned and have no equivalence ancestor. We assign them to zero.
	for (i = 1; i < f.n_initial+1U; ++i){
		v = cdcl_variables + i;
		if (!IS_VAR_UNASSIGNED((main_varData + i)) || IS_VAR_DISABLED((main_varData + i))) continue;
		//Ignore variables that are assigned or have an equivalence ancestor.
		if (!IS_CDCL_VAR_PTR_UNASSIGNED(v) || GET_CDCL_VAR_PTR_ISEQUIVTO(v) != 0U) continue;
		//Otherwise, assign the variable to zero.
		SET_CDCL_VAR_PTR_ASS_0(v);
		#ifdef VERBOSE_CDCL
		printf("%d ", -GET_CDCL_VAR_PTR_ID(v));
		#endif
	}

	#ifdef VERBOSE_CDCL
	printf("done.\nc     CDCL: Assigning remaining equivalent variables: ");
	#endif

	//In this sweep, we look for the unassigned variables that do have an ancestor. We assign them correctly.
	for (i = 1; i < f.n_initial+1U; ++i){
		v = cdcl_variables + i;
		if (IS_VAR_DISABLED((main_varData + i))) continue;
		//Ignore assigned variables.
		if (!IS_CDCL_VAR_PTR_UNASSIGNED(v)) continue;
		//If the variable is unassigned, then it must have an equivalence ancestor. We will assign all the equivalent
		//variables recursively.
		cdcl_variables_assignEquivVars_assign(v);
		#ifdef VERBOSE_CDCL
		printf("%d[%d] ", IS_CDCL_VAR_PTR_TRUE(v) ? GET_CDCL_VAR_PTR_ID(v) : -GET_CDCL_VAR_PTR_ID(v), GET_CDCL_VAR_PTR_ISEQUIVTO(v));
		#endif
	}

	#ifdef VERBOSE_CDCL
	printf("done.\nc     CDCL: A complete assignment has been created.\n");
	#endif
}

void cdcl_variables_reset(){
	cdcl_variables = NULL;

	cdcl_vars_q = NULL;
	cdcl_vars_e = NULL;
	cdcl_vars_e_used = 0;

	cdcl_variables_S = NULL;
	cdcl_variables_S_leaf = 0;

	cdcl_eqVars = NULL;
}

void cdcl_variables_init(){
	uint32_t i;
	cdclVariable *v;

	cdcl_variables = malloc(sizeof(cdclVariable)*(f.n_initial+1U));
	if (cdcl_variables == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the variables. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 0; i < f.n_initial+1U; ++i){
		cdcl_variables[i].id = i;
		cdcl_variables[i].baseData = 0U;
		cdcl_variables[i].stamp = 0U;
		cdcl_variables[i].reason = NULL;
		cdcl_variables[i].decLevel = 0;
		cdcl_variables[i].isEquivTo = 0;
		cdcl_variables[i].activity = ZERO;
		cdcl_variables[i].agility = ZERO;
		cdcl_variables[i].vSPos = 0;
	}

	//The cdcl_vars_q array will later be used as a heap array for the variable priority queue.
	cdcl_vars_q = malloc(sizeof(cdclVariable*)*(f.n_initial+2U));
	if (cdcl_vars_q == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the enabled variables. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_vars_e = cdcl_vars_q + 1;
	cdcl_vars_e_used = 0;

	cdcl_variables_S = malloc(sizeof(cdclVariable*)*(f.n_initial+2U));
	if (cdcl_variables_S == NULL){
		printf("c ERROR. CDCL was unable to allocate memory for the variables stack. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	cdcl_variables_S_leaf = 1;

	cdcl_eqVars = malloc(sizeof(cdclEqVar)*(f.n_initial+1));
	if (cdcl_eqVars == NULL){
		printf("c ERROR. The cdcl_eqVars array could not be initialized by CDCL. Out of memory?\n");
		cdcl_returnCode = CDCL_ERROR;
		#ifdef COLLECTSTATS
		++stats_cdcl_return_ERROR;
		#endif
		return;
	}
	for (i = 1; i < f.n_initial+1; ++i){
		v = cdcl_variables + i;
		cdcl_eqVars[i].representedBy = NULL;
		cdcl_eqVars[i].numRepresents = 1;
		cdcl_eqVars[i].equiv = malloc(sizeof(cdclVariable*)*1);
		cdcl_eqVars[i].equiv[0] = v;
	}
}

void cdcl_variables_dispose(){
	uint32_t i;

	if (cdcl_variables != NULL){
		free(cdcl_variables);
		cdcl_variables = NULL;
	}

	if (cdcl_vars_q != NULL){
		free(cdcl_vars_q);
		cdcl_vars_q = NULL;
		cdcl_vars_e = NULL;
	}
	cdcl_vars_e_used = 0;

	if (cdcl_variables_S != NULL){
		free(cdcl_variables_S);
		cdcl_variables_S = NULL;
	}
	cdcl_variables_S_leaf = 0;

	if (cdcl_eqVars != NULL) {
		for (i = 1; i < f.n_initial+1; ++i){
			if (cdcl_eqVars[i].equiv != NULL){
				free(cdcl_eqVars[i].equiv);
				cdcl_eqVars[i].equiv = NULL;
			}
		}
		free(cdcl_eqVars);
		cdcl_eqVars = NULL;
	}
}

#ifdef VERBOSE_CDCL
void cdcl_variables_printQueue_var(cdclVariable* v, uint32_t pos){
	uint32_t i, nextPos, numSpace = 0;
	if (pos > 1){
		if (pos % 2U == 1){
			numSpace = log2(pos - 1);
		} else {
			numSpace = log2(pos);
		}
	}
	printf("c     CDCL: ");
	for(i = 0; i < numSpace; ++i){
		printf("  ");
	}
	//Print the current element of the queue.
	printf("%6d[%u](%f)\n", GET_CDCL_VAR_PTR_ID(v), GET_CDCL_VAR_PTR_OFFSET(v), GET_CDCL_VAR_PTR_ACTIVITY(v));
	//If available, print the left child.
	nextPos = 2*pos;
	if (nextPos <= cdcl_vars_e_used){
		v = cdcl_vars_q[nextPos];
		cdcl_variables_printQueue_var(v, nextPos);
	}
	//If available, print the right child.
	nextPos = 2*pos + 1;
	if (nextPos <= cdcl_vars_e_used){
		v = cdcl_vars_q[nextPos];
		cdcl_variables_printQueue_var(v, nextPos);
	}
}

void cdcl_variables_printQueue(){
	printf("c     CDCL: VARIABLE PRIORITY QUEUE:\n");
	printf("c     CDCL:   Linear queue array: ");
	int32_t i;
	cdclVariable *v;
	for (i = 0; i < cdcl_vars_e_used; ++i){
		v = cdcl_vars_q[i+1];
		printf("%d[%u](%f) ", GET_CDCL_VAR_PTR_ID(v), GET_CDCL_VAR_PTR_OFFSET(v), GET_CDCL_VAR_PTR_ACTIVITY(v));
	}
	printf("\nc     CDCL:  Queue tree: \n");
	if (cdcl_vars_e_used > 0) {
		v = cdcl_vars_q[1];
		cdcl_variables_printQueue_var(v, 1);
	}
}

void cdcl_variables_printEnabled(){
	uint32_t i;
	printf("c     CDCL:   Enabled variables (%u):\n", cdcl_vars_e_used);
	for (i=0; i < cdcl_vars_e_used; ++i){
		cdcl_variables_printVariable(cdcl_vars_e[i]);
	}
}

void cdcl_variables_printAssignments(){
	uint32_t i;
	cdclVariable *v;
	printf("c     CDCL:   Variable Assignments: ");
	for (i = 1; i < f.n_initial; ++i){
		//We only check on variables that are still enabled in the global formula.
		if (!IS_VAR_DISABLED((main_varData + i))){
			v = cdcl_variables + i;
			printf("%d|",GET_CDCL_VAR_PTR_ID(v));
			if (IS_CDCL_VAR_PTR_UNASSIGNED(v)){
				printf("2 ");
			} else if (IS_CDCL_VAR_PTR_TRUE(v)){
				printf("1 ");
			} else if (IS_CDCL_VAR_PTR_FALSE(v)){
				printf("0 ");
			} else {
				printf("? ");
			}
		}
	}
	printf("\n");
}

void cdcl_variables_printVariable(cdclVariable *v){
	printf("c     CDCL:   VAR: %6d REASON: %p DECLEVEL: %u ISEQUIVTO: %d BASE: ", GET_CDCL_VAR_PTR_ID(v), (void*)GET_CDCL_VAR_PTR_REASON(v),
			GET_CDCL_VAR_PTR_DECLEVEL(v), GET_CDCL_VAR_PTR_ISEQUIVTO(v));
	PRINT_BITS_UINT32(GET_CDCL_VAR_PTR_BASEDATA(v));
	printf(" ASS: %1u OFFSET: %6u ACT: %6.6f\n",
			GET_CDCL_VAR_PTR_ASS_UINT(v),GET_CDCL_VAR_PTR_OFFSET(v),GET_CDCL_VAR_PTR_ACTIVITY(v));
}

void cdcl_variables_printAll(){
	uint32_t i;
	printf("c     CDCL:   VARIABLE DATA:\n");
	for(i = 0; i < cdcl_vars_e_used; ++i){
		cdcl_variables_printVariable(cdcl_vars_e[i]);
	}
	printf("c     CDCL:\n");
}

uint32_t cdcl_variables_verifyQueueChildren(cdclVariable *v){
	uint32_t leftPos = 2U*GET_CDCL_VAR_PTR_OFFSET(v), rightPos = 2U*GET_CDCL_VAR_PTR_OFFSET(v)+1U;

	if (!IS_VAR_UNASSIGNED((main_varData + GET_CDCL_VAR_PTR_ID(v)))){
		printf("\nA variable in the CDCL queue is already assigned in the outer formula.\n");
		cdcl_variables_printVariable(v);
		return 0;
	}

	if (IS_VAR_DISABLED((main_varData + GET_CDCL_VAR_PTR_ID(v)))){
		printf("\nA variable in the CDCL queue is already disabled in the outer formula.\n");
		cdcl_variables_printVariable(v);
		return 0;
	}

	if (GET_VAR_ISEQUIVTO((main_varData + GET_CDCL_VAR_PTR_ID(v))) != 0){
		printf("\nA variable in the CDCL queue has an equivalence in the outer formula.\n");
		cdcl_variables_printVariable(v);
		return 0;
	}

	if (leftPos <= cdcl_vars_e_used){
		if (GET_CDCL_VAR_PTR_ACTIVITY(cdcl_vars_q[leftPos]) > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			printf("\nThe first variable is child of the second but has a larger activity.\n");
			cdcl_variables_printVariable(cdcl_vars_q[leftPos]);
			cdcl_variables_printVariable(v);
			return 0;
		}

		if (!cdcl_variables_verifyQueueChildren(cdcl_vars_q[leftPos])){
			return 0;
		}
	}
	if (rightPos <= cdcl_vars_e_used){
		if (GET_CDCL_VAR_PTR_ACTIVITY(cdcl_vars_q[rightPos]) > GET_CDCL_VAR_PTR_ACTIVITY(v)){
			printf("\nThe first variable is child of the second but has a larger activity.\n");
			cdcl_variables_printVariable(cdcl_vars_q[rightPos]);
			cdcl_variables_printVariable(v);
			return 0;
		}

		if (!cdcl_variables_verifyQueueChildren(cdcl_vars_q[rightPos])){
			return 0;
		}
	}

	return 1;
}

uint32_t cdcl_variables_verifyQueue(){
	//This method checks that the variable queue is in order. That is, it must be a correct heap.
	cdclVariable *v;
	if (cdcl_vars_e_used == 0) return 1;
	v = cdcl_vars_q[1];

	if (!cdcl_variables_verifyQueueChildren(v)){
		return 0;
	}
	return 1;
}
#endif
