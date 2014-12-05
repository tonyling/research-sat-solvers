/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef RESET_H_
#define RESET_H_

#include "../dimetheus.h"

uint32_t reset_returnCode;

void reset_extern_resetAll();

void reset_resetPhase();
void reset_initPhase();
void reset_disposePhase();

#endif /* RESET_H_ */
