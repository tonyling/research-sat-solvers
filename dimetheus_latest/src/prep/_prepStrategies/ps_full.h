/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_FULL_H_
#define PS_FULL_H_

#include "prepStrategies.h"

void prep_strategy_full_printHelp();	//Print the strategy help.

void prep_strategy_full_reset();		//Reset everything for this strategy.
void prep_strategy_full_init();			//Initialize everything for this strategy.
void prep_strategy_full_dispose();		//Dispose everything for this strategy.

void prep_strategy_full_execute();		//Execute the preprocessing strategy.
#endif /* PS_FULL_H_ */
