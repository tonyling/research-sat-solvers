/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef HANDLERESULT_H_
#define HANDLERESULT_H_

#include "../dimetheus.h"

uint32_t handleresult_returnCode;
eqVar* handleresult_eqVars;			//The variables together with equivalence classes.

//MODULES of the HANDLERESULT phase.
#include "SOLWRITER/solwriter.h"

void handleresult_extern_handle();

void handleresult_resetPhase();
void handleresult_initPhase();
void handleresult_disposePhase();

#endif /* HANDLERESULT_H_ */
