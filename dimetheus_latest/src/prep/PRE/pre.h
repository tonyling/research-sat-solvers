/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PRE_H_
#define PRE_H_

#include "../prep.h"

#define PRE_VERSION_MA 2
#define PRE_VERSION_MI 210
#define PRE_VERSION_NAME "PRE"

//Global variables and Macros of the PRE module.
int32_t pre_returnCode;			//The return code of the PRE module.

int32_t** pre_res;				//The set of resolvents, mostly used by NIVER.
int32_t pre_resAvail, pre_resUsed;//How many such resolvents have been initialized? How many are already in use?
int32_t* pre_resMemAvail, *pre_resMemUsed;//How much memory is allocated for a specific resolvent? How much is in use (size)?

coveredLit *pre_covLits;		//The set of covered literals computed by CLA. The available is always f.n_vars_e.
int32_t pre_covLitsUsed;		//The number of covered literals current used.

clause** pre_G_negV;			//During gate extraction, this is the set of gate defining clauses containing literal +v.
int32_t pre_G_negVAvail;		//The amount of memory positions allocated in G_negV.
int32_t pre_G_negVUsed;			//The amount of memory used.
clause** pre_G_posV;			//During gate extraction, this is the set of gate defining clauses containing literal -v.
int32_t pre_G_posVAvail;		//The amount of memory positions allocated in G_posV.
int32_t pre_G_posVUsed;			//The amount of memory used.

literal** pre_litPerm;			//An array that can be used to store literal permutations - unchanged by variable disabling.

literal** pre_UNHIDING_lStack;	//The literal stack used by UNHIDING.
literal** pre_UNHIDING_lStack_current;//The current position in the UNHIDING stack.
literal** pre_UNHIDING_SPlus;	//The SPlus set contains the literals of a clause as pointers.
literal** pre_UNHIDING_SMinus;	//The SMinus set contains the opposite literals of a clause as pointers.

literal*** pre_ELS_classes;		//Equivalence classes of literals found during ELS.
uint32_t pre_ELS_classes_avail;	//The number of ELS classes that have been initialized.
uint32_t pre_ELS_classes_used;	//The current number of ELS classes used.
uint32_t* pre_ELS_classes_memAvail;//The size of each used class.
uint32_t* pre_ELS_classes_memUsed;//The amount of elements for each class.
literal** pre_ELS_lStack;		//The literal stack used by Equivalent Literal Substitution.
literal** pre_ELS_lStack_current;//The literal stack current position for a new element.

//The following macro is used to reset the resolvents for a new cycle of NIVER.
#define PRE_RESET_ALL_RESOLVENTS()	( pre_resUsed = 0 )

//The following macro is used to reset a specific resolvent before using it anew for resolution.
#define PRE_RESET_RESOLVENT( __resNum ) {\
		pre_resMemUsed[ __resNum ] = 0;\
}

//The following macro enlarges the amount of ELS classes available, if necessary.
#define PRE_ENLARGE_ELS_CLASSES_IF_NECESSARY( __counter ) {\
		if (pre_ELS_classes_used == pre_ELS_classes_avail){\
			__counter = pre_ELS_classes_avail;\
			pre_ELS_classes_avail += BLOCKSIZE;\
			pre_ELS_classes 			= realloc(pre_ELS_classes, sizeof(literal**) * pre_ELS_classes_avail);\
			pre_ELS_classes_memUsed 	= realloc(pre_ELS_classes_memUsed, sizeof(uint32_t) * pre_ELS_classes_avail);\
			pre_ELS_classes_memAvail 	= realloc(pre_ELS_classes_memAvail, sizeof(uint32_t) * pre_ELS_classes_avail);\
			while (__counter < pre_ELS_classes_avail){\
				pre_ELS_classes[__counter] = malloc(sizeof(literal*) * BLOCKSIZE);\
				pre_ELS_classes_memAvail[__counter] = BLOCKSIZE;\
				pre_ELS_classes_memUsed[__counter] = 0;\
				++__counter;\
			}\
		}\
}

//The following macro adds a literal to the current equivalence class and enlarges its size, if necessary.
#define PRE_ADD_LIT_TO_CURRENT_ELS_CLASS(__l) {\
		pre_ELS_classes[pre_ELS_classes_used][pre_ELS_classes_memUsed[pre_ELS_classes_used]++] = __l;\
		if (pre_ELS_classes_memUsed[pre_ELS_classes_used] == pre_ELS_classes_memAvail[pre_ELS_classes_used]){\
			pre_ELS_classes_memAvail[pre_ELS_classes_used] *= 2;\
			pre_ELS_classes[pre_ELS_classes_used] = realloc(pre_ELS_classes[pre_ELS_classes_used], \
								sizeof(literal*) * pre_ELS_classes_memAvail[pre_ELS_classes_used]);\
		}\
}

//The following macro is used to add another literal to the current resolvent. It increases the resolvent if necessary.
#define PRE_ADD_LIT_TO_CURRENT_RESOLVENT( __lit ) { \
		pre_res[pre_resUsed][pre_resMemUsed[pre_resUsed]++] = __lit; \
		if (pre_resMemUsed[pre_resUsed] == pre_resMemAvail[pre_resUsed]) {\
			pre_resMemAvail[pre_resUsed] *= 2; \
			pre_res[pre_resUsed] = realloc(pre_res[pre_resUsed], sizeof(int32_t) * pre_resMemAvail[pre_resUsed]); \
		} \
}

//The following macro is used every time a new resolvent was generated. The __counter is just a for-loop variable. We must
//increase with a factor of 2 here otherwise we must rewrite the for loop initialization.
#define PRE_INCREASE_NUM_RESOLVENTS( __counter ) {\
		if (++pre_resUsed == pre_resAvail) {\
			pre_resAvail *= 2; \
			pre_resMemAvail = realloc(pre_resMemAvail, sizeof(int32_t)*pre_resAvail); \
			pre_resMemUsed = realloc(pre_resMemUsed, sizeof(int32_t)*pre_resAvail); \
			pre_res = realloc(pre_res, sizeof(int32_t*)*pre_resAvail);\
			for (__counter = pre_resAvail>>1; __counter < pre_resAvail; ++__counter){\
				pre_res[ __counter ] = malloc(sizeof(int32_t)*BLOCKSIZE); \
				pre_resMemAvail[ __counter ] = BLOCKSIZE; \
				pre_resMemUsed[ __counter ] = 0; \
			}\
		} \
}

//The following macro is used to print a resolvent given its number and a counter.
#define PRE_PRINT_RESOLVENT( __rNum , __counter) { \
	printf("c RES "); \
	for (__counter = 0; __counter < pre_resMemUsed[__rNum]; ++__counter){ \
		printf("%d ", pre_res[__rNum][__counter]); \
	} \
	printf("\n"); \
}

//The following macro is used to add a clause to the set G_posV. It increases the set if necessary. The set is never reduced.
#define PRE_ADD_CLS_TO_GPOSV( __cPtr ) { \
	pre_G_posV[ pre_G_posVUsed++ ] = __cPtr; \
	if ( pre_G_posVUsed == pre_G_posVAvail ) { \
		pre_G_posVAvail *= 2; \
		pre_G_posV = realloc(pre_G_posV, sizeof(clause*)*pre_G_posVAvail); \
	} \
}

//The following macro is used to add a clause to the set G_negV. It increases the set if necessary. The set is never reduced.
#define PRE_ADD_CLS_TO_GNEGV( __cPtr ) { \
		pre_G_negV[ pre_G_negVUsed++ ] = __cPtr; \
	if ( pre_G_negVUsed == pre_G_negVAvail ) { \
		pre_G_negVAvail *= 2; \
		pre_G_negV = realloc(pre_G_negV, sizeof(clause*)*pre_G_negVAvail); \
	} \
}

//The following macros are used to determine if G_negV or G_posV are still empty.
#define PRE_IS_GNEGV_EMPTY() (!pre_G_negVUsed)
#define PRE_IS_GPOSV_EMPTY() (!pre_G_posVUsed)

//The following macro is used to reset the gate definition arrays.
#define PRE_RESET_GDEFARRAYS() pre_G_negVUsed = 0; pre_G_posVUsed = 0;

//Methods exclusively accessible to the PRE module (INTERN).
int32_t pre_intern_ACCE_cla();			//Performs covered literal addition assuming that the clause is given in the UP queue.
void pre_intern_GE_extract(variable*);	//To eliminate a variable once a definition is found.
void pre_intern_GE_AND(variable*);		//To check if a variable has an AND gate definition.
void pre_intern_GE_OR(variable*);		//To check if a variable has an OR gate definition.
void pre_intern_GE_XOR(variable*);		//To check if a variable has an XOR gate definition.
void pre_intern_GE_SIM3(variable *);	//To check if a variable has an SIM gate definition of size 3.
void pre_intern_GE_SIM4(variable *);	//To check if a variable has an SIM gate definition of size 4.

int32_t pre_intern_UNHIDING_SSet_incDSC(const void*, const void*);//Sorts the literals with increasing discovery time.
int32_t pre_intern_UNHIDING_SSet_decDSC(const void*, const void*);//Sorts the literals with increasing discovery time.

void pre_intern_UNHIDING_uhle(clause*); //This performs an UHLE check on the given clause (which must be disabled).
uint32_t pre_intern_UNHIDING_uhte(clause*);	//This performs an UHTE check on the given clause (which must be disabled).
uint32_t pre_intern_UNHIDING_stamp(literal*,uint32_t);	//The implementation of the advanced stamping for UNHIDING.

clause* pre_intern_NHBR_detect(int32_t);//Performs unit propagation prioritizing binary clauses. Returns NULL if no
										//conflict arises and adds new binary clauses to the formula if it detects them.

void pre_intern_ELS_replaceLit(literal*, literal*);//Replaces all occurrences of the first literal with the second.
uint32_t pre_intern_ELS_strongConnect(literal*, uint32_t);//Creates the strongly connected components.

//Methods PROVIDED by the module (EXTERN).
void pre_extern_printVersion();			//Prints the version of this module.
void pre_extern_SE();					//Subsumption elimination.
void pre_extern_PLE();					//Pure literal elimination.
void pre_extern_FLD();					//Double-look Failed literal detection.
void pre_extern_SLFLD();				//Single-look Failed literal detection.
void pre_extern_STR();					//Strengthening.
void pre_extern_NIVER();				//Non Increasing Variable Elimination by Resolution.
void pre_extern_ATE();					//Asymmetric tautology elimination.
void pre_extern_ACCE();					//Asymmetric covered clause elimination.
void pre_extern_GE();					//Gate extraction.
void pre_extern_UNHIDING(uint32_t);		//Performs advanced unhiding simplifications for the given number of rounds.
void pre_extern_NHBR();					//Performs non-transitive hyper binary resolution using unit propagation.
void pre_extern_TERNARY();				//Performs ternary resolution.
void pre_extern_ELS();					//Performs equivalent literal substitution.

//The following methods are used to initialize this module (called by the one who wants to use the external methods).
void pre_resetModule();
void pre_initModule();
void pre_disposeModule();

#endif /* PRE_H_ */
