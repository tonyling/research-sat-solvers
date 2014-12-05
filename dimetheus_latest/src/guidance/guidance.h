/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef GUIDANCE_H_
#define GUIDANCE_H_

#include "../dimetheus.h"

uint32_t guidance_returnCode;		//The return code of the guides phase.

#define GUIDE_MINVALUE 0
#define GUIDE_MAXVALUE 10

#define GUIDE_MANUAL 0
#include "_guides/g_manual.h"

#define GUIDE_AUTOADAPT 1
#include "_guides/g_autoadapt.h"

#define GUIDE_ATTONLY 2
#include "_guides/g_attonly.h"

#define GUIDE_RANDFORONLY 3
#include "_guides/g_randforonly.h"

#define GUIDE_PREPONLY 4
#include "_guides/g_preponly.h"

#define GUIDE_CDCL 5
#include "_guides/g_cdcl.h"

#define GUIDE_SLS 6
#include "_guides/g_sls.h"

#define GUIDE_PMPCDCL 7
#include "_guides/g_pmpcdcl.h"

#define GUIDE_PMPSLS 8
#include "_guides/g_pmpsls.h"

#define GUIDE_HYBRIDMSC 9
#include "_guides/g_hybridmsc.h"

#define GUIDE_EMP 10
#include "_guides/g_emp.h"

#define GUIDE_TESTING 999
#include "_guides/g_testing.h"

#define GUIDANCE_NUMCANBEADAPTED 6
static const int32_t guidance_canBeAdapted[GUIDANCE_NUMCANBEADAPTED] = {
	GUIDE_CDCL,
	GUIDE_SLS,
	GUIDE_PMPCDCL,
	GUIDE_PMPSLS,
	GUIDE_HYBRIDMSC,
	GUIDE_EMP
};

//Methods to initialize this phase (are called by the one who wants to execute the above functions).
void guidance_resetPhase();			//Reset the guidance.
void guidance_initPhase();			//For the guidance.
void guidance_disposePhase();		//For freeing all related memory.

static inline void guidance_extern_printNameOfGuide(uint32_t guideNum){
	if (guideNum == GUIDE_MANUAL){
		printf("GUIDE_MANUAL");
	} else if (guideNum == GUIDE_AUTOADAPT){
		printf("GUIDE_AUTOADAPT");
	} else if (guideNum == GUIDE_ATTONLY){
		printf("GUIDE_ATTONLY");
	} else if (guideNum == GUIDE_RANDFORONLY){
		printf("GUIDE_RANDFORONLY");
	} else if (guideNum == GUIDE_PREPONLY){
		printf("GUIDE_PREPONLY");
	} else if (guideNum == GUIDE_CDCL){
		printf("GUIDE_CDCL");
	} else if (guideNum == GUIDE_SLS){
		printf("GUIDE_SLS");
	} else if (guideNum == GUIDE_PMPCDCL){
		printf("GUIDE_PMPCDCL");
	} else if (guideNum == GUIDE_PMPSLS){
		printf("GUIDE_PMPSLS");
	} else if (guideNum == GUIDE_HYBRIDMSC){
		printf("GUIDE_HYBRIDMSC");
	} else if (guideNum == GUIDE_EMP){
		printf("GUIDE_EMP");
	} else if (guideNum == GUIDE_TESTING){
		printf("GUIDE_TESTING");
	} else {
		printf("UNKNOWN");
	}
}

static inline uint32_t guidance_extern_isGuideAdaptable(uint32_t guideNum){
	uint32_t i;

	for (i = 0; i < GUIDANCE_NUMCANBEADAPTED; ++i){
		if (guidance_canBeAdapted[i] == guideNum) return 1;
	}

	return 0;
}

void guidance_extern_enforceGuideAfterAdaptation();	//This method will enforce everything a guide requires after adaptation.

static inline void guidance_printHelp(){
	//Print the help to all the guides we have.
	guide_manual_printHelp();
	guide_autoadapt_printHelp();
    guide_attonly_printHelp();
    guide_randforonly_printHelp();
    guide_preponly_printHelp();
    guide_cdcl_printHelp();
    guide_sls_printHelp();
    guide_pmpcdcl_printHelp();
    guide_pmpsls_printHelp();
    guide_hybridmsc_printHelp();
    guide_emp_printHelp();
    guide_testing_printHelp();
}

void (*guidance_extern_apply)(int, char**);		//Implemented by the specific guides.
void guidance_intern_getAllParams(int, char**);	//Determines the adaptability of strategies and parameters.
void guidance_intern_printGuidanceResult();		//Prints the result of applying the guide.
int32_t guidance_intern_strStartsWith(const char*, const char*); //Checking if a given string starts with something given.

//Phases and their parameters
//RESET
void guidance_intern_getResetParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputResetSetting();
#endif

//PARAMS
void guidance_intern_getParamsParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputParamsSetting();
#endif

//STARTUP
void guidance_intern_getStartupParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputStartupSetting();
#endif

//CLASSIFY
int32_t guidance_classifyStrategy;
void guidance_intern_getClassifyParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputClassifySetting();
#endif

//ADAPT
int32_t guidance_adaptStrategy;
void guidance_intern_getAdaptParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputAdaptSetting();
#endif

//PREP
int32_t guidance_adaptPrepStrategy,
			guidance_prepStrategy;
int32_t guidance_adaptPrepStrategyParams,
			guidance_prepReviveCls,
			guidance_prepUNHRounds,
			guidance_prepACCEMax,
			guidance_prepACCEMin,
			guidance_prepACCEMinClsSize,
			guidance_prepACCESaturate,
			guidance_prepNIVERMax,
			guidance_prepATEMin,
			guidance_prepGEMaxAONN,
			guidance_prepGEMaxXOR,
			guidance_prepTERNARYLitMax,
			guidance_prepTERNARYMax,
			guidance_prepNHBRMin;
void guidance_intern_getPrepParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputPrepSetting();
#endif

//INP
int32_t guidance_adaptInpStrategy,
			guidance_inpStrategy;
int32_t guidance_adaptInpStrategyParams,
			guidance_inpStrMaxSize,
			guidance_inpTernaryLitMax,
			guidance_inpTernaryMax,
			guidance_inpNiverMax,
			guidance_inpGEMaxAONN,
			guidance_inpGEMaxXOR,
			guidance_inpNHBRMin,
			guidance_inpUNHRounds;
void guidance_intern_getInpParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputInpSetting();
#endif

//SEARCH
int32_t guidance_adaptSearchStrategy,
			guidance_searchStrategy;
int32_t guidance_adaptSearchStrategyParams;
float_ty 	guidance_searchMIDBlockSize,
			guidance_searchMinClsImpact,
			guidance_searchImpactMaxFactor,
			guidance_searchMaxMagForSLS,
			guidance_searchRetreatFact,
			guidance_searchMPWeightModSAT,
			guidance_searchMPWeightModUNS;
void guidance_intern_getSearchParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSearchSetting();
#endif

//HANDLERESULT
void guidance_intern_getHandleresultParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputHandleresultSetting();
#endif

//SHUTDOWN
void guidance_intern_getShutdownParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputShutdownSetting();
#endif

//Modules and their parameters
//DETERM
//Has no parameters yet.
void guidance_intern_getDETERMParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputDETERMSetting();
#endif

//SPECCER
//Has no parameters yet.
void guidance_intern_getSPECCERParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSPECCERSetting();
#endif

//LOADER
//Has no parameters yet.
void guidance_intern_getLOADERParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputLOADERSetting();
#endif

//ATTRIBUTOR
//Has no parameters yet.
void guidance_intern_getATTRIBUTORParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputATTRIBUTORSetting();
#endif

//RANDOMFOREST
//Has no parameters yet.
void guidance_intern_getRANDOMFORESTParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputRANDOMFORESTSetting();
#endif

//ITEADAPTER
//Has no parameters yet.
void guidance_intern_getITEADAPTERParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputITEADAPTERSetting();
#endif

//PRE
//Has no parameters yet.
void guidance_intern_getPREParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputPRESetting();
#endif

//SATELITE
//Has no parameters yet.
void guidance_intern_getSATELITEParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSATELITESetting();
#endif

//IN
//Has no parameters yet.
void guidance_intern_getINParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputINSetting();
#endif


//MP
//MP Update Rule
int32_t guidance_adaptMpUR,
			guidance_mpUpdateRule;
int32_t guidance_adaptMpURParams;
float_ty 	guidance_mpRho,
			guidance_mpSigma,
			guidance_mpPhi,
			guidance_mpUpsilon,
			guidance_mpMaxConvergenceDiff;
void guidance_intern_getMPParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputMPSetting();
#endif

//SLS
//SLS Assignment Init Rule
int32_t guidance_adaptSlsAIR,
			guidance_slsAssInitRule;
int32_t guidance_adaptSlsAIRParams;
			//Currently, there are no parameters.

//SLS Try Restart Rule
int32_t guidance_adaptSlsTRR,
			guidance_slsTryRestartRule;
int32_t guidance_adaptSlsTRRParams;
float_ty 	guidance_slsMaxFlipsA,
			guidance_slsMaxFlipsB;
int32_t 	guidance_slsMaxTries;

//SLS Pick and Flip Variable Rule
int32_t guidance_adaptSlsPAFVR,
			guidance_slsPickAndFlipVarRule;
int32_t guidance_adaptSlsPAFVRParams;
float_ty 	guidance_slsNoise,
			guidance_slsProbsatCB,
			guidance_slsProbsatCBShift;
void guidance_intern_getSLSParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSLSSetting();
#endif

//CDCL
//CDCL Restart Rule
int32_t guidance_adaptCdclRR,
			guidance_cdclRestartRule;
int32_t guidance_adaptCdclRRParams,
 			guidance_cdclRestartInnerouterInit,
 			guidance_cdclRestartLubyUnit;
float_ty 	guidance_cdclRestartStagMaxFrac,
			guidance_cdclRestartInnerouterInc,
			guidance_cdclRestartVarAgiIncBump,
			guidance_cdclRestartMaxAgiFrac,
			guidance_cdclRestartMaxAgiOutFrac;

//CDCL Select Variable Rule (with activity initialization)
int32_t guidance_adaptCdclSVR,
			guidance_cdclSelectVarRule;
int32_t guidance_adaptCdclSVRParams;
float_ty 	guidance_cdclSelectVarActIncBump,
			guidance_cdclSelectVarRandProb;
int32_t guidance_adaptCdclSVAI,
			guidance_cdclSelectVarActInit;
int32_t guidance_adaptCdclSVAIParams;
			//Currently, there are no parameters.

//CDCL Select Direction Rule (with direction initialization)
int32_t guidance_adaptCdclSDR,
			guidance_cdclSelectDirRule;
int32_t guidance_adaptCdclSDRParams,
			guidance_cdclSelectDirMinFlipDist;
int32_t guidance_adaptCdclSDDI,
			guidance_cdclSelectDirDirInit;
int32_t guidance_adaptCdclSDDIParams;
			//Currently, there are no parameters.

//CDCL Conflict Analysis Rule
int32_t guidance_adaptCdclCAR,
			guidance_cdclConflictAnalysisRule;
int32_t guidance_adaptCdclCARParams;
			//Currently, there are no parameters.

//CDCL Maintenance Rule
int32_t guidance_adaptCdclMR,
			guidance_cdclMaintenanceRule;
int32_t guidance_adaptCdclMRParams,
			guidance_cdclMaintenanceBase,
			guidance_cdclMaintenanceBoost;
float_ty 	guidance_cdclMaintenanceMinDev,
			guidance_cdclMaintenanceMaxAvg,
			guidance_cdclMaintenanceActIncBump,
			guidance_cdclMaintenanceInitial,
			guidance_cdclMaintenanceIncrease;

//CDCL Inprocessing Rule
int32_t guidance_adaptCdclIR,
			guidance_cdclInprocessingRule;
int32_t guidance_adaptCdclIRParams;
int32_t 	guidance_cdclIRUNHRounds,
			guidance_cdclIRTERNARYLitMax,
			guidance_cdclIRTERNARYMax,
			guidance_cdclIRNIVERMax,
			guidance_cdclIRGEMaxAONN,
			guidance_cdclIRGEMaxXOR,
			guidance_cdclIRStrMaxSize,
			guidance_cdclIRNHBRMin;
float_ty 	guidance_cdclIRMinConfDistance;

//CDCL Strengthen Learned Rule
int32_t guidance_adaptCdclSLR,
			guidance_cdclStrLearnedRule;
int32_t guidance_adaptCdclSLRParams;
			//Currently, there are no parameters.

//CDCL Strengthen Other Rule
int32_t guidance_adaptCdclSOR,
			guidance_cdclStrOtherRule;
int32_t guidance_adaptCdclSORParams;
			//Currently, there are no parameters.

//CDCL Compute BJ Level Rule
int32_t guidance_adaptCdclCBJLR,
			guidance_cdclComputeBJLRule;
int32_t guidance_adaptCdclCBJLRParams;
			//Currently, there are no parameters.

//CDCL Abort Rule
int32_t guidance_adaptCdclAR,
			guidance_cdclAbortRule;
int32_t guidance_adaptCdclARParams;
			//Currently, there are no parameters.
void guidance_intern_getCDCLParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputCDCLSetting();
#endif

//SOLWRITER
//Has no parameters yet.
void guidance_intern_getSOLWRITERParams(int, char**);
#ifdef VERBOSE_GUIDANCE
void guidance_intern_outputSOLWRITERSetting();
#endif

//Then, we use the following macro to check that a specific parameter has not been set.
#define GUIDANCE_CHECKUNSET( __gpar ) { \
  if ( __gpar != -1 ){ \
	  guidance_returnCode = CONFLICTINGPARAMS; \
	  printf("c   GUIDANCE:   ERROR: Capability " #__gpar " should not be set, but it is.\n"); \
  } \
}

//Then, we use the following macro to explicitly forbid a parameter setting.
#define GUIDANCE_FORBID( __gpar , __spar , __forbidden ) { \
  if ( __gpar != -1 && __spar == __forbidden ){ \
	  guidance_returnCode = CONFLICTINGPARAMS; \
	  printf("c   GUIDANCE:   ERROR: Capability " #__gpar " has forbidden value " #__forbidden " but was provided that way.\n"); \
  }\
}
//Then, we use the following macro to apply default values in case a parameter was not given.
#define GUIDANCE_APPLYDEFAULT(__gpar , __spar , __default) { \
  if ( __gpar == -1 ){ \
	  __spar = __default; \
  } \
}
//Finally, we use the following macro to simplify the source a bit. It checks whether a parameter is set, if it is set it is
//set to the required value and if it is not set, enforces the default.
#define GUIDANCE_CHECKANDSET( __gpar , __spar , __required ) { \
  if ( __gpar != -1 ){ \
	if ( __gpar != __required ){ \
		guidance_returnCode = CONFLICTINGPARAMS; \
		printf("c   GUIDANCE:   ERROR: Capability " #__gpar " has required value " #__required " but was provided otherwise.\n"); \
	} \
  } else { \
	  __spar = __required; \
  }\
}
#endif /* GUIDANCE_H_ */
