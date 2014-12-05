/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SHUTDOWN_H_
#define SHUTDOWN_H_

#include "../dimetheus.h"

uint32_t shutdown_returnCode;

void shutdown_extern_shutdown();

void shutdown_resetPhase();
void shutdown_initPhase();
void shutdown_disposePhase();

#endif /* SHUTDOWN_H_ */
