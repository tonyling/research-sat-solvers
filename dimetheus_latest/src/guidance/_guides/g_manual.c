/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_manual_printHelp(){
	printf("c      %-3d: MANUAL\n", GUIDE_MANUAL);
    printf("c           Behavior: Allows you to set everything yourself (might brake the algorithm).\n");
    printf("c           May use : Any phase and module.\n");
    printf("c           Enforces: Nothing.\n");
    printf("c           Forbids : Nothing.\n");
}

void guide_manual_apply(int argc, char** argv){
	//The manual guide is doing nothing for you -- you have to do all the parameter and strategy settings manually :-)
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif

	guidance_intern_printGuidanceResult();

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Enforcing and setting defaults...\n");
	#endif

	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_manual_enforceAfterAdaptation(){

	#ifdef VERBOSE_GUIDANCE

	#endif
}
