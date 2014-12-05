/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "computeBJLRules.h"

void cdcl_computeBJLRule_firstassert_printHelp(){
	printf("c      %-3d: FIRSTASSERT:\n", CDCL_COMPUTEBJLRULE_FIRSTASSERT);
    printf("c           Behavior: Picks the first asserting level of all learned clauses.\n");
}

void cdcl_computeBJLRule_firstassert_prepare(){
	//Nothing is to be done to initialize first assert compute back-jump level rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     CBJLR-Plug-in [%-3d: FIRSTASSERT]:\n", CDCL_COMPUTEBJLRULE_FIRSTASSERT);
	#endif
}

uint32_t cdcl_computeBJLRule_firstassert_compute(){
	//This method will inspect all newly learned clauses. The second watcher for each clause is the back-jump level this
	//clause implies. The first watcher is the literal that this clause implies in that level. That is, we want to find out
	//which of the clauses has the highest decision level in the variable of  watcher 2. This is return.
	//If any of the learned clauses is unit, we must return DL0 as result -- the first watcher then is the new assignment
	//for DL0.
	cdclClause *c;
	cdclVariable *v;
	uint32_t i, targetLevel = UINT32_MAX;

	for (i = cdcl_cls_l_used - cdcl_numLearned; i < cdcl_cls_l_used; ++i){
		//We get the next learned clause.
		c = cdcl_cls_l[i];
		//We check if the learned clause is of size 1. If so, it is a unit clause and the decision level we want to
		//back-jump to MUST be level 0.
		if (GET_CDCL_CLS_SIZE(c) == 1u){
			//Yes, we can stop and signal that we must roll back to level 0.
			targetLevel = 0u;
			break;
		}
		//If the clause has at least size 2, we have the two watched literals. We check if the variable of WL2 in this clause
		//has a higher decision level than what we have computed so far. If so, we reduce the back-jump level even further.
		v = cdcl_variables + abs(GET_CDCL_CLS_W2(c));
		if (GET_CDCL_VAR_PTR_DECLEVEL(v) < targetLevel){
			targetLevel = GET_CDCL_VAR_PTR_DECLEVEL(v);
		}
	}

	return targetLevel;
}

