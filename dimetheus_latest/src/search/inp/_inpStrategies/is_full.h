/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef IS_FULL_H_
#define IS_FULL_H_

#include "inpStrategies.h"

void inp_strategy_full_printHelp();	//Show strategy help.

void inp_strategy_full_reset();		//Reset everything for this strategy.
void inp_strategy_full_init();		//Initialize everything for this strategy.
void inp_strategy_full_dispose();		//Dispose everything for this strategy.

void inp_strategy_full_execute();		//Execute the strategy.

#endif /* IS_FULL_H_ */
