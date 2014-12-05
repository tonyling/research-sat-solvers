/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CAR_ALLDEC_H_
#define CAR_ALLDEC_H_

#include "conflictAnalysisRules.h"

void cdcl_conflictAnalysisRule_alldec_printHelp();
void cdcl_conflictAnalysisRule_alldec_prepare();
uint32_t cdcl_conflictAnalysisRule_alldec_analyze();

#endif /* CAR_ALLDEC_H_ */
