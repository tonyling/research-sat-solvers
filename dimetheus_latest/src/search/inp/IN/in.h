/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IN_H_
#define IN_H_

#include "../inp.h"

#define IN_VERSION_MA 2
#define IN_VERSION_MI 210
#define IN_VERSION_NAME "IN"

int32_t in_returnCode;					//The return code of the IN module.

//Methods exclusive to this module.
clause* in_intern_fld_testPropagate(literal*); 			//Propagates a literal temporarily.
int32_t in_intern_unhiding_SSet_incDSC(const void*, const void*);	//Comparators for unhiding: increasing DSC.
int32_t in_intern_unhiding_SSet_decDSC(const void*, const void*);	//Comparators for unhiding: decreasing DSC.
void in_intern_unhiding_uhle(clause*);					//Checks for UHLEs for the given clause in unhiding.
uint32_t in_intern_unhiding_uhte(clause*);				//Checks if the given clause is UHTE in unhiding.
uint32_t in_intern_unhiding_stamp(literal*, uint32_t);	//Performs the advanced stamping for unhiding.
clause* in_intern_nhbr_detect(literal*);				//Performs UP with implications prioritized and learns binaries.
clause* in_intern_treelook_detect(literal*);			//Propagates a tree-look decision and might learn binary clauses.
void in_intern_els_replaceLit(literal*, literal*);		//Replaces a literal with another literal as being equivalent.
uint32_t in_intern_els_strongConnect(literal*, uint32_t);//Creates equivalence classes using Tarjan's algorithm.
void in_intern_ge_extract(variable*);					//To extract the given variable along with the current GDEF.
void in_intern_ge_AND(variable*);						//Performs AND GE on the given variable.
void in_intern_ge_OR(variable*);						//Performs OR GE on the given variable.
void in_intern_ge_SIM3(variable*);						//Performs SIM GE on the given variable for gates of size 3.
void in_intern_ge_SIM4(variable*);						//Performs SIM GE on the given variable for gates of size 4.
void in_intern_ge_XOR(variable*);						//Performs XOR GE on the given variable.

//Extern methods.
void in_extern_printVersion();
void in_extern_SE(); 				//Performs subsumption elimination.
void in_extern_STR();				//Performs strengthening.
void in_extern_PLE();				//Performs pure literal elimination.
void in_extern_FLD();				//Performs failed literal detection.
void in_extern_TERNARY();			//Performs ternary resolution.
void in_extern_UNHIDING(uint32_t);	//Performs UNHIDING for the given number of rounds.
void in_extern_NHBR();				//Performs non-transitive hyper binary resolution.
void in_extern_ELS();				//Performs equivalent literal substitution.
void in_extern_TREELOOK();			//Performs tree-based look-ahead.
void in_extern_NIVER();				//Performs non-increasing variable elimination by resolution.
void in_extern_GE();				//Performs gate extraction.

//The following methods are used to initialize his module (called by the one who wants to use the provided methods.)
void in_resetModule();
void in_initModule();
void in_disposeModule();
#endif /* IN_H_ */
