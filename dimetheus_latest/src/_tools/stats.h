/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef STATS_H_
#define STATS_H_

#include "tools.h"

#ifdef COLLECTSTATS
//MAIN
float_ty stats_main_time_init;

//RESET
float_ty stats_reset_time_total;
uint64_t stats_reset_component_totalCalls;

//PARAMS
float_ty stats_params_time_total;
uint64_t stats_params_component_totalCalls;

//GUIDANCE
float_ty stats_guidance_time_total;
uint64_t stats_guidance_component_totalCalls;

//STARTUP
float_ty stats_startup_time_total;
uint64_t stats_startup_component_totalCalls;

//DETERM
float_ty stats_determ_time_total;
uint64_t stats_determ_component_totalCalls;

//SPECCER
float_ty stats_speccer_time_total;
uint64_t stats_speccer_component_totalCalls;

//LOADER
float_ty stats_loader_time_total;
uint64_t stats_loader_component_totalCalls;
uint32_t stats_loader_numVariables;
uint32_t stats_loader_numInitialUnits;
uint32_t stats_loader_numInitialClauses;

//PREP
float_ty stats_prep_time_total;
float_ty stats_prep_clauseCoverRateOrig;
float_ty stats_prep_clauseCoverRatePrep;
uint64_t stats_prep_component_totalCalls;

//CLASSIFY
float_ty stats_classify_time_total;
float_ty stats_classify_time_simplify;
uint64_t stats_classify_assignedVars;
uint64_t stats_classify_component_totalCalls;

//ATTRIBUTOR
float_ty stats_attributor_time_init;
float_ty stats_attributor_time_total;
float_ty stats_attributor_time_BASIC;
float_ty stats_attributor_time_VCG;
float_ty stats_attributor_time_VG;
float_ty stats_attributor_time_CG;
float_ty stats_attributor_time_B;
float_ty stats_attributor_time_HP;
float_ty stats_attributor_time_BIG;
float_ty stats_attributor_time_UP;
float_ty stats_attributor_time_RW;
float_ty stats_attributor_time_ELS;
uint64_t stats_attributor_numTimeouts;
uint64_t stats_attributor_numSkipped;
uint64_t stats_attributor_component_totalCalls;

//RANDOMFOREST
float_ty stats_randomforest_time_init;
float_ty stats_randomforest_time_total;
uint64_t stats_randomforest_component_totalCalls;

//ADAPT
float_ty stats_adapt_time_total;
uint64_t stats_adapt_component_totalCalls;

//ITEADAPTER
float_ty stats_iteadapter_time_total;
float_ty stats_iteadapter_time_init;
uint64_t stats_iteadapter_component_totalCalls;

//PRE
float_ty stats_pre_time_total;
uint64_t stats_pre_component_totalCalls;
int64_t stats_pre_varRemoved;
int64_t stats_pre_varRemoved_SE;
int64_t stats_pre_varRemoved_FLD;
int64_t stats_pre_varRemoved_PLE;
int64_t stats_pre_varRemoved_STR;
int64_t stats_pre_varRemoved_NIVER;
int64_t stats_pre_varRemoved_ATE;
int64_t stats_pre_varRemoved_ACCE;
int64_t stats_pre_varRemoved_GE;
int64_t stats_pre_varRemoved_UNHIDING;
int64_t stats_pre_varRemoved_NHBR;
int64_t stats_pre_varRemoved_TERNARY;
int64_t stats_pre_varRemoved_ELS;
int64_t stats_pre_clsRemoved;
int64_t stats_pre_clsRemoved_SE;
int64_t stats_pre_clsRemoved_FLD;
int64_t stats_pre_clsRemoved_PLE;
int64_t stats_pre_clsRemoved_STR;
int64_t stats_pre_clsRemoved_NIVER;
int64_t stats_pre_clsRemoved_ATE;
int64_t stats_pre_clsRemoved_ACCE;
int64_t stats_pre_clsRemoved_GE;
int64_t stats_pre_clsRemoved_UNHIDING;
int64_t stats_pre_clsRemoved_NHBR;
int64_t stats_pre_clsRemoved_TERNARY;
int64_t stats_pre_clsRemoved_ELS;

//SATELITE
float_ty stats_satelite_time_total;
uint64_t stats_satelite_component_totalCalls;

//INP related.
float_ty stats_inp_time_total;
uint64_t stats_inp_component_totalCalls;

//IN
float_ty stats_in_time_total;
uint64_t stats_in_component_totalCalls;
int64_t stats_in_varRemoved;
int64_t stats_in_varRemoved_SE;
int64_t stats_in_varRemoved_FLD;
int64_t stats_in_varRemoved_PLE;
int64_t stats_in_varRemoved_STR;
int64_t stats_in_clsRemoved;
int64_t stats_in_clsRemoved_SE;
int64_t stats_in_clsRemoved_FLD;
int64_t stats_in_clsRemoved_PLE;
int64_t stats_in_clsRemoved_STR;

//SEARCH related.
float_ty stats_search_time_total;
float_ty stats_search_time_varSorting;
float_ty stats_search_time_prepareAss;
uint64_t stats_search_component_totalCalls;
uint64_t* stats_search_impactsB;
uint64_t stats_search_childClausesRetainedB;
uint64_t* stats_search_impactsL;
uint64_t stats_search_childClausesRetainedL;
uint64_t stats_search_restarts;

//MP
float_ty stats_mp_time_total;
float_ty stats_mp_time_init;
float_ty stats_mp_time_biasCompute;
float_ty stats_mp_time_iterating;
float_ty stats_mp_time_retaining;
uint64_t stats_mp_component_totalCalls;
uint64_t stats_mp_return_CONVERGED;
uint64_t stats_mp_return_UNCONVERGED;
uint64_t stats_mp_return_PARAMAG;
uint64_t stats_mp_return_NONPARA;
uint64_t stats_mp_return_ERROR;
uint64_t stats_mp_passing_numIterations;
uint64_t stats_mp_passing_totalMsgUpdates;
uint64_t stats_mp_computation_numDiv;
uint64_t stats_mp_computation_numMult;
#define STATS_MP_COUNTHWLITS_MAX 10
uint32_t stats_mp_countedClauses;
uint32_t* stats_mp_hwLits;
#define STATS_MP_COUNTNWLITS_MAX 10
uint32_t* stats_mp_nwLits;
#define STATS_MP_COUNTLWLITS_MAX 10
uint32_t* stats_mp_lwLits;

//SLS
float_ty stats_sls_time_total;
float_ty stats_sls_time_init;
float_ty stats_sls_time_initTry;
float_ty stats_sls_time_flips;
uint64_t stats_sls_component_totalCalls;
uint64_t stats_sls_return_UNKNOWN;
uint64_t stats_sls_return_SAT;
uint64_t stats_sls_return_ERROR;
uint64_t stats_sls_ls_totalTries;
uint64_t stats_sls_ls_totalFlips;

//CDCL
float_ty stats_cdcl_time_total;
float_ty stats_cdcl_time_init;
float_ty stats_cdcl_time_up;
float_ty stats_cdcl_time_confAnalysis;
float_ty stats_cdcl_time_str;
float_ty stats_cdcl_time_maint;
float_ty stats_cdcl_time_reduce;
float_ty stats_cdcl_time_inp;
uint64_t stats_cdcl_component_totalCalls;
uint64_t stats_cdcl_return_UNKNOWN;
uint64_t stats_cdcl_return_SAT;
uint64_t stats_cdcl_return_UNSAT;
uint64_t stats_cdcl_return_ERROR;
uint64_t stats_cdcl_numDecisions;
uint64_t stats_cdcl_numDecisionsRandom;
uint64_t stats_cdcl_numReductions;
uint64_t stats_cdcl_numRedSatCls;
uint64_t stats_cdcl_numRedFalsLits;
uint64_t stats_cdcl_numMaintenances;
uint64_t stats_cdcl_numMaintWithGlue;
uint64_t stats_cdcl_numMaintWithActivity;
uint64_t stats_cdcl_numInprocessing;
uint64_t stats_cdcl_numLearnedRemoved;
uint64_t stats_cdcl_numConflictsFound;
uint64_t stats_cdcl_numClausesLearned;
uint64_t stats_cdcl_numBackjumps;
uint64_t stats_cdcl_numRestarts;
uint64_t stats_cdcl_numRestartProhibits;
uint64_t stats_cdcl_numVarAgiRescales;
uint64_t stats_cdcl_numPropagations;
uint64_t stats_cdcl_numVarActRescales;
uint64_t stats_cdcl_numClsActRescales;
uint64_t stats_cdcl_numStrChecks;
uint64_t stats_cdcl_numStrSuccess;
uint64_t stats_cdcl_numStrSigChecks;
uint64_t stats_cdcl_numStrSigSuccess;
uint64_t stats_cdcl_numStrSuccessAreReasons;
uint64_t stats_cdcl_numStrSuccessNoReasons;
uint64_t stats_cdcl_numStrSuccessIsRepl;
uint64_t stats_cdcl_numSorChecks;
uint64_t stats_cdcl_numSorSuccLarge;
uint64_t stats_cdcl_numSorSuccBin;
uint64_t stats_cdcl_numSorSuccUnit;
uint64_t stats_cdcl_numAbort;
uint64_t stats_cdcl_numAbortAssignmentMin;
uint64_t stats_cdcl_numAbortAssignmentMax;
uint64_t stats_cdcl_numAbortDLZero;
uint64_t stats_cdcl_numAbortClsImpact;
uint64_t* stats_cdcl_decLevelVisited;
#define STATS_CDCL_DECLEVELVISITED_MAX 50
uint64_t* stats_cdcl_learnedLength;
#define STATS_CDCL_LEARNEDLENGTH_MAX 50
uint64_t* stats_cdcl_glueDev;
uint64_t* stats_cdcl_glueAvg;

//HANDLERESULT related.
float_ty stats_handleresult_time_total;
uint64_t stats_handleresult_component_totalCalls;

//SOLWRITER
float_ty stats_solwriter_time_total;
uint64_t stats_solwriter_component_totalCalls;
float_ty stats_solwriter_assignmentBiasAux;
float_ty stats_solwriter_assignmentBiasInp;
float_ty stats_solwriter_assignmentBiasAll;
uint32_t stats_solwriter_numAuxVars;
uint32_t stats_solwriter_numInpVars;
uint32_t stats_solwriter_numAllVars;
uint32_t stats_solwriter_numPosVars;
uint32_t stats_solwriter_numNegVars;
uint32_t stats_solwriter_numUnaVars;

//SHUTDOWN related.
float_ty stats_shutdown_time_total;
uint64_t stats_shutdown_component_totalCalls;

//GENERAL INFORMATION related.
#define STATS_GENERAL_COUNTSATLITS_MAX 10
uint32_t stats_general_countedClauses;
uint32_t* stats_general_satLitsByLength;

void stats_reset();		//To reset everything back to NULL.
void stats_init();		//To initialize, once the formula specifics are known.
void stats_dispose();	//To dispose everything.
void stats_print();		//To print out all statistics.
#endif

#define STATS_GET_CURRENT_TIME_IN_SEC() ((float_ty)clock())/((float_ty)CLOCKS_PER_SEC)

#endif /* STATS_H_ */
