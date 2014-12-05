/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef ADAPT_H_
#define ADAPT_H_

#include "../dimetheus.h"
#include "_adaptStrategies/adaptStrategies.h"

//MODULES for the adaptation phase.
#include "ITEADAPTER/iteadapter.h"

//Global variables of the adaptation phase.
int32_t adapt_returnCode;			//The return code of the adaptation phase.

//Methods provided exclusively to this phase (INTERN).

//Methods PROVIDED by this phase (EXTERN).
void adapt_extern_adaptation();		//Executes the adaptation for the current formula.


//Methods to initialize this phase (are called by the one who wants to execute the above functions).
void adapt_resetPhase();			//Reset the classification.
void adapt_initPhase();				//For the classification.
void adapt_disposePhase();			//For freeing all related memory.

#endif /* ADAPT_H_ */
