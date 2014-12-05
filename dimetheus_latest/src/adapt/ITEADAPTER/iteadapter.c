/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "iteadapter.h"

void iteadapter_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_iteadapter_component_totalCalls;
	#endif
	#ifdef VERBOSE_ITEADAPTER
	printf("c     ITEADAPTER: Component reset...\n");
	#endif
	iteadapter_returnCode = ITEADAPTER_UNKNOWN;
	#ifdef COLLECTSTATS
	stats_iteadapter_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void iteadapter_initModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_iteadapter_component_totalCalls;
	#endif
	#ifdef VERBOSE_ITEADAPTER
	printf("c     ITEADAPTER: Component init...\n");
	#endif
	iteadapter_returnCode = ITEADAPTER_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_iteadapter_time_init += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	stats_iteadapter_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void iteadapter_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_iteadapter_component_totalCalls;
	#endif
	#ifdef VERBOSE_ITEADAPTER
	printf("c     ITEADAPTER: Component dispose...\n");
	#endif

	iteadapter_returnCode = ITEADAPTER_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_iteadapter_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void iteadapter_extern_printVersion(){
	printf("[Adaptation      ] [Parameter adapter              ] :: %3d.%-4d :: %s",
			ITEADAPTER_VERSION_MA, ITEADAPTER_VERSION_MI, ITEADAPTER_VERSION_NAME);
}

void iteadapter_extern_adaptAllAllowed(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_iteadapter_component_totalCalls;
	#endif
	iteadapter_intern_generic_adaptAll();

	#ifdef COLLECTSTATS
	++stats_iteadapter_component_totalCalls;
	stats_iteadapter_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
