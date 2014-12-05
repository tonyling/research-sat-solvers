/*	DIMETHEUS SAT SOLVER
 * 	Author:		Oliver Gableske	(oliver@gableske.net)
 *	Website:	http://www.gableske.net/dimetheus
 *  License:	See ./doc/license.txt
 */

#ifndef PS_GIGRID_H_
#define PS_GIGRID_H_

#include "prepStrategies.h"

void prep_strategy_gigrid_printHelp();

void prep_strategy_gigrid_reset();			//Reset everything for this strategy.
void prep_strategy_gigrid_init();			//Initialize everything for this strategy.
void prep_strategy_gigrid_dispose();		//Dispose everything for this strategy.

void prep_strategy_gigrid_execute();		//Execute the preprocessing.

#endif /* PS_GIGRID_H_ */
