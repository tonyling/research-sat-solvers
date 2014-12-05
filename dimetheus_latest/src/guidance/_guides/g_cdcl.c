/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_cdcl_printHelp(){
	printf("c      %-3d: CDCL\n", GUIDE_CDCL);
    printf("c           Behavior: Perform conflict-driven clause learning.\n");
    printf("c           May use : CLASSIFY[ATTRIBUTOR RANDOMFOREST] ADAPT[ITEADAPTER] PREP[PRE SATELITE] SEARCH[CDCL]\n");
    printf("c           Enforces: -searchStrategy %d, -adaptSearchStrategy 0,\n"
    	   "c                     -inpStrategy %d, -adaptInpStrategy 0, -adaptInpStrategyParams 0,\n"
    	   "c                     -cdclSelectVarRule %d, -adaptCdclSVR 0,\n"
    	   "c                     -cdclAbortRule %d -adaptCdclAR 0\n",
    				   		      SEARCH_STRATEGY_CDCL,
    				   		      INP_STRATEGY_NULL,
    				   		      CDCL_SELECTVARRULE_RRVSIDS,
    				   		      CDCL_ABORTRULE_NULL);
    printf("c           Forbids : Modification of the enforced parameters. Adaptation of the enforced parameters.\n");

}

void guide_cdcl_apply(int argc, char** argv){
	//This guide will enforce conflict-driven clause learning.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif

	//First, acquire all parameter settings and adaptation capabilities.
	guidance_intern_getAllParams(argc, argv);

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Enforcing and setting defaults...\n");
	#endif

	//Second, apply default values, if possible.
	//CLASSIFY
	GUIDANCE_APPLYDEFAULT(guidance_classifyStrategy, param_classifyStrategy, CLASSIFY_STRATEGY_RANDFOR);

	//ADAPT
	GUIDANCE_APPLYDEFAULT(guidance_adaptStrategy, param_adaptStrategy, ADAPT_STRATEGY_ITEGENERIC);

	//Third, check and enforce parameter settings for this guide.
	//INP
	GUIDANCE_CHECKANDSET(guidance_inpStrategy, param_inpStrategy, INP_STRATEGY_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategy, param_adaptInpStrategy, 0);
	GUIDANCE_CHECKANDSET(guidance_adaptInpStrategyParams, param_adaptInpStrategyParams, 0);

	//SEARCH
	GUIDANCE_CHECKANDSET(guidance_searchStrategy, param_searchStrategy, SEARCH_STRATEGY_CDCL);
	GUIDANCE_CHECKANDSET(guidance_adaptSearchStrategy, param_adaptSearchStrategy, 0);

	//CDCL
	GUIDANCE_CHECKANDSET(guidance_cdclSelectVarRule, param_cdclSelectVarRule, CDCL_SELECTVARRULE_RRVSIDS);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclSVR, param_adaptCdclSVR, 0);

	GUIDANCE_CHECKANDSET(guidance_cdclAbortRule, param_cdclAbortRule, CDCL_ABORTRULE_NULL);
	GUIDANCE_CHECKANDSET(guidance_adaptCdclAR, param_adaptCdclAR, 0);

	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guide_cdcl_enforceAfterAdaptation(){
	param_inpStrategy = INP_STRATEGY_NULL;
	param_searchStrategy = SEARCH_STRATEGY_CDCL;
	param_cdclSelectVarRule = CDCL_SELECTVARRULE_RRVSIDS;
	param_cdclAbortRule = CDCL_ABORTRULE_NULL;
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE:   inpStrategy = INP_STRATEGY_NULL\n");
	printf("c     GUIDANCE:   searchStrategy = SEARCH_STRATEGY_CDCL\n");
	printf("c     GUIDANCE:   cdclSelectVarRule = CDCL_SELECTVARRULE_RRVSIDS\n");
	printf("c     GUIDANCE:   cdclAbortRule = CDCL_ABORTRULE_NULL\n");
	#endif
}
