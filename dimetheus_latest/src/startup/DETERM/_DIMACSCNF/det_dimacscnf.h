/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef DET_DIMACSCNF_H_
#define DET_DIMACSCNF_H_

#include "../determ.h"

uint32_t determ_determineInputFormat_dimacscnf();

void determ_determineInputFormat_dimacscnf_resetPlugin();
void determ_determineInputFormat_dimacscnf_initPlugin();
void determ_determineInputFormat_dimacscnf_disposePlugin();

#endif /* DET_DIMACSCNF_H_ */
