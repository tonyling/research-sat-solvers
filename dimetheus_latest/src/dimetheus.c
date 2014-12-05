/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 *
 *  The following disclaimer is stated only once, but is applicable for
 * 	all files contained in the BOSSLS project:
 *
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, version 3. This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details. You should have received a copy of the GNU General Public License along with
 *  this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dimetheus.h"

void main_reset(){
	#ifndef SILENT
	printf("c RESET...\n");
	#endif
	reset_resetPhase();
	reset_initPhase();
	if (reset_returnCode == RESET_UNKNOWN) reset_extern_resetAll();
	if (reset_returnCode == RESET_ERROR){
		main_returnCode = ERROR;
	} else if (reset_returnCode == RESET_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	reset_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_params(int argc, char** argv){
	#ifndef SILENT
	printf("c PARAMS...\n");
	#endif
	params_initPhase();
	if (params_returnCode == PARAMS_UNKNOWN) params_extern_readSpecial(argc, argv);
	if (params_returnCode == PARAMS_UNKNOWN) params_extern_readNormal(argc, argv);
	if (params_returnCode == PARAMS_ERROR){
		main_returnCode = ERROR;
	} else if (params_returnCode == PARAMS_SPECIALPARAM_HELP){
		main_returnCode = SPECIALPARAM_HELP;
	} else if (params_returnCode == PARAMS_SPECIALPARAM_VERSION){
		main_returnCode = SPECIALPARAM_VERSION;
	} else if (params_returnCode == PARAMS_SPECIALPARAM_SPECS){
		main_returnCode = SPECIALPARAM_SPECS;
	} else if (params_returnCode == PARAMS_BADPARAM){
		main_returnCode = BADPARAM;
	} else if (params_returnCode == PARAMS_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	params_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_guidance(int argc, char** argv){
	#ifndef SILENT
	printf("c GUIDANCE...\n");fflush(stdout);
	#endif
	guidance_initPhase();
	if (guidance_returnCode == GUIDANCE_UNKNOWN) guidance_extern_apply(argc, argv);
	if (guidance_returnCode == GUIDANCE_CONFLICTINGPARAMS){
		main_returnCode = CONFLICTINGPARAMS;
	} else if (guidance_returnCode == GUIDANCE_ERROR){
		main_returnCode = ERROR;
	} else if (guidance_returnCode == GUIDANCE_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	guidance_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_startup(){
	#ifndef SILENT
	printf("c STARTUP...\n");fflush(stdout);
	#endif
	startup_initPhase();
	if (startup_returnCode == STARTUP_UNKNOWN) startup_extern_start();
	if (startup_returnCode == STARTUP_ERROR){
		main_returnCode = ERROR;
	} else if (startup_returnCode == STARTUP_BADFORMULA){
		main_returnCode = BADFORMULA;
	} else if (startup_returnCode == STARTUP_BADINPUTFORMAT){
		main_returnCode = BADINPUTFORMAT;
	} else if (startup_returnCode == STARTUP_UNKNOWN){
		main_returnCode = UNKNOWN;
	} else if (startup_returnCode == STARTUP_SAT){
		main_returnCode = SAT;
	} else if (startup_returnCode == STARTUP_UNSAT){
		main_returnCode = UNSAT;
	}
	startup_disposePhase();

	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_classify(){
	#ifndef SILENT
	printf("c CLASSIFY...\n");fflush(stdout);
	#endif
	classify_initPhase();
	if (classify_returnCode == CLASSIFY_UNKNOWN) classify_extern_classify();
	if (classify_returnCode == CLASSIFY_ERROR){
		main_returnCode = ERROR;
	} else if (classify_returnCode == CLASSIFY_UNKNOWN){
		main_returnCode = UNKNOWN;
	} else if (classify_returnCode == CLASSIFY_SAT){
		main_returnCode = SAT;
	} else if (classify_returnCode == CLASSIFY_UNSAT){
		main_returnCode = UNSAT;
	}
	classify_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_adapt(){
	#ifndef SILENT
	printf("c ADAPT...\n");fflush(stdout);
	#endif
	adapt_initPhase();
	if (adapt_returnCode == UNKNOWN) adapt_extern_adaptation();
	if (adapt_returnCode == ADAPT_ERROR){
		main_returnCode = ERROR;
	} else if (adapt_returnCode == ADAPT_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	adapt_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_preprocess(){
	#ifndef SILENT
	printf("c PREPROCESS...\n");fflush(stdout);
	#endif
	prep_initPhase();
	if (prep_returnCode == PREP_UNKNOWN) prep_extern_preprocessing();
	if (prep_returnCode == PREP_ERROR){
		main_returnCode = ERROR;
	} else if (prep_returnCode == PREP_UNSAT){
		main_returnCode = UNSAT;
	} else if (prep_returnCode == PREP_SAT){
		main_returnCode = SAT;
	} else if (prep_returnCode == PREP_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	prep_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_search(){
	#ifndef SILENT
	printf("c SEARCH...\n");fflush(stdout);
	#endif
	search_initPhase();
	if (search_returnCode == SEARCH_UNKNOWN) search_extern_searching();
	if (search_returnCode == SEARCH_ERROR){
		main_returnCode = ERROR;
	} else if (search_returnCode == SEARCH_UNSAT){
		main_returnCode = UNSAT;
	} else if (search_returnCode == SEARCH_SAT){
		main_returnCode = SAT;
	} else if (search_returnCode == SEARCH_UNKNOWN){
		main_returnCode = UNKNOWN;
	}
	search_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_handleresult(){
	#ifndef SILENT
	printf("c HANDLERESULT...\n");fflush(stdout);
	#endif
	handleresult_initPhase();
	if (handleresult_returnCode == HANDLERESULT_UNKNOWN) handleresult_extern_handle();
	if (handleresult_returnCode == HANDLERESULT_ERROR){
		main_returnCode = ERROR;
	} else if (handleresult_returnCode == HANDLERESULT_WRONGSOLUTION){
		main_returnCode = WRONGSOLUTION;
	}
	handleresult_disposePhase();
	#ifdef VERBOSE
	printf("c\n");fflush(stdout);
	#endif
}

void main_shutdown(){
	#ifndef SILENT
	printf("c SHUTDOWN...\n");fflush(stdout);
	#endif
	shutdown_initPhase();
	if (shutdown_returnCode == SHUTDOWN_UNKNOWN) shutdown_extern_shutdown();
	shutdown_disposePhase();
}

void main_printSolverHeader(){
	#ifndef SILENT
	printf("c DIMETHEUS SAT Solver\n"
			"c   Version: %d.%d.%d\n"
			"c   Author:  Oliver Gableske (oliver@gableske.net)\n"
			"c   All rights reserved.\n"
			"c   Website: See https://www.gableske.net/dimetheus\n"
			"c   License: INFO: See ./doc/license.txt. Contact the author if the file is missing.\n"
			"c\n", VERSION_MA, VERSION_MI, VERSION_RE);
	#endif
}

void main_printVersion(){
	printf("c Version information:\n");
	printf("c  DIMETHEUS: %d.%d ", VERSION_MA, VERSION_MI);
	//Check if we compile on a 32 bit system.
	#ifdef SYS32BIT
	//We are compiling for a 32-bit system.
	printf(" (32 bit)\n");
	#else
	//We are compiling for a 64-bit system
	printf(" (64 bit)\n");
	#endif
	printf("c    SVN revision (Head): %d\n", VERSION_RE);
	printf("c    SVN check in (Time): %s\n", VERSION_CI);
	printf("c  Module versions:\n");
	printf("c    NUM [MODULE FOR PHASE] [TYPE/TASK                      ] :: VERSION  :: MODULE NAME\n");
	printf("c    000 "); determ_extern_printVersion(); printf("\n");
	printf("c    010 "); speccer_extern_printVersion(); printf("\n");
	printf("c    020 "); loader_extern_printVersion(); printf("\n");
	printf("c    030 "); attributor_extern_printVersion(); printf("\n");
	printf("c    040 "); randomforest_extern_printVersion(); printf("\n");
	printf("c    050 "); iteadapter_extern_printVersion(); printf("\n");
	printf("c    060 "); pre_extern_printVersion(); printf("\n");
	printf("c    061 "); satelite_extern_printVersion(); printf("\n");
	printf("c    070 "); in_extern_printVersion(); printf("\n");
	printf("c    080 "); mp_extern_printVersion(); printf("\n");
	printf("c    081 "); sls_extern_printVersion(); printf("\n");
	printf("c    082 "); cdcl_extern_printVersion(); printf("\n");
	printf("c    090 "); solwriter_extern_printVersion(); printf("\n");
	printf("c License Information:\n");
	printf("c  License hash: %s\n", DIMETHEUS_LICENSE_HASH);
	printf("c\n");
}

void main_printSpecs(){
	int32_t i;
	printf("c Binary specifications:\n");
	printf("c MAIN:\n");
	printf("c   MAIN: FLAGS-GENERAL: ");
	#ifdef VERBOSE
	printf("VERBOSE ");
	#endif
	#ifdef COLLECTSTATS
	printf("COLLECTSTATS ");
	#endif
	#ifdef SCORING
	printf("SCORING ");
	#endif
	#ifdef NEWSCHOOLRAND
	printf("NEWSCHOOLRAND ");
	#endif
	#ifdef DOUBLEMODE
	printf("DOUBLEMODE ");
	#endif
	printf("\n");
	printf("c   MAIN: FLAGS-PHASES: ");
	#ifdef VERBOSE_RESET
	printf("VERBOSE_RESET ");
	#endif
	#ifdef VERBOSE_PARAMS
	printf("VERBOSE_PARAMS ");
	#endif
	#ifdef VERBOSE_STARTUP
	printf("VERBOSE_PARAMS ");
	#endif
	#ifdef VERBOSE_GUIDANCE
	printf("VERBOSE_GUIDANCE ");
	#endif
	#ifdef VERBOSE_CLASSIFY
	printf("VERBOSE_CLASSIFY ");
	#endif
	#ifdef VERBOSE_CLASSIFY_ATTVALUE
	printf("VERBOSE_CLASSIFY_ATTVALUE ");
	#endif
	#ifdef VERBOSE_CLASSIFY_ATTCLASS
	printf("VERBOSE_CLASSIFY_ATTCLASS ");
	#endif
	#ifdef VERBOSE_ADAPT
	printf("VERBOSE_ADAPT ");
	#endif
	#ifdef VERBOSE_PREP
	printf("VERBOSE_PREP ");
	#endif
	#ifdef VERBOSE_SEARCH
	printf("VERBOSE_SEARCH ");
	#endif
	#ifdef VERBOSE_INP
	printf("VERBOSE_INP ");
	#endif
	printf("\n");
	printf("c   MAIN: FLAGS-MODULES: ");
	#ifdef VERBOSE_DETERM
	printf("VERBOSE_DETERM ");
	#endif
	#ifdef VERBOSE_SPECCER
	printf("VERBOSE_LOADER ");
	#endif
	#ifdef VERBOSE_LOADER
	printf("VERBOSE_LOADER ");
	#endif
	#ifdef VERBOSE_ATTRIBUTOR
	printf("VERBOSE_ATTRIBUTOR ");
	#endif
	#ifdef VERBOSE_RANDOMFOREST
	printf("VERBOSE_RANDOMFOREST ");
	#endif
	#ifdef VERBOSE_ITEADAPTER
	printf("VERBOSE_ITEADAPTER ");
	#endif
	#ifdef VERBOSE_PRE
	printf("VERBOSE_PRE ");
	#endif
	#ifdef VERBOSE_SATELITE
	printf("VERBOSE_SATELITE ");
	#endif
	#ifdef VERBOSE_IN
	printf("VERBOSE_IN ");
	#endif
	#ifdef VERBOSE_MP
	printf("VERBOSE_MP ");
	#endif
	#ifdef VERBOSE_SLS
	printf("VERBOSE_SLS ");
	#endif
	#ifdef VERBOSE_CDCL
	printf("VERBOSE_CDCL ");
	#endif
	#ifdef VERBOSE_SOLWRITER
	printf("VERBOSE_SOLWRITER ");
	#endif
	printf("\n");
	printf("c   MAIN: MEMORY: Data structures memory size in bits:\n");
	printf("c   MAIN: MEMORY:   Size of address data-type:             %d\n", (int32_t) (8*sizeof(int32_t*)));
	printf("c   MAIN: MEMORY:   Size of long data-type:                %d\n", (int32_t) (8*sizeof(int64_t)));
	printf("c   MAIN: MEMORY:   Size of floating point data-type:      %d\n", (int32_t) (8*sizeof(float_ty)));
	printf("c   MAIN: MEMORY:   Size of clause data-type:              %d\n", (int32_t) (8*sizeof(clause)));
	printf("c   MAIN: MEMORY:   Size of removed clause data-type:      %d\n", (int32_t) (8*sizeof(remClause)));
	printf("c   MAIN: MEMORY:   Size of variable data-type:            %d\n", (int32_t) (8*sizeof(variable)));
	printf("c   MAIN: MEMORY:   Size of literal data-type:             %d\n", (int32_t) (8*sizeof(literal)));
	printf("c   MAIN: MEMORY:   Size of CNF data-type:                 %d\n", (int32_t) (8*sizeof(CNF)));
	printf("c   MAIN: LIMITS: Limits/constants:\n");
	printf("c   MAIN: LIMITS:   Maximum number of variables supported: %u\n", VAR_OFFSET_MASK);
	printf("c   MAIN: LIMITS:   Maximum number of clauses supported:   %u\n", CLS_OFFSET_MASK);
	printf("c   MAIN: LIMITS:   RANDMAX = %u\n", RANDMAX);
	printf("c   MAIN: LIMITS:   OORANDMAX = 1.0/RANDMAX: %1.32f\n", OORANDMAX);
	printf("c   RESET Phase:\n");
	printf("c     RESET: nothing to report.\n");
	printf("c   PARAMS Phase:\n");
	params_extern_printAdaptableWithPreconditions("c     PARAMS: PARAM-ADAPT-PRECONDITION-PARAMS:");
	params_extern_printFuzzedParametrs("c     PARAMS: PARAM-RANDOM-SETTING:");
	#ifdef SPECS_PARAMGRAPH
	params_extern_writeParameterGraph("c     PARAMS: PARAM-GRAPH-CREATION:");
	#endif
	printf("c   STARTUP Phase:\n");
	startup_extern_printKnownFormats("c     STARTUP: KNOWN-FORMATS: ");
	printf("c   GUIDANCE Phase:\n");
	for (i = 0; i < GUIDANCE_NUMCANBEADAPTED; ++i){
		printf("c     GUIDANCE: ADAPTABLE-GUIDES: ");
		guidance_extern_printNameOfGuide(guidance_canBeAdapted[i]);printf(" %d\n", guidance_canBeAdapted[i]);
	}
	printf("c   CLASSIFY Phase:\n");
	classify_extern_printKnownDomains("c     CLASSIFY: KNOWN-DOMAINS: ");
	printf("c     CLASSIFY: FLOAT-PRECISION: %d\n", PARAM_DEFAULT_CLASSIFYFLOATPREC);
	#ifdef VERBOSE_CLASSIFY_ATTVALUE
	printf("c     CLASSIFY: FORMULA-ATTNAMES-GENERIC: ");MAIN_PRINT_FATTNAMES_GENERIC();printf("\n");
	printf("c     CLASSIFY: FORMULA-ATTNAMES-RANDOM: ");MAIN_PRINT_FATTNAMES_RANDOM();printf("\n");
	printf("c     CLASSIFY: FORMULA-ATTNAMES-CRAFTED: ");MAIN_PRINT_FATTNAMES_CRAFTED();printf("\n");
	printf("c     CLASSIFY: FORMULA-ATTNAMES-APPLICATION: ");MAIN_PRINT_FATTNAMES_APPLICATION();printf("\n");
	#endif
	classify_strategies_extern_printStrategyNamesAndIDs("c     CLASSIFY: STRATEGY-NAMES-IDS:");
	printf("c   ADAPT Phase:\n");
	adapt_strategies_extern_printStrategyNamesAndIDs("c     ADAPT: STRATEGY-NAMES-IDS:");
	printf("c   PREP Phase:\n");
	prep_strategies_extern_printStrategyNamesAndIDs("c     PREP: STRATEGY-NAMES-IDS:");
	printf("c   INP Phase:\n");
	inp_strategies_extern_printStrategyNamesAndIDs("c     INP: STRATEGY-NAMES-IDS:");
	printf("c   SEARCH Phase:\n");
	search_strategies_extern_printStrategyNamesAndIDs("c     SEARCH: STRATEGY-NAMES-IDS:");
	printf("c   SEARCH Modules:\n");
	mp_extern_updateRules_printNamesAndIDs("c     SEARCH: MP: RULE-NAMES-IDS:");
	sls_extern_pickAndFlipVarRules_printNamesAndIDs("c     SEARCH: SLS: RULE-NAMES-IDS:");
	sls_extern_assInitRules_printNamesAndIDs("c     SEARCH: SLS: RULE-NAMES-IDS:");
	sls_extern_tryRestartRules_printNamesAndIDs("c     SEARCH: SLS: RULE-NAMES-IDS:");
	cdcl_extern_selectVarRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_selectDirRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_conflictAnalysisRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_strLearnedRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_strOtherRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_computeBJLRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_restartRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_maintenanceRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_inprocessingRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	cdcl_extern_abortRules_printNamesAndIDs("c     SEARCH: CDCL: RULE-NAMES-IDS:");
	printf("c\n");
}


#ifndef COMPILEASLIBRARY
void main_handle_signal(int signal){
	#ifndef SILENT
    printf("c\nc SIGNAL %d RECEIVED.\nc DISPOSING ALL AND SHUTTING DOWN...\nc\n", signal);
	#endif
    main_returnCode = SIGNAL;
	main_handleresult();
	main_shutdown();
    fflush(stdout);
    exit(main_returnCode);
}

int32_t main(int argc, char** argv){
	main_printSolverHeader();

	main_reset();

	if (main_returnCode == UNKNOWN) main_params(argc,argv);
	if (main_returnCode == UNKNOWN) main_guidance(argc,argv);

	if (main_returnCode == UNKNOWN){
		main_startup();

		if (main_returnCode == UNKNOWN)	main_classify();
		if (main_returnCode == UNKNOWN)	main_adapt();
		if (main_returnCode == UNKNOWN)	main_preprocess();
		if (main_returnCode == UNKNOWN) main_search();
	}

	main_handleresult();
	main_shutdown();

	return main_returnCode;
}
#endif
