/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "classifyStrategies.h"

void classify_strategy_randfor_printHelp(){
	printf("c      %-3d: RANDFOR:\n", CLASSIFY_STRATEGY_RANDFOR);
    printf("c           Behavior: Computes formula attributes and uses a random forest for classification.\n");
}

void classify_strategy_randfor_reset(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy reset (RANDFOR)...\n");
	#endif

	attributor_resetModule();
	randomforest_resetModule();
}

void classify_strategy_randfor_init(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy init (RANDFOR)...\n");
	#endif

	attributor_initModule();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN){
		classify_returnCode = CLASSIFY_ERROR;
		return;
	}
	randomforest_initModule();
	if (randomforest_returnCode != RANDOMFOREST_UNKNOWN){
		classify_returnCode = CLASSIFY_ERROR;
	}
}

void classify_strategy_randfor_dispose(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy dispose (RANDFOR)...\n");
	#endif
	attributor_disposeModule();
	randomforest_disposeModule();
}

void classify_strategy_randfor_execute(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy execute (RANDFOR)...\n");
	#endif

	attributor_extern_computeAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) {classify_returnCode = CLASSIFY_ERROR; return;}

	classify_intern_setPrecision();

	randomforest_extern_classify();
	if (randomforest_returnCode != RANDOMFOREST_UNKNOWN) {classify_returnCode = CLASSIFY_ERROR; return;}

	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy done (RANDFOR).\n");
	#endif
}

