/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "satelite.h"

void satelite_resetModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_satelite_component_totalCalls;
	#endif
	#ifdef VERBOSE_SATELITE
	printf("c     SATELITE: Component reset...\n");
	#endif
	satelite_returnCode = SATELITE_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_satelite_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}


void satelite_initModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_satelite_component_totalCalls;
	#endif
	#ifdef VERBOSE_SATELITE
	printf("c     SATELITE: Component init...\n");
	#endif
	satelite_returnCode = SATELITE_UNKNOWN;

	#ifdef COLLECTSTATS
	stats_satelite_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}


void satelite_disposeModule(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_satelite_component_totalCalls;
	#endif
	#ifdef VERBOSE_SATELITE
	printf("c     SATELITE: Component dispose...\n");
	#endif

	#ifdef COLLECTSTATS
	stats_satelite_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}


void satelite_extern_printVersion(){
	printf("[Preprocessing   ] [Preprocessor                   ] :: %3d.%-4d :: %s",
			SATELITE_VERSION_MA, SATELITE_VERSION_MI, SATELITE_VERSION_NAME);
}


void satelite_extern_simplify(){
	#ifdef VERBOSE_SATELITE
	printf("c     SATELITE: Simplify...\n");
	#endif
	printf("c     SATELITE: Warning: The SATELITE module is currently under development an must not be used.\n");
	printf("c Error. Do not use the SATELITE preprocessor.\n");
	satelite_returnCode = SATELITE_ERROR;
}
