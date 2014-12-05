/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "classifyStrategies.h"

void classify_strategy_attonly_printHelp(){
	printf("c      %-3d: ATTONLY:\n", CLASSIFY_STRATEGY_ATTONLY);
    printf("c           Behavior: Just computes the formula attributes using CLASS but does not classify the formula.\n");
}

void classify_strategy_attonly_reset(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy reset (ATTONLY)...\n");
	#endif

	attributor_resetModule();
}

void classify_strategy_attonly_init(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy init (ATTONLY)...\n");
	#endif

	attributor_initModule();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN){
		classify_returnCode = CLASSIFY_ERROR;
		return;
	}
}

void classify_strategy_attonly_dispose(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy dispose (ATTONLY)...\n");
	#endif
	attributor_disposeModule();
}

void classify_strategy_attonly_execute(){
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy execute (ATTONLY)...\n");
	#endif
	attributor_extern_computeAttributes();
	if (attributor_returnCode != ATTRIBUTOR_UNKNOWN) {classify_returnCode = CLASSIFY_ERROR; return;}
	classify_intern_setPrecision();
	#ifdef VERBOSE_CLASSIFY
	printf("c   CLASSIFY: Strategy done (ATTONLY).\n");
	#endif
}


