/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef STROTHERRULES_H_
#define STROTHERRULES_H_

#include "../cdcl.h"

#define CDCL_STROTHERRULE_MINVALUE 0
#define CDCL_STROTHERRULE_MAXVALUE 1

#define CDCL_STROTHERRULE_NULL 0	//If no strengthening is to be performed on other clauses with the learned clauses.
#include "sor_null.h"

#define CDCL_STROTHERRULE_WTWO 1 //If strengthening is to be done using the second watcher as resolution variable.
#include "sor_wtwo.h"

//Select Variable Rule plug-in internal data-structures.
//(none yet)

static inline void cdcl_strOtherRules_printHelp(){
	cdcl_strOtherRule_null_printHelp();
	cdcl_strOtherRule_wtwo_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_strOtherRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-STROTHER-RULES: CDCL_STROTHERRULE_NULL %d\n", prefix, CDCL_STROTHERRULE_NULL);
	printf("%s CDCL-STROTHER-RULES: CDCL_STROTHERRULE_WTWO %d\n", prefix, CDCL_STROTHERRULE_WTWO);
}
void (*cdcl_strOtherRule_prepare)();	//This points to one of the strengthening rules call preparation methods.
uint32_t (*cdcl_strOtherRule_str)();	//This points to one of the strengthening rules. This method will try to strengthen
										//clauses with the learned clause. This must return a BJ level if it differs from the
										//level we have computed so far (which is provided).

void cdcl_extern_strOtherRules_switch(int32_t);//Use to switch the strengthening rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_strOtherRules_resetPlugin();
void cdcl_strOtherRules_initPlugin();
void cdcl_strOtherRules_disposePlugin();

#endif /* STROTHERRULES_H_ */
