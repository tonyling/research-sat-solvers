/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_SIMPLE_H_
#define IS_SIMPLE_H_

#include "inpStrategies.h"

void inp_strategy_simple_printHelp();	//Show strategy help.

void inp_strategy_simple_reset();		//Reset everything for this strategy.
void inp_strategy_simple_init();		//Initialize everything for this strategy.
void inp_strategy_simple_dispose();		//Dispose everything for this strategy.

void inp_strategy_simple_execute();		//Execute the strategy.

#endif /* IS_SIMPLE_H_ */
