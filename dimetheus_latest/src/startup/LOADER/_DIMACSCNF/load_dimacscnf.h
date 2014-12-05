/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef LOAD_DIMACSCNF_H_
#define LOAD_DIMACSCNF_H_

#include "../loader.h"

void loader_loadClauses_dimacscnf();

void loader_loadClauses_dimacscnf_resetPlugin();
void loader_loadClauses_dimacscnf_initPlugin();
void loader_loadClauses_dimacscnf_disposePlugin();

#endif /* LOAD_DIMACSCNF_H_ */
