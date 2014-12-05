/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IR_NULL_H_
#define IR_NULL_H_

#include "inprocessingRules.h"

void cdcl_inprocessingRule_null_printHelp();
void cdcl_inprocessingRule_null_prepare();
cdclClause* cdcl_inprocessingRule_null_inprocess();

#endif /* IR_NULL_H_ */
