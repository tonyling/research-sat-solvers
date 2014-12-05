/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_STRONG_H_
#define PS_STRONG_H_

#include "prepStrategies.h"

void prep_strategy_strong_printHelp();	//Print the strategy help.

void prep_strategy_strong_reset();		//Reset everything for this strategy.
void prep_strategy_strong_init();		//Initialize everything for this strategy.
void prep_strategy_strong_dispose();	//Dispose everything for this strategy.

void prep_strategy_strong_execute();	//Execute the preprocessing strategy.

#endif /* PS_STRONG_H_ */
