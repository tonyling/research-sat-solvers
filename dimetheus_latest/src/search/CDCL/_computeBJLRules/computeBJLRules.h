/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef COMPUTEBJLRULES_H_
#define COMPUTEBJLRULES_H_

#include "../cdcl.h"

#define CDCL_COMPUTEBJLRULE_MINVALUE 0
#define CDCL_COMPUTEBJLRULE_MAXVALUE 0

#define CDCL_COMPUTEBJLRULE_FIRSTASSERT 0	//Computes the BJ level as the first asserting level of all learned clauses.
#include "cbjlr_firstassert.h"

//Compute Back-Jump Level Rule plug-in internal data-structures.
//(none yet)

static inline void cdcl_computeBJLRules_printHelp(){
	cdcl_computeBJLRule_firstassert_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_computeBJLRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-COMPUTEBJL-RULES: CDCL_COMPUTEBJLRULE_FIRSTASSERT %d\n", prefix, CDCL_COMPUTEBJLRULE_FIRSTASSERT);
}
void (*cdcl_computeBJLRule_prepare)();		//This points to one of the computation rules call preparation methods.
uint32_t (*cdcl_computeBJLRule_compute)();	//This points to one of the computation rules given the number of newly
											//learned clauses. This method MUST return the target back-jump level.
											//This method MUST return 0, if a unit clause was learned.

void cdcl_extern_computeBJLRules_switch(int32_t);//Use to switch the computation rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_computeBJLRules_resetPlugin();
void cdcl_computeBJLRules_initPlugin();
void cdcl_computeBJLRules_disposePlugin();

#endif /* COMPUTEBJLRULES_H_ */
