/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_testing_printHelp(){
	printf("c      %-3d: TESTING\n", GUIDE_TESTING);
    printf("c           Behavior: A special testing guide used to ease the development. Do not use.\n");
    printf("c           May use : Any phase and module.\n");
    printf("c           Enforces: Subject to changes. Do not use.\n");
    printf("c           Forbids : Subject to changes. Do not use.\n");
}

void guide_testing_apply(int argc, char** argv){
	//This is a testing guide that frequently changes. Do not use it.

	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif

	//Acquire all parameter settings and adaptation capabilities.
	guidance_intern_getAllParams(argc, argv);

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Enforcing and setting defaults...\n");
	#endif

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_testing_enforceAfterAdaptation(){

	#ifdef VERBOSE_GUIDANCE

	#endif
}
