/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_reasons_printHelp(){
	printf("c      %-3d: REASONS:\n", CDCL_STRLEARNEDRULE_REASONS);
    printf("c           Behavior: Strengthens learned clauses with resolution on reasons from literals in the clause.\n");
}

void cdcl_strLearnedRule_reasons_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: REASONS]:\n", CDCL_STRLEARNEDRULE_REASONS);
	#endif
}

void cdcl_strLearnedRule_reasons_str(){
	//We try to strengthen the learned clauses using resolution. The resolution partners are those clauses that act as
	//reasons on the decision stack.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	#endif
	cdclClause *c, *d;
	cdclLiteral *oppLit, *aLit;
	uint64_t testSig, sig;
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
		sig = 0ULL;
		++cdcl_litStamp;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c, i)),cdcl_litStamp);
			sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, i));
		}
		//We now walk through all the literals in the clause, and for each of them that was not assigned as a decision, we
		//will look at the reason for this variable to be in the clause. This reason might be used to resolve out the literal.
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
			//at checkPos via resolution. We first create the test signature and check with it if a strengthening is possible.
			testSig = sig | GET_CDCL_LIT_SIG((-GET_CDCL_CLS_LITNUM(c,checkPos)));
			#ifdef COLLECTSTATS
			++stats_cdcl_numStrSigChecks;
			#endif
			if((testSig & GET_CDCL_CLS_SIG(d)) != GET_CDCL_CLS_SIG(d)) {
				//No strengthening is possible. Increase the writePos. Write the literal we were unable to remove. Override
				//writePos with checkPos.
				#ifdef COLLECTSTATS
				++stats_cdcl_numStrSigSuccess;
				#endif
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
				continue;
			}
			//The signature test did not rule out the clause d. We must check all the literals in d and see if they are all
			//part of clause c.
			oppLit = cdcl_literals - GET_CDCL_CLS_LITNUM(c,checkPos);

			for (i = 0; i < GET_CDCL_CLS_SIZE(d); ++i){
				aLit = cdcl_literals + GET_CDCL_CLS_LITNUM(d,i);
				if (aLit != oppLit && GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp) break;
			}
			//We check for an early abort in the check loop above. If so, we know that at least one literal in d does
			//not appear in c. So no strengthening is possible. Otherwise, strengthening IS possible.
			if (i < GET_CDCL_CLS_SIZE(d)) {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
			} else {
				//Strengthening IS possible. That is, we do not want to retain the literal currently found at
				//checkPos. We do not increase the writePos and just remove the stamp from the literal signaling that it is
				//not found in the clause anymore.
				SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c,checkPos)), 0);
				//We then continue with the next literal to check in c.
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
