/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ABORTRULES_H_
#define ABORTRULES_H_

#include "../cdcl.h"

#define CDCL_ABORTRULE_MINVALUE 0
#define CDCL_ABORTRULE_MAXVALUE 2

#define CDCL_ABORTRULE_NULL 0	//If no aborts in the CDCL search are performed.
#include "ar_null.h"

#define CDCL_ABORTRULE_PMPCDCL 1 //If the abort is controlled by the search strategy PMPCDCL.
#include "ar_pmpcdcl.h"

#define CDCL_ABORTRULE_HYBRIDMSC 2 //If the abort is controlled by the search strategy HYBRIDMSC.
#include "ar_hybridmsc.h"

//Abort Rule plug-in internal data-structures.


static inline void cdcl_abortRules_printHelp(){
	cdcl_abortRule_null_printHelp();
	cdcl_abortRule_pmpcdcl_printHelp();
	cdcl_abortRule_hybridmsc_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void cdcl_extern_abortRules_printNamesAndIDs(char* prefix){
	printf("%s CDCL-ABORT-RULES: CDCL_ABORTRULE_NULL %d\n", prefix, CDCL_ABORTRULE_NULL);
	printf("%s CDCL-ABORT-RULES: CDCL_ABORTRULE_PMPCDCL %d\n", prefix, CDCL_ABORTRULE_PMPCDCL);
	printf("%s CDCL-ABORT-RULES: CDCL_ABORTRULE_HYBRIDMSC %d\n", prefix, CDCL_ABORTRULE_HYBRIDMSC);
}
void (*cdcl_abortRule_prepare)();//This points to one of the abort rules call preparation methods.
uint32_t (*cdcl_abortRule_check)();//This points to one of the abort rules. It return 1 if the abort condition holds.
void (*cdcl_abortRule_signalMaintenance)();//This points to one of the abort rule signaling methods.
void (*cdcl_abortRule_signalRestart)();//This points to one of the abort rule signaling methods.

void cdcl_extern_abortRules_switch(int32_t);//Use to switch the restart rule without re-initializing the CDCL.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void cdcl_abortRules_resetPlugin();
void cdcl_abortRules_initPlugin();
void cdcl_abortRules_disposePlugin();

#endif /* ABORTRULES_H_ */
