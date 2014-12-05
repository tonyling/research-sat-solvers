/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "strOtherRules.h"

void cdcl_strOtherRule_wtwo_printHelp(){
	printf("c      %-3d: WTWO:\n", CDCL_STROTHERRULE_WTWO);
    printf("c           Behavior: Strengthens clauses using the WL2 literal for resolution.\n");
}

void cdcl_strOtherRule_wtwo_prepare(){
	#ifdef VERBOSE_CDCL
	printf("c     CDCL:     SOR-Plug-in [%-3d: WTWO]:\n", CDCL_STROTHERRULE_WTWO);
	#endif
}

uint32_t cdcl_strOtherRule_wtwo_str(){
	//We use the last learned clause and try to strengthen other clauses with it using watcher two for resolution.
	cdclClause *c = cdcl_cls_l[cdcl_cls_l_used - 1], *d, *e;
	uint64_t sig, newSig;
	int32_t resLit, found, aLit, bLit, i;
	uint32_t numC, numD, resLitPos = 0, restart = 1;


	//We strengthen with short clauses only.
	if (GET_CDCL_CLS_SIZE(c) < 2 || GET_CDCL_CLS_SIZE(c) > 3) return cdcl_targetBJLevel;

	while (restart){
		restart = 0;
		sig = 0ULL;

		resLit = GET_CDCL_CLS_W2(c);

		//First we create a signature of c that does not contain the signature bit for the literal we resolve on.
		if (GET_CDCL_CLS_SIZE(c) == 2){
			//Clause c is binary.
			if (GET_CDCL_CLS_LITNUM(c, 0) != resLit) {
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, 0));
			} else {
				resLitPos = 0;
			}
			if (GET_CDCL_CLS_LITNUM(c, 1) != resLit) {
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, 1));
			} else {
				resLitPos = 1;
			}
		} else {
			//Clause c is ternary.
			if (GET_CDCL_CLS_LITNUM(c, 0) != resLit) {
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, 0));
			} else {
				resLitPos = 0;
			}
			if (GET_CDCL_CLS_LITNUM(c, 1) != resLit) {
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, 1));
			} else {
				resLitPos = 1;
			}
			if (GET_CDCL_CLS_LITNUM(c, 2) != resLit) {
				sig |= GET_CDCL_LIT_SIG(GET_CDCL_CLS_LITNUM(c, 2));
			} else {
				resLitPos = 2;
			}
		}
		//Now the signature without the resolution literal has been created.

		//Walk through the occurrence list of the second watcher literal with opposite sign and check if any clause in here can
		//be strengthened by the learned clause.
		for (i = 0; i < GET_CDCL_LIT_NUMOCCS_USED(-resLit); ++i){
			d = GET_CDCL_LIT_OCCNUM(-resLit, i);
			//Fast check if strengthening is possible. We rule out d if it is too large or the signatures do not match.
			if (GET_CDCL_CLS_SIZE(d) > 3
					|| GET_CDCL_CLS_SIZE(c) > GET_CDCL_CLS_SIZE(d)
					|| (sig & ~GET_CDCL_CLS_SIG(d)) !=0ULL ) continue;

			#ifdef COLLECTSTATS
			++stats_cdcl_numSorChecks;
			#endif

			//The simple checks did not rule out clause d. We must check if all the literals from c if they are found in d.
			found = 0;
			for (numC = 0; numC < GET_CDCL_CLS_SIZE(c); ++numC){
				if (GET_CDCL_CLS_LITNUM(c,numC) == resLit) {
					continue;//We ignore the literal we resolve on.
				}
				found = 0;
				for (numD = 0; numD < GET_CDCL_CLS_SIZE(d); ++numD){
					if (GET_CDCL_CLS_LITNUM(c,numC) == -GET_CDCL_CLS_LITNUM(d,numD)){
						found = 0;//This resolution would end in a tautology.
						break;
					} else if (GET_CDCL_CLS_LITNUM(c,numC) == GET_CDCL_CLS_LITNUM(d,numD)){
						found = 1;//The literal from c, which is not resLit, has been found in d.
						break;
					}
				}
				if (!found){
					break;
				}
			}
			if (!found) continue;

			if (GET_CDCL_CLS_SIZE(c) == 3 && GET_CDCL_CLS_SIZE(d) == 3){
				//This operation will drop literal resLit from clause c (the newly learned clause) which is then binary. Since
				//the clause has not yet been added to any lists, we must not remove it from any lists.
				//We remove the literal at position resLitPos in c, because this is the literal that can be strengthened away.
				//We also update the clauses signature and size.
				#ifdef COLLECTSTATS
				if (GET_CDCL_CLS_SIZE(c) > STATS_CDCL_LEARNEDLENGTH_MAX){
					--stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
				} else {
					--stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(c)];
				}
				#endif
				SET_CDCL_CLS_SIG(c, sig);
				SET_CDCL_CLS_SIZE_DEC(c);
				SET_CDCL_CLS_LITNUM(c, resLitPos, GET_CDCL_CLS_LITNUM(c, GET_CDCL_CLS_SIZE(c)));

				aLit = GET_CDCL_CLS_LITNUM(c, 0);
				bLit = GET_CDCL_CLS_LITNUM(c, 1);

				//We must now figure out what the new target level is that we must back-jump to. WL1 must be the literal that
				//has the higher decision level.
				if (GET_CDCL_VAR_DECLEVEL(abs(aLit)) < GET_CDCL_VAR_DECLEVEL(abs(bLit))){
					SET_CDCL_CLS_W1(c, bLit);
					SET_CDCL_CLS_REPLIT(c, bLit);
					SET_CDCL_CLS_W2(c, aLit);
					if (GET_CDCL_VAR_DECLEVEL(abs(aLit)) < cdcl_targetBJLevel){
						cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(aLit));
					}
				} else {
					SET_CDCL_CLS_W1(c, aLit);
					SET_CDCL_CLS_REPLIT(c, aLit);
					SET_CDCL_CLS_W2(c, bLit);
					if (GET_CDCL_VAR_DECLEVEL(abs(bLit)) < cdcl_targetBJLevel){
						cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(bLit));
					}
				}
				//We have successfully made the learned clause a binary clause. We try again with the binary.
				restart = 1;
				#ifdef COLLECTSTATS
				if (GET_CDCL_CLS_SIZE(c) > STATS_CDCL_LEARNEDLENGTH_MAX){
					++stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
				} else {
					++stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(c)];
				}
				++stats_cdcl_numSorSuccLarge;
				#endif
				break;
			} else if (GET_CDCL_CLS_SIZE(c) == 2 && GET_CDCL_CLS_SIZE(d) == 3){
				//We create a second learned clause, that is binary, that is a copy of d without -resLit.
				//First, we ensure that we can learn a new clause.
				newSig = 0ULL;

				ENSURE_CDCL_CLS_LEARNED_AVAIL_FOR(1u, numC, e);
				e = cdcl_cls_l[cdcl_cls_l_used];
				REALLOC_CDCL_CLS_TO_SIZE(e, 3);
				RESET_CDCL_CLS(e);

				aLit = GET_CDCL_CLS_LITNUM(d, 0);
				if (aLit != -resLit){
					SET_CDCL_CLS_LITNUM(e, GET_CDCL_CLS_SIZE(e), aLit);
					newSig |= GET_CDCL_LIT_SIG(aLit);
					SET_CDCL_CLS_SIZE_INC(e);
				}
				aLit = GET_CDCL_CLS_LITNUM(d, 1);
				if (aLit != -resLit){
					SET_CDCL_CLS_LITNUM(e, GET_CDCL_CLS_SIZE(e), aLit);
					newSig |= GET_CDCL_LIT_SIG(aLit);
					SET_CDCL_CLS_SIZE_INC(e);
				}
				aLit = GET_CDCL_CLS_LITNUM(d, 2);
				if (aLit != -resLit){
					SET_CDCL_CLS_LITNUM(e, GET_CDCL_CLS_SIZE(e), aLit);
					newSig |= GET_CDCL_LIT_SIG(aLit);
					SET_CDCL_CLS_SIZE_INC(e);
				}

				SET_CDCL_CLS_LITNUM(e, GET_CDCL_CLS_SIZE(e), 0);
				aLit = GET_CDCL_CLS_LITNUM(e, 0);
				bLit = GET_CDCL_CLS_LITNUM(e, 1);

				SET_CDCL_CLS_LBD(e, 2);
				SET_CDCL_CLS_SIG(e, newSig);


				//We must now figure out, what literal has the higher decision level, this will be watcher 2.
				if (IS_CDCL_VAR_UNASSIGNED(abs(aLit)) && IS_CDCL_VAR_UNASSIGNED(abs(bLit))){
					//If both variables are unassigned, we can use arbitrary watchers and representative literal.
					SET_CDCL_CLS_W1(e, aLit);
					SET_CDCL_CLS_REPLIT(e, aLit);
					SET_CDCL_CLS_W2(e, bLit);
				} else if (!IS_CDCL_VAR_UNASSIGNED(abs(aLit)) && IS_CDCL_VAR_UNASSIGNED(abs(bLit))){
					//Here, the first variable is assigned, the second is unassigned. The first literal must be the
					//second watcher, as it would imply the asserting level.
					SET_CDCL_CLS_W1(e, bLit);
					SET_CDCL_CLS_REPLIT(e, bLit);
					SET_CDCL_CLS_W2(e, aLit);
					if (IS_CDCL_LIT_UNSAT_ON_DL(aLit, cdcl_targetBJLevel) && GET_CDCL_VAR_DECLEVEL(abs(aLit)) < cdcl_targetBJLevel){
						cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(aLit));
					}
				} else if (IS_CDCL_VAR_UNASSIGNED(abs(aLit)) && !IS_CDCL_VAR_UNASSIGNED(abs(bLit))){
					//Here, the second variable is assigned, the first is unassigned. The second literal must be the
					//second watcher, as it would imply the asserting level.
					SET_CDCL_CLS_W1(e, aLit);
					SET_CDCL_CLS_REPLIT(e, aLit);
					SET_CDCL_CLS_W2(e, bLit);
					if (IS_CDCL_LIT_UNSAT_ON_DL(bLit, cdcl_targetBJLevel) && GET_CDCL_VAR_DECLEVEL(abs(bLit)) < cdcl_targetBJLevel){
						cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(bLit));
					}
				} else {
					//We must now figure out what the new target level is that we must back-jump to. WL1 must be the literal
					//that has the higher decision level.
					if (GET_CDCL_VAR_DECLEVEL(abs(aLit)) < GET_CDCL_VAR_DECLEVEL(abs(bLit))){
						SET_CDCL_CLS_W1(e, bLit);
						SET_CDCL_CLS_REPLIT(e, bLit);
						SET_CDCL_CLS_W2(e, aLit);
						if (IS_CDCL_LIT_UNSAT_ON_DL(aLit, cdcl_targetBJLevel) && GET_CDCL_VAR_DECLEVEL(abs(aLit)) < cdcl_targetBJLevel){
							cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(aLit));
						}
					} else {
						SET_CDCL_CLS_W1(e, aLit);
						SET_CDCL_CLS_REPLIT(e, aLit);
						SET_CDCL_CLS_W2(e, bLit);
						if (IS_CDCL_LIT_UNSAT_ON_DL(bLit, cdcl_targetBJLevel) && GET_CDCL_VAR_DECLEVEL(abs(bLit)) < cdcl_targetBJLevel){
							cdcl_targetBJLevel = GET_CDCL_VAR_DECLEVEL(abs(bLit));
						}
					}
				}

				++cdcl_cls_l_used;
				++cdcl_numLearned;
				#ifdef COLLECTSTATS
				if (GET_CDCL_CLS_SIZE(e) > STATS_CDCL_LEARNEDLENGTH_MAX){
					++stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
				} else {
					++stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(e)];
				}
				++stats_cdcl_numSorSuccBin;
				#endif
			} else if (GET_CDCL_CLS_SIZE(c) == 2 && GET_CDCL_CLS_SIZE(d) == 2){
				//In this case, we can remove from clause c the resLit and c will become unit. This is the best case that can
				//happen, because we can now back-jump to DL0.
				#ifdef COLLECTSTATS
				if (GET_CDCL_CLS_SIZE(c) > STATS_CDCL_LEARNEDLENGTH_MAX){
					--stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
				} else {
					--stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(c)];
				}
				#endif
				if (GET_CDCL_CLS_LITNUM(c, 0) == resLit){
					//The first literal in the clause must be removed. The second literal is the unit.
					SET_CDCL_CLS_LITNUM(c, 0, GET_CDCL_CLS_LITNUM(c, 1));
					SET_CDCL_CLS_SIZE_DEC(c);
					SET_CDCL_CLS_W1(c, GET_CDCL_CLS_LITNUM(c,0));
					SET_CDCL_CLS_W2(c, 0);
					SET_CDCL_CLS_SIG(c, 0ULL);
				} else {
					//The second literal in the clause must be removed. The first literal is the unit.
					SET_CDCL_CLS_SIZE_DEC(c);
					SET_CDCL_CLS_W1(c, GET_CDCL_CLS_LITNUM(c,0));
					SET_CDCL_CLS_W2(c, 0);
					SET_CDCL_CLS_SIG(c, 0ULL);
				}
				//We can stop as we detected a failed literal in this "unit" clause now.
				cdcl_targetBJLevel = 0;
				#ifdef COLLECTSTATS
				if (GET_CDCL_CLS_SIZE(c) > STATS_CDCL_LEARNEDLENGTH_MAX){
					++stats_cdcl_learnedLength[STATS_CDCL_LEARNEDLENGTH_MAX+1];
				} else {
					++stats_cdcl_learnedLength[GET_CDCL_CLS_SIZE(c)];
				}
				++stats_cdcl_numSorSuccUnit;
				#endif
				break;
			}
		}
	}

	return cdcl_targetBJLevel;
}



