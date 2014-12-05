/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strLearnedRules.h"

void cdcl_strLearnedRule_allocc_printHelp(){
	printf("c      %-3d: ALLOCC:\n", CDCL_STRLEARNEDRULE_ALLOCC);
    printf("c           Behavior: Strengthens learned clauses with resolution on clauses containing an opposite literal.\n");
}

void cdcl_strLearnedRule_allocc_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SLR-Plug-in [%-3d: ALLOCC]:\n", CDCL_STRLEARNEDRULE_ALLOCC);
	#endif
}

void cdcl_strLearnedRule_allocc_str(){
	//We try to strengthen the learned clauses using resolution. For each literal l in the learned clause c, we check all
	//occurrences d of -l and see if all literals in d (except -l) also appear in c. If so, we can remove l from c.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	cdclVariable *strVar;
	#endif
	cdclClause *c, *d;
	cdclLiteral *oppLit, *resLit, *aLit;
	uint64_t testSig, sig;
	uint32_t learnedNum, occNum, i, erase, checkPos, writePos;

	for (learnedNum = cdcl_cls_l_used - cdcl_numLearned; learnedNum < cdcl_cls_l_used; ++learnedNum){
		c = cdcl_cls_l[learnedNum];
		//We ignore c if it is a unit clause.
		if (GET_CDCL_CLS_SIZE(c) == 1U) continue;
		#ifdef COLLECTSTATS
		++stats_cdcl_numStrChecks;
		#endif
		writePos = 0;

		//First, we stamp all the literals in c. This way we can easily check if a given literal is present in c without
		//the need to perform search.
		sig = 0ULL;
		++cdcl_litStamp;
		for (i = 0; i < GET_CDCL_CLS_SIZE(c); ++i){
			SET_CDCL_LIT_PTR_STAMP((cdcl_literals + GET_CDCL_CLS_LITNUM(c, i)), cdcl_litStamp);
			sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, i));
		}
		//We check the literal at checkPos position in clause c. If it cannot be removed, we write it to the writePos position.
		for (checkPos = 0; checkPos < GET_CDCL_CLS_SIZE(c); ++checkPos){
			//We grab the literals we want to use for resolution as pointers.
			resLit = cdcl_literals + GET_CDCL_CLS_LITNUM(c,checkPos);
			oppLit = cdcl_literals - GET_CDCL_CLS_LITNUM(c,checkPos);
			erase = 0;

			//We create a test signature that contains all the signature bits of the clause c and the signature of the oppLit.
			//In that way, we can pick a clause d and perform an AND operation like this: testSig & d->sig. If this results
			//in some signature NOT equal to the d->sig, then d contains some literal not part of c. This means that no
			//strengthening is possible.
			testSig = sig;
			testSig |= GET_CDCL_LIT_SIG(GET_CDCL_LIT_ID_FROM_PTR(oppLit));
			//We walk through the implication lists of resLit to check if any of the binary clauses removes a literal.
			for (occNum = 0; occNum < GET_CDCL_LIT_PTR_NUMIMPS_USED(resLit); occNum+=2){
				aLit = cdcl_literals + GET_CDCL_LIT_PTR_IMPNUM(resLit, occNum);
				if (GET_CDCL_LIT_PTR_STAMP(aLit) == cdcl_litStamp) {
					erase = 1;
					break;
				}
			}

			//Then, we walk through the occurrence list of oppLit -- all these clauses are possible resolution partners. The
			//resolution partners with size 2 can now be ignored because they have been checked above.
			if (!erase)
			for (occNum = 0; occNum < GET_CDCL_LIT_PTR_NUMOCCS_USED(oppLit); ++occNum){
				//Grab a resolution partner.
				d = GET_CDCL_LIT_PTR_OCCNUM(oppLit,occNum);
				//We now have clauses c and d and can check if all literals in d also appear in c (ignoring oppLit in d).
				//We first check if the size of d is is larger than the size of c. If so, no strengthening is possible.
				if (GET_CDCL_CLS_SIZE(d) == 2 || GET_CDCL_CLS_SIZE(d) > GET_CDCL_CLS_SIZE(c)) continue;
				//Then, we perform the signature test. If testSig & d->sig != d->sig, then d contains a literal not in
				//clause c. Therefore, no strengthening is possible.
				#ifdef COLLECTSTATS
				++stats_cdcl_numStrSigChecks;
				#endif
				if((testSig & GET_CDCL_CLS_SIG(d)) != GET_CDCL_CLS_SIG(d)) {
					#ifdef COLLECTSTATS
					++stats_cdcl_numStrSigSuccess;
					#endif
					continue;
				}
				//Neither the size nor the signature test can guarantee that no strengthening is possible. We must check all
				//the literals in d if they appear in c. We walk though the literals of d and check if all the literals
				//except oppLit are stamped.
				for (i = 0; i < GET_CDCL_CLS_SIZE(d); ++i){
					aLit = cdcl_literals + GET_CDCL_CLS_LITNUM(d,i);
					if (aLit != oppLit && GET_CDCL_LIT_PTR_STAMP(aLit) != cdcl_litStamp) break;
				}
				//We check for an early abort in the check loop above. If so, we know that at least one literal in d does
				//not appear in c. So no strengthening is possible. Otherwise, strengthening IS possible.
				if (i < GET_CDCL_CLS_SIZE(d)) continue;
				//We can remove resLit from clause c -- it is resolved out by clause d.
				erase = 1;
				break;
			}
			//We check if we had an early break. If this is the case then the clause d we have stored can resolve away resLit.
			if (erase){
				//Yes, a strengthening with d is possible. That is, we do not want to retain the literal currently found at
				//checkPos. We do not increase the writePos and just remove the stamp from the resLit signaling that it is
				//not found in the clause anymore.
				SET_CDCL_LIT_PTR_STAMP(resLit, 0);
				//We then continue with the next literal to check in c.
				#ifdef COLLECTSTATS
				strVar = cdcl_variables + abs(GET_CDCL_LIT_ID_FROM_PTR(resLit));
				if (GET_CDCL_VAR_PTR_REASON(strVar) != NULL && IS_CDCL_CLS_REASON(GET_CDCL_VAR_PTR_REASON(strVar))){
					++stats_cdcl_numStrSuccessAreReasons;
				} else {
					++stats_cdcl_numStrSuccessNoReasons;
				}
				#endif
			} else {
				//No strengthening is possible. Write the literal we were unable to remove. Override writePos with checkPos.
				SET_CDCL_CLS_LITNUM(c, writePos, GET_CDCL_CLS_LITNUM(c, checkPos));
				//Increase the write position.
				++writePos;
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
