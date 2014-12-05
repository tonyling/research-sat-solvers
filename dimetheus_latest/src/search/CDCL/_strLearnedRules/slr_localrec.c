/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_localrec_printHelp(){
	printf("c      %-3d: LOCALREC:\n", CDCL_STRLEARNEDRULE_LOCALREC);
    printf("c           Behavior: Performs strengthening by removing literals for which the antecedent is marked recursive.\n");
}

void cdcl_strLearnedRule_localrec_prepare(){
	//Nothing is to be done to initialize a call to the null strengthening rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: LOCALREC]:\n", CDCL_STRLEARNEDRULE_LOCALREC);
	#endif
}

uint32_t cdcl_strLearnedRule_localrec_checkRec(cdclClause *c, int32_t impliedLit){
	//We need to verify that for all literals in c (ignoring the implied literal), the literal is either marked, assigned in
	//decision level zero, or recursively marked. If a literal is not marked, not assigned in DL0 and has no reason,
	//we must return 0.
	cdclClause *antecedent;
	uint32_t i;
	int32_t lit;

	for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
		//Grab the literal.
		lit = GET_CDCL_CLS_LITNUM(c,i);
		//If this literal has been stamped in a decision level that has not been stamped, then it cannot successfully be found
		//stamped recursively. At least the decision in this level will not be stamped.
		if (GET_CDCL_VAR_STAMP(GET_CDCL_VAR_DECLEVEL(abs(lit))) != cdcl_litStamp){
			//No, we can abort early.
			return 0;
		}
		//We do not check on the recursed literal in the reason.
		if (lit == -impliedLit) continue;
		//No need to recurse on a literal that is already stamped.
		if (GET_CDCL_LIT_STAMP(lit) == cdcl_litStamp) continue;
		//No need to recurse on a literal that is assigned in DL0.
		if (GET_CDCL_VAR_DECLEVEL(abs(lit)) == 0) continue;
		//We now check if the variable has an antecedent.
		antecedent = GET_CDCL_VAR_REASON(abs(lit));
		//The literal is neither stamped nor assigned in DL0. If it has no reason, then it was a decision. We must return 0.
		if (antecedent == NULL) return 0;
		//Otherwise, we must recurse on the literals in the antecedent. If this returns 0, we must return 0 as well.
		if (!cdcl_strLearnedRule_localrec_checkRec(antecedent, lit)){
			//Nope, the antecedent contained something not stamped.
			return 0;
		}
	}
	//Since we now know that the implied literal has been marked recursively, we mark it now.
	SET_CDCL_LIT_STAMP(impliedLit, cdcl_litStamp);
	SET_CDCL_LIT_STAMP(-impliedLit, cdcl_litStamp);
	return 1;
}

void cdcl_strLearnedRule_localrec_str(){
	//Strengthening using local minimization with the antecedent stamps.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdclClause *c, *antecedent;
	int32_t impliedLit;
	uint32_t learnedNum, i, checkPos, writePos;

	for (learnedNum = cdcl_cls_l_used - cdcl_numLearned; learnedNum < cdcl_cls_l_used; ++learnedNum){
		c = cdcl_cls_l[learnedNum];
		//We ignore c if it is a unit clause.
		if (GET_CDCL_CLS_SIZE(c) == 1U) continue;
		#ifdef COLLECTSTATS
		++stats_cdcl_numStrChecks;
		#endif

		//First, we stamp all the literals in c. We stamp all the decision levels of variables assigned in c.
		++cdcl_litStamp;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			impliedLit = GET_CDCL_CLS_LITNUM(c, i);
			SET_CDCL_LIT_STAMP(impliedLit,cdcl_litStamp);
			SET_CDCL_LIT_STAMP(-impliedLit,cdcl_litStamp);
			//We can understand the decision level of the impliedLit as a variable and stamp it. This way, we can later check
			//if literals have been assigned in decision levels not been stamped. If this is the case, the recursive check
			//would always fail.
			SET_CDCL_VAR_STAMP(GET_CDCL_VAR_DECLEVEL(abs(impliedLit)), cdcl_litStamp);
		}
		//We also stamp level zero.
		SET_CDCL_VAR_STAMP(0, cdcl_litStamp);

		//We now walk through all the literals in the clause, and for each of them we take a look at the reason (if existent).
		//If all the literals in the reason are stamped, we drop the literal from the clause.
		writePos = 0;
		for (checkPos = 0; checkPos < GET_CDCL_CLS_SIZE(c); ++checkPos){
			//Grab the literal.
			impliedLit = GET_CDCL_CLS_LITNUM(c,checkPos);
			//Grab the reason of the literal.
			antecedent = GET_CDCL_VAR_REASON(abs(GET_CDCL_CLS_LITNUM(c,checkPos)));
			//If the literal has no reason, then it was a decision.
			if (antecedent == NULL) {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
				continue;
			}
			//We need to check if all literals in the antecedent clause of the implied literal are marked at least recursively.
			if (!cdcl_strLearnedRule_localrec_checkRec(antecedent, impliedLit)) {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
			} else {
				//Strengthening IS possible. That is, we do not want to retain the literal currently found at
				//checkPos. We do not increase the writePos. We then continue with the next literal to check in c.
				#ifdef COLLECTSTATS
				++stats_cdcl_numStrSuccessAreReasons;
				#endif
			}
		}
		//Check if this clause experienced some changes.
		if (writePos < checkPos){
			//We have removed at least one literal in this clause. We must now terminate it anew.
			SET_CDCL_CLS_SIZE(c, writePos);
			SET_CDCL_CLS_LITNUM(c, writePos, 0);
			#ifdef COLLECTSTATS
			++stats_cdcl_numStrSuccess;
			#endif
		}

	}
	#ifdef COLLECTSTATS
	stats_cdcl_time_str += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}



