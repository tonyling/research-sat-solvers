/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "paramfuzzing.h"

int32_t params_getRandI(int32_t min, int32_t max){
	if (min == max) return min;
	return rand_intMinMax(min, max);
}

float_ty params_getRandF(float_ty min, float_ty max){
	if (min == max) return min;
	return rand_floatMinMax(min, max);
}

void params_extern_printFuzzedParametrs(char* prefix){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_params_component_totalCalls;
	#endif

	//Prepare the random number generator and print the prefix string.
	srand((uint32_t)time(NULL) + param_seed);
	rand_init();
	printf("%s ", prefix);

	//We either use random integers or floats, depending on the parameter type.
	uint32_t ri;
	float_ty rf;

	//First, we pick a preprocessing strategy. The preprocessing strategy is always independent of the rest.
	ri = params_getRandI(PARAM_FUZZMIN_PREPSTRATEGY, PARAM_FUZZMAX_PREPSTRATEGY);printf("-prepStrategy %d ", ri);
	if (ri == PREP_STRATEGY_NULL){
		//Nothing to pick here.
	} else if (ri == PREP_STRATEGY_SIMPLE){
		ri = params_getRandI(PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);printf("-prepUNHRounds %d ", ri);
	} else if (ri == PREP_STRATEGY_STRONG){
		ri = params_getRandI(PARAM_FUZZMIN_PREPTERNARYLITMAX, PARAM_FUZZMAX_PREPTERNARYLITMAX);printf("-prepTERNARYLitMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPTERNARYMAX, PARAM_FUZZMAX_PREPTERNARYMAX);printf("-prepTERNARYMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPNIVERMAX, PARAM_FUZZMAX_PREPNIVERMAX);printf("-prepNIVERMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPNHBRMIN, PARAM_FUZZMAX_PREPNHBRMIN);printf("-prepNHBRMin %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);printf("-prepUNHRounds %d ", ri);
	} else if (ri == PREP_STRATEGY_FULL){
		ri = params_getRandI(PARAM_FUZZMIN_PREPTERNARYLITMAX, PARAM_FUZZMAX_PREPTERNARYLITMAX);printf("-prepTERNARYLitMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPTERNARYMAX, PARAM_FUZZMAX_PREPTERNARYMAX);printf("-prepTERNARYMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPNIVERMAX, PARAM_FUZZMAX_PREPNIVERMAX);printf("-prepNIVERMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPNHBRMIN, PARAM_FUZZMAX_PREPNHBRMIN);printf("-prepNHBRMin %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPUNHROUNDS, PARAM_FUZZMAX_PREPUNHROUNDS);printf("-prepUNHRounds %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPGEMAXAONN, PARAM_FUZZMAX_PREPGEMAXAONN);printf("-prepGEMaxAONN %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPGEMAXXOR, PARAM_FUZZMAX_PREPGEMAXXOR);printf("-prepGEMaxXOR %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPACCESATURATE, PARAM_FUZZMAX_PREPACCESATURATE);printf("-prepACCESaturate %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPACCEMAX, PARAM_FUZZMAX_PREPACCEMAX);printf("-prepACCEMax %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPACCEMINCLSSIZE, PARAM_FUZZMAX_PREPACCEMINCLSSIZE);printf("-prepACCEMinClsSize %d ", ri);
		ri = params_getRandI(PARAM_FUZZMIN_PREPREVIVECLS, PARAM_FUZZMAX_PREPREVIVECLS);printf("-prepReviveCls %d ", ri);
	} else if (ri == PREP_STRATEGY_RAND){
		//Nothing to pick here.
	}

	//Second, we pick the guide. Everything else is based on it.
	ri = params_getRandI(PARAM_FUZZMIN_GUIDE, PARAM_FUZZMAX_GUIDE);printf("-guide %d ", ri);
    if (ri == GUIDE_CDCL){
   		rf = params_getRandF(PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);printf("-cdclSelectVarActIncBump %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_CDCLSELECTVARRANDPROB, PARAM_FUZZMAX_CDCLSELECTVARRANDPROB);printf("-cdclSelectVarRandProb %f ", rf);

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLSELECTVARACTINIT, PARAM_FUZZMAX_CDCLSELECTVARACTINIT);printf("-cdclSelectVarActInit %d ", ri);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSELECTDIRRULE, PARAM_FUZZMAX_CDCLSELECTDIRRULE);printf("-cdclSelectDirRule %d ", ri);
   		if (ri == CDCL_SELECTDIRRULE_PHASEFLIP){
   	   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSELECTDIRMINFLIPDIST, PARAM_FUZZMAX_CDCLSELECTDIRMINFLIPDIST);printf("-cdclSelectDirMinFlipDist %d ", ri);
   		}

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSELECTDIRDIRINIT, PARAM_FUZZMAX_CDCLSELECTDIRDIRINIT);printf("-cdclSelectDirDirInit %d ", ri);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTRLEARNEDRULE, PARAM_FUZZMAX_CDCLSTRLEARNEDRULE);printf("-cdclStrLearnedRule %d ", ri);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTROTHERRULE, PARAM_FUZZMAX_CDCLSTROTHERRULE);printf("-cdclStrOtherRule %d ", ri);

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTRULE, PARAM_FUZZMAX_CDCLRESTARTRULE);
    	printf("-cdclRestartRule %d ", ri);
    	if (ri == CDCL_RESTARTRULE_NULL){
    	} else if (ri == CDCL_RESTARTRULE_LUBY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    	} else if (ri == CDCL_RESTARTRULE_LUBYAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_LUBYSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTER){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC); printf("-cdclRestartMaxAgiOutFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCERULE, PARAM_FUZZMAX_CDCLMAINTENANCERULE);
    	printf("-cdclMaintenanceRule %d ", ri);
    	if (ri == CDCL_MAINTENANCERULE_NULL){
    	} else if (ri == CDCL_MAINTENANCERULE_INACTIVITY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBD){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACT){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACTLUBY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLINPROCESSINGRULE, PARAM_FUZZMAX_CDCLINPROCESSINGRULE);
    	printf("-cdclInprocessingRule %d ", ri);
    	if (ri == CDCL_INPROCESSINGRULE_NULL){
    	} else if (ri == CDCL_INPROCESSINGRULE_REDUCEONLY){
    	} else if (ri == CDCL_INPROCESSINGRULE_SIMPLE){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    	} else if (ri == CDCL_INPROCESSINGRULE_FULL){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXAONN, PARAM_FUZZMAX_CDCLIRGEMAXAONN); printf("-cdclIRGEMaxAONN %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXXOR, PARAM_FUZZMAX_CDCLIRGEMAXXOR); printf("-cdclIRGEMaxXOR %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRSTRMAXSIZE, PARAM_FUZZMAX_CDCLIRSTRMAXSIZE); printf("-cdclIRStrMaxSize %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRNIVERMAX, PARAM_FUZZMAX_CDCLIRNIVERMAX); printf("-cdclIRNIVERMax %d ", ri);
    	}
    } else if (ri == GUIDE_SLS){
    	ri = params_getRandI(PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE, PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE);
    	printf("-slsPickAndFlipVarRule %d ", ri);
    	if (ri == SLS_PICKANDFLIPVARRULE_PROBSATPB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT);printf("-slsProbsatCBShift %f ", rf);
    	} else if (ri == SLS_PICKANDFLIPVARRULE_PROBSATEB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_SLSTRYRESTARTRULE,PARAM_FUZZMAX_SLSTRYRESTARTRULE); printf("-slsTryRestartRule %d ", ri);
    	if (ri == SLS_TRYRESTARTRULE_CONST){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_LINEAR){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_SQRT){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_EXP){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	}

    } else if (ri == GUIDE_PMPCDCL){
    	ri = params_getRandI(PARAM_FUZZMIN_INPSTRATEGY,PARAM_FUZZMAX_INPSTRATEGY);	printf("-inpStrategy %d ", ri);
    	if (ri == INP_STRATEGY_NULL){
    	} else if (ri == INP_STRATEGY_REDUCEONLY){
    	} else if (ri == INP_STRATEGY_SIMPLE){
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYLITMAX,PARAM_FUZZMAX_INPTERNARYLITMAX); printf("-inpTernaryLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYMAX,PARAM_FUZZMAX_INPTERNARYMAX); printf("-inpTernaryMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPUNHROUNDS,PARAM_FUZZMAX_INPUNHROUNDS); printf("-inpUNHRounds %d ", ri);
    	} else if (ri == INP_STRATEGY_FULL){
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYLITMAX,PARAM_FUZZMAX_INPTERNARYLITMAX); printf("-inpTernaryLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYMAX,PARAM_FUZZMAX_INPTERNARYMAX); printf("-inpTernaryMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPUNHROUNDS,PARAM_FUZZMAX_INPUNHROUNDS); printf("-inpUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPGEMAXAONN,PARAM_FUZZMAX_INPGEMAXAONN); printf("-inpGEMaxAONN %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPGEMAXXOR,PARAM_FUZZMAX_INPGEMAXXOR); printf("-inpGEMaxXOR %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPSTRMAXSIZE,PARAM_FUZZMAX_INPSTRMAXSIZE); printf("-inpStrMaxSize %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPNIVERMAX,PARAM_FUZZMAX_INPNIVERMAX); printf("-inpNiverMax %d ", ri);
    	} else if (ri == INP_STRATEGY_RAND){
    	}
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE); printf("-searchMIDBlockSize %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR, PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR); printf("-searchImpactMaxFactor %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMINCLSIMPACT, PARAM_FUZZMAX_SEARCHMINCLSIMPACT); printf("-searchMinClsImpact %f ", rf);

    	rf = params_getRandF(PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO); printf("-mpRho %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA); printf("-mpSigma %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI); printf("-mpPhi %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON); printf("-mpUpsilon %f ", rf);

   		rf = params_getRandF(PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);printf("-cdclSelectVarActIncBump %f ", rf);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTRLEARNEDRULE, PARAM_FUZZMAX_CDCLSTRLEARNEDRULE);printf("-cdclStrLearnedRule %d ", ri);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTROTHERRULE, PARAM_FUZZMAX_CDCLSTROTHERRULE);printf("-cdclStrOtherRule %d ", ri);

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTRULE, PARAM_FUZZMAX_CDCLRESTARTRULE);
    	printf("-cdclRestartRule %d ", ri);
    	if (ri == CDCL_RESTARTRULE_NULL){
    	} else if (ri == CDCL_RESTARTRULE_LUBY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    	} else if (ri == CDCL_RESTARTRULE_LUBYAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_LUBYSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTER){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC); printf("-cdclRestartMaxAgiOutFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCERULE, PARAM_FUZZMAX_CDCLMAINTENANCERULE);
    	printf("-cdclMaintenanceRule %d ", ri);
    	if (ri == CDCL_MAINTENANCERULE_NULL){
    	} else if (ri == CDCL_MAINTENANCERULE_INACTIVITY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBD){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACT){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACTLUBY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLINPROCESSINGRULE, PARAM_FUZZMAX_CDCLINPROCESSINGRULE);
    	printf("-cdclInprocessingRule %d ", ri);
    	if (ri == CDCL_INPROCESSINGRULE_NULL){
    	} else if (ri == CDCL_INPROCESSINGRULE_REDUCEONLY){
    	} else if (ri == CDCL_INPROCESSINGRULE_SIMPLE){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    	} else if (ri == CDCL_INPROCESSINGRULE_FULL){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXAONN, PARAM_FUZZMAX_CDCLIRGEMAXAONN); printf("-cdclIRGEMaxAONN %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXXOR, PARAM_FUZZMAX_CDCLIRGEMAXXOR); printf("-cdclIRGEMaxXOR %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRSTRMAXSIZE, PARAM_FUZZMAX_CDCLIRSTRMAXSIZE); printf("-cdclIRStrMaxSize %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRNIVERMAX, PARAM_FUZZMAX_CDCLIRNIVERMAX); printf("-cdclIRNIVERMax %d ", ri);
    	}
    } else if (ri == GUIDE_PMPSLS){
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE); printf("-searchMIDBlockSize %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMAXMAGFORSLS, PARAM_FUZZMAX_SEARCHMAXMAGFORSLS); printf("-searchMaxMagForSLS %f ", rf);

    	rf = params_getRandF(PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO); printf("-mpRho %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA); printf("-mpSigma %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI); printf("-mpPhi %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON); printf("-mpUpsilon %f ", rf);

    	ri = params_getRandI(PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE, PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE);
    	printf("-slsPickAndFlipVarRule %d ", ri);
    	if (ri == SLS_PICKANDFLIPVARRULE_PROBSATPB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT);printf("-slsProbsatCBShift %f ", rf);
    	} else if (ri == SLS_PICKANDFLIPVARRULE_PROBSATEB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    	}

       	ri = params_getRandI(PARAM_FUZZMIN_SLSTRYRESTARTRULE,PARAM_FUZZMAX_SLSTRYRESTARTRULE);printf("-slsTryRestartRule %d ", ri);
       	ri = params_getRandI(PARAM_FUZZMIN_SLSMAXTRIES,PARAM_FUZZMAX_SLSMAXTRIES);printf("-slsMaxTries %d ", ri);
    	if (ri == SLS_TRYRESTARTRULE_CONST){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_LINEAR){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_SQRT){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_EXP){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	}
    } else if (ri == GUIDE_HYBRIDMSC){
    	ri = params_getRandI(PARAM_FUZZMIN_INPSTRATEGY,PARAM_FUZZMAX_INPSTRATEGY);	printf("-inpStrategy %d ", ri);
    	if (ri == INP_STRATEGY_NULL){
    	} else if (ri == INP_STRATEGY_REDUCEONLY){
    	} else if (ri == INP_STRATEGY_SIMPLE){
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYLITMAX,PARAM_FUZZMAX_INPTERNARYLITMAX); printf("-inpTernaryLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYMAX,PARAM_FUZZMAX_INPTERNARYMAX); printf("-inpTernaryMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPUNHROUNDS,PARAM_FUZZMAX_INPUNHROUNDS); printf("-inpUNHRounds %d ", ri);
    	} else if (ri == INP_STRATEGY_FULL){
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYLITMAX,PARAM_FUZZMAX_INPTERNARYLITMAX); printf("-inpTernaryLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPTERNARYMAX,PARAM_FUZZMAX_INPTERNARYMAX); printf("-inpTernaryMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPUNHROUNDS,PARAM_FUZZMAX_INPUNHROUNDS); printf("-inpUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPGEMAXAONN,PARAM_FUZZMAX_INPGEMAXAONN); printf("-inpGEMaxAONN %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPGEMAXXOR,PARAM_FUZZMAX_INPGEMAXXOR); printf("-inpGEMaxXOR %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPSTRMAXSIZE,PARAM_FUZZMAX_INPSTRMAXSIZE); printf("-inpStrMaxSize %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_INPNIVERMAX,PARAM_FUZZMAX_INPNIVERMAX); printf("-inpNiverMax %d ", ri);
    	} else if (ri == INP_STRATEGY_RAND){
    	}

    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMIDBLOCKSIZE, PARAM_FUZZMAX_SEARCHMIDBLOCKSIZE); printf("-searchMIDBlockSize %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHIMPACTMAXFACTOR, PARAM_FUZZMAX_SEARCHIMPACTMAXFACTOR); printf("-searchImpactMaxFactor %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMINCLSIMPACT, PARAM_FUZZMAX_SEARCHMINCLSIMPACT); printf("-searchMinClsImpact %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHRETREATFACT, PARAM_FUZZMAX_SEARCHRETREATFACT); printf("-searchRetreatFact %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMAXMAGFORSLS, PARAM_FUZZMAX_SEARCHMAXMAGFORSLS); printf("-searchMaxMagForSLS %f ", rf);

    	rf = params_getRandF(PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO); printf("-mpRho %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA); printf("-mpSigma %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI); printf("-mpPhi %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON); printf("-mpUpsilon %f ", rf);

    	ri = params_getRandI(PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE, PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE);
    	printf("-slsPickAndFlipVarRule %d ", ri);
    	if (ri == SLS_PICKANDFLIPVARRULE_PROBSATPB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT);printf("-slsProbsatCBShift %f ", rf);
    	} else if (ri == SLS_PICKANDFLIPVARRULE_PROBSATEB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    	}

        ri = params_getRandI(PARAM_FUZZMIN_SLSTRYRESTARTRULE,PARAM_FUZZMAX_SLSTRYRESTARTRULE); printf("-slsTryRestartRule %d ", ri);
       	ri = params_getRandI(PARAM_FUZZMIN_SLSMAXTRIES,PARAM_FUZZMAX_SLSMAXTRIES); printf("-slsMaxTries %d ", ri);
    	if (ri == SLS_TRYRESTARTRULE_CONST){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_LINEAR){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_SQRT){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	} else if (ri == SLS_TRYRESTARTRULE_EXP){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSB, PARAM_FUZZMAX_SLSMAXFLIPSB); printf("-slsMaxFlipsB %f ", rf);
    	}

   		rf = params_getRandF(PARAM_FUZZMIN_CDCLSELECTVARACTINCBUMP, PARAM_FUZZMAX_CDCLSELECTVARACTINCBUMP);printf("-cdclSelectVarActIncBump %f ", rf);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTRLEARNEDRULE, PARAM_FUZZMAX_CDCLSTRLEARNEDRULE);printf("-cdclStrLearnedRule %d ", ri);

   		ri = params_getRandI(PARAM_FUZZMIN_CDCLSTROTHERRULE, PARAM_FUZZMAX_CDCLSTROTHERRULE);printf("-cdclStrOtherRule %d ", ri);

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTRULE, PARAM_FUZZMAX_CDCLRESTARTRULE);
    	printf("-cdclRestartRule %d ", ri);
    	if (ri == CDCL_RESTARTRULE_NULL){
    	} else if (ri == CDCL_RESTARTRULE_LUBY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    	} else if (ri == CDCL_RESTARTRULE_LUBYAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_LUBYSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTLUBYUNIT, PARAM_FUZZMAX_CDCLRESTARTLUBYUNIT); printf("-cdclRestartLubyUnit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTER){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERAGILITY){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTVARAGIINCBUMP, PARAM_FUZZMAX_CDCLRESTARTVARAGIINCBUMP); printf("-cdclRestartVarAgiIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIFRAC); printf("-cdclRestartMaxAgiFrac %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTMAXAGIOUTFRAC, PARAM_FUZZMAX_CDCLRESTARTMAXAGIOUTFRAC); printf("-cdclRestartMaxAgiOutFrac %f ", rf);
    	} else if (ri == CDCL_RESTARTRULE_INNEROUTERSTAG){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINIT, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINIT); printf("-cdclRestartInnerouterInit %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTINNEROUTERINC, PARAM_FUZZMAX_CDCLRESTARTINNEROUTERINC); printf("-cdclRestartInnerouterInc %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLRESTARTSTAGMAXFRAC, PARAM_FUZZMAX_CDCLRESTARTSTAGMAXFRAC); printf("-cdclRestartStagMaxFrac %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCERULE, PARAM_FUZZMAX_CDCLMAINTENANCERULE);
    	printf("-cdclMaintenanceRule %d ", ri);
    	if (ri == CDCL_MAINTENANCERULE_NULL){
    	} else if (ri == CDCL_MAINTENANCERULE_INACTIVITY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBD){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACT){
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBASE, PARAM_FUZZMAX_CDCLMAINTENANCEBASE); printf("-cdclMaintenanceBase %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLMAINTENANCEBOOST, PARAM_FUZZMAX_CDCLMAINTENANCEBOOST); printf("-cdclMaintenanceBoost %d ", ri);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    	} else if (ri == CDCL_MAINTENANCERULE_LBDINACTLUBY){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINITIAL, PARAM_FUZZMAX_CDCLMAINTENANCEINITIAL); printf("-cdclMaintenanceInitial %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEINCREASE, PARAM_FUZZMAX_CDCLMAINTENANCEINCREASE); printf("-cdclMaintenanceIncrease %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEACTINCBUMP, PARAM_FUZZMAX_CDCLMAINTENANCEACTINCBUMP); printf("-cdclMaintenanceActIncBump %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMINDEV, PARAM_FUZZMAX_CDCLMAINTENANCEMINDEV); printf("-cdclMaintenanceMinDev %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLMAINTENANCEMAXAVG, PARAM_FUZZMAX_CDCLMAINTENANCEMAXAVG); printf("-cdclMaintenanceMaxAvg %f ", rf);
    	}

    	ri = params_getRandI(PARAM_FUZZMIN_CDCLINPROCESSINGRULE, PARAM_FUZZMAX_CDCLINPROCESSINGRULE);
    	printf("-cdclInprocessingRule %d ", ri);
    	if (ri == CDCL_INPROCESSINGRULE_NULL){
    	} else if (ri == CDCL_INPROCESSINGRULE_REDUCEONLY){
    	} else if (ri == CDCL_INPROCESSINGRULE_SIMPLE){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    	} else if (ri == CDCL_INPROCESSINGRULE_FULL){
    		rf = params_getRandF(PARAM_FUZZMIN_CDCLIRMINCONFDISTANCE, PARAM_FUZZMAX_CDCLIRMINCONFDISTANCE); printf("-cdclIRMinConfDistance %f ", rf);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRUNHROUNDS, PARAM_FUZZMAX_CDCLIRUNHROUNDS); printf("-cdclIRUNHRounds %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYLITMAX, PARAM_FUZZMAX_CDCLIRTERNARYLITMAX); printf("-cdclIRTERNARYLitMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRTERNARYMAX, PARAM_FUZZMAX_CDCLIRTERNARYMAX); printf("-cdclIRTERNARYMax %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXAONN, PARAM_FUZZMAX_CDCLIRGEMAXAONN); printf("-cdclIRGEMaxAONN %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRGEMAXXOR, PARAM_FUZZMAX_CDCLIRGEMAXXOR); printf("-cdclIRGEMaxXOR %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRSTRMAXSIZE, PARAM_FUZZMAX_CDCLIRSTRMAXSIZE); printf("-cdclIRStrMaxSize %d ", ri);
    		ri = params_getRandI(PARAM_FUZZMIN_CDCLIRNIVERMAX, PARAM_FUZZMAX_CDCLIRNIVERMAX); printf("-cdclIRNIVERMax %d ", ri);
    	}
    } else if (ri == GUIDE_EMP){
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMPWEIGHTMODSAT, PARAM_FUZZMAX_SEARCHMPWEIGHTMODSAT); printf("-searchMPWeightModSAT %f ", rf);
    	rf = params_getRandF(PARAM_FUZZMIN_SEARCHMPWEIGHTMODUNS, PARAM_FUZZMAX_SEARCHMPWEIGHTMODUNS); printf("-searchMPWeightModUNS %f ", rf);

    	rf = params_getRandF(PARAM_FUZZMIN_MPRHO, PARAM_FUZZMAX_MPRHO); printf("-mpRho %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPSIGMA, PARAM_FUZZMAX_MPSIGMA); printf("-mpSigma %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPPHI, PARAM_FUZZMAX_MPPHI); printf("-mpPhi %f ", rf);
        rf = params_getRandF(PARAM_FUZZMIN_MPUPSILON, PARAM_FUZZMAX_MPUPSILON); printf("-mpUpsilon %f ", rf);

    	ri = params_getRandI(PARAM_FUZZMIN_SLSPICKANDFLIPVARRULE, PARAM_FUZZMAX_SLSPICKANDFLIPVARRULE);
    	printf("-slsPickAndFlipVarRule %d ", ri);
    	if (ri == SLS_PICKANDFLIPVARRULE_PROBSATPB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCBSHIFT, PARAM_FUZZMAX_SLSPROBSATCBSHIFT);printf("-slsProbsatCBShift %f ", rf);
    	} else if (ri == SLS_PICKANDFLIPVARRULE_PROBSATEB){
    		rf = params_getRandF(PARAM_FUZZMIN_SLSPROBSATCB, PARAM_FUZZMAX_SLSPROBSATCB);printf("-slsProbsatCB %f ", rf);
    	}

        printf("-slsTryRestartRule 1 ");
       	printf("-slsMaxTries 1 ");
   		rf = params_getRandF(PARAM_FUZZMIN_SLSMAXFLIPSA, PARAM_FUZZMAX_SLSMAXFLIPSA); printf("-slsMaxFlipsA %f ", rf);
    }

	printf("\n");
	#ifdef COLLECTSTATS
	stats_params_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}
