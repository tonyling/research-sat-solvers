/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef OR_APLEFOO_H_
#define OR_APLEFOO_H_

#include "../solwriter.h"

void solwriter_aplefoo_printUNKNOWN();
void solwriter_aplefoo_printSAT();
void solwriter_aplefoo_printUNSAT();

void solwriter_aplefoo_resetPlugin();
void solwriter_aplefoo_initPlugin();
void solwriter_aplefoo_disposePlugin();

#endif /* OR_APLEFOO_H_ */
