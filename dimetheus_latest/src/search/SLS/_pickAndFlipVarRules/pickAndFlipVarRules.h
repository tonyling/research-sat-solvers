/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PICKANDFLIPVARRULES_H_
#define PICKANDFLIPVARRULES_H_

#include "../sls.h"

#define SLS_PICKANDFLIPVARRULE_MINVALUE 0
#define SLS_PICKANDFLIPVARRULE_MAXVALUE 2

#define SLS_PICKANDFLIPVARRULE_WALKSAT 0	//Pick and flip variables WalkSAT style.
#include "pafvr_walksat.h"

#define SLS_PICKANDFLIPVARRULE_PROBSATPB 1	//Pick and flip variables ProbSAT style (polynomial brake-only scores).
#include "pafvr_probsatPB.h"

#define SLS_PICKANDFLIPVARRULE_PROBSATEB 2	//Pick and flip variables ProbSAT style (exponential brake-only scores).
#include "pafvr_probsatEB.h"


//Pick and Flip Variable plug-in internal data-structures.
float_ty *sls_probsat_scoreLookupB;		//The lookup-table for scores for the ProbSAT heuristic (break value).

static inline void sls_pickAndFlipVarRules_printHelp(){
	sls_pickAndFlipVarRule_walksat_printHelp();
	sls_pickAndFlipVarRule_probsatPB_printHelp();
	sls_pickAndFlipVarRule_probsatEB_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void sls_extern_pickAndFlipVarRules_printNamesAndIDs(char* prefix){
	printf("%s SLS-PICKANDFLIPVAR-RULES: SLS_PICKANDFLIPVARRULE_WALKSAT %d\n", prefix, SLS_PICKANDFLIPVARRULE_WALKSAT);
	printf("%s SLS-PICKANDFLIPVAR-RULES: SLS_PICKANDFLIPVARRULE_PROBSATPB %d\n", prefix, SLS_PICKANDFLIPVARRULE_PROBSATPB);
	printf("%s SLS-PICKANDFLIPVAR-RULES: SLS_PICKANDFLIPVARRULE_PROBSATEB %d\n", prefix, SLS_PICKANDFLIPVARRULE_PROBSATEB);
}
void (*sls_pickAndFlipVarRule_prepare)();	//This points to one of the flip rules call preparation methods available.
void (*sls_pickAndFlipVarRule_paf)();		//This points to one of the flip rules that are available.

void sls_extern_pickAndFlipVarRules_switch(int32_t);//Use to switch the flip rule without re-initializing the whole SLS module.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void sls_pickAndFlipVarRules_resetPlugin();
void sls_pickAndFlipVarRules_initPlugin();
void sls_pickAndFlipVarRules_disposePlugin();

#endif /* PICKANDFLIPVARRULES_H_ */
