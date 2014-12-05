/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IR_FULL_H_
#define IR_FULL_H_

#include "inprocessingRules.h"

void cdcl_inprocessingRule_full_printHelp();
void cdcl_inprocessingRule_full_prepare();
cdclClause* cdcl_inprocessingRule_full_inprocess();

#endif /* IR_FULL_H_ */
