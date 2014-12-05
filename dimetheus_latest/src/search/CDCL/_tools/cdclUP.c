/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "cdclUP.h"

cdclClause* cdcl_up_propagate(){
	cdclLiteral* l;  			//The current literal to propagate.
	cdclVariable* v; 			//The corresponding variable.
	cdclClause* c;	 			//The clause we currently work on when updating the watchers.
	int32_t i,lid, lit, litPos;	//The integer representation of the literal and a temporary literal and a literal position.
	int32_t offset; 			//The offset in the watcher list for the clause we currently work on.
	//This method will perform unit propagation from the current position in the decision stack. We assume that the UP is
	//initialized correctly, that is the cdcl_up_current pointer points to the position in the decision stack that we want to
	//continue a unit propagation from. Additionally, we assume that the cdcl_up_end pointer points to the next position
	//in the decision stack where we want to append new elements at. We also assume that the cdcl_up_stamp is fresh.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif

	cdclLiteral **checkFrom, **checkTo;

	//We propagate as long as we still have elements left.
	while (cdcl_up_current < cdcl_up_end){

		//Retain the information where we start to propagate.
		checkFrom = cdcl_up_current;

		do {
			//We grab the next literal for which we want to assign the variable.
			l = *(cdcl_up_current++);
			lid = GET_CDCL_LIT_ID_FROM_PTR(l);
			//We assign the corresponding variable.
			if (lid < 0){
				//The literal is negative. The variable must be assigned to 0.
				v = cdcl_variables - lid;
				SET_CDCL_VAR_PTR_ASS_0(v);
			} else {
				//the literal is positive. The variable must be assigned to 1.
				v = cdcl_variables + lid;
				SET_CDCL_VAR_PTR_ASS_1(v);
			}
			//We disable the variable, such that it does not appear in the variable priority queue anymore.
			cdcl_variables_Q_delete(v);
			#ifdef COLLECTSTATS
			++stats_cdcl_numPropagations;
			#endif

			//Walk through the implication list of l. All the implications must be put on the stack. If any of them fails,
			//we can return with a conflict.
			for (i = 0; i < GET_CDCL_LIT_PTR_NUMIMPS_USED(l); i+=2){
				lit = GET_CDCL_LIT_PTR_IMPNUM(l, i);
				if (CDCL_UP_IS_OPPLIT_ON_S(lit) || IS_CDCL_LIT_UNSAT(lit)){
					//We have already stamped the opposite literal, and as such, it must already be in the queue. Conflict.
					#ifdef COLLECTSTATS
					stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
					#endif
					return GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i);
				} else if (!CDCL_UP_IS_LIT_ON_S(lit) && !IS_CDCL_LIT_SAT(lit)){
					//The element is not yet on the stack. We put it there and stamp it.
					v = cdcl_variables + abs(lit);
					CDCL_UP_ADD_IMP(lit , v, GET_CDCL_LIT_PTR_IMPNUM_REASON(l, i));
				} //Else, the literal is already enqueued.
			}
		} while (cdcl_up_current < cdcl_up_end);

		//Retain the information where to stop checking long clauses.
		checkTo = cdcl_up_end;

		do {
			//Grab one of the propagated literals.
			l = *(checkFrom++);
			lid = GET_CDCL_LIT_ID_FROM_PTR(l);
			//We must now walk through the watcher lists of the literal that becomes false, that is, -l is checked.
			l = cdcl_literals - lid;
			//Walk through the watcher lists of l (the literal that becomes false). Find replacements for WL1 and WL2. If any
			//of the clauses becomes unit we must put the other watcher on the CDCL UP as it is the last literal that can make
			//the clause. If this fails, we can return with a conflict.

			//WL1
			offset = 0;
			while (GET_CDCL_LIT_PTR_NUMW1_USED(l) > offset){
				c = GET_CDCL_LIT_PTR_W1_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CDCL_CLS_REPLIT(c);
				if (IS_CDCL_LIT_SAT(lit) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W2(c))){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW1(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative literal. We make it so. Since the clause is satisfied, we do not need to change
						//the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the second watcher.
					if (lit == GET_CDCL_CLS_W2(c)) continue;
					//If the literal is neither the second watcher nor assigned we can use it as watcher replacement.
					if (!CDCL_UP_IS_OPPLIT_ON_S(lit) && !IS_CDCL_LIT_UNSAT(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W1(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W1(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W1LIST(-lid, litPos);
					ADD_CDCL_CLS_TO_LIT_W1LIST(c, lit);
				} else {
					//No, we must put the second watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CDCL_CLS_W2(c);
					if (IS_CDCL_LIT_UNSAT(lit) || CDCL_UP_IS_OPPLIT_ON_S(lit)){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						#ifdef COLLECTSTATS
						stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
						#endif
						return c;
					} else if (!IS_CDCL_LIT_SAT(lit) && !CDCL_UP_IS_LIT_ON_S(lit)){
						//The element is not yet on the stack. We put it there and stamp it.
						v = cdcl_variables + abs(lit);
						CDCL_UP_ADD_IMP(lit, v, c);
					} //Else, the literal is already on the stack.
				}
			}

			//WL2
			offset = 0;
			while (GET_CDCL_LIT_PTR_NUMW2_USED(l) > offset){
				c = GET_CDCL_LIT_PTR_W2_WATCHED_CLS(l, offset);
				//Before we start fiddling around with the clause, we check its representative literal.
				lit = GET_CDCL_CLS_REPLIT(c);
				if (IS_CDCL_LIT_SAT(lit) || IS_CDCL_LIT_SAT(GET_CDCL_CLS_W1(c))){
						//The representative literal is satisfied. We do not have to look at the clause any further.
						++offset;
						continue;
				}
				//We retain the information at what position the watcher is found.
				litPos = GET_CDCL_CLS_WHEREW2(c);
				//We now try to find out if we can replace the representative by some satisfied literal in the clause.
				for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
					lit = GET_CDCL_CLS_LITNUM(c,i);
					if (IS_CDCL_LIT_SAT(lit)){
						//We have found a literal in the clause that satisfies the clause. This should have been the
						//representative literal. We make it so. Since the clause is satisfied, we do not need to change
						//the watcher.
						SET_CDCL_CLS_REPLIT(c, lit);
						++offset; i=-1; //We set i to -1 to signal that we have found a repLit replacement.
						break;
					}
					//We ignore the literal if it is the first watcher.
					if (lit == GET_CDCL_CLS_W1(c)) continue;
					//If the literal is neither the first watcher nor assigned we can use it as watcher replacement.
					if (!CDCL_UP_IS_OPPLIT_ON_S(lit) && !IS_CDCL_LIT_UNSAT(lit)){
						//We have found an unassigned literal that can serve as watcher.
						SET_CDCL_CLS_W2(c, lit);
						break;
					}
				}
				//Check if the representative literal replacement was successful. If so, we are done with this clause.
				if (i<0) continue;
				//Check if we found a new watcher.
				if (GET_CDCL_CLS_W2(c) != -lid){
					//Yes, we can perform the replacement.
					REM_CDCL_CLS_FROM_LIT_W2LIST(-lid, litPos);
					ADD_CDCL_CLS_TO_LIT_W2LIST(c, lit);
				} else {
					//No, we must put the first watcher of the clause on the stack, if possible. If not we found a conflict.
					++offset;
					lit = GET_CDCL_CLS_W1(c);
					if (IS_CDCL_LIT_UNSAT(lit) || CDCL_UP_IS_OPPLIT_ON_S(lit)){
						//We have already stamped the opposite literal, and as such, it must already be on the stack. Conflict.
						#ifdef COLLECTSTATS
						stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
						#endif
						return c;
					} else if (!IS_CDCL_LIT_SAT(lit) && !CDCL_UP_IS_LIT_ON_S(lit)){
						//The element is not yet on the stack. We put it there and stamp it.
						v = cdcl_variables + abs(lit);
						CDCL_UP_ADD_IMP(lit, v, c);
					} //Else, the literal is already on the stack.
				}
			}
		} while (checkFrom < checkTo);
	}

	//In case no conflict occurred, we can terminate the current decision level and return NULL (no conflict clause).
	CDCL_UP_TERMINATE_CURRENT();
	#ifdef COLLECTSTATS
	stats_cdcl_time_up += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
	return NULL;
}

void cdcl_up_prepare(){
	//To prepare the unit propagation for the next call to the CDCL module. Both pointers point to the beginning of DL 1.
	CDCL_UP_SET_DECLEVEL(cdcl_decLevel);
	//The UP stamp is initialized to 1 (the stamps are all initialized to 0).
	cdcl_up_stamp = 1;
}

void cdcl_up_reset(){
	//To reset the unit propagation when the module is reset.
	cdcl_up_current = NULL;
	cdcl_up_end = NULL;
	cdcl_up_stamp = 0;
}

void cdcl_up_init(){
	//To initialize the unit propagation when the module is initialized.
	CDCL_UP_SET_DECLEVEL(cdcl_decLevel);
	//The UP stamp is initialized to 1 (the stamps are all initialized to 0).
	cdcl_up_stamp = 1;
}

void cdcl_up_dispose(){
	//To dispose the unit propagation when the module is disposed.
	cdcl_up_current = NULL;
	cdcl_up_end = NULL;
	cdcl_up_stamp = 0;
}

#ifdef VERBOSE_CDCL
void cdcl_up_printCurrentLevel(){
	cdclLiteral *l;
	cdclLiteral **upPtr = cdcl_up_current;
	printf("c     CDCL:   CURRENT UP (DL%u, %p %p): ", cdcl_decLevel, (void*)cdcl_up_current, (void*)cdcl_up_end);
	while ((l = *upPtr) != NULL && upPtr < cdcl_up_end){
		printf("%d ", GET_CDCL_LIT_ID_FROM_PTR(l));
		++upPtr;
	}
	printf("NULL\n");
}
#endif
