/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IR_TESTING_H_
#define IR_TESTING_H_

#include "inprocessingRules.h"

void cdcl_inprocessingRule_testing_printHelp();
void cdcl_inprocessingRule_testing_prepare();
cdclClause* cdcl_inprocessingRule_testing_inprocess();

#endif /* IR_TESTING_H_ */
