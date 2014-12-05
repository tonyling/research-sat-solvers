/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "complexUP.h"

clause* main_complexup_propagate(uint32_t disableAssigned){
	literal* l;  				//The current literal to propagate.
	variable* v; 				//The corresponding variable.
	clause* c;	 				//The clause we currently work on when updating the watchers.
	int32_t i,lid, lit, litPos;	//The integer representation of the literal and a temporary literal and a literal position.
	int32_t offset; 			//The offset in the watcher list for the clause we currently work on.
	//This method will perform unit propagation from the current position in the decision stack. We assume that the UP is
	//initialized correctly, that is the main_complexUP_current pointer points to the position in the decision stack that we
	//want to continue a unit propagation from. Additionally, we assume that the main_complexUP_end pointer points to the next
	//position in the decision stack where we want to append new elements at. We also assume that the main_up_stamp is fresh.

	//We propagate as long as we still have elements left.
	while (main_complexUP_current < main_complexUP_end){
		//We grab the next literal for which we want to assign the variable.
		l = *(main_complexUP_current++);
		lid = GET_LIT_NUM_FROM_PTR(l);
		//We assign the corresponding variable.
		if (lid < 0){
			//The literal is negative. The variable must be assigned to 0.
			v = main_varData - lid;
			SET_VAR_ASS_0(v);
		} else {
			//the literal is positive. The variable must be assigned to 1.
			v = main_varData + lid;
			SET_VAR_ASS_1(v);
		}

		if (disableAssigned && !IS_VAR_DISABLED(v)) variables_disable(v);

		//Walk through the implication list of l. All the implications must be put on the stack. If any of them fails,
		//we can return with a conflict.
		for (i = 0; i < GET_LIT_NUMIMP_USED(l); i+=2){
			lit = GET_LIT_IMPNUM(l, i);
			v = main_varData + abs(lit);
			if (IS_VAR_UNASSIGNED(v)){
				if (MAIN_COMPLEXUP_IS_OPPLIT_ON_S(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					return GET_LIT_IMPNUM_REASON(l, i);
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					MAIN_COMPLEXUP_ADD_IMP(lit , v, GET_LIT_IMPNUM_REASON(l, i));
				} //Else, the literal is already enqueued.
			}
		}

		//We must now walk through the watcher lists of the literal that becomes false, that is, -l is checked.
		l = main_litData - lid;
		//Walk through the watcher lists of l (the literal that becomes false). Find replacements for WL1 and WL2. If any
		//of the clauses becomes unit we must put the other watcher on the MAIN UP as it is the last literal that can make
		//the clause. If this fails, we can return with a conflict.

		//WL1
		offset = 0;
		while (GET_LIT_NUMW1_USED(l) > offset){
			c = GET_LIT_W1_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit)){
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
					//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = main_varData + abs(lit);
					MAIN_COMPLEXUP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}

		//WL2
		offset = 0;
		while (GET_LIT_NUMW2_USED(l) > offset){
			c = GET_LIT_W2_WATCHED_CLS(l, offset);
			//Before we start fiddling around with the clause, we check its representative literal.
			lit = GET_CLS_REPLIT(c);
			if (IS_LIT_SAT(lit)){
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
					return c;
				} else if (!MAIN_COMPLEXUP_IS_LIT_ON_S(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = main_varData + abs(lit);
					MAIN_COMPLEXUP_ADD_IMP(lit, v, c);
				} //Else, the literal is already on the stack.
			}
		}
	}

	//In case no conflict occurred, we can terminate the current decision level and return NULL (no conflict clause).
	MAIN_COMPLEXUP_TERMINATE_CURRENT();
	return NULL;
}

void main_complexup_reset(){
	main_complexUP_current = NULL;
	main_complexUP_end = NULL;
}

uint32_t main_complexup_init(){
	MAIN_COMPLEXUP_SET_DECLEVEL(main_decLevel);
	return 1;
}

void main_complexup_dispose(){
	main_complexUP_current = NULL;
	main_complexUP_end = NULL;
}

#ifdef VERBOSE
void main_complexup_printCurrentLevel(){
	literal *l;
	literal **upPtr = main_complexUP_current;
	printf("c CURRENT UP (DL%u, %p %p): ", main_decLevel, (void*)main_complexUP_current, (void*)main_complexUP_end);
	while ((l = *upPtr) != NULL && upPtr < main_complexUP_end){
		printf("%d ", GET_LIT_NUM_FROM_PTR(l));
		++upPtr;
	}
	printf("NULL\n");
}

void main_complexup_printFromLevel(uint32_t theLevel){
	uint32_t dl=theLevel;
	literal *l;
	literal **upPtr, **upPtrEnd;

	while (dl <= main_decLevel){
		upPtr = main_decStackPtrs[dl];
		if (dl < main_decLevel){
			upPtrEnd = main_decStackPtrs[dl+1];
		} else {
			upPtrEnd = main_complexUP_end;
		}
		printf("c (DL%u, %p %p): ", dl, (void*)upPtr, (void*)upPtrEnd);
		while ((l = *upPtr) != NULL && upPtr < upPtrEnd){
			printf("%d ", GET_LIT_NUM_FROM_PTR(l));
			++upPtr;
		}
		printf("NULL\n");

		++dl;
	}
}
#endif
