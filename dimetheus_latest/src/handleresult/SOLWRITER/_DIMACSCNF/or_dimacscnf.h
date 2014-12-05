/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef OR_DIMACSCNF_H_
#define OR_DIMACSCNF_H_

#include "../solwriter.h"

void solwriter_dimacscnf_printUNKNOWN();
void solwriter_dimacscnf_printSAT();
void solwriter_dimacscnf_printUNSAT();

void solwriter_dimacscnf_resetPlugin();
void solwriter_dimacscnf_initPlugin();
void solwriter_dimacscnf_disposePlugin();

#endif /* OR_DIMACSCNF_H_ */
