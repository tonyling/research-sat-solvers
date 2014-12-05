/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef G_PMPSLS_H_
#define G_PMPSLS_H_

#include "../guidance.h"

void guide_pmpsls_printHelp();
void guide_pmpsls_apply(int, char**);
void guide_pmpsls_enforceAfterAdaptation();

#endif /* G_PMPSLS_H_ */
