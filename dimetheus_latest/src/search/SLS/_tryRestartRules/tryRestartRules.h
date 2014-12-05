/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef TRYRESTARTRULES_H_
#define TRYRESTARTRULES_H_

#include "../sls.h"

#define SLS_TRYRESTARTRULE_MINVALUE 0
#define SLS_TRYRESTARTRULE_MAXVALUE 4

#define SLS_TRYRESTARTRULE_SINGLEUNLIMITED 0	//Perform a single try with unlimited flips.
#include "trr_singleunlimited.h"

#define SLS_TRYRESTARTRULE_CONST 1				//Perform a fixed number of tries with a constant number of flips.
#include "trr_const.h"

#define SLS_TRYRESTARTRULE_LINEAR 2				//Perform a fixed number of tries with linearly increasing number of flips.
#include "trr_linear.h"

#define SLS_TRYRESTARTRULE_SQRT 3				//Perform a fixed number of tries with square root increasing number of flips.
#include "trr_sqrt.h"

#define SLS_TRYRESTARTRULE_EXP 4				//Perform a fixed number of tries with exponential increasing number of flips.
#include "trr_exp.h"

uint64_t sls_maxNumFlips;				//The number of flips per try that are allowed. Zero for unlimited.
uint64_t sls_maxNumTries;				//The number of tries that are allowed. Zero for unlimited.

uint64_t sls_numFlips;					//The number of flips made.
uint64_t sls_numTries;					//The number of tries made.

static inline void sls_tryRestartRules_printHelp(){
	sls_tryRestartRule_singleunlimited_printHelp();
	sls_tryRestartRule_const_printHelp();
	sls_tryRestartRule_linear_printHelp();
	sls_tryRestartRule_sqrt_printHelp();
	sls_tryRestartRule_exp_printHelp();
}

//Extern interface methods -- can be called by the one who initializes the component.
static inline void sls_extern_tryRestartRules_printNamesAndIDs(char* prefix){
	printf("%s SLS-TRYRESTART-RULES: SLS_TRYRESTARTRULE_SINGLEUNLIMITED %d\n", prefix, SLS_TRYRESTARTRULE_SINGLEUNLIMITED);
	printf("%s SLS-TRYRESTART-RULES: SLS_TRYRESTARTRULE_CONST %d\n", prefix, SLS_TRYRESTARTRULE_CONST);
	printf("%s SLS-TRYRESTART-RULES: SLS_TRYRESTARTRULE_LINEAR %d\n", prefix, SLS_TRYRESTARTRULE_LINEAR);
	printf("%s SLS-TRYRESTART-RULES: SLS_TRYRESTARTRULE_SQRT %d\n", prefix, SLS_TRYRESTARTRULE_SQRT);
	printf("%s SLS-TRYRESTART-RULES: SLS_TRYRESTARTRULE_EXP %d\n", prefix, SLS_TRYRESTARTRULE_EXP);
}
void (*sls_tryRestartRule_prepare)();	//This points to one of the try restart rules call preparation methods available.
void (*sls_tryRestartRule_prepareTry)();//This points to one of the try restart rules re-initializations that are available.

void sls_extern_tryRestartRules_switch(int32_t);//Use to switch the flip rule without re-initializing the whole SLS module.

//Interface Methods for initializing the component -- called by the one who wants to use the above methods.
void sls_tryRestartRules_resetPlugin();
void sls_tryRestartRules_initPlugin();
void sls_tryRestartRules_disposePlugin();

#endif /* TRYRESTARTRULES_H_ */
