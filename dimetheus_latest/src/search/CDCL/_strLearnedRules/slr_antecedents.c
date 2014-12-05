/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_antecedents_printHelp(){
	printf("c      %-3d: ANTECEDENTS:\n", CDCL_STRLEARNEDRULE_ANTECEDENTS);
    printf("c           Behavior: Strengthens learned clauses with resolution on reasons from literals in the clause.\n");
}

void cdcl_strLearnedRule_antecedents_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: ANTECEDENTS]:\n", CDCL_STRLEARNEDRULE_ANTECEDENTS);
	#endif
}

int32_t cdcl_strLearnedRule_antecedents_intern_comp(const void* v, const void* w){
	//The variables v and w are literal IDs. We grab the corresponding variables and switch their positions in the clause if
	//and only if the decision level of w is smaller than the decision level of v.
	if ( GET_CDCL_VAR_DECLEVEL(abs(*((int32_t*)w))) < GET_CDCL_VAR_DECLEVEL(abs(*((int32_t*)v))) ){
		return 1;
	}
	return -1;
}

void cdcl_strLearnedRule_antecedents_str(){
	//We try to strengthen the learned clauses using resolution. The resolution partners are those clauses that act as
	//reasons on the decision stack. We are allowed to replace a literal in the learned clause as long as the replacements
	//have a higher decision level.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	uint32_t replacement;
	#endif
	cdclClause *c, *d;
	cdclLiteral *oppLit, *aLit, *missingLit = NULL;
	cdclVariable *resVar, *missingVar;
	uint32_t learnedNum, i, checkPos, writePos, numMissing;

	for (learnedNum = cdcl_cls_l_used - cdcl_numLearned; learnedNum < cdcl_cls_l_used; ++learnedNum){
		c = cdcl_cls_l[learnedNum];
		//We ignore c if it is a unit clause.
		if (GET_CDCL_CLS_SIZE(c) == 1U) continue;
		#ifdef COLLECTSTATS
		++stats_cdcl_numStrChecks;
		replacement = 0;
		#endif

		//First, we stamp all the literals in c. This way we can easily check if a given literal is present in c without
		//the need to perform search.
		++cdcl_litStamp;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c, i)),cdcl_litStamp);
		}

		//We now sort the literals in the clause such that those with the largest decision level number are found at the end.
		//We therefore aim to remove/replace those literals first, that have been assigned first.
		qsort(GET_CDCL_CLS_LITS(c),GET_CDCL_CLS_SIZE(c),sizeof(int32_t),cdcl_strLearnedRule_antecedents_intern_comp);

		writePos = 0;
		for (checkPos = 0; checkPos < GET_CDCL_CLS_SIZE(c); ++checkPos){
			d = GET_CDCL_VAR_PTR_REASON((cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,checkPos))));
			if (d == NULL) {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
				continue;
			}

			//The variable was assigned because of some reason clause d. We will now check d and count the number of literals
			//in d that are missing in c (ignoring the literal we perform resolution with). If this is zero, we can do
			//strengthening. If this is one, we can check if the one missing literal has a higher decision level than the
			//variable we resolve with. If so, we can replace the resolving literal with the one that has a higher decision
			//level.

			oppLit = cdcl_literals - GET_CDCL_CLS_LITNUM(c,checkPos);
			resVar = cdcl_variables + abs(GET_CDCL_CLS_LITNUM(c,checkPos));
			numMissing = 0;
			for (i = 0; i < GET_CDCL_CLS_SIZE(d); ++i){
				aLit = cdcl_literals + GET_CDCL_CLS_LITNUM(d,i);
				if (aLit != oppLit && GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp){
					//Check, if too many literals are already missing.
					if (++numMissing > 1U) break;
					missingVar = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(aLit));
					//If not, check if the missing literal has a strictly higher decision level than the variable of oppLit.
					if (GET_CDCL_VAR_PTR_DECLEVEL(missingVar) >= GET_CDCL_VAR_PTR_DECLEVEL(resVar)){
						//No, we have a missing literal here but its decision level does not allow us to replace the resolving
						//literal with aLit. Therefore, no strengthening is possible.
						++numMissing; break;
					} else {
						//The missing literal has a strictly higher decision level and we could replace the resLit with it
						//as long as no other literal turn up missing.
						missingLit = aLit;
					}
				}
			}
			//Now IF the number of missing literals is zero, we have classical strengthening and just want to get rid of
			//the literal that is currently the resLit. If numMissing is exactly 1 we can replace resLit with aLit, as it
			//has a strictly larger decision level. If numMissing is larger 2, we cannot do anything.

			if (numMissing == 0U){
				//Strengthening is possible. That is, we do not want to retain the literal currently found at
				//checkPos. We do not increase the writePos and just remove the stamp from the literal signaling that it is
				//not found in the clause anymore.
				SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c,checkPos)), 0);
				//We then continue with the next literal to check in c.
			} else if (numMissing == 1U) {
				//A replacement is possible. We reset the stamp of the replacement literal to signal that it is not in the
				//clause anymore. We then override the literal found at checkPos with the one we have as replacement. We set
				//its stamp to the current literal stamp to signal that it is in the clause now.
				SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c,checkPos)), 0);
				//Write the replacement.
				SET_CDCL_CLS_LITNUM(c, checkPos, GET_CDCL_LIT_ID_FROM_PTR(missingLit));
				SET_CDCL_LIT_PTR_STAMP(missingLit ,cdcl_litStamp);
				//We must decrease checkPos by one such that the next literal that is checked is the one we just added.
				--checkPos;
				#ifdef COLLECTSTATS
				replacement = 1U;
				#endif
			} else {
				//No action can be performed. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				++writePos;
			}
		}
		//Check if this clause experienced some changes.
		if (writePos < checkPos){
			//We have removed at least one literal in this clause. We must now terminate it anew.
			SET_CDCL_CLS_SIZE(c, writePos);
			SET_CDCL_CLS_LITNUM(c, writePos, 0);
			#ifdef COLLECTSTATS
			++stats_cdcl_numStrSuccessAreReasons;
			++stats_cdcl_numStrSuccess;
			#endif
		}
		#ifdef COLLECTSTATS
		if (replacement){
			++stats_cdcl_numStrSuccessIsRepl;
		}
		#endif
	}
	#ifdef COLLECTSTATS
	stats_cdcl_time_str += (STATS_GET_CURRENT_TIME_IN_SEC() - time_start);
	#endif
}
