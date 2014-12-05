/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#include "guidance.h"

void guidance_resetPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: RESET...\n");
	#endif
	guidance_returnCode = GUIDANCE_UNKNOWN;
	guidance_extern_apply = &guide_autoadapt_apply;
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guidance_initPhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: INITIALIZE...\n");
	printf("c   GUIDANCE: Resetting values...\n");
	#endif
	guidance_returnCode = GUIDANCE_UNKNOWN;

	//CLASSIFY
	guidance_classifyStrategy = -1;

	//ADAPT
	guidance_adaptStrategy = -1;

	//PREP
	guidance_adaptPrepStrategy = -1;
		guidance_prepStrategy = -1;
	guidance_adaptPrepStrategyParams = -1,
		guidance_prepReviveCls = -1,
		guidance_prepUNHRounds = -1,
		guidance_prepACCEMax = -1,
		guidance_prepACCEMinClsSize = -1,
		guidance_prepACCESaturate = -1,
		guidance_prepNIVERMax = -1,
		guidance_prepATEMin = -1,
		guidance_prepGEMaxAONN = -1,
		guidance_prepGEMaxXOR = -1,
		guidance_prepTERNARYLitMax = -1,
		guidance_prepTERNARYMax = -1,
		guidance_prepNHBRMin = -1;

	//INP
	guidance_adaptInpStrategy = -1;
		guidance_inpStrategy = -1;
	guidance_adaptInpStrategyParams = -1;
		guidance_inpStrMaxSize = -1;
		guidance_inpTernaryLitMax = -1;
		guidance_inpTernaryMax = -1;
		guidance_inpNiverMax = -1;
		guidance_inpGEMaxAONN = -1;
		guidance_inpGEMaxXOR = -1;
		guidance_inpNHBRMin = -1;
		guidance_inpUNHRounds = -1;

	//SEARCH
	guidance_adaptSearchStrategy = -1;
		guidance_searchStrategy = -1;
	guidance_adaptSearchStrategyParams = -1;
		guidance_searchMIDBlockSize = -1;
		guidance_searchMinClsImpact = -1;
		guidance_searchImpactMaxFactor = -1;
		guidance_searchMaxMagForSLS = -1;
		guidance_searchRetreatFact = -1;
		guidance_searchMPWeightModSAT = -1;
		guidance_searchMPWeightModUNS = -1;

	//HANDLERESULT
	//Nothing to initialize yet.

	//SHUTDOWN
	//Nothing to initialize yet.

	//DETERM
	//Nothing to initialize yet.

	//SPECCER
	//Nothing to initialize yet.

	//LOADER
	//Nothing to initialize yet.

	//ATTRIBUTOR
	//Nothing to initialize yet.

	//RANDOMFOREST
	//Nothing to initialize yet.

	//ITEADAPTER
	//Nothing to initialize yet.

	//PRE
	//Nothing to initialize yet.

	//SATELITE
	//Nothing to initialize yet.

	//IN
	//Nothing to initialize yet.

	//MP
	//MP Update Rule
	guidance_adaptMpUR = -1;
		guidance_mpUpdateRule = -1;
	guidance_adaptMpURParams = -1;
		guidance_mpRho = -1;
		guidance_mpSigma = -1;
		guidance_mpPhi = -2,
		guidance_mpUpsilon = -1,
		guidance_mpMaxConvergenceDiff = -1;

	//SLS
	//SLS Assignment Init Rule
	guidance_adaptSlsAIR = -1;
		guidance_slsAssInitRule = -1;
	guidance_adaptSlsAIRParams = -1;
		//Currently, there are no parameters.
	//SLS Try Restart Rule
	guidance_adaptSlsTRR = -1;
		guidance_slsTryRestartRule = -1;
	guidance_adaptSlsTRRParams = -1;
		guidance_slsMaxFlipsA = -1;
		guidance_slsMaxFlipsB = -1;
		guidance_slsMaxTries = -1;
	//SLS Pick and Flip Variable Rule
	guidance_adaptSlsPAFVR = -1;
		guidance_slsPickAndFlipVarRule = -1;
	guidance_adaptSlsPAFVRParams = -1;
		guidance_slsNoise = -1;
		guidance_slsProbsatCB = -1;
		guidance_slsProbsatCBShift = -1;

	//CDCL
	//CDCL Restart Rule
	guidance_adaptCdclRR = -1;
		guidance_cdclRestartRule = -1;
	guidance_adaptCdclRRParams = -1;
		guidance_cdclRestartInnerouterInit = -1;
		guidance_cdclRestartLubyUnit = -1;
		guidance_cdclRestartStagMaxFrac = -1;
		guidance_cdclRestartInnerouterInc = -1;
		guidance_cdclRestartVarAgiIncBump = -1;
		guidance_cdclRestartMaxAgiFrac = -1;
		guidance_cdclRestartMaxAgiOutFrac = -1;

	//CDCL Select Variable Rule (with activity initialization)
	guidance_adaptCdclSVR = -1;
		guidance_cdclSelectVarRule = -1;
	guidance_adaptCdclSVRParams = -1;
		guidance_cdclSelectVarActIncBump = -1;
		guidance_cdclSelectVarRandProb = -1;
	guidance_adaptCdclSVAI = -1;
		guidance_cdclSelectVarActInit = -1;
	guidance_adaptCdclSVAIParams = -1;
		//Currently, there are no parameters.

	//CDCL Select Direction Rule (with direction initialization)
	guidance_adaptCdclSDR = -1;
		guidance_cdclSelectDirRule = -1;
	guidance_adaptCdclSDRParams = -1;
		guidance_cdclSelectDirMinFlipDist = -1;
	guidance_adaptCdclSDDI = -1;
		guidance_cdclSelectDirDirInit = -1;
	guidance_adaptCdclSDDIParams = -1;
		//Currently, there are no parameters.

	//CDCL Conflict Analysis Rule
	guidance_adaptCdclCAR = -1;
		guidance_cdclConflictAnalysisRule = -1;
	guidance_adaptCdclCARParams = -1;
		//Currently, there are no parameters.

	//CDCL Maintenance Rule
	guidance_adaptCdclMR = -1;
		guidance_cdclMaintenanceRule = -1;
	guidance_adaptCdclMRParams = -1;
		guidance_cdclMaintenanceBase = -1;
		guidance_cdclMaintenanceBoost = -1;
		guidance_cdclMaintenanceMinDev = -1;
		guidance_cdclMaintenanceMaxAvg = -1;
		guidance_cdclMaintenanceActIncBump = -1;
		guidance_cdclMaintenanceInitial = -1;
		guidance_cdclMaintenanceIncrease = -1;

	//CDCL Inprocessing Rule
	guidance_adaptCdclIR = -1;
		guidance_cdclInprocessingRule = -1;
	guidance_adaptCdclIRParams = -1;
	    guidance_cdclIRUNHRounds = -1;
		guidance_cdclIRTERNARYLitMax = -1;
		guidance_cdclIRTERNARYMax = -1;
		guidance_cdclIRNIVERMax = -1;
		guidance_cdclIRGEMaxAONN = -1;
		guidance_cdclIRGEMaxXOR = -1;
		guidance_cdclIRStrMaxSize = -1;
		guidance_cdclIRNHBRMin = -1;
		guidance_cdclIRMinConfDistance = -1;

	//CDCL Strengthen Learned Rule
	guidance_adaptCdclSLR = -1;
		guidance_cdclStrLearnedRule = -1;
	guidance_adaptCdclSLRParams = -1;
		//Currently, there are no parameters.

	//CDCL Strengthen Other Rule
	guidance_adaptCdclSOR = -1;
		guidance_cdclStrOtherRule = -1;
	guidance_adaptCdclSORParams = -1;
		//Currently, there are no parameters.

	//CDCL Compute BJ Level Rule
	guidance_adaptCdclCBJLR = -1;
		guidance_cdclComputeBJLRule = -1;
	guidance_adaptCdclCBJLRParams = -1;
		//Currently, there are no parameters.

	//CDCL Abort Rule
	guidance_adaptCdclAR = -1;
		guidance_cdclAbortRule = -1;
	guidance_adaptCdclARParams = -1;
		//Currently, there are no parameters.
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Setting pointer to... ");
	#endif
	if (param_guide == GUIDE_MANUAL){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_manual_apply;
	} else if (param_guide == GUIDE_AUTOADAPT){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_autoadapt_apply;
	} else if (param_guide == GUIDE_ATTONLY){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_attonly_apply;
	} else if (param_guide == GUIDE_RANDFORONLY){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_randforonly_apply;
	} else if (param_guide == GUIDE_PREPONLY){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_preponly_apply;
	} else if (param_guide == GUIDE_CDCL){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_cdcl_apply;
	} else if (param_guide == GUIDE_SLS){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_sls_apply;
	} else if (param_guide == GUIDE_PMPCDCL){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_pmpcdcl_apply;
	} else if (param_guide == GUIDE_PMPSLS){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_pmpsls_apply;
	} else if (param_guide == GUIDE_HYBRIDMSC){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_hybridmsc_apply;
	} else if (param_guide == GUIDE_EMP){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_emp_apply;
	} else if (param_guide == GUIDE_TESTING){
		#ifdef VERBOSE_GUIDANCE
		guidance_extern_printNameOfGuide(param_guide);
		#endif
		guidance_extern_apply = &guide_testing_apply;
	} else {
		#ifdef VERBOSE_GUIDANCE
		printf("\n");
		#endif
		printf("c ERROR. Initialization for guide %d failed. No such guide.\n", param_guide);
		guidance_returnCode = GUIDANCE_ERROR;
		return;
	}
	#ifdef VERBOSE_GUIDANCE
	printf("\n");
	#endif
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guidance_disposePhase(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: DISPOSE...\n");
	#endif
	guidance_returnCode = GUIDANCE_UNKNOWN;
	guidance_extern_apply = &guide_autoadapt_apply;
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

void guidance_extern_enforceGuideAfterAdaptation(){
	#ifdef COLLECTSTATS
	float_ty time_start = STATS_GET_CURRENT_TIME_IN_SEC();
	++stats_guidance_component_totalCalls;
	#endif
	#ifdef VERBOSE_GUIDANCE
	printf("c     GUIDANCE: Enforcing remaining settings after adaptation for guide %d (", param_guide);
	guidance_extern_printNameOfGuide(param_guide);
	printf(")\n");
	#endif

	if (param_guide == GUIDE_MANUAL){
		guide_manual_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_ATTONLY){
		guide_attonly_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_RANDFORONLY){
		guide_randforonly_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_PREPONLY){
		guide_preponly_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_CDCL){
		guide_cdcl_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_SLS){
		guide_sls_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_PMPCDCL){
		guide_pmpcdcl_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_PMPSLS){
		guide_pmpsls_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_HYBRIDMSC){
		guide_hybridmsc_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_EMP){
		guide_emp_enforceAfterAdaptation();
	} else if (param_guide == GUIDE_TESTING){
		guide_testing_enforceAfterAdaptation();
	}
	#ifdef COLLECTSTATS
	stats_guidance_time_total += STATS_GET_CURRENT_TIME_IN_SEC() - time_start;
	#endif
}

int32_t guidance_intern_strStartsWith(const char *a, const char *b){
	if(strncmp(a, b, strlen(b)) == 0) return 1;
	return 0;
}

void guidance_intern_getAllParams(int argc, char** argv){
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Determining parameter settings and adaptation capabilities...\n");
	#endif
	//Acquire phase parameters and adaptation capabilities.
	guidance_intern_getResetParams(argc, argv);
	guidance_intern_getParamsParams(argc, argv);
	guidance_intern_getStartupParams(argc, argv);
	guidance_intern_getClassifyParams(argc, argv);
	guidance_intern_getAdaptParams(argc, argv);
	guidance_intern_getPrepParams(argc, argv);
	guidance_intern_getInpParams(argc, argv);
	guidance_intern_getSearchParams(argc, argv);
	guidance_intern_getHandleresultParams(argc, argv);
	guidance_intern_getShutdownParams(argc, argv);

	//Acquire module parameters and adaptation capabilities.
	guidance_intern_getDETERMParams(argc, argv);
	guidance_intern_getSPECCERParams(argc, argv);
	guidance_intern_getLOADERParams(argc, argv);
	guidance_intern_getATTRIBUTORParams(argc, argv);
	guidance_intern_getRANDOMFORESTParams(argc, argv);
	guidance_intern_getITEADAPTERParams(argc, argv);
	guidance_intern_getPREParams(argc, argv);
	guidance_intern_getSATELITEParams(argc, argv);
	guidance_intern_getINParams(argc, argv);
	guidance_intern_getMPParams(argc, argv);
	guidance_intern_getSLSParams(argc, argv);
	guidance_intern_getCDCLParams(argc, argv);
	guidance_intern_getSOLWRITERParams(argc, argv);
}

void guidance_intern_printGuidanceResult(){
	//In case something went wrong, we can stop right away.
	if (guidance_returnCode == GUIDANCE_CONFLICTINGPARAMS){
		printf("c ERROR. The selected guide is %d (", param_guide);guidance_extern_printNameOfGuide(param_guide);printf(").\n");
		printf("c ERROR. Parameter settings are in conflict with the guide selected. See the help (call with -h) for details.\n");
		return;
	}

	//Otherwise, we output the success news and the settings for the used modules and phases.
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE: Guide %d [", param_guide);
	guidance_extern_printNameOfGuide(param_guide);
	printf("] successfully enforced.\n");
	printf("c   GUIDANCE: Presenting parameter settings and adaptation capabilities...\n");

	if (param_guide == GUIDE_MANUAL || param_guide == GUIDE_AUTOADAPT || param_guide == GUIDE_TESTING){
		guidance_intern_outputResetSetting();
		guidance_intern_outputParamsSetting();
		guidance_intern_outputStartupSetting();
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputInpSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputDETERMSetting();
		guidance_intern_outputSPECCERSetting();
		guidance_intern_outputLOADERSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputINSetting();
		guidance_intern_outputMPSetting();
		guidance_intern_outputSLSSetting();
		guidance_intern_outputCDCLSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
		guidance_intern_outputShutdownSetting();
	} else if (param_guide == GUIDE_ATTONLY){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputATTRIBUTORSetting();
	} else if (param_guide == GUIDE_CDCL){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputCDCLSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_HYBRIDMSC){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputInpSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputINSetting();
		guidance_intern_outputMPSetting();
		guidance_intern_outputSLSSetting();
		guidance_intern_outputCDCLSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_PMPCDCL){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputInpSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputINSetting();
		guidance_intern_outputMPSetting();
		guidance_intern_outputCDCLSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_PMPSLS){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputMPSetting();
		guidance_intern_outputSLSSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_EMP){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputInpSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputMPSetting();
		guidance_intern_outputSLSSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_PREPONLY){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputDETERMSetting();
		guidance_intern_outputSPECCERSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	} else if (param_guide == GUIDE_RANDFORONLY){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
	} else if (param_guide == GUIDE_SLS){
		guidance_intern_outputClassifySetting();
		guidance_intern_outputAdaptSetting();
		guidance_intern_outputPrepSetting();
		guidance_intern_outputSearchSetting();
		guidance_intern_outputATTRIBUTORSetting();
		guidance_intern_outputRANDOMFORESTSetting();
		guidance_intern_outputITEADAPTERSetting();
		guidance_intern_outputPRESetting();
		guidance_intern_outputSATELITESetting();
		guidance_intern_outputSLSSetting();
		guidance_intern_outputHandleresultSetting();
		guidance_intern_outputSOLWRITERSetting();
	}

	#endif
}

//PHASES and their parameters.
void guidance_intern_getResetParams(int argc, char** argv){
	//This method will read all RESET related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   RESET phase\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputResetSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for RESET.
	printf("c   GUIDANCE:   RESET current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getParamsParams(int argc, char** argv){
	//This method will read all PARAMS related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   PARAMS phase\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputParamsSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for PARAMS.
	printf("c   GUIDANCE:   PARAMS current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getStartupParams(int argc, char** argv){
	//This method will read all STARTUP related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.
	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   STARTUP phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptStartup")
				&& !guidance_intern_strStartsWith(argv[parameter], "-startup"))
				|| guidance_intern_strStartsWith(argv[parameter], "-startupOutput")
				|| !strcmp("-startupInputFormat",argv[parameter])) continue;

		printf("c ERROR. GUIDANCE did not recognize parameter %s in STARTUP.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputStartupSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for STARTUP.
	printf("c   GUIDANCE:   STARTUP current parameter setting.\n");
	printf("c   GUIDANCE:     startupInputFormat %d\n", param_startupInputFormat);
}
#endif

void guidance_intern_getClassifyParams(int argc, char** argv){
	//This method will read all CLASSIFY related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	//We must ignore -classifyInputDomain, if we ever want to add something here.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   CLASSIFY phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptClassify")
				&& !guidance_intern_strStartsWith(argv[parameter], "-classify"))
				|| guidance_intern_strStartsWith(argv[parameter], "-classifyOutput")
				|| !strcmp("-classifyInputDomain",argv[parameter])
				|| !strcmp("-classifyAttSetTime",argv[parameter])
				|| !strcmp("-classifyFloatPrec",argv[parameter])
				|| !strcmp("-classifyMaxVGSize",argv[parameter])
				|| !strcmp("-classifyMaxCGSize",argv[parameter])
				|| !strcmp("-classifyMaxUPImps",argv[parameter])
				|| !strcmp("-classifyMaxUPn",argv[parameter])
				|| !strcmp("-classifyPGFA",argv[parameter])
				|| !strcmp("-classifyPGFB",argv[parameter])
				|| !strcmp("-classifyPGFC",argv[parameter])
				|| !strcmp("-classifySimplify",argv[parameter])) continue;

		if (!strcmp("-classifyStrategy",argv[parameter]))			{guidance_classifyStrategy = param_classifyStrategy;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in CLASSIFY.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputClassifySetting(){
	//This method outputs the adaptation capabilities and enforced parameters for CLASSIFY.
	printf("c   GUIDANCE:   CLASSIFY current parameter setting.\n");
	printf("c   GUIDANCE:     classifyInputDomain %d\n", param_classifyInputDomain);
	printf("c   GUIDANCE:     classifyAttSetTime %f\n", param_classifyAttSetTime);
	printf("c   GUIDANCE:     classifyFloatPrec %d\n", param_classifyFloatPrec);
	printf("c   GUIDANCE:     classifyMaxVGSize %d\n", param_classifyMaxVGSize);
	printf("c   GUIDANCE:     classifyMaxGCSize %d\n", param_classifyMaxCGSize);
	printf("c   GUIDANCE:     classifyMaxUPImps %d\n", param_classifyMaxUPImps);
	printf("c   GUIDANCE:     classifyMaxUPn %d\n", param_classifyMaxUPn);
	printf("c   GUIDANCE:     classifyPGFA %f\n", param_classifyPGFA);
	printf("c   GUIDANCE:     classifyPGFB %f\n", param_classifyPGFB);
	printf("c   GUIDANCE:     classifyPGFC %f\n", param_classifyPGFC);
	printf("c   GUIDANCE:     classifySimplify %d\n", param_classifySimplify);
	printf("c   GUIDANCE:     classifyStrategy %d\n", param_classifyStrategy);
}
#endif


void guidance_intern_getAdaptParams(int argc, char** argv){
	//This method will read all ADAPT related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   ADAPT phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptAdapt")
				&& !guidance_intern_strStartsWith(argv[parameter], "-adapt"))
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptOutput")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptPrep")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptInp")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptSearch")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptDeterm")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptSpeccer")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptLoader")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptMp")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptSls")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptCdcl")
				|| guidance_intern_strStartsWith(argv[parameter], "-adaptSolwriter")) continue;

		if (!strcmp("-adaptStrategy",argv[parameter]))				{guidance_adaptStrategy = param_classifyStrategy;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in ADAPT.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputAdaptSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for ADAPT.
	printf("c   GUIDANCE:   ADAPT current parameter setting.\n");
	printf("c   GUIDANCE:     adaptStrategy %d\n", param_adaptStrategy);
}
#endif

void guidance_intern_getPrepParams(int argc, char** argv){
	//This method will read all PREP related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   PREP phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptPrep")
				&& !guidance_intern_strStartsWith(argv[parameter], "-prep"))
				|| guidance_intern_strStartsWith(argv[parameter], "-prepOutput")) continue;

		if (!strcmp("-adaptPrepStrategy",argv[parameter]))			{guidance_adaptPrepStrategy = param_adaptPrepStrategy;continue;}
		if (!strcmp("-prepStrategy",argv[parameter]))				{guidance_prepStrategy = param_prepStrategy;continue;}
		if (!strcmp("-adaptPrepStrategyParams",argv[parameter]))	{guidance_adaptPrepStrategyParams = param_adaptPrepStrategyParams;continue;}
		if (!strcmp("-prepReviveCls",argv[parameter]))				{guidance_prepReviveCls = param_prepReviveCls;continue;}
		if (!strcmp("-prepUNHRounds",argv[parameter]))				{guidance_prepUNHRounds = param_prepUNHRounds;continue;}
		if (!strcmp("-prepACCEMax",argv[parameter]))				{guidance_prepACCEMax = param_prepACCEMax;continue;}
		if (!strcmp("-prepACCEMinClsSize",argv[parameter]))			{guidance_prepACCEMinClsSize = param_prepACCEMinClsSize;continue;}
		if (!strcmp("-prepACCESaturate",argv[parameter]))			{guidance_prepACCESaturate = param_prepACCESaturate;continue;}
		if (!strcmp("-prepNIVERMax",argv[parameter]))				{guidance_prepNIVERMax = param_prepNIVERMax;continue;}
		if (!strcmp("-prepATEMin",argv[parameter]))					{guidance_prepATEMin = param_prepATEMin;continue;}
		if (!strcmp("-prepGEMaxAONN",argv[parameter]))				{guidance_prepGEMaxAONN = param_prepGEMaxAONN;continue;}
		if (!strcmp("-prepGEMaxXOR",argv[parameter]))				{guidance_prepGEMaxXOR = param_prepGEMaxXOR;continue;}
		if (!strcmp("-prepTERNARYLitMax",argv[parameter]))			{guidance_prepTERNARYLitMax = param_prepTERNARYLitMax;continue;}
		if (!strcmp("-prepTERNARYMax",argv[parameter]))				{guidance_prepTERNARYMax = param_prepTERNARYMax;continue;}
		if (!strcmp("-prepNHBRMin",argv[parameter]))				{guidance_prepNHBRMin = param_prepNHBRMin;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in PREP.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	if (guidance_adaptPrepStrategy == -1) param_adaptPrepStrategy = 1;
	if (guidance_prepStrategy != -1) param_adaptPrepStrategy = 0;

	if (guidance_adaptPrepStrategyParams == -1) param_adaptPrepStrategyParams = 1;
	if (guidance_prepReviveCls != -1
			|| guidance_prepACCEMax != -1
			|| guidance_prepACCEMinClsSize != -1
			|| guidance_prepACCESaturate != -1
			|| guidance_prepNIVERMax != -1
			|| guidance_prepATEMin != -1
			|| guidance_prepGEMaxAONN != -1
			|| guidance_prepGEMaxXOR != -1
			|| guidance_prepTERNARYLitMax != -1
			|| guidance_prepTERNARYMax != -1
			|| guidance_prepNHBRMin != -1
			|| guidance_prepUNHRounds != -1){
		param_adaptPrepStrategyParams = 0;
	}
}

#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputPrepSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for PREP.
	printf("c   GUIDANCE:   PREP current parameter setting.\n");
	if (!param_adaptPrepStrategy){
		printf("c   GUIDANCE:     adaptPrepStrategy 0\n");
		printf("c   GUIDANCE:     prepStrategy %d\n", param_prepStrategy);
	} else {
		printf("c   GUIDANCE:     adaptPrepStrategy 1\n");
	}
	if (!param_adaptPrepStrategyParams){
		printf("c   GUIDANCE:     adaptPrepStrategyParams 0\n");
		printf("c   GUIDANCE:       prepReviveCls %d\n", param_prepReviveCls);
		printf("c   GUIDANCE:       prepUNHRounds %d\n", param_prepUNHRounds);
		printf("c   GUIDANCE:       prepACCEMax %d\n", param_prepACCEMax);
		printf("c   GUIDANCE:       prepACCEMinClsSize %d\n", param_prepACCEMinClsSize);
		printf("c   GUIDANCE:       prepACCESaturate %d\n", param_prepACCESaturate);
		printf("c   GUIDANCE:       prepNIVERMax %d\n", param_prepNIVERMax);
		printf("c   GUIDANCE:       prepATEMin %d\n", param_prepATEMin);
		printf("c   GUIDANCE:       prepGEMaxAONN %d\n", param_prepGEMaxAONN);
		printf("c   GUIDANCE:       prepGEMaxXOR %d\n", param_prepGEMaxXOR);
		printf("c   GUIDANCE:       prepTERNARYLitMax %d\n", param_prepTERNARYLitMax);
		printf("c   GUIDANCE:       prepTERNARYMax %d\n", param_prepTERNARYMax);
		printf("c   GUIDANCE:       prepNHBRMin %d\n", param_prepNHBRMin);
	} else {
		printf("c   GUIDANCE:     adaptPrepStrategyParams 1\n");
	}

}
#endif

void guidance_intern_getInpParams(int argc, char** argv){
	//This method will read all INP related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   INP phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptInp")
			&& !guidance_intern_strStartsWith(argv[parameter], "-inp"))
			|| guidance_intern_strStartsWith(argv[parameter], "-inpOutput")) continue;

		if (!strcmp("-adaptInpStrategy",argv[parameter]))			{guidance_adaptInpStrategy = param_adaptInpStrategy;continue;}
		if (!strcmp("-inpStrategy",argv[parameter]))				{guidance_inpStrategy = param_inpStrategy;continue;}
		if (!strcmp("-adaptInpStrategyParams",argv[parameter]))		{guidance_adaptInpStrategyParams = param_adaptInpStrategyParams;continue;}
		if (!strcmp("-inpStrMaxSize",argv[parameter]))				{guidance_inpStrMaxSize = param_inpStrMaxSize;continue;}
		if (!strcmp("-inpTernaryLitMax",argv[parameter]))			{guidance_inpTernaryLitMax = param_inpTernaryLitMax;continue;}
		if (!strcmp("-inpTernaryMax",argv[parameter]))				{guidance_inpTernaryMax = param_inpTernaryMax;continue;}
		if (!strcmp("-inpNiverMax",argv[parameter]))				{guidance_inpNiverMax = param_inpNiverMax;continue;}
		if (!strcmp("-inpGEMaxAONN",argv[parameter]))				{guidance_inpGEMaxAONN = param_inpGEMaxAONN;continue;}
		if (!strcmp("-inpGEMaxXOR",argv[parameter]))				{guidance_inpGEMaxXOR = param_inpGEMaxXOR;continue;}
		if (!strcmp("-inpNHBRMin",argv[parameter]))					{guidance_inpNHBRMin = param_inpNHBRMin;continue;}
		if (!strcmp("-inpUNHRounds",argv[parameter]))				{guidance_inpUNHRounds = param_inpUNHRounds;continue;}

		printf("c ERROR. GUIDANCE did not regocnize parameter %s in INP.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	if (guidance_adaptInpStrategy == -1) param_adaptInpStrategy = 1;
	if (guidance_inpStrategy != -1) param_adaptInpStrategy = 0;
	if (guidance_adaptInpStrategyParams == -1) param_adaptInpStrategyParams = 1;
	if (guidance_inpStrMaxSize != -1
			|| guidance_inpTernaryLitMax != -1
			|| guidance_inpTernaryMax != -1
			|| guidance_inpNiverMax != -1
			|| guidance_inpGEMaxAONN != -1
			|| guidance_inpGEMaxXOR != -1
			|| guidance_inpNHBRMin != -1
			|| guidance_inpUNHRounds != -1){
		param_adaptInpStrategyParams = 0;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputInpSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for INP.
	printf("c   GUIDANCE:   INP current parameter setting.\n");
	if (!param_adaptInpStrategy){
		printf("c   GUIDANCE:     adaptInpStrategy 0\n");
		printf("c   GUIDANCE:       inpStrategy %d\n", param_inpStrategy);
	} else {
		printf("c   GUIDANCE:     adaptInpStrategy 1\n");
	}
	if (!param_adaptInpStrategyParams){
		printf("c   GUIDANCE:     adaptInpStrategyParams 0\n");
		printf("c   GUIDANCE:       inpStrMaxSize %d\n", param_inpStrMaxSize);
		printf("c   GUIDANCE:       inpTernaryLitMax %d\n", param_inpTernaryLitMax);
		printf("c   GUIDANCE:       inpTernaryMax %d\n", param_inpTernaryMax);
		printf("c   GUIDANCE:       inpNiverMax %d\n", param_inpNiverMax);
		printf("c   GUIDANCE:       inpGEMaxAONN %d\n", param_inpGEMaxAONN);
		printf("c   GUIDANCE:       inpGEMaxXOR %d\n", param_inpGEMaxXOR);
		printf("c   GUIDANCE:       inpNHBRMin %d\n", param_inpNHBRMin);
		printf("c   GUIDANCE:       inpUNHRounds %d\n", param_inpUNHRounds);
	} else {
		printf("c   GUIDANCE:     adaptInpStrategyParams 1\n");
	}
}
#endif

void guidance_intern_getSearchParams(int argc, char** argv){
	//This method will read all SEARCH related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SEARCH phase\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptSearch")
				&& !guidance_intern_strStartsWith(argv[parameter], "-search"))
				|| guidance_intern_strStartsWith(argv[parameter], "-searchOutput")) continue;

		if (!strcmp("-adaptSearchStrategy",argv[parameter]))		{guidance_adaptSearchStrategy = param_adaptSearchStrategy;continue;}
		if (!strcmp("-searchStrategy",argv[parameter]))				{guidance_searchStrategy = param_searchStrategy;continue;}
		if (!strcmp("-adaptSearchStrategyParams",argv[parameter]))	{guidance_adaptSearchStrategyParams = param_adaptSearchStrategyParams;continue;}
		if (!strcmp("-searchMIDBlockSize",argv[parameter]))			{guidance_searchMIDBlockSize = param_searchMIDBlockSize;continue;}
		if (!strcmp("-searchMinClsImpact",argv[parameter]))			{guidance_searchMinClsImpact = param_searchMinClsImpact;continue;}
		if (!strcmp("-searchImpactMaxFactor",argv[parameter]))		{guidance_searchImpactMaxFactor = param_searchImpactMaxFactor;continue;}
		if (!strcmp("-searchMaxMagForSLS",argv[parameter]))			{guidance_searchMaxMagForSLS = param_searchMaxMagForSLS;continue;}
		if (!strcmp("-searchRetreatFact",argv[parameter]))			{guidance_searchRetreatFact = param_searchRetreatFact;continue;}
		if (!strcmp("-searchMPWeightModSAT",argv[parameter]))		{guidance_searchMPWeightModSAT = param_searchMPWeightModSAT;continue;}
		if (!strcmp("-searchMPWeightModUNS",argv[parameter]))		{guidance_searchMPWeightModUNS = param_searchMPWeightModUNS;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in SEARCH.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	if (guidance_adaptSearchStrategy == -1 ) param_adaptSearchStrategy = 1;
	if (guidance_searchStrategy != -1) param_adaptSearchStrategy = 0;
	if (guidance_adaptSearchStrategyParams == -1) param_adaptSearchStrategyParams = 1;
	if(guidance_searchMIDBlockSize != -1.0
			|| guidance_searchMinClsImpact != -1.0
			|| guidance_searchImpactMaxFactor != -1.0
			|| guidance_searchMaxMagForSLS != -1.0
			|| guidance_searchRetreatFact != -1.0
			|| guidance_searchMPWeightModSAT != -1.0
			|| guidance_searchMPWeightModUNS != -1.0){
		param_adaptSearchStrategyParams = 0;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSearchSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SEARCH.
	printf("c   GUIDANCE:   SEARCH current parameter setting.\n");
	if (!param_adaptSearchStrategy){
		printf("c   GUIDANCE:     adaptSearchStrategy 0\n");
		printf("c   GUIDANCE:       searchStrategy %d\n", param_searchStrategy);
	} else {
		printf("c   GUIDANCE:     adaptSearchStrategy 1\n");
	}
	if (!param_adaptSearchStrategyParams){
		printf("c   GUIDANCE:     adaptSearchStrategyParams 0\n");
		printf("c   GUIDANCE:       searchMIDBlockSize %f\n", param_searchMIDBlockSize);
		printf("c   GUIDANCE:       searchMinClsImpact %f\n", param_searchMinClsImpact);
		printf("c   GUIDANCE:       searchImpactMaxFactor %f\n", param_searchImpactMaxFactor);
		printf("c   GUIDANCE:       searchMaxMagForSLS %f\n", param_searchMaxMagForSLS);
		printf("c   GUIDANCE:       searchRetreatFact %f\n", param_searchRetreatFact);
		printf("c   GUIDANCE:       searchMPWeightModSAT %f\n", param_searchMPWeightModSAT);
		printf("c   GUIDANCE:       searchMPWeightModUNS %f\n", param_searchMPWeightModUNS);
	} else {
		printf("c   GUIDANCE:     adaptSearchStrategyParams 1\n");
	}
}
#endif

void guidance_intern_getHandleresultParams(int argc, char** argv){
	//This method will read all HANDLERESULT related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   HANDLERESULT phase\n");
	#endif

	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if (!guidance_intern_strStartsWith(argv[parameter], "-handleresult")
				|| guidance_intern_strStartsWith(argv[parameter], "-handleresultOutput")
				|| !strcmp("-handleresultCoarsen",argv[parameter])) continue;

		printf("c ERROR. GUIDANCE did not regocnize parameter %s in HANDLERESULT.\n", argv[parameter]);
				guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputHandleresultSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for HANDLERESULT.
	printf("c   GUIDANCE:   HANDLERESULT current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getShutdownParams(int argc, char** argv){
	//This method will read all SHUTDOWN related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SHUTDOWN phase\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputShutdownSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SHUTDOWN.
	printf("c   GUIDANCE:   SHUTDOWN current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

//MODULES and their parameters.
void guidance_intern_getDETERMParams(int argc, char** argv){
	//This method will read all DETERM related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   DETERM module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputDETERMSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for DETERM.
	printf("c   GUIDANCE:   DETERM current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getSPECCERParams(int argc, char** argv){
	//This method will read all SPECCER related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SPECCER module\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptSpeccer")
				&& !guidance_intern_strStartsWith(argv[parameter], "-speccer"))
				|| guidance_intern_strStartsWith(argv[parameter], "-speccerOutput")
				|| !strcmp("-speccerNNFFirst",argv[parameter])
		) continue;

		printf("c ERROR. GUIDANCE did not recognize parameter %s in SPECCER.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSPECCERSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SPECCER.
	printf("c   GUIDANCE:   SPECCER current parameter setting.\n");
	printf("c   GUIDANCE:     speccerNNFFirst %d\n", param_speccerNNFFirst);
}
#endif

void guidance_intern_getLOADERParams(int argc, char** argv){
	//This method will read all LOADER related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   LOADER module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputLOADERSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for LOADER.
	printf("c   GUIDANCE:   LOADER current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getATTRIBUTORParams(int argc, char** argv){
	//This method will read all ATTRIBUTOR related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   ATTRIBUTOR module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputATTRIBUTORSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for ATTRIBUTOR.
	printf("c   GUIDANCE:   ATTRIBUTOR current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getRANDOMFORESTParams(int argc, char** argv){
	//This method will read all RANDOMFOREST related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   RANDOMFOREST module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputRANDOMFORESTSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for RANDOMFOREST.
	printf("c   GUIDANCE:   RANDOMFOREST current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getITEADAPTERParams(int argc, char** argv){
	//This method will read all ITEADAPTER related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   ITEADAPTER module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputITEADAPTERSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for ITEADAPTER.
	printf("c   GUIDANCE:   ITEADAPTER current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getPREParams(int argc, char** argv){
	//This method will read all PRE related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   PRE module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputPRESetting(){
	//This method outputs the adaptation capabilities and enforced parameters for PRE.
	printf("c   GUIDANCE:   PRE current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getSATELITEParams(int argc, char** argv){
	//This method will read all SATELITE related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SATELITE module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSATELITESetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SATELITE.
	printf("c   GUIDANCE:   SATELITE current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getINParams(int argc, char** argv){
	//This method will read all IN related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   IN module\n");
	#endif
	//Currently, there is nothing we can check here.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputINSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for IN.
	printf("c   GUIDANCE:   IN current parameter setting.\n");
	printf("c   GUIDANCE:     currently has no parameters\n");
}
#endif

void guidance_intern_getMPParams(int argc, char** argv){
	//This method will read all MP related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   MP module\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptMp")
				&& !guidance_intern_strStartsWith(argv[parameter], "-mp"))
				|| guidance_intern_strStartsWith(argv[parameter], "-mpOutput")
				|| !strcmp("-mpMaxNumIterations",argv[parameter])
				|| !strcmp("-mpMaxNonImpIterations",argv[parameter])
				|| !strcmp("-mpMaxMagnetization",argv[parameter])
				|| !strcmp("-mpEpsilon",argv[parameter])
				|| !strcmp("-mpWeightNoise",argv[parameter])
		) continue;

		if (!strcmp("-adaptMpUR",argv[parameter]))				{guidance_adaptMpUR = param_adaptMpUR;continue;}
		if (!strcmp("-mpUpdateRule",argv[parameter]))			{guidance_mpUpdateRule = param_mpUpdateRule;continue;}
		if (!strcmp("-adaptMpURParams",argv[parameter]))		{guidance_adaptMpURParams = param_adaptMpURParams;continue;}
		if (!strcmp("-mpRho",argv[parameter]))					{guidance_mpRho = param_mpRho;continue;}
		if (!strcmp("-mpSigma",argv[parameter]))				{guidance_mpSigma = param_mpSigma;continue;}
		if (!strcmp("-mpPhi",argv[parameter]))					{guidance_mpPhi = param_mpPhi;continue;}
		if (!strcmp("-mpUpsilon",argv[parameter]))				{guidance_mpUpsilon = param_mpUpsilon;continue;}
		if (!strcmp("-mpMaxConvergenceDiff",argv[parameter]))	{guidance_mpMaxConvergenceDiff = param_mpMaxConvergenceDiff;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in MP.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	if (guidance_adaptMpUR == -1) param_adaptMpUR = 1;
	if (guidance_mpUpdateRule != -1) param_adaptMpUR = 0;
	if (guidance_adaptMpURParams == -1) param_adaptMpURParams = 1;
	if (guidance_mpRho != -1.0
			|| guidance_mpSigma != -1.0
			|| guidance_mpPhi != -2.0
			|| guidance_mpUpsilon != -1.0
			|| guidance_mpMaxConvergenceDiff != -1.0){
		param_adaptMpURParams = 0;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputMPSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for MP.
	printf("c   GUIDANCE:   MP current parameter setting.\n");
	printf("c   GUIDANCE:     mpMaxNumIterations %d\n", param_mpMaxNumIterations);
	printf("c   GUIDANCE:     mpMaxNonImpIterations %d\n", param_mpMaxNonImpIterations);
	printf("c   GUIDANCE:     param_mpMaxMagnetization %f\n", param_mpMaxMagnetization);
	printf("c   GUIDANCE:     param_mpEpsilon %f\n", param_mpEpsilon);
	printf("c   GUIDANCE:     param_mpWeightNoise %f\n", param_mpWeightNoise);
	if (!param_adaptMpUR){
		printf("c   GUIDANCE:     adaptMpUR 0\n");
		printf("c   GUIDANCE:       mpUpdateRule %d\n", param_mpUpdateRule);
	} else {
		printf("c   GUIDANCE:     adaptMpUR 1\n");
	}
	if (!param_adaptMpURParams){
		printf("c   GUIDANCE:     adaptMpURParams 0\n");
		printf("c   GUIDANCE:       mpRho %f\n", param_mpRho);
		printf("c   GUIDANCE:       mpSigma %f\n", param_mpSigma);
		printf("c   GUIDANCE:       mpPhi %f\n", param_mpPhi);
		printf("c   GUIDANCE:       mpUpsilon %f\n", param_mpUpsilon);
		printf("c   GUIDANCE:       mpMaxConvergenceDiff %f\n", param_mpMaxConvergenceDiff);
	} else {
		printf("c   GUIDANCE:     adaptMpURParams 1\n");
	}
}
#endif

void guidance_intern_getSLSParams(int argc, char** argv){
	//This method will read all SLS related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SLS module\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptSls")
				&& !guidance_intern_strStartsWith(argv[parameter], "-sls"))
				|| guidance_intern_strStartsWith(argv[parameter], "-slsOutput")) continue;

		//SLS Assignment Init Rule
		if (!strcmp("-adaptSlsAIR",argv[parameter]))			{guidance_adaptSlsAIR = param_adaptSlsAIR;continue;}
		if (!strcmp("-slsAssInitRule",argv[parameter]))			{guidance_slsAssInitRule = param_slsAssInitRule;continue;}
		if (!strcmp("-adaptSlsAIRParams",argv[parameter]))		{guidance_adaptSlsAIRParams = param_adaptSlsAIRParams;continue;}
		//SLS Try Restart Rule
		if (!strcmp("-adaptSlsTRR",argv[parameter]))			{guidance_adaptSlsTRR = param_adaptSlsTRR;continue;}
		if (!strcmp("-slsTryRestartRule",argv[parameter]))		{guidance_slsTryRestartRule = param_slsTryRestartRule;continue;}
		if (!strcmp("-adaptSlsTRRParams",argv[parameter]))		{guidance_adaptSlsTRRParams = param_adaptSlsTRRParams;continue;}
		if (!strcmp("-slsMaxFlipsA",argv[parameter]))			{guidance_slsMaxFlipsA = param_slsMaxFlipsA;continue;}
		if (!strcmp("-slsMaxFlipsB",argv[parameter]))			{guidance_slsMaxFlipsB = param_slsMaxFlipsB;continue;}
		if (!strcmp("-slsMaxTries",argv[parameter]))			{guidance_slsMaxTries = param_slsMaxTries;continue;}
		//SLS Pick and Flip Variable Rule
		if (!strcmp("-adaptSlsPAFVR",argv[parameter]))			{guidance_adaptSlsPAFVR = param_adaptSlsPAFVR;continue;}
		if (!strcmp("-slsPickAndFlipVarRule",argv[parameter]))	{guidance_slsPickAndFlipVarRule = param_slsPickAndFlipVarRule;continue;}
		if (!strcmp("-adaptSlsPAFVRParams",argv[parameter]))	{guidance_adaptSlsPAFVRParams = param_adaptSlsPAFVRParams;continue;}
		if (!strcmp("-slsNoise",argv[parameter]))				{guidance_slsNoise = param_slsNoise;continue;}
		if (!strcmp("-slsProbsatCB",argv[parameter]))			{guidance_slsProbsatCB = param_slsProbsatCB;continue;}
		if (!strcmp("-slsProbsatCBShift",argv[parameter]))		{guidance_slsProbsatCBShift = param_slsProbsatCBShift;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in SLS.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	//SLS Assignment Init Rule
	if (guidance_adaptSlsAIR == -1) param_adaptSlsAIR = 1;
	if (guidance_slsAssInitRule != -1) param_adaptSlsAIR = 0;
	if (guidance_adaptSlsAIRParams == -1) param_adaptSlsAIRParams = 1; //Currently there are no parameters.
	//SLS Try Restart Rule
	if (guidance_adaptSlsTRR == -1) param_adaptSlsTRR = 1;
	if (guidance_slsTryRestartRule != -1) param_adaptSlsTRR = 0;
	if (guidance_adaptSlsTRRParams == -1) param_adaptSlsTRRParams = 1;
	if (guidance_slsMaxFlipsA != -1.0
			|| guidance_slsMaxFlipsB != -1.0
			|| guidance_slsMaxTries != -1){
		param_adaptSlsTRRParams = 0;
	}
	//SLS Pick And Flip Variable Rule
	if (guidance_adaptSlsPAFVR == -1) param_adaptSlsPAFVR = 1;
	if (guidance_slsPickAndFlipVarRule != -1) param_adaptSlsPAFVR = 0;
	if (guidance_adaptSlsPAFVRParams == -1) param_adaptSlsPAFVRParams = 1;
	if (guidance_slsNoise != -1.0
			|| guidance_slsProbsatCB != -1.0
			|| guidance_slsProbsatCBShift != -1.0){
		param_adaptSlsPAFVRParams = 0;
	}
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSLSSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SLS.
	printf("c   GUIDANCE:   SLS current parameter setting.\n");
	if (!param_adaptSlsAIR){
		printf("c   GUIDANCE:     adaptSlsAIR 0\n");
		printf("c   GUIDANCE:       slsAssInitRule %d\n", param_slsAssInitRule);
	} else {
		printf("c   GUIDANCE:     adaptSlsAIR 1\n");
	}
	if (!param_adaptSlsAIRParams){
		printf("c   GUIDANCE:     adaptSlsAIRParams 0\n");
		printf("c   GUIDANCE:       currently, there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptSlsAIRParams 1\n");
	}

	if (!param_adaptSlsTRR){
		printf("c   GUIDANCE:     adaptSlsTRR 0\n");
		printf("c   GUIDANCE:       slsTryRestartRule %d\n", param_slsTryRestartRule);
	} else {
		printf("c   GUIDANCE:     adaptSlsTRR 1\n");
	}
	if (!param_adaptSlsTRRParams){
		printf("c   GUIDANCE:     adaptSlsTRRParams 0\n");
		printf("c   GUIDANCE:       slsMaxFlipsA %f\n", param_slsMaxFlipsA);
		printf("c   GUIDANCE:       slsMaxFlipsB %f\n", param_slsMaxFlipsB);
		printf("c   GUIDANCE:       slsMaxTries %d\n", param_slsMaxTries);
	} else {
		printf("c   GUIDANCE:     adaptSlsTRRParams 1\n");
	}

	if (!param_adaptSlsPAFVR){
		printf("c   GUIDANCE:     adaptSlsPAFVR 0\n");
		printf("c   GUIDANCE:       slsPickAndFlipVarRule %d\n", param_slsPickAndFlipVarRule);
	} else {
		printf("c   GUIDANCE:     adaptSlsPAFVR 1\n");
	}
	if (!param_adaptSlsPAFVRParams){
		printf("c   GUIDANCE:     adaptSlsPAFVRParams 0\n");
		printf("c   GUIDANCE:       slsNoise %f\n", param_slsNoise);
		printf("c   GUIDANCE:       slsProbsatCB %f\n", param_slsProbsatCB);
		printf("c   GUIDANCE:       slsProbsatCBShift %f\n", param_slsProbsatCBShift);
	} else {
		printf("c   GUIDANCE:     adaptSlsPAFVRParams 1\n");
	}
}
#endif

void guidance_intern_getCDCLParams(int argc, char** argv){
	//This method will read all CDCL related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   CDCL module\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptCdcl")
				&& !guidance_intern_strStartsWith(argv[parameter], "-cdcl"))
				|| guidance_intern_strStartsWith(argv[parameter], "-cdclOutput")) continue;

		//CDCL
		//CDCL Restart Rule
		if (!strcmp("-adaptCdclRR",argv[parameter]))				{guidance_adaptCdclRR = param_adaptCdclRR;continue;}
		if (!strcmp("-cdclRestartRule",argv[parameter]))			{guidance_cdclRestartRule = param_cdclRestartRule;continue;}
		if (!strcmp("-adaptCdclRRParams",argv[parameter]))			{guidance_adaptCdclRRParams = param_adaptCdclRRParams;continue;}
		if (!strcmp("-cdclRestartStagMaxFrac",argv[parameter]))		{guidance_cdclRestartStagMaxFrac = param_cdclRestartStagMaxFrac;continue;}
		if (!strcmp("-cdclRestartInnerouterInit",argv[parameter]))	{guidance_cdclRestartInnerouterInit = param_cdclRestartInnerouterInit;continue;}
		if (!strcmp("-cdclRestartInnerouterInc",argv[parameter]))	{guidance_cdclRestartInnerouterInc = param_cdclRestartInnerouterInc;continue;}
		if (!strcmp("-cdclRestartLubyUnit",argv[parameter]))		{guidance_cdclRestartLubyUnit = param_cdclRestartLubyUnit;continue;}
		if (!strcmp("-cdclRestartVarAgiIncBump",argv[parameter]))	{guidance_cdclRestartVarAgiIncBump = param_cdclRestartVarAgiIncBump;continue;}
		if (!strcmp("-cdclRestartMaxAgiFrac",argv[parameter]))		{guidance_cdclRestartMaxAgiFrac = param_cdclRestartMaxAgiFrac;continue;}
		if (!strcmp("-cdclRestartMaxAgiOutFrac",argv[parameter]))	{guidance_cdclRestartMaxAgiOutFrac = param_cdclRestartMaxAgiOutFrac;continue;}
		//CDCL Select Variable Rule
		if (!strcmp("-adaptCdclSVR",argv[parameter]))				{guidance_adaptCdclSVR = param_adaptCdclSVR;continue;}
		if (!strcmp("-cdclSelectVarRule",argv[parameter]))			{guidance_cdclSelectVarRule = param_cdclSelectVarRule;continue;}
		if (!strcmp("-adaptCdclSVRParams",argv[parameter]))			{guidance_adaptCdclSVRParams = param_adaptCdclSVRParams;continue;}
		if (!strcmp("-cdclSelectVarActIncBump",argv[parameter]))	{guidance_cdclSelectVarActIncBump = param_cdclSelectVarActIncBump;continue;}
		if (!strcmp("-cdclSelectVarRandProb",argv[parameter]))		{guidance_cdclSelectVarRandProb = param_cdclSelectVarRandProb;continue;}
		if (!strcmp("-adaptCdclSVAI",argv[parameter]))				{guidance_adaptCdclSVAI = param_adaptCdclSVAI;continue;}
		if (!strcmp("-cdclSelectVarActInit",argv[parameter]))		{guidance_cdclSelectVarActInit = param_cdclSelectVarActInit;continue;}
		if (!strcmp("-adaptCdclSVAIParams",argv[parameter]))		{guidance_adaptCdclSVAIParams = param_adaptCdclSVAIParams;continue;}
		//CDCL Select Direction Rule
		if (!strcmp("-adaptCdclSDR",argv[parameter]))				{guidance_adaptCdclSDR = param_adaptCdclSDR;continue;}
		if (!strcmp("-cdclSelectDirRule",argv[parameter]))			{guidance_cdclSelectDirRule = param_cdclSelectDirRule;continue;}
		if (!strcmp("-adaptCdclSDRParams",argv[parameter]))			{guidance_adaptCdclSDRParams = param_adaptCdclSDRParams;continue;}
		if (!strcmp("-cdclSelectDirMinFlipDist",argv[parameter]))	{guidance_cdclSelectDirMinFlipDist = param_cdclSelectDirMinFlipDist;continue;}
		if (!strcmp("-adaptCdclSDDI",argv[parameter]))				{guidance_adaptCdclSDDI = param_adaptCdclSDDI;continue;}
		if (!strcmp("-cdclSelectDirDirInit",argv[parameter]))		{guidance_cdclSelectDirDirInit = param_cdclSelectDirDirInit;continue;}
		if (!strcmp("-adaptCdclSDDIParams",argv[parameter]))		{guidance_adaptCdclSDDIParams = param_adaptCdclSDDIParams;continue;}
		//CDCL Conflict Analysis Rule
		if (!strcmp("-adaptCdclCAR",argv[parameter]))				{guidance_adaptCdclCAR = param_adaptCdclCAR;continue;}
		if (!strcmp("-cdclConflictAnalysisRule",argv[parameter]))	{guidance_cdclConflictAnalysisRule = param_cdclConflictAnalysisRule;continue;}
		if (!strcmp("-adaptCdclCARParams",argv[parameter]))			{guidance_adaptCdclCARParams = param_adaptCdclCARParams;continue;}
		//CDCL Maintenance Rule
		if (!strcmp("-adaptCdclMR",argv[parameter]))				{guidance_adaptCdclMR = param_adaptCdclMR;continue;}
		if (!strcmp("-cdclMaintenanceRule",argv[parameter]))		{guidance_cdclMaintenanceRule = param_cdclMaintenanceRule;continue;}
		if (!strcmp("-adaptCdclMRParams",argv[parameter]))			{guidance_adaptCdclMRParams = param_adaptCdclMRParams;continue;}
		if (!strcmp("-cdclMaintenanceMinDev",argv[parameter]))		{guidance_cdclMaintenanceMinDev = param_cdclMaintenanceMinDev;continue;}
		if (!strcmp("-cdclMaintenanceMaxAvg",argv[parameter]))		{guidance_cdclMaintenanceMaxAvg = param_cdclMaintenanceMaxAvg;continue;}
		if (!strcmp("-cdclMaintenanceActIncBump",argv[parameter]))	{guidance_cdclMaintenanceActIncBump = param_cdclMaintenanceActIncBump;continue;}
		if (!strcmp("-cdclMaintenanceInitial",argv[parameter]))		{guidance_cdclMaintenanceInitial = param_cdclMaintenanceInitial;continue;}
		if (!strcmp("-cdclMaintenanceIncrease",argv[parameter]))	{guidance_cdclMaintenanceIncrease = param_cdclMaintenanceIncrease;continue;}
		if (!strcmp("-cdclMaintenanceBase",argv[parameter]))		{guidance_cdclMaintenanceBase = param_cdclMaintenanceBase;continue;}
		if (!strcmp("-cdclMaintenanceBoost",argv[parameter]))		{guidance_cdclMaintenanceBoost = param_cdclMaintenanceBoost;continue;}
		//CDCL Inprocessing Rule
		if (!strcmp("-adaptCdclIR",argv[parameter]))				{guidance_adaptCdclIR = param_adaptCdclIR;continue;}
		if (!strcmp("-cdclInprocessingRule",argv[parameter]))		{guidance_cdclInprocessingRule = param_cdclInprocessingRule;continue;}
		if (!strcmp("-adaptCdclIRParams",argv[parameter]))			{guidance_adaptCdclIRParams = param_adaptCdclIRParams;continue;}
		if (!strcmp("-cdclIRUNHRounds",argv[parameter]))			{guidance_cdclIRUNHRounds = param_cdclIRUNHRounds;continue;}
		if (!strcmp("-cdclIRTERNARYLitMax",argv[parameter]))		{guidance_cdclIRTERNARYLitMax = param_cdclIRTERNARYLitMax;continue;}
		if (!strcmp("-cdclIRTERNARYMax",argv[parameter]))			{guidance_cdclIRTERNARYMax = param_cdclIRTERNARYMax;continue;}
		if (!strcmp("-cdclIRNIVERMax",argv[parameter]))				{guidance_cdclIRNIVERMax = param_cdclIRNIVERMax;continue;}
		if (!strcmp("-cdclIRGEMaxAONN",argv[parameter]))			{guidance_cdclIRGEMaxAONN = param_cdclIRGEMaxAONN;continue;}
		if (!strcmp("-cdclIRGEMaxXOR",argv[parameter]))				{guidance_cdclIRGEMaxXOR = param_cdclIRGEMaxXOR;continue;}
		if (!strcmp("-cdclIRStrMaxSize",argv[parameter]))			{guidance_cdclIRStrMaxSize = param_cdclIRStrMaxSize;continue;}
		if (!strcmp("-cdclIRNHBRMin",argv[parameter]))				{guidance_cdclIRNHBRMin = param_cdclIRNHBRMin;continue;}
		if (!strcmp("-cdclIRMinConfDistance",argv[parameter]))		{guidance_cdclIRMinConfDistance = param_cdclIRMinConfDistance;continue;}
		//CDCL Strengthen Learned Rule
		if (!strcmp("-adaptCdclSLR",argv[parameter]))				{guidance_adaptCdclSLR = param_adaptCdclSLR;continue;}
		if (!strcmp("-cdclStrLearnedRule",argv[parameter]))			{guidance_cdclStrLearnedRule = param_cdclStrLearnedRule;continue;}
		if (!strcmp("-adaptCdclSLRParams",argv[parameter]))			{guidance_adaptCdclSLRParams = param_adaptCdclSLRParams;continue;}
		//CDCL Strengthen Other Rule
		if (!strcmp("-adaptCdclSOR",argv[parameter]))				{guidance_adaptCdclSOR = param_adaptCdclSOR;continue;}
		if (!strcmp("-cdclStrOtherRule",argv[parameter]))			{guidance_cdclStrOtherRule = param_cdclStrOtherRule;continue;}
		if (!strcmp("-adaptCdclSORParams",argv[parameter]))			{guidance_adaptCdclSORParams = param_adaptCdclSORParams;continue;}
		//CDCL Compute BJ Level Rule
		if (!strcmp("-adaptCdclCBJLR",argv[parameter]))				{guidance_adaptCdclCBJLR = param_adaptCdclCBJLR;continue;}
		if (!strcmp("-cdclComputeBJLRule",argv[parameter]))			{guidance_cdclComputeBJLRule = param_cdclComputeBJLRule;continue;}
		if (!strcmp("-adaptCdclCBJLRParams",argv[parameter]))		{guidance_adaptCdclCBJLRParams = param_adaptCdclCBJLRParams;continue;}
		//CDCL Abort Rule
		if (!strcmp("-adaptCdclAR",argv[parameter]))				{guidance_adaptCdclAR = param_adaptCdclAR;continue;}
		if (!strcmp("-cdclAbortRule",argv[parameter]))				{guidance_cdclAbortRule = param_cdclAbortRule;continue;}
		if (!strcmp("-adaptCdclARParams",argv[parameter]))			{guidance_adaptCdclARParams = param_adaptCdclARParams;continue;}

		printf("c ERROR. GUIDANCE did not recognize parameter %s in CDCL.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

	//CDCL Restart Rule
	if (guidance_adaptCdclRR == -1) param_adaptCdclRR = 1;
	if (guidance_cdclRestartRule != -1) param_adaptCdclRR = 0;
	if (guidance_adaptCdclRRParams == -1) param_adaptCdclRRParams = 1;
	if (guidance_cdclRestartStagMaxFrac != -1.0
			|| guidance_cdclRestartInnerouterInit != -1
			|| guidance_cdclRestartInnerouterInc != -1.0
			|| guidance_cdclRestartLubyUnit != -1
			|| guidance_cdclRestartVarAgiIncBump != -1.0
			|| guidance_cdclRestartMaxAgiFrac != -1.0
			|| guidance_cdclRestartMaxAgiOutFrac != -1.0){
		param_adaptCdclRRParams = 0;
	}
	//CDCL Select Variable Rule (with activity initialization)
	if (guidance_adaptCdclSVR == -1) param_adaptCdclSVR = 1;							//SVR
	if (guidance_cdclSelectVarRule != -1) param_adaptCdclSVR = 0;
	if (guidance_adaptCdclSVRParams == -1) param_adaptCdclSVRParams = 1;
	if (guidance_cdclSelectVarActIncBump != -1.0
			|| guidance_cdclSelectVarRandProb != -1.0){
		param_adaptCdclSVRParams = 0;
	}
	if (guidance_adaptCdclSVAI == -1) param_adaptCdclSVAI = 1;							//SVAI
	if (guidance_cdclSelectVarActInit != -1)	param_adaptCdclSVAI = 0;
	if (guidance_adaptCdclSVAIParams == -1) param_adaptCdclSVAIParams = 1;
	//Currently, there are no parameters.

	//CDCL Select Direction Rule (with direction initialization)
	if (guidance_adaptCdclSDR == -1) param_adaptCdclSDR = 1;							//SDR
	if (guidance_cdclSelectDirRule != -1) param_adaptCdclSDR = 0;
	if (guidance_adaptCdclSDRParams == -1) param_adaptCdclSDRParams = 1;
	if (guidance_cdclSelectDirMinFlipDist != -1.0) {
		param_adaptCdclSDRParams = 0;
	}
	if (guidance_adaptCdclSDDI == -1) param_adaptCdclSDDI = 1;							//SDDI
	if (guidance_cdclSelectDirDirInit != -1)	param_adaptCdclSDDI = 0;
	if (guidance_adaptCdclSDDIParams == -1) param_adaptCdclSDDIParams = 1;
	//Currently, there are no parameters.

	//CDCL Conflict Analysis Rule
	if (guidance_adaptCdclCAR == -1) param_adaptCdclCAR = 1;
	if (guidance_cdclConflictAnalysisRule != -1) param_adaptCdclCAR = 0;
	if (guidance_adaptCdclCARParams == -1) param_adaptCdclCARParams = 1;
	//Currently, there are no parameters.

	//CDCL Maintenance Rule
	if (guidance_adaptCdclMR == -1) param_adaptCdclMR = 1;
	if (guidance_cdclMaintenanceRule != -1) param_adaptCdclMR = 0;
	if (guidance_adaptCdclMRParams == -1) param_adaptCdclMRParams = 1;
	if (guidance_cdclMaintenanceMinDev != -1.0
			|| guidance_cdclMaintenanceMaxAvg != -1.0
			|| guidance_cdclMaintenanceActIncBump != -1.0
			|| guidance_cdclMaintenanceInitial != -1.0
			|| guidance_cdclMaintenanceIncrease != -1.0
			|| guidance_cdclMaintenanceBase != -1
			|| guidance_cdclMaintenanceBoost != -1){
		param_adaptCdclMRParams = 0;
	}

	//CDCL Inprocessing Rule
	if (guidance_adaptCdclIR == -1) param_adaptCdclIR = 1;
	if (guidance_cdclInprocessingRule != -1) param_adaptCdclIR = 0;
	if (guidance_adaptCdclIRParams == -1) param_adaptCdclIRParams = 1;
	if (guidance_cdclIRUNHRounds != -1
			|| guidance_cdclIRTERNARYLitMax != -1
			|| guidance_cdclIRTERNARYMax != -1
			|| guidance_cdclIRNIVERMax != -1
			|| guidance_cdclIRGEMaxAONN != -1
			|| guidance_cdclIRGEMaxXOR != -1
			|| guidance_cdclIRStrMaxSize != -1
			|| guidance_cdclIRNHBRMin != -1
			|| guidance_cdclIRMinConfDistance != -1.0){
		param_adaptCdclIRParams = 0;
	}
	//CDCL Strengthen Learned Rule
	if (guidance_adaptCdclSLR == -1) param_adaptCdclSLR = 1;
	if (guidance_cdclStrLearnedRule != -1) param_adaptCdclSLR = 0;
	if (guidance_adaptCdclSLRParams == -1) param_adaptCdclSLRParams = 1;
	//Currently, there are no parameters.

	//CDCL Strengthen Other Rule
	if (guidance_adaptCdclSOR == -1) param_adaptCdclSOR = 1;
	if (guidance_cdclStrOtherRule != -1) param_adaptCdclSOR = 0;
	if (guidance_adaptCdclSORParams == -1) param_adaptCdclSORParams = 1;
	//Currently, there are no parameters.

	//CDCL Compute BJ Level Rule
	if (guidance_adaptCdclCBJLR == -1) param_adaptCdclCBJLR = 1;
	if (guidance_cdclComputeBJLRule != -1) param_adaptCdclCBJLR = 0;
	if (guidance_adaptCdclCBJLRParams == -1) param_adaptCdclCBJLRParams = 1;
	//Currently, there are no parameters.

	//CDCL Abort Rule
	if (guidance_adaptCdclAR == -1) param_adaptCdclAR = 1;
	if (guidance_cdclAbortRule != -1) param_adaptCdclAR = 0;
	if (guidance_adaptCdclARParams == -1) param_adaptCdclARParams = 1;
	//Currently, there are no parameters.
}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputCDCLSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for CDCL.
	printf("c   GUIDANCE:   CDCL current parameter setting.\n");
	if (!param_adaptCdclRR){
		printf("c   GUIDANCE:     adaptCdclRR 0\n");
		printf("c   GUIDANCE:       cdclRestartRule %d\n", param_cdclRestartRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclRR 1\n");
	}
	if (!param_adaptCdclRRParams){
		printf("c   GUIDANCE:     adaptCdclRRParams 0\n");
		printf("c   GUIDANCE:       cdclRestartStagMaxFrac %f\n", param_cdclRestartStagMaxFrac);
		printf("c   GUIDANCE:       cdclRestartInnerouterInit %d\n", param_cdclRestartInnerouterInit);
		printf("c   GUIDANCE:       cdclRestartInnerouterInc %f\n", param_cdclRestartInnerouterInc);
		printf("c   GUIDANCE:       cdclRestartLubyUnit %d\n", param_cdclRestartLubyUnit);
		printf("c   GUIDANCE:       cdclRestartVarAgiIncBump %f\n", param_cdclRestartVarAgiIncBump);
		printf("c   GUIDANCE:       cdclRestartMaxAgiFrac %f\n", param_cdclRestartMaxAgiFrac);
		printf("c   GUIDANCE:       cdclRestartMaxAgiOutFrac %f\n", param_cdclRestartMaxAgiOutFrac);
	} else {
		printf("c   GUIDANCE:     adaptCdclRRParams 1\n");
	}

	if (!param_adaptCdclSVR){
		printf("c   GUIDANCE:     adaptCdclSVR 0\n");
		printf("c   GUIDANCE:     cdclSelectVarRule %d\n", param_cdclSelectVarRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclSVR 1\n");
	}
	if (!param_adaptCdclSVRParams){
		printf("c   GUIDANCE:     adaptCdclSVRParams 0\n");
		printf("c   GUIDANCE:       cdclSelectVarActIncBump %f\n", param_cdclSelectVarActIncBump);
		printf("c   GUIDANCE:       cdclSelectVarRandProb %f\n", param_cdclSelectVarRandProb);
	} else {
		printf("c   GUIDANCE:     adaptCdclSVRParams 1\n");
	}

	if (!param_adaptCdclSVAI){
		printf("c   GUIDANCE:     adaptCdclSVAI 0\n");
		printf("c   GUIDANCE:       cdclSelectVarActInit %d\n", param_cdclSelectVarActInit);
	} else {
		printf("c   GUIDANCE:     adaptCdclSVAI 1\n");
	}
	if (!param_adaptCdclSVAIParams){
		printf("c   GUIDANCE:     adaptCdclSVAIParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclSVAIParams 1\n");
	}

	if (!param_adaptCdclSDR){
		printf("c   GUIDANCE:     adaptCdclSDR 0\n");
		printf("c   GUIDANCE:       cdclSelectDirRule %d\n", param_cdclSelectDirRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclSDR 1\n");
	}
	if (!param_adaptCdclSDRParams){
		printf("c   GUIDANCE:     adaptCdclSDRParams 0\n");
		printf("c   GUIDANCE:       cdclSelectDirMinFlipDist %d\n", param_cdclSelectDirMinFlipDist);
	} else {
		printf("c   GUIDANCE:     adaptCdclSDRParams 1\n");
	}

	if (!param_adaptCdclSDDI){
		printf("c   GUIDANCE:     adaptCdclSDDI 0\n");
		printf("c   GUIDANCE:       cdclSelectDirDirInit %d\n", param_cdclSelectDirDirInit);
	} else {
		printf("c   GUIDANCE:     adaptCdclSDDI 1\n");
	}
	if (!param_adaptCdclSDDIParams){
		printf("c   GUIDANCE:     adaptCdclSDDIParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclSDDIParams 1\n");
	}

	if (!param_adaptCdclCAR){
		printf("c   GUIDANCE:     adaptCdclCAR 0\n");
		printf("c   GUIDANCE:       cdclConflictAnalysisRule %d\n", param_cdclConflictAnalysisRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclCAR 1\n");
	}
	if (!param_adaptCdclCARParams){
		printf("c   GUIDANCE:     adaptCdclCARParams 0\n");
		printf("c   GUIDANCE:       currently, there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclCARParams 1\n");
	}

	if (!param_adaptCdclMR){
		printf("c   GUIDANCE:     adaptCdclMR 0\n");
		printf("c   GUIDANCE:       cdclMaintenanceRule %d\n", param_cdclMaintenanceRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclMR 1\n");
	}
	if (!param_adaptCdclMRParams){
		printf("c   GUIDANCE:     adaptCdclMRParams 0\n");
		printf("c   GUIDANCE:       cdclMaintenanceMinDev %f\n", param_cdclMaintenanceMinDev);
		printf("c   GUIDANCE:       cdclMaintenanceMaxAvg %f\n", param_cdclMaintenanceMaxAvg);
		printf("c   GUIDANCE:       cdclMaintenanceActIncBump %f\n", param_cdclMaintenanceActIncBump);
		printf("c   GUIDANCE:       cdclMaintenanceInitial %f\n", param_cdclMaintenanceInitial);
		printf("c   GUIDANCE:       cdclMaintenanceIncrease %f\n", param_cdclMaintenanceIncrease);
		printf("c   GUIDANCE:       cdclMaintenanceBase %d\n", param_cdclMaintenanceBase);
		printf("c   GUIDANCE:       cdclMaintenanceBoost %d\n", param_cdclMaintenanceBoost);
	} else {
		printf("c   GUIDANCE:     adaptCdclMRParams 1\n");
	}

	if (!param_adaptCdclIR){
		printf("c   GUIDANCE:     adaptCdclIR 0\n");
		printf("c   GUIDANCE:       cdclInprocessingRule %d\n", param_cdclInprocessingRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclIR 1\n");
	}
	if (!param_adaptCdclIRParams){
		printf("c   GUIDANCE:     adaptCdclIRParams 0\n");
		printf("c   GUIDANCE:       cdclIRUNHRounds %d\n", param_cdclIRUNHRounds);
		printf("c   GUIDANCE:       cdclIRTERNARYLitMax %d\n", param_cdclIRTERNARYLitMax);
		printf("c   GUIDANCE:       cdclIRTERNARYMax %d\n", param_cdclIRTERNARYMax);
		printf("c   GUIDANCE:       cdclIRNIVERMax %d\n", param_cdclIRNIVERMax);
		printf("c   GUIDANCE:       cdclIRGEMaxAONN %d\n", param_cdclIRGEMaxAONN);
		printf("c   GUIDANCE:       cdclIRGEMaxXOR %d\n", param_cdclIRGEMaxXOR);
		printf("c   GUIDANCE:       cdclIRStrMaxSize %d\n", param_cdclIRStrMaxSize);
		printf("c   GUIDANCE:       cdclIRNHBRMin %d\n", param_cdclIRNHBRMin);
		printf("c   GUIDANCE:       cdclIRMinConfDistance %f\n", param_cdclIRMinConfDistance);
	} else {
		printf("c   GUIDANCE:     adaptCdclIRParams 1\n");
	}

	if (!param_adaptCdclSLR){
		printf("c   GUIDANCE:     adaptCdclSLR 0\n");
		printf("c   GUIDANCE:       cdclStrLearnedRule %d\n", param_cdclStrLearnedRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclSLR 1\n");
	}
	if (!param_adaptCdclSLRParams){
		printf("c   GUIDANCE:     adaptCdclSLRParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclSLRParams 1\n");
	}

	if (!param_adaptCdclSOR){
		printf("c   GUIDANCE:     adaptCdclSOR 0\n");
		printf("c   GUIDANCE:       cdclStrOtherRule %d\n", param_cdclStrOtherRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclSOR 1\n");
	}
	if (!param_adaptCdclSORParams){
		printf("c   GUIDANCE:     adaptCdclSORParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclSORParams 1\n");
	}

	if (!param_adaptCdclCBJLR){
		printf("c   GUIDANCE:     adaptCdclCBJLR 0\n");
		printf("c   GUIDANCE:       cdclComputeBJLRule %d\n", param_cdclComputeBJLRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclCBJLR 1\n");
	}
	if (!param_adaptCdclCBJLRParams){
		printf("c   GUIDANCE:     adaptCdclCBJLRParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclCBJLRParams 1\n");
	}

	if (!param_adaptCdclAR){
		printf("c   GUIDANCE:     adaptCdclAR 0\n");
		printf("c   GUIDANCE:       cdclAbortRule %d\n", param_cdclAbortRule);
	} else {
		printf("c   GUIDANCE:     adaptCdclAR 1\n");
	}
	if (!param_adaptCdclARParams){
		printf("c   GUIDANCE:     adaptCdclARParams 0\n");
		printf("c   GUIDANCE:       currently there are no parameters.\n");
	} else {
		printf("c   GUIDANCE:     adaptCdclARParams 1\n");
	}
}
#endif

void guidance_intern_getSOLWRITERParams(int argc, char** argv){
	//This method will read all SOLWRITER related parameters from the command line and determine their value and the
	//remaining adaptation capabilities.

	#ifdef VERBOSE_GUIDANCE
	printf("c   GUIDANCE:   SOLWRITER module\n");
	#endif
	int32_t parameter;

	for (parameter=1; parameter<argc; ++parameter){
		if ((!guidance_intern_strStartsWith(argv[parameter], "-adaptSolwriter")
				&& !guidance_intern_strStartsWith(argv[parameter], "-solwriter"))
				|| guidance_intern_strStartsWith(argv[parameter], "-solwriterOutput")
				|| !strcmp("-solwriterPrintAssignment",argv[parameter])
		) continue;

		printf("c ERROR. GUIDANCE did not recognize parameter %s in SOLWRITER.\n", argv[parameter]);
		guidance_returnCode = GUIDANCE_CONFLICTINGPARAMS; return;
	}

}
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSOLWRITERSetting(){
	//This method outputs the adaptation capabilities and enforced parameters for SOLWRITER.
	printf("c   GUIDANCE:   SOLWRITER current parameter setting.\n");
	printf("c   GUIDANCE:     solwriterPrintAssignment %d\n", param_solwriterPrintAssignment);
}
#endif
