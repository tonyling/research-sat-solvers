/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <time.h>

#define VERSION_MA 2					//The major version number.
#define VERSION_MI 101					//The minor version number.
#define VERSION_RE 1004					//The revision number.
#define VERSION_CI "04. August 2014, 15:44 CET" //The date and time for the revision commit.

//#define COMPILEASLIBRARY				//This define is being used in case the solver is compiled as static library. DO NOT
										//MODIFY THIS HERE. This comment is just for clarifying that this define may exist.
										//If you want to compile the solver as static or dynamic library, go to the source
										//directory and type "make BINTYPE=STATICLIB" or "make BINTYPE=DYNAMICLIB".

//#define COLLECTSTATS					//If you want to collect and output statistics gathered during execution (slow!).
//#define SCORING						//If you want the solver to provide a score in the end, scoring how "well" it went.
#define NEWSCHOOLRAND					//Use the new linear congruence random number generator instead of C build in one.
#define BLOCKSIZE 8						//A block is 8 integers in size. This is the initial size of dynamic arrays. MUST BE
										//1 OR A MULTIPLE OF 2!
#define DOUBLEMODE						//Enable this for maximum precision with floating points.
//#define SPECS_PARAMGRAPH				//In case the EDACC parameter graph is to be created when specs are presented.

//#define SILENT						//In case you want all phases and modules to shut up.
//#define VERYVERBOSE					//In case you want to enable all output. SILENT overrules this.

//#define VERBOSE						//In case you want to see output of the global main component of the solver.
//#define VERBOSE_RESET					//In case you want to see output during the reset phase.
//#define VERBOSE_PARAMS				//In case you want to see output during the parameter reading phase.
//#define VERBOSE_GUIDANCE				//In case you want to see output during the guidance phase.
//#define VERBOSE_STARTUP				//In case you want to see output during the startup phase.
//#define VERBOSE_CLASSIFY				//In case you want to see output during the classification phase.
//#define VERBOSE_CLASSIFY_ATTVALUE		//In case you want to see the values for the formula attributes.
//#define VERBOSE_CLASSIFY_ATTCLASS		//In case you want to see the computed formula class.
//#define VERBOSE_ADAPT					//In case you want to see output during the adaptation phase.
//#define VERBOSE_PREP					//In case you want to see output during the preprocessing phase.
//#define VERBOSE_SEARCH				//In case you want to see output during the search phase.
//#define VERBOSE_INP					//In case you want to see output during the in-processing phase.
//#define VERBOSE_HANDLERESULT			//In case you want to see output during the result-handling phase.
//#define VERBOSE_SHUTDOWN				//In case you want to see output during the shutdown phase.

//#define VERBOSE_DETERM				//In case you want to see the output of the DETERM module.
//#define VERBOSE_SPECCER				//In case you want to see the output of the SPECCER module.
//#define VERBOSE_LOADER				//In case you want to see the output of the LOADER module.
//#define VERBOSE_ATTRIBUTOR			//In case you want to see the output of the ATTRIBUTOR module.
//#define VERBOSE_RANDOMFOREST			//In case you want to see the output of the RANDOMFOREST module.
//#define VERBOSE_ITEADAPTER			//In case you want to see the output of the ITEADAPTER module.
//#define VERBOSE_PRE					//In case you want to see the output of the PRE module.
//#define VERBOSE_SATELITE				//In case you want to see the output of the SATELITE module.
//#define VERBOSE_IN					//In case you want to see the output of the IN module.
//#define VERBOSE_MP					//In case you want to see the output of the MP module.
//#define VERBOSE_SLS					//In case you want to see the output of the SLS module.
//#define VERBOSE_CDCL					//In case you want to see the output of the CDCL module.
//#define VERBOSE_SOLWRITER				//In case you want to see the output of the SOLWRITER module.


//RETURN CODES FOR PHASES.
//MAIN
#define UNKNOWN 0
#define SAT 10
#define UNSAT 20
#define ERROR 30
#define SPECIALPARAM_HELP 31
#define SPECIALPARAM_VERSION 32
#define SPECIALPARAM_SPECS 33
#define WRONGSOLUTION 35
#define BADPARAM 36
#define BADINPUTFORMAT 37
#define BADFORMULA 38
#define CONFLICTINGPARAMS 39
#define SIGNAL 40

//RESET PHASE
#define RESET_UNKNOWN 0
#define RESET_ERROR 40

//PARAMS PHASE
#define PARAMS_UNKNOWN 0
#define PARAMS_SPECIALPARAM_HELP 20
#define PARAMS_SPECIALPARAM_VERSION 21
#define PARAMS_SPECIALPARAM_SPECS 22
#define PARAMS_BADPARAM 39
#define PARAMS_ERROR 40

//GUIDANCE PHASE
#define GUIDANCE_UNKNOWN 0
#define GUIDANCE_CONFLICTINGPARAMS 39
#define GUIDANCE_ERROR 40

//STARTUP PHASE
#define STARTUP_UNKNOWN 0
#define STARTUP_SAT 10
#define STARTUP_UNSAT 20
#define STARTUP_BADINPUTFORMAT 38
#define STARTUP_BADFORMULA 39
#define STARTUP_ERROR 40

//CLASSIFY PHASE
#define CLASSIFY_UNKNOWN 0
#define CLASSIFY_SAT 10
#define CLASSIFY_UNSAT 20
#define CLASSIFY_ERROR 40

//ADAPT PHASE
#define ADAPT_UNKNOWN 0
#define ADAPT_ERROR 40

//PREPROCESSING PHASE
#define PREP_UNKNOWN 1
#define PREP_SAT 11
#define PREP_UNSAT 21
#define PREP_ERROR 31

//INPROCESSING PHASE
#define INP_UNKNOWN 2
#define INP_SAT 12
#define INP_UNSAT 22
#define INP_ERROR 32

//SEARCH PHASE
#define SEARCH_UNKNOWN 3
#define SEARCH_SAT 13
#define SEARCH_UNSAT 23
#define SEARCH_ERROR 33

//HANDLERESULT PHASE
#define HANDLERESULT_UNKNOWN 0
#define HANDLERESULT_CORRECTSOLUTION 10
#define HANDLERESULT_WRONGSOLUTION 39
#define HANDLERESULT_ERROR 40

//SHUTDOWN PHASE
#define SHUTDOWN_UNKNOWN 0
#define SHUTDOWN_ERROR 40

//RETURN CODES FOR MODULES
//DETERM MODULE
#define DETERM_UNKNOWN 0
#define DETERM_ERROR 40

//SPECCER MODULE
#define SPECCER_UNKNOWN 0
#define SPECCER_BADTOKEN 39
#define SPECCER_ERROR 40

//LOADER MODULE
#define LOADER_UNKNOWN 0
#define LOADER_SAT 10
#define LOADER_UNSAT 20
#define LOADER_BADFORMULA 39
#define LOADER_ERROR 40

//ATTRIBUTOR MODULE
#define ATTRIBUTOR_UNKNOWN 0
#define ATTRIBUTOR_ERROR 40

//RANDOMFOREST MODULE
#define RANDOMFOREST_UNKNOWN 0
#define RANDOMFOREST_ERROR 40

//ITEADAPTER MODULE
#define ITEADAPTER_UNKNOWN 0
#define ITEADAPTER_ERROR 40

//PRE MODULE
#define PRE_UNKNOWN 0
#define PRE_SAT 10
#define PRE_UNSAT 20
#define PRE_ERROR 40

//SATELITE MODULE
#define SATELITE_UNKNOWN 0
#define SATELITE_SAT 10
#define SATELITE_UNSAT 20
#define SATELITE_ERROR 40

//IN MODULE
#define IN_UNKNOWN 0
#define IN_SAT 10
#define IN_UNSAT 20
#define IN_ERROR 40

//MP MODULE
#define MP_UNKNOWN 0
#define MP_CONVERGED 10
#define MP_CONVERGED_PARAMAG 11
#define MP_CONVERGED_NONPARA 12
#define MP_UNCONVERGED 20
#define MP_ERROR 40

//SLS MODULE
#define SLS_UNKNOWN 0
#define SLS_SAT 10
#define SLS_ERROR 40

//CDCL MODULE
#define CDCL_UNKNOWN 0
#define CDCL_SAT 10
#define CDCL_UNSAT 20
#define CDCL_ERROR 40

//SOLWRITER MODULE
#define SOLWRITER_UNKNOWN 0
#define SOLWRITER_ERROR 40

#ifdef DOUBLEMODE
typedef double float_ty;
#define VAR_MAX_ACTIVITY_VALUE 1e100
#define VAR_MAX_AGILITY_VALUE 1e100
#define CLS_MAX_ACTIVITY_VALUE 1e100
#define VQ_SCORE_FACTOR 1000000000.0
#define THREE 3.0
#define TWO 2.0
#define ONE 1.0
#define HALF 0.5
#define ZERO 0.0
#define LARGEFLOAT 100000000.0
#else
typedef float float_ty;
#define VAR_MAX_ACTIVITY_VALUE 2147483648.0f
#define VAR_MAX_AGILITY_VALUE 2147483648.0f
#define CLS_MAX_ACTIVITY_VALUE 2147483648.0f
#define VQ_SCORE_FACTOR 1000000000.0f
#define THREE 3.0f
#define TWO 2.0f
#define ONE 1.0f
#define HALF 0.5f
#define ZERO 0.0f
#define LARGEFLOAT 100000000.0f
#endif

//Check if we compile on a 32 bit system.
#if !(defined __LP64__ || defined __LLP64__) || defined _WIN32 && !defined _WIN64
//We are compiling for a 32-bit system.
typedef int32_t addr_t;
#define SYS32BIT
#else
//We are compiling for a 64-bit system
typedef int64_t addr_t;
#define SYS64BIT
#endif

//Static global functions.
static inline uint32_t POPCOUNT_UINT64(uint64_t x){//Computes number of set bits in x. Knuth TAOCP Volume 4 F1A, page 11.
	x = x-( (x>>1) & 0x5555555555555555ull);
	x = (x & 0x3333333333333333ull) + ((x >> 2) & 0x3333333333333333ull);
	x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0full;
	return (0x0101010101010101ull*x >> 56);
}

static inline void PRINT_BITS_UINT32(uint32_t x){
	int32_t counter;
	for (counter = 31; counter > -1; --counter){ \
		printf("%d", (int32_t)(( x >> counter ) & 1U));\
	} \
}

static inline void PRINT_BITS_UINT64(uint64_t x){
	int32_t counter;
	for (counter = 63; counter > -1; --counter){ \
		printf("%d", (int32_t)(( x >> counter ) & 1U));\
	} \
}

//Macro stuff.
//Rotating stuff.
#define ROTATE_32RIGHT_UINT64( __value ) ( (__value >> 32) | (__value << 32) )
#define ROTATE_16RIGHT_UINT32( __value ) ( (__value >> 16) | (__value << 16) )

//REDEFINES DEPENDING ON MADE SELECTION
#ifdef COMPILEASLIBRARY
#define SILENT
#undef COLLECTSTATS
#undef SCORING
#endif


#ifdef SILENT
#undef VERYVERBOSE

#undef VERBOSE
#undef VERBOSE_RESET
#undef VERBOSE_PARAMS
#undef VERBOSE_GUIDANCE
#undef VERBOSE_STARTUP
#undef VERBOSE_CLASSIFY
#undef VERBOSE_CLASSIFY_ATTVALUE
#undef VERBOSE_CLASSIFY_ATTCLASS
#undef VERBOSE_ADAPT
#undef VERBOSE_PREP
#undef VERBOSE_SEARCH
#undef VERBOSE_INP
#undef VERBOSE_HANDLERESULT
#undef VERBOSE_SHUTDOWN

#undef VERBOSE_DETERM
#undef VERBOSE_SPECCER
#undef VERBOSE_LOADER
#undef VERBOSE_ATTRIBUTOR
#undef VERBOSE_RANDOMFOREST
#undef VERBOSE_ITEADAPTER
#undef VERBOSE_PRE
#undef VERBOSE_SATELITE
#undef VERBOSE_IN
#undef VERBOSE_MP
#undef VERBOSE_SLS
#undef VERBOSE_CDCL
#undef VERBOSE_SOLWRITER
#endif


#ifdef VERYVERBOSE
#define VERBOSE
#define VERBOSE_RESET
#define VERBOSE_PARAMS
#define VERBOSE_GUIDANCE
#define VERBOSE_STARTUP
#define VERBOSE_CLASSIFY
#define VERBOSE_CLASSIFY_ATTVALUE
#define VERBOSE_CLASSIFY_ATTCLASS
#define VERBOSE_ADAPT
#define VERBOSE_PREP
#define VERBOSE_SEARCH
#define VERBOSE_INP
#define VERBOSE_HANDLERESULT
#define VERBOSE_SHUTDOWN

#define VERBOSE_DETERM
#define VERBOSE_SPECCER
#define VERBOSE_LOADER
#define VERBOSE_ATTRIBUTOR
#define VERBOSE_RANDOMFOREST
#define VERBOSE_ITEADAPTER
#define VERBOSE_PRE
#define VERBOSE_SATELITE
#define VERBOSE_IN
#define VERBOSE_MP
#define VERBOSE_SLS
#define VERBOSE_CDCL
#define VERBOSE_SOLWRITER
#endif

#endif /* DEFINES_H_ */
