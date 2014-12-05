/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SPEC_APLEFOO_H_
#define SPEC_APLEFOO_H_

#include "../speccer.h"

#ifdef COMPILEASLIBRARY
uint32_t speccer_loadSpecs_aplefoo_currentCharNum;
#endif

void speccer_loadSpecs_aplefoo();

void speccer_loadSpecs_aplefoo_resetPlugin();
void speccer_loadSpecs_aplefoo_initPlugin();
void speccer_loadSpecs_aplefoo_disposePlugin();

#endif /* SPEC_APLEFOO_H_ */
