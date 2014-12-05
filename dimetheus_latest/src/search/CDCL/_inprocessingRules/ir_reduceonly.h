/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IR_REDUCEONLY_H_
#define IR_REDUCEONLY_H_

#include "inprocessingRules.h"

void cdcl_inprocessingRule_reduceonly_printHelp();
void cdcl_inprocessingRule_reduceonly_prepare();
cdclClause* cdcl_inprocessingRule_reduceonly_inprocess();

#endif /* IR_REDUCEONLY_H_ */
