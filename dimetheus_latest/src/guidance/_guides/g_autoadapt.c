/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "../guidance.h"

void guide_autoadapt_printHelp(){
	printf("c      %-3d: AUTOADAPT\n", GUIDE_AUTOADAPT);
    printf("c           Behavior: Classify the formula and adapt as best as possible to solve it.\n");
    printf("c           May use : All phases and modules.\n");
    printf("c           Forbids : Using any parameter starting with -adapt, -prep, -inp, -search, -mp, -sls, or -cdcl,\n"
    	   "c                     except the allowed ones in the list below.\n");
    printf("c           Allows  : Using output parameters, like -prepOutput or -cdclOutput.\n"
    	   "c                     Using -classifyStrategy > %d and -adaptStrategy > %d.\n",
    	   	   	   	   	   	   	  CLASSIFY_STRATEGY_ATTONLY, ADAPT_STRATEGY_NULL);
}

void guide_autoadapt_apply(int argc, char** argv){
	//This guide will enforce that the adaptation picks the optimal solver behavior.
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif

	int32_t checkOK = 1, p = 0;

	//First, acquire all parameter settings and adaptation capabilities.
	guidance_intern_getAllParams(argc, argv);

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Enforcing and setting defaults...\n");
	#endif

	for (p = 1; p < argc; ++p){
		//Check, that the classifyStrategy, if provided at all, obeys the guide.
		if (guidance_intern_strStartsWith(argv[p], "-classifyStrategy")){
			if (param_classifyStrategy < CLASSIFY_STRATEGY_ATTONLY + 1){
				printf("c ERROR. You are not allowed to set classifyStrategy to %d.\n", param_classifyStrategy);
				checkOK = 0;
				break;
			}
			continue;
		}

		//Check, that the adaptStrategy, if provided at all, obeys the guide.
		if (guidance_intern_strStartsWith(argv[p], "-adaptStrategy")){
			if (param_adaptStrategy < ADAPT_STRATEGY_NULL + 1){
				printf("c ERROR. You are not allowed to set adaptStrategy to %d.\n", param_adaptStrategy);
				checkOK = 0;
				break;
			}
			continue;
		}

		//Ignore any output related parameters.
		if (guidance_intern_strStartsWith(argv[p], "-prepOutput")
				|| guidance_intern_strStartsWith(argv[p], "-cdclOutput")){
			continue;
		}

		//Check, that no remaining parameter starting with
		// -adapt
		// -prep
		// -inp
		// -search
		// -mp
		// -sls
		// -cdcl
		//is provided, because that must be figured out by the adapter used later.
		if (guidance_intern_strStartsWith(argv[p], "-adapt")){
			printf("c ERROR. You are not allowed to set the ADAPT parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-prep")){
			printf("c ERROR. You are not allowed to set the PREP parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-inp")){
			printf("c ERROR. You are not allowed to set the INP parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-search")){
			printf("c ERROR. You are not allowed to set the SEARCH parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-sls")){
			printf("c ERROR. You are not allowed to set the SLS parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-mp")){
			printf("c ERROR. You are not allowed to set the MP parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}

		if (guidance_intern_strStartsWith(argv[p], "-cdcl")){
			printf("c ERROR. You are not allowed to set the CDCL parameter %s.\n", argv[p]);
			checkOK = 0;
			break;
		}
	}

	if (checkOK){
		//Everything was provided correctly or left unprovided. Set some defaults, if still applicable.
		//CLASSIFY
		GUIDANCE_APPLYDEFAULT(guidance_classifyStrategy, param_classifyStrategy, CLASSIFY_STRATEGY_RANDFOR);
		//ADAPT
		GUIDANCE_APPLYDEFAULT(guidance_adaptStrategy, param_adaptStrategy, ADAPT_STRATEGY_ITEGENERIC);
	} else {
		//Something went wrong.
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS;
	}
	guidance_intern_printGuidanceResult();
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
