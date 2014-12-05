/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_TESTING_H_
#define IS_TESTING_H_

#include "inpStrategies.h"

void inp_strategy_testing_printHelp();	//Show strategy help.

void inp_strategy_testing_reset();		//Reset everything for this strategy.
void inp_strategy_testing_init();		//Initialize everything for this strategy.
void inp_strategy_testing_dispose();	//Dispose everything for this strategy.

void inp_strategy_testing_execute();	//Execute the strategy.

#endif /* IS_TESTING_H_ */
