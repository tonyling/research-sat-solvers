/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "adaptStrategies.h"

void adapt_strategy_itegeneric_printHelp(){
	printf("c      %-3d: ITEGENERIC:\n", ADAPT_STRATEGY_ITEGENERIC);
    printf("c           Behavior: Uses the classification information to adapt via the ITEADAPTER.\n");
    printf("c           Adaption: Can adapt all strategies and parameters (must be enabled for each strategy or parameter).\n");
}

void adapt_strategy_itegeneric_reset(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy reset (ITEGENERIC)...\n");
	#endif

}

void adapt_strategy_itegeneric_init(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy init (ITEGENERIC)...\n");
	#endif

}

void adapt_strategy_itegeneric_dispose(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy dispose (ITEGENERIC)...\n");
	#endif

}

void adapt_strategy_itegeneric_execute(){
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Strategy execute (ITEGENERIC)...\n");
	#endif
	if (!MAIN_GET_FATT_CLASS()){
		#ifdef VERBOSE_ADAPT
		printf("c   ADAPT: Adaptation not possible, since the formula has not been classified.\n");
		#endif
		return;
	}

	//Check what class we have.
	#ifdef VERBOSE_ADAPT
	printf("c   ADAPT: Adaptation based on class %d (",	MAIN_GET_FATT_CLASS());
	tools_cnfClasses_printClassName(MAIN_GET_FATT_CLASS());
	printf(") provided by the classifier.\n");
	#endif
	iteadapter_extern_adaptAllAllowed();
	if (iteadapter_returnCode != ITEADAPTER_UNKNOWN){
		printf("c ERROR. Adaptation failed.\n");
		adapt_returnCode = ADAPT_ERROR;
		return;
	}
}
