/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef CAR_FIRSTUIP_H_
#define CAR_FIRSTUIP_H_

#include "conflictAnalysisRules.h"

void cdcl_conflictAnalysisRule_firstuip_printHelp();
void cdcl_conflictAnalysisRule_firstuip_prepare();
uint32_t cdcl_conflictAnalysisRule_firstuip_analyze();

#endif /* CAR_FIRSTUIP_H_ */
