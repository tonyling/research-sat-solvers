/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef STRLEARNEDRULES_H_
#define STRLEARNEDRULES_H_

#include "../cdcl.h"

#define CDCL_STRLEARNEDRULE_MINVALUE 0
#define CDCL_STRLEARNEDRULE_MAXVALUE 5

#define CDCL_STRLEARNEDRULE_NULL 0	//If no strengthening is to be performed on the learned clauses.
#include "slr_null.h"

#define CDCL_STRLEARNEDRULE_ALLOCC 1 //Strengthening by resolution via inspecting all occurrences with opposite literals.
#include "slr_allocc.h"

#define CDCL_STRLEARNEDRULE_REASONS 2 //Strengthening by resolution via inspecting of literals in the learned clause.
#include "slr_reasons.h"

#define CDCL_STRLEARNEDRULE_ANTECEDENTS 3 //Strengthening by resolution via inspecting of literals in the learned clause. Is
									      //allowed to replace literals with other literals as long as the replacements have a
									      //higher decision level.
#include "slr_antecedents.h"

#define CDCL_STRLEARNEDRULE_LOCAL 4 //Strengthening by removing literals for which all antecedents in the reason are marked.
#include "slr_local.h"

#define CDCL_STRLEARNEDRULE_LOCALREC 5 //Recursively removing literals for which all antecedents in the reason are marked.
#include "slr_localrec.h"

//Select Variable Rule plug-in internal data-structures.
//(none yet)

static inline void cdcl_strLearnedRules_printHelp(){
	cdcl_strLearnedRule_null_printHelp();
	cdcl_strLearnedRule_allocc_printHelp();
	cdcl_strLearnedRule_reasons_printHelp();
	cdcl_strLearnedRule_antecedents_printHelp();
	cdcl_strLearnedRule_local_printHelp();
	cdcl_strLearnedRule_localrec_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_strLearnedRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_NULL %d\n", prefix, CDCL_STRLEARNEDRULE_NULL);
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_ALLOCC %d\n", prefix, CDCL_STRLEARNEDRULE_ALLOCC);
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_REASONS %d\n", prefix, CDCL_STRLEARNEDRULE_REASONS);
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_ANTECEDENTS %d\n", prefix, CDCL_STRLEARNEDRULE_ANTECEDENTS);
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_LOCAL %d\n", prefix, CDCL_STRLEARNEDRULE_LOCAL);
	printf("%s CDCL-STRLEARNED-RULES: CDCL_STRLEARNEDRULE_LOCALREC %d\n", prefix, CDCL_STRLEARNEDRULE_LOCALREC);
}

void (*cdcl_strLearnedRule_prepare)();	//This points to one of the strengthening rules call preparation methods.
void (*cdcl_strLearnedRule_str)();		//This points to one of the strengthening rules. This method will try to strengthen
										//all the learned clauses. This method must ensure that the size value is correct and
										//the literals are terminated with 0.

void cdcl_extern_strLearnedRules_switch(int32_t);//Use to switch the strengthening rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_strLearnedRules_resetPlugin();
void cdcl_strLearnedRules_initPlugin();
void cdcl_strLearnedRules_disposePlugin();

#endif /* STRLEARNEDRULES_H_ */
