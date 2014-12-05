/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IR_SIMPLE_H_
#define IR_SIMPLE_H_

#include "inprocessingRules.h"

void cdcl_inprocessingRule_simple_printHelp();
void cdcl_inprocessingRule_simple_prepare();
cdclClause* cdcl_inprocessingRule_simple_inprocess();

#endif /* IR_SIMPLE_H_ */
