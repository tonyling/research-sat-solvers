/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */


#include "prepStrategies.h"

void prep_strategy_satelite_printHelp(){
	printf("c      %-3d: SATELITE:\n", PREP_STRATEGY_SATELITE);
    printf("c           Behavior: Performs preprocessing according to SatELite (not yet implemented, do not use).\n");
}

void prep_strategy_satelite_reset(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy reset (SATELITE)...\n");
	#endif
	satelite_resetModule();
}

void prep_strategy_satelite_init(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy init (SATELITE)...\n");
	#endif
	satelite_initModule();
	if (satelite_returnCode != SATELITE_UNKNOWN){
		prep_returnCode = PREP_ERROR;
		return;
	}
}

void prep_strategy_satelite_dispose(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy dispose (SATELITE)...\n");
	#endif
	satelite_disposeModule();
}

void prep_strategy_satelite_execute(){
	#ifdef VERBOSE_PREP
	printf("c   PREP: Strategy execute (SATELITE)...\n");
	#endif

    //The strategy merely calls the SATELITE preprocessor.
	satelite_extern_simplify();

	if (satelite_returnCode == SATELITE_ERROR){
		prep_returnCode = PREP_ERROR;
	} else if (satelite_returnCode == SATELITE_SAT){
		prep_returnCode = PREP_SAT;
	} else if (satelite_returnCode == SATELITE_UNSAT){
		prep_returnCode = PREP_SAT;
	}
}
