/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_SIMPLE_H_
#define PS_SIMPLE_H_

#include "prepStrategies.h"

void prep_strategy_simple_printHelp();

void prep_strategy_simple_reset();			//Reset everything for this strategy.
void prep_strategy_simple_init();			//Initialize everything for this strategy.
void prep_strategy_simple_dispose();		//Dispose everything for this strategy.

void prep_strategy_simple_execute();		//Execute the preprocessing.

#endif /* PS_SIMPLE_H_ */
