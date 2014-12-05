/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_local_printHelp(){
	printf("c      %-3d: LOCAL:\n", CDCL_STRLEARNEDRULE_LOCAL);
    printf("c           Behavior: Performs strengthening by removing literals for which the antecedent is marked.\n");
}

void cdcl_strLearnedRule_local_prepare(){
	//Nothing is to be done to initialize a call to the null strengthening rule.
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: LOCAL]:\n", CDCL_STRLEARNEDRULE_LOCAL);
	#endif
}

void cdcl_strLearnedRule_local_str(){
	//Strengthening using local minimization with the antecedent stamps.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdclClause *c, *d;
	cdclLiteral *aLit;
	uint32_t learnedNum, i, checkPos, writePos;

	for (learnedNum = cdcl_cls_l_used - cdcl_numLearned; learnedNum < cdcl_cls_l_used; ++learnedNum){
		c = cdcl_cls_l[learnedNum];
		//We ignore c if it is a unit clause.
		if (GET_CDCL_CLS_SIZE(c) == 1U) continue;
		#ifdef COLLECTSTATS
		++stats_cdcl_numStrChecks;
		#endif

		//First, we stamp all the literals in c. This way we can easily check if a given literal is present in c without
		//the need to perform search. We also update the local signature of the clause to be able to do the signature test.

		++cdcl_litStamp;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			SET_CDCL_LIT_STAMP(GET_CDCL_CLS_LITNUM(c, i),cdcl_litStamp);
			SET_CDCL_LIT_STAMP(-GET_CDCL_CLS_LITNUM(c, i),cdcl_litStamp);
		}
		//We now walk through all the literals in the clause, and for each of them we take a look at the reason (if existent).
		//If all the literals in the reason are stamped, we drop the literal from the clause.
		writePos = 0;
		for (checkPos = 0; checkPos < GET_CDCL_CLS_SIZE(c); ++checkPos){
			d = GET_CDCL_VAR_PTR_REASON((cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,checkPos))));
			if (d == NULL) {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
				continue;
			}

			//The variable was assigned because of some reason clause d. We will now check if d can resolve out the literal
			//at checkPos via the inspection of its reason. If all the literals in the reason clause are stamped, we can
			//drop the literal.
			for (i = 0; i < GET_CDCL_CLS_SIZE(d); ++i){
				aLit = cdcl_literals + GET_CDCL_CLS_LITNUM(d,i);
				if (GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp && GET_CDCL_VAR_DECLEVEL(abs(GET_CDCL_CLS_LITNUM(d,i))) > 0){
					//Early break since this literal in reason d is not stamped.
					break;
				}
			}
			//If we had an early break, we have found at least one literal in the reason clause d that was not stamped. We
			//must therefore keep the literal at this position.
			if (i < GET_CDCL_CLS_SIZE(d)) {
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


