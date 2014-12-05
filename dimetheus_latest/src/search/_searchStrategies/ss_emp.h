/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef SS_EMP_H_
#define SS_EMP_H_

#include "searchStrategies.h"

float_ty search_strategy_emp_weightModSAT;						//The amount of weight decrease for MPWRL.
float_ty search_strategy_emp_weightModUNS;						//The amount of weight increase for MPWRL.

void search_strategy_emp_printHelp();							//Show strategy help.

void search_strategy_emp_reset();								//Reset everything for this strategy.
void search_strategy_emp_init();								//Initialize everything for this strategy.
void search_strategy_emp_dispose();								//Dispose everything for this strategy.

void search_strategy_emp_updateMPWeightsSLS();					//Update the weights based on the SLS assignment.

void search_strategy_emp_execute();								//Execute the search.

#endif /* SS_EMP_H_ */
