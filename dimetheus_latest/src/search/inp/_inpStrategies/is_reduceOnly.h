/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_REDUCEONLY_H_
#define IS_REDUCEONLY_H_

#include "inpStrategies.h"

void inp_strategy_reduceOnly_printHelp();	//Show strategy help.

void inp_strategy_reduceOnly_reset();		//Reset everything for this strategy.
void inp_strategy_reduceOnly_init();		//Initialize everything for this strategy.
void inp_strategy_reduceOnly_dispose();		//Dispose everything for this strategy.

void inp_strategy_reduceOnly_execute();		//Execute the strategy.

#endif /* IS_REDUCEONLY_H_ */
