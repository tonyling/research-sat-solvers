/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SEARCH_H_
#define SEARCH_H_

#include "../dimetheus.h"

//MODULES of the search phase.
#include "inp/inp.h"
#include "MP/mp.h"
#include "SLS/sls.h"
#include "CDCL/cdcl.h"

#include "_searchStrategies/searchStrategies.h"

//Global variables of the search phase.
int32_t search_returnCode;				//The return code of the search phase.

//The following macros are used to interpret the variable data. Computing the score from a variable activity understood as
//MP bias simply means to multiply it with a large float such that the comma position moves right.
#define GET_VAR_Q_SCORE_FROM_VAR_ACTIVITY( __vPtr ) ( VQ_SCORE_FACTOR * GET_VAR_ACTIVITY(__vPtr) )

#define GET_VAR_Q_SCORE_FROM_VAR_ACTIVITY_NEGATIVE( __vPtr ) ( -VQ_SCORE_FACTOR * GET_VAR_ACTIVITY(__vPtr) )

//Methods provided exclusively to the phase (INTERN).
void search_intern_transferSLSAss();		//For all variables not globally assigned, we use the SLS assignment.
void search_intern_transferCDCLAss();		//For all variables not globally assigned, we use the CDCL assignment.

//Method PROVIDED by this phase (EXTERN).
void search_extern_searching();	//To execute the search.

//Methods to initialize this phase (called by the component that wants to call the above method).
void search_resetPhase();		//To reset the search phase.
void search_initPhase();		//To initialize it.
void search_disposePhase();		//To dispose it.

#endif /* SEARCH_H_ */
