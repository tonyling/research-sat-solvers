/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ASSINITRULES_H_
#define ASSINITRULES_H_

#include "../sls.h"

#define SLS_ASSINITRULE_MINVALUE 0
#define SLS_ASSINITRULE_MAXVALUE 2

#define SLS_ASSINITRULE_RANDOM 0			//Initialize starting assignment randomly.
#include "air_random.h"

#define SLS_ASSINITRULE_PROVIDED 1			//Use the provided phases for initializing the SLS starting assignment.
#include "air_provided.h"

#define SLS_ASSINITRULE_PROVIDEDTHENRAND 2	//Use the provided phases for first try, and afterwards use random initialization.
#include "air_providedthenrand.h"

static inline void sls_assInitRules_printHelp(){
	sls_assInitRule_random_printHelp();
	sls_assInitRule_provided_printHelp();
	sls_assInitRule_providedthenrand_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void sls_extern_assInitRules_printNamesAndIDs(char* prefix){
	printf("%s SLS-ASSINIT-RULES: SLS_ASSINITRULE_RANDOM %d\n", prefix, SLS_ASSINITRULE_RANDOM);
	printf("%s SLS-ASSINIT-RULES: SLS_ASSINITRULE_PROVIDED %d\n", prefix, SLS_ASSINITRULE_PROVIDED);
	printf("%s SLS-ASSINIT-RULES: SLS_ASSINITRULE_PROVIDEDTHENRAND %d\n", prefix, SLS_ASSINITRULE_PROVIDEDTHENRAND);
}
void (*sls_assInitRule_prepare)();	//This points to one of the initialization rules call preparation methods available.
void (*sls_assInitRule_ai)();		//This points to one of the initialization rules that are available.

void sls_extern_assInitRules_switch(int32_t);//Use to switch the initialization rule without re-initializing the SLS module.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void sls_assInitRules_resetPlugin();
void sls_assInitRules_initPlugin();
void sls_assInitRules_disposePlugin();

#endif /* ASSINITRULES_H_ */
